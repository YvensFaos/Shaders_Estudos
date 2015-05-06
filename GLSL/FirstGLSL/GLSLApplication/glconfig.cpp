#include "glconfig.h"

#pragma region players include

#include "glplayer.h"
#include "glfreeplayer.h"
#include "glwalkthroughplayer.h"
#include "glrecordpathplayer.h"
#include "gledemakerplayer.h"

#pragma endregion

GLConfig::GLConfig()
{
	height = 640;
	width  = 480;

	mode = FREE_MODE;
	type = NONE;

	title = "Blank Title";

	scenarioNumber = -1;
	pathIdentifier = 0;
	logIdentifier  = 0;
	edeDepth = 0;

	repeatable = false;
	logResults = false;
	coloredNodes = false;
}

GLConfig::~GLConfig()
{ }

GLPlayer* GLConfig::getGLPlayer(void)
{
	switch(mode)
	{
		case FREE_MODE:        return new GLFreePlayer(*this);
		case WALKTHROUGH_MODE: return new GLWalkthroughPlayer(*this);
		case RECORD_PATH:	   return new GLRecordPathPlayer(*this);
		case EDE_MAKER:        return new GLEDEMakerPlayer(*this);
		default:               return new GLPlayer(*this);
	}
}

GLPlayer* GLConfig::getGLPlayer(PLAYER_MODE mode)
{
	this->mode = mode;
	return getGLPlayer();
}

