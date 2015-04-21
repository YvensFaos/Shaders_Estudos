#include "glscenario.h"

#include <stdio.h>

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

	meshHandler = GLMeshHandler(config->objectName, config->objectPath);
	cameraHandler = GLCameraHandler(config->pathfilePath, config->pathfileName, config->repeatable);
}

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
		case 13: dest = "ravine"; break;
		case 14: dest = "viaduct"; break;
		case 15: dest = "gorge"; break;
		case 16: dest = "nucleus"; break;
		case 17: dest = "coaltown"; break;
		case 18: dest = "goldrush"; break;
		default: dest = ""; break;
	}
}
