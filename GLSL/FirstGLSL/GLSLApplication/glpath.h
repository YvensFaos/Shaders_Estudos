#pragma once

#include <vector>
#include "glcamera.h"

class GLPath
{
public:
	int size;

	GLPath(void);
	GLPath(char* pathfile);
	GLPath(char* pathfilePath, char* pathfileName);
	~GLPath(void);

	GLCameraStep* getStep(int index);
	GLCameraStep* getStep(int index, glm::vec3 translate);

	std::vector<GLCameraStep>* steps;

	void readPathFile(char* pathfile);
};