
#ifndef __VERTEX_H__
#define __VERTEX_H__

#include "Vector.h"

struct Vertex {

	Vertex() {}

	Vertex( Vector const& p ) : position( p ), normal( Vector( 0.0, 0.0, 0.0 ) ), texel( Vector( 0.0, 0.0, 0.0 ) ) {}

	Vertex( Vector const& p, Vector const& t ) : position( p ), normal( Vector( 0.0, 0.0, 0.0 ) ), texel( t ) {}

	Vertex( Vector const& p, Vector const& n, Vector const& t ) : position( p ), normal( n ), texel( t ) {}

	Vertex( Vertex const& rhs ) {
		position	= rhs.position;
		normal		= rhs.normal;
		texel		= rhs.texel;
	}

	Vertex& operator=( Vertex const& rhs ) {
		if( &rhs == this )
			return *this;
		position	= rhs.position;
		normal		= rhs.normal;
		texel		= rhs.texel;
		return *this;
	}

	Vector		position;
	Vector		normal;
	Vector		texel;
};

#endif
