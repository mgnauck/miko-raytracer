
#ifndef _GLOBAL_ILLUMINATION_LIGHT_H__
#define _GLOBAL_ILLUMINATION_LIGHT_H__

#include "Light.h"

class GlobalIlluminationLight : public Light {
public:
	GlobalIlluminationLight();
	GlobalIlluminationLight( GlobalIlluminationLight const& rhs );
	virtual ~GlobalIlluminationLight() {}

	Vector						getLightVector( HitProperties const& hp ) const;
	Vector						calcIntensity( Raytracer* rt, HitProperties const& hp ) const;
	bool						intersect( Ray const& r, HitProperties* hp ) const;
	GlobalIlluminationLight&	operator=( GlobalIlluminationLight const& rhs );
};

#endif
