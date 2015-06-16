#pragma once

#include "glenums.h"
#include "gldynamic.h"

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

	char* title;

	bool repeatable;

	//Path dos arquivos (modelos) a serem lidos
	char* objectPath;
	//Nome do objeto que será carregado
	char* objectName;
	//Número do cenário
	int scenarioNumber;
	//Path dos arquivos de path
	char* pathfilePath;
	//Nome do arquivo de path
	char* pathfileName;
	//Identificador único de path para a gravação de novos paths
	int pathIdentifier;
	//Mensagem extra para o path, caso seja necessário (Para o nome do arquivo)
	char* pathExtraMsg;
	//Path dos arquivos de log
	char* logPath;
	//Identificador único de log
	int logIdentifier;
	//Mensagem extra para o log, caso seja necessário (Para o nome do arquivo)
	char* logExtraMsg;
	//Se salvará o log de resultados (total). Apenas para modo Walkthrough
	bool logResults;
	//Profundidade padrão da ede
	int edeDepth;
	//Colore diferente os triângulos pertencentes a um nó da EDE
	bool coloredNodes;
	//Habilita os dinâmicos
	bool enableDynamics;
	//Habilita o teste de dinâmicos na EDE
	bool edeTestDynamics;
	//Habilita o teste de dinâmicos no frustum
	bool frustumTestDynamics;

	//Lista de objetos dinâmicos
	std::vector<GLDynamicObject> dynamics;
public:
	GLConfig(void);
	~GLConfig(void);

	GLPlayer* getGLPlayer(void);
	GLPlayer* getGLPlayer(PLAYER_MODE mode);
};