#ifndef UTILS_H_
#define UTILS_H_

#define MAT_ELEM( mat, type, x, y ) reinterpret_cast<type *>( ( mat ).data + \
	( mat ).step * ( y ) + ( mat ).elemSize() * ( x ) )

#define SAFE_DELETE( p ) { \
	if ( p != NULL ) \
	{ \
		delete p; \
		p = NULL; \
	} \
}

#define SAFE_DELETE_ARRAY( p ) { \
	if ( p != NULL ) \
	{ \
		delete [] p; \
		p = NULL; \
	} \
}

template<typename T> void SafeDeleteVectorItems( std::vector<T> v )
{
	while ( v.size() > 0 )
	{
		T item = v.back();
		v.pop_back();
		SAFE_DELETE( item );
	}
}

namespace utils
{
	template<typename T> void swap( T & a, T & b )
	{
		const T tmp = a;
		a = b;
		b = tmp;
	}
}

std::streamsize GetFileSize64( const char * file_name );
char * lTrim( char * s );
char * rTrim( char * s );
char * trim( char *s );


std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v");
std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v");
std::string& trim(std::string& s, const char* t = " \t\n\r\f\v");
std::vector<std::string> splitWords(const std::string &s);

void glfw_callback(const int error, const char * description);
void GLAPIENTRY gl_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void * user_param);
void framebuffer_resize_callback(GLFWwindow * window, int width, int height);
char * LoadShader(const char * file_name);
GLint CheckShader(const GLenum shader);

template<typename T> inline void swap(T & a, T & b)
{
	const T tmp = a;
	a = b;
	b = tmp;
}


#endif