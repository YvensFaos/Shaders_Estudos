#pragma once

enum PLAYER_MODE
{
	FREE_MODE, WALKTHROUGH_MODE, RECORD_PATH
};

#define XAXIS glm::vec3(1.0f,0.0f,0.0f)
#define YAXIS glm::vec3(0.0f,1.0f,0.0f)
#define ZAXIS glm::vec3(0.0f,0.0f,1.0f)

#define PI180 3.14159265f/180.0f