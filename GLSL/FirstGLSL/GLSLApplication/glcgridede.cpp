#include "glcgridede.h"

#include "openGLWrapper.h"
#include "glconfig.h"
#include "edlogger.h"
#include "glmathhelper.h"

GLCGridEDE::GLCGridEDE(void)
{ }

GLCGridEDE::~GLCGridEDE(void)
{ }

void GLCGridEDE::loadEDE(GLConfig* config)
{
	//Para carregar uma octree a partir de um arquivo
}

void GLCGridEDE::renderEDE(GLFrustum* frustum, GLMeshHandler* handler, GLConfig* config, float* info)
{
	GLint pos = glGetUniformLocation(OpenGLWrapper::programObject, "pos");
	glUniform4f(pos, 0.0f, 0.0f, 0.0f, 0.0f);
	GLint sca = glGetUniformLocation(OpenGLWrapper::programObject, "sca");
	glUniform4f(sca, 1.0f, 1.0f, 1.0f, 1.0f);

	int nodesCount = grid.nodesCount;

	std::vector<GLBaseGridNode*> visibleNodes;
	GLBaseGridNode* node;

	int vertexCount = 0;
	for (int i = 0; i < nodesCount; i++)
	{
		info[0] += 1;
		node = &grid.nodes[i];
		node->visible = INVISIBLE;

		if (node->numIndicesTotal > 0 && frustum->intercepts(&node->min, &node->max))
		{
			info[1] += 1;
			node->visible = VISIBLE;
			vertexCount += node->mesh->verticesCount;
		}
	}

	glm::vec3* vertices = new glm::vec3[vertexCount];
	glm::vec3* normals  = new glm::vec3[vertexCount];

	int index = 0;
	int nodeVerticesCount = 0;
	GLMesh3D* mesh;

	for (int i = 0; i < nodesCount; i++)
	{
		node = &grid.nodes[i];
		if (node->visible == VISIBLE)
		{
			nodeVerticesCount = node->mesh->verticesCount;
			mesh = node->mesh;

			for (int j = 0; j < nodeVerticesCount; j++)
			{
				vertices[index] = glm::vec3(mesh->vertexes[j]);
				normals[index] = glm::vec3(mesh->normals[j]);
				index++;
			}
		}
	}

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(OpenGLWrapper::normalLoc, 3, GL_FLOAT, GL_FALSE, 0, normals);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	info[2] = vertexCount;

	delete[] vertices;
	delete[] normals;
}

void GLCGridEDE::calculateEDE(GLMeshHandler* handler, GLConfig* config)
{
	this->edeDepth = config->edeDepth;

	char logLine[128];
	sprintf(logLine, "Iniciado a grid tamanho %f.", powf(BASE_NODE_COUNT, config->edeDepth));
	logger->logLineTimestamp(logLine);
	grid = GLBaseGrid(handler, config->edeDepth, logger);
	logger->logLineTimestamp("Concluindo a grid!");
	sprintf(logLine, "Memória usada: %d.", grid.getMemory());
	logger->logLineTimestamp(logLine);

	bufferizeEDE(config);
}

void GLCGridEDE::exportEDE(GLConfig* config)
{
	//Para salvar a octree em um arquivo
}

void GLCGridEDE::calculateMemory(void)
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

std::string GLCGridEDE::getName(void)
{
	return CGRID_NAME;
}