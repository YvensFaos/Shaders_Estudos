#ifndef __OPENGL_WRAPPER__
#define __OPENGL_WRAPPER__

#define GLEW_STATIC

#include <GL\glew.h>
#include "GLFW/glfw3.h"

#include "glplayer.h"

typedef void (*loopCallback)(void);
typedef bool (*runningCallback)(void);

class OpenGLWrapper
{
public:
	static GLuint programObject;
	static GLuint normalLoc;
	static GLPlayer player;
	static GLFWwindow* window;
private:
	static float ratio;
	static loopCallback callback;
	static runningCallback running;
public: 
	static void error_callback(int error, const char* description);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
	static void initialize(loopCallback callback, bool antialiasing, int multisampling);
	static void loop_callback(loopCallback callback);
	static void running_callback(runningCallback running);
	static void glLoop(void);
	static void finalize(void);

	static GLuint loadShader(const char *shaderSrc, GLenum type);
};

#endif