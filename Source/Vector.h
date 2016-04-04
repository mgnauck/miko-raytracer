
#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <cmath>
#include <algorithm>
#include <cassert>
#include <iostream>
#include "numbers.h"

#define __PORTABLE__

struct Vector {

	typedef double value_type;

	Vector() { w = static_cast<value_type>( 1 ); }
	
	Vector( value_type _x, value_type _y, value_type _z ): x( _x ), y( _y ), z( _z ), w( static_cast<value_type>( 1 ) ) {}
	
	Vector( value_type _x, value_type _y, value_type _z, value_type _w ): x( _x ), y( _y ), z( _z ), w( _w ) {}

	Vector( Vector const& rhs ) {
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
	}

	value_type dot( Vector const& rhs ) const {
		return ( x * rhs[0] ) + ( y * rhs[1] ) + ( z * rhs[2] );
	}

	value_type magnitudeSquared() const {
		return dot( *this );
	}

	value_type magnitude() const {
		return static_cast<value_type>( sqrt( magnitudeSquared() ) );
	}

	Vector cross( Vector const& rhs ) const;
	void normalize();
	Vector normalized() const;
	void homogenize();

	Vector negated() const {
		return Vector( -x, -y, -z, w );
	}

	Vector modulated( Vector const& s ) const {
		Vector tmp( *this );
		tmp.x *= s.x;
		tmp.y *= s.y;
		tmp.z *= s.z;
		return tmp;
	}

	void clamp( double min, double max ) {
		x = ( x < min ) ? min : x;
		x = ( x > max ) ? max : x;
		y = ( y < min ) ? min : y;
		y = ( y > max ) ? max : y;
		z = ( z < min ) ? min : z;
		z = ( z > max ) ? max : z;
	}

	Vector reflect( Vector const& nrm ) const;
	bool refract( Vector const& nrm, value_type n1, value_type n2, Vector* refraction ) const;

	Vector rand() const {						// random points on sphere

		double	t		= rnd( -1.0, 1.0 );
		double	alpha	= rnd( 0.0, 2.0 * PI );
		double	r		= sqrt( 1.0 - t * t );	

		return Vector( r * cos( alpha ), t, r * sin( alpha ) ).normalized();
	}

	Vector hemisphereRand( Vector const& nrm ) {

		Vector tmp = rand();
		while( tmp.dot( nrm ) < 0.0 )			// ahhaha :(
			tmp = rand();

		return tmp;
	}

	unsigned int toRGB32() {

		int	r	= static_cast<int>( x * 255.0 );
		int	g	= static_cast<int>( y * 255.0 );
		int	b	= static_cast<int>( z * 255.0 );

		r = std::max( r, 0 );
		r = std::min( r, 255 );
		g = std::max( g, 0 );
		g = std::min( g, 255 );
		b = std::max( b, 0 );
		b = std::min( b, 255 );

		return ( ( 0xff << 24 ) | ( b << 16 ) | ( g << 8 ) | r );
	}

	Vector const& operator*=( value_type rhs ) {
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	Vector const& operator+=( Vector const& rhs ) {
		x += rhs[0];
		y += rhs[1];
		z += rhs[2];
		return *this;
	}

	Vector const& operator-=( Vector const& rhs ) {
		x -= rhs[0];
		y -= rhs[1];
		z -= rhs[2];
		return *this;
	}

	Vector&	operator=( Vector const& rhs ) {
		if( this == &rhs )
			return *this;

		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;

		return *this;
	}

#ifdef __PORTABLE__

	value_type&	operator[]( int pos ) {
        switch( pos ) {
                case 0: return x;
                case 1: return y;
                case 2: return z;
        }

		return w;
	}

	value_type const& operator[]( int pos ) const {
		switch( pos ) {
			case 0: return x;
			case 1: return y;
			case 2: return z;
		}

		return w;
	}

	value_type	x;
	value_type	y;
	value_type	z;
	value_type	w;

#else

	union {
		struct { value_type x, y, z, w };
		value_type v[ 4 ];
	}

#endif

};

Vector operator*( Vector const& lhs, Vector::value_type rhs );
Vector operator*( Vector::value_type lhs, Vector const& rhs );
Vector operator+( Vector const& lhs, Vector const& rhs );
Vector operator-( Vector const& lhs, Vector const& rhs );
bool operator==( Vector const& lhs, Vector const& rhs );
bool operator!=( Vector const& lhs, Vector const& rhs );
std::ostream& operator<<( std::ostream& lhs, Vector const& rhs );

#endif
