#pragma once

#include "glbasicede.h"
#include "glroctree.h"

class GLROctreeEDE : public GLBasicEDE
{
private:
	GLROctree octree;
	int edeDepth;

public:
	GLROctreeEDE(void);
	~GLROctreeEDE(void);

public:
	//Carregar a EDE de um arquivo
	void loadEDE(GLConfig* config);
	//Chamar os métodos de draw da mesh de acordo com a EDE
	void renderEDE(GLFrustum* frustum, GLMeshHandler* handler, GLConfig* config, float* info);
	//Calcula a EDE para ser utilizada
	void calculateEDE(GLMeshHandler* handler, GLConfig* config);
	//Salva a EDE calculada em arquivo
	void exportEDE(GLConfig* config);
	//Retorna o nome específico da ede
	char* getName(char* name);
private:
	void calculateMemory(void);
	VISIBILITY_STATUS checkVisibility(GLFrustum* frustum, GLOctreeNode* node, float* info);
	void recursiveDraw(GLConfig* config, GLOctreeNode* node, float* info);
};