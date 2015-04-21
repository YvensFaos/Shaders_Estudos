#include "glconfig.h"

#pragma region players include

#include "glplayer.h"
#include "glfreeplayer.h"
#include "glwalkthroughplayer.h"

#pragma endregion

GLConfig::GLConfig()
{
	height = 640;
	width  = 480;

	mode = FREE_MODE;

	title = "Blank Title";

	scenarioNumber = -1;
}

GLConfig::~GLConfig()
{ }

GLPlayer* GLConfig::getGLPlayer(PLAYER_MODE mode)
{
	this->mode = mode;
	switch(mode)
	{
		case FREE_MODE:        return new GLFreePlayer(*this);
		case WALKTHROUGH_MODE: return new GLWalkthroughPlayer(*this);
		default:               return new GLPlayer(*this);
	}
}