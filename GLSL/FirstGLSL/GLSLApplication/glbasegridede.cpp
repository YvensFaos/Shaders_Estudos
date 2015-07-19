#include "glbasegridede.h"

#include "openGLWrapper.h"
#include "glconfig.h"
#include "edlogger.h"
#include "glmathhelper.h"

GLBaseGridEDE::GLBaseGridEDE(void)
{ }

GLBaseGridEDE::~GLBaseGridEDE(void)
{ }

void GLBaseGridEDE::loadEDE(GLConfig* config) 
{
	//Para carregar uma octree a partir de um arquivo
}

void GLBaseGridEDE::renderEDE(GLFrustum* frustum, GLMeshHandler* handler, GLConfig* config, float* info) 
{
	GLint pos = glGetUniformLocation(OpenGLWrapper::programObject, "pos");
	glUniform4f(pos, 0.0f, 0.0f, 0.0f, 0.0f);
	GLint sca = glGetUniformLocation(OpenGLWrapper::programObject, "sca");
	glUniform4f(sca, 1.0f, 1.0f, 1.0f, 1.0f);

	int nodesCount = grid.nodesCount;

	std::vector<GLBaseGridNode*> visibleNodes;
	GLBaseGridNode* node;

	for(int i = 0; i < nodesCount; i++)
	{
		info[0] += 1;
		node = &grid.nodes[i];

		if(frustum->intercepts(&node->min, &node->max))
		{
			info[1] += 1;

			visibleNodes.push_back(node);
			node->visible = VISIBLE;
		}
	}


}

void GLBaseGridEDE::calculateEDE(GLMeshHandler* handler, GLConfig* config) 
{
	this->edeDepth = config->edeDepth;

	char logLine[128];
	sprintf(logLine, "Iniciado a octree tamanho %d.", this->edeDepth);
	logger->logLineTimestamp(logLine);
	grid = GLBaseGrid(handler, logger);
	logger->logLineTimestamp("Concluindo a octree!");
	sprintf(logLine, "Memória usada: %d.", grid.getMemory());
	logger->logLineTimestamp(logLine);

	bufferizeEDE(config);
}

void GLBaseGridEDE::exportEDE(GLConfig* config) 
{
	//Para salvar a octree em um arquivo
}

void GLBaseGridEDE::calculateMemory(void) 
{
	if(memoryUsed != 0)
	{
		return;
	}
	if(grid.memoryUsed == 0)
	{
		memoryUsed = grid.getMemory();
	}
	else
	{
		memoryUsed = grid.memoryUsed;
	}
}

std::string GLBaseGridEDE::getName(void)
{
	return BASEGRID_NAME;
}