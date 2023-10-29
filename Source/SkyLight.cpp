
#include "SkyLight.h"

SkyLight::SkyLight() : Light() {}

SkyLight::SkyLight( Vector const& c ) : Light( Matrix().makeIdentity(), c ) {}

SkyLight::SkyLight( SkyLight const& rhs ) : Light( rhs ) {}

Vector SkyLight::getLightVector( HitProperties const& hp ) const {
	// skylight is everywhere, return normal itself
	return hp.normal;
}

Vector SkyLight::calcIntensity( Raytracer* rt, HitProperties const& hp ) const {

	// count is squared to be coherent with area lights subdiv
	int		count		= rt->getActRecursion().lightSamples * rt->getActRecursion().lightSamples;
	double	intensity	= 0.0;

	for( int i=0; i<count; i++ ) {
		Ray ray( Ray().makeRandHemisphereRay( hp ) );
		if( !rt->getScene()->intersect( ray, 1000000.0 ) )
			// intensity depends on angle of ray
			intensity += ray.direction.dot( hp.normal );
	}

	intensity /= static_cast<double>( count );

	return color * intensity;
}

bool SkyLight::intersect( Ray const& r, HitProperties* hp ) const {
	hp->ray			= r;
	hp->distance	= 1000000.0;	// this is hopefully far far away
	hp->objectPtr	= const_cast<SkyLight*>( this );
	return true;
}

SkyLight& SkyLight::operator=( SkyLight const& rhs ) {
	if( &rhs == this )
		return *this;
	Light::operator=( rhs );
	return *this;
}