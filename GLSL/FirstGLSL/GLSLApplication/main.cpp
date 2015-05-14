#include "openGLWrapper.h"

#include "glplayer.h"
#include "glshaderloader.h"
#include "glenums.h"
#include "glbatch.h"
#include "glbuffer.h"

//Include the standard C++ headers  
#include <stdio.h>  
#include <stdlib.h>  

//#define mia
//#define desktop
#define notebook

//#define individual
#define batch

GLPlayer* player;

int main()  
{  

#ifdef individual
	GLConfig config;
	config.width  = 800;
	config.height = 600;
	config.title  = "Teste";

	//Configurações de PATH que são exclusivos do computador usado
#ifdef mia
	//TODO ajustar
	config.objectPath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Models/";
	config.pathfilePath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Paths/";
	config.logPath = "E:/Repositorios/Shaders_Estudos/Logs/Log Desktop/";
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
#endif

	config.objectName = "nucleus.obj";
	//Se eu setar o nome, não preciso do número, mas não tem problema
	config.scenarioNumber = 16;

	config.pathfileName = "nucleus-[1][test]";
	//config.pathfileName = "saw-[1][test]-[1][test]";
	config.repeatable = false;
	config.logResults = false;

	//Seta um valor inicial para o identifier; Caso seja -1, o código
	//Inicializa com 0 e começa a contagem a partir daí
	config.pathIdentifier = 1;
	//Adiciona um texto adicional ao arquivo de path
	config.pathExtraMsg = "test";
	//Inicializa com 0 e deve ser mudado em tempo de execução, caso necessário
	config.logIdentifier = 1;
	//Adiciona um texto adicional ao arquivo de logw
	config.logExtraMsg = "test";
	//Altura padrão de uma EDE
	config.edeDepth = 2;
	//EDE escolhida
	config.type = ROCTREE;
	//Colore os nós em tons diferente
	config.coloredNodes = true;

	config.title = "Teste de Titulo!";

	PLAYER_MODE mode;
	mode = FREE_MODE;
	mode = EDE_MAKER;
	mode = RECORD_PATH;
	mode = WALKTHROUGH_MODE;

	//Nesse método, o player é inicializado e a configuração é linkada ao player
	player = config.getGLPlayer(mode);

	OpenGLWrapper::player = player;
	OpenGLWrapper::initialize(true, 4);
	OpenGLWrapper::glLoop();
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

	GLBufferHandler::clearBuffers();
}  