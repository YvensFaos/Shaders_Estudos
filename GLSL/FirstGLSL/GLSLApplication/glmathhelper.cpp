#include "glmathhelper.h"

#include "glcamera.h"

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
    d = -glm::dot(n, pt1);
    normalize();
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

//GLFrustum

GLFrustum::GLFrustum(glm::mat4* mvp)
{
	//FAZER
}

GLFrustum::GLFrustum(float fov, float aspect, float nearp, float farp, GLCamera* camera)
{
	float angle = fov/2;
	angle = angle*PI180;

	glm::vec3* up = &camera->up;
	glm::vec3* apex = &camera->position;
	glm::vec3* direction = &camera->direction;

	glm::vec3 auxAxis;
	auxAxis = glm::cross(*up, *direction);

	glm::vec3 nearPoint = glm::vec3(direction->x, direction->y, direction->z);
	nearPoint *= nearp;
	nearPoint += *apex;
	glm::vec3 farPoint  = glm::vec3(direction->x, direction->y, direction->z);
	farPoint  *= farp;
	farPoint  += *apex;

	float nearValue = nearp*tan(angle);
	float farValue  = farp*tan(angle);

	glm::vec3 nearRightPoint = glm::vec3(auxAxis.x, auxAxis.y, auxAxis.z);
	nearRightPoint *= nearValue;
	glm::vec3 nearLeftPoint = glm::vec3(-auxAxis.x, -auxAxis.y, -auxAxis.z);
	nearLeftPoint *= nearValue;
	glm::vec3 farRightPoint = glm::vec3(auxAxis.x, auxAxis.y, auxAxis.z);
	farRightPoint *= farValue;
	glm::vec3 farLeftPoint = glm::vec3(-auxAxis.x, -auxAxis.y, -auxAxis.z);
	farLeftPoint *= farValue;

	angle = aspect/2;
	angle = angle*PI180;
	nearValue = nearp*tan(angle);
	farValue = farp*tan(angle);

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
}

GLFrustum::~GLFrustum(void)
{ }