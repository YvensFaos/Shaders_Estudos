#include "glplayer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "assimp\Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <GL\glew.h>
#include <GL\glut.h>
#include "GLFW/glfw3.h"

#include "openGLWrapper.h"
#include "edlogger.h"
#include "glbuffer.h"

#include <stdio.h>

GLPlayer::GLPlayer()
{ }

GLPlayer::~GLPlayer()
{ }

GLPlayer::GLPlayer(GLConfig config)
{
	initializeGLPlayer(config);
}

void GLPlayer::initializeGLPlayer(GLConfig config)
{
	this->config = config;

	angle = 0.0f;
	isRunning = true;
	updateMouse = false;

	xpos = config.width / 2.0f;
	ypos = config.height / 2.0f;

	deltaTime = 1.0f / 60.0f;
	lastTime = 0;

	camera = new GLCamera();
	char* path = config.objectPath;

	if (GLBufferHandler::checkForMeshHandler(config.objectName))
	{
		meshHandler = GLBufferHandler::meshHandlerBuffer[config.objectName];
	}
	else
	{
		meshHandler = new GLMeshHandler(config.objectName, path);
		GLBufferHandler::addToMeshHandlerBuffer(config.objectName, meshHandler);
	}

	title = new char[256];
}

void GLPlayer::step(void)
{
	double firstTime = glfwGetTime();

	if (glfwGetMouseButton(OpenGLWrapper::window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		updateMousePos();
	}

	camera->calculateMatrix(&config, xpos, ypos, deltaTime);
	xpos = config.width / 2.0f;
	ypos = config.height / 2.0f;

	glm::mat4 ModelMatrix = glm::mat4(1.0);
	glm::mat4 MVP = camera->projectionMatrix * camera->viewMatrix * ModelMatrix;

	GLint model = glGetUniformLocation(OpenGLWrapper::programObject, "mvp");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(MVP));

	GLint loc = glGetUniformLocation(OpenGLWrapper::programObject, "baseColor");
	glUniform4f(loc, 0.75f, 0.64f, 0.04f, 1.0f);

	GLint pos = glGetUniformLocation(OpenGLWrapper::programObject, "vDir");
	glUniform3f(pos, camera->direction.x, camera->direction.y, camera->direction.z);

	pos = glGetUniformLocation(OpenGLWrapper::programObject, "angle");
	glUniform1f(pos, angle);
	angle += 0.1f;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, config.width, config.height);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(OpenGLWrapper::programObject);

	meshHandler->render();

	double lastTime = glfwGetTime();
	deltaTime = float(lastTime - firstTime);
	deltaTime = (deltaTime == 0) ? 0.0015 : deltaTime;

	sprintf(title, "%s - fps[%.2f]", config.title, (float)(1 / deltaTime));
	glfwSetWindowTitle(OpenGLWrapper::window, title);
}

bool GLPlayer::running(void)
{
	return isRunning;
}

void GLPlayer::keyBoard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			isRunning = false;
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		if (config.mode == FREE_MODE)
		{
			if (key == GLFW_KEY_Z){
				camera->position += camera->up * deltaTime * camera->speed;
			}
			if (key == GLFW_KEY_X){
				camera->position -= camera->up * deltaTime * camera->speed;
			}
			if (key == GLFW_KEY_W){
				camera->position += camera->direction * deltaTime * camera->speed;
			}
			if (key == GLFW_KEY_S){
				camera->position -= camera->direction * deltaTime * camera->speed;
			}
			if (key == GLFW_KEY_A){
				camera->position -= camera->right * deltaTime * camera->speed;
			}
			if (key == GLFW_KEY_D){
				camera->position += camera->right * deltaTime * camera->speed;
			}
		}

		//Depuração

		if (key == GLFW_KEY_1)
		{
			//Zoom IN
			camera->zoom(-0.005f);
		}
		if (key == GLFW_KEY_2)
		{
			//Zoom OUT
			camera->zoom(+0.005f);
		}
		if (key == GLFW_KEY_5)
		{
			//PrintScreen

			EDPrinter printer = EDPrinter();
			char filename[256];
			sprintf(filename, "%sRandomPrint-%s[%f]-x.bmp", config.objectPath, config.objectName, deltaTime);
			printer.printScreen(&config, filename);
		}
	}
}

void GLPlayer::mouse(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			updateMouse = true;
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			glfwSetCursorPos(OpenGLWrapper::window, config.width / 2.0f, config.height / 2.0f);
		}
	}
	if (action == GLFW_RELEASE)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			updateMouse = false;
		}
	}
}

void GLPlayer::updateMousePos()
{
	glfwGetCursorPos(OpenGLWrapper::window, &xpos, &ypos);
}

///Inicializa as luzes da cena
void GLPlayer::lights(void)
{
	GLfloat matAmbient[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat matDiffuse[] = { 0.8f, 0.4f, 0.4f, 1.0f };
	GLfloat lightAmbient[] = { 1.f, 1.f, 1.f, 1.f };
	GLfloat lightDiffuse[] = { 1.f, 1.f, 1.f, 1.f };

	GLfloat lightPos1[] = { 20.0, 20.0, 20.0, 1.0 };
	GLfloat lightPos2[] = { -20.0, 20.0, 20.0, 1.0 };
	GLfloat lightPos3[] = { 20.0, 20.0, -20.0, 1.0 };
	GLfloat lightPos4[] = { -20.0, 20.0, -20.0, 1.0 };
	GLfloat spotDir1[] = { -0.5f, -0.5f, -0.5f };
	GLfloat spotDir2[] = { 0.5f, -0.5f, -0.5f };
	GLfloat spotDir3[] = { -0.5f, -0.5f, 0.5f };
	GLfloat spotDir4[] = { 0.5f, -0.5f, 0.5f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDir1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 40.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 30.0);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDir2);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 40.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 30.0);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos3);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDir3);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 40.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 30.0);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos4);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDir4);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 40.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 30.0);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
}