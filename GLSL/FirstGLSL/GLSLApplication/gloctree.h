#pragma once

#include "glmesh3d.h"
#include "edlogger.h"

class GLOctreeNode
{
public:
	glm::vec3 min;
	glm::vec3 max;

	glm::vec3 nodeColor;

	bool hasNodes;
	std::vector<GLOctreeNode> nodes;

	int numMeshes;
	std::vector<int>* indexes;
	std::vector<glm::vec3> vertexes;
	std::vector<glm::vec3> normals;
public:
	GLOctreeNode(void);
	GLOctreeNode(glm::vec3 min, glm::vec3 max, GLMeshHandler* handler, int depth, std::vector<int>* previousIndexes, EDLogger* logger);
	~GLOctreeNode(void);

	int getMemory(void);
	void optimizeNode(EDLogger* logger);
};

class GLOctree
{
public:
	EDLogger* logger;
	GLOctreeNode root;
	int memoryUsed;

public:
	GLOctree(void);
	GLOctree(GLMeshHandler* handler, int depth, EDLogger* logger);
	~GLOctree(void);

	int getMemory(void);
private: 
	void logTree(void);
	void optimizeTree(void);
};