
#ifndef __DIRECTIONAL_LIGHT_H__
#define __DIRECTIONAL_LIGHT_H__

#include "Light.h"

class DirectionalLight : public Light {
public:
	DirectionalLight();
	DirectionalLight( Matrix const& m, Vector const& c );
	DirectionalLight( DirectionalLight const& rhs );
	virtual ~DirectionalLight() {}

	Vector				getLightVector( HitProperties const& hp ) const;
	Vector				calcIntensity( Raytracer* rt, HitProperties const& hp ) const;
	bool				intersect( Ray const& r, HitProperties* hp ) const;
	DirectionalLight&	operator=( DirectionalLight const& rhs );
};

#endif
