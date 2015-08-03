#pragma once

#include <math.h>

#include "glm\glm.hpp"

//Originalmente de http://tog.acm.org/resources/GraphicsGems/gemsiii/triangleCube.c

/* this version of SIGN3 shows some numerical instability, and is improved
 * by using the uncommented macro that follows, and a different test with it */
#define EPS 10e-5
#define SIGN3( A ) \
    (((A).x < EPS) ? 4 : 0 | ((A).x > -EPS) ? 32 : 0 | \
    ((A).y < EPS) ? 2 : 0 | ((A).y > -EPS) ? 16 : 0 | \
    ((A).z < EPS) ? 1 : 0 | ((A).z > -EPS) ? 8 : 0)

#define CROSS( A, B, C ) { \
  (C).x =  (A).y * (B).z - (A).z * (B).y; \
  (C).y = -(A).x * (B).z + (A).z * (B).x; \
  (C).z =  (A).x * (B).y - (A).y * (B).x; \
	        }
#define SUB( A, B, C ) { \
  (C).x =  (A).x - (B).x; \
  (C).y =  (A).y - (B).y; \
  (C).z =  (A).z - (B).z; \
	        }
#define LERP( A, B, C) ((B)+(A)*((C)-(B)))
#define MIN3(a,b,c) ((((a)<(b))&&((a)<(c))) ? (a) : (((b)<(c)) ? (b) : (c)))
#define MAX3(a,b,c) ((((a)>(b))&&((a)>(c))) ? (a) : (((b)>(c)) ? (b) : (c)))
#define INSIDE 0
#define OUTSIDE 1

typedef glm::vec3 Point3;

typedef struct{
	Point3 v1;                 /* Vertex1 */
	Point3 v2;                 /* Vertex2 */
	Point3 v3;                 /* Vertex3 */
} Triangle3;

class TriangleCube
{
public:
	static long face_plane(Point3 p);
	static long bevel_2d(Point3 p);
	static long bevel_3d(Point3 p);
	static long check_point(Point3 p1, Point3 p2, float alpha, long mask);
	static long check_line(Point3 p1, Point3 p2, long outcode_diff);
	static long point_triangle_intersection(Point3 p, Triangle3 t);
	static long t_c_intersection(Triangle3 t);
	static bool testIntersection(glm::vec3* p1, glm::vec3* p2, glm::vec3* p3, glm::vec3* min, glm::vec3* max);
};