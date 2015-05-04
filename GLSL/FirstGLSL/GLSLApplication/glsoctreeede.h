#pragma once

#include "glbasicede.h"
#include "glsoctree.h"

class GLSOctreeEDE : public GLBasicEDE
{
private:
	GLSOctree octree;
	int edeDepth;

public:
	GLSOctreeEDE(void);
	~GLSOctreeEDE(void);

public:
	//Carregar a EDE de um arquivo
	void loadEDE(GLConfig* config);
	//Chamar os m�todos de draw da mesh de acordo com a EDE
	void renderEDE(GLFrustum* frustum, GLMeshHandler* handler, GLConfig* config, float* info);
	//Calcula a EDE para ser utilizada
	void calculateEDE(GLMeshHandler* handler, GLConfig* config);
	//Salva a EDE calculada em arquivo
	void exportEDE(GLConfig* config);
	//Retorna o nome espec�fico da ede
	char* getName(char* name);
private:
	void calculateMemory(void);
};