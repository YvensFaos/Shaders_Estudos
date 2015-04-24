#include "glbasicede.h"

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