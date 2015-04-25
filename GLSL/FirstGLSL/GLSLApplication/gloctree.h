#pragma once

#include "glmesh3d.h"

class GLOctreeNode
{
public:
	glm::vec3 min;
	glm::vec3 max;

	GLOctreeNode* nodes;

	std::vector<int>* indexes;
public:
	GLOctreeNode(void);
	GLOctreeNode(glm::vec3 min, glm::vec3 max, GLMeshHandler* handler, int depth);
	GLOctreeNode(glm::vec3 min, glm::vec3 max, GLMeshHandler* handler, int depth, std::vector<int>** previousIndexes);
	~GLOctreeNode(void);
};

class GLOctree
{
public:
	GLOctreeNode root;

public:
	GLOctree(void);
	GLOctree(GLMeshHandler* handler, int depth);
	~GLOctree(void);
};