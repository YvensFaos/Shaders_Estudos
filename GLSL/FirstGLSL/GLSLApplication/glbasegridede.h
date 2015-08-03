#pragma once

#include "glbasicede.h"
#include "glbasegrid.h"

class GLBaseGridEDE : public GLBasicEDE
{
private:
	GLBaseGrid grid;
	int edeDepth;

public:
	GLBaseGridEDE(void);
	~GLBaseGridEDE(void);
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
	std::string getName(void);
private:
	void calculateMemory(void);
};