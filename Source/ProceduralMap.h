
#ifndef __PROCEDURAL_MAP_H__
#define __PROCEDURAL_MAP_H__

#include "Map.h"

class ProceduralMap : public Map {
public:
	virtual ~ProceduralMap() {}

	bool isProcedural() const {
		return true;
	}

	// TODO: implement for procedural maps
	virtual Vector getDerived( Vector const& tex ) const {
		return Vector( 1.0, 1.0, 1.0 );
	}
};

#endif
