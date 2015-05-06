#include "openGLWrapper.h"

#include "glplayer.h"
#include "glshaderloader.h"
#include "glenums.h"
#include "glbatch.h"

//Include the standard C++ headers  
#include <stdio.h>  
#include <stdlib.h>  

//#define mia
#define desktop
//#define notebook

#define individual
#define batch

GLPlayer* player;

int main()  
{  

#ifdef individual
	GLConfig config;
	config.width  = 800;
	config.height = 600;
	config.title  = "Teste";

	//Configura��es de PATH que s�o exclusivos do computador usado
#ifdef mia
	//TODO ajustar
	config.objectPath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Models/";
	config.pathfilePath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Paths/";
	config.logPath = "E:/Repositorios/Shaders_Estudos/Logs/Log Desktop/";
#endif
#ifdef desktop
	config.objectPath = "E:/Repositorios/Shaders_Estudos/Models/";
	config.pathfilePath = "E:/Repositorios/Shaders_Estudos/Paths/";
	config.logPath = "E:/Repositorios/Shaders_Estudos/Logs/Log Desktop/";
#endif
#ifdef notebook
	config.objectPath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Models/";
	config.pathfilePath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Paths/";
	config.logPath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Logs/Log Notebook/";
#endif

	config.objectName = "goldrush.obj";
	//Se eu setar o nome, n�o preciso do n�mero, mas n�o tem problema
	config.scenarioNumber = 18;

	config.pathfileName = "goldrush--[3][test]";
	//config.pathfileName = "saw-[1][test]-[1][test]";
	config.repeatable = false;
	config.logResults = true;

	//Seta um valor inicial para o identifier; Caso seja -1, o c�digo
	//Inicializa com 0 e come�a a contagem a partir da�
	config.pathIdentifier = 1;
	//Adiciona um texto adicional ao arquivo de path
	config.pathExtraMsg = "test";
	//Inicializa com 0 e deve ser mudado em tempo de execu��o, caso necess�rio
	config.logIdentifier = 1;
	//Adiciona um texto adicional ao arquivo de logw
	config.logExtraMsg = "test";
	//Altura padr�o de uma EDE
	config.edeDepth = 3;
	//EDE escolhida
	config.type = ROCTREE;
	//Colore os n�s em tons diferente
	config.coloredNodes = true;

	//Fazer um teste na replicated: verificar se a qtde. enviada � maior do que a total

	PLAYER_MODE mode;
	mode = FREE_MODE;
	mode = EDE_MAKER;
	mode = RECORD_PATH;
	mode = WALKTHROUGH_MODE;

	//Nesse m�todo, o player � inicializado e a configura��o � linkada ao player
	player = config.getGLPlayer(mode);

	OpenGLWrapper::player = player;
	OpenGLWrapper::initialize(true, 4);
	OpenGLWrapper::glLoop();
#endif

#ifdef batch

	char batchPath[128];

	#ifdef mia
		//TODO ajustar
		sprintf(batchPath, "E:/Repositorios/Shaders_Estudos/Batch/Batch Desktop/");
	#endif
	#ifdef desktop
		sprintf(batchPath, "E:/Repositorios/Shaders_Estudos/Batch/Batch Desktop/");
	#endif
	#ifdef notebook
		sprintf(batchPath, "E:/Repositorios/Shaders_Estudos/Batch/Batch Desktop/");
	#endif

	std::vector<char*> batchFiles;
	batchFiles.push_back("exemplo");

	GLBatch* batchFile;
	for(int i = 0; i < batchFiles.size(); i++)
	{
		batchFile = new GLBatch(batchPath, batchFiles[i]);
		std::vector<GLConfig> configurations = batchFile->getTestBatch();

		for(int i = 0; i < configurations.size(); i++)
		{
			player = configurations.at(i).getGLPlayer();
			OpenGLWrapper::player = player;
			OpenGLWrapper::initialize(true, 4);
			OpenGLWrapper::glLoop();
		}

		delete batchFile;
	}
#endif
}  