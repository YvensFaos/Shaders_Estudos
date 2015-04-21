#pragma once

#include "glm\glm.hpp"
#include <vector>

#define PATH_EXTENSION ".pathx"

class GLCameraStep
{
public:
	glm::vec3 position;
	glm::vec3 up;
	glm::vec3 lookat;

	float fov;
public:
	GLCameraStep(void);
	GLCameraStep(glm::vec3 position, glm::vec3 up, glm::vec3 lookat, float fov);
	~GLCameraStep(void);

private:
	void initialize(glm::vec3 position, glm::vec3 up, glm::vec3 lookat, float fov);
};

class GLCameraHandler
{
public:
	char* path;
	char* filename;

	bool repeated;
	bool finished;
private:
	int index;
	int size;

	std::vector<GLCameraStep> steps;
public:
	GLCameraHandler(void);
	GLCameraHandler(char* path, char* filename, bool repeated);
	~GLCameraHandler(void);

	GLCameraStep* nextStep();
	GLCameraStep* actualStep();
	GLCameraStep* getStep(int index);

	int getIndex();
private:
	void initialize(void);

	void readPathFile(void);
};

class GLCamera
{
public:
	glm::vec3 position;

	glm::vec3 direction;
	glm::vec3 up;
	glm::vec3 right;

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	float horizontalAngle;
	float verticalAngle;
	float fov;

	float speed;
	float mouseSpeed;

public:
	GLCamera(void);
	~GLCamera(void);

	void zoom(float value);

	void calculateMatrix(float xpos, float ypos, float deltaTime, float width, float height);
	void calculateMatrix(GLCameraStep* step,     float deltaTime, float width, float height);
};