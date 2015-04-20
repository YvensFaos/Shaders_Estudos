#ifndef __GL_SCENARIO__
#define __GL_SCENARIO__

#include "glconfig.h"

class GLScenario
{
public:
	char* name;
	int identifier;

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

#endif