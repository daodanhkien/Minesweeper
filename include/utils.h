#ifndef UTILS_H
#define UTILS_H
#include <glad/glad.h>

void init();
unsigned int LoadTexture(const char* name, unsigned int toType = GL_RGBA);
unsigned int LoadShaderFromFile(const char* vertexPath, const char* fragmentPath);
unsigned int LoadShaderFromFile(const char* vertexPath, const char* geometryPath, const char* fragmentPath);
unsigned int LoadShaderFromString(const char* vertexString, const char* fragmentString);
unsigned int LoadShaderFromString(const char* vertexString, const char* geometryString, const char* fragmentString);

#endif
