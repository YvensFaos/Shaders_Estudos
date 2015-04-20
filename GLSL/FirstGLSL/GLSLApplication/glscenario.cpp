#include "glscenario.h"

#include <stdio.h>

GLScenario::GLScenario(void)
{ }

GLScenario::GLScenario(char* name, GLConfig* config)
{ 
	this->name = new char[128];
	sprintf(this->name, "%s", name);

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
	delete name;
}

void GLScenario::initialize(GLConfig* config)
{
	this->config = config;

	//TODO
}

int  GLScenario::getIdentifierByName(char* name)
{
	if(name == "saw")
	{
		return 10;
	}
	else if(name == "ravine")
	{
		return 13;
	}
	else if(name == "viaduct")
	{
		return 14;
	}
	else if(name == "gorge")
	{
		return 15;
	}
	else if(name == "nucleus")
	{
		return 16;
	}
	else if(name == "coaltown")
	{
		return 17;
	}
	else if(name == "goldRush")
	{
		return 18;
	}
	else if(name == "goldenPlains")
	{
		return 19;
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
