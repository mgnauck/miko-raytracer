
#include "AreaLight.h"
#include "HitProperties.h"
#include "Intersect.h"

AreaLight::AreaLight() : Light(), extend( 0.0, 0.0, 0.0 ) {}

AreaLight::AreaLight( Matrix const& m, Vector const& c, Vector const& ext ) : Light( m, c ), extend( ext ) {}

AreaLight::AreaLight( AreaLight const& rhs ) : Light( rhs ), extend( rhs.getExtend() ) {}

AreaLight::~AreaLight() {
}

Vector const& AreaLight::getExtend() const {
	return extend;
}

Vector AreaLight::getLightVector( HitProperties const& hp ) const {
	return ( getMatrix().getTranslation() - hp.point ).normalized();
}

Vector AreaLight::calcIntensity( Raytracer* rt, HitProperties const& hp ) const {

	// get position + orientation (up=normal of area light)
	Vector pos		= getMatrix().getTranslation();
	Vector up		= getMatrix().getUp();
	Vector right	= getMatrix().getRight();
	Vector forward	= getMatrix().getForward();

	// count = light samples
	int	count = rt->getActRecursion().lightSamples;

	// prepare count sample positions on arealight for shadow rays
	std::vector<Vector>	samplePositions;
	samplePositions.reserve( count * count );

	// prepare a regular grid
	double dx	= ( extend.x * 2.0 ) / static_cast<double>( count );
	double dz	= ( extend.y * 2.0 ) / static_cast<double>( count );
	double dx2	= dx * 0.5;
	double dz2	= dz * 0.5;
	
	double zs = -extend.y + dz2;
	for( int j=0; j<count; j++ ) { 

		double xs = -extend.x + dx2;
		for( int i=0; i<count; i++ ) {

			// jitter regular grid
			double x = xs + rnd( -dx2, dx2 );
			double z = zs + rnd( -dz2, dz2 );
			
			// 'locate' sampple pos on arealights surface
			samplePositions.push_back( pos + ( right * x ) + ( forward * z ) );
			xs += dx;
		}
		zs += dz;
	}

	// calc arealights intensity at particular point..
	double intensity = 0.0;

	// shoot 'count' shadow rays
	for( int i=0; i<count; i++ ) {

		double	dist;
		Ray		r( hp.point, samplePositions[ i ], &dist );

		// check that shadow ray is 'in direction' of surfacepoints normal
		if( ( hp.normal.dot( r.direction ) > 0.0 ) ) {	
			// check if shadow ray faces arealight
			double val = r.direction.dot( up.negated() );
			if( ( val > 0.0 ) && !rt->getScene()->intersect( r, dist ) )
				// intensity depend on angle between arealight and surface thus add val
				intensity += val;	//+= 1.0;
		}
	}

	// average intensity
	intensity /= static_cast<double>( count );

	// clean
	samplePositions.clear();

	// light color
	return color * intensity;
}

bool AreaLight::intersect( Ray const& r, HitProperties* hp ) const {

	// ray in object space
	Ray ray( getInverse() * r.origin, ( getInverse() * r.direction ).normalized() );

	double t;

	if( Intersect::RayPlane( ray, 0.0, Vector( 0.0, 1.0, 0.0 ), &t ) ) {

// FIXME: einfach im object space machen du knall0r!

		// world space hit point
		Vector to( ( r.origin + t * r.direction ) - getMatrix().getTranslation() );
		// check in worldspace
		// project vector to hitpoint on forward axis and right axis
		double v1 = abs( getMatrix().getRight().dot( to ) );
		double v2 = abs( getMatrix().getForward().dot( to ) );
		
		if( ( v1 < extend.x ) && ( v2 < extend.y ) ) {
			hp->ray			= r;
			hp->distance	= t;
			hp->objectPtr	= const_cast<AreaLight*>( this );
			return true;
		}
	}

	return false;
}

AreaLight& AreaLight::operator=( AreaLight const& rhs ) {
	if( &rhs == this )
		return *this;
	Light::operator=( rhs );
	extend = rhs.getExtend();
	return *this;
}