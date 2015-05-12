#pragma once

#include "glconfig.h"
#include "glmesh3d.h"
#include "glcamera.h"

class GLScenario
{
public:
	char* name;
	int identifier;

	GLMeshHandler* meshHandler;
	GLCameraHandler cameraHandler;

private:
	GLConfig* config;

public:
	GLScenario(void);
	GLScenario(char* name, GLConfig* config);
	GLScenario(int identifier, GLConfig* config);

	~GLScenario(void);

public:
	static GLCameraStep* defaultStartPosition(int identifier);
	static float defaultCameraSpeed(int identifier);
private:
	void initialize(GLConfig* config);

	static int getIdentifierByName(char* name);
	static void getNameByIdentifier(int identifier, char* dest);
};