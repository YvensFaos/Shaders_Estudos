#include "gloctreeede.h"

#include "glconfig.h"

GLOctreeEDE::GLOctreeEDE(void)
{ }

GLOctreeEDE::~GLOctreeEDE(void)
{ }

void GLOctreeEDE::loadEDE(GLConfig* config) 
{

}

void GLOctreeEDE::renderEDE(GLFrustum* frustum, GLMeshHandler* handler, GLConfig* config) 
{

}

void GLOctreeEDE::calculateEDE(GLMeshHandler* handler, GLConfig* config) 
{
	octree = GLOctree(handler, config->edeDepth);
}

void GLOctreeEDE::exportEDE(GLConfig* config) 
{
}

void GLOctreeEDE::calculateMemory(void) 
{
}