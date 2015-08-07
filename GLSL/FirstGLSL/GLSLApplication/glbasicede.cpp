#include "glbasicede.h"

#include "glconfig.h"
#include "gloctreeede.h"
#include "glsoctreeede.h"
#include "glroctreeede.h"
#include "glbasegridede.h"
#include "glcgridede.h"

#include "glbuffer.h"

GLBasicEDE::GLBasicEDE(void)
{
	testDynamics = false;
	memoryUsed = -1;
}

int GLBasicEDE::getMemory(void)
{
	if (memoryUsed == -1)
	{
		//Calcula o valor da memória
		calculateMemory();
	}
	return memoryUsed;
}

GLBasicEDE* GLBasicEDE::instantiate(GLConfig* config, bool* loaded)
{
	std::string identifier = "";
	identifier += config->objectName;
	identifier += ":";
	identifier += std::to_string(config->edeDepth);
	identifier += GLBasicEDE::getEDEName(config);

	if (GLBufferHandler::checkForEDE(identifier))
	{
		*loaded = true;
		return GLBufferHandler::edeBuffer[identifier];
	}

	switch (config->type)
	{
	case NONE: return nullptr;
	case OCTREE:
		return new GLOctreeEDE();
	case SOCTREE:
			return new GLSOctreeEDE();
	case ROCTREE:
			return new GLROctreeEDE();
	case BASEGRID:
			return new GLBaseGridEDE();
	case CGRID:
			return new GLCGridEDE();
	default: return nullptr;
	}
}

void GLBasicEDE::setLogger(EDLogger* logger)
{
	this->logger = logger;
}

std::string GLBasicEDE::getEDEName(GLConfig* config)
{
	switch (config->type)
	{
	case NONE: return nullptr;
	case OCTREE:
		return OCTREE_NAME;
	case SOCTREE:
		return SOCTREE_NAME;
	case ROCTREE:
		return ROCTREE_NAME;
	case BASEGRID:
		return BASEGRID_NAME;
	case CGRID:
		return CGRID_NAME;
	default: return nullptr;
	}
}

void GLBasicEDE::bufferizeEDE(GLConfig* config)
{
	std::string identifier = config->objectName + ':' + std::to_string(config->edeDepth);
	identifier = identifier + this->getName();
	GLBufferHandler::addToEDEBuffer(identifier, this);
}