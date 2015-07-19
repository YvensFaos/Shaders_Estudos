#pragma once

#include "glmesh3d.h"
#include "edlogger.h"

#define NODE_COUNT 36

class GLBaseGridNode
{
public:
	glm::vec3 min;
	glm::vec3 max;

	glm::vec3 nodeColor;

	//Usado por alguns algoritmos
	VISIBILITY_STATUS visible;

	int numMeshes;
	int numIndicesTotal;
	std::vector<int>* indexes;
public:
	GLBaseGridNode(void);
	GLBaseGridNode(glm::vec3 min, glm::vec3 max, GLMeshHandler* handler, EDLogger* logger);
	~GLBaseGridNode(void);

	int getMemory(void);
};

class GLBaseGrid
{
public:
	EDLogger* logger;
	GLBaseGridNode* nodes;
	int memoryUsed;

	int nodesCount;
public:
	GLBaseGrid(void);
	GLBaseGrid(GLMeshHandler* handler, EDLogger* logger);
	~GLBaseGrid(void);

	int getMemory(void);
private:
	void logStructure(void);
};
