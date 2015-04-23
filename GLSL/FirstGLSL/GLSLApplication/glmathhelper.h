#pragma once

#include "glm\glm.hpp"

#define VEC3_PRINT(v) v.x,v.y,v.z
#define VEC4_PRINT(v) v.x,v.y,v.z,v.w

#define VEC3_EQUALS(v,u)  (fabs(v[0] - u[0]) < 0.001f) && (fabs(v[1] - u[1]) < 0.001f) && (fabs(v[2] - u[2]) < 0.001f)
#define VEC4_EQUALS(v,u)  (fabs(v[0] - u[0]) < 0.001f) && (fabs(v[1] - u[1]) < 0.001f) && (fabs(v[2] - u[2]) < 0.001f) && (fabs(v[3] - u[3]) < 0.001f)

#define XAXIS glm::vec3(1.0f,0.0f,0.0f)
#define YAXIS glm::vec3(0.0f,1.0f,0.0f)
#define ZAXIS glm::vec3(0.0f,0.0f,1.0f)

#define PI180 3.14159265f/180.0f