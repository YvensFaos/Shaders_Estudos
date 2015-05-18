#include "gldynamic.h"

#include "glbuffer.h"

GLDynamic::GLDynamic(void)
{ 
	index = -1;
	moveable = false;
	standPosition = glm::vec3(0.0f, 0.0f, 0.0f);
}

GLDynamic::GLDynamic(char* modelPath, char* model3d, char* pathlocation, char* pathName)
{ 
	if(GLBufferHandler::checkForMeshHandler(model3d))
	{
		handler = new GLMeshHandler(model3d, modelPath);
	}

	index = -1;
	moveable = false;
	standPosition = glm::vec3(0.0f, 0.0f, 0.0f);
}



