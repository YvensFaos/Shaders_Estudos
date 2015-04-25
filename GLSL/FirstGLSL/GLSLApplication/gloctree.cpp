#include "gloctree.h"

#include "glmathhelper.h"

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

	//Verifica o que pertence e o que não

	std::vector<int>* indexes = new std::vector<int>[handler->numMeshes];

	for(int i = 0; i < handler->numMeshes; i++)
	{
		GLMesh3D* mesh = &handler->meshes.at(i);
		
		for(int j = 0; j < mesh->verticesCount;)
		{

		}
	}
}

GLOctreeNode::~GLOctreeNode(void)
{ }

//GLOctree

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