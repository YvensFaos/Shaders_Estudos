#include "glmesh3d.h"

#include "IL\il.h"

#include <GL\glew.h>
#include "GLFW\glfw3.h"

#include "openGLWrapper.h"
#include "glprinthelper.h"
#include "glmathhelper.h"

GLuint GLMeshHandler::gl_index = 0;

//GLMeshHandler
GLMeshHandler::GLMeshHandler(void)
{ }

GLMeshHandler::~GLMeshHandler(void)
{ }

GLMeshHandler::GLMeshHandler(char* file)
{
	printf("Loading: %s\n", file);

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

	max = glm::vec3(MIN_FLOAT, MIN_FLOAT, MIN_FLOAT);
	min = glm::vec3(MAX_FLOAT, MAX_FLOAT, MAX_FLOAT);

	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		meshes.push_back(GLMesh3D(i, gl_index++, scene));

		GLMesh3D* meshp = &meshes.at(meshes.size() - 1);

		//Pegar os maiores e menores
#pragma region buscar valores max e min
		if (meshp->max.x > max.x)
		{
			max.x = meshp->max.x;
		}
		if (meshp->max.y > max.y)
		{
			max.y = meshp->max.y;
		}
		if (meshp->max.z > max.z)
		{
			max.z = meshp->max.z;
		}

		if (meshp->min.x < min.x)
		{
			min.x = meshp->min.x;
		}
		if (meshp->min.y < min.y)
		{
			min.y = meshp->min.y;
		}
		if (meshp->min.z < min.z)
		{
			min.z = meshp->min.z;
		}
#pragma endregion
	}

	this->numMeshes = scene->mNumMeshes;
}

GLMeshHandler::GLMeshHandler(char* model3d, char* path)
{
	char sFilePath[512];
	sprintf(sFilePath, "%s%s", path, model3d);

	this->path = new char[256];
	sprintf(this->path, "%s", path);
	this->model3d = new char[256];
	sprintf(this->model3d, "%s", model3d);

	printf("Loading: %s\n", sFilePath);

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(sFilePath, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);

	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		meshes.push_back(GLMesh3D(i, gl_index++, scene));
	}

	this->numMeshes = scene->mNumMeshes;
}

int GLMeshHandler::render(void)
{
	int verticesCount = 0;
	for (int i = 0; i < numMeshes; i++)
	{
		verticesCount += meshes.at(i).render();
	}

	return verticesCount;
}

void GLMeshHandler::prerender(int mesh)
{
	meshes.at(mesh).prerender();
}

int GLMeshHandler::render(int mesh, int startIndex, int finishIndex)
{
	GLMesh3D* meshe = &meshes.at(mesh);
	return meshe->render(startIndex, finishIndex);
}

//GLMesh3D
GLMesh3D::GLMesh3D(void)
{
	index = -1;
	max = glm::vec3(-1.0f);
	min = glm::vec3(-1.0f);
	center = glm::vec3(-1.0f);

	hasNormals = false;
	verticesCount = 0;
}

GLMesh3D::GLMesh3D(std::vector<glm::vec3>* vertexes, std::vector<glm::vec3>* normals)
{
	index = 1;
	max = glm::vec3(-1.0f);
	min = glm::vec3(-1.0f);
	center = glm::vec3(-1.0f);

	hasNormals = true;
	verticesCount = vertexes->size();

	this->vertexes = new glm::vec3[vertexes->size()];
	this->normals = new glm::vec3[normals->size()];

	for (int i = 0; i < verticesCount; i++)
	{
		this->vertexes[i] = vertexes->at(i);
		this->normals[i] = normals->at(i);
	}
}

GLMesh3D::GLMesh3D(int index, int glindex, const aiScene* scene)
{
	aiMesh* mesh = scene->mMeshes[index];
	this->index = glindex;

	hasNormals = mesh->HasNormals();
	printf("Has %d normals\n", hasNormals);

	verticesCount = mesh->mNumFaces * 3;

	vertexes = new glm::vec3[verticesCount];
	normals = new glm::vec3[verticesCount];
	uvs = new glm::vec2[verticesCount];

	max = glm::vec3(MIN_FLOAT, MIN_FLOAT, MIN_FLOAT);
	min = glm::vec3(MAX_FLOAT, MAX_FLOAT, MAX_FLOAT);

	int k = 0;
	for (int i = 0; i < verticesCount / 3; i++)
	{
		aiFace* face = &mesh->mFaces[i];
		/*if(hasNormals)
		{
		aiVector3D* norm = &mesh->mNormals[i];
		normals[i] = glm::vec3(norm->x, norm->y, norm->z);
		}*/

		for (int j = 0; j < face->mNumIndices; j++)
		{
			aiVector3D* vec = &mesh->mVertices[face->mIndices[j]];
			aiVector3D* uv = &mesh->mTextureCoords[0][face->mIndices[j]];

			vertexes[k] = glm::vec3(vec->x, vec->y, vec->z);

			//Pegar os maiores e menores
#pragma region buscar valores max e min
			if (vec->x > max.x)
			{
				max.x = vec->x;
			}
			if (vec->y > max.y)
			{
				max.y = vec->y;
			}
			if (vec->z > max.z)
			{
				max.z = vec->z;
			}

			if (vec->x < min.x)
			{
				min.x = vec->x;
			}
			if (vec->y < min.y)
			{
				min.y = vec->y;
			}
			if (vec->z < min.z)
			{
				min.z = vec->z;
			}
#pragma endregion

			//if(hasNormals)
			//{
			//	aiVector3D* norm = &mesh->mNormals[face->mIndices[j]];
			//	normals[k] = glm::vec3(norm->x, norm->y, norm->z);
			//}

			k++;
		}
	}

	center = glm::vec3(min.x + (max.x - min.x) / 2.0f, min.y + (max.y - min.y) / 2.0f, min.z + (max.z - min.z) / 2.0f);

	int walls = 0;
	int j = 0;
	for (int i = 0; i < verticesCount; i += 3)
	{
		glm::vec3 a = glm::vec3(vertexes[i]);
		a -= vertexes[i + 1];
		glm::vec3 b = glm::vec3(vertexes[i + 2]);
		b -= vertexes[i + 1];

		glm::vec3 normal = glm::normalize(glm::cross(b, a));

		if (normal.y == 0)
		{
			walls++;


		}

		normals[j] = normal;
		normals[j + 1] = normal;
		normals[j + 2] = normal;

		j += 3;
	}

	hasNormals = true;
	printf("Walls count: %d\n", walls);
}

GLMesh3D::~GLMesh3D(void)
{ }

int GLMesh3D::render(void)
{
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertexes);

	if (hasNormals)
	{
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(OpenGLWrapper::normalLoc, 3, GL_FLOAT, GL_FALSE, 0, normals);
	}
	glDrawArrays(GL_TRIANGLES, 0, verticesCount);

	return verticesCount;
}

void GLMesh3D::prerender(void)
{
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertexes);

	if (hasNormals)
	{
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(OpenGLWrapper::normalLoc, 3, GL_FLOAT, GL_FALSE, 0, normals);
	}
}

int GLMesh3D::render(int startIndex, int finishIndex)
{
	glDrawArrays(GL_TRIANGLES, startIndex, finishIndex - startIndex + 1);

	return finishIndex - startIndex + 1;
}

int GLMesh3D::getMemory(void)
{
	int memory = 0;
	memory += sizeof(index) + sizeof(max) + sizeof(min) + sizeof(center) + sizeof(bool) + sizeof(int);

	//Vezes 3 porque � UV Normals e Vertexes
	memory += verticesCount*sizeof(glm::vec3) * 3;

	return memory;
}