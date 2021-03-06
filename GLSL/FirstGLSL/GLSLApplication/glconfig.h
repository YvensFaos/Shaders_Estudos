#pragma once

#include "glenums.h"
#include "gldynamic.h"
#include "glocclussion.h"

class GLPlayer;

class GLConfig
{
public:
	int height;
	int width;

	float aspect;

	float fov;
	float near;
	float far;

	PLAYER_MODE mode;
	EDE_TYPE type;
	OCCLUSION_TYPE occlusion;

	char* title;

	bool repeatable;

	//Path dos arquivos (modelos) a serem lidos
	char* objectPath;
	//Nome do objeto que ser� carregado
	char* objectName;
	//N�mero do cen�rio
	int scenarioNumber;
	//Path dos arquivos de path
	char* pathfilePath;
	//Nome do arquivo de path
	char* pathfileName;
	//Identificador �nico de path para a grava��o de novos paths
	int pathIdentifier;
	//Mensagem extra para o path, caso seja necess�rio (Para o nome do arquivo)
	char* pathExtraMsg;
	//Path dos arquivos de log
	char* logPath;
	//Identificador �nico de log
	int logIdentifier;
	//Mensagem extra para o log, caso seja necess�rio (Para o nome do arquivo)
	char* logExtraMsg;
	//Se salvar� o log de resultados (total). Apenas para modo Walkthrough
	bool logResults;
	//Profundidade padr�o da ede
	int edeDepth;
	//Colore diferente os tri�ngulos pertencentes a um n� da EDE
	bool coloredNodes;
	//Habilita os din�micos
	bool enableDynamics;
	//Habilita o teste de din�micos na EDE
	bool edeTestDynamics;
	//Habilita o teste de din�micos no frustum
	bool frustumTestDynamics;
	//Habilita o c�lculo da aabb do frustum de acordo com as configura��es da EDE usada
	bool calculateFrustumAABB;
	//Habilita os testes de occlusion culling na estrutura
	bool occlusionCulling;


	//Lista de objetos din�micos
	std::vector<GLDynamicObject> dynamics;

	//Lista de indexes que devem ser gravados (imagens para an�lise)
	std::vector<int> recordingIndexes;
public:
	GLConfig(void);
	~GLConfig(void);

	GLPlayer* getGLPlayer(void);
	GLPlayer* getGLPlayer(PLAYER_MODE mode);

	void addIndexes(int value);
	void addIndexes(int start, int finish);
};