
#ifndef __MAP_H__
#define __MAP_H__

#include "Vector.h"

class Map {
public:
	virtual ~Map() {}

	virtual bool isProcedural() const {
		return false;
	}

	virtual Vector get( Vector const& tex ) const = 0;			// bilinear filtered
	virtual Vector getDerived( Vector const& tex ) const = 0;	// differential in u dir
};

#endif