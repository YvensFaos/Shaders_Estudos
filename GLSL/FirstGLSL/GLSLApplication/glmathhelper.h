#pragma once

#include "glm\glm.hpp"

//Forward
class GLCamera;

#define VEC3_PRINT(v) v.x,v.y,v.z
#define VEC3P_PRINT(v) v->x,v->y,v->z
#define VEC4_PRINT(v) v.x,v.y,v.z,v.w

#define VEC3_EQUALS(v,u)  (fabs(v[0] - u[0]) < 0.001f) && (fabs(v[1] - u[1]) < 0.001f) && (fabs(v[2] - u[2]) < 0.001f)
#define VEC4_EQUALS(v,u)  (fabs(v[0] - u[0]) < 0.001f) && (fabs(v[1] - u[1]) < 0.001f) && (fabs(v[2] - u[2]) < 0.001f) && (fabs(v[3] - u[3]) < 0.001f)

#define XAXIS glm::vec3(1.0f,0.0f,0.0f)
#define YAXIS glm::vec3(0.0f,1.0f,0.0f)
#define ZAXIS glm::vec3(0.0f,0.0f,1.0f)

#define MAX_FLOAT 2.0e10
#define MIN_FLOAT 2.0e-10

#define PI180 3.14159265f/180.0f

#define FRUSTUM_FTL 0
#define FRUSTUM_FTR 1
#define FRUSTUM_FBL 2
#define FRUSTUM_FBR 3
#define FRUSTUM_NTL 4
#define FRUSTUM_NTR 5
#define FRUSTUM_NBL 6
#define FRUSTUM_NBR 7

#define PLANE_NEAR 0
#define PLANE_LEFT 1
#define PLANE_TOP 2
#define PLANE_RIGHT 3
#define PLANE_BOTTOM 4
#define PLANE_FAR 5

//Peguei em http://voxelengine.googlecode.com/svn/trunk/Plane.cpp
class GLPlane
{
public:
	glm::vec3 n;
	float d;

public:
	GLPlane();
	~GLPlane();

	GLPlane(float a, float b, float c, float d);
	GLPlane(const glm::vec3 &pt, const glm::vec3 &normal);
	GLPlane(const glm::vec3 &pt1, const glm::vec3 &pt2, const glm::vec3 &pt3);

public:
	const glm::vec3 &normal() const;
	glm::vec3 &normal();
	void normalize();
	void set(float a, float b, float c, float d);
	void fromPointNormal(const glm::vec3 &pt, const glm::vec3 &normal);
	void fromPoints(const glm::vec3 &pt1, const glm::vec3 &pt2, const glm::vec3 &pt3);

public:
	static float dot(const GLPlane &p, const glm::vec3 &pt);
};

class GLFrustum
{
public:
	glm::vec3 corners[8];
	GLPlane planes[6];

public:
	GLFrustum(void) {};
	GLFrustum::GLFrustum(float fov, float aspect, float nearp, float farp, GLCamera* camera);
	~GLFrustum(void);

	//Testa um ponto no espa�o
	bool containsPoint(float x, float y, float z);
	//Testa todos os v�rtices de uma AABB
	bool containsAnyVertexOf(glm::vec3* min, glm::vec3* max);
	//Testa uma esfera
	bool containsSphere(glm::vec3* center, float radius);
	//Testa se o frustum intercepta a AABB
	bool intercepts(glm::vec3* min, glm::vec3* max);
};