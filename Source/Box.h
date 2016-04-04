
#ifndef __BOX_H__
#define __BOX_H__

#include "Primitive.h"

class Box : public Primitive {
public:
	Box();
	Box( Matrix const& m, Vector const& minExtend, Vector const& maxExtend );
	Box( Box const& rhs );

	void			setMin( Vector const& minExt );
	void			setMax( Vector const& maxExt );
	Vector const&	getMin() const;
	Vector const&	getMax() const;

	void			transform();

	bool			intersect( Ray const& r, HitProperties* hp ) const;
	bool			intersect( Ray const& r, double dist ) const;
	void			completeHitProperties( HitProperties* hp ) const;

	Box&			operator=( Box const& rhs );

private:
	Vector			min;
	Vector			max;
};

#endif
