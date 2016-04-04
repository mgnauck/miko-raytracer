
#include "Box.h"
#include "Intersect.h"
#include "HitProperties.h"
#include "Material.h"

Box::Box() : Primitive(), min( 0.0, 0.0, 0.0 ), max( 0.0, 0.0, 0.0 ) {}

Box::Box( Matrix const& m, Vector const& minExt, Vector const& maxExt ) : Primitive( m ), min( minExt ), max( maxExt ) {}

Box::Box( Box const& rhs ) : Primitive( rhs ), min( rhs.getMin() ), max( rhs.getMax() ) {}

void Box::setMin( Vector const& minExt ) {
	min = minExt;
}

void Box::setMax( Vector const& maxExt ) {
	max = maxExt;
}

Vector const& Box::getMin() const {
	return min;
}

Vector const& Box::getMax() const {
	return max;
}

void Box::transform() {
}

bool Box::intersect( Ray const& r, HitProperties* hp ) const {

	// ray in object space
	Ray ray( getInverse() * r.origin, ( getInverse() * r.direction ).normalized() );

	double	tnear, tfar;
	bool	inside;
	Vector	nrm;

	if( Intersect::RayBox( ray, Vector( 0.0, 0.0, 0.0 ), min, max, &tnear, &tfar, &inside, &nrm ) ) {

		hp->distance	= tnear;
		hp->normal		= nrm;		// os normal
		hp->ray			= r;
		hp->inside		= inside;
		hp->objectPtr	= const_cast<Box*>( this );
		return true;
	}

	return false;
}

bool Box::intersect( Ray const& r, double dist ) const {
	// ray in object space
	Ray ray( getInverse() * r.origin, ( getInverse() * r.direction ).normalized() );
	return Intersect::RayBox( ray, Vector( 0.0, 0.0, 0.0 ), min, max, dist ) ;
}

void Box::completeHitProperties( HitProperties* hp ) const {

	// should work..
	// hp->point = hp->ray.origin + hp->distance * hp->ray.direction;

	Vector osPos = getInverse() * hp->ray.origin;
	Vector osDir = ( getInverse() * hp->ray.direction ).normalized();

	// calc os hitpoint
	hp->point = osPos + hp->distance * osDir;

	// calc extend lengths
	Vector len( max - min );
	Vector pos( hp->point );

	// find os binormal & tangent
	// find tex coords depending on os normal + os hitpoint
	// bias/scale into [0,1]
	if( hp->normal.x == -1.0 ) {

		hp->texel.x	= ( -min.z + pos.z ) / len.z;
		hp->texel.y	= 1.0 - ( -min.y + pos.y ) / len.y;

		if( material->hasNormalMap() ) {
			hp->tangent	= Vector( 0.0, 0.0, -1.0 );
			hp->binormal= Vector( 0.0, 1.0, 0.0 );
		}
	}

	if( hp->normal.x == 1.0 ) {
		hp->texel.x	= 1.0 - ( -min.z + pos.z ) / len.z;
		hp->texel.y	= 1.0 - ( -min.y + pos.y ) / len.y;

		if( material->hasNormalMap() ) {
			hp->tangent	= Vector( 0.0, 0.0, 1.0 );
			hp->binormal= Vector( 0.0, -1.0, 0.0 );
		}
	}
	///
	if( hp->normal.y == -1.0 ) {
		hp->texel.x	= ( -min.x + pos.x ) / len.x;
		hp->texel.y	= 1.0 - ( -min.z + pos.z ) / len.z;

		if( material->hasNormalMap() ) {
			hp->tangent	= Vector( 0.0, 0.0, 1.0 );
			hp->binormal= Vector( -1.0, 0.0, 0.0 );
		}
	}

	if( hp->normal.y == 1.0 ) {
		hp->texel.x	= ( -min.x + pos.x ) / len.x;
		hp->texel.y	= ( -min.z + pos.z ) / len.z;

		if( material->hasNormalMap() ) {
			hp->tangent	= Vector( 0.0, 0.0, 1.0 );
			hp->binormal= Vector( 1.0, 0.0, 0.0 );
		}
	}
	///
	if( hp->normal.z == -1.0 ) {
		hp->texel.x	= 1.0 - ( -min.x + pos.x ) / len.x;
		hp->texel.y	= 1.0 - ( -min.y + pos.y ) / len.y;

		if( material->hasNormalMap() ) {
			hp->tangent	= Vector( 0.0, 1.0, 1.0 );
			hp->binormal= Vector( 1.0, 0.0, 0.0 );
		}
	}

	if( hp->normal.z == 1.0 ) {
		hp->texel.x	= ( -min.x + pos.x ) / len.x;
		hp->texel.y	= 1.0 - ( -min.y + pos.y ) / len.y;

		if( material->hasNormalMap() ) {
			hp->tangent	= Vector( 0.0, -1.0, 1.0 );
			hp->binormal= Vector( 1.0, 0.0, 0.0 );
		}
	}

	// into ws
	hp->point	= getMatrix() * hp->point;

	// tile
	hp->texel.x *= getTiling().x;
	hp->texel.y *= getTiling().y;

	// transform normal to ws
	hp->normal.w	= 0.0;
	hp->normal		= getInverse().transposed() * hp->normal;
	hp->normal.w	= 0.0;
	hp->normal.normalize();
	
	// transform tangent to ws
	hp->tangent.w	= 0.0;
	hp->tangent		= getInverse().transposed() * hp->tangent;
	hp->tangent.w	= 0.0;
	hp->tangent.normalize();

	// transform binormal to ws
	hp->binormal.w	= 0.0;
	hp->binormal	= getInverse().transposed() * hp->binormal;
	hp->binormal.w	= 0.0;
	hp->binormal.normalize();
}

Box& Box::operator=( Box const& rhs ) {
	if( &rhs == this )
		return *this;
	Primitive::operator=( rhs );
	min = rhs.getMin();
	max = rhs.getMax();
	return *this;
}