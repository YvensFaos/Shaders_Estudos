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
	GLchar* vertexShaderText[1];
	unsigned long len[1];
	int success = GLShaderLoader::loadshader("vertexshader.txt", vertexShaderText, len);

	printf("Resultado: %d\n", success);

	//GLuint vertexShader, fragmentShader;

	//vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	//fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

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