#pragma once

#pragma region define cenarios

#define SAW config.objectName = "saw.obj"; \
			config.scenarioNumber = 10; \
			config.pathfileName = "saw-[walk]-[1]"; \
			int qttDynamics = 1; \
			std::vector<GLDynamicObject>* pointerDynamics; \
			pointerDynamics = GLDynamic::generateDynamics(modelPath, "bunny.obj",  modelPathLocation, "saw-[walk]-[1].pathx", qttDynamics, dynamicTrans, dynamicScale); \
			totalDynamics.insert(totalDynamics.end(), pointerDynamics->begin(), pointerDynamics->end()); \
			config.dynamics = totalDynamics;

#define CITYCYCLES config.objectName = "citycycles.obj"; \
				 config.scenarioNumber = 11; \
				 config.pathfileName = "citycycles-[walk]"; \
				 dynamicScale = glm::vec3(0.2f, 0.2f, 0.2f); \
				 int qttDynamics = 10; \
				 std::vector<GLDynamicObject>* pointerDynamics; \
				 pointerDynamics = GLDynamic::generateDynamics(modelPath, "bunny.obj",  modelPathLocation, "citycycles-[walk]-[1][[dyn]].pathx", qttDynamics, dynamicTrans, dynamicScale); \
				 totalDynamics.insert(totalDynamics.end(), pointerDynamics->begin(), pointerDynamics->end()); \
				 pointerDynamics = GLDynamic::generateDynamics(modelPath, "bunny.obj",  modelPathLocation, "citycycles-[walk]-[2][[dyn]].pathx", qttDynamics, dynamicTrans, dynamicScale); \
				 totalDynamics.insert(totalDynamics.end(), pointerDynamics->begin(), pointerDynamics->end()); \
				 pointerDynamics = GLDynamic::generateDynamics(modelPath, "bunny.obj",  modelPathLocation, "citycycles-[walk]-[3][[dyn]].pathx", qttDynamics, dynamicTrans, dynamicScale); \
				 totalDynamics.insert(totalDynamics.end(), pointerDynamics->begin(), pointerDynamics->end()); \
				 pointerDynamics = GLDynamic::generateDynamics(modelPath, "bunny.obj",  modelPathLocation, "citycycles-[walk]-[4][[dyn]].pathx", qttDynamics, dynamicTrans, dynamicScale); \
				 totalDynamics.insert(totalDynamics.end(), pointerDynamics->begin(), pointerDynamics->end()); \
				 config.dynamics = totalDynamics;

#define RAVINE config.objectName = "ravine.obj"; \
				 config.scenarioNumber = 13; \
				 config.pathfileName = "ravine-[walk]-[1]"; \
				 int qttDynamics = 0;

#define VIADUCT config.objectName= "viaduct.obj"; \
				config.scenarioNumber = 14; \
				config.pathfileName = "viaduct-[walk]-[1]"; \
				 int qttDynamics = 0;

#define COALTOWN config.objectName = "coaltown.obj"; \
				 config.scenarioNumber = 17; \
				 config.pathfileName = "coaltown-[walk]"; \
			     int qttDynamics = 10; \
				 std::vector<GLDynamicObject>* pointerDynamics; \
				 pointerDynamics = GLDynamic::generateDynamics(modelPath, "bunny.obj",  modelPathLocation, "coaltown-[1][test]-[1][dynl].pathx", qttDynamics, dynamicTrans, dynamicScale); \
				 totalDynamics.insert(totalDynamics.end(), pointerDynamics->begin(), pointerDynamics->end()); \
				 pointerDynamics = GLDynamic::generateDynamics(modelPath, "bunny.obj",  modelPathLocation, "coaltown-[1][test]-[2][dynl].pathx", qttDynamics, dynamicTrans, dynamicScale); \
				 totalDynamics.insert(totalDynamics.end(), pointerDynamics->begin(), pointerDynamics->end()); \
				 pointerDynamics = GLDynamic::generateDynamics(modelPath, "bunny.obj",  modelPathLocation, "coaltown-[1][test]-[3][dynl].pathx", qttDynamics, dynamicTrans, dynamicScale); \
				 totalDynamics.insert(totalDynamics.end(), pointerDynamics->begin(), pointerDynamics->end()); \
				 pointerDynamics = GLDynamic::generateDynamics(modelPath, "bunny.obj",  modelPathLocation, "coaltown-[1][test]-[4][dynl].pathx", qttDynamics, dynamicTrans, dynamicScale); \
				 totalDynamics.insert(totalDynamics.end(), pointerDynamics->begin(), pointerDynamics->end()); \
				 config.dynamics = totalDynamics;

#define GOLDRUSH config.objectName = "goldrush.obj"; \
				 config.scenarioNumber = 18; \
				 config.pathfileName = "goldrush-[walk]-[1]"; \
				 int qttDynamics = 0;
#pragma endregion

//Macro de teste
#define TEST_BODY(modeM,t,aa) \
    config.coloredNodes = false; \
	config.enableDynamics = false; \
	config.edeTestDynamics = false; \
	config.frustumTestDynamics = true; \
	config.title = t; \
	PLAYER_MODE mode = modeM; \
	player = config.getGLPlayer(mode); \
	OpenGLWrapper::player = player; \
	OpenGLWrapper::initialize(true, aa); \
	OpenGLWrapper::glLoop();
