
#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "Renderable.h"
#include "Shadeable.h"

class Primitive : public Renderable, public Shadeable {
public:
	Primitive() : Renderable(), Shadeable() {}
	Primitive( Matrix const& m ) : Renderable( m ), Shadeable() {}
	Primitive( Primitive const& rhs ) : Renderable( rhs ), Shadeable( rhs ) {}
	
	virtual ~Primitive() {}

	bool isLight() const {
		return false;
	}

	Primitive& operator=( Primitive const& rhs ) {
		if( &rhs == this )
			return *this;
		Renderable::operator=( rhs );
		Shadeable::operator=( rhs );
		return *this;
	}
};

#endif
