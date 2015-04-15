#include "openGLWrapper.h"

#include "glshaderloader.h"

#include <stdio.h>

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
  
	int width = player.config.width;
	int height = player.config.height;
	char* title = player.config.title;

    window = glfwCreateWindow(width, height, title, NULL, NULL);  
  
    if (!window)  
    {  
        fprintf( stderr, "Failed to open GLFW window.\n" );  
        glfwTerminate();  
        return;
    }  
  
	loop_callback(callback);

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

	unsigned long vlen;
	unsigned long flen;
	GLchar* vertexShaderText = GLShaderLoader::loadshader("vertexshader.txt", &vlen);
	GLchar* fragmentShaderText = GLShaderLoader::loadshader("fragmentshader.txt", &flen);

	GLuint ShaderProgram = glCreateProgram();
	GLuint vertexShaderObject, fragmentShaderObject;

	vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	GLint vleng[1];
	vleng[0] = vlen;

	GLint fleng[1];
	fleng[0] = vlen;
	glShaderSource(vertexShaderObject, 1, &vertexShaderText, vleng);
	glShaderSource(fragmentShaderObject, 1, &fragmentShaderText, fleng);
	
	glCompileShaderARB(vertexShaderObject);
	glCompileShaderARB(fragmentShaderObject);

	player.lights();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);

	player.actualWindow = window;
}

void OpenGLWrapper::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{ 
	player.keyBoard(window, key, scancode, action, mods); 
}

void OpenGLWrapper::error_callback(int error, const char* description)
{ }

void OpenGLWrapper::loop_callback(loopCallback callback)
{ 
	OpenGLWrapper::callback = callback; 
}

void OpenGLWrapper::running_callback(runningCallback running)
{ 
	OpenGLWrapper::running = running; 
}

void OpenGLWrapper::glLoop()
{
	while (!glfwWindowShouldClose(window) && (running)())
    {

		int width = player.config.width;
		int height = player.config.height;

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