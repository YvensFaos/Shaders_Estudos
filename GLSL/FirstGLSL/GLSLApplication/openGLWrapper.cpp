#include "openGLWrapper.h"

#include "glshaderloader.h"
#include "glmathhelper.h"

#include <stdio.h>

//Public
glm::vec4 OpenGLWrapper::DEFAULT_CLEAR_COLOR = glm::vec4(0.2f, 0.2f, 0.2f, 0.0f);
glm::vec4 OpenGLWrapper::RECORDING_CLEAR_COLOR = glm::vec4(0.6f, 0.2f, 0.2f, 0.0f);

glm::vec4 OpenGLWrapper::ACTUAL_CLEAR_COLOR = glm::vec4(VEC4_PRINT(OpenGLWrapper::DEFAULT_CLEAR_COLOR));

GLuint OpenGLWrapper::programObject;
GLuint OpenGLWrapper::dynamicObject;
GLuint OpenGLWrapper::normalLoc;
GLuint OpenGLWrapper::positionLoc;
GLPlayer* OpenGLWrapper::player;
GLFWwindow* OpenGLWrapper::window;

//Private
float OpenGLWrapper::ratio;

void OpenGLWrapper::initialize(bool antialiasing, int multisampling)
{
	glfwSetErrorCallback(OpenGLWrapper::error_callback);
	if (!glfwInit())
	{
		return;
	}

	if (antialiasing)
	{
		glfwWindowHint(GLFW_SAMPLES, multisampling);
	}

	int width = player->config.width;
	int height = player->config.height;
	char* title = player->config.title;

	window = glfwCreateWindow(width, height, title, NULL, NULL);

	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, OpenGLWrapper::key_callback);
	glfwSetMouseButtonCallback(window, OpenGLWrapper::mouse_callback);

	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		return;
	}

	glClearColor(VEC4_PRINT(ACTUAL_CLEAR_COLOR));

	glfwSetErrorCallback(OpenGLWrapper::error_callback);
	glfwInit();

	unsigned long vlen;
	unsigned long flen;
	GLchar* vertexShaderText = GLShaderLoader::loadshader("mvpshader.txt", &vlen);
	GLchar* fragmentShaderText = GLShaderLoader::loadshader("fragmentshader.txt", &flen);

	GLchar* dynamicVertexText = GLShaderLoader::loadshader("dynamicvshader.txt", &vlen);
	GLchar* dynamicFragmeText = GLShaderLoader::loadshader("dynamicnshader.txt", &vlen);

	GLuint vertexShader;
	GLuint fragmentShader;

	GLuint dvertexShader;
	GLuint dfragmentShader;

	GLint linked;

	vertexShader = OpenGLWrapper::loadShader(vertexShaderText, GL_VERTEX_SHADER);
	fragmentShader = OpenGLWrapper::loadShader(fragmentShaderText, GL_FRAGMENT_SHADER);

	dvertexShader = OpenGLWrapper::loadShader(dynamicVertexText, GL_VERTEX_SHADER);
	dfragmentShader = OpenGLWrapper::loadShader(dynamicFragmeText, GL_FRAGMENT_SHADER);

	OpenGLWrapper::programObject = glCreateProgram();
	glAttachShader(OpenGLWrapper::programObject, vertexShader);
	glAttachShader(OpenGLWrapper::programObject, fragmentShader);

	OpenGLWrapper::dynamicObject = glCreateProgram();
	glAttachShader(OpenGLWrapper::dynamicObject, dvertexShader);
	glAttachShader(OpenGLWrapper::dynamicObject, dfragmentShader);

	normalLoc = 1;
	glBindAttribLocation(OpenGLWrapper::programObject, normalLoc, "vNormal");
	glBindAttribLocation(OpenGLWrapper::dynamicObject, normalLoc, "vNormal");

	int n = 0;

	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &n);
	printf("Max vertex attribs: %d\n", n);

	glLinkProgram(OpenGLWrapper::programObject);
	glGetProgramiv(OpenGLWrapper::programObject, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		GLint infolen = 0;
		glGetProgramiv(OpenGLWrapper::programObject, GL_INFO_LOG_LENGTH, &infolen);

		if (infolen > 1)
		{
			char* infolog = new char[infolen];
			glGetProgramInfoLog(OpenGLWrapper::programObject, infolen, NULL, infolog);
			printf("%s\n", infolog);

			delete infolog;
		}

		glDeleteProgram(OpenGLWrapper::programObject);
	}

	glLinkProgram(OpenGLWrapper::dynamicObject);
	glGetProgramiv(OpenGLWrapper::dynamicObject, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		GLint infolen = 0;
		glGetProgramiv(OpenGLWrapper::dynamicObject, GL_INFO_LOG_LENGTH, &infolen);

		if (infolen > 1)
		{
			char* infolog = new char[infolen];
			glGetProgramInfoLog(OpenGLWrapper::dynamicObject, infolen, NULL, infolog);
			printf("%s\n", infolog);

			delete infolog;
		}

		glDeleteProgram(OpenGLWrapper::dynamicObject);
	}

	player->lights();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);
}

GLuint OpenGLWrapper::loadShader(const char *shaderSrc, GLenum type)
{
	GLuint shader;
	GLint compiled;

	shader = glCreateShader(type);
	if (shader == 0)
	{
		return 0;
	}
	glShaderSource(shader, 1, &shaderSrc, NULL);

	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 0)
		{
			char* infolog = new char[infoLen];
			glGetShaderInfoLog(shader, infoLen, NULL, infolog);
			printf("%s\n", infolog);

			delete infolog;
		}

		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

void OpenGLWrapper::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	player->keyBoard(window, key, scancode, action, mods);
}

void OpenGLWrapper::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	player->mouse(window, button, action, mods);
}

void OpenGLWrapper::error_callback(int error, const char* description)
{ }

void OpenGLWrapper::glLoop()
{
	while (!glfwWindowShouldClose(window) && player->running())
	{
		int width = player->config.width;
		int height = player->config.height;

		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);

		glLoadIdentity();

		player->step();

		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
}