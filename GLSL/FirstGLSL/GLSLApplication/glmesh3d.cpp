#include "glmesh3d.h"

#include "IL\il.h"

#include <GL\glew.h>
#include "GLFW\glfw3.h"

#include "openGLWrapper.h"
#include "glprinthelper.h"

GLuint GLMeshHandler::gl_index = 0;

//GLMeshHandler
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
	const aiScene* scene = importer.ReadFile(sFilePath, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices  | aiProcess_SortByPType); 

	for(int i = 0; i < scene->mNumMeshes; i++)
	{
		meshes.push_back(GLMesh3D(i, gl_index++, scene));
	}

	this->numMeshes = scene->mNumMeshes;
}

void GLMeshHandler::render(void)
{
	for(int i = 0; i < numMeshes; i++)
	{
		meshes.at(i).render();
	}
}

//GLMesh3D

GLMesh3D::GLMesh3D(int index, int glindex, const aiScene* scene)
{
	aiMesh* mesh = scene->mMeshes[index];
	this->index = glindex;

	hasNormals = mesh->HasNormals();
	printf("Has %d normals\n", hasNormals);

	verticesCount = mesh->mNumFaces * 3;

	vertexes = new glm::vec3[verticesCount];
	normals =  new glm::vec3[verticesCount];
	uvs =      new glm::vec2[verticesCount];

	int k = 0;
	for(int i = 0; i < verticesCount /3; i++)
	{
		aiFace* face = &mesh->mFaces[i];
		if(hasNormals)
		{
			aiVector3D* norm = &mesh->mNormals[i];
			normals[i] = glm::vec3(norm->x, norm->y, norm->z);
		}

		for(int j = 0; j < face->mNumIndices; j++)
		{
			aiVector3D* vec = &mesh->mVertices[face->mIndices[j]];
			aiVector3D* uv = &mesh->mTextureCoords[0][face->mIndices[j]];

			vertexes[k] = glm::vec3(vec->x, vec->y, vec->z);

			if(hasNormals)
			{
				aiVector3D* norm = &mesh->mNormals[face->mIndices[j]];
				normals[k] = glm::vec3(norm->x, norm->y, norm->z);
			}

			k++;
		}
	}

	if(!hasNormals)
	{
		//Calculate
		int j = 0;
		for(int i = 0; i < verticesCount; i += 3)
		{
			glm::vec3 a = glm::vec3(vertexes[i]);
			a -= vertexes[i + 1];
			glm::vec3 b = glm::vec3(vertexes[i + 2]);
			b -= vertexes[i + 1];

			normals[j]     = glm::normalize(glm::cross(a,b));
			normals[j + 1] = glm::normalize(glm::cross(a,b));
			normals[j + 2] = glm::normalize(glm::cross(a,b));

			j += 3;
		}

		hasNormals = true;
	}
}

GLMesh3D::~GLMesh3D(void)
{
}

void GLMesh3D::render(void)
{
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertexes);

	if(hasNormals)
	{
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(OpenGLWrapper::normalLoc, 3, GL_FLOAT, GL_FALSE, 0, normals);
	}
	glDrawArrays(GL_TRIANGLES, 0, verticesCount);
}