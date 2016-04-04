
#include "Plane.h"
#include "Intersect.h"
#include "HitProperties.h"
#include "Material.h"

Plane::Plane() : Primitive(), normal( 0.0, 0.0, 0.0 ), d( 0.0 ), length( 1000.0 ), width( 1000.0 ) {}

Plane::Plane( Matrix const& m, double l, double w ) : Primitive( m ), normal( getMatrix().getUp() ), d( getMatrix().getTranslation().magnitude() ), length( l ), width( w ) {}

Plane::Plane( Plane const& rhs ) : Primitive( rhs ), normal( rhs.getNormal() ), d( rhs.getD() ), length( rhs.getLength() ), width( rhs.getWidth() ) {}

Plane::~Plane() {}

Vector const& Plane::getNormal() const {
	return normal;
}

double Plane::getD() const {
	return d;
}

double Plane::getLength() const {
	return length;
}

double Plane::getWidth() const {
	return width;
}

void Plane::transform() {
	d		= getMatrix().getTranslation().magnitude();	// distance to origin
	normal	= getMatrix().getUp();						// up is normal
}

bool Plane::intersect( Ray const& r, HitProperties* hp ) const {

	// ray in object space
	Ray ray( getInverse() * r.origin, ( getInverse() * r.direction ).normalized() );

	double t;

	if( Intersect::RayPlane( ray, 0.0, Vector( 0.0, 1.0, 0.0 ), &t ) ) {

		Vector tmp( ray.origin + t * ray.direction );

		double l = 0.5 * length;
		double w = 0.5 * width;

		if( ( tmp.x < w ) && ( tmp.x > -w ) && ( tmp.z < l ) && ( tmp.z > -l ) ) {

			hp->distance	= t;
			hp->ray			= r;
			hp->objectPtr	= const_cast<Plane*>( this );
			return true;
		}
	}

	return false;
}

bool Plane::intersect( Ray const& r, double dist ) const {

	// ray in object space
	Ray		ray( getInverse() * r.origin, ( getInverse() * r.direction ).normalized() );
	double	t;

	if( Intersect::RayPlane( r, 0.0, Vector( 0.0, 1.0, 0.0 ), dist, &t ) ) {

		Vector tmp( ray.origin + t * ray.direction );

		double l = 0.5 * length;
		double w = 0.5 * width;

		if( ( tmp.x < w ) && ( tmp.x > -w ) && ( tmp.z < l ) && ( tmp.z > -l ) )
			return true;
	}

	return false;
}

void Plane::completeHitProperties( HitProperties* hp ) const {


	Vector osPos = getInverse() * hp->ray.origin;
	Vector osDir = ( getInverse() * hp->ray.direction ).normalized();

	hp->point	= osPos + hp->distance * osDir;				// calc os hitpoint
	hp->normal	= getMatrix().getUp();
	hp->inside	= false;

	if( material->hasNormalMap() ) {
		hp->tangent	= getMatrix().getRight();
		hp->binormal= getMatrix().getForward();
	}

	// calc texturecoord
	hp->texel	= getTiling();

	//Vector toVec( hp->point - getMatrix().getTranslation() );
	Vector toVec( hp->point );

	// project toVec on right and forward
	// offset center to hide abs()
	//hp->texel.x	*= abs( 1000.0 + getMatrix().getRight().dot( toVec ) );
	//hp->texel.y	*= abs( 1000.0 + getMatrix().getForward().dot( toVec ) );
	hp->texel.x	*= abs( 1000.0 + Vector( 1.0, 0.0, 0.0 ).dot( toVec ) * ( 1.0 / width ) );
	hp->texel.y	*= abs( 1000.0 + Vector( 0.0, 0.0, 1.0 ).dot( toVec ) * ( 1.0 / length ) );

	hp->point	= getMatrix() * hp->point;					// transform hitpoint to ws
}

Plane& Plane::operator=( Plane const& rhs ) {
	if( &rhs == this )
		return *this;
	Primitive::operator=( rhs );
	normal = rhs.getNormal();
	d = rhs.getD();
	length = rhs.getLength();
	width = rhs.getWidth();
	return *this;
}