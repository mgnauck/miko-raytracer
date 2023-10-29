
#ifndef __RENDERABLE_H__
#define __RENDERABLE_H__

#include "Transformable.h"
#include "Intersectable.h"

// combines properties
class Renderable : public Transformable, public Intersectable {
public:
	Renderable() : Transformable(), Intersectable() {}
	Renderable( Matrix const& m ) : Transformable( m ), Intersectable() {}
	Renderable( Renderable const& rhs ) : Transformable( rhs ), Intersectable( rhs ) {}
	
	virtual ~Renderable() {}

	virtual void transform() = 0;
	virtual bool isLight() const = 0;

	Renderable& operator=( Renderable const& rhs ) {
		if( &rhs == this )
			return *this;
		Transformable::operator=( rhs );
		return *this;
	}
};

#endif
