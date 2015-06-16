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

	this->batchFilename = new char[128];
	sprintf(this->batchFilename, "%s", batchFilename);

	batchTestSize = 0;
}

GLBatch::~GLBatch(void)
{ }

/**
===== Exemplo de Batch

#Teste de batch
#repeated:1
#resolution:800 600
#camera:60.0 0.1 250.0
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

void GLBatch::getTestBatch(std::vector<GLConfig>* configurations)
{
	char file[256];
	sprintf(file, "%s%s%s", this->batchPath, this->batchFilename, BATCH_TYPE);

	EDFileReader reader = EDFileReader(file);
	char* line;
	std::string copy;

	int i = 0;

	//Inicialização de variáveis
	int pos = 0;
	int scenario = 0;
	int width = 0;
	int height = 0;
	float fov = 0;
	float near = 0;
	float far = 0;
	int repeated = 0;
	int logidentifier = 0;
	bool logresults = false;
	EDE_TYPE ede = NONE;
	int ededepth = 1;
	bool coloredNodes = false;
	PLAYER_MODE mode = WALKTHROUGH_MODE;

	while(!reader.eof())
	{
#pragma region leitura dos dados
		line = reader.readLnStr();
		//Nome do teste para a janela
		char testName[128];
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

		//Configurações da câmera
		fov = 0;
		near = 0;
		far = 0;
		line = reader.readLnStr();
		sscanf(line, "#camera:%f %f %f", &fov, &near, &far);

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

		//Nome do arquivo do obj do cenário
		line = reader.readLnStr();
		copy = std::string(line);
		pos = copy.find_first_of(':');
		copy = copy.substr(pos + 1, copy.length());
		char* objectName = new char[copy.size() + 1];
		std::copy(copy.begin(), copy.end(), objectName);
		objectName[copy.size()] = '\0';

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
		int lr = 0;
		line = reader.readLnStr();
		sscanf(line, "#logresult:%d", &lr);
		logresults = (lr == 0) ? false : true;

		//Identificador do arquivo de log (padrão é 0)
		line = reader.readLnStr();
		sscanf(line, "#logidentifier:%d", &logidentifier);

		//Texto adicional para o arquivo de log (para o nome do arquivo)
		line = reader.readLnStr();
		copy = std::string(line);
		pos = copy.find_first_of(':');
		copy = copy.substr(pos + 1, copy.length());
		char* logExtraMsg = new char[copy.size() + 1];
		std::copy(copy.begin(), copy.end(), logExtraMsg);
		logExtraMsg[copy.size()] = '\0';

		//Busca o tipo da EDE a ser testada
		char edename[128];
		line = reader.readLnStr();
		sscanf(line, "#edetype:%s", edename);
		ede = getEdetype(edename);

		//Altura da estrutura a ser testada (Default = 1)
		ededepth = 1; 
		line = reader.readLnStr();
		sscanf(line, "#ededepth:%d", &ededepth);

		//Nós coloridos
		int cn = 0;
		line = reader.readLnStr();
		sscanf(line, "#colorednodes:%d", &cn);
		coloredNodes = (cn == 0) ? false : true;

		char modename[128];
		line = reader.readLnStr();
		sscanf(line, "#mode:%s", modename);
		mode = getPlayermode(modename);
#pragma endregion
		
		//Para garantir que haverá pelo menos 1 execução desse batch
		repeated = (repeated <= 0)? 1 : repeated;
		for(int i = 0; i < repeated; i++)
		{
			configurations->push_back(GLConfig());
			GLConfig* pointer = &configurations->at(configurations->size() - 1);
			pointer->scenarioNumber = scenario;
			pointer->width = width;
			pointer->height = height;
			pointer->fov = fov;
			pointer->near = near;
			pointer->far = far;
			//Identificador é 10* o identificador + i, sendo i a iteração
			pointer->logIdentifier = logidentifier*10 + i;
			pointer->edeDepth = ededepth;
			pointer->logResults = logresults;
			pointer->mode = mode;
			pointer->type = ede;

			//Copiando as strings agora
			pointer->title = new char[128];
			sprintf(pointer->title,   "%s", testName);
			pointer->objectName = new char[128];
			sprintf(pointer->objectName,   "%s", objectName);
			pointer->objectPath = new char[128];
			sprintf(pointer->objectPath,   "%s", objectPath);
			pointer->pathfilePath = new char[128];
			sprintf(pointer->pathfilePath, "%s", pathfilePath);
			pointer->pathfileName = new char[128];
			sprintf(pointer->pathfileName, "%s", pathfileName);
			pointer->logPath = new char[128];
			sprintf(pointer->logPath,      "%s", logPath);
			pointer->logExtraMsg = new char[128];
			sprintf(pointer->logExtraMsg,  "%s", logExtraMsg);
		}
	}
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

PLAYER_MODE GLBatch::getPlayermode(char* modetype)
{
	char f = modetype[0];
	switch (f)
	{
	case 'w': 
		return WALKTHROUGH_MODE;
	case 'e':
		return EDE_MAKER;
	case 'f':
		return FREE_MODE;
	default:
		break;
	}
}