#ifndef __GL_MESH_3D__
#define __GL_MESH_3D__

#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

#include <vector>

#include "glm\glm.hpp"

typedef unsigned int GLuint;

struct VertexData {
	glm::vec3 position; //Posição
	glm::vec3 normal;   //Normal
	glm::vec3 tangent;  //Reflection
	glm::vec3 color;    //Vertex Color
	glm::vec2 uv;       //UV
};

class GLMesh3D
{
public:
	GLuint index;

	glm::vec3 max;
	glm::vec3 min;
	glm::vec3 center;

	char* model3d;
	char* path;

	bool hasNormals;
	int verticesCount;

	char* trifaces;

	glm::vec3* vertexes;
	glm::vec3* normals;
	glm::vec2* uvs;
public:
	GLMesh3D(int index, int glindex, const aiScene* scene);
	~GLMesh3D(void);
	
	void resetTrifaces(void);
	void render(void);
	void prerender(void);
	void render(int startIndex, int finishIndex);
private: 
	bool loaded; 
};

class GLMeshHandler
{
public:
	int numMeshes;

	static GLuint gl_index;

	char* model3d;
	char* path;

	std::vector<GLMesh3D> meshes;
public:
	GLMeshHandler(void);
	GLMeshHandler(char* model3d, char* path);
	~GLMeshHandler(void);

	void resetTrifaces(void);
	void render(void);
	void prerender(int mesh);
	void render(int mesh, int startIndex, int finishIndex);
private:
	bool loaded;
};
#endif