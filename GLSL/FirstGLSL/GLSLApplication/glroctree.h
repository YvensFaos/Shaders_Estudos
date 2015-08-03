#pragma once

#include "gloctree.h"

class GLROctree
{
public:
	EDLogger* logger;
	GLOctreeNode root;
	int memoryUsed;

	GLMesh3D staticMesh;
	std::vector<GLMesh3D*> testedMeshes;
public:
	GLROctree(void);
	GLROctree(GLMeshHandler* handler, int depth, EDLogger* logger);
	~GLROctree(void);

	int getMemory(void);
private:
	void createNodeMeshes(GLMeshHandler* handler);
	void logTree(int verticesCount);
};