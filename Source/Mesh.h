
#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include "Vertex.h"
#include "Triangle.h"
#include "Primitive.h"

class BoundingVolume;

class Mesh : public Primitive {
	public:
		Mesh();
		Mesh( Matrix const& m );
		~Mesh();

		void					add( Vertex const& v );
		void					add( Triangle const& t );

		void					calcNormals();
		void					setBoundingVolume( BoundingVolume* bv );

		void					transform();
		bool					intersect( Ray const& r, HitProperties* hp ) const;
		bool					intersect( Ray const& r, double dist ) const;
		void					completeHitProperties( HitProperties* hp ) const;

	private:		
		Mesh( Mesh const& );								// no dups here
		Mesh&					operator=( Mesh const& );

		bool					intersect( Ray const& r, HitProperties* hp, Triangle const& tri ) const;
		bool					intersect( Ray const& r, double dist, Triangle const& tri ) const;

		std::vector<Vertex>		vertices;
		std::vector<Triangle>	triangles;

		BoundingVolume*			boundingVolume;
};

#endif
