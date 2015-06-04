#include "gloctreeede.h"

#include "openGLWrapper.h"
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

void GLOctreeEDE::renderEDE(GLFrustum* frustum, GLMeshHandler* handler, GLConfig* config, float* info) 
{
	GLOctreeNode* stack[256];
	int stackSize = 1;
	int nodeCounter = 0;
	stack[0] = &octree.root;

	//[0]~ Qtde. Nós testados
	//[1]~ Qtde. Nós Visíveis
	//[2]~ Qtde. Triângulos Enviados
	//[3]~ Qtde. Draw Calls

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

				for(int i = 0; i < top->numMeshes; i++)
				{
					handler->prerender(i);

					std::vector<int>* printIndex = &top->indexes[i];

					for(int j = 0; j < printIndex->size();)
					{
						handler->render(i, printIndex->at(j), printIndex->at(j + 1));
						info[2] += printIndex->at(j + 1) - printIndex->at(j) + 1;
						info[3] += 1;
						j += 2;
					}
				}

				if(config->enableDynamics && testDynamics)
				{
					for(int i  = 0; i < config->dynamics.size(); i++)
					{
						GLDynamicObject* obj = &config->dynamics.at(i);
						if(!obj->visible)
						{
							if(GLAABB::intercepts(obj->meshHandler->max, obj->meshHandler->min, top->max, top->min))
							{
								obj->visible = true;
							}
						}
					}
				}
			}
		}
	}

	//Verificar dinâmicos


}

void GLOctreeEDE::calculateEDE(GLMeshHandler* handler, GLConfig* config) 
{
	this->edeDepth = config->edeDepth;

	char logLine[128];
	sprintf(logLine, "Iniciado a octree tamanho %d.", this->edeDepth);
	logger->logLineTimestamp(logLine);
	octree = GLOctree(handler, this->edeDepth, logger);
	logger->logLineTimestamp("Concluindo a octree!");
	sprintf(logLine, "Memória usada: %d.", octree.getMemory());
	logger->logLineTimestamp(logLine);

	bufferizeEDE(config);
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

std::string GLOctreeEDE::getName(void)
{
	return OCTREE_NAME;
}