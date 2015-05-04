#include "glbasicede.h"

#include "glconfig.h"
#include "gloctreeede.h"
#include "glsoctreeede.h"
#include "glroctreeede.h"

GLBasicEDE::GLBasicEDE(void)
{
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
	switch(config->type)
	{
		case NONE: return nullptr;
		case OCTREE: return new GLOctreeEDE();
		case SOCTREE: return new GLSOctreeEDE();
		case ROCTREE: return new GLROctreeEDE();
		default: return nullptr;
	}
}

void GLBasicEDE::setLogger(EDLogger* logger)
{
	this->logger = logger;
}