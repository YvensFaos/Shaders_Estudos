#pragma once

#include "glmesh3d.h"
#include "glpath.h"

#include <string.h>

class GLDynamicObject
{
public:
	std::string pathName;
	std::string dynamicName;

	glm::vec3 translate;
	glm::vec3 position;

	int index;
	GLPath* pathReference;
	GLMeshHandler* meshHandler;
public:
	GLDynamicObject(void);
	GLDynamicObject(std::string pathName, std::string dynamicName, int index, glm::vec3 translate);
	~GLDynamicObject(void);

	void update(void);
	void draw(void);
};

class GLDynamic
{
public:
	static std::vector<GLDynamicObject>* generateDynamics(std::string modelPath, std::string model, std::string pathPath, std::string path, int quantity, glm::vec3 translate);
};
