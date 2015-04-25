#include "gloctree.h"

#include "glmathhelper.h"
#include "triangle_cube.h"

//GLOctree Node

GLOctreeNode::GLOctreeNode(void)
{
	min = glm::vec3(MIN_FLOAT);
	max = glm::vec3(MAX_FLOAT);
}

GLOctreeNode::GLOctreeNode(glm::vec3 min, glm::vec3 max, GLMeshHandler* handler, int depth)
{
	this->min = glm::vec3(min);
	this->max = glm::vec3(max);

	int inside = 0;
	int discar = 0;
	int totalV = 0;

	//Verifica o que pertence e o que não
	indexes = new std::vector<int>[handler->numMeshes];
	for(int i = 0; i < handler->numMeshes; i++)
	{
		GLMesh3D* mesh = &handler->meshes.at(i);
		
		totalV += mesh->verticesCount;
		for(int j = 0; j < mesh->verticesCount;)
		{
			glm::vec3* p1 = &mesh->vertexes[j];
			glm::vec3* p2 = &mesh->vertexes[j + 1];
			glm::vec3* p3 = &mesh->vertexes[j + 2];
			
			if(TriangleCube::testIntersection(p1, p2, p3, &this->min, &this->max))
			{
				indexes[i].push_back(j);
				indexes[i].push_back(j + 1);
				indexes[i].push_back(j + 2);
				
				inside += 3;
			}
			else
			{
				discar += 3;
			}
			j += 3;
		}
	}

	printf("Inside: %d/%d [%d]\n", inside, totalV, discar);

	if(--depth > 0)
	{
		//Então possui filhos
		glm::vec3 center = glm::vec3(min.x + (max.x - min.x)/2.0f, min.y + (max.y - min.y)/2.0f, min.z + (max.z - min.z)/2.0f);

		nodes = new GLOctreeNode[8];
		int nodePos = 0;
		//Triviais
		nodes[nodePos++] = GLOctreeNode(min, center, handler, depth, &indexes);
		nodes[nodePos++] = GLOctreeNode(center, max, handler, depth, &indexes);

		//Não Triviais
		nodes[nodePos++] = GLOctreeNode(glm::vec3(center.x, min.y, min.z), glm::vec3(max.x, center.y, center.z), handler, depth, &indexes);
		nodes[nodePos++] = GLOctreeNode(glm::vec3(min.x, center.y, min.z), glm::vec3(center.x, max.y, center.z), handler, depth, &indexes);
		nodes[nodePos++] = GLOctreeNode(glm::vec3(min.x, min.y, center.z), glm::vec3(center.x, center.y, max.z), handler, depth, &indexes);
		nodes[nodePos++] = GLOctreeNode(glm::vec3(min.x, center.y, center.z), glm::vec3(center.x, max.y, max.z), handler, depth, &indexes);
		nodes[nodePos++] = GLOctreeNode(glm::vec3(center.x, min.y, center.z), glm::vec3(max.x, center.y, max.z), handler, depth, &indexes);
		nodes[nodePos++] = GLOctreeNode(glm::vec3(center.x, center.y, min.z), glm::vec3(max.x, max.y, center.z), handler, depth, &indexes);

		//Depois de todos os filhos processados
		//Apaga os vetores de índices porque são usados apenas pelos filhos
		for(int i = 0; i < handler->numMeshes; i++)
		{
			indexes[i].clear();
		}
	}
	//Se depth - 1 = 0, então é folha
}

GLOctreeNode::GLOctreeNode(glm::vec3 min, glm::vec3 max, GLMeshHandler* handler, int depth, std::vector<int>** previousIndexes)
{
	this->min = glm::vec3(min);
	this->max = glm::vec3(max);

	int inside = 0;
	int totalV = 0;

	//Verifica o que pertence e o que não
	indexes = new std::vector<int>[handler->numMeshes];

	for(int i = 0; i < handler->numMeshes; i++)
	{
		GLMesh3D* mesh = &handler->meshes.at(i);
		
		std::vector<int>* previousList = previousIndexes[i];

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

	printf("Inside: %d/%d\n", inside, totalV);

	if(--depth > 0)
	{
		//Então possui filhos
		glm::vec3 center = glm::vec3(min.x + (max.x - min.x)/2.0f, min.y + (max.y - min.y)/2.0f, min.z + (max.z - min.z)/2.0f);

		nodes = new GLOctreeNode[8];
		int nodePos = 0;
		//Triviais
		nodes[nodePos++] = GLOctreeNode(min, center, handler, depth, &indexes);
		nodes[nodePos++] = GLOctreeNode(center, max, handler, depth, &indexes);

		//Não Triviais
		nodes[nodePos++] = GLOctreeNode(glm::vec3(center.x, min.y, min.z), glm::vec3(max.x, center.y, center.z), handler, depth, &indexes);
		nodes[nodePos++] = GLOctreeNode(glm::vec3(min.x, center.y, min.z), glm::vec3(center.x, max.y, center.z), handler, depth, &indexes);
		nodes[nodePos++] = GLOctreeNode(glm::vec3(min.x, min.y, center.z), glm::vec3(center.x, center.y, max.z), handler, depth, &indexes);
		nodes[nodePos++] = GLOctreeNode(glm::vec3(min.x, center.y, center.z), glm::vec3(center.x, max.y, max.z), handler, depth, &indexes);
		nodes[nodePos++] = GLOctreeNode(glm::vec3(center.x, min.y, center.z), glm::vec3(max.x, center.y, max.z), handler, depth, &indexes);
		nodes[nodePos++] = GLOctreeNode(glm::vec3(center.x, center.y, min.z), glm::vec3(max.x, max.y, center.z), handler, depth, &indexes);

		//Depois de todos os filhos processados
		//Apaga os vetores de índices porque são usados apenas pelos filhos
		for(int i = 0; i < handler->numMeshes; i++)
		{
			indexes[i].clear();
		}
	}
	//Se depth - 1 = 0, então é folha
}

GLOctreeNode::~GLOctreeNode(void)
{ }

//GLOctree

GLOctree::GLOctree()
{ }

GLOctree::GLOctree(GLMeshHandler* handler, int depth)
{
	glm::vec3 max = glm::vec3(0.0f);
	glm::vec3 min = glm::vec3(0.0f);

	for(int i = 0; i < handler->numMeshes; i++)
	{
		GLMesh3D* mesh = &handler->meshes.at(i);

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

		//Os nós são criados a partir da raíz
		root = GLOctreeNode(min, max, handler, depth);
	}
}

GLOctree::~GLOctree(void)
{
}