#include "glmathhelper.h"

#include "glcamera.h"

#include "openGLWrapper.h"
#include <GL\glew.h>
#include "GLFW\glfw3.h"

//#define DRAW_F_BBOX

//GLPlane

GLPlane::GLPlane(void)
{ }

GLPlane::GLPlane(float a, float b, float c, float d) : n(a,b,c), d(d)
{ }

GLPlane::GLPlane(const glm::vec3 &pt, const glm::vec3 &normal)
{
	fromPointNormal(pt, normal);
}

GLPlane::~GLPlane(void)
{ }

GLPlane::GLPlane(const glm::vec3 &pt1, const glm::vec3 &pt2, const glm::vec3 &pt3)
{
    fromPoints(pt1, pt2, pt3);
}

float GLPlane::dot(const GLPlane &p, const glm::vec3 &pt)
{
    // Returns:
    //  > 0 if the point 'pt' lies in front of the plane 'p'
    //  < 0 if the point 'pt' lies behind the plane 'p'
    //    0 if the point 'pt' lies on the plane 'p'
    //
    // The signed distance from the point 'pt' to the plane 'p' is returned.

    return glm::dot(p.n, pt) + p.d;
}

void GLPlane::fromPointNormal(const glm::vec3 &pt, const glm::vec3 &normal)
{
    set(normal.x, normal.y, normal.z, -glm::dot(normal, pt));
    normalize();
}

void GLPlane::fromPoints(const glm::vec3 &pt1, const glm::vec3 &pt2, const glm::vec3 &pt3)
{
    n = glm::cross(pt2 - pt1, pt3 - pt1);
	n = glm::normalize(n);
    d = -glm::dot(n, pt1);
    //normalize();
}

const glm::vec3 &GLPlane::normal() const
{
    return n;
}

glm::vec3 &GLPlane::normal()
{
    return n;
}

void GLPlane::normalize()
{
	float length = 1.0f / n.length();
    n *= length;
    d *= length;
}

void GLPlane::set(float a, float b, float c, float d)
{
	n.x = a;
	n.y = b;
	n.z = c;
	this->d = d;
}

//GLRay

GLRay::GLRay(void)
{
	dir = glm::vec3(0.0f, 0.0f, 1.0f);
	org = glm::vec3(0.0f, 0.0f, 0.0f);
}

GLRay::GLRay(glm::vec3 dir, glm::vec3 org)
{
	this->dir = glm::vec3(dir);
	this->dir = glm::normalize(this->dir);

	this->org = glm::vec3(org);
}

GLRay::~GLRay(void)
{ }

bool GLRay::intersect(GLAABB* aabb)
{
	glm::vec3 dirfrac;
	glm::vec3 lb = aabb->min;
	glm::vec3 rt = aabb->max;

	// r.dir is unit direction vector of ray
	dirfrac.x = 1.0f / dir.x;
	dirfrac.y = 1.0f / dir.y;
	dirfrac.z = 1.0f / dir.z;
	// lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
	// r.org is origin of ray

	float t1 = (lb.x - org.x)*dirfrac.x;
	float t2 = (rt.x - org.x)*dirfrac.x;
	float t3 = (lb.y - org.y)*dirfrac.y;
	float t4 = (rt.y - org.y)*dirfrac.y;
	float t5 = (lb.z - org.z)*dirfrac.z;
	float t6 = (rt.z - org.z)*dirfrac.z;

	float tmin = MAX(MAX(MIN(t1, t2), MIN(t3, t4)), MIN(t5, t6));
	float tmax = MIN(MIN(MAX(t1, t2), MAX(t3, t4)), MAX(t5, t6));
	float t = 0;

	// if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
	if (tmax < 0)
	{
		t = tmax;
		return false;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
	{
		t = tmax;
		return false;
	}

	t = tmin;

	//Talvez checar o tamanho do T com o tamanho do Frustum

	return true;
}

//GLAABB

GLAABB::GLAABB(glm::vec3 min, glm::vec3 max)
{
	this->min = min;
	this->max = max;
	hasPlanes = false;
}

GLAABB::~GLAABB(void)
{ }

bool GLAABB::intercepts(GLAABB* another)
{
	glm::vec3 anotherMin = glm::vec3(another->min);
	glm::vec3 anotherMax = glm::vec3(another->max);

	return GLAABB::intercepts(this->max, this->min, anotherMax, anotherMin);
}

bool GLAABB::intercepts(glm::vec3 min, glm::vec3 max)
{
	return GLAABB::intercepts(this->max, this->min, max, min);
}

bool GLAABB::interceptsAsPlanes(glm::vec3* corners, int size)
{
	if(!hasPlanes)
	{
		generatePlanes();
	}

	float x = 0;
	float y = 0;
	float z = 0;

	for(int j = 0; j < size; j++)
	{
		x = corners[j].x;
		y = corners[j].y;
		z = corners[j].z;

		for (int i = 0; i < 6; ++i)
		{
			GLPlane* plane = &planes[i];
			float side = plane->n.x * x + plane->n.y * y + plane->n.z * z + plane->d;
			if (side <= 0)
			{
				return true;
			}
		}
	}
	return false;
}

void GLAABB::generatePlanes(void)
{
	glm::vec3 ntl, ftl, fbl, nbr, fbr;

	ntl = glm::vec3(min.x, max.y, min.z);
	fbl = glm::vec3(min.x, min.y, max.z);
	nbr = glm::vec3(max.x, min.y, min.z);
	fbr = glm::vec3(max.x, min.y, max.z);
	ftl = glm::vec3(min.x, max.y, max.z);

	int i = 0;
	planes[i++].fromPoints(min, ntl, fbl);
	planes[i++].fromPoints(fbr, max, nbr);
	planes[i++].fromPoints(fbr, max, fbl);
	planes[i++].fromPoints(min, ntl, nbr);
	planes[i++].fromPoints(min, fbl, nbr);
	planes[i++].fromPoints(max, ftl, ntl);
}

bool GLAABB::intercepts(glm::vec3 max1, glm::vec3 min1, glm::vec3 max2, glm::vec3 min2)
{
	return max1.x >= min2.x 
		&& max1.y >= min2.y
		&& max1.z >= min2.z
		&& min1.x <= max2.x
		&& min1.y <= max2.y
		&& min1.z <= max2.z;
}

void GLAABB::drawAABB(glm::vec3 min, glm::vec3 max, glm::vec3 position)
{
	GLint verticesCount = 36;
	glm::vec3* vertexes = new glm::vec3[verticesCount];
	glm::vec3* normals  = new glm::vec3[verticesCount];

	int i = 0;
	int j = 0;

	glm::vec3 a;
	glm::vec3 b;
	glm::vec3 n;
	glm::vec3 m = glm::vec3(0.0f, 0.0f, 0.0f);

	vertexes[i++] = glm::vec3(min.x, min.y, min.z);
	vertexes[i++] = glm::vec3(min.x, max.y, min.z);
	vertexes[i++] = glm::vec3(min.x, min.y, max.z);


	vertexes[i++] = glm::vec3(min.x, max.y, min.z);
	vertexes[i++] = glm::vec3(min.x, max.y, max.z);
	vertexes[i++] = glm::vec3(min.x, min.y, max.z);

	a = (glm::vec3(min.x, max.y, min.z) - glm::vec3(min.x, min.y, min.z));
	b = (glm::vec3(min.x, min.y, max.z) - glm::vec3(min.x, min.y, min.z));

	n = glm::cross(a,b);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);

	vertexes[i++] = glm::vec3(max.x, min.y, min.z);
	vertexes[i++] = glm::vec3(max.x, max.y, min.z);
	vertexes[i++] = glm::vec3(max.x, min.y, max.z);

	vertexes[i++] = glm::vec3(max.x, max.y, min.z);
	vertexes[i++] = glm::vec3(max.x, max.y, max.z);
	vertexes[i++] = glm::vec3(max.x, min.y, max.z);

	a = (glm::vec3(max.x, max.y, min.z) - glm::vec3(max.x, min.y, min.z));
	b = (glm::vec3(max.x, min.y, max.z) - glm::vec3(max.x, min.y, min.z));

	n = glm::cross(a,b);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);

	vertexes[i++] = glm::vec3(min.x, min.y, min.z);
	vertexes[i++] = glm::vec3(min.x, max.y, min.z);
	vertexes[i++] = glm::vec3(max.x, min.y, min.z);

	vertexes[i++] = glm::vec3(min.x, max.y, min.z);
	vertexes[i++] = glm::vec3(max.x, max.y, min.z);
	vertexes[i++] = glm::vec3(max.x, min.y, min.z);

	a = (glm::vec3(min.x, max.y, min.z) - glm::vec3(min.x, min.y, min.z));
	b = (glm::vec3(max.x, min.y, min.z) - glm::vec3(min.x, min.y, min.z));

	n = glm::cross(a,b);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);

	vertexes[i++] = glm::vec3(min.x, min.y, max.z);
	vertexes[i++] = glm::vec3(min.x, max.y, max.z);
	vertexes[i++] = glm::vec3(max.x, min.y, max.z);

	vertexes[i++] = glm::vec3(min.x, max.y, max.z);
	vertexes[i++] = glm::vec3(max.x, max.y, max.z);
	vertexes[i++] = glm::vec3(max.x, min.y, max.z);

	a = (glm::vec3(min.x, max.y, max.z) - glm::vec3(min.x, min.y, max.z));
	b = (glm::vec3(max.x, min.y, max.z) - glm::vec3(min.x, min.y, max.z));

	n = glm::cross(a,b);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);

	vertexes[i++] = glm::vec3(min.x, min.y, min.z);
	vertexes[i++] = glm::vec3(max.x, min.y, min.z);
	vertexes[i++] = glm::vec3(min.x, min.y, max.z);

	vertexes[i++] = glm::vec3(max.x, min.y, min.z);
	vertexes[i++] = glm::vec3(max.x, min.y, max.z);
	vertexes[i++] = glm::vec3(min.x, min.y, max.z);

	a = (glm::vec3(max.x, min.y, min.z) - glm::vec3(min.x, min.y, min.z));
	b = (glm::vec3(min.x, min.y, max.z) - glm::vec3(min.x, min.y, min.z));

	n = glm::cross(a,b);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);

	vertexes[i++] = glm::vec3(min.x, max.y, min.z);
	vertexes[i++] = glm::vec3(max.x, max.y, min.z);
	vertexes[i++] = glm::vec3(min.x, max.y, max.z);

	vertexes[i++] = glm::vec3(max.x, max.y, min.z);
	vertexes[i++] = glm::vec3(max.x, max.y, max.z);
	vertexes[i++] = glm::vec3(min.x, max.y, max.z);

	a = (glm::vec3(max.x, max.y, min.z) - glm::vec3(min.x, max.y, min.z));
	b = (glm::vec3(min.x, max.y, max.z) - glm::vec3(min.x, max.y, min.z));

	n = glm::cross(a,b);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);
	SET_NORMAL_ARRAY(normals,j,n,m);

	GLint pos = glGetUniformLocation(OpenGLWrapper::programObject, "pos");
	glUniform4f(pos, position.x, position.y, position.z, 0.0f);

	GLint sca = glGetUniformLocation(OpenGLWrapper::programObject, "sca");
	glUniform4f(sca, 1.0f, 1.0f, 1.0f, 1.0f);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertexes);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(OpenGLWrapper::normalLoc, 3, GL_FLOAT, GL_FALSE, 0, normals);

	glDrawArrays(GL_TRIANGLES, 0, verticesCount);
}

//GLFrustum

GLFrustum::GLFrustum(float nearp, float farp, float aspect, GLCameraStep* cameraStep)
{
	float fovx = tan(((cameraStep->fov / 2)*aspect) * PI180);
	float fovy = tan( (cameraStep->fov / 2)*PI180);

	float test = cameraStep->fov*aspect;

	glm::vec3* up = &cameraStep->up;
	glm::vec3* apex = &cameraStep->position;
	glm::vec3* direction = &cameraStep->direction;

	glm::vec3 auxAxis = cameraStep->right;

	glm::vec3 nearPoint = glm::vec3(direction->x, direction->y, direction->z);
	nearPoint *= nearp;
	nearPoint += *apex;
	glm::vec3 farPoint  = glm::vec3(direction->x, direction->y, direction->z);
	farPoint  *= farp;
	farPoint  += *apex;

	float nearValue = nearp*fovx;
	float farValue  = farp*fovx;

	glm::vec3 nearRightPoint = glm::vec3(auxAxis.x, auxAxis.y, auxAxis.z);
	nearRightPoint *= nearValue;
	glm::vec3 nearLeftPoint = glm::vec3(-auxAxis.x, -auxAxis.y, -auxAxis.z);
	nearLeftPoint *= nearValue;
	glm::vec3 farRightPoint = glm::vec3(auxAxis.x, auxAxis.y, auxAxis.z);
	farRightPoint *= farValue;
	glm::vec3 farLeftPoint = glm::vec3(-auxAxis.x, -auxAxis.y, -auxAxis.z);
	farLeftPoint *= farValue;

	nearValue = nearp*fovy;
	farValue = farp*fovy;

	glm::vec3 nearTopPoint = glm::vec3(up->x, up->y, up->z);
	nearTopPoint *= nearValue;
	glm::vec3 nearBottomPoint = glm::vec3(-up->x, -up->y, -up->z);
	nearBottomPoint *= nearValue;
	glm::vec3 farTopPoint = glm::vec3(up->x, up->y, up->z);
	farTopPoint *= farValue;
	glm::vec3 farBottomPoint = glm::vec3(-up->x, -up->y, -up->z);
	farBottomPoint *= farValue;

	glm::vec3 ftl, ftr, fbr, fbl, ntl, ntr, nbr, nbl;

	ftl += farPoint;
	ftl += farLeftPoint;
	ftl += farTopPoint;
	ftr += farPoint;
	ftr += farRightPoint;
	ftr += farTopPoint;
	fbl += farPoint;
	fbl += farLeftPoint;
	fbl += farBottomPoint;
	fbr += farPoint;
	fbr += farRightPoint;
	fbr += farBottomPoint;

	ntl += nearPoint;
	ntl += nearLeftPoint;
	ntl += nearTopPoint;
	ntr += nearPoint;
	ntr += nearRightPoint;
	ntr += nearTopPoint;
	nbl += nearPoint;
	nbl += nearLeftPoint;
	nbl += nearBottomPoint;
	nbr += nearPoint;
	nbr += nearRightPoint;
	nbr += nearBottomPoint;

	planes[PLANE_RIGHT].fromPoints(ntr, fbr, ftr);
	planes[PLANE_RIGHT].normalize();
	planes[PLANE_LEFT].fromPoints(nbl, ftl, fbl);
	planes[PLANE_LEFT].normalize();
	planes[PLANE_BOTTOM].fromPoints(nbr, fbl, fbr);
	planes[PLANE_BOTTOM].normalize();
	planes[PLANE_TOP].fromPoints(ntl, ftr, ftl);
	planes[PLANE_TOP].normalize();
	planes[PLANE_FAR].fromPoints(fbl, ftl, ftr);
	planes[PLANE_FAR].normalize();
	planes[PLANE_NEAR].fromPoints(nbr, ntr, ntl);
	planes[PLANE_NEAR].normalize();

	corners[FRUSTUM_FTL] = ftl;
	corners[FRUSTUM_FTR] = ftr;
	corners[FRUSTUM_FBL] = fbl;
	corners[FRUSTUM_FBR] = fbr;
	corners[FRUSTUM_NTL] = ntl;
	corners[FRUSTUM_NTR] = ntr;
	corners[FRUSTUM_NBL] = nbl;
	corners[FRUSTUM_NBR] = nbr;

	//generateRays();
	generateAABB();
}

GLFrustum::GLFrustum(float aspect, GLCamera* camera)
{
	float fovx = tan(((camera->fov / 2)*aspect) * PI180);
	float fovy = tan( (camera->fov / 2)*PI180);

	glm::vec3* up = &camera->up;
	glm::vec3* apex = &camera->position;
	glm::vec3* direction = &camera->direction;

	glm::vec3 auxAxis = camera->right;

	glm::vec3 nearPoint = glm::vec3(direction->x, direction->y, direction->z);
	nearPoint *= camera->near;
	nearPoint += *apex;
	glm::vec3 farPoint  = glm::vec3(direction->x, direction->y, direction->z);
	farPoint  *= camera->far;
	farPoint  += *apex;

	float nearValue = camera->near*fovx;
	float farValue  = camera->far*fovx;

	glm::vec3 nearRightPoint = glm::vec3(auxAxis.x, auxAxis.y, auxAxis.z);
	nearRightPoint *= nearValue;
	glm::vec3 nearLeftPoint = glm::vec3(-auxAxis.x, -auxAxis.y, -auxAxis.z);
	nearLeftPoint *= nearValue;
	glm::vec3 farRightPoint = glm::vec3(auxAxis.x, auxAxis.y, auxAxis.z);
	farRightPoint *= farValue;
	glm::vec3 farLeftPoint = glm::vec3(-auxAxis.x, -auxAxis.y, -auxAxis.z);
	farLeftPoint *= farValue;

	nearValue = camera->near*fovy;
	farValue = camera->far*fovy;

	glm::vec3 nearTopPoint = glm::vec3(up->x, up->y, up->z);
	nearTopPoint *= nearValue;
	glm::vec3 nearBottomPoint = glm::vec3(-up->x, -up->y, -up->z);
	nearBottomPoint *= nearValue;
	glm::vec3 farTopPoint = glm::vec3(up->x, up->y, up->z);
	farTopPoint *= farValue;
	glm::vec3 farBottomPoint = glm::vec3(-up->x, -up->y, -up->z);
	farBottomPoint *= farValue;

	glm::vec3 ftl, ftr, fbr, fbl, ntl, ntr, nbr, nbl;

	ftl += farPoint;
	ftl += farLeftPoint;
	ftl += farTopPoint;
	ftr += farPoint;
	ftr += farRightPoint;
	ftr += farTopPoint;
	fbl += farPoint;
	fbl += farLeftPoint;
	fbl += farBottomPoint;
	fbr += farPoint;
	fbr += farRightPoint;
	fbr += farBottomPoint;

	ntl += nearPoint;
	ntl += nearLeftPoint;
	ntl += nearTopPoint;
	ntr += nearPoint;
	ntr += nearRightPoint;
	ntr += nearTopPoint;
	nbl += nearPoint;
	nbl += nearLeftPoint;
	nbl += nearBottomPoint;
	nbr += nearPoint;
	nbr += nearRightPoint;
	nbr += nearBottomPoint;

	planes[PLANE_RIGHT].fromPoints(ntr, fbr, ftr);
	planes[PLANE_RIGHT].normalize();
	planes[PLANE_LEFT].fromPoints(nbl, ftl, fbl);
	planes[PLANE_LEFT].normalize();
	planes[PLANE_BOTTOM].fromPoints(nbr, fbl, fbr);
	planes[PLANE_BOTTOM].normalize();
	planes[PLANE_TOP].fromPoints(ntl, ftr, ftl);
	planes[PLANE_TOP].normalize();
	planes[PLANE_FAR].fromPoints(fbl, ftl, ftr);
	planes[PLANE_FAR].normalize();
	planes[PLANE_NEAR].fromPoints(nbr, ntr, ntl);
	planes[PLANE_NEAR].normalize();

	corners[FRUSTUM_FTL] = ftl;
	corners[FRUSTUM_FTR] = ftr;
	corners[FRUSTUM_FBL] = fbl;
	corners[FRUSTUM_FBR] = fbr;
	corners[FRUSTUM_NTL] = ntl;
	corners[FRUSTUM_NTR] = ntr;
	corners[FRUSTUM_NBL] = nbl;
	corners[FRUSTUM_NBR] = nbr;

	//generateRays();
	generateAABB();
}

GLFrustum::~GLFrustum(void)
{ }

bool GLFrustum::containsAnyVertexOf(glm::vec3* min, glm::vec3* max)
{
	glm::vec3* corners[2] = {min, max};
	int cx = 0, cy = 0, cz = 0;
	for (int i = 0; i < 8; i++)
	{
		if (containsPoint(corners[cx]->x, corners[cy]->y, corners[cz]->z))
		{
			return true;
		}
		if (!(cz ^= 1))
		{
			if (!(cy ^= 1))
			{
				cx ^= 1;
			}
		}
	}
	return false;
}

bool GLFrustum::containsPoint(float x, float y, float z)
{
	for (int i = 0; i < 6; ++i)
	{
		GLPlane* plane = &planes[i];
		float side = plane->n.x * x + plane->n.y * y + plane->n.z * z + plane->d;
		if (side <= 0)
		{
			return false;
		}
	}
	return true;
}

bool GLFrustum::containsSphere(glm::vec3* center, float radius)
{
	for (int i = 0; i < 6; ++i)
	{
		GLPlane* plane = &planes[i];
		float dist = plane->n.x * center->x + plane->n.y * center->y + plane->n.z * center->z + plane->d;
		if (dist <= radius)
		{
			return false;
		}
	}
	return true;
}

bool GLFrustum::intercepts(GLAABB* aabb)
{
	return intercepts(&aabb->min, &aabb->max);
}

bool GLFrustum::intercepts(glm::vec3* min, glm::vec3* max)
{

	bool found = containsAnyVertexOf(min, max);

	if(!found)
	{
		GLAABB* aabb = new GLAABB(*min, *max);

		////colocar o teste de raio aqui
		//for(int i = 0; i < 4; i++)
		//{
		//	if(rays[i].intersect(aabb))
		//	{
		//		found = true;
		//	}
		//}

		found = aabb->intercepts(this->min, this->max);

		delete aabb;
	}

	return found;
}

void GLFrustum::draw(void)
{
	GLint verticesCount = 36;
	glm::vec3* vertexes = new glm::vec3[verticesCount];
	glm::vec3* normals  = new glm::vec3[verticesCount];
	glm::vec3 m = glm::vec3(0.0f, 0.0f, 0.0f);

	int i = 0;
	int j = 0;

#pragma region near
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_NBL]);
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_NTL]);
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_NBR]);

	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_NTL]);
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_NTR]);
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_NBR]);

	for(int k = 0; k < 6; k++)
	{
		SET_NORMAL_ARRAY(normals,j,planes[PLANE_NEAR].n, m);
	}
#pragma endregion

#pragma region far
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_FBL]);
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_FTL]);
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_FBR]);

	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_FTL]);
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_FTR]);
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_FBR]);

	for(int k = 0; k < 6; k++)
	{
		SET_NORMAL_ARRAY(normals,j,planes[PLANE_FAR].n, m);
	}
#pragma endregion

#pragma region left
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_NBL]);
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_NTL]);
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_FBL]);

	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_NTL]);
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_FTL]);
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_FBL]);

	for(int k = 0; k < 6; k++)
	{
		SET_NORMAL_ARRAY(normals,j,planes[PLANE_LEFT].n, m);
	}
#pragma endregion

#pragma region right
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_NBR]);
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_NTR]);
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_FBR]);

	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_NTR]);
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_FTR]);
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_FBR]);

	for(int k = 0; k < 6; k++)
	{
		SET_NORMAL_ARRAY(normals,j,planes[PLANE_RIGHT].n, m);
	}
#pragma endregion

#pragma region bottom
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_NBL]);
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_FBL]);
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_NBR]);

	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_FBL]);
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_FBR]);
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_NBR]);

	for(int k = 0; k < 6; k++)
	{
		SET_NORMAL_ARRAY(normals,j,planes[PLANE_BOTTOM].n, m);
	}
#pragma endregion

#pragma region top
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_NTL]);
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_FTL]);
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_NTR]);

	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_FTL]);
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_FTR]);
	SET_VERTEX_ARRAY(vertexes,i,corners[FRUSTUM_NTR]);

	for(int k = 0; k < 6; k++)
	{
		SET_NORMAL_ARRAY(normals,j,planes[PLANE_TOP].n, m);
	}
#pragma endregion

	GLint loc = glGetUniformLocation(OpenGLWrapper::programObject, "baseColor");
	glUniform4f(loc, 0.78f, 0.004f, 0.1025f, 0.4f);

	GLint poss = glGetUniformLocation(OpenGLWrapper::programObject, "pos");
	glUniform4f(poss, 0.0f, 0.0f, 0.0f, 0.0f);

	GLint sca = glGetUniformLocation(OpenGLWrapper::programObject, "sca");
	glUniform4f(sca, 1.0f, 1.0f, 1.0f, 1.0f);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertexes);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(OpenGLWrapper::normalLoc, 3, GL_FLOAT, GL_FALSE, 0, normals);

	glDrawArrays(GL_TRIANGLES, 0, verticesCount);

#ifdef DRAW_F_BBOX
	glm::vec3* lmin = new glm::vec3(MAX_FLOAT);
	glm::vec3* lmax = new glm::vec3(MIN_FLOAT);
	
	j = 0;
	while(j < 8)
	{
		if(lmin->x >= corners[j].x)
		{
			lmin->x = corners[j].x;
		}
		if(lmax->x <= corners[j].x)
		{
			lmax->x = corners[j].x;
		}
		if(lmin->y >= corners[j].y)
		{
			lmin->y = corners[j].y;
		}
		if(lmax->y <= corners[j].y)
		{
			lmax->y = corners[j].y;
		}
		if(lmin->z >= corners[j].z)
		{
			lmin->z = corners[j].z;
		}
		if(lmax->z <= corners[j].z)
		{
			lmax->z = corners[j].z;
		}
		j++;
	}

	printf("Min: %4.2f %4.2f %4.2f = Max: %4.2f %4.2f %4.2f\n", VEC3P_PRINT(lmin), VEC3P_PRINT(lmax));
	GLAABB::drawAABB(*lmin, *lmax, glm::vec3(0.0f, 0.0f, 0.0f));

	delete lmin;
	delete lmax;
#endif
}

void GLFrustum::generateRays(void)
{
	int i = 0;
	rays[i++] = GLRay(corners[FRUSTUM_FBL] - corners[FRUSTUM_NBL], corners[FRUSTUM_NBL]);
	rays[i++] = GLRay(corners[FRUSTUM_FTL] - corners[FRUSTUM_NTL], corners[FRUSTUM_NTL]);
	rays[i++] = GLRay(corners[FRUSTUM_FBR] - corners[FRUSTUM_NBR], corners[FRUSTUM_NBR]);
	rays[i++] = GLRay(corners[FRUSTUM_FTR] - corners[FRUSTUM_NTR], corners[FRUSTUM_NTR]);
}

void GLFrustum::generateAABB(void)
{
	min = glm::vec3(MAX_FLOAT, MAX_FLOAT, MAX_FLOAT);
	max = glm::vec3(MIN_FLOAT, MIN_FLOAT, MIN_FLOAT);

	glm::vec3* corner;
	for(int i = 0; i < 8; i++)
	{
		corner = &corners[i];

		if(min.x > corner->x)
		{
			min.x = corner->x;
		}
		if(min.y > corner->y)
		{
			min.y = corner->y;
		}
		if(min.z > corner->z)
		{
			min.z = corner->z;
		}

		if(max.x < corner->x)
		{
			max.x = corner->x;
		}
		if(max.y < corner->y)
		{
			max.y = corner->y;
		}
		if(max.z < corner->z)
		{
			max.z = corner->z;
		}
	}
}