
#include "Triangle.h"

Triangle::Triangle( value_type _a, value_type _b, value_type _c ) : a( _a ), b( _b ), c( _c ), normal( 0.0, 0.0, 0.0, 0.0 ) {
}

void Triangle::calcNormal( Vector const& v1, Vector const& v2, Vector const& v3 ) {
	normal		= ( v2 - v1 ).cross( v3 - v1 );
	normal.w	= 0.0;
	normal.normalize();
}
