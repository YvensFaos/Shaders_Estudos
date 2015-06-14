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

//GLAABB

bool GLAABB::intercepts(glm::vec3 max1, glm::vec3 min1, glm::vec3 max2, glm::vec3 min2)
{
	return max1.x >= min2.x 
		&& max1.y >= min2.y
		&& max1.z >= min2.z
		&& min1.x <= max2.x
		&& min1.y <= max2.y
		&& min1.z <= max2.z;
}

//GLFrustum

GLFrustum::GLFrustum(float nearp, float farp, GLCameraStep* cameraStep)
{
	float angle = cameraStep->fov/2;
	angle = angle*PI180;

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

	angle = cameraStep->fov/2;
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

GLFrustum::GLFrustum(GLCamera* camera)
{
	float angle = camera->fov/2;
	angle = angle*PI180;

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

	float nearValue = camera->near*tan(angle);
	float farValue  = camera->far*tan(angle);

	glm::vec3 nearRightPoint = glm::vec3(auxAxis.x, auxAxis.y, auxAxis.z);
	nearRightPoint *= nearValue;
	glm::vec3 nearLeftPoint = glm::vec3(-auxAxis.x, -auxAxis.y, -auxAxis.z);
	nearLeftPoint *= nearValue;
	glm::vec3 farRightPoint = glm::vec3(auxAxis.x, auxAxis.y, auxAxis.z);
	farRightPoint *= farValue;
	glm::vec3 farLeftPoint = glm::vec3(-auxAxis.x, -auxAxis.y, -auxAxis.z);
	farLeftPoint *= farValue;

	angle = camera->fov/2;
	angle = angle*PI180;
	nearValue = camera->near*tan(angle);
	farValue = camera->far*tan(angle);

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

bool GLFrustum::intercepts(glm::vec3* min, glm::vec3* max)
{
	bool found = containsAnyVertexOf(min, max);
	if (!found)
	{
		glm::vec3 center = glm::vec3(min->x + (max->x - min->x)/2.0f, min->y + (max->y - min->y)/2.0f, min->z + (max->z - min->z)/2.0f);
		float x = abs(max->x - min->x);
		float y = abs(max->y - min->y);
		float z = abs(max->z - min->z);

		float radius = x + y + (abs(x - y))/2.0f;
		radius = radius + z + (abs(radius - z))/2.0f;
		found = containsSphere(&center, radius);
	}

	if(!found)
	{
		glm::vec3 nbl = corners[FRUSTUM_NBL];
		glm::vec3 ftr = corners[FRUSTUM_FTR];

		float x = ftr.x - nbl.x;
		float y = ftr.y - nbl.y;
		float z = ftr.z - nbl.z;

		glm::vec3* lmin = new glm::vec3(MAX_FLOAT);
		glm::vec3* lmax = new glm::vec3(MIN_FLOAT);
	
		int j = 0;
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

		found = (max->x >= lmin->x) && (min->x <= lmax->x) 
			 && (max->y >= lmin->y) && (min->y <= lmax->y)
			 && (max->z >= lmin->z) && (min->z <= lmax->z);

		delete lmin;
		delete lmax;
	}
	return found;
}