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

#ifdef mia
	//TODO ajustar
	config.objectPath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Models/";
#endif
#ifdef desktop
	config.objectPath = "E:/Repositorios/Shaders_Estudos/Models/";
#endif
#ifdef notebook
	config.objectPath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Models/";
#endif

	config.objectName = "saw.obj";
	//Se eu setar o nome, não preciso do número, mas não tem problema
	config.scenarioNumber = 10;

	player = config.getGLPlayer(FREE_MODE);

	OpenGLWrapper::player = *player;
	OpenGLWrapper::initialize(callBack, true, 4);
	OpenGLWrapper::running_callback(running);
	OpenGLWrapper::glLoop();
}  