#ifndef GL_UTILS_H_
#define GL_UTILS_H_

void SetMatrix4x4( const GLuint program, const GLfloat * data, const char * matrix_name );
void CreateBindlessTexture(GLuint & texture, GLuint64 & handle, const int width, const int height, const GLvoid * data);

#endif
