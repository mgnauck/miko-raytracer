
#include "Matrix.h"

Matrix const& Matrix::makeRotation( Vector const& v, value_type a ) {	// axis + angle
																		// assumes axis is normalized
	value_type	sina	= sin( a );										// otherwise scaling :> :>
	value_type	cosa	= cos( a );
	value_type	cosa1	= static_cast<value_type>( 1.0 ) - cosa;
	value_type	xy		= v.x * v.y;
	value_type	xz		= v.x * v.z;
	value_type	yz		= v.y * v.z;
	value_type	xsina	= v.x * sina;
	value_type	ysina	= v.y * sina;
	value_type	zsina	= v.z * sina;

	m[0][0] = cosa + v.x * v.x * cosa1;
	m[1][0] = zsina + xy * cosa1;
	m[2][0] = -ysina + xz * cosa1;
	m[0][1] = -zsina + xy * cosa1;
	m[1][1] = cosa + v.y * v.y * cosa1;
	m[2][1] = xsina + yz * cosa1;
	m[0][2] = ysina + xz * cosa1;
	m[1][2] = -xsina + yz * cosa1;
	m[2][2] = cosa + v.z * v.z * cosa1;

	m[3][0] = m[3][1] = m[3][2] = m[0][3] = m[1][3] = m[2][3] = 0.0;
	m[3][3] = 1.0;

	return *this;
}

Matrix const& Matrix::makePerspectiveProjection( value_type fov, value_type aspect, value_type near, value_type far ) {

	/*
		perspective projection (stöcker:)

		2*near/(right-left)				0				(right+left)/(right-left)				0
		0						2*near/(top-bottom)		(top+bottom)/(top-bottom)				0
		0								0				-(far+near)/(far-near)		-2*far*near/(far-near)
		0								0						-1								0

	*/

	value_type	y		= static_cast<value_type>( near * tan( fov * PI  / 360.0 ) );
	value_type	x		= y * aspect;

	value_type	top		= y;
	value_type	bottom	= -y;
	value_type	left	= -x;
	value_type	right	= x;

	value_type	a		= static_cast<value_type>( 1.0 / ( right - left ) );
	value_type	b		= static_cast<value_type>( 1.0 / ( top - bottom ) );
	value_type	c		= static_cast<value_type>( 1.0 / ( far - near ) );
	value_type	near2	= near + near;

	*this = Matrix(	near2 * a, static_cast<value_type>( 0.0 ), ( right + left ) * a, static_cast<value_type>( 0.0 ),
					static_cast<value_type>( 0.0 ), near2 * b, ( top + bottom ) * b, static_cast<value_type>( 0.0 ),
					static_cast<value_type>( 0.0 ), static_cast<value_type>( 0.0 ), -( far + near ) * c, -far * near2 * c,
					static_cast<value_type>( 0.0 ), static_cast<value_type>( 0.0 ), static_cast<value_type>( -1.0 ), static_cast<value_type>( 0.0 )
			);

	return *this;
}

Matrix operator*( Matrix const& lhs, Matrix::value_type rhs ) {
	return Matrix( lhs ) *= rhs;	
}

Matrix operator*( Matrix::value_type lhs, Matrix const& rhs ) {
	return rhs * lhs;
}

Matrix operator*( Matrix const& lhs, Matrix const& rhs ) {
	return Matrix( lhs ) *= rhs;
}

Vector operator*( Matrix const& lhs, Vector const& rhs ) {
	Vector tmp;
	for( int j=0; j<4; j++ ) {
		Matrix::value_type sum = static_cast<Matrix::value_type>( 0.0 );
		for( int i=0; i<4; i++ )
			sum += lhs( j, i ) * rhs[ i ];
		tmp[j] = sum;
	}
	return tmp;
}

Matrix operator+( Matrix const& lhs, Matrix const& rhs ) {
	return Matrix( lhs ) += rhs;
}

Matrix operator-( Matrix const& lhs, Matrix const& rhs ) {
	return Matrix( lhs ) -= rhs;
}


std::ostream& operator<<( std::ostream& lhs, Matrix const& rhs ) {

	lhs << rhs.getRow( 0 );
	lhs << rhs.getRow( 1 );
	lhs << rhs.getRow( 2 );
	lhs << rhs.getRow( 3 );
	return lhs;
}
