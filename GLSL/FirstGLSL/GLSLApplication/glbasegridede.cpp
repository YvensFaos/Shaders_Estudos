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

	for (int i = 0; i < nodesCount; i++)
	{
		info[0] += 1;
		node = &grid.nodes[i];

		if (node->numIndicesTotal > 0 && frustum->intercepts(&node->min, &node->max))
		{
			if (config->coloredNodes)
			{
				GLint loc = glGetUniformLocation(OpenGLWrapper::programObject, "baseColor");
				glUniform4f(loc, node->nodeColor.r, node->nodeColor.g, node->nodeColor.b, 1.0f);
			}
			info[1] += 1;

			node->mesh->prerender();
			info[2] += node->mesh->render();
		}
	}
}

void GLBaseGridEDE::calculateEDE(GLMeshHandler* handler, GLConfig* config)
{
	this->edeDepth = config->edeDepth;

	char logLine[128];
	sprintf(logLine, "Iniciado a grid tamanho %f.", powf(BASE_NODE_COUNT, config->edeDepth));
	logger->logLineTimestamp(logLine);
	grid = GLBaseGrid(handler, config->edeDepth, logger);
	logger->logLineTimestamp("Concluindo a grid!");
	sprintf(logLine, "Qtde. Células: %d.", grid.nodesCount);
	logger->logLineTimestamp(logLine);
	sprintf(logLine, "Memória usada: %d.", grid.getMemory());
	logger->logLineTimestamp(logLine);

	if (config->calculateFrustumAABB)
	{
		float tanfovx = tan(((config->fov / 2)*config->aspect) * PI180);
		float sqrt = tanfovx * grid.cellArea;
		sqrt = sqrtf(sqrt);
		sqrt += config->near;

		GLFrustum::aabbFactor = sqrt / config->far;

		sprintf(logLine, "Fator utilizado: %4.2f.", GLFrustum::aabbFactor);
		logger->logLineTimestamp(logLine);
	}

	bufferizeEDE(config);
}

void GLBaseGridEDE::exportEDE(GLConfig* config)
{
	//Para salvar a octree em um arquivo
}

void GLBaseGridEDE::calculateMemory(void)
{
	if (memoryUsed != 0)
	{
		return;
	}
	if (grid.memoryUsed == 0)
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