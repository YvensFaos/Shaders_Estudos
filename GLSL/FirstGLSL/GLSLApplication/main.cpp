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
  
bool running(void)
{
	return player->running();
}
  
void callBack()
{
	player->step();
}

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

	config.objectName = "saw.obj";
	//Se eu setar o nome, não preciso do número, mas não tem problema
	config.scenarioNumber = 10;

	config.pathfileName = "saw";
	config.repeatable = true;

	player = config.getGLPlayer(FREE_MODE);
	player = config.getGLPlayer(WALKTHROUGH_MODE);

	OpenGLWrapper::player = *player;
	OpenGLWrapper::initialize(callBack, true, 4);
	OpenGLWrapper::running_callback(running);
	OpenGLWrapper::glLoop();
}  