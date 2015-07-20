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

		if(node->numIndicesTotal > 0 && frustum->intercepts(&node->min, &node->max))
		{
			info[1] += 1;

			visibleNodes.push_back(node);
		}
	}

	int numMeshes = handler->numMeshes;
	int indexSize = 0;

	std::vector<glm::vec3> vertexes;
	std::vector<glm::vec3> normals;

	GLMesh3D* mesh;

	for(int i = 0; i < visibleNodes.size(); i++)
	{
		node = visibleNodes[i];

		for(int j = 0; j < numMeshes; j++)
		{
			indexSize = node->localIndexes[j].size();
			mesh = &handler->meshes[j];

			for(int k = 0; k < indexSize; k++)
			{
				if(indexes[j][node->localIndexes[j].at(k)] == 0)
				{
					vertexes.push_back(mesh->vertexes[node->localIndexes[j].at(k)]);
					normals.push_back(mesh->normals[node->localIndexes[j].at(k)]);

					indexes[j][node->localIndexes[j].at(k)] = 0;
				}
			}
		}
	}

	mesh = new GLMesh3D(&vertexes, &normals);
	mesh->prerender();
	mesh->render();

	delete mesh;

	cleanIndexes(handler);
}

void GLBaseGridEDE::calculateEDE(GLMeshHandler* handler, GLConfig* config) 
{
	this->edeDepth = config->edeDepth;
	
	indexes = new int*[handler->numMeshes];
	createIndexes(handler);

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

void GLBaseGridEDE::createIndexes(GLMeshHandler* handler) 
{
	int count = 0;

	for(int i = 0; i < handler->numMeshes; i++)
	{
		GLMesh3D* mesh = &handler->meshes[i];

		count = mesh->verticesCount;
		indexes[i] = new int[count];

		for(int j = 0; j < count; j++)
		{
			indexes[i][j] = 0;
		}
	}
}

void GLBaseGridEDE::cleanIndexes(GLMeshHandler* handler) 
{
	int count = 0;

	for(int i = 0; i < handler->numMeshes; i++)
	{
		GLMesh3D* mesh = &handler->meshes[i];

		count = mesh->verticesCount;

		for(int j = 0; j < count; j++)
		{
			indexes[i][j] = 0;
		}
	}
}

std::string GLBaseGridEDE::getName(void)
{
	return BASEGRID_NAME;
}