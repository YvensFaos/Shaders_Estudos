#pragma once

#include "glplayer.h"
#include "glscenario.h"
#include "glbasicede.h"
#include "edlogger.h"
#include "glmathhelper.h"

class GLSequentialFlyingRecorderPlayer : public GLPlayer
{
private:
	int printCounter;
	int printIndex;

	char* modeTitle;
	char* logLine;

	bool paused;
	bool logged;

	EDPrinter recorder;

	GLBasicEDE* ede;
	GLScenario* scenario;
	GLCameraHandler* cameraHandler;
	EDLogger* logger;

	GLFrustum frustum;

	GLCameraStep* actualStep;
	GLCameraStep* walkStep;

	float info[INFO_SIZE];
public:
	GLSequentialFlyingRecorderPlayer(void);
	GLSequentialFlyingRecorderPlayer(GLConfig config);
	~GLSequentialFlyingRecorderPlayer(void);

	void initializeGLPlayer(GLConfig config);

	void step(void);
	bool running(void);

	void lights(void);

	void keyBoard(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouse(GLFWwindow* window, int button, int action, int mods);

	void updateMousePos(void);
private:
	void pause();
	bool isPaused();
};