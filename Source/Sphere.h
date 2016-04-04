
#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "Primitive.h"

class Sphere : public Primitive {
public:
	Sphere();
	Sphere( Matrix const& m, double r );
	Sphere( Sphere const& rhs );
	~Sphere();

	void		setRadius( double r );
	double		getRadius() const;

	void		transform();
	bool		intersect( Ray const& r, HitProperties* hp ) const;
	bool		intersect( Ray const& r, double dist ) const;
	void		completeHitProperties( HitProperties* hp ) const;

	Sphere&		operator=( Sphere const& rhs );

private:
	double		radius;
};

#endif
