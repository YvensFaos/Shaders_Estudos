#include "glrecordpathplayer.h"

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
#include "glmathhelper.h"

#include <stdio.h>

GLRecordPathPlayer::GLRecordPathPlayer(void)
{ }

GLRecordPathPlayer::~GLRecordPathPlayer(void)
{ }

GLRecordPathPlayer::GLRecordPathPlayer(GLConfig config)
{
	initializeGLPlayer(config);
}

void GLRecordPathPlayer::initializeGLPlayer(GLConfig config)
{
	this->config = config;
	this->printCounter = 0;

	recording = false;

	isRunning = true;
	updateMouse = false;

	xpos = config.width / 2.0f;
	ypos = config.height / 2.0f;

	deltaTime = 1.0f/60.0f;
	lastTime = 0;

	char* path = config.objectPath;

	//Checando se o nome foi setado corretamente
	if(config.objectName && config.objectName[0] != '\0')
	{
		this->scenario = new GLScenario(config.objectName, &config);
	}
	else
	{
		//Se não tiver o nome, busca pelo identificador
		this->scenario = new GLScenario(config.scenarioNumber, &config);
	}

	camera = new GLCamera();
	cameraHandler = &scenario->cameraHandler;

	actualStep = GLScenario::defaultStartPosition(scenario->identifier);
	camera->setValues(actualStep);
	camera->calculateMatrix(actualStep, 0, config.width, config.height);
	//camera->speed = GLScenario::defaultCameraSpeed(scenario->identifier);
	camera->speed = 0.75f;

	meshHandler = &scenario->meshHandler;

	title = new char[256];
	modeTitle = new char[256];
	sprintf(modeTitle, "Recording Path [%s] - Scenario:%s - ", (recording) ? "on" : "off", scenario->name);
}

void GLRecordPathPlayer::step(void)
{
	double firstTime = glfwGetTime();

	bool pressingMouse = glfwGetMouseButton(OpenGLWrapper::window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

	if(pressingMouse)
	{
		updateMousePos();		
	}

	camera->calculateMatrix(xpos, ypos, deltaTime, config.width, config.height);
	xpos = config.width / 2.0f;
	ypos = config.height / 2.0f;

	if(recording && pressingMouse)
	{
		cameraHandler->addStepRecording(camera);
	}

	glm::mat4 ModelMatrix = glm::mat4(1.0);
    glm::mat4 MVP = camera->projectionMatrix * camera->viewMatrix * ModelMatrix;

	GLint model = glGetUniformLocation(OpenGLWrapper::programObject, "mvp");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(MVP));
	
	GLint loc = glGetUniformLocation(OpenGLWrapper::programObject, "baseColor");
	glUniform4f(loc, 0.75f, 0.64f, 0.04f, 1.0f);

	GLint pos = glGetUniformLocation(OpenGLWrapper::programObject, "vDir");
	glUniform3f(pos, camera->direction.x, camera->direction.y, camera->direction.z);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, config.width, config.height);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(VEC4_PRINT(OpenGLWrapper::ACTUAL_CLEAR_COLOR));
	glUseProgram(OpenGLWrapper::programObject);

	meshHandler->render();

	double lastTime = glfwGetTime();
	deltaTime = float(lastTime - firstTime);
	deltaTime = (deltaTime == 0) ? 0.0015 : deltaTime;

	sprintf(title, "%s%s - fps[%.2f][%d]", modeTitle, config.title, (float) (1 / deltaTime), cameraHandler->getIndex());
	glfwSetWindowTitle(OpenGLWrapper::window, title);

	if(cameraHandler->finished && !cameraHandler->repeated)
	{
		isRunning = false;
	}
}

bool GLRecordPathPlayer::running(void)
{
	return isRunning;
}

void GLRecordPathPlayer::keyBoard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			isRunning = false;
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		//Controle da Câmera

		if (key == GLFW_KEY_Z){
			camera->position += camera->up * 1.f * camera->speed;
			if(recording)
			{
				cameraHandler->addStepRecording(camera);
			}
		}
		if (key == GLFW_KEY_X){
			camera->position -= camera->up * 1.f * camera->speed;
			if(recording)
			{
				cameraHandler->addStepRecording(camera);
			}
		}
		if (key == GLFW_KEY_W){
			camera->position += camera->direction * 1.f * camera->speed;
			if(recording)
			{
				cameraHandler->addStepRecording(camera);
			}
		}
		if (key == GLFW_KEY_S){
			camera->position -= camera->direction * 1.f * camera->speed;
			if(recording)
			{
				cameraHandler->addStepRecording(camera);
			}
		}
		if (key == GLFW_KEY_A){
			camera->position -= camera->right * 1.f * camera->speed;
			if(recording)
			{
				cameraHandler->addStepRecording(camera);
			}
		}
		if (key == GLFW_KEY_D){
			camera->position += camera->right * 1.f * camera->speed;
			if(recording)
			{
				cameraHandler->addStepRecording(camera);
			}
		}

		//Controle de Zoom
		if(key == GLFW_KEY_1)
		{
			//Zoom IN
			camera->zoom(-0.005f);
		}
		if(key == GLFW_KEY_2)
		{
			//Zoom OUT
			camera->zoom(+0.005f);
		}

		//Controle de Velocidade da Câmera
		if(key == GLFW_KEY_6)
		{
			//Zoom IN
			camera->speed += 0.05f;
			printf("Speed: %f\n", camera->speed);
		}
		if(key == GLFW_KEY_7)
		{
			//Zoom OUT
			camera->speed -= 0.05f;
			printf("Speed: %f\n", camera->speed);
		}

		if(key == GLFW_KEY_T)
		{
			//Liga e desliga a gravação
			record();

			if(recording)
			{
				//Grava passo inicial
				cameraHandler->stardRecording(camera);
			}
			else
			{
				cameraHandler->stopRecording();
			}
		}

		//Depuração
		if(key == GLFW_KEY_4)
		{
			//TODO print da câmera e do actualstep aqui!
			camera->print();
		}
		if(key == GLFW_KEY_5)
		{
			//PrintScreen
			EDPrinter printer = EDPrinter();
			char filename[256];
			sprintf(filename, "%sRecordPrint-%s[%d]-x.bmp", config.objectPath, config.objectName, printCounter++);
			printer.printScreen(&config, filename);
		}
	}
}

void GLRecordPathPlayer::record()
{
	recording = !recording;
	if(recording)
	{
		OpenGLWrapper::ACTUAL_CLEAR_COLOR = glm::vec4(VEC4_PRINT(OpenGLWrapper::RECORDING_CLEAR_COLOR));
	}
	else
	{
		OpenGLWrapper::ACTUAL_CLEAR_COLOR = glm::vec4(VEC4_PRINT(OpenGLWrapper::DEFAULT_CLEAR_COLOR));
	}
}

bool GLRecordPathPlayer::isRecording()
{
	return recording;
}

void GLRecordPathPlayer::mouse(GLFWwindow* window, int button, int action, int mods)
{ 
	if(action == GLFW_PRESS)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			updateMouse = true;
		}
	}
	if(action == GLFW_RELEASE)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			updateMouse = false;
		}
	}
}

void GLRecordPathPlayer::updateMousePos()
{
	glfwGetCursorPos(OpenGLWrapper::window, &xpos, &ypos);
}

void GLRecordPathPlayer::lights(void)
{
	GLfloat matAmbient[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat matDiffuse[] = { 0.8f, 0.4f, 0.4f, 1.0f };
	GLfloat lightAmbient[] = {1.f, 1.f, 1.f, 1.f} ;
	GLfloat lightDiffuse[] = {1.f, 1.f, 1.f, 1.f} ;

	GLfloat lightPos1[] = { 20.0, 20.0, 20.0, 1.0 };
	GLfloat lightPos2[] = { -20.0, 20.0, 20.0, 1.0 };
	GLfloat lightPos3[] = { 20.0, 20.0, -20.0, 1.0 };
	GLfloat lightPos4[] = { -20.0, 20.0, -20.0, 1.0 };
	GLfloat spotDir1[] =  { -0.5f, -0.5f, -0.5f };
	GLfloat spotDir2[] =  { 0.5f, -0.5f, -0.5f };
	GLfloat spotDir3[] =  { -0.5f, -0.5f, 0.5f };
	GLfloat spotDir4[] =  { 0.5f, -0.5f, 0.5f };

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