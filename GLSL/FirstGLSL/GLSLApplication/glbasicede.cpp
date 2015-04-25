#include "glbasicede.h"

#include "glconfig.h"

#include "gloctreeede.h"

GLBasicEDE::GLBasicEDE(void)
{
	memoryUsed = -1;
}

int GLBasicEDE::getMemory(void)
{
	if(memoryUsed == -1)
	{
		//Calcula o valor da mem�ria
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
		default: return nullptr;
	}
}