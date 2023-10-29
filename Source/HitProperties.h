
#ifndef __HIT_PROPERTIES_H__
#define __HIT_PROPERTIES_H__

#include "Ray.h"

class Renderable;

class HitProperties {
public:
	HitProperties() : objectPtr( 0 ) {}

	HitProperties( HitProperties const& rhs ) {
		ray			= rhs.ray;
		point		= rhs.point;
		distance	= rhs.distance;
		normal		= rhs.normal;
		tangent		= rhs.tangent;
		binormal	= rhs.binormal;
		texel		= rhs.texel;
		inside		= rhs.inside;
		objectPtr	= rhs.objectPtr;
		outColor	= rhs.outColor;
	}

	HitProperties& operator=( HitProperties const& rhs ) {
		if( this == &rhs )
			return *this;
		ray			= rhs.ray;
		point		= rhs.point;
		distance	= rhs.distance;
		normal		= rhs.normal;
		tangent		= rhs.tangent;
		binormal	= rhs.binormal;
		texel		= rhs.texel;
		inside		= rhs.inside;
		objectPtr	= rhs.objectPtr;
		outColor	= rhs.outColor;
		return *this;
	}

	Ray				ray;
	Vector			point;
	double			distance;
	Vector			normal;
	Vector			tangent;
	Vector			binormal;
	Vector			texel;
	bool			inside;
	Renderable*		objectPtr;
	Vector			outColor;
};

#endif
