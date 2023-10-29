
#ifndef __TRANSFORMABLE_H__
#define __TRANSFORMABLE_H__

#include "Matrix.h"

class Transformable {
public:
	Transformable() : matrix( Matrix().makeIdentity() ), inverse( matrix ) {}
	Transformable( Matrix const& m ) : matrix( m ), inverse( matrix.inverted() ) {}
	Transformable( Transformable const& rhs ) : matrix( rhs.getMatrix() ), inverse( rhs.getInverse() ) {}
	
	virtual ~Transformable() {}

	void setMatrix( Matrix const& m ) {
		matrix	= m;
		inverse	= matrix.inverted();
	}

	Matrix const& getMatrix() const {
		return matrix;
	}

	Matrix const& getInverse() const {
		return inverse;
	}

	Transformable& operator=( Transformable const& rhs ) {
		if( this == &rhs )
			return *this;
		matrix	= rhs.getMatrix();
		inverse	= rhs.getInverse();
		return *this;	
	}

private:
	Matrix matrix;
	Matrix inverse;
};

#endif