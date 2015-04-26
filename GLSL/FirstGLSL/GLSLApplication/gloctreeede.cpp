#include "gloctreeede.h"

#include "glconfig.h"
#include "edlogger.h"
#include "glmathhelper.h"

GLOctreeEDE::GLOctreeEDE(void)
{ }

GLOctreeEDE::~GLOctreeEDE(void)
{ }

void GLOctreeEDE::loadEDE(GLConfig* config) 
{
	//Para carregar uma octree a partir de um arquivo
}

void GLOctreeEDE::renderEDE(GLFrustum* frustum, GLMeshHandler* handler, GLConfig* config) 
{
	GLOctreeNode* stack[256];
	int stackSize = 1;
	int nodeCounter = 0;
	stack[0] = &octree.root;

	while(stackSize != 0)
	{
		GLOctreeNode* top = stack[--stackSize];

		if(frustum->intercepts(&top->min, &top->max))
		{
			if(top->hasNodes)
			{
				for(int i = 0; i < top->nodes.size(); i++)
				{
					stack[stackSize++] = &top->nodes.at(i);
				}
			}
			else
			{
				for(int i = 0; i < top->numMeshes; i++)
				{
					std::vector<int>* printIndex = &top->indexes[i];

					for(int j = 0; j < printIndex->size();)
					{
						handler->render(i, j, j + 2);
						j += 3;
					}
				}
			}
		}
	}
}

void GLOctreeEDE::calculateEDE(GLMeshHandler* handler, GLConfig* config) 
{
	this->edeDepth = config->edeDepth;

	char logLine[128];
	sprintf(logLine, "Iniciado a octree tamanho %d.", this->edeDepth);
	logger->logLineTimestamp(logLine);
	octree = GLOctree(handler, this->edeDepth, logger);
	logger->logLineTimestamp("Concluindo a octree!");
	sprintf(logLine, "Mémória usada: %d.", octree.getMemory());
	logger->logLineTimestamp(logLine);
}

void GLOctreeEDE::exportEDE(GLConfig* config) 
{
	//Para salvar a octree em um arquivo
}

void GLOctreeEDE::calculateMemory(void) 
{
	if(memoryUsed != 0)
	{
		return;
	}
	if(octree.memoryUsed == 0)
	{
		memoryUsed = octree.getMemory();
	}
	else
	{
		memoryUsed = octree.memoryUsed;
	}
}


char* GLOctreeEDE::getName(char* name)
{
	sprintf(name, "Octree");

	return name;
}