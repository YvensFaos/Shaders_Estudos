#ifndef __GL_CONFIG__
#define __GL_CONFIG__

#include "glenums.h"

class GLConfig
{
public:
	int height;
	int width;

	PLAYER_MODE mode;

	char* title;
public:
	GLConfig(void);
	~GLConfig(void);
};

#endif