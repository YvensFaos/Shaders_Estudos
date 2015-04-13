#ifndef __GL_PLAYER__
#define __GL_PLAYER__

#include "glconfig.h"

struct GLFWwindow;

class GLPlayer
{
private:
	char* title;

	bool isRunning;
public:
	GLConfig config;

	GLFWwindow* actualWindow;
public:
	GLPlayer(void);
	GLPlayer(GLConfig config);
	~GLPlayer(void);

	void initializeGLPlayer(GLConfig config);

	void step(void);
	bool running(void);

	void lights(void);

	void keyBoard(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif