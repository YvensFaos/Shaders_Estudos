#include "glcamera.h"

#include "glm\gtc\matrix_transform.hpp"

#include <stdio.h>

#include "edfile.h"

//GLCameraStep

GLCameraStep::GLCameraStep(void)
{
	initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 45.0f);
}

GLCameraStep::GLCameraStep(glm::vec3 position, glm::vec3 up, glm::vec3 lookat, float fov)
{
	initialize(position, up, lookat, fov);
}

GLCameraStep::~GLCameraStep(void)
{ }

void GLCameraStep::initialize(glm::vec3 position, glm::vec3 up, glm::vec3 lookat, float fov)
{
	this->position = position;
	this->up = up;
	this->lookat = lookat;

	this->fov = fov;
}

//GLCameraHandler

GLCameraHandler::GLCameraHandler(void)
{ }

GLCameraHandler::GLCameraHandler(char* path, char* filename, bool repeated)
{
	this->path = path;
	this->filename = filename;
	this->repeated = repeated;

	initialize();
}

GLCameraHandler::~GLCameraHandler(void)
{ }

GLCameraStep* GLCameraHandler::nextStep()
{
	if(index == size - 1)
	{
		if(repeated)
		{
			index = 0;
		}
		else
		{
			finished = true;
			return &steps[index];
		}
	}
	return &steps[index++];
}

GLCameraStep* GLCameraHandler::actualStep()
{
	return &steps[index];
}

GLCameraStep* GLCameraHandler::getStep(int index)
{
	return &steps[index];
}

void GLCameraHandler::initialize(void)
{
	index = 0;
	finished = false;

	readPathFile();
}

void GLCameraHandler::readPathFile(void)
{
	char file[256];
	sprintf(file, "%s%s%s", path, filename, PATH_EXTENSION);
	
	EDFileReader reader = EDFileReader(file);
	this->size = reader.readLnInt();

	for(int i = 0; i < size; i++)
	{
		float posx = reader.readLnFloat();
		float posy = reader.readLnFloat();
		float posz = reader.readLnFloat();

		float lookx = reader.readLnFloat();
		float looky = reader.readLnFloat();
		float lookz = reader.readLnFloat();

		float upx = reader.readLnFloat();
		float upy = reader.readLnFloat();
		float upz = reader.readLnFloat();

		float fov = reader.readLnFloat();

		steps.push_back(GLCameraStep(glm::vec3(posx,posy,posz), glm::vec3(lookx,looky,lookz), glm::vec3(upx,upy,upz), fov));
	}
}

//GLCamera

GLCamera::GLCamera(void)
{
	position = glm::vec3(0,0,5);
	horizontalAngle = 3.14f;
	verticalAngle = 0.0f;
	fov = 45.0f;

	speed = 10.0f;
	mouseSpeed = 0.005;

	direction = glm::vec3(0.0, 0.0, 1.0f);
	up = glm::vec3(0.0, 1.0, 0.0f);
	right = glm::cross(direction, up);
}

GLCamera::~GLCamera(void)
{ }


void GLCamera::zoom(float value)
{
	fov += value;
	fov = (fov <= 0)? value : fov;
	fov = (fov > 180)? 180.0f - value : fov;

	printf("FOV atual: [%f] (zoom de %f)\n", fov, value);
}

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

	projectionMatrix = glm::perspective(fov, width/ (float)height, 0.1f, 500.0f);
	viewMatrix = glm::lookAt(position, position + direction, up);
}

void GLCamera::calculateMatrix(GLCameraStep* step, float deltaTime, float width, float height)
{
	direction = step->lookat - step->position;
	direction = glm::normalize(direction);
	up = step->up;
	right = glm::cross(direction, up);

	projectionMatrix = glm::perspective(step->fov, width/ (float)height, 0.1f, 500.0f);
	viewMatrix = glm::lookAt(position, position + direction, up);
}