#include "glcamera.h"

#include "glm\gtc\matrix_transform.hpp"

#include <stdio.h>

GLCamera::GLCamera(void)
{
	position = glm::vec3(0,0,5);
	horizontalAngle = 3.14f;
	verticalAngle = 0.0f;
	initialFoV = 45.0f;

	speed = 10.0f;
	mouseSpeed = 0.005;

	direction = glm::vec3(0.0, 0.0, 1.0f);
	up = glm::vec3(0.0, 1.0, 0.0f);
	right = glm::cross(direction, up);
}

GLCamera::~GLCamera(void)
{ }

void GLCamera::calculateMatrix(float xpos, float ypos, float deltaTime, float width, float height)
{
	horizontalAngle += mouseSpeed * deltaTime * float(width/2 - xpos );
	verticalAngle   += mouseSpeed * deltaTime * float(height/2 - ypos );

	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f),
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);

	up = glm::cross(right, direction);

	projectionMatrix = glm::perspective(initialFoV, width/ (float)height, 0.1f, 500.0f);
	viewMatrix = glm::lookAt(position, position + direction, up);
	
}