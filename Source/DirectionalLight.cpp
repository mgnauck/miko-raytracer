
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light() {}

DirectionalLight::DirectionalLight( Matrix const& m, Vector const& c ) : Light( m, c ) {}

DirectionalLight::DirectionalLight( DirectionalLight const& rhs ) : Light( rhs ) {
}

Vector DirectionalLight::getLightVector( HitProperties const& hp ) const {
	// forward is our light direction vector -> negate for dots
	return getMatrix().getForward().negated();
}

Vector DirectionalLight::calcIntensity( Raytracer* rt, HitProperties const& hp ) const {

	Vector	direction( getMatrix().getForward().negated() );
	double	intensity = hp.normal.normalized().dot( direction );
	
	if( intensity < 0.0 )
		return Vector( 0.0, 0.0, 0.0 );

	if( getShadowing() && rt->getScene()->intersect( Ray( hp.point, direction ), 1000000.0 ) )
		return Vector( 0.0, 0.0, 0.0 );

	return color;

	// depend on angle
	//return color * intensity;
}

bool DirectionalLight::intersect( Ray const& r, HitProperties* hp ) const {
	// y0!
	return false;
}

DirectionalLight& DirectionalLight::operator=( DirectionalLight const& rhs ) {
	if( this == &rhs )
		return *this;
	Light::operator=( rhs );
	return *this;
}
