#include "glcamera.h"

#include "glm\gtc\matrix_transform.hpp"

#include <stdio.h>

#include "edfile.h"
#include "glenums.h"
#include "glmathhelper.h"
#include "glbuffer.h"

//GLCameraStep

GLCameraStep::GLCameraStep(void)
{
	initialize(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 45.0f);
}

GLCameraStep::GLCameraStep(glm::vec3 position, glm::vec3 up, glm::vec3 direction, float fov)
{
	initialize(position, up, direction, fov);
}

GLCameraStep::GLCameraStep(GLCamera* camera)
{
	initialize(camera->position, camera->up, camera->direction, camera->fov);
}

GLCameraStep::~GLCameraStep(void)
{ }

void GLCameraStep::initialize(glm::vec3 position, glm::vec3 up, glm::vec3 direction, float fov)
{
	this->position = position;
	this->up = up;
	this->direction = direction;

	right = glm::cross(up, direction);

	this->fov = fov;
}

void GLCameraStep::zoom(float value)
{
	fov += value;
	fov = (fov <= 0)? value : fov;
	fov = (fov > 180)? 180.0f - value : fov;
}

void GLCameraStep::rotate(glm::vec3 around, float angle)
{
	if(VEC3_EQUALS(XAXIS, around))
	{
		//TODO
	}
	if(VEC3_EQUALS(YAXIS, around))
	{
		float _xV = 0.f;
		float _zV = 0.f;
		float cosa = 0.f;
		float sina = 0.f;

		angle = angle*PI180;

		cosa = cos(angle);
		sina = sin(angle);

		_xV = direction.x;
		_zV = direction.z;
			
		direction.x =  _xV*cosa + _zV*sina;
		direction.z = -_xV*sina + _zV*cosa;
	}
	if(VEC3_EQUALS(ZAXIS, around))
	{
		//TODO
	}
	//TODO para um eixo arbitrário
}

void GLCameraStep::print(void)
{
	printf("GLCameraStep:\n");
	printf("Position: %4.2f %4.2f %4.2f\n", VEC3_PRINT(position));
	printf("Direction: %4.2f %4.2f %4.2f\n", VEC3_PRINT(direction));
	printf("Up: %4.2f %4.2f %4.2f\n", VEC3_PRINT(up));
	printf("Right: %4.2f %4.2f %4.2f\n", VEC3_PRINT(right));
	printf("FoV: %4.2f\n", fov);
}

//GLCameraHandler

GLCameraHandler::GLCameraHandler(void)
{ }

GLCameraHandler::GLCameraHandler(char* pathfilePath, char* pathfileName, bool repeated)
{
	this->path = new char[256];
	sprintf(this->path, "%s", pathfilePath);
	this->filename = new char[256];
	sprintf(this->filename, "%s", pathfileName);
	this->repeated = repeated;

	index = 0;
	finished = false;

	if(GLBufferHandler::checkForPath(pathfileName))
	{
		steps = GLBufferHandler::pathBuffer[pathfileName];
	}
	else
	{
		readPathFile();
		GLBufferHandler::addToPathBuffer(pathfileName, steps);
	}
}

GLCameraHandler::GLCameraHandler(char* pathfilePath, char* pathfileName, int pathIdentifier, char* pathExtraMsg)
{
	this->path = pathfilePath;
	this->filename = pathfileName;
	this->repeated = repeated;
	this->pathIdentifier = pathIdentifier;
	this->pathExtraMsg = pathExtraMsg;

	index = 0;
	finished = false;
	steps = nullptr;
}

GLCameraHandler::~GLCameraHandler(void)
{ }

GLCameraStep* GLCameraHandler::nextStep()
{
	if(index >= steps->size() - 1)
	{

		if(repeated)
		{
			index = 0;
		}
		else
		{
			finished = true;
			return &steps->at(index);
		}
	}
	return &steps->at(index++);
}

GLCameraStep* GLCameraHandler::actualStep()
{
	return &steps->at(index);
}

GLCameraStep* GLCameraHandler::getStep(int index)
{
	return &steps->at(index);
}

void GLCameraHandler::readPathFile(void)
{
	char file[256];
	sprintf(file, "%s%s%s", this->path, this->filename, PATH_EXTENSION);
	
	EDFileReader reader = EDFileReader(file);
	this->size = reader.readLnInt();

	steps = new std::vector<GLCameraStep>();

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

		steps->push_back(GLCameraStep(glm::vec3(posx,posy,posz), glm::vec3(upx,upy,upz), glm::vec3(lookx,looky,lookz), fov));
	}

	reader.close();
}

int GLCameraHandler::getIndex()
{
	return index;
}

void GLCameraHandler::stardRecording(GLCamera* firstStep)
{
	if(!steps)
	{
		steps = new std::vector<GLCameraStep>();
	}

	steps->clear();
	steps->push_back(GLCameraStep(firstStep));
	size = 1;
}

void GLCameraHandler::addStepRecording(GLCamera* step)
{
	steps->push_back(GLCameraStep(step));
	size++;
}

void GLCameraHandler::stopRecording(void)
{
	//Gravar o path em um arquivo de path

	char recordedFilename[512];
	sprintf(recordedFilename, "%s%s-[%d][%s]%s", path, filename, pathIdentifier, pathExtraMsg, PATH_EXTENSION);

	EDFileWriter writer = EDFileWriter(recordedFilename);
	writer.writeLnInt(size);

	for(int i = 0; i < size; i++)
	{
		GLCameraStep* step = &steps->at(i);
		writer.writeLnFloat(step->position.x);
		writer.writeLnFloat(step->position.y);
		writer.writeLnFloat(step->position.z);

		writer.writeLnFloat(step->direction.x);
		writer.writeLnFloat(step->direction.y);
		writer.writeLnFloat(step->direction.z);

		writer.writeLnFloat(step->up.x);
		writer.writeLnFloat(step->up.y);
		writer.writeLnFloat(step->up.z);

		writer.writeLnFloat(step->fov);
	}

	writer.close();

	pathIdentifier++;
}

//GLCamera

GLCamera::GLCamera(void)
{
	position = glm::vec3(0,0,5);
	horizontalAngle = 3.14f;
	verticalAngle = 0.0f;
	fov = 45.0f;

	speed = 100.0f;
	mouseSpeed = 0.005;

	direction = glm::vec3(0.0, 0.0, 1.0f);
	up = glm::vec3(0.0, 1.0, 0.0f);
	right = glm::cross(direction, up);

	near = 0.1f;
	far = 500.0f;
}

GLCamera::~GLCamera(void)
{ }


void GLCamera::zoom(float value)
{
	fov += value;
	fov = (fov <= 0)? value : fov;
	fov = (fov > 180)? 180.0f - value : fov;

	//printf("FOV atual: [%f] (zoom de %f)\n", fov, value);
}

void GLCamera::setValues(GLCameraStep* step)
{
	position.x = step->position.x;
	position.y = step->position.y;
	position.z = step->position.z;

	direction.x = step->direction.x;
	direction.y = step->direction.y;
	direction.z = step->direction.z;

	up.x = step->up.x;
	up.y = step->up.y;
	up.z = step->up.z;

	right = glm::cross(up, direction);
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

	projectionMatrix = glm::perspective(fov, width/ (float)height, near, far);
	viewMatrix = glm::lookAt(position, position + direction, up);
}

void GLCamera::calculateMatrix(GLCameraStep* step, float deltaTime, float width, float height)
{
	this->position = step->position;
	this->direction = step->direction;
	this->up = step->up;
	this->right = step->right;

	projectionMatrix = glm::perspective(step->fov, width/ (float)height, 0.1f, 500.0f);
	viewMatrix = glm::lookAt(position, position + direction, up);
}

void GLCamera::print(void)
{
	printf("GLCamera:\n");
	printf("Position: %4.2f %4.2f %4.2f\n", VEC3_PRINT(position));
	printf("Direction: %4.2f %4.2f %4.2f\n", VEC3_PRINT(direction));
	printf("Up: %4.2f %4.2f %4.2f\n", VEC3_PRINT(up));
	printf("Right: %4.2f %4.2f %4.2f\n", VEC3_PRINT(right));
	printf("FoV: %4.2f\n", fov);
	printf("Speed: %4.2f\n", speed);
	printf("Mouse Speed: %4.2f\n", mouseSpeed);
}