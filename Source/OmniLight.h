
#ifndef _OMNI_LIGHT_H__
#define _OMNI_LIGHT_H__

#include "Light.h"

class OmniLight : public Light {
public:
	OmniLight();
	OmniLight( Matrix const& m, Vector const& c );
	OmniLight( OmniLight const& rhs );
	virtual ~OmniLight() {}

	Vector		getLightVector( HitProperties const& hp ) const;
	Vector		calcIntensity( Raytracer* rt, HitProperties const& hp ) const;
	bool		intersect( Ray const& r, HitProperties* hp ) const;
	OmniLight&	operator=( OmniLight const& rhs );
};

#endif
