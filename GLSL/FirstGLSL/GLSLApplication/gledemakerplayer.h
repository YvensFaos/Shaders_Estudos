#pragma once

#include "glplayer.h"
#include "glscenario.h"
#include "edlogger.h"
#include "glmathhelper.h"

#include "glbasicede.h"

class GLEDEMakerPlayer : public GLPlayer
{
private:
	int printCounter;
	float aspect;

	char* modeTitle;
	char* logLine;

	GLScenario* scenario;
	GLCameraHandler* cameraHandler;
	EDLogger* logger;

	GLBasicEDE* ede;

	GLFrustum frustum;
public:
	GLEDEMakerPlayer(void);
	GLEDEMakerPlayer(GLConfig config);
	~GLEDEMakerPlayer(void);

	void initializeGLPlayer(GLConfig config);

	void step(void);
	bool running(void);

	void lights(void);

	void keyBoard(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouse(GLFWwindow* window, int button, int action, int mods);

private:
	void updateMousePos();
};