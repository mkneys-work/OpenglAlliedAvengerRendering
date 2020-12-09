#ifndef RASTERIZER_H
#define RASTERIZER_H

#pragma once

#include "surface.h"
#include "camera.h"
#include "Scene.h"

class Rasterizer
{
public:
	Rasterizer(const int width, const int height, const float fov_y, const Vector3 view_from, const Vector3 view_at);
	~Rasterizer();


	int initDevice();
	int realeaseDevice();
	void loadScene(const std::string file_name);
	void initMaterials();
	int initFrameBuffer();
	int initBuffers();
	int RenderFrame();

private:
	GLuint _fragmentShader{ 0 };
	GLuint _shaderProgram{ 0 };
	GLuint _vertexShader{ 0 };
	GLuint _ssboMaterials{ 0 };
	GLuint _vao{ 0 };
	GLuint _vbo{ 0 };
	GLuint _fbo{ 0 };
	GLuint _rboColor { 0 };
	GLuint _rboPosition { 0 };
	GLuint _rboDepth { 0 };
	GLFWwindow* _window;
	int _trianglesCount;
	Scene* _scene;


	Camera _camera;
	std::vector<Surface *> _surfaces;
	std::vector<Material *> _materials;

#pragma pack( push, 1 ) // 1 B alignment
	struct GLMaterial
	{
		Color3f diffuse; // 3 * 4B
		GLbyte pad0[4];
		Color3f specular;
		GLbyte pad1[4];
		Color3f ambient;
		GLint shininess;
		GLuint64 tex_diffuse_handle{ 0 }; // 1 * 8 B
		GLbyte pad3[8];
	};
#pragma pack( pop )
};

#endif