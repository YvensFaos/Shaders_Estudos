#pragma once

#include "glm\glm.hpp"

#define VEC3_PRINT(v) v.x,v.y,v.z

#define XAXIS glm::vec3(1.0f,0.0f,0.0f)
#define YAXIS glm::vec3(0.0f,1.0f,0.0f)
#define ZAXIS glm::vec3(0.0f,0.0f,1.0f)

#define PI180 3.14159265f/180.0f

bool operator==(const glm::vec3 &vecA, const glm::vec3 &vecB) 
{ 
   const double epsilion = 0.0001;  // choose something apprpriate.

   return    fabs(vecA[0] -vecB[0]) < epsilion   
          && fabs(vecA[1] -vecB[1]) < epsilion   
          && fabs(vecA[2] -vecB[2]) < epsilion;
} 