#include "pch.h"
#include "Rasterizer.h"
#include "utils.h"
#include "matrix4x4.h"
#include "glutils.h"

Rasterizer::Rasterizer(const int width, const int height, const float fovY, const Vector3 viewFrom, const Vector3 viewAt)
{
	_camera = Camera(width, height, fovY, viewFrom, viewAt);
}


Rasterizer::~Rasterizer()
{
}

void Rasterizer::initMaterials() {

	GLMaterial* glMaterials = new GLMaterial[_materials.size()];
	int m = 0;
	for (const auto & material : _materials) {
		Texture * tex_diffuse = material->getTexture(Material::TextureMapSlotNumber::DIFFUSE);
		if (tex_diffuse) {
			GLuint id = 0;
			CreateBindlessTexture(id, glMaterials[m].tex_diffuse_handle, tex_diffuse->getWidth(), tex_diffuse->getHeight(), tex_diffuse->getData());
			glMaterials[m].diffuse = Color3f{ 1.0f, 1.0f, 1.0f };
		}
		else {
			GLuint id = 0;
			GLubyte data[] = { 255, 255, 255, 255 };
			CreateBindlessTexture(id, glMaterials[m].tex_diffuse_handle, 1, 1, data);
			glMaterials[m].diffuse = material->getDiffuseColor();
		}		
		glMaterials[m].specular = material->getSpecularColor();
		glMaterials[m].ambient = material->getAmbientColor();
		glMaterials[m].shininess = material->getShininess();
		
		m++;
	}
	_ssboMaterials = 0;
	glGenBuffers(1, &_ssboMaterials);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssboMaterials);
	const GLsizeiptr gl_materials_size = sizeof(GLMaterial) * _materials.size();
	glBufferData(GL_SHADER_STORAGE_BUFFER, gl_materials_size, glMaterials, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _ssboMaterials);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	SAFE_DELETE_ARRAY(glMaterials);
}

int Rasterizer::initFrameBuffer() {
	int msaaSamples = 0;
	glGetIntegerv(GL_SAMPLES, &msaaSamples);

	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	
	// position renderbuffer.
	glGenRenderbuffers(1, &_rboPosition);
	glBindRenderbuffer(GL_RENDERBUFFER, _rboPosition);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, _camera.width , _camera.height);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaaSamples, GL_RGBA32F, _camera.width, _camera.height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _rboPosition);

	// Color renderbuffer.
	glGenRenderbuffers(1, &_rboColor);
	glBindRenderbuffer(GL_RENDERBUFFER, _rboColor);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, _camera.width, _camera.height);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaaSamples, GL_SRGB8_ALPHA8, _camera.width, _camera.height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_RENDERBUFFER, _rboColor);
	
	// Depth renderbuffer
	glGenRenderbuffers(1, &_rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, _rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _camera.width, _camera.height);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaaSamples, GL_DEPTH_COMPONENT24, _camera.width, _camera.height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _rboDepth);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) return -1;
	return S_OK;
}

void Rasterizer::loadScene(const std::string file_name) {
	_scene = new Scene("../../data/", "6887_allied_avenger_gi.obj", "../../data/");
	_surfaces = _scene->getSurfaces();
	_materials = _scene->getMaterials();
	_trianglesCount = 0;

	for (auto surface : _surfaces)
	{
		_trianglesCount += surface->getTrianglesCount();
	}

	initBuffers();
}

int Rasterizer::initDevice() {
	glfwSetErrorCallback(glfw_callback);

	if (!glfwInit())
	{
		return(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

	_window = glfwCreateWindow(_camera.width, _camera.height, "Alied avenger rendering", nullptr, nullptr);
	if (!_window)
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwSetFramebufferSizeCallback(_window, framebuffer_resize_callback);
	glfwMakeContextCurrent(_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		if (!gladLoadGL())
		{
			return EXIT_FAILURE;
		}
	}

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(gl_callback, nullptr);

	printf("OpenGL %s, ", glGetString(GL_VERSION));
	printf("%s", glGetString(GL_RENDERER));
	printf(" (%s)\n", glGetString(GL_VENDOR));
	printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glViewport(0, 0, _camera.width, _camera.height);
	glClipControl(GL_LOWER_LEFT, GL_NEGATIVE_ONE_TO_ONE);
	
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	const char * vertex_shader_source = LoadShader("basic_shader.vert");
	glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
	glCompileShader(vertex_shader);
	SAFE_DELETE_ARRAY(vertex_shader_source);
	CheckShader(vertex_shader);

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	const char * fragment_shader_source = LoadShader("basic_shader.frag");
	glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
	glCompileShader(fragment_shader);
	SAFE_DELETE_ARRAY(fragment_shader_source);

	CheckShader(fragment_shader);

	_shaderProgram = glCreateProgram();
	glAttachShader(_shaderProgram, vertex_shader);
	glAttachShader(_shaderProgram, fragment_shader);
	glLinkProgram(_shaderProgram);
	glUseProgram(_shaderProgram);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return S_OK;
}

int Rasterizer::initBuffers() {
	const int no_vertices = _trianglesCount * 3;
	Vertex* vertices = new Vertex[no_vertices];

	const int vertex_stride = sizeof(Vertex);

	int k = 0;
	for (auto surface : _surfaces)
	{
		for (int i = 0; i < surface->getTrianglesCount(); ++i)
		{
			Triangle & triangle = surface->getTriangle(i);
		
			for (int j = 0; j < 3; ++j, ++k)
			{
				vertices[k] = triangle.getVertex(j);
				vertices[k].materialIndex = surface->getMaterial()->getMaterialIndex();
			}

		}

	}

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo); 
	glBufferData(GL_ARRAY_BUFFER, no_vertices * sizeof(Vertex), vertices, GL_STATIC_DRAW); 
																		
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertex_stride, (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertex_stride, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, vertex_stride, (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, vertex_stride, (void*)(11 * sizeof(float)));
	glEnableVertexAttribArray(4);

	glVertexAttribIPointer(5, 1, GL_INT, vertex_stride, (void*) (14 * sizeof(float)));
	glEnableVertexAttribArray(5);

	delete[] vertices;
	return S_OK;
}

int Rasterizer::realeaseDevice() {
	glDeleteShader(_vertexShader);
	glDeleteShader(_fragmentShader);
	glDeleteProgram(_shaderProgram);
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);

	glfwTerminate();
	return S_OK;
}


int Rasterizer::RenderFrame() {
	glBindVertexArray(_vao);
	while (!glfwWindowShouldClose(_window))
	{
		glUseProgram(_shaderProgram);
		glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		Vector3 lightPoss = Vector3(50, 0, 120);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		Matrix4x4 model;
		
		Matrix4x4 mvp = _camera.projectionMatrix * _camera.viewMatrix * model;
		Matrix4x4 mvn = model * _camera.viewMatrix;

		SetMatrix4x4(_shaderProgram, mvp.getData(), "MVP");
		SetMatrix4x4(_shaderProgram, mvn.getData(), "MVN");
		
		const GLint possLocation = glGetUniformLocation(_shaderProgram, "lightPossition");
		glUniform3f(possLocation ,lightPoss.x, lightPoss.y, lightPoss.z);

		const GLint viewFrom = glGetUniformLocation(_shaderProgram, "viewFrom");
		glUniform3f(viewFrom, _camera.getViewFrom().x, _camera.getViewFrom().y, _camera.getViewFrom().z);

		glDrawArrays(GL_TRIANGLES, 0, _trianglesCount * 3);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, _fbo);
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glDrawBuffer(GL_BACK_LEFT);
		glBlitFramebuffer(0, 0, _camera.width, _camera.height, 0, 0, _camera.width, _camera.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glfwSwapBuffers(_window);
		glfwSwapInterval(1);
		glfwPollEvents();
	}

	realeaseDevice();
	return S_OK;
}
