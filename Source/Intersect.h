
#ifndef __INTERSECT_H__
#define __INTERSECT_H__

class Ray;
struct Vector;

class Intersect {
public:
	static bool RayPlane( Ray const& r, double d, Vector const& nrm, double* t );
	static bool RayPlane( Ray const& r, double d, Vector const& nrm, double dist, double* t );

	static bool RaySphere( Ray const& r, Vector const& center, double radius, double* tnear, double* tfar, bool* in );
	static bool RaySphere( Ray const& r, Vector const& center, double radius, double dist );

	static bool RayCylinder( Ray const& r, Vector const& center, double radius, double* tnear, double* tfar, bool* in );
	static bool RayCylinder( Ray const& r, Vector const& center, double radius, double dist );

	static bool RayBox( Ray const& r, Vector const& center, Vector const& min, Vector const& max, double* tnear, double* tfar, bool* inside, Vector* nrm );
	static bool RayBox( Ray const& r, Vector const& center, Vector const& min, Vector const& max, double dist );

	static bool RayTriangle( Ray const& r, Vector const& v0, Vector const& v1, Vector const& v2, double* t, double* u, double* v );
	static bool RayTriangle( Ray const& r, Vector const& v0, Vector const& v1, Vector const& v2, double dist );
};

#endif
