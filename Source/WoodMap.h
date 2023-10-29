
#ifndef __WOOD_MAP_H__
#define __WOOD_MAP_H__

#include "ProceduralMap.h"
#include "Perlin.h"

class WoodMap : public ProceduralMap {
public:
	WoodMap() : perlin( 227 ) {}

	// FIXME: sucks!
	Vector get( Vector const& tex ) const {

		/*double bump = perlin.Perlin2D( tex.x * 50.0, tex.y * 50.0, 3, 0.25 );
		bump = ( bump < 0.0 ) ? 0.0 : 0.5;*/

		double grain = ( 1.0 + perlin.Perlin2D( tex.x * 18.0, tex.y * 18.0, 1, 0.35 ) ) * 20.0;
		return Vector( 1.0, 0.5, 0.0 ) * ( ( grain - static_cast<int>( grain ) ) );
	}

private:
	Perlin	perlin;
};

#endif
