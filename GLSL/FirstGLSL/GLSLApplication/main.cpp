#include "openGLWrapper.h"

#include "glplayer.h"
#include "glshaderloader.h"
#include "glenums.h"
#include "glbatch.h"
#include "glbuffer.h"

#pragma region define cenarios

#define SAW config.objectName = "saw.obj"; \
			config.scenarioNumber = 10; \
			config.pathfileName = "saw-[walk]-[1]";

#define CITYCYCLES config.objectName = "citycycles.obj"; \
				 config.scenarioNumber = 11; \
				 config.pathfileName = "citycycles-[walk]";

#define RAVINE config.objectName = "ravine.obj"; \
				 config.scenarioNumber = 13; \
				 config.pathfileName = "ravine-[walk]-[1]";

#define VIADUCT config.objectName= "viaduct.obj"; \
				config.scenarioNumber = 14; \
				config.pathfileName = "viaduct-[walk]-[1]";

#define COALTOWN config.objectName = "coaltown.obj"; \
				 config.scenarioNumber = 17; \
				 config.pathfileName = "coaltown-[walk]"; \
			     int qttDynamics = 10; \
				 std::vector<GLDynamicObject>* pointerDynamics; \
				 pointerDynamics = GLDynamic::generateDynamics(modelPath, "bunny.obj",  modelPathLocation, "coaltown-[1][test]-[1][dynl].pathx", qttDynamics, glm::vec3(0,0,0)); \
				 totalDynamics.insert(totalDynamics.end(), pointerDynamics->begin(), pointerDynamics->end()); \
				 pointerDynamics = GLDynamic::generateDynamics(modelPath, "bunny.obj",  modelPathLocation, "coaltown-[1][test]-[1][dynl].pathx", qttDynamics, glm::vec3(0,0,-2)); \
				 totalDynamics.insert(totalDynamics.end(), pointerDynamics->begin(), pointerDynamics->end()); \
				 pointerDynamics = GLDynamic::generateDynamics(modelPath, "bunny.obj",  modelPathLocation, "coaltown-[1][test]-[2][dynl].pathx", qttDynamics, glm::vec3(0,0,0)); \
				 totalDynamics.insert(totalDynamics.end(), pointerDynamics->begin(), pointerDynamics->end()); \
				 pointerDynamics = GLDynamic::generateDynamics(modelPath, "bunny.obj",  modelPathLocation, "coaltown-[1][test]-[3][dynl].pathx", qttDynamics, glm::vec3(0,0,0)); \
				 totalDynamics.insert(totalDynamics.end(), pointerDynamics->begin(), pointerDynamics->end()); \
				 pointerDynamics = GLDynamic::generateDynamics(modelPath, "bunny.obj",  modelPathLocation, "coaltown-[1][test]-[4][dynl].pathx", qttDynamics, glm::vec3(0,0,0)); \
				 totalDynamics.insert(totalDynamics.end(), pointerDynamics->begin(), pointerDynamics->end()); \
				 config.dynamics = totalDynamics;

#define GOLDRUSH config.objectName = "goldrush.obj"; \
				 config.scenarioNumber = 18; \
				 config.pathfileName = "goldrush-[walk]-[1]";

//Macro de teste
#define TEST_BODY(modeM,t,aa) config.coloredNodes = false; \
	config.enableDynamics = false; \
	config.edeTestDynamics = false; \
	config.frustumTestDynamics = true; \
	config.title = t; \
	PLAYER_MODE mode = modeM; \
	player = config.getGLPlayer(mode); \
	OpenGLWrapper::player = player; \
	OpenGLWrapper::initialize(true, aa); \
	OpenGLWrapper::glLoop();

#pragma endregion

#include <stdio.h>  
#include <stdlib.h>  

//#define mia
//#define desktop
#define notebook

//#define individual
//#define batch
#define recordPath

GLPlayer* player;

int main()  
{  
#ifdef individual
	GLConfig config;
	config.width  = 800;
	config.height = 600;
	config.aspect = (float) config.width/config.height;
	config.title  = "Teste";
	config.fov =  45.0f;
	config.far = 500.0f;
	config.near =  0.1f;

	std::string modelPath;
	std::string modelPathLocation;

	//Configurações de PATH que são exclusivos do computador usado
#ifdef mia
	config.objectPath = "X:/Yvens Rebouças/GIT/Shaders_Estudos/Models/";
	config.pathfilePath = "X:/Yvens Rebouças/GIT/Shaders_Estudos/Paths/";
	config.logPath = "X:/Yvens Rebouças/GIT/Shaders_Estudos/Logs/Log Mia/";
	modelPath = "X:/Yvens Rebouças/GIT/Shaders_Estudos/Models/";
	modelPathLocation = "X:/Yvens Rebouças/GIT/Shaders_Estudos/Paths/";
#endif
#ifdef desktop
	config.objectPath = "F:/Repositorios/Shaders_Estudos/Models/";
	config.pathfilePath = "F:/Repositorios/Shaders_Estudos/Paths/";
	config.logPath = "F:/Repositorios/Shaders_Estudos/Logs/Log Desktop/";
#endif
#ifdef notebook
	config.objectPath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Models/";
	config.pathfilePath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Paths/";
	config.logPath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Logs/Log Notebook/";
	modelPath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Models/";
	modelPathLocation = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Paths/";
#endif

	//Quando passa de 150 que a coisa começa a degringolar total
	std::vector<GLDynamicObject> totalDynamics = std::vector<GLDynamicObject>();
	
	//Setando a configuração pelo macro
	//COALTOWN;
	//GOLDRUSH;
	//SAW;
	//RAVINE;
	CITYCYCLES;

	config.repeatable = false;
	config.logResults = true;

	//Seta um valor inicial para o identifier; Caso seja -1, o código
	//Inicializa com 0 e começa a contagem a partir daí
	config.pathIdentifier = 1;
	//Adiciona um texto adicional ao arquivo de path
	config.pathExtraMsg = "walk";

	int numberTests = 2;
	config.logExtraMsg = "test-mia-aa[0]";
	for (int i = 0; i < numberTests; i++)
	{
		//Inicializa com 0 e deve ser mudado em tempo de execução, caso necessário
		config.logIdentifier = i + 1;
		//Adiciona um texto adicional ao arquivo de log
		//Altura padrão de uma EDE
		config.edeDepth = 3;
		//EDE escolhida
		config.type = ROCTREE;
		
		TEST_BODY(WALKTHROUGH_MODE, "teste", 0);
	}
	config.logExtraMsg = "test-mia-aa[1]";
	for (int i = 0; i < numberTests; i++)
	{
		//Inicializa com 0 e deve ser mudado em tempo de execução, caso necessário
		config.logIdentifier = i + 1;
		//Adiciona um texto adicional ao arquivo de log
		//Altura padrão de uma EDE
		config.edeDepth = 3;
		//EDE escolhida
		config.type = ROCTREE;

		TEST_BODY(WALKTHROUGH_MODE, "teste", 1);
	}
	config.logExtraMsg = "test-mia-aa[2]";
	for (int i = 0; i < numberTests; i++)
	{
		//Inicializa com 0 e deve ser mudado em tempo de execução, caso necessário
		config.logIdentifier = i + 1;
		//Adiciona um texto adicional ao arquivo de log
		//Altura padrão de uma EDE
		config.edeDepth = 3;
		//EDE escolhida
		config.type = ROCTREE;

		TEST_BODY(WALKTHROUGH_MODE, "teste", 2);
	}
	config.logExtraMsg = "test-mia-aa[0]";
	for(int i = 0; i < numberTests; i++)
	{
		//Inicializa com 0 e deve ser mudado em tempo de execução, caso necessário
		config.logIdentifier = i + 1;
		//Altura padrão de uma EDE
		config.edeDepth = 3;
		//EDE escolhida
		config.type = SOCTREE;

		TEST_BODY(WALKTHROUGH_MODE, "teste", 0);
	}
	for (int i = 0; i < numberTests; i++)
	{
		//Inicializa com 0 e deve ser mudado em tempo de execução, caso necessário
		config.logIdentifier = i + 1;
		//Altura padrão de uma EDE
		config.edeDepth = 3;
		//EDE escolhida
		config.type = OCTREE;
		
		TEST_BODY(WALKTHROUGH_MODE, "teste", 0);
	}
	for (int i = 0; i < numberTests; i++)
	{
		//Inicializa com 0 e deve ser mudado em tempo de execução, caso necessário
		config.logIdentifier = i + 1;
		//Altura padrão de uma EDE
		config.edeDepth = 3;
		//EDE escolhida
		config.type = NONE;
		
		TEST_BODY(WALKTHROUGH_MODE, "teste", 0);
	}
#endif

#ifdef batch
	char batchPath[128];

	#ifdef mia
		//TODO ajustar
		sprintf(batchPath, "F:/Repositorios/Shaders_Estudos/Batch/Batch Desktop/");
	#endif
	#ifdef desktop
		sprintf(batchPath, "F:/Repositorios/Shaders_Estudos/Batch/Batch Desktop/");
	#endif
	#ifdef notebook
		sprintf(batchPath, "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Batch/Batch Notebook/");
	#endif

	std::vector<char*> batchFiles;
	batchFiles.push_back("octrees-nos");

	GLBatch* batchFile;
	for(int i = 0; i < batchFiles.size(); i++)
	{
		batchFile = new GLBatch(batchPath, batchFiles[i]);
		std::vector<GLConfig> configurations;
		batchFile->getTestBatch(&configurations);

		while(configurations.size() > 0)
		{
			GLConfig* pointer = &configurations.at(0);

			player = pointer->getGLPlayer();
			OpenGLWrapper::player = player;
			OpenGLWrapper::initialize(true, 4);
			OpenGLWrapper::glLoop();

			configurations.erase(configurations.begin());
		}

		delete batchFile;
	}
#endif

#ifdef recordPath
	GLConfig config;
	config.width  = 800;
	config.height = 600;
	config.aspect = (float) config.width/config.height;
	config.title  = "Teste";
	config.fov =  45.0f;
	config.far = 500.0f;
	config.near =  0.1f;

	std::string modelPath;
	std::string modelPathLocation;

	//Configurações de PATH que são exclusivos do computador usado
#ifdef mia
	config.objectPath = "X:/Yvens Rebouças/GIT/Shaders_Estudos/Models/";
	config.pathfilePath = "X:/Yvens Rebouças/GIT/Shaders_Estudos/Paths/";
	config.logPath = "X:/Yvens Rebouças/GIT/Shaders_Estudos/Logs/Log Mia/";
	modelPath = "X:/Yvens Rebouças/GIT/Shaders_Estudos/Models/";
	modelPathLocation = "X:/Yvens Rebouças/GIT/Shaders_Estudos/Paths/";
#endif
#ifdef desktop
	config.objectPath = "F:/Repositorios/Shaders_Estudos/Models/";
	config.pathfilePath = "F:/Repositorios/Shaders_Estudos/Paths/";
	config.logPath = "F:/Repositorios/Shaders_Estudos/Logs/Log Desktop/";
#endif
#ifdef notebook
	config.objectPath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Models/";
	config.pathfilePath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Paths/";
	config.logPath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Logs/Log Notebook/";
	modelPath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Models/";
	modelPathLocation = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Paths/";
#endif

	//Quando passa de 150 que a coisa começa a degringolar total
	std::vector<GLDynamicObject> totalDynamics = std::vector<GLDynamicObject>();
	
	//Setando a configuração pelo macro
	//RAVINE;
	VIADUCT;
	//SAW;
	//COALTOWN;
	//GOLDRUSH;
	//CITYCYCLES;

	config.repeatable = false;
	config.logResults = true;

	config.pathIdentifier = 1;
	config.pathExtraMsg = "walk";
	config.logExtraMsg = "test-mia-x";
	config.logIdentifier = 0;
	config.edeDepth = 0;
	config.type = NONE;
		
	//TEST_BODY(RECORD_PATH, "teste");
	TEST_BODY(WALKTHROUGH_MODE, "teste");
#endif

	GLBufferHandler::clearBuffers();
}  


/////////////////////////////////////////////////////////////////////////////
///BACK UP DE CÓDIGO
/*
int numberTests = 1;
	for (int i = 0; i < numberTests; i++)
	{
		//Inicializa com 0 e deve ser mudado em tempo de execução, caso necessário
		config.logIdentifier = i + 1;
		//Adiciona um texto adicional ao arquivo de log
		config.logExtraMsg = "test-mia-x";
		//Altura padrão de uma EDE
		config.edeDepth = 3;
		//EDE escolhida
		config.type = ROCTREE;
		//Colore os nós em tons diferente
		config.coloredNodes = false;

		config.enableDynamics = false;
		config.edeTestDynamics = false;
		config.frustumTestDynamics = true;

		config.title = "T";

		PLAYER_MODE mode;
		mode = EDE_MAKER;
		mode = FREE_MODE;
		mode = RECORD_PATH;
		mode = BIRDS_EYE;
		mode = WALKTHROUGH_MODE;

		//Nesse método, o player é inicializado e a configuração é linkada ao player
		player = config.getGLPlayer(mode);

		OpenGLWrapper::player = player;
		OpenGLWrapper::initialize(true, 4);
		OpenGLWrapper::glLoop();
	}
*/