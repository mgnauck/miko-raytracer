
#ifndef __BOUNDING_SPHERE_H__
#define __BOUNDING_SPHERE_H__

#include "BoundingVolume.h"

class BoundingSphere : public BoundingVolume {
public:
	BoundingSphere();
	~BoundingSphere();

	void		create( std::vector<Vertex> const& vertices );
	bool		intersect( Ray const& r, HitProperties* hp ) const;
	bool		intersect( Ray const& r, double dist ) const;

private:
	double		radius;
	Vector		center;
};

#endif