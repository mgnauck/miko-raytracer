
#include "GlobalIlluminationLight.h"

GlobalIlluminationLight::GlobalIlluminationLight() : Light() {}

GlobalIlluminationLight::GlobalIlluminationLight( GlobalIlluminationLight const& rhs ) : Light( rhs ) {}

Vector GlobalIlluminationLight::getLightVector( HitProperties const& hp ) const {
	return hp.normal;
}

Vector GlobalIlluminationLight::calcIntensity( Raytracer* rt, HitProperties const& hp ) const {

	int		count = rt->getActRecursion().samples;
	Vector	global( 0.0, 0.0, 0.0 );

	if( !count || !rt->recursionValid() )
		return global;

	for( int i=0; i<count; i++ ) {

		Ray ray( Ray().makeRandHemisphereRay( hp ) );
		rt->increaseRecursion();
		
		// sample hemisphere. result weighted by angle
		global += hp.normal.dot( ray.direction ) * rt->traceRay( ray );

		rt->decreaseRecursion();
	}

	return global * ( 1.0 / static_cast<double>( count ) );
}

bool GlobalIlluminationLight::intersect( Ray const& r, HitProperties* hp ) const {
	return false;
}

GlobalIlluminationLight& GlobalIlluminationLight::operator=( GlobalIlluminationLight const& rhs ) {
	if( &rhs == this )
		return *this;
	Light::operator=( rhs );
	return *this;
}