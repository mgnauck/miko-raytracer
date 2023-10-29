
#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <vector>
#include "Renderable.h"
#include "Raytracer.h"
#include "HitProperties.h"

class Light : public Renderable {
public:
	Light() : Renderable(), shadowing( true ) {}
	Light( Matrix const& m, Vector const& c ) : Renderable( m ), color( c ), shadowing( true ) {}
	Light( Light const& rhs ) : Renderable( rhs ), color( rhs.getColor() ), shadowing( rhs.getShadowing() ) {}

	virtual ~Light() {}

	void setColor( Vector const& c ) {
		color = c;
	}

	Vector const& getColor() const {
		return color;
	}

	void setShadowing( bool s ) {
		shadowing = s;
	}

	bool getShadowing() const {
		return shadowing;
	}

	virtual Vector getLightVector( HitProperties const& hp ) const = 0;
	virtual Vector calcIntensity( Raytracer* rt, HitProperties const& hp ) const = 0;

	virtual void transform() {
	}

	bool isLight() const {
		return true;
	}

	bool intersect( Ray const& r, double dist ) const {
		// never use that function to intersect with lights!!
		assert( 0 );
		return false;
	}

	Light& operator=( Light const& rhs ) {
		if( &rhs == this )
			return *this;
		Renderable::operator=( rhs );
		color = rhs.getColor();
		return *this;
	}

protected:
	Vector	color;
	bool	shadowing;
};

#endif
