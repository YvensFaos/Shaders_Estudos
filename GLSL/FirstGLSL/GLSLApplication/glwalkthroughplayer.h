#pragma once

#include "glplayer.h"
#include "glscenario.h"

class GLWalkthroughPlayer : public GLPlayer
{
private:
	int printCounter;
	char* modeTitle;
	
	bool paused;

	GLScenario* scenario;
	GLCameraHandler* cameraHandler;
public:
	GLWalkthroughPlayer(void);
	GLWalkthroughPlayer(GLConfig config);
	~GLWalkthroughPlayer(void);

	void initializeGLPlayer(GLConfig config);

	void step(void);
	bool running(void);

	void lights(void);

	void keyBoard(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouse(GLFWwindow* window, int button, int action, int mods);

private:
	void pause();
	bool isPaused();
};