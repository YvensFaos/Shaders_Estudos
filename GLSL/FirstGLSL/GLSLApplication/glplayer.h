#ifndef __GL_PLAYER__
#define __GL_PLAYER__

#include "glconfig.h"
#include "glcamera.h"
#include "glmesh3d.h"

struct GLFWwindow;

class GLPlayer
{
private:
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

	GLMesh3D* mesh;
	GLCamera* camera;
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
	void mouse(GLFWwindow* window, int button, int action, int mods);

	void updateMousePos();
};

#endif