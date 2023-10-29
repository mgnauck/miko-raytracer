
#ifndef __RAY_H__
#define __RAY_H__

#include "Vector.h"

class HitProperties;
class Camera;
class Primitive;

class Ray {
public:
	explicit Ray() : media( 1.0 ) {}

	explicit Ray( Vector const& o, Vector const& d ) : origin( o ), direction( d ), media( 1.0 ) { direction.w = 0.0; }

	explicit Ray( Vector const& o, Vector const& d, double m ) : origin( o ), direction( d ), media( m ) { direction.w = 0.0; }

	explicit Ray( Vector const& pos, Vector const& target, double* dist ) {

		// construct ray from target
		origin		= pos;
		direction	= target - pos;
		direction.w = 0.0;
		*dist		= direction.magnitude();
		direction.normalize();
		origin		+= direction * EPSILON;
		media		= 1.0;
	}

	Ray( Ray const& rhs ) {
		origin		= rhs.origin;
		direction	= rhs.direction;
		media		= rhs.media;
	}

	Ray const& makePrimaryRay( double x, double y, Camera const& cam );
	Ray const& makeReflectionRay( HitProperties const& hp );
	Ray const& makeRefractionRay( HitProperties const& hp );
	Ray const& makeRandHemisphereRay( HitProperties const& hp );

	Ray& operator=( Ray const& rhs ) {
		if( &rhs == this )
			return *this;
		origin		= rhs.origin;
		direction	= rhs.direction;
		media		= rhs.media;
		return *this;
	}

	Vector		origin;
	Vector		direction;
	double		media;
};

#endif
