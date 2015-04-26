#pragma once

class GLFrustum;
class GLConfig;
class GLMeshHandler;
class EDLogger;

class GLBasicEDE
{
protected:
	int memoryUsed;
	EDLogger* logger;

public:
	GLBasicEDE(void);
	virtual ~GLBasicEDE(void) {};

public:
	//Carregar a EDE de um arquivo
	virtual void loadEDE(GLConfig* config) = 0;
	//Chamar os m�todos de draw da mesh de acordo com a EDE
	virtual void renderEDE(GLFrustum* frustum, GLMeshHandler* handler, GLConfig* config) = 0;
	//Calcula a EDE para ser utilizada
	virtual void calculateEDE(GLMeshHandler* handler, GLConfig* config) = 0;
	//Salva a EDE calculada em arquivo
	virtual void exportEDE(GLConfig* config) = 0;
	//Calcula o custo individual da estrutura
	int getMemory(void);
	//Seta um logger para a gera��o e funcionamento da ede
	void setLogger(EDLogger* logger);
	//Retorna o nome espec�fico da ede
	virtual char* getName(void) = 0;
private:
	virtual void calculateMemory(void) = 0;

public:
	//Instancia a EDE correspondente ao tipo especificado nas configura��es
	static GLBasicEDE* instantiate(GLConfig* config);
};