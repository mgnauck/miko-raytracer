
#ifndef __CHECKER_MAP_H__
#define __CHECKER_MAP_H__

#include "ProceduralMap.h"

class CheckerMap : public ProceduralMap {
public:
	CheckerMap( double w, double h, Vector const& c1, Vector const& c2 ) : invWidth( 1.0 / w ), invHeight( 1.0 / h ), col1( c1 ), col2( c2 ) {}

	Vector get( Vector const& tex ) const {
		// x xor y = even -> col1
		// x xor y = odd -> col2
		return ( ( static_cast<int>( tex.x * invWidth ) % 2 ) ^ ( static_cast<int>( tex.y * invHeight ) % 2 ) ) ? col1 : col2;
	}

private:
	double	invWidth;
	double	invHeight;
	Vector	col1;
	Vector	col2;
};

#endif
