
#include "AABB.h"
#include "Intersect.h"
#include "HitProperties.h"

AABB::AABB() : BoundingVolume(), min( 0.0, 0.0, 0.0 ), max( 0.0, 0.0, 0.0 ) {}

AABB::~AABB() {}

void AABB::create( std::vector<Vertex> const& vertices ) {

	// y0!
	if( vertices.empty() )
		return;

	min = max = vertices.front().position;

	// loop starts at 1
	if( vertices.size() == 1 )
		return;

	// find smallest and largest extend per axis
	for( std::vector<Vertex>::const_iterator it=vertices.begin() + 1; it!=vertices.end(); ++it ) {

		for( int i=0; i<3; i++) {
			if( it->position[ i ] < min[ i ] )
				min[ i ] = it->position[ i ];
			else if( it->position[ i ] > max[ i ] )
				max[ i ] = it->position[ i ];
		}
	}
}

bool AABB::intersect( Ray const& r, HitProperties* hp ) {

	// FIXME:
	// substitute with simple aabb test!
	// raybox test is with arbitrary oriented box..

	double	tnear, tfar;
	Vector	nrm;
	bool	inside;

	if( Intersect::RayBox( r, ( max - min ) * 0.5, min, max, &tnear, &tfar, &inside, &nrm ) ) {
		hp->distance	= tnear;
		hp->inside		= inside;
		hp->normal		= nrm;
		hp->ray			= r;
		return true;
	}

	return false;
}

bool AABB::intersect( Ray const& r, double dist ) {
	return Intersect::RayBox( r, ( max - min ) * 0.5, min, max, dist );
}