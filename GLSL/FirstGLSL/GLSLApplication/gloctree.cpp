#include "gloctree.h"

#include "glmathhelper.h"
#include "triangle_cube.h"
#include <random>
//GLOctree Node

GLOctreeNode::GLOctreeNode(void)
{
	min = glm::vec3(MIN_FLOAT);
	max = glm::vec3(MAX_FLOAT);

	numMeshes = -1;
	hasNodes = false;
	visible = INVISIBLE;
}

GLOctreeNode::GLOctreeNode(glm::vec3 min, glm::vec3 max, GLMeshHandler* handler, int depth, std::vector<int>* previousIndexes, EDLogger* logger, bool clearIndexes)
{
	this->min = glm::vec3(min);
	this->max = glm::vec3(max);

	hasNodes = false;
	visible = INVISIBLE;

	int inside = 0;
	int totalV = 0;

	nodeColor = glm::vec3(rand() % 255 / 255.f, rand() % 255 / 255.f, rand() % 255 / 255.f);
	char logLine[128];
	sprintf(logLine, "Node color: %f %f %f", VEC3_PRINT(nodeColor));
	logger->logLineTimestamp(logLine);

	//Verifica o que pertence e o que não
	indexes = new std::vector<int>[handler->numMeshes];

	numMeshes = handler->numMeshes;

	for(int i = 0; i < handler->numMeshes; i++)
	{
		GLMesh3D* mesh = &handler->meshes.at(i);
		
		std::vector<int>* previousList = &previousIndexes[i];

		if(previousList && !previousList->empty())
		{
			totalV += previousList->size();
			for(int j = 0; j < previousList->size();)
			{
				glm::vec3* p1 = &mesh->vertexes[previousList->at(j)];
				glm::vec3* p2 = &mesh->vertexes[previousList->at(j + 1)];
				glm::vec3* p3 = &mesh->vertexes[previousList->at(j + 2)];
			
				if(TriangleCube::testIntersection(p1, p2, p3, &this->min, &this->max))
				{
					indexes[i].push_back(previousList->at(j));
					indexes[i].push_back(previousList->at(j + 1));
					indexes[i].push_back(previousList->at(j + 2));

					inside += 3;
				}
				j += 3;
			}
		}
	}

	sprintf(logLine,"Inside: %d/%d", inside, totalV);
	logger->logLineTimestamp(logLine);

	if(--depth > 0 && inside > 0)
	{
		//Então possui filhos
		glm::vec3 center = glm::vec3(min.x + (max.x - min.x)/2.0f, min.y + (max.y - min.y)/2.0f, min.z + (max.z - min.z)/2.0f);
		hasNodes = true;
		//Triviais
		nodes.push_back(GLOctreeNode(min, center, handler, depth, indexes, logger, true));
		nodes.push_back(GLOctreeNode(center, max, handler, depth, indexes, logger, true));

		//Não Triviais
		nodes.push_back(GLOctreeNode(glm::vec3(center.x, min.y, min.z), glm::vec3(max.x, center.y, center.z), handler, depth, indexes, logger, true));
		nodes.push_back(GLOctreeNode(glm::vec3(min.x, center.y, min.z), glm::vec3(center.x, max.y, center.z), handler, depth, indexes, logger, true));
		nodes.push_back(GLOctreeNode(glm::vec3(min.x, min.y, center.z), glm::vec3(center.x, center.y, max.z), handler, depth, indexes, logger, true));
		nodes.push_back(GLOctreeNode(glm::vec3(min.x, center.y, center.z), glm::vec3(center.x, max.y, max.z), handler, depth, indexes, logger, true));
		nodes.push_back(GLOctreeNode(glm::vec3(center.x, min.y, center.z), glm::vec3(max.x, center.y, max.z), handler, depth, indexes, logger, true));
		nodes.push_back(GLOctreeNode(glm::vec3(center.x, center.y, min.z), glm::vec3(max.x, max.y, center.z), handler, depth, indexes, logger, true));

		//Depois de todos os filhos processados
		//Apaga os vetores de índices porque são usados apenas pelos filhos
		if(clearIndexes)
		{
			for(int i = 0; i < handler->numMeshes; i++)
			{
				indexes[i].clear();
			}
		}
	}
	//Se depth - 1 = 0, então é folha
}

GLOctreeNode::~GLOctreeNode(void)
{ }

int GLOctreeNode::getMemory(void)
{
	//Memória estruturas básicas (min, max, hasNodes, numMeshes, nodeColor)
	int memory = sizeof(min) + sizeof(max) + sizeof(int) + sizeof(bool) + sizeof(nodeColor);

	//Memória da lista de índices
	for(int i = 0; i < numMeshes; i++)
	{
		memory += sizeof(int) * indexes[i].size();
	}

	memory += sizeof(vertexes);
	memory += sizeof(normals);
	memory += mesh.getMemory();

	if(hasNodes)
	{
		for(int i = 0; i < nodes.size(); i++)
		{
			memory += nodes.at(i).getMemory();
		}
	}

	return memory;
}

void GLOctreeNode::optimizeNode(EDLogger* logger)
{
	logger->logLineTimestamp("Otimizando node...");

	if(hasNodes)
	{
		logger->logLineTimestamp("Chamando nós filhos...");
		for(int i = 0; i < nodes.size(); i++)
		{
			nodes.at(i).optimizeNode(logger);
		}
	}
	else
	{
		logger->logLineTimestamp("Otimizando arrays de mesh...");
		char logLine[128];
		for(int i = 0; i < numMeshes; i++)
		{
			std::vector<int>* lindexes = &indexes[i];

			if(lindexes->size() > 0)
			{
				std::sort(lindexes->begin(), lindexes->end());
				std::vector<int> continuosIndexes;
				continuosIndexes.push_back(lindexes->at(0));

				int preOpt = lindexes->size();

				int previous = lindexes->at(0);
				for(int j = 1; j < lindexes->size(); j++)
				{
					if(previous + 1 != lindexes->at(j))
					{
						continuosIndexes.push_back(previous);
						continuosIndexes.push_back(lindexes->at(j));
					}
					previous = lindexes->at(j);
				}
				continuosIndexes.push_back(lindexes->at(lindexes->size() - 1));

				lindexes->clear();
				for(int j = 0; j < continuosIndexes.size(); j++)
				{
					lindexes->push_back(continuosIndexes.at(j));
				}
				int posOpt = lindexes->size();

				sprintf(logLine, "[Mesh %d] Otimizado de %d -> %d índices.", i, preOpt, posOpt);
				logger->logLineTimestamp(logLine);
			}
			else
			{
				logger->logLineTimestamp("Nó sem índices!");
			}
		}
	}
}

void GLOctreeNode::generateMesh(EDLogger* logger)
{
	mesh.hasNormals = true;
	mesh.verticesCount = vertexes.size();

	mesh.vertexes = new glm::vec3[mesh.verticesCount];
	mesh.normals  = new glm::vec3[mesh.verticesCount];
	for(int i = 0; i < mesh.verticesCount; i++)
	{
		glm::vec3* v = &vertexes.at(i);
		mesh.vertexes[i] = glm::vec3(v->x, v->y, v->z);
		v = &normals.at(i);
		mesh.normals[i] = glm::vec3(v->x, v->y, v->z);
	}

	vertexes.clear();
	normals.clear();

	char logline[128];
	sprintf(logline, "Mesh de nó criada! - %d v e %d n.", mesh.verticesCount, mesh.verticesCount);
	logger->logLineTimestamp(logline);
	sprintf(logline, "Custo da Mesh : %d", mesh.getMemory());
	logger->logLineTimestamp(logline);
}

//GLOctree

GLOctree::GLOctree()
{ 
	memoryUsed = 0;
}

GLOctree::GLOctree(GLMeshHandler* handler, int depth, EDLogger* logger)
{
	this->logger = logger;

	glm::vec3 max = glm::vec3(0.0f);
	glm::vec3 min = glm::vec3(0.0f);

	logger->logLineTimestamp("Calculando AABB da raíz");
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

	//Os nós são criados a partir da raíz
	root = GLOctreeNode(min, max, handler, depth, indexes, logger, false);
	optimizeTree();
	logTree();
	memoryUsed = root.getMemory();
}

GLOctree::~GLOctree(void)
{ }

int GLOctree::getMemory(void)
{
	return memoryUsed;
}

void GLOctree::optimizeTree(void)
{
	root.optimizeNode(logger);
}

void GLOctree::logTree(void)
{
	GLOctreeNode* stack[256];
	int stackSize = 1;
	int nodeCounter = 0;
	stack[0] = &root;

	char logLine[128];
	while(stackSize != 0)
	{
		GLOctreeNode* top = stack[--stackSize];
		nodeCounter++;

		sprintf(logLine, "Node - [%d] - [%d]", top->indexes[0].size(), top->nodes.size());
		logger->logLineTimestamp(logLine);

		if(top->hasNodes)
		{
			for(int i = 0; i < top->nodes.size(); i++)
			{
				stack[stackSize++] = &top->nodes.at(i);
			}
		}
	}

	sprintf(logLine, "Total Nodes: %d", nodeCounter);
	logger->logLineTimestamp(logLine);
}