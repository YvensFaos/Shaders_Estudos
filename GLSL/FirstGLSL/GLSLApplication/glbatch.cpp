#include "glbatch.h"

#include "edfile.h"

#include <string.h>

GLBatch::GLBatch(void)
{
	batchTestSize = 0;
}

GLBatch::GLBatch(char* batchPath, char* batchFilename)
{
	this->batchPath = new char[128];
	sprintf(this->batchPath, "%s", batchPath);

	this->bacthFilename = new char[128];
	sprintf(this->bacthFilename, "%s", bacthFilename);

	batchTestSize = 0;
}

GLBatch::~GLBatch(void)
{ }

/**
===== Exemplo de Batch

#Teste de batch
#repeated:1
#resolution:800 600
#scenario:18
#objectPath:E:/Repositorios/Shaders_Estudos/Models/
#pathfilePath:E:/Repositorios/Shaders_Estudos/Paths/
#pathfileName:goldrush--[1][test]
#logPath:E:/Repositorios/Shaders_Estudos/Logs/Log Desktop/
#logresult:1
#logidentifier:1
#logextra:n
#edetype:octree
#ededepth:3
#colorednodes:18
#mode:walkthrough

*/

std::vector<GLConfig> GLBatch::getTestBatch(void)
{
	char file[256];
	sprintf(file, "%s%s%s", this->batchPath, this->bacthFilename, BATCH_TYPE);

	EDFileReader reader = EDFileReader(file);
	char* line;
	std::string copy;

	std::vector<GLConfig> configurations;
	int i = 0;

	//Inicialização de variáveis
	int pos = 0;
	int scenario = 0;
	int width = 0;
	int height = 0;
	int repeated = 0;
	int logidentifier = 0;
	bool logresults = false;
	EDE_TYPE ede = NONE;
	int ededepth = 1;
	bool coloredNodes = false;
	PLAYER_MODE mode = WALKTHROUGH_MODE;

	while(!reader.eof())
	{
		line = reader.readLnStr();

		//Nome do teste para a janela
		char testName[256];
		sprintf(testName, "%s", line);

		//Quantidade de repetições
		repeated = 0;
		line = reader.readLnStr();
		sscanf(line, "#repeated:%d", &repeated);

		//Resolução da janela
		width = 0;
		height = 0;
		line = reader.readLnStr();
		sscanf(line, "#resolution:%d %d", &width, &height);

		//Numeração do cenário
		scenario = 0;
		line = reader.readLnStr();
		sscanf(line, "#scenario:%d", &scenario);

		//Path para o obj do cenário
		line = reader.readLnStr();
		copy = std::string(line);
		pos = copy.find_first_of(':');
		copy = copy.substr(pos + 1, copy.length());
		char* objectPath = new char[copy.size() + 1];
		std::copy(copy.begin(), copy.end(), objectPath);
		objectPath[copy.size()] = '\0';

		//Path para o arquivo de path
		line = reader.readLnStr();
		copy = std::string(line);
		pos = copy.find_first_of(':');
		copy = copy.substr(pos + 1, copy.length());
		char* pathfilePath = new char[copy.size() + 1];
		std::copy(copy.begin(), copy.end(), pathfilePath);
		pathfilePath[copy.size()] = '\0';

		//Nome do arquivo de path
		line = reader.readLnStr();
		copy = std::string(line);
		pos = copy.find_first_of(':');
		copy = copy.substr(pos + 1, copy.length());
		char* pathfileName = new char[copy.size() + 1];
		std::copy(copy.begin(), copy.end(), pathfileName);
		pathfileName[copy.size()] = '\0';

		//Path para o arquivo de log
		line = reader.readLnStr();
		copy = std::string(line);
		pos = copy.find_first_of(':');
		copy = copy.substr(pos + 1, copy.length());
		char* logPath = new char[copy.size() + 1];
		std::copy(copy.begin(), copy.end(), logPath);
		logPath[copy.size()] = '\0';

		//Se deve fazer o log dos resultados
		line = reader.readLnStr();
		sscanf(line, "#logresult:%d", &logresults);

		//Identificador do arquivo de log (padrão é 0)
		line = reader.readLnStr();
		sscanf(line, "#logidentifier:%d", &logidentifier);

		//Texto adicional para o arquivo de log (para o nome do arquivo)
		line = reader.readLnStr();
		copy = std::string(line);
		pos = copy.find_first_of(':');
		copy = copy.substr(pos + 1, copy.length());
		char* logExtra = new char[copy.size() + 1];
		std::copy(copy.begin(), copy.end(), logExtra);
		logExtra[copy.size()] = '\0';

		char edename[128];
		line = reader.readLnStr();
		sscanf(line, "#edetype:%s", edename);
		ede = getEdetype(edename);


	}

	return configurations;
}

EDE_TYPE GLBatch::getEdetype(char* edename)
{
	char f = edename[0];
	switch (f)
	{
	case 'o':
		return OCTREE;
	case 's':
		return SOCTREE;
	case 'r':
		return ROCTREE;
	default:
		break;
	}

	return NONE;
}