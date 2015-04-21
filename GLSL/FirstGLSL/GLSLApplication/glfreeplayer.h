#pragma once

#include "glplayer.h"

class GLFreePlayer : public GLPlayer
{
private:
	int printCounter;
	char* modeTitle;
public:
	GLFreePlayer(void);
	GLFreePlayer(GLConfig config);
	~GLFreePlayer(void);

	void initializeGLPlayer(GLConfig config);

	void step(void);
	bool running(void);

	void lights(void);

	void keyBoard(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouse(GLFWwindow* window, int button, int action, int mods);

	void updateMousePos();
};