#include "glconfig.h"

#pragma region players include

#include "glplayer.h"
#include "glfreeplayer.h"
#include "glwalkthroughplayer.h"
#include "glrecordpathplayer.h"
#include "gledemakerplayer.h"
#include "glbirdseyeplayer.h"
#include "glsequentialrecorderplayer.h"

#include <algorithm>

#pragma endregion

GLConfig::GLConfig()
{
	height = 640;
	width = 480;

	mode = FREE_MODE;
	type = NONE;

	title = "Blank Title";

	scenarioNumber = -1;
	pathIdentifier = 0;
	logIdentifier = 0;
	edeDepth = 0;

	repeatable = false;
	logResults = false;
	coloredNodes = false;
	enableDynamics = false;
	edeTestDynamics = false;
	frustumTestDynamics = true;
}

GLConfig::~GLConfig()
{ }

GLPlayer* GLConfig::getGLPlayer(void)
{
	switch (mode)
	{
	case FREE_MODE:				return new GLFreePlayer(*this);
	case WALKTHROUGH_MODE:		return new GLWalkthroughPlayer(*this);
	case RECORD_PATH:			return new GLRecordPathPlayer(*this);
	case EDE_MAKER:				return new GLEDEMakerPlayer(*this);
	case BIRDS_EYE:				return new GLBirdsEyePlayer(*this);
	case SEQUENTIAL_RECORDER:	return new GLSequentialRecorderPlayer(*this);
	default:					return new GLPlayer(*this);
	}
}

GLPlayer* GLConfig::getGLPlayer(PLAYER_MODE mode)
{
	this->mode = mode;
	return getGLPlayer();
}

//Adiciona um index ao vetor de indexes a serem gravados
void GLConfig::addIndexes(int value)
{
	addIndexes(value, value);
}

//Adiciona um range de indexes ao vetor de indexes a serem gravados
void GLConfig::addIndexes(int start, int finish)
{
	if (start == finish)
	{
		//Mesmo valor
		recordingIndexes.push_back(start);
	}
	else
	{
		if (start > finish)
		{
			addIndexes(finish, start);
		}
		else
		{
			for (int i = start; i <= finish; i++)
			{
				recordingIndexes.push_back(i);
			}
		}
	}

	//Ordena
	std::sort(recordingIndexes.begin(), recordingIndexes.begin() + recordingIndexes.size());
}