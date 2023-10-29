
#include "SpotLight.h"

SpotLight::SpotLight() : OmniLight(), outerAngle( 30.0 ), innerAngle( 10.0 ) {}

SpotLight::SpotLight( Matrix const& m, Vector const& c, double oa, double ia ) : OmniLight( m, c ), outerAngle( oa ), innerAngle( ia ) {}

SpotLight::SpotLight( SpotLight const& rhs ) : OmniLight( rhs ), outerAngle( rhs.getOuterAngle() ), innerAngle( rhs.getInnerAngle() ) {}

double SpotLight::getOuterAngle() const {
	return outerAngle;
}

double SpotLight::getInnerAngle() const {
	return innerAngle;
}

Vector SpotLight::getLightVector( HitProperties const& hp ) const {
	return ( getMatrix().getTranslation() - hp.point ).normalized();
}

Vector SpotLight::calcIntensity( Raytracer* rt, HitProperties const& hp ) const {

	// calc light vec
	Vector lightVec( getLightVector( hp ) );

	// intensity is normal dot light vector
	double intensity = hp.normal.dot( lightVec );

	if( intensity < 0.0 )
		return Vector( 0.0, 0.0, 0.0 );

	// get light direction
	Vector lightDir( getMatrix().getForward() );

	// calc angle between lightVec and lightDir
	// multiplied by 2 because we only calculate half angle but need the total angle
	double angle = abs( acos( lightVec.dot( lightDir.negated() ) ) * RAD2DEG * 2.0 );

	// inside spot area?
	if( angle < outerAngle ) {
		
		// inside fall off area?
		if( angle > innerAngle ) {
			// calc linear fall off
			angle = ( angle - innerAngle ) / ( outerAngle - innerAngle );
			intensity *= ( 1.0 - angle );
		}

	} else
		return Vector( 0.0, 0.0, 0.0 );

	// construct ray
	double	dist;
	Ray		r( hp.point, getMatrix().getTranslation(), &dist );

	if( getShadowing() && rt->getScene()->intersect( r, dist ) )
		return Vector( 0.0, 0.0, 0.0 );

	// depends on light dir
	return color * intensity;
}

bool SpotLight::intersect( Ray const& r, HitProperties* hp ) const {
	// unlikely
	return false;
}

SpotLight& SpotLight::operator=( SpotLight const& rhs ) {
	if( &rhs == this )
		return *this;
	OmniLight::operator=( rhs );
	outerAngle = rhs.getOuterAngle();
	innerAngle = rhs.getInnerAngle();
	return *this;
}