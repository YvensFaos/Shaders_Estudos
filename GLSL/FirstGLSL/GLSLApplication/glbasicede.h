#pragma once

#include <string>

class GLFrustum;
class GLConfig;
class GLMeshHandler;
class EDLogger;

#define OCTREE_NAME  "Octree\0"
#define SOCTREE_NAME "Smart Octree\0"
#define ROCTREE_NAME "Replicated Octree\0"
#define BASEGRID_NAME "Base Grid\0"

class GLBasicEDE
{
protected:
	int memoryUsed;
	EDLogger* logger;

public:
	bool testDynamics;

public:
	GLBasicEDE(void);
	virtual ~GLBasicEDE(void) {};

public:
	//Carregar a EDE de um arquivo
	virtual void loadEDE(GLConfig* config) = 0;
	//Chamar os métodos de draw da mesh de acordo com a EDE
	virtual void renderEDE(GLFrustum* frustum, GLMeshHandler* handler, GLConfig* config, float* info) = 0;
	//Calcula a EDE para ser utilizada
	virtual void calculateEDE(GLMeshHandler* handler, GLConfig* config) = 0;
	//Salva a EDE calculada em arquivo
	virtual void exportEDE(GLConfig* config) = 0;
	//Calcula o custo individual da estrutura
	int getMemory(void);
	//Seta um logger para a geração e funcionamento da ede
	void setLogger(EDLogger* logger);
	//Retorna o nome específico da ede
	virtual std::string getName(void) = 0;
	//Bufferiza a ede de acordo com seu nome
	void bufferizeEDE(GLConfig* config);
private:
	virtual void calculateMemory(void) = 0;

public:
	//Instancia a EDE correspondente ao tipo especificado nas configurações
	static GLBasicEDE* instantiate(GLConfig* config);
};