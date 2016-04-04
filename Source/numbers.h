
#ifndef __NUMBERS_H__
#define __NUMBERS_H__

#include <cstdlib>

typedef double value_type;

extern value_type const	PI;
extern value_type const	TWOPI;
extern value_type const	EPSILON;
extern value_type const	DEG2RAD;
extern value_type const	RAD2DEG;

template<typename T>
T rnd( T min, T max ) {
	return min + ( max - min ) * ( std::rand() / static_cast<T>( RAND_MAX ) );
}

#endif
