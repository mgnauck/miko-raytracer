
#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "Vector.h"

struct Triangle {

	typedef int value_type;

	Triangle() {}
	Triangle( value_type _a, value_type _b, value_type _c );

	void calcNormal( Vector const& v1, Vector const& v2, Vector const& v3 );

	value_type const& operator[]( int pos ) const {

		switch( pos ) {
			case 0: return a;
			case 1: return b;
		}

		return c;
	}

	value_type	a;
	value_type	b;
	value_type	c;
	Vector		normal;
};

#endif
