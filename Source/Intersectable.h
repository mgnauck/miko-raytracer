
#ifndef __INTERSECTABLE_H__
#define __INTERSECTABLE_H__

class Ray;
class HitProperties;

class Intersectable {
public:
	virtual ~Intersectable() {}

	virtual bool intersect( Ray const& r, HitProperties* hp ) const = 0;
	virtual bool intersect( Ray const& r, double dist ) const = 0;
};

#endif