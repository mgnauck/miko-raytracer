
#ifndef __NOISE_MAP_H__
#define __NOISE_MAP_H__

#include "ProceduralMap.h"
#include "Perlin.h"

class NoiseMap : public ProceduralMap {
public:
	NoiseMap( Vector const& m = Vector( 1.0, 1.0, 1.0 ) ) : mask( m ), perlin( 127 ) {}

	Vector get( Vector const& tex ) const {
		return mask * ( 1.0 + perlin.Perlin2D( tex.x * 28.0, tex.y * 28.0, 4, 0.25 ) );
	}

private:
	Vector	mask;
	Perlin	perlin;
};

#endif
