#include "glmesh3d.h"

#include "IL\il.h"

#include <GL\glew.h>
#include "GLFW\glfw3.h"

#include "openGLWrapper.h"
#include "glprinthelper.h"

GLMesh3D::GLMesh3D(char* model3d, char* path)
{
	char sFilePath[512];
	sprintf(sFilePath, "%s%s", path, model3d);

	this->path = new char[256];
	sprintf(this->path, "%s", path);
	this->model3d = new char[256];
	sprintf(this->model3d, "%s", model3d);

	Assimp::Importer importer; 
	const aiScene* scene = importer.ReadFile(sFilePath, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices  | aiProcess_SortByPType); 
 
	verticesCount = scene->mMeshes[0]->mNumFaces * 3;
	vertexes = new glm::vec3[verticesCount];
	normals = new glm::vec3[verticesCount];

	hasNormals = scene->mMeshes[0]->HasNormals();
	printf("Has %d normals\n", hasNormals);
	int k = 0;
	for(int i = 0; i < verticesCount /3; i++)
	{
		aiFace* face = &scene->mMeshes[0]->mFaces[i];
		if(hasNormals)
		{
			aiVector3D* norm = &scene->mMeshes[0]->mNormals[i];
			normals[i] = glm::vec3(norm->x, norm->y, norm->z);
		}

		for(int j = 0; j < face->mNumIndices; j++)
		{
			aiVector3D* vec = &scene->mMeshes[0]->mVertices[face->mIndices[j]];
			vertexes[k] = glm::vec3(vec->x, vec->y, vec->z);

			if(hasNormals)
			{
				aiVector3D* norm = &scene->mMeshes[0]->mNormals[face->mIndices[j]];
				normals[k] = glm::vec3(norm->x, norm->y, norm->z);
			}

			k++;
		}
	}

	if(hasNormals)
	{
		PRINT_VEC(vertexes[0]);
		PRINT_VEC(vertexes[1]);
		PRINT_VEC(vertexes[2]);
		PRINT_VEC(normals[0]);

		//Testar

		glm::vec3 a = glm::vec3(vertexes[0]);
		a -= vertexes[1];
		glm::vec3 b = glm::vec3(vertexes[2]);
		b -= vertexes[1];

		a = glm::cross(a,b);

		PRINT_VEC(a);
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