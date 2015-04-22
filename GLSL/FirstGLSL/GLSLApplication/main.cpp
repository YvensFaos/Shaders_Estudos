#include "openGLWrapper.h"

#include "glplayer.h"
#include "glshaderloader.h"
#include "glenums.h"

//Include the standard C++ headers  
#include <stdio.h>  
#include <stdlib.h>  

//#define mia
//#define desktop
#define notebook

GLPlayer* player;

int main()  
{  
	GLConfig config;
	config.width  = 800;
	config.height = 600;
	config.title  = "Teste";

	//Configurações de PATH que são exclusivos do computador usado
#ifdef mia
	//TODO ajustar
	config.objectPath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Models/";
	config.pathfilePath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Paths/";
#endif
#ifdef desktop
	config.objectPath = "E:/Repositorios/Shaders_Estudos/Models/";
	config.pathfilePath = "E:/Repositorios/Shaders_Estudos/Paths/";
#endif
#ifdef notebook
	config.objectPath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Models/";
	config.pathfilePath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Paths/";
#endif

	config.objectName = "coaltown.obj";
	//Se eu setar o nome, não preciso do número, mas não tem problema
	config.scenarioNumber = 17;

	config.pathfileName = "coaltown";
	config.repeatable = true;

	//Seta um valor inicial para o identifier; Caso seja -1, o código
	//Inicializa com 0 e começa a contagem a partir daí
	config.pathIdentifier = 1;

	PLAYER_MODE mode;
	mode = FREE_MODE;
	mode = RECORD_PATH;
	mode = WALKTHROUGH_MODE;

	player = config.getGLPlayer(mode);

	OpenGLWrapper::player = player;
	OpenGLWrapper::initialize(true, 4);
	OpenGLWrapper::glLoop();
}  