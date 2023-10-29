
#ifndef _SKY_LIGHT_H__
#define _SKY_LIGHT_H__

#include "Light.h"

class SkyLight : public Light {
public:
	SkyLight();
	SkyLight( Vector const& c );
	SkyLight( SkyLight const& rhs );
	virtual ~SkyLight() {}

	Vector		getLightVector( HitProperties const& hp ) const;
	Vector		calcIntensity( Raytracer* rt, HitProperties const& hp ) const;
	bool		intersect( Ray const& r, HitProperties* hp ) const;
	SkyLight&	operator=( SkyLight const& rhs );
};

#endif
