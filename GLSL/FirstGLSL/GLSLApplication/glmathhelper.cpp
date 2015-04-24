#include "glmathhelper.h"

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

GLFrustum::~GLFrustum(void)
{
	if(corners)
	{
		delete corners;
	}
	if(planes)
	{
		delete planes;
	}
}