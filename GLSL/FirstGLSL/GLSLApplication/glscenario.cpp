#include "glscenario.h"

#include <stdio.h>

#include "glbuffer.h"

GLScenario::GLScenario(void)
{ }

GLScenario::GLScenario(char* name, GLConfig* config)
{ 
	this->name = name;

	identifier = GLScenario::getIdentifierByName(this->name);

	initialize(config);
}

GLScenario::GLScenario(int identifier, GLConfig* config)
{ 
	this->identifier = identifier;

	this->name = new char[128];
	GLScenario::getNameByIdentifier(identifier, name);

	initialize(config);
}

GLScenario::~GLScenario(void)
{
	if(name)
		delete name;
}

void GLScenario::initialize(GLConfig* config)
{
	this->config = config;

	if(GLBufferHandler::checkForMeshHandler(config->objectName))
	{
		meshHandler = GLBufferHandler::meshHandlerBuffer[config->objectName];
	}
	else
	{
		meshHandler = new GLMeshHandler(config->objectName, config->objectPath);
		GLBufferHandler::addToMeshHandlerBuffer(config->objectName, meshHandler);
	}

	switch (config->mode)
	{
	case WALKTHROUGH_MODE:
	case BIRDS_EYE:
		//Aqui o path é lido para ser executado
		cameraHandler = GLCameraHandler(config->pathfilePath, config->pathfileName, config->repeatable);
		break;
	case RECORD_PATH:
		//Aqui o pathfilePath será o destino do novo path, o identificar é para não sobreescrever
		cameraHandler = GLCameraHandler(config->pathfilePath, config->pathfileName, config->pathIdentifier, config->pathExtraMsg);
		break;
	default:
		break;
	}
}

#pragma region buscar identificadores
int  GLScenario::getIdentifierByName(char* name)
{
	if(name[0] == 's')
	{
		//Saw
		return 10;
	}
	if(name[0] == 'r')
	{
		//Ravine
		return 13;
	}
	if(name[0] == 'v')
	{
		//Viaduct
		return 14;
	}
	if(name[0] == 'g')
	{
		if(name[3] == 'r')
		{
			//Gorge
			return 15;
		}
		if(name[3] == 'r')
		{
			//Gorge
			return 15;
		}
		if(name[4] == 'd')
		{
			//GoldRush
			return 18;
		}
		else
		{
			//GoldenPlains
			return 19;
		}
	}
	if(name[0] == 'n')
	{
		//Nucleus
		return 16;
	}
	if(name[0] == 'c')
	{
		//Citycycles
		if(name[1] == 'i')
		{
			return 11;
		}
		//Coaltown
		return 17;
	}
	
	return -1;
}

void GLScenario::getNameByIdentifier(int identifier, char* dest)
{
	switch (identifier)
	{
		case 10: dest = "saw"; break;
		case 11: dest = "citycycles"; break;
		case 13: dest = "ravine"; break;
		case 14: dest = "viaduct"; break;
		case 15: dest = "gorge"; break;
		case 16: dest = "nucleus"; break;
		case 17: dest = "coaltown"; break;
		case 18: dest = "goldrush"; break;
		default: dest = ""; break;
	}
}
#pragma endregion

GLCameraStep* GLScenario::defaultStartPosition(int identifier)
{
	switch (identifier)
	{
		case 10: 
			return new GLCameraStep(glm::vec3(287.0f,11.0f,17.0f),glm::vec3(0.0f,1.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f),45.0); 
			break;
		case 11:
			return new GLCameraStep(glm::vec3(0.47, -1.35f, -4.68f),glm::vec3(0.0f,1.0f,0.0f),glm::vec3(0.0f,0.0f, 1.0f),45.0); 
			break;
		case 13:
			return new GLCameraStep(glm::vec3(23.9f,-0.5f,-57.5f),glm::vec3(0.0f,1.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f),45.0); 
			break;
		case 14: 
			return new GLCameraStep(glm::vec3(-38.9f,3.2f,137.0f),glm::vec3(0.0f,1.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f),45.0); 
			break;
		case 15: 
			return new GLCameraStep(glm::vec3(-372.0f,3.6f,35.0f),glm::vec3(0.0f,1.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f),45.0); 
			break;
		case 16: 
			return new GLCameraStep(glm::vec3(7.3f,-8.2f,4.3f),glm::vec3(0.0f,1.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f),45.0); 
			break;
		case 17: 
			return new GLCameraStep(glm::vec3(-24.0f,20.0f,92.0f),glm::vec3(0.0f,1.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f),45.0); 
			break;
		case 18: 
			return new GLCameraStep(glm::vec3(-42.0f,9.0f,-82.0f),glm::vec3(0.0f,1.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f),45.0); 
			break;
		default: 
			return new GLCameraStep(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f),45.0); 
			break;
	}
}

GLCameraStep* GLScenario::defaultBirdPosition(int identifier)
{
	switch (identifier)
	{
		//TODO fazer para os demais
		case 10: 
			return new GLCameraStep(glm::vec3(287.0f,11.0f,17.0f),glm::vec3(0.0f,1.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f),45.0); 
			break;
		case 11:
			return new GLCameraStep(glm::vec3(16.58, 32.14f, 2.42f),glm::vec3(-0.87f,0.43f,-0.24f),glm::vec3(-0.42f,-0.90f, -0.11f),45.0); 
			break;
		case 13:
			return new GLCameraStep(glm::vec3(23.9f,-0.5f,-57.5f),glm::vec3(0.0f,1.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f),45.0); 
			break;
		case 14: 
			return new GLCameraStep(glm::vec3(-38.9f,3.2f,137.0f),glm::vec3(0.0f,1.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f),45.0); 
			break;
		case 15: 
			return new GLCameraStep(glm::vec3(-372.0f,3.6f,35.0f),glm::vec3(0.0f,1.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f),45.0); 
			break;
		case 16: 
			return new GLCameraStep(glm::vec3(7.3f,-8.2f,4.3f),glm::vec3(0.0f,1.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f),45.0); 
			break;
		case 17: 
			return new GLCameraStep(glm::vec3(-67.65f, 188.47f, 135.96f),glm::vec3(0.54f, 0.66f, -0.53f),glm::vec3(0.47f,-0.75f,-0.46f),45.0); 
			break;
		case 18: 
			return new GLCameraStep(glm::vec3(-42.0f,9.0f,-82.0f),glm::vec3(0.0f,1.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f),45.0); 
			break;
		default: 
			return new GLCameraStep(glm::vec3(155.62f,363.80f,128.16f),glm::vec3(-0.76f,0.56f,-0.33f),glm::vec3(-0.51f,-0.83f,-0.22f),45.0); 
			break;
	}
}

float GLScenario::defaultCameraSpeed()
{
	//Por enquanto, tá default 0.75 para qualquer cenário
	switch (identifier)
	{
		case 10: return 0.75f;
		case 11: return 0.02f;
		default: return 0.13f;
	}
}
