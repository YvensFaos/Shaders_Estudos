#pragma once

#include "glm\glm.hpp"
#include <vector>

#define PATH_EXTENSION ".pathx"

//Forward declaration
class GLCamera;
class GLConfig;

class GLCameraStep
{
public:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 right;
	glm::vec3 up;

	float fov;
public:
	GLCameraStep(void);
	GLCameraStep(glm::vec3 position, glm::vec3 up, glm::vec3 direction, float fov);
	GLCameraStep(GLCamera* camera);
	~GLCameraStep(void);

	void zoom(float value);
	void rotate(glm::vec3 around, float angle);

	void print(void);
private:
	void initialize(glm::vec3 position, glm::vec3 up, glm::vec3 direction, float fov);
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

	int pathIdentifier;
	char* pathExtraMsg;

	std::vector<GLCameraStep>* steps;
public:
	GLCameraHandler(void);
	GLCameraHandler(char* pathfilePath, char* pathfileName, bool repeated);
	GLCameraHandler(char* pathfilePath, char* pathfileName, int pathIdentifier, char* pathExtraMsg);
	~GLCameraHandler(void);

	GLCameraStep* nextStep();
	GLCameraStep* actualStep();
	GLCameraStep* getStep(int index);

	int getIndex();

	void stardRecording(GLCamera* firstStep);
	void addStepRecording(GLCamera* step);
	void stopRecording(void);
private:
	void readPathFile(void);
	void recordPathFile(void);
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

	float near;
	float far;
public:
	GLCamera(void);
	GLCamera(GLConfig* config);
	~GLCamera(void);

	void zoom(float value);

	void setValues(GLCameraStep* step);

	void calculateMatrix(GLConfig* config, float xpos, float ypos, float deltaTime);
	void calculateMatrix(GLCameraStep* step, GLConfig* config, float deltaTime);

	void print();
};