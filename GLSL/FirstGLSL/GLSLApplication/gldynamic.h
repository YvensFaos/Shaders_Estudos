#pragma once

#include "glmesh3d.h"

#include <string.h>

class GLDynamicObject
{
public:
	std::string pathName;
	std::string dynamicName;
	int index;
	
public:
	GLDynamicObject(void);
	GLDynamicObject(std::string pathName, std::string dynamicName, int index);
	~GLDynamicObject(void);
};

class GLDynamic
{
private:
	int index;
	glm::vec3 standPosition;
	std::vector<GLCameraStep>* steps;
public:
	GLMeshHandler* handler;
	std::string name;
	bool moveable;

public:
	GLDynamic(void);
	GLDynamic(char* modelPath, char* model3d, glm::vec3 position);
	GLDynamic(char* modelPath, char* model3d, char* pathLocation, char* pathName);
	~GLDynamic(void);

	void update(void);
	int draw(void);

public:
	static void getTestDynamic(std::vector<GLDynamic>* dynamics);

private:
	void readDynamic(char* filepath, char* pathlocation);
	void readPathFile(char* pathLocation, char* pathName);
};
