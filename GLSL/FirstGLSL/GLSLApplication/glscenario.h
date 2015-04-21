#pragma once

#include "glconfig.h"
#include "glmesh3d.h"

class GLScenario
{
public:
	char* name;
	int identifier;

	GLMeshHandler* meshHandler;
private:
	GLConfig* config;
public:
	GLScenario(void);
	GLScenario(char* name, GLConfig* config);
	GLScenario(int identifier, GLConfig* config);

	~GLScenario(void);

private:
	void initialize(GLConfig* config);

	static int getIdentifierByName(char* name);
	static void getNameByIdentifier(int identifier, char* dest);
};