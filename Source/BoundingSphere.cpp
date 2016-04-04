
#include "BoundingSphere.h"
#include "Intersect.h"
#include "HitProperties.h"

BoundingSphere::BoundingSphere() : BoundingVolume(), radius( 0.0 ), center( 0.0, 0.0, 0.0 ) {}

BoundingSphere::~BoundingSphere() {}

void BoundingSphere::create( std::vector<Vertex> const& vertices ) {

	if( vertices.empty() )
		return;

	// sum all vertices
	for( std::vector<Vertex>::const_iterator it=vertices.begin(); it!=vertices.end(); ++it )
		center += it->position;

	// get average center position
	center *= ( 1.0 / static_cast<double>( vertices.size() ) );

	// find greatest distance to center
	for( std::vector<Vertex>::const_iterator it=vertices.begin(); it!=vertices.end(); ++it ) {
		double tmp = ( it->position - center ).magnitudeSquared();
		radius = ( tmp > radius ) ? tmp : radius;
	}

	radius = sqrt( radius );
}

bool BoundingSphere::intersect( Ray const& r, HitProperties* hp ) const {

	double	tnear, tfar;
	bool	inside;

	if( Intersect::RaySphere( r, center, radius, &tnear, &tfar, &inside ) ) {
		hp->distance	= tnear;
		hp->inside		= inside;
		hp->ray			= r;
		return true;
	}

	return false;
}

bool BoundingSphere::intersect( Ray const& r, double dist ) const {
	return Intersect::RaySphere( r, center, radius, dist );
}
