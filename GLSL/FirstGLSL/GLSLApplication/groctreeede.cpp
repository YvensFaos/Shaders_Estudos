#include "glroctreeede.h"

#include "openGLWrapper.h"
#include "glconfig.h"
#include "edlogger.h"
#include "glmathhelper.h"

GLROctreeEDE::GLROctreeEDE(void)
{ }

GLROctreeEDE::~GLROctreeEDE(void)
{ }

void GLROctreeEDE::loadEDE(GLConfig* config) 
{
	//Para carregar uma octree a partir de um arquivo
}

void GLROctreeEDE::renderEDE(GLFrustum* frustum, GLMeshHandler* handler, GLConfig* config, float* info) 
{
	//[0]~ Qtde. Nós testados
	//[1]~ Qtde. Nós Visíveis
	//[2]~ Qtde. Triângulos Enviados
	//[3]~ Qtde. Draw Calls

	checkVisibility(frustum, config, &octree.root, info);

}

bool GLROctreeEDE::checkVisibility(GLFrustum* frustum, GLConfig* config, GLOctreeNode* node, float* localInfo)
{
	//[0]~ Qtde. Nós testados
	//[1]~ Qtde. Nós Visíveis
	//[2]~ Qtde. Triângulos Enviados
	//[3]~ Qtde. Draw Calls

	localInfo[0] += 1;
	node->visible = false;

	GLOctreeNode* nodePointer;
	if(frustum->intercepts(&node->min, &node->max))
	{
		localInfo[1] += 1;
		node->visible = true;
		if(node->hasNodes)
		{
			for(int i = 0; i < node->nodes.size(); i++)
			{
				node->visible &= checkVisibility(frustum, config, &node->nodes.at(i), localInfo);
			}

			//Se todos os filhos estão visíveis, então renderiza o pai
			if(!node->visible)
			{
				if(config->coloredNodes)
				{
					GLint loc = glGetUniformLocation(OpenGLWrapper::programObject, "baseColor");
					glUniform4f(loc, node->nodeColor.r, node->nodeColor.g, node->nodeColor.b, 1.0f);
				}

				for(int i = 0; i < node->nodes.size(); i++)
				{
					nodePointer = &node->nodes.at(i);

					if(nodePointer->visible)
					{
						nodePointer->mesh.prerender();
						nodePointer->mesh.render();
						localInfo[2] += nodePointer->mesh.verticesCount;
						localInfo[3] += 1;
					}
				}
			}
		}
	}

	return node->visible;
}

void GLROctreeEDE::calculateEDE(GLMeshHandler* handler, GLConfig* config) 
{
	this->edeDepth = config->edeDepth;

	char logLine[128];
	sprintf(logLine, "Iniciado a replicated octree tamanho %d.", this->edeDepth);
	logger->logLineTimestamp(logLine);
	octree = GLROctree(handler, this->edeDepth, logger);
	logger->logLineTimestamp("Concluindo a s-octree!");
	sprintf(logLine, "Memória usada: %d.", octree.getMemory());
	logger->logLineTimestamp(logLine);
}

void GLROctreeEDE::exportEDE(GLConfig* config) 
{
	//Para salvar a octree em um arquivo
}

void GLROctreeEDE::calculateMemory(void) 
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


char* GLROctreeEDE::getName(char* name)
{
	sprintf(name, "Replicated Octree");

	return name;
}