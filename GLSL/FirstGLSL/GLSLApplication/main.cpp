#include "openGLWrapper.h"

#include "glplayer.h"
#include "glshaderloader.h"
#include "glenums.h"
#include "glbatch.h"
#include "glbuffer.h"

#include "glscenariomacros.h"

#include <stdio.h>
#include <stdlib.h>

//#define mia
#define desktop
//#define notebook

//#define individual
//#define batch
//#define recordPath
#define cleanBuild

GLPlayer* player;

int main()
{
	GLConfig config;

	glm::vec3 dynamicScale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 dynamicTrans = glm::vec3(0.0f, 0.0f, 0.0f);

	std::string modelPath;
	std::string modelPathLocation;

	int antialiasing = 0;

	config.width = 800;
	config.height = 600;
	config.aspect = (float)config.width / config.height;
	config.title = "Teste";
	config.fov = 45.0f;
	config.far = 500.0f;
	config.near = 0.1f;

	std::vector<GLDynamicObject> totalDynamics = std::vector<GLDynamicObject>();

	//Identifica qual a maquina que rodará os testes
#pragma region definir a maquina de teste
#ifdef mia
	config.objectPath =   "X:/Yvens Rebouças/GIT/Shaders_Estudos/Models/";
	config.pathfilePath = "X:/Yvens Rebouças/GIT/Shaders_Estudos/Paths/";
	config.logPath =      "X:/Yvens Rebouças/GIT/Shaders_Estudos/Logs/Log Mia/";
	modelPath =           "X:/Yvens Rebouças/GIT/Shaders_Estudos/Models/";
	modelPathLocation =   "X:/Yvens Rebouças/GIT/Shaders_Estudos/Paths/";
#endif
#ifdef desktop
	config.objectPath = "F:/Repositorios/Shaders_Estudos/Models/";
	config.pathfilePath = "F:/Repositorios/Shaders_Estudos/Paths/";
	config.logPath = "F:/Repositorios/Shaders_Estudos/Logs/Log Desktop/";
	modelPath = "F:/Repositorios/Shaders_Estudos/Models/";
	modelPathLocation = "F:/Repositorios/Shaders_Estudos/Paths/";
#endif
#ifdef notebook
	config.objectPath =   "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Models/";
	config.pathfilePath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Paths/";
	config.logPath =      "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Logs/Log Notebook/";
	modelPath =           "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Models/";
	modelPathLocation =   "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Paths/";
#endif
#pragma endregion

	//Macro do cenário:
	COALTOWN;
	//GOLDRUSH;
	//SAW;
	//RAVINE;
	//CITYCYCLES;

#ifdef individual
	config.repeatable = false;
	config.logResults = true;
	config.pathIdentifier = 1;

	int numberTests = 2;
	config.edeDepth = 3;
	config.logExtraMsg = "mia-release";
	for (int i = 0; i < numberTests; i++)
	{
		config.logIdentifier = i + 1;
		config.type = ROCTREE;
		TEST_BODY(WALKTHROUGH_MODE, "roctree", antialiasing);
	}
	for(int i = 0; i < numberTests; i++)
	{
		config.logIdentifier = i + 1;
		config.type = SOCTREE;
		TEST_BODY(WALKTHROUGH_MODE, "soctree", antialiasing);
	}
	for (int i = 0; i < numberTests; i++)
	{
		config.logIdentifier = i + 1;
		config.type = OCTREE;

		TEST_BODY(WALKTHROUGH_MODE, "octree", antialiasing);
	}
	for (int i = 0; i < numberTests; i++)
	{
		config.logIdentifier = i + 1;
		config.type = BASEGRID;
		config.edeDepth = 6; //64 nós
		TEST_BODY(WALKTHROUGH_MODE, "basegrid", antialiasing);
	}
	for (int i = 0; i < numberTests; i++)
	{
		config.logIdentifier = i + 1;
		config.type = NONE;

		TEST_BODY(WALKTHROUGH_MODE, "none", antialiasing);
	}
#endif

	//TODO precisa ser atualizado!
#ifdef batch
	char batchPath[128];

#ifdef mia
	//TODO ajustar
	sprintf(batchPath, "F:/Repositorios/Shaders_Estudos/Batch/Batch Desktop/");
#endif
#ifdef desktop
	sprintf(batchPath, "F:/Repositorios/Shaders_Estudos/Batch/Batch Desktop/");
#endif
#ifdef notebook
	sprintf(batchPath, "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Batch/Batch Notebook/");
#endif

	std::vector<char*> batchFiles;
	batchFiles.push_back("octrees-nos");

	GLBatch* batchFile;
	for(int i = 0; i < batchFiles.size(); i++)
	{
		batchFile = new GLBatch(batchPath, batchFiles[i]);
		std::vector<GLConfig> configurations;
		batchFile->getTestBatch(&configurations);

		while(configurations.size() > 0)
		{
			GLConfig* pointer = &configurations.at(0);

			player = pointer->getGLPlayer();
			OpenGLWrapper::player = player;
			OpenGLWrapper::initialize(true, 4);
			OpenGLWrapper::glLoop();

			configurations.erase(configurations.begin());
		}

		delete batchFile;
	}
#endif

#ifdef recordPath
	std::string modelPath;
	std::string modelPathLocation;

	//Configurações de PATH que são exclusivos do computador usado
#ifdef mia
	config.objectPath = "X:/Yvens Rebouças/GIT/Shaders_Estudos/Models/";
	config.pathfilePath = "X:/Yvens Rebouças/GIT/Shaders_Estudos/Paths/";
	config.logPath = "X:/Yvens Rebouças/GIT/Shaders_Estudos/Logs/Log Mia/";
	modelPath = "X:/Yvens Rebouças/GIT/Shaders_Estudos/Models/";
	modelPathLocation = "X:/Yvens Rebouças/GIT/Shaders_Estudos/Paths/";
#endif
#ifdef desktop
	config.objectPath = "F:/Repositorios/Shaders_Estudos/Models/";
	config.pathfilePath = "F:/Repositorios/Shaders_Estudos/Paths/";
	config.logPath = "F:/Repositorios/Shaders_Estudos/Logs/Log Desktop/";
	modelPath = "F:/Repositorios/Shaders_Estudos/Models/";
	modelPathLocation = "F:/Repositorios/Shaders_Estudos/Paths/";
#endif
#ifdef notebook
	config.objectPath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Models/";
	config.pathfilePath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Paths/";
	config.logPath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Logs/Log Notebook/";
	modelPath = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Models/";
	modelPathLocation = "C:/Users/Yvens/Documents/GitHub/Shaders_Estudos/Paths/";
#endif

	config.repeatable = false;
	config.logResults = false;

	config.pathIdentifier = 1;
	config.pathExtraMsg = "[dyn]";
	config.logExtraMsg = "test-mia-comparacao-x";
	config.logIdentifier = 0;
	config.edeDepth = 6;
	config.type = NONE;

	TEST_BODY(WALKTHROUGH_MODE, "teste", 0);
	//TEST_BODY(RECORD_PATH, "teste", 0);
	//TEST_BODY(BIRDS_EYE, "teste", 0);
	/*for (int i = 0; i < 1; i++)
	{
	config.logIdentifier = i + 1;
	TEST_BODY(WALKTHROUGH_MODE, "teste", 0);
	}*/

	//TEST_BODY(WALKTHROUGH_MODE, "teste", 0);
#endif

#ifdef cleanBuild
	config.repeatable = false;
	config.logResults = false;

	config.pathIdentifier = 1;
	config.pathExtraMsg = "[dyn]";
	config.logExtraMsg = "clean";
	config.logIdentifier = 0;
	config.edeDepth = 6;
	config.type = BASEGRID;

	config.addIndexes(546);

	config.coloredNodes = true;
	config.enableDynamics = false;
	config.edeTestDynamics = false;
	config.frustumTestDynamics = true;
	config.title = "Clean Build";

	PLAYER_MODE mode = SEQUENTIAL_RECORDER;
	player = config.getGLPlayer(mode);
	OpenGLWrapper::player = player;
	OpenGLWrapper::initialize(true, antialiasing);
	OpenGLWrapper::glLoop();
#endif

	GLBufferHandler::clearBuffers();
}