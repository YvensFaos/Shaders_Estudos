#include "openGLWrapper.h"

#include <stdio.h>
#include <GL\glew.h>
#include "GLFW/glfw3.h"

#include "glplayer.h"

GLPlayer OpenGLWrapper::player;
float OpenGLWrapper::ratio;
GLFWwindow* OpenGLWrapper::window;
loopCallback OpenGLWrapper::callback;
runningCallback OpenGLWrapper::running;

void OpenGLWrapper::initialize(loopCallback callback, bool antialiasing, int multisampling)
{
    glfwSetErrorCallback(OpenGLWrapper::error_callback);  
    if (!glfwInit())  
    {  
        return;  
    }  
  
	if(antialiasing)
	{
		glfwWindowHint(GLFW_SAMPLES, multisampling);
	}
  
	int width = 600; //player.config.width;
	int height = 600; //player.config.height;
	char* title = ""; //player.config.mainTitle;
    window = glfwCreateWindow(640, 480, "Test Window", NULL, NULL);  
  
    if (!window)  
    {  
        fprintf( stderr, "Failed to open GLFW window.\n" );  
        glfwTerminate();  
        return;
    }  
  
    glfwMakeContextCurrent(window);  
	player.actualWindow = window;
    glfwSetKeyCallback(window, OpenGLWrapper::key_callback);  
    GLenum err = glewInit();

    if (err != GLEW_OK)   
    {  
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));  
        return;  
    }  
  
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  

	glfwSetErrorCallback(OpenGLWrapper::error_callback);
	glfwInit();

	//player.initializeLigths();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);

	player.actualWindow = window;
}

void OpenGLWrapper::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{ 
	//player.keyBoard(window, key, scancode, action, mods); 
}

void OpenGLWrapper::error_callback(int error, const char* description)
{ }

void OpenGLWrapper::loop_callback(loopCallback callback)
{ OpenGLWrapper::callback = callback; }

void OpenGLWrapper::running_callback(runningCallback running)
{ OpenGLWrapper::running = running; }

void OpenGLWrapper::glLoop()
{
	while (!glfwWindowShouldClose(window) && (running)())
    {

		int width = 600; //player.config.width;
		int height = 600; //player.config.height;

		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);

        glLoadIdentity();

		(callback)();

		glFlush();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	glfwDestroyWindow(window);
}