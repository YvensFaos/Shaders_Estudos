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

	bool visible;

	int index;
	GLPath* pathReference;
	GLMeshHandler* meshHandler;
public:
	GLDynamicObject(void);
	GLDynamicObject(std::string pathName, std::string dynamicName, int index, glm::vec3 translate);
	~GLDynamicObject(void);

	void update(void);
	void draw(void);
	void draw(glm::vec3 pos);

	//Draw Bouding Box
	void drawBox(void);

	void getBounds(glm::vec3 bounds[2]);
};

class GLDynamic
{
public:
	static std::vector<GLDynamicObject>* generateDynamics(std::string modelPath, std::string model, std::string pathPath, std::string path, int quantity, glm::vec3 translate);
};
