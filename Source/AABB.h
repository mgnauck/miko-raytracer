
#ifndef __AABB_H__
#define __AABB_H__

#include "BoundingVolume.h"

class AABB : public BoundingVolume {
public:
	AABB();
	~AABB();

	void		create( std::vector<Vertex> const& vertices );
	// TODO:
	bool		intersect( Ray const& r, HitProperties* hp );
	bool		intersect( Ray const& r, double dist );

private:
	Vector		min;
	Vector		max;
};

#endif