#ifndef __GL_MESH_3D__
#define __GL_MESH_3D__

#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

#include <vector>
#include <map>

#include "glm\glm.hpp"

typedef unsigned int GLuint;

class GLMesh3D
{
public:
	char* model3d;
	char* path;

	bool hasNormals;
	int verticesCount;
	glm::vec3* vertexes;
	glm::vec3* normals;
public:
	GLMesh3D(char* model3d, char* path);
	~GLMesh3D(void);

	void render(void);
private: 
	bool loaded; 
	std::map<std::string, GLuint> textureIdMap;	
};

#endif