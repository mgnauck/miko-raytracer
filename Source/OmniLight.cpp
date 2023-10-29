
#include "OmniLight.h"

OmniLight::OmniLight() : Light() {}

OmniLight::OmniLight( Matrix const& m, Vector const& c ) : Light( m, c ) {}

OmniLight::OmniLight( OmniLight const& rhs ) : Light( rhs ) {}

Vector OmniLight::getLightVector( HitProperties const& hp ) const {
	return ( getMatrix().getTranslation() - hp.point ).normalized();
}

Vector OmniLight::calcIntensity( Raytracer* rt, HitProperties const& hp ) const {

	// construct shadow feeler
	double	dist;
	Ray		r( hp.point, getMatrix().getTranslation(), &dist );
	double	intensity = hp.normal.dot( r.direction );

	if( intensity < 0.0 )
		return Vector( 0.0, 0.0, 0.0 );

	if( getShadowing() && rt->getScene()->intersect( r, dist ) )
		return Vector( 0.0, 0.0, 0.0 );

	return color;
}

bool OmniLight::intersect( Ray const& r, HitProperties* hp ) const {
	// unlikely
	return false;
}

OmniLight& OmniLight::operator=( OmniLight const& rhs ) {
	if( &rhs == this )
		return *this;
	Light::operator=( rhs );
	return *this;
}