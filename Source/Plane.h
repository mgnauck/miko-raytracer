
#ifndef __PLANE_H__
#define __PLANE_H__

#include "Primitive.h"

class Plane : public Primitive {
public:
	Plane();
	Plane( Matrix const& m, double length, double width );
	Plane( Plane const& rhs );
	~Plane();

	Vector const&	getNormal() const;
	double			getD() const;
	double			getLength() const;
	double			getWidth() const;

	void			transform();
	bool			intersect( Ray const& r, HitProperties* hp ) const;
	bool			intersect( Ray const& r, double dist ) const;
	void			completeHitProperties( HitProperties* hp ) const;

	Plane&			operator=( Plane const& rhs );

private:
	double			d;
	double			length;
	double			width;
	Vector			normal;
};

#endif
