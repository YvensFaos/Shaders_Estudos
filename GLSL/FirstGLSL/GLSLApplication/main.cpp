#include "openGLWrapper.h"

#include "glplayer.h"
#include "glshaderloader.h"

//Include the standard C++ headers  
#include <stdio.h>  
#include <stdlib.h>  


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

	player = new GLPlayer(config);
	OpenGLWrapper::player = *player;
	OpenGLWrapper::initialize(callBack, true, 4);
	OpenGLWrapper::running_callback(running);
	OpenGLWrapper::glLoop();
}  