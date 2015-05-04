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

	checkVisibility(frustum, &octree.root, info);
	recursiveDraw(config, &octree.root, info);
}

VISIBILITY_STATUS GLROctreeEDE::checkVisibility(GLFrustum* frustum, GLOctreeNode* node, float* info)
{
	//[0]~ Qtde. Nós testados
	//[1]~ Qtde. Nós Visíveis
	//[2]~ Qtde. Triângulos Enviados
	//[3]~ Qtde. Draw Calls

	info[0] += 1;
	node->visible = INVISIBLE;

	if(frustum->intercepts(&node->min, &node->max))
	{
		info[1] += 1;
		node->visible = VISIBLE;
		if(node->hasNodes)
		{
			bool test = true;
			for(int i = 0; i < node->nodes.size(); i++)
			{
				test &= (checkVisibility(frustum, &node->nodes.at(i), info) == VISIBLE) ? true : false;
			}

			if(!test)
			{
				node->visible = PARTIALLY_VISIBLE;
			}
		}
	}

	return node->visible;
}

void GLROctreeEDE::recursiveDraw(GLConfig* config, GLOctreeNode* node, float* info)
{
	switch(node->visible)
	{
	case VISIBLE:
		{
			if(config->coloredNodes)
			{
				GLint loc = glGetUniformLocation(OpenGLWrapper::programObject, "baseColor");
				glUniform4f(loc, node->nodeColor.r, node->nodeColor.g, node->nodeColor.b, 1.0f);
			}

			node->mesh.prerender();
			node->mesh.render();
			info[2] += node->mesh.verticesCount;
			info[3] += 1;
		}
		break;
	case PARTIALLY_VISIBLE:
		{
			for(int i = 0; i < node->nodes.size(); i++)
			{
				recursiveDraw(config, &node->nodes.at(i), info);
			}
		}
		break;
	}
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