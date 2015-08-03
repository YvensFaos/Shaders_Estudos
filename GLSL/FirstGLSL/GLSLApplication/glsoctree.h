#pragma once

#include "gloctree.h"

class GLSOctree
{
public:
	EDLogger* logger;
	GLOctreeNode root;
	int memoryUsed;

	GLMesh3D staticMesh;
	std::vector<GLMesh3D*> testedMeshes;
public:
	GLSOctree(void);
	GLSOctree(GLMeshHandler* handler, int depth, EDLogger* logger);
	~GLSOctree(void);

	int getMemory(void);
private:
	void createNodeMeshes(GLMeshHandler* handler);
	void logTree(int verticesCount);
};