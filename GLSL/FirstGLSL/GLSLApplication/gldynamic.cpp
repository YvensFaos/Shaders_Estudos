#include "gldynamic.h"

#include "glbuffer.h"

//GLDynamicObject
GLDynamicObject::GLDynamicObject(void)
{
	pathName = "";
	dynamicName = "";
	index = -1;
}

GLDynamicObject::GLDynamicObject(std::string pathName, std::string dynamicName, int index)
{
	this->pathName = pathName;
	this->dynamicName = dynamicName;
	this->index;
}

GLDynamicObject::~GLDynamicObject(void)
{ }

//GLDynamic
GLDynamic::GLDynamic(void)
{ 
	index = -1;
	moveable = false;
	standPosition = glm::vec3(0.0f, 0.0f, 0.0f);
}

GLDynamic::GLDynamic(char* modelPath, char* model3d, glm::vec3 position)
{ 
	if(GLBufferHandler::checkForMeshHandler(model3d))
	{
		handler = GLBufferHandler::meshHandlerBuffer[model3d];
	}
	else
	{
		handler = new GLMeshHandler(model3d, modelPath);
		GLBufferHandler::addToMeshHandlerBuffer(model3d, handler);
	}

	index = 0;
	moveable = true;
	standPosition = glm::vec3(position.x, position.y, position.z);
}

GLDynamic::GLDynamic(char* modelPath, char* model3d, char* pathLocation, char* pathName)
{ 
	if(GLBufferHandler::checkForMeshHandler(model3d))
	{
		handler = GLBufferHandler::meshHandlerBuffer[model3d];
	}
	else
	{
		handler = new GLMeshHandler(model3d, modelPath);
		GLBufferHandler::addToMeshHandlerBuffer(model3d, handler);
	}

	if(GLBufferHandler::checkForPath(pathName))
	{
		steps = GLBufferHandler::pathBuffer[pathName];
	}
	else
	{
		readPathFile(pathLocation, pathName);
		GLBufferHandler::addToPathBuffer(pathName, steps);
	}

	index = 0;
	moveable = true;
	standPosition = glm::vec3(0.0f, 0.0f, 0.0f);
}



