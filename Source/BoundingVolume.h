
#ifndef __BOUNDING_VOLUME_H__
#define __BOUNDING_VOLUME_H__

#include <vector>
#include "Intersectable.h"
#include "Vertex.h"

class BoundingVolume : public Intersectable {
public:
	virtual ~BoundingVolume() {}
	virtual void create( std::vector<Vertex> const& vertices ) = 0;
};

#endif