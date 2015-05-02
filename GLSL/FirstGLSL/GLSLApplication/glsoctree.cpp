#include "glsoctree.h"

#include "openGLWrapper.h"
#include "glconfig.h"
#include "edlogger.h"
#include "glmathhelper.h"

GLSOctree::GLSOctree(void)
{
	memoryUsed = 0;
}

GLSOctree::GLSOctree(GLMeshHandler* handler, int depth, EDLogger* logger)
{
	this->logger = logger;

	glm::vec3 max = glm::vec3(0.0f);
	glm::vec3 min = glm::vec3(0.0f);

	logger->logLineTimestamp("Calculando AABB da raíz");
#pragma region calculando aabb
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
#pragma endregion

	//Análise das meshes estáticas, se houver
	
	int staticVerticesCount = 0;
	std::vector<GLMesh3D*> staticMeshes;
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

	staticMesh.normals = new glm::vec3[staticVerticesCount];
	staticMesh.vertexes = new glm::vec3[staticVerticesCount];
	staticMesh.hasNormals = true;
	staticMesh.verticesCount = staticVerticesCount;

	int k = 0;
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
}

