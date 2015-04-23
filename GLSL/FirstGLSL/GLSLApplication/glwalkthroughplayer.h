#pragma once

#include "glplayer.h"
#include "glscenario.h"
#include "edlogger.h"

class GLWalkthroughPlayer : public GLPlayer
{
private:
	int printCounter;
	char* modeTitle;
	char* logLine;
	
	bool paused;
	bool logged;

	GLScenario* scenario;
	GLCameraHandler* cameraHandler;

	EDLogger* logger;
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