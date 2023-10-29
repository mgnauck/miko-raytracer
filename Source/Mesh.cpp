
#include "Mesh.h"
#include "Intersect.h"
#include "HitProperties.h"
#include "BoundingVolume.h"
#include "Material.h"

Mesh::Mesh() : Primitive() {
	vertices.reserve( 20000 );
	triangles.reserve( 20000 );
	boundingVolume = 0;
}

Mesh::Mesh( Matrix const& m ) : Primitive( m ) {
	vertices.reserve( 20000 );
	triangles.reserve( 20000 );
	boundingVolume = 0;
}

Mesh::~Mesh() {

	vertices.clear();
	triangles.clear();
	delete boundingVolume;
}

void Mesh::add( Vertex const& v ) {
	vertices.push_back( v );
}

void Mesh::add( Triangle const& t ) {
	triangles.push_back( t );
}

void Mesh::calcNormals() {

	for( std::vector<Triangle>::iterator it=triangles.begin(); it!=triangles.end(); ++it )
		it->calcNormal( vertices[ it->a ].position, vertices[ it->b ].position, vertices[ it->c ].position );

	for( std::vector<Triangle>::const_iterator it = triangles.begin(); it != triangles.end(); ++it )
		for( int j=0; j<3; j++ )
			vertices[ ( *it )[ j ] ].normal += it->normal;
	
	for( std::vector<Vertex>::iterator it = vertices.begin(); it != vertices.end(); ++it ) {
		it->normal.w = 0.0;	// safety :D
		it->normal.normalize();
	}
}

void Mesh::setBoundingVolume( BoundingVolume* bv ) {

	boundingVolume = bv;
	boundingVolume->create( vertices );
}

void Mesh::transform() {

	Matrix const&	mat		= getMatrix();
	Matrix			mat2	= mat.inverted().transposed();

	// pre-transform vertices and normals
	for( std::vector<Vertex>::iterator it=vertices.begin(); it!=vertices.end(); ++it ) {
		
		it->position	= mat * it->position;

		it->normal.w	= 0.0;
		it->normal		= mat2 * it->normal;
		it->normal.normalize();
	}

	// recreate bounding volume
	boundingVolume->create( vertices );
}

bool Mesh::intersect( Ray const& r, HitProperties* hp ) const {

	assert( boundingVolume );

	// test with bounding volume
	if( boundingVolume )
		if( !boundingVolume->intersect( r, hp ) )	// ignore hp
			return false;

	bool firstHit = false;

	// test with triangles
	for( std::vector<Triangle>::const_iterator it=triangles.begin(); it!=triangles.end(); ++it ) {

		HitProperties act;

		if( intersect( r, &act, *it ) && ( !firstHit || ( act.distance < hp->distance ) ) ) {
			*hp			= act;
			firstHit	= true;
		}
	}

	return firstHit;
}

bool Mesh::intersect( Ray const& r, HitProperties* hp, Triangle const& tri ) const {

	Vertex const& v0 = vertices[ tri.a ];
	Vertex const& v1 = vertices[ tri.b ];
	Vertex const& v2 = vertices[ tri.c ];

	double t, u, v;

	if( Intersect::RayTriangle( r, v0.position, v1.position, v2.position, &t, &u, &v ) ) {

		double w = 1.0 - u - v;

		hp->distance	= t;
		hp->ray			= r;
		hp->normal		= w * v0.normal + u * v1.normal + v * v2.normal;
		hp->normal.normalize();	// mööh
		hp->texel		= w * v0.texel + u * v1.texel + v * v2.texel;
		hp->objectPtr	= const_cast<Mesh*>( this );

		return true;
	}

	return false;
}

bool Mesh::intersect( Ray const& r, double dist ) const {

	// bv first, triangles..
	if( boundingVolume )
		if( !boundingVolume->intersect( r, dist ) )
			return false;

	for( std::vector<Triangle>::const_iterator it=triangles.begin(); it!=triangles.end(); ++it )
		if( intersect( r, dist, *it ) )
			return true;

	return false;
}

bool Mesh::intersect( Ray const& r, double dist, Triangle const& tri ) const {
	Vector const& v0 = vertices[ tri.a ].position;
	Vector const& v1 = vertices[ tri.b ].position;
	Vector const& v2 = vertices[ tri.c ].position;
	return Intersect::RayTriangle( r, v0, v1, v2, dist );
}

void Mesh::completeHitProperties( HitProperties* hp ) const {

	hp->point	= hp->ray.origin + hp->distance * hp->ray.direction;
	hp->inside	= false;

	if( material->hasNormalMap() ) {

		// swizzle normal to get semiperpendicular vector
		hp->tangent		= Vector( hp->normal.y, hp->normal.z, -hp->normal.x );
		// calc binormal
		hp->binormal	= hp->normal.cross( hp->tangent );
		hp->binormal.normalize();
		// recalc tangent
		hp->tangent		= hp->normal.cross( hp->binormal );
		hp->tangent.normalize();
	}
}

