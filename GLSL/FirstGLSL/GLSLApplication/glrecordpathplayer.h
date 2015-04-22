#pragma once

#include "glplayer.h"
#include "glscenario.h"

class GLRecordPathPlayer : public GLPlayer
{
private:
	int printCounter;
	int pathCounter;
	char* modeTitle;

	bool recording;
	
	GLCameraStep* actualStep;

	GLScenario* scenario;
	GLCameraHandler* cameraHandler;
public:
	GLRecordPathPlayer(void);
	GLRecordPathPlayer(GLConfig config);
	~GLRecordPathPlayer(void);

	void initializeGLPlayer(GLConfig config);

	void step(void);
	bool running(void);

	void lights(void);

	void keyBoard(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouse(GLFWwindow* window, int button, int action, int mods);
private:
	void record();
	bool isRecording();
};