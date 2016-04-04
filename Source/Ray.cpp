
#include "Ray.h"
#include <iostream>
#include "HitProperties.h"
#include "Camera.h"
#include "Primitive.h"

Ray const& Ray::makePrimaryRay( double x, double y, Camera const& cam ) {

	Matrix const&	mat	= cam.getMatrix();

	origin		= cam.getEye();
	direction	= ( mat.getRight() * x + mat.getUp() * y + mat.getForward() * ( 1.0 / cam.getDistToViewplane() ) ).normalized();
	media		= 1.0;

	return *this;
}

Ray const& Ray::makeReflectionRay( HitProperties const& hp ) {

	origin		= hp.point;
	direction	= hp.ray.direction.negated().reflect( hp.normal ).normalized();
	media		= hp.ray.media;

	return *this;
}

Ray const& Ray::makeRefractionRay( HitProperties const& hp ) {

	double	leavingMedia	= hp.ray.media;
	double	enteringMedia	= dynamic_cast<Primitive*>( hp.objectPtr )->getMaterial()->refractiveIndex;

	if( leavingMedia == enteringMedia )
		enteringMedia = 1.0;

	Vector dir;

	if( hp.ray.direction.refract( hp.normal, leavingMedia, enteringMedia, &dir ) ) {

		dir.w = 0.0;
		dir.normalize();

		origin		= hp.point + dir * EPSILON;
		direction	= dir;
		media		= enteringMedia;

	} else {

		// total reflection
		origin		= hp.point + hp.ray.direction * EPSILON;
		direction	= hp.ray.direction;
		media		= enteringMedia;
	}

	return *this;
}

Ray const& Ray::makeRandHemisphereRay( HitProperties const& hp ) {

	origin		= hp.point,
	direction	= Vector().hemisphereRand( hp.normal );
	media		= 1.0;

	direction.w = 0.0;
	direction.normalize();
	origin		+= direction * EPSILON;

	return *this;
}