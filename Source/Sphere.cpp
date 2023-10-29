
#include "Sphere.h"
#include "Intersect.h"
#include "HitProperties.h"
#include "Material.h"

Sphere::Sphere() : Primitive(), radius( 0.0 ) {}

Sphere::Sphere( Matrix const& m, double r ) : Primitive( m ), radius( r ) {}

Sphere::Sphere( Sphere const& rhs ) : Primitive( rhs ), radius( rhs.getRadius() ) {}

Sphere::~Sphere() {}

void Sphere::setRadius( double r ) {
	radius = r;
}

double Sphere::getRadius() const {
	return radius;
}

void Sphere::transform() {
}

bool Sphere::intersect( Ray const& r, HitProperties* hp ) const {

	// ray in object space
	Ray ray( getInverse() * r.origin, ( getInverse() * r.direction ).normalized() );

	double	tnear;
	double	tfar;
	bool	inside;

	if( Intersect::RaySphere( ray, Vector( 0.0, 0.0, 0.0 ), radius, &tnear, &tfar, &inside ) ) {

		hp->distance	= tnear;
		hp->inside		= inside;
		hp->ray			= r;
		hp->objectPtr	= const_cast<Sphere*>( this );

		return true;
	}

	return false;
}

bool Sphere::intersect( Ray const& r, double dist ) const {

	// ray in object space
	Ray ray( getInverse() * r.origin, ( getInverse() * r.direction ).normalized() );
	return Intersect::RaySphere( ray, Vector( 0.0, 0.0, 0.0 ), radius, dist );
}

void Sphere::completeHitProperties( HitProperties* hp ) const {
	
	// back to object space
	Vector osPos = getInverse() * hp->ray.origin;
	Vector osDir = ( getInverse() * hp->ray.direction ).normalized();

	hp->point		= osPos + hp->distance * osDir;				// calc os hitpoint

	if( !hp->inside )
		hp->normal		= hp->point;							// calc os normal = hitpoint - center
	else
		hp->normal		= hp->point.negated();					// calc os normal = center - hitpoint
	
	hp->normal.w	= 0.0;
	hp->point		= getMatrix() * hp->point;					// transform hitpoint to ws
	hp->normal		= getInverse().transposed() * hp->normal;	// normal to ws with inverse transposed
	hp->normal.w	= 0.0;
	hp->normal.normalize();

	if( material->hasNormalMap() ) {

		// swizzle normal to get semiperpendicular vector
		hp->tangent		= Vector( hp->normal.y, hp->normal.z, -hp->normal.x );
		// calc binormal
		hp->binormal	= hp->normal.cross( hp->tangent );
		hp->binormal.w	= 0.0;
		hp->binormal.normalize();
		// recalc tangent
		hp->tangent		= hp->normal.cross( hp->binormal );
		hp->tangent.w	= 0.0;
		hp->tangent.normalize();
	}

	// calc texture coords with help orientation vectors
	Vector toVec( hp->normal );
	
	// v = angle between up and hitpoint
	double beta	= acos( getMatrix().getUp().dot( toVec ) );
	// u = angle between right and hitpoint
	double alpha = acos( getMatrix().getRight().dot( toVec ) / sin( beta ) );
	
	// clamp to range
	double u = alpha / TWOPI;
	double v = beta / PI;

	// backside of sphere
	if( getMatrix().getForward().dot( toVec ) > 0.0 )
		u = 1.0 - u;

	hp->texel	= getTiling();
	hp->texel.x	*= u;
	hp->texel.y	*= v;
}

Sphere& Sphere::operator=( Sphere const& rhs ) {
	if( &rhs == this )
		return *this;
	Primitive::operator=( rhs );
	radius = rhs.getRadius();
	return *this;
}
