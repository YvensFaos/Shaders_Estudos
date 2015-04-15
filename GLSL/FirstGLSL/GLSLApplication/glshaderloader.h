#ifndef __GL_SHADER_LOADER__
#define __GL_SHADER_LOADER__

#include <GL\glew.h>
#include "GLFW/glfw3.h"

#include <stdio.h>
#include <fstream>

class GLShaderLoader
{
public:
	static unsigned long getFileLength(std::ifstream& file);
	static GLchar* loadshader(char* filename, unsigned long* len);
	static void unloadshader(GLubyte** ShaderSource);
};

#endif