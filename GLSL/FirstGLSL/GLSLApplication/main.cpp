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
	config.width = 640;
	config.height = 480;
	config.title = "teste";

	player = new GLPlayer(config);
	OpenGLWrapper::player = *player;
	OpenGLWrapper::initialize(callBack, true, 4);
	OpenGLWrapper::running_callback(running);
	OpenGLWrapper::glLoop();
}  