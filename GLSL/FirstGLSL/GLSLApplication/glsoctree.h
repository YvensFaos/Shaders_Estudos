#pragma once

#include "gloctree.h"

#define STATIC_THRESHOLD 210

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
	void createNodeMeshes(GLMeshHandler* handler);
private:
	void logTree(void);
};