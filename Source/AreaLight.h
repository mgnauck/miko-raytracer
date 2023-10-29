
#ifndef _AREA_LIGHT_H__
#define _AREA_LIGHT_H__

#include "Light.h"

class AreaLight : public Light {
public:
	AreaLight();
	AreaLight( Matrix const& m, Vector const& c, Vector const& ext );
	AreaLight( AreaLight const& rhs );
	~AreaLight();

	Vector const&	getExtend() const;
	Vector			getLightVector( HitProperties const& hp ) const;
	Vector			calcIntensity( Raytracer* rt, HitProperties const& hp ) const;
	bool			intersect( Ray const& r, HitProperties* hp ) const;
	AreaLight&		operator=( AreaLight const& rhs );

private:
	Vector			extend;		// x and y define arealights extend
};

#endif