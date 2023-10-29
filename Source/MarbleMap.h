
#ifndef __MARBLE_MAP_H__
#define __MARBLE_MAP_H__

#include "ProceduralMap.h"
#include "Perlin.h"

class MarbleMap : public ProceduralMap {
public:
	MarbleMap( Vector const& m = Vector( 1.0, 1.0, 1.0 ) ) : mask( m ), perlin( 127 ) {}

	Vector get( Vector const& tex ) const {
		return mask * ( 0.5 + cos( tex.x + perlin.Perlin2D( tex.x * 28.0, tex.y * 28.0, 7, 0.25 ) ) );
	}

private:
	Vector	mask;
	Perlin	perlin;
};

#endif
