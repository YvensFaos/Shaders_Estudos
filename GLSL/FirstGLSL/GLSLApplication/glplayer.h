#pragma once

#include "glconfig.h"
#include "glcamera.h"
#include "glmesh3d.h"

#define INFO_SIZE 4

struct GLFWwindow;

class GLPlayer
{
protected:
	char* title;

	float angle;
	bool isRunning;
	bool updateMouse;

	double xpos;
	double ypos;

	float deltaTime;
	float lastTime;
public:
	GLConfig config;

	GLMeshHandler* meshHandler;
	GLCamera* camera;
	GLFWwindow* actualWindow;
public:
	GLPlayer(void);
	GLPlayer(GLConfig config);
	~GLPlayer(void);

	virtual void initializeGLPlayer(GLConfig config);

	virtual void step(void);
	virtual bool running(void);

	virtual void lights(void);

	virtual void keyBoard(GLFWwindow* window, int key, int scancode, int action, int mods);
	virtual void mouse(GLFWwindow* window, int button, int action, int mods);

	virtual void updateMousePos();
};