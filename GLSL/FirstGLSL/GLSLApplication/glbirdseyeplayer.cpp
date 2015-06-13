#include "glbirdseyeplayer.h"

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

#include <stdio.h>
#include <string>

GLBirdsEyePlayer::GLWalkthroughPlayer(void)
{ }

GLBirdsEyePlayer::~GLWalkthroughPlayer(void)
{ }

GLBirdsEyePlayer::GLWalkthroughPlayer(GLConfig config)
{
	initializeGLPlayer(config);
}

void GLBirdsEyePlayer::initializeGLPlayer(GLConfig config)
{
	this->config = config;
	this->printCounter = 0;

	angle = 0.0f;
	isRunning = true;
	updateMouse = false;
	paused = false;

	logged = false;

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
		//Se n�o tiver o nome, busca pelo identificador
		this->scenario = new GLScenario(config.scenarioNumber, &config);
	}

	camera = new GLCamera();
	cameraHandler = &scenario->cameraHandler;
	camera->calculateMatrix(cameraHandler->actualStep(), 0, config.width, config.height);
	meshHandler = scenario->meshHandler;

	char logName[512];
	if(config.type != NONE)
	{
		ede = GLBasicEDE::instantiate(&config);
		ede->testDynamics = config.edeTestDynamics;

		std::string edeName = ede->getName();

		char edeLogName[512];
		sprintf(edeLogName, "%s%s-%s-making[%d]%s", config.logPath, scenario->name, edeName.c_str(), config.edeDepth, LOG_EXTENSION);
		EDLogger edeLogger(edeLogName);

		double firstTime = glfwGetTime();
		printf("Carregar a EDE\n");
		ede->setLogger(&edeLogger);
		ede->calculateEDE(meshHandler, &config);
		double lastTime = glfwGetTime();
		lastTime = float(lastTime - firstTime);

		sprintf(edeLogName, "Tempo de processamento: %f", lastTime);
		edeLogger.logLineTimestamp(edeLogName);

		edeLogger.closeLog();

		sprintf(logName, "%s%s[%d]-%s[%s=%d][%s]%s", config.logPath, scenario->name, config.logIdentifier, config.logExtraMsg, edeName.c_str(), config.edeDepth, config.pathfileName, LOG_EXTENSION);
	}
	else
	{
		sprintf(logName, "%s%s[%d]-%s[%s]%s", config.logPath, scenario->name, config.logIdentifier, config.logExtraMsg, config.pathfileName, LOG_EXTENSION);
	}

	logger = new EDLogger(logName);

	title = new char[256];
	modeTitle = new char[256];
	sprintf(modeTitle, "[%s] - Scenario:%s - ", config.title, scenario->name);
}

void GLBirdsEyePlayer::step(void)
{
	double firstTime = glfwGetTime();
	
	GLCameraStep* step;
	if(isPaused())
	{
		step = cameraHandler->actualStep();
	}
	else
	{
		step = cameraHandler->nextStep();
	}

	camera->calculateMatrix(step, deltaTime, config.width, config.height);

	glm::mat4 ModelMatrix = glm::mat4(1.0);
    glm::mat4 MVP = camera->projectionMatrix * camera->viewMatrix * ModelMatrix;

	GLint model = glGetUniformLocation(OpenGLWrapper::programObject, "mvp");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(MVP));
	
	GLint loc = glGetUniformLocation(OpenGLWrapper::programObject, "baseColor");
	glUniform4f(loc, 0.75f, 0.64f, 0.04f, 1.0f);

	GLint pos = glGetUniformLocation(OpenGLWrapper::programObject, "vDir");
	glUniform3f(pos, step->direction.x, step->direction.y, step->direction.z);

	pos = glGetUniformLocation(OpenGLWrapper::programObject, "pos");
	glUniform4f(pos, 0.0f, 0.0f, 0.0f, 0.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, config.width, config.height);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(OpenGLWrapper::programObject);
	
	int verticesCount = 0;
	memset(info, 0, sizeof(float)*INFO_SIZE);

	if(config.type != NONE)
	{
		frustum = GLFrustum(camera->fov + 15.0f, camera->fov + 15.0f, camera->near, camera->far, camera);
		ede->renderEDE(&frustum, meshHandler, &config, info);
	}
	else
	{
		verticesCount = meshHandler->render();
	}

	int visibleBunnies = 0;

	if(config.enableDynamics)
	{
		glm::vec3* bounds = new glm::vec3[2];
		for(int i  = 0; i < config.dynamics.size(); i++)
		{
			GLDynamicObject* obj = &config.dynamics.at(i);

			if(!config.edeTestDynamics)
			{
				obj->visible = true;
			}
			if(config.frustumTestDynamics)
			{
				obj->visible = false;
				obj->getBounds(bounds);
				if(frustum.intercepts(&bounds[0], &bounds[1]))
				{
					obj->visible = true;
					visibleBunnies++;
				}
			}

			loc = glGetUniformLocation(OpenGLWrapper::programObject, "baseColor");
			glUniform4f(loc, 0.008f, 0.24f, 0.74f, 1.0f);

			obj->draw();
			obj->update();
		}

		delete bounds;
	}

	double lastTime = glfwGetTime();
	deltaTime = float(lastTime - firstTime);
	deltaTime = (deltaTime == 0) ? 0.0015 : deltaTime;

	sprintf(title, "%s - fps[%.2f][v = %d][%d]", modeTitle, (float) (1 / deltaTime), visibleBunnies,cameraHandler->getIndex());
	glfwSetWindowTitle(OpenGLWrapper::window, title);

	if(config.logResults && !logged)
	{
		if(config.type != NONE)
		{
			delete logLine;
			logLine = new char[64];
			std::string sdeltaTime = std::to_string(deltaTime);
			std::replace(sdeltaTime.begin(), sdeltaTime.end(), '.', ',');
			sprintf(logLine, "%s;%d;%d;%d;%d", sdeltaTime.c_str(), (int)info[0], (int)info[1], (int)info[2], (int)info[3]);
			logger->logLine(logLine);
		}
		else
		{
			delete logLine;
			logLine = new char[64];
			std::string sdeltaTime = std::to_string(deltaTime);
			std::replace(sdeltaTime.begin(), sdeltaTime.end(), '.', ',');
			sprintf(logLine, "%s;%d", sdeltaTime.c_str(), verticesCount);
			logger->logLine(logLine);
		}
	}

	if(cameraHandler->finished && !cameraHandler->repeated)
	{
		if(config.logResults)
		{
			logger->closeLog();
			logged = true;
		}
		if(!cameraHandler->repeated)
		{
			isRunning = false;
		}
	}
}

bool GLBirdsEyePlayer::running(void)
{
	return isRunning;
}

void GLBirdsEyePlayer::keyBoard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			isRunning = false;
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		//Depura��o

		if(key == GLFW_KEY_SPACE || key == GLFW_KEY_1)
		{
			//Pausa o Walkthrough
			printf("PAUSE!");
			pause();
		}

		if(key == GLFW_KEY_5)
		{
			//PrintScreen
			EDPrinter printer = EDPrinter();
			char filename[256];
			sprintf(filename, "%sWalkPrint-%s[%d]-x.bmp", config.objectPath, config.objectName, printCounter++);
			printer.printScreen(&config, filename);
		}
	}
}

void GLBirdsEyePlayer::pause()
{
	paused = !paused;
}

bool GLBirdsEyePlayer::isPaused()
{
	return paused;
}

void GLBirdsEyePlayer::mouse(GLFWwindow* window, int button, int action, int mods)
{ }

void GLBirdsEyePlayer::lights(void)
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