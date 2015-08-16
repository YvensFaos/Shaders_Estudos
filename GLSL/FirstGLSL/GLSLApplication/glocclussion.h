#pragma once

#include "glmathhelper.h"

enum OCCLUSION_TYPE
{
	BASE_OCCLUDER, EDE_OCCLUDER
};

class Occluder
{
	glm::vec3 direction;
	glm::vec3* triangles;
};

class OccluderGrouper
{

};