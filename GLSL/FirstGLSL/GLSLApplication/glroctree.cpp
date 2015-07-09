#include "glroctree.h"

#include "openGLWrapper.h"
#include "glconfig.h"
#include "edlogger.h"
#include "glmathhelper.h"

GLROctree::GLROctree(void)
{
	memoryUsed = 0;
}

GLROctree::GLROctree(GLMeshHandler* handler, int depth, EDLogger* logger)
{
	this->logger = logger;

	glm::vec3 max = glm::vec3(0.0f);
	glm::vec3 min = glm::vec3(0.0f);

	logger->logLineTimestamp("Calculando AABB da raíz");
#pragma region calculando aabb
	for(int i = 0; i < handler->numMeshes; i++)
	{
		GLMesh3D* mesh = &handler->meshes.at(i);

		if(mesh->verticesCount > 0)
		{
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
	}
#pragma endregion

	char logLine[128];
	sprintf(logLine, "Mínimo: %4.2f %4.2f %4.2f", VEC3_PRINT(min));
	logger->logLineTimestamp(logLine);
	sprintf(logLine, "Máximo: %4.2f %4.2f %4.2f", VEC3_PRINT(max));
	logger->logLineTimestamp(logLine);
	//Análise das meshes estáticas, se houver
	
	int staticVerticesCount = 0;
	std::vector<GLMesh3D*> staticMeshes;
	int k = 0;
	for(int i = 0; i < handler->numMeshes; i++)
	{
		GLMesh3D* mesh = &handler->meshes.at(i);
		if(mesh->verticesCount < STATIC_THRESHOLD)
		{
			staticMeshes.push_back(mesh);
			staticVerticesCount += mesh->verticesCount;
		}
		else
		{
			testedMeshes.push_back(mesh);
		}
	}

	int verticesCount = 0;
	std::vector<int>* indexes = new std::vector<int>[testedMeshes.size()];
	for(int i = 0; i < testedMeshes.size(); i++)
	{
		GLMesh3D* mesh = testedMeshes.at(i);
		verticesCount += mesh->verticesCount;
		for(int j = 0; j < mesh->verticesCount; j++)
		{
			indexes[i].push_back(j);
		}
	}

	sprintf(logLine, "Static Meshes: %d\r\n%sTested Meshes: %d", staticMeshes.size(), LOG_SPACING, testedMeshes.size());
	logger->logLineTimestamp(logLine);

	staticMesh.normals = new glm::vec3[staticVerticesCount];
	staticMesh.vertexes = new glm::vec3[staticVerticesCount];
	staticMesh.hasNormals = true;
	staticMesh.verticesCount = staticVerticesCount;

	sprintf(logLine, "Static Meshes Vertice Count: %d", staticVerticesCount);
	logger->logLineTimestamp(logLine);
	logger->logLineTimestamp("Gerando mesh estática única...");

	k = 0;
	for(int i = 0; i < staticMeshes.size(); i++)
	{
		GLMesh3D* mesh = staticMeshes.at(i);
		for(int j = 0; j < mesh->verticesCount; j++)
		{
			staticMesh.vertexes[k] = glm::vec3(mesh->vertexes[j]);
			staticMesh.normals[k] = glm::vec3(mesh->normals[j]);
			k++;
		}
	}
	logger->logLineTimestamp("Finalizado!");

	GLMeshHandler newHandler;
	newHandler.numMeshes = testedMeshes.size();
	for(int i = 0; i < newHandler.numMeshes; i++)
	{
		newHandler.meshes.push_back(*testedMeshes.at(i));
	}

	logger->logLineTimestamp("Gerando nós a partir das malhas testáveis...");
	root = GLOctreeNode(min, max, &newHandler, depth, indexes, logger, false);
	logger->logLineTimestamp("Finalizado!");

	logger->logLineTimestamp("Gerando meshes para os nós folhas ...");
	createNodeMeshes(&newHandler);
	logger->logLineTimestamp("Finalizado!");

	logTree(verticesCount);
	memoryUsed = root.getMemory();
}

GLROctree::~GLROctree(void)
{ }

void GLROctree::createNodeMeshes(GLMeshHandler* handler)
{
	GLOctreeNode* stack[256];
	int stackSize = 1;
	int nodeCounter = 0;
	stack[0] = &root;

	int index;
	while(stackSize != 0)
	{
		GLOctreeNode* top = stack[--stackSize];

		if(top->hasNodes)
		{
			for(int i = 0; i < top->nodes.size(); i++)
			{
				stack[stackSize++] = &top->nodes.at(i);
			}
		}

		for(int i = 0; i < handler->numMeshes; i++)
		{
			GLMesh3D* mesh = &handler->meshes.at(i);

			int size = top->indexes[i].size();

			for(int j = 0; j < top->indexes->size(); j++)
			{
				index = top->indexes[i].at(j);

				//TODO precisa verificar isso aqui
				if(mesh->verticesCount < index)
				{
					printf("ERRO!\n");
				}

				glm::vec3* vertex = &mesh->vertexes[index];
				glm::vec3* normal = &mesh->normals[index];

				top->vertexes.push_back(glm::vec3(vertex->x, vertex->y, vertex->z));
				top->normals.push_back(glm::vec3(normal->x, normal->y, normal->z));
			}
		}

		top->generateMesh(logger);
	}
}

void GLROctree::logTree(int verticesCount)
{
	GLOctreeNode* stack[256];
	int stackSize = 1;
	int nodeCounter = 0;
	stack[0] = &root;

	int doubled = 0;

	char logLine[128];
	while(stackSize != 0)
	{
		GLOctreeNode* top = stack[--stackSize];
		nodeCounter++;

		doubled += top->mesh.verticesCount;
		sprintf(logLine, "Vertices count: %d", top->mesh.verticesCount);
		logger->logLineTimestamp(logLine);

		if(top->hasNodes)
		{
			for(int i = 0; i < top->nodes.size(); i++)
			{
				stack[stackSize++] = &top->nodes.at(i);
			}
		}
	}

	doubled -= verticesCount;
	sprintf(logLine, "Vértices duplicados: %d", doubled);
	logger->logLineTimestamp(logLine);
	float percent = doubled * 100 / (float) verticesCount;
	sprintf(logLine, "Percentual de duplicados: %f", percent);
	logger->logLineTimestamp(logLine);

	sprintf(logLine, "Total Nodes: %d", nodeCounter);
	logger->logLineTimestamp(logLine);
}

int GLROctree::getMemory(void)
{
	return memoryUsed;
}