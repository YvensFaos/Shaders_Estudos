#pragma once

#include "glmesh3d.h"

#include <string.h>

class GLDynamic
{
private:
	int index;
	glm::vec3 standPosition;

public:
	GLMeshHandler* handler;
	std::string name;
	bool moveable;

public:
	GLDynamic(void);
	GLDynamic(char* modelPath, char* model3d, char* pathlocation, char* pathName);
	~GLDynamic(void);

	void update(void);
	int draw(void);

public:
	static void getTestDynamic(std::vector<GLDynamic>* dynamics);

private:
	void readDynamic(char* filepath, char* pathlocation);
};
