#pragma once

#define GLEW_STATIC

#include <GL\glew.h>
#include "GLFW/glfw3.h"

#include "glplayer.h"
class OpenGLWrapper
{
public:
	static GLuint programObject;
	static GLuint normalLoc;
	static GLuint positionLoc;

	static GLPlayer* player;
	static GLFWwindow* window;
private:
	static float ratio;
public: 
	static void error_callback(int error, const char* description);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
	static void initialize(bool antialiasing, int multisampling);
	static void glLoop(void);
	static void finalize(void);

	static GLuint loadShader(const char *shaderSrc, GLenum type);
};