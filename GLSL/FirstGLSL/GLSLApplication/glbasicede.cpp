#include "glbasicede.h"

#include "glconfig.h"
#include "gloctreeede.h"
#include "glsoctreeede.h"
#include "glroctreeede.h"

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
	std::string identifier = IDENTIFIER_CODE;
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
		default: return nullptr;
	}
}

void GLBasicEDE::setLogger(EDLogger* logger)
{
	this->logger = logger;
}

void GLBasicEDE::bufferizeEDE(GLConfig* config)
{
	std::string identifier = IDENTIFIER_CODE;
	identifier = identifier + this->getName();
	GLBufferHandler::addToEDEBuffer(identifier, this);
}