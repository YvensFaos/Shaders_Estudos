#pragma once

#include "glplayer.h"
#include "glscenario.h"
#include "glbasicede.h"
#include "edlogger.h"
#include "glmathhelper.h"

class GLSequentialRecorderPlayer : public GLPlayer
{
private:
	int printCounter;
	int printIndex;

	char* modeTitle;
	char* logLine;

	bool paused;
	bool logged;
	bool recording;

	EDPrinter recorder;

	GLBasicEDE* ede;
	GLScenario* scenario;
	GLCameraHandler* cameraHandler;
	EDLogger* logger;

	GLFrustum frustum;

	float info[INFO_SIZE];
public:
	GLSequentialRecorderPlayer(void);
	GLSequentialRecorderPlayer(GLConfig config);
	~GLSequentialRecorderPlayer(void);

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