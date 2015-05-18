#pragma once

#include "glplayer.h"
#include "gldynamic.h"

#include <vector>

//GL Batch File = gbf
#define BATCH_TYPE ".gbf"

class GLBatch
{
private:
	GLPlayer* player;
	char* batchPath;
	char* batchFilename;
public:
	int batchTestSize;

	GLBatch(void);
	GLBatch(char* batchPath, char* batchFilename);
	~GLBatch(void);

	void getTestBatch(std::vector<GLConfig>* configurations);
	
private:
	EDE_TYPE getEdetype(char* edename);
	PLAYER_MODE getPlayermode(char* modetype);
};