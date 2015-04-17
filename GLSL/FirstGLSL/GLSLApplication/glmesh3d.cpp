#include "glmesh3d.h"

#include "IL\il.h"

#include <GL\glew.h>
#include "GLFW\glfw3.h"

#include "openGLWrapper.h"

GLMesh3D::GLMesh3D(char* model3d, char* path)
{
	char sFilePath[512];
	sprintf(sFilePath, "%s%s", path, model3d);

	this->path = new char[256];
	sprintf(this->path, "%s", path);
	this->model3d = new char[256];
	sprintf(this->model3d, "%s", model3d);

	Assimp::Importer importer; 
	const aiScene* scene = importer.ReadFile( sFilePath, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices  | aiProcess_SortByPType); 
 
	verticesCount = scene->mMeshes[0]->mNumFaces * 3;
	vertexes = new glm::vec3[verticesCount];
	normals = new glm::vec3[verticesCount / 3];

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
			vertexes[k++] = glm::vec3(vec->x, vec->y, vec->z);
		}
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