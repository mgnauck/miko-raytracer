
#ifndef _SPOT_LIGHT_H__
#define _SPOT_LIGHT_H__

#include "OmniLight.h"

// outerAngle is maximum fall off area value
// innerAngle is maximum hot spot area value

class SpotLight : public OmniLight {		// is omni with direction + angle
public:
	SpotLight();
	SpotLight( Matrix const& m, Vector const& c, double oa, double ia );
	SpotLight( SpotLight const& rhs );
	virtual ~SpotLight() {}

	double		getOuterAngle() const;
	double		getInnerAngle() const;
	Vector		getLightVector( HitProperties const& hp ) const;
	Vector		calcIntensity( Raytracer* rt, HitProperties const& hp ) const;
	bool		intersect( Ray const& r, HitProperties* hp ) const;
	SpotLight&	operator=( SpotLight const& rhs );

private:
	double		outerAngle;
	double		innerAngle;
};

#endif