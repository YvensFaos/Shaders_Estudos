#include "glbasegrid.h"

#include "glmathhelper.h"
#include "triangle_cube.h"
#include <random>

//GLBaseGridNode

GLBaseGridNode::GLBaseGridNode(void)
{
	min = glm::vec3(MIN_FLOAT);
	max = glm::vec3(MAX_FLOAT);

	numMeshes = -1;
	numIndicesTotal = -1;
	visible = INVISIBLE;
}

GLBaseGridNode::GLBaseGridNode(glm::vec3 min, glm::vec3 max, std::vector<int>* indexes, GLMeshHandler* handler, EDLogger* logger)
{
	this->min = glm::vec3(min);
	this->max = glm::vec3(max);

	visible = INVISIBLE;

	int inside = 0;
	int totalV = 0;

	nodeColor = glm::vec3(rand() % 255 / 255.f, rand() % 255 / 255.f, rand() % 255 / 255.f);
	char logLine[128];
	sprintf(logLine, "Node color: %f %f %f", VEC3_PRINT(nodeColor));
	logger->logLineTimestamp(logLine);

	numMeshes = handler->numMeshes;

	//Verifica o que pertence e o que não
	localIndexes = new std::vector<int>[numMeshes];

	for(int i = 0; i < numMeshes; i++)
	{
		localIndexes[i].clear();
		GLMesh3D* mesh = &handler->meshes.at(i);
		
		for(int j = 0; j < indexes->size();)
		{
			glm::vec3* p1 = &mesh->vertexes[indexes->at(j)];
			glm::vec3* p2 = &mesh->vertexes[indexes->at(j + 1)];
			glm::vec3* p3 = &mesh->vertexes[indexes->at(j + 2)];
			
			if(TriangleCube::testIntersection(p1, p2, p3, &this->min, &this->max))
			{
				localIndexes[i].push_back(indexes->at(j));
				localIndexes[i].push_back(indexes->at(j + 1));
				localIndexes[i].push_back(indexes->at(j + 2));

				inside += 3;
			}
			j += 3;
		}
	}

	sprintf(logLine,"Inside: %d/%d", inside, totalV);

	numIndicesTotal = inside;
	logger->logLineTimestamp(logLine);

	generateMesh(handler, logger);
}

void GLBaseGridNode::generateMesh(GLMeshHandler* handler, EDLogger* logger)
{
	int indexSize = localIndexes->size();
	std::vector<glm::vec3> vertexes;
	std::vector<glm::vec3> normals;

	for (int j = 0; j < numMeshes; j++)
	{
		indexSize = localIndexes[j].size();
		mesh = &handler->meshes[j];

		for (int k = 0; k < indexSize; k++)
		{
			vertexes.push_back(mesh->vertexes[localIndexes[j].at(k)]);
			normals.push_back(mesh->normals[localIndexes[j].at(k)]);
		}
	}

	mesh = new GLMesh3D(&vertexes, &normals);
}

GLBaseGridNode::~GLBaseGridNode(void)
{ }

int GLBaseGridNode::getMemory(void)
{
	//Memória estruturas básicas (min, max, hasNodes, numMeshes, nodeColor)
	int memory = sizeof(min) + sizeof(max) + sizeof(int) + sizeof(bool) + sizeof(nodeColor);

	//Memória da lista de índices
	for(int i = 0; i < numMeshes; i++)
	{
		memory += sizeof(int) * localIndexes[i].size();
	}

	return memory;
}

//GLBaseGrid

GLBaseGrid::GLBaseGrid(void)
{
	memoryUsed = 0;
}

GLBaseGrid::GLBaseGrid(GLMeshHandler* handler, EDLogger* logger)
{
	this->logger = logger;

	glm::vec3 max = glm::vec3(0.0f);
	glm::vec3 min = glm::vec3(0.0f);

	logger->logLineTimestamp("Calculando AABB da estrutura toda");
	std::vector<int>* indexes = new std::vector<int>[handler->numMeshes];
	for(int i = 0; i < handler->numMeshes; i++)
	{
		GLMesh3D* mesh = &handler->meshes.at(i);

		if(mesh->verticesCount > 0)
		{
			for(int j = 0; j < mesh->verticesCount; j++)
			{
				indexes[i].push_back(j);
			}

			#pragma region buscar valores max e min
			if(mesh->max.x > max.x)
			{
				max.x = mesh->max.x;
			}
			if(mesh->max.y > max.y)
			{
				max.y = mesh->max.y;
			}
			if(mesh->max.z > max.z)
			{
				max.z = mesh->max.z;
			}

			if(mesh->min.x < min.x)
			{
				min.x = mesh->min.x;
			}
			if(mesh->min.y < min.y)
			{
				min.y = mesh->min.y;
			}
			if(mesh->min.z < min.z)
			{
				min.z = mesh->min.z;
			}
			#pragma endregion
		}
		else
		{
			indexes[i].clear();
		}
	}

	char logLine[128];
	sprintf(logLine, "Mínimo: %4.2f %4.2f %4.2f", VEC3_PRINT(min));
	logger->logLineTimestamp(logLine);
	sprintf(logLine, "Máximo: %4.2f %4.2f %4.2f", VEC3_PRINT(max));
	logger->logLineTimestamp(logLine);

	nodesCount = NODE_COUNT;
	float sqrtNodesCount = sqrt(nodesCount);
	float minX = min.x;

	float stepX = (max.x - min.x) / sqrtNodesCount;
	float stepZ = (max.z - min.z) / sqrtNodesCount;

	glm::vec3 stepMin = min;
	glm::vec3 stepMax = min;
	stepMax.x += stepX;
	stepMax.y = max.y;
	stepMax.z += stepZ;

	nodes = new GLBaseGridNode[nodesCount];
	int k = 0;
	for(int i = 0; i < sqrtNodesCount; i++)
	{
		for(int j = 0; j < sqrtNodesCount; j++)
		{
			nodes[k++] = GLBaseGridNode(stepMin, stepMax, indexes, handler, logger);
			stepMin.x += stepX;
			stepMax.x += stepX;
		}
		stepMin.x = min.x;
		stepMax.x = min.x + stepX;

		stepMin.z += stepZ;
		stepMax.z += stepZ;
	}

	for(int i = 0; i < nodesCount; i++)
	{
		memoryUsed += nodes[i].getMemory();
	}
	logStructure();
}

GLBaseGrid::~GLBaseGrid(void)
{ }

int GLBaseGrid::getMemory(void)
{
	return memoryUsed;
}

void GLBaseGrid::logStructure(void)
{
	char logLine[128];

	GLBaseGridNode* node;
	for(int i = 0; i < nodesCount; i++)
	{
		node = &nodes[i];

		sprintf(logLine, "Node - [%d]", node->numIndicesTotal);
		logger->logLineTimestamp(logLine);
	}

	sprintf(logLine, "Total Nodes: %d", nodesCount);
	logger->logLineTimestamp(logLine);
}