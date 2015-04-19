#ifndef __GL_CAMERA__
#define __GL_CAMERA__

#include "glm\glm.hpp"

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

	void calculateMatrix(float xpos, float ypos, float deltaTime, float width, float height);
	void calculateMatrix(GLCameraStep* step,     float deltaTime, float width, float height);
};

#endif