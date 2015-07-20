#include "glbasicede.h"

#include "glconfig.h"
#include "gloctreeede.h"
#include "glsoctreeede.h"
#include "glroctreeede.h"
#include "glbasegridede.h"

#include "glbuffer.h"

GLBasicEDE::GLBasicEDE(void)
{
	testDynamics = false;
	memoryUsed = -1;
}

int GLBasicEDE::getMemory(void)
{
	if(memoryUsed == -1)
	{
		//Calcula o valor da memória
		calculateMemory();
	}
	return memoryUsed;
}

GLBasicEDE* GLBasicEDE::instantiate(GLConfig* config)
{
	std::string identifier = "";
	identifier += config->objectName;
	identifier += ":";
	identifier += std::to_string(config->edeDepth);

	switch(config->type)
	{
		case NONE: return nullptr;
		case OCTREE: 
			identifier = identifier + OCTREE_NAME;
			if(GLBufferHandler::checkForEDE(identifier))
			{
				return GLBufferHandler::edeBuffer[identifier];
			}
			else
			{
				return new GLOctreeEDE();
			}
		case SOCTREE: 
			identifier = identifier + SOCTREE_NAME;
			if(GLBufferHandler::checkForEDE(identifier))
			{
				return GLBufferHandler::edeBuffer[identifier];
			}
			else
			{
				return new GLSOctreeEDE();
			}
		case ROCTREE: 
			identifier = identifier + ROCTREE_NAME;
			if(GLBufferHandler::checkForEDE(identifier))
			{
				return GLBufferHandler::edeBuffer[identifier];
			}
			else
			{
				return new GLROctreeEDE();
			}
		case BASEGRID:
			identifier = identifier + BASEGRID_NAME;
			if(GLBufferHandler::checkForEDE(identifier))
			{
				return GLBufferHandler::edeBuffer[identifier];
			}
			else
			{
				return new GLBaseGridEDE();
			}
		default: return nullptr;
	}
}

void GLBasicEDE::setLogger(EDLogger* logger)
{
	this->logger = logger;
}

void GLBasicEDE::bufferizeEDE(GLConfig* config)
{
	std::string identifier = config->objectName + ':' + std::to_string(config->edeDepth);
	identifier = identifier + this->getName();
	GLBufferHandler::addToEDEBuffer(identifier, this);
}