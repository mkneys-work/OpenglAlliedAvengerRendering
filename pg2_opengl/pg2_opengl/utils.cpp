#include "pch.h"
#include "utils.h"
#include <iterator>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <limits>

std::streamsize GetFileSize64(const char * file_name)
{
	std::ifstream file;
	file.open(file_name, std::ios::in | std::ios::binary);
	file.ignore(std::numeric_limits<std::streamsize>::max());
	std::streamsize length = file.gcount();
	file.clear();
	file.seekg(0, std::ios_base::beg);
	file.close();
	return length;
}

char * lTrim( char * s )
{
    while ( isspace( *s ) || ( *s == 0 ) )
	{
		++s;
	};

    return s;
}

char * rTrim( char * s )
{
    char * back = s + strlen( s );

    while ( isspace( *--back ) );

    *( back + 1 ) = '\0';

    return s;
}

char * trim( char * s )
{
	return rTrim( lTrim( s ) );
}

std::string& rtrim(std::string& s, const char* t)
{
	s.erase(s.find_last_not_of(t) + 1);
	return s;
}

std::string& ltrim(std::string& s, const char* t)
{
	s.erase(0, s.find_first_not_of(t));
	return s;
}

std::string& trim(std::string& s, const char* t)
{
	return ltrim(rtrim(s, t), t);
}

std::vector<std::string> splitWords(const std::string &s) {
	std::string buf;
	std::stringstream ss(s); 

	std::vector<std::string> tokens;

	while (ss >> buf)
		tokens.push_back(buf);
	return tokens;
}

void glfw_callback(const int error, const char * description)
{
	printf("GLFW Error (%d): %s\n", error, description);
}

void GLAPIENTRY gl_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void * user_param)
{
	printf("GL %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "Error" : "Message"),
		type, severity, message);
}

void framebuffer_resize_callback(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

char * LoadShader(const char * file_name)
{
	FILE * file = fopen(file_name, "rt");

	if (file == NULL)
	{
		printf("IO error: File '%s' not found.\n", file_name);

		return NULL;
	}

	size_t file_size = static_cast<size_t>(GetFileSize64(file_name));
	char * shader = NULL;

	if (file_size < 1)
	{
		printf("Shader error: File '%s' is empty.\n", file_name);
	}
	else
	{
		shader = new char[file_size + 1];
		memset(shader, 0, sizeof(*shader) * (file_size + 1));

		size_t bytes = 0;

		do
		{
			bytes += fread(shader, sizeof(char), file_size, file);
		} while (!feof(file) && (bytes < file_size));

		if (!feof(file) && (bytes != file_size))
		{
			printf("IO error: Unexpected end of file '%s' encountered.\n", file_name);
		}
	}

	fclose(file);
	file = NULL;

	return shader;
}

GLint CheckShader(const GLenum shader)
{
	GLint status = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	printf("Shader compilation %s.\n", (status == GL_TRUE) ? "was successful" : "FAILED");

	if (status == GL_FALSE)
	{
		int info_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_length);
		char * info_log = new char[info_length];
		memset(info_log, 0, sizeof(*info_log) * info_length);
		glGetShaderInfoLog(shader, info_length, &info_length, info_log);

		printf("Error log: %s\n", info_log);

		SAFE_DELETE_ARRAY(info_log);
	}

	return status;
}
