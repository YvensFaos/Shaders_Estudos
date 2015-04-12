#ifndef __OBJ_READER__
#define __OBJ_READER__

#include <vector>

#include "glm\vec2.hpp"
#include "glm\vec3.hpp"

class ObjReader
{
public:
	ObjReader(void);
	~ObjReader(void);

	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals; 
public:
	void loadObj(char* path);
};

#endif