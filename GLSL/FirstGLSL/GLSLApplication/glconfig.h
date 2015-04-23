#pragma once

#include "glenums.h"

class GLPlayer;

class GLConfig
{
public:
	int height;
	int width;

	PLAYER_MODE mode;

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

public:
	GLConfig(void);
	~GLConfig(void);

	GLPlayer* getGLPlayer(PLAYER_MODE mode);
};