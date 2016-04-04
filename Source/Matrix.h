
#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "Vector.h"

struct Matrix {
	typedef double value_type;

	Matrix() {}

	Matrix( Matrix const& rhs ) { 
		for( int j=0; j<4; j++ )
			for( int i=0; i<4; i++ )
				m[j][i] = rhs.m[j][i];
	}

	Matrix(	value_type m00, value_type m01, value_type m02, value_type m03,
			value_type m10, value_type m11, value_type m12, value_type m13,
			value_type m20, value_type m21, value_type m22, value_type m23,
			value_type m30, value_type m31, value_type m32, value_type m33 ) {

		m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
		m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
		m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
		m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
	}

	Matrix const& makeIdentity() {
		for( int j=0; j<4; j++ ) {
			for( int i=0; i<4; i++ )
				m[j][i] = value_type( 0 );
			m[j][j] = value_type( 1 );
		}

		return *this;
	}

	void transpose() {
		Matrix	tmp( *this );
		for( int j=0; j<4; j++ )
			for( int i=0; i<4; i++ )
				m[j][i] = tmp( i, j );
	}

	Matrix transposed() const {
		Matrix tmp( *this );
		tmp.transpose();
		return tmp;
	}

	void setTranslation( Vector const& t ) {
		setCol( 3, t );
	}

	Matrix const& makeTranslation( Vector const& t ) {
		makeIdentity();
		setTranslation( t );
		return *this;
	}

	Matrix const& makeScale( Vector const& scale ) {
		makeIdentity();
		m[0][0] = scale.x;
		m[1][1] = scale.y;
		m[2][2] = scale.z;
		return *this;
	}

	Matrix const& makeRotation( Vector const& v, value_type a );		// axis/angle

	Matrix const& makeRotationX( value_type a ) {
		value_type	sina	= sin( a );
		value_type	cosa	= cos( a );

		makeIdentity();

		m[ 1 ][ 1 ] = cosa;
		m[ 1 ][ 2 ] = -sina;
		m[ 2 ][ 1 ] = sina;
		m[ 2 ][ 2 ] = cosa;
		
		return *this;
	}

	Matrix const& makeRotationY( value_type a ) {
		value_type	sina	= sin( a );
		value_type	cosa	= cos( a );

		makeIdentity();

		m[ 0 ][ 0 ] = cosa;
		m[ 0 ][ 2 ] = sina;
		m[ 2 ][ 0 ] = -sina;
		m[ 2 ][ 2 ] = cosa;

		return *this;
	}

	Matrix const& makeRotationZ( value_type a ) {
		value_type	sina	= sin( a );
		value_type	cosa	= cos( a );

		makeIdentity();

		m[ 0 ][ 0 ] = cosa;
		m[ 0 ][ 1 ] = -sina;
		m[ 1 ][ 0 ] = sina;
		m[ 1 ][ 1 ] = cosa;

		return *this;
	}

	Matrix const& makePerspectiveProjection( value_type fov, value_type aspect, value_type near, value_type far );

	Matrix const& setRow( int rowNum, Vector const& v ) {
		m[ rowNum ][ 0 ] = v[ 0 ];
		m[ rowNum ][ 1 ] = v[ 1 ];
		m[ rowNum ][ 2 ] = v[ 2 ];

		return *this;
	}

	Matrix const& setCol( int colNum, Vector const& v ) {
		m[ 0 ][ colNum ] = v[ 0 ];
		m[ 1 ][ colNum ] = v[ 1 ];
		m[ 2 ][ colNum ] = v[ 2 ];

		return *this;
	}

	value_type det2x2( value_type a, value_type b, value_type c, value_type d ) const {
		// a b
		// c d
		return ( a * d ) - ( b * c );
	}

	value_type det3x3(	value_type a1, value_type b1, value_type c1, 
						value_type a2, value_type b2, value_type c2, 
						value_type a3, value_type b3, value_type c3 
						) const {
		// a1 b1 c1
		// a2 b2 c2
		// a3 b3 c3

		return a1 * b2 * c3 + a2 * b3 * c1 + a3 * b1 * c2 - c1 * b2 * a3 - c2 * b3 * a1 - c3 * b1 * a2;
	}

	value_type det() const {

		// laplacescher entwicklungssatz, entwicklung nach der ersten (=0) zeile
		// (es wird immer die j-te spalte und die entwicklungszeile gestrichen)
		// vorzeichen wechsel schachbrettartig ( -1^(i+j) )

		value_type d;

		d = m[ 0 ][ 0 ] * det3x3(	m[1][1], m[1][2], m[1][3],		// 0te spalte gestrichen
									m[2][1], m[2][2], m[2][3],
									m[3][1], m[3][2], m[3][3] );
		
		d -= m[ 0 ][ 1 ] * det3x3(	m[1][0], m[1][2], m[1][3],		// 1te spalte gestrichen
									m[2][0], m[2][2], m[2][3],
									m[3][0], m[3][2], m[3][3] );

		d += m[ 0 ][ 2 ] * det3x3(	m[1][0], m[1][1], m[1][3],		// 2te
									m[2][0], m[2][1], m[2][3],
									m[3][0], m[3][1], m[3][3] );

		d -= m[ 0 ][ 3 ] * det3x3(	m[1][0], m[1][1], m[1][2],		// 3te
									m[2][0], m[2][1], m[2][2],
									m[3][0], m[3][1], m[3][2] );
		return d;
	}

	Matrix inverted() const {

		// inverse = ( 1 / detA ) * transpose( Aadj )
		// Aadj = (-1)^(i+j) * det( Aij ) (A mit ite-zeile & jte-spalte gestrichen)

		value_type d = this->det();

		assert( d != static_cast<value_type>( 0.0 ) );

		Matrix inv;

		inv.m[ 0 ][ 0 ] =  det3x3( m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3] );
		inv.m[ 0 ][ 1 ] = -det3x3( m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3] );
		inv.m[ 0 ][ 2 ] =  det3x3( m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3] );
		inv.m[ 0 ][ 3 ] = -det3x3( m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2] );

		inv.m[ 1 ][ 0 ] = -det3x3( m[0][1], m[0][2], m[0][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3] );
		inv.m[ 1 ][ 1 ] =  det3x3( m[0][0], m[0][2], m[0][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3] );
		inv.m[ 1 ][ 2 ] = -det3x3( m[0][0], m[0][1], m[0][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3] );
		inv.m[ 1 ][ 3 ] =  det3x3( m[0][0], m[0][1], m[0][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2] );

		inv.m[ 2 ][ 0 ] =  det3x3( m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[3][1], m[3][2], m[3][3] );
		inv.m[ 2 ][ 1 ] = -det3x3( m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[3][0], m[3][2], m[3][3] );
		inv.m[ 2 ][ 2 ] =  det3x3( m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[3][0], m[3][1], m[3][3] );
		inv.m[ 2 ][ 3 ] = -det3x3( m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[3][0], m[3][1], m[3][2] );

		inv.m[ 3 ][ 0 ] = -det3x3( m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3] );
		inv.m[ 3 ][ 1 ] =  det3x3( m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3] );
		inv.m[ 3 ][ 2 ] = -det3x3( m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3] );
		inv.m[ 3 ][ 3 ] =  det3x3( m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2] );

		inv.transpose();
		inv *= ( static_cast<value_type>( 1.0 ) / d );

		return inv;
	}

	Vector getTranslation() const {
		return getCol( 3 );
	}

	Vector getRight( bool nrm = true ) const {
		Vector ri( getRow( 0 ) );
		ri.w = 0.0;
		if( !nrm )
			return ri;

		return ri.normalized();
	}

	Vector getUp( bool nrm = true ) const {
		Vector up( getRow( 1 ) );
		up.w = 0.0;
		if( !nrm )
			return up;

		return up.normalized();
	}

	Vector getForward( bool nrm = true ) const {
		Vector fw( getRow( 2 ) );
		fw.w = 0.0;
		if( !nrm )
			return fw;

		return fw.normalized();
	}

	Vector getRow( int rowNum ) const { 
		return Vector(	m[ rowNum ][ 0 ], 
						m[ rowNum ][ 1 ],
						m[ rowNum ][ 2 ],
						m[ rowNum ][ 3 ] );
	}

	Vector getCol( int colNum ) const {
		return Vector(	m[ 0 ][ colNum ], 
						m[ 1 ][ colNum ],
						m[ 2 ][ colNum ],
						m[ 3 ][ colNum ] );
	}

	Matrix const& operator*=( value_type rhs ) {
		for( int j=0; j<4; j++ )
			for( int i=0; i<4; i++ )
				m[j][i] *= rhs;
		return *this;
	}

	Matrix const& operator*=( Matrix const& rhs ) {
		Matrix tmp;
		for( int k=0; k<4; k++ ) {
			for( int j=0; j<4; j++ ) {			
				value_type	sum = value_type( 0 );	
				for( int i=0; i<4; i++ )
					sum += m[j][i] * rhs.m[i][k];
				tmp.m[j][k] = sum;
			}
		}
		*this = tmp;
		return *this;
	}

	Matrix const& operator+=( Matrix const& rhs ) {
		for( int j=0; j<4; j++ )
			for( int i=0; i<4; i++ )
				m[j][i] += rhs( j, i );
		return *this;
	}

	Matrix const& operator-=( Matrix const& rhs ) {
		for( int j=0; j<4; j++ )
			for( int i=0; i<4; i++ )
				m[j][i] -= rhs( j, i );
		return *this;
	}

	Matrix& operator=( Matrix const& rhs ) {
		if( this == &rhs )
			return *this;

		for( int j=0; j<4; j++ )
			for( int i=0; i<4; i++ )
				m[j][i] = rhs.m[j][i];

		return *this;
	}

	value_type& operator()( int row, int col ) {
		return m[row][col];
	}

	const value_type& operator()( int row, int col ) const {
		return m[row][col];
	}

	value_type	m[ 4 ][ 4 ];
};

Matrix operator*( Matrix const& lhs, Matrix::value_type rhs );
Matrix operator*( Matrix::value_type lhs, Matrix const& rhs );
Matrix operator*( Matrix const& lhs, Matrix const& rhs );
Vector operator*( Matrix const& lhs, Vector const& rhs );
Matrix operator+( Matrix const& lhs, Matrix const& rhs );
Matrix operator-( Matrix const& lhs, Matrix const& rhs );
std::ostream& operator<<( std::ostream& lhs, Matrix const& rhs );

#endif
