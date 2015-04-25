#pragma once

#include "glbasicede.h"

class GLOctreeEDE : public GLBasicEDE
{
public:
	GLOctreeEDE(void);
	~GLOctreeEDE(void);

public:
	//Carregar a EDE de um arquivo
	void loadEDE(GLConfig* config);
	//Chamar os métodos de draw da mesh de acordo com a EDE
	void renderEDE(GLFrustum* frustum, GLMeshHandler* handler, GLConfig* config);
	//Calcula a EDE para ser utilizada
	void calculateEDE(GLMeshHandler* handler, GLConfig* config);
	//Salva a EDE calculada em arquivo
	void exportEDE(GLConfig* config);

private:
	void calculateMemory(void);
};