#include "glsoctreeede.h"

#include "openGLWrapper.h"
#include "glconfig.h"
#include "edlogger.h"
#include "glmathhelper.h"

GLSOctreeEDE::GLSOctreeEDE(void)
{ }

GLSOctreeEDE::~GLSOctreeEDE(void)
{ }

void GLSOctreeEDE::loadEDE(GLConfig* config) 
{
	//Para carregar uma octree a partir de um arquivo
}

void GLSOctreeEDE::renderEDE(GLFrustum* frustum, GLMeshHandler* handler, GLConfig* config, float* info) 
{
	GLOctreeNode* stack[256];
	int stackSize = 1;
	int nodeCounter = 0;
	stack[0] = &octree.root;

	//[0]~ Qtde. Nós testados
	//[1]~ Qtde. Nós Visíveis
	//[2]~ Qtde. Triângulos Enviados
	//[3]~ Qtde. Draw Calls

	GLMesh3D* mesh;
	while(stackSize != 0)
	{
		GLOctreeNode* top = stack[--stackSize];
		info[0] += 1;

		if(frustum->intercepts(&top->min, &top->max))
		{
			info[1] += 1;

			if(top->hasNodes)
			{
				for(int i = 0; i < top->nodes.size(); i++)
				{
					stack[stackSize++] = &top->nodes.at(i);
				}
			}
			else
			{
				if(config->coloredNodes)
				{
					GLint loc = glGetUniformLocation(OpenGLWrapper::programObject, "baseColor");
					glUniform4f(loc, top->nodeColor.r, top->nodeColor.g, top->nodeColor.b, 1.0f);
				}

				mesh = &top->mesh;
				mesh->prerender();
				mesh->render();
			}
		}
	}

}

void GLSOctreeEDE::calculateEDE(GLMeshHandler* handler, GLConfig* config) 
{
	this->edeDepth = config->edeDepth;

	char logLine[128];
	sprintf(logLine, "Iniciado a s-octree tamanho %d.", this->edeDepth);
	logger->logLineTimestamp(logLine);
	octree = GLSOctree(handler, this->edeDepth, logger);
	logger->logLineTimestamp("Concluindo a s-octree!");
	sprintf(logLine, "Memória usada: %d.", octree.getMemory());
	logger->logLineTimestamp(logLine);
}

void GLSOctreeEDE::exportEDE(GLConfig* config) 
{
	//Para salvar a octree em um arquivo
}

void GLSOctreeEDE::calculateMemory(void) 
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


char* GLSOctreeEDE::getName(char* name)
{
	sprintf(name, "S-Octree");

	return name;
}