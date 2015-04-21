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
	//Nome do objeto que ser� carregado
	char* objectName;
	//N�mero do cen�rio
	int scenarioNumber;
	//Path dos arquivos de path
	char* pathfilePath;
	//Nome do arquivo de path
	char* pathfileName;

public:
	GLConfig(void);
	~GLConfig(void);

	GLPlayer* getGLPlayer(PLAYER_MODE mode);
};