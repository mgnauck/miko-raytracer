
#include "Vector.h"

Vector Vector::cross( Vector const& rhs ) const {
	Vector tmp;

	tmp.x = y * rhs[2] - z * rhs[1];
	tmp.y = z * rhs[0] - x * rhs[2]; 
	tmp.z = x * rhs[1] - y * rhs[0];
	tmp.w = 1.0;

	return tmp;
}

void Vector::normalize() {
	value_type	tmp = magnitude();

	assert( tmp != static_cast<value_type>( 0.0 ) );
	tmp = static_cast<value_type>( 1.0 ) / tmp;

	x *= tmp;
	y *= tmp;
	z *= tmp;
}

Vector Vector::normalized() const {
	Vector		temp( *this );
	value_type	len = magnitude();

	assert( len != static_cast<value_type>( 0.0 ) );
	len = static_cast<value_type>( 1.0 ) / len;

	temp.x *= len;
	temp.y *= len;
	temp.z *= len;

	return temp;
}

void Vector::homogenize() {
	assert( w != 0 );
	value_type tmp = static_cast<value_type>( 1 ) / w;
	x *= tmp;
	y *= tmp;
	z *= tmp;
	w = static_cast<value_type>( 1 );
}

Vector Vector::reflect( Vector const& nrm ) const {
	value_type	val	= static_cast<value_type>( 2.0 ) * nrm.dot( *this );
	Vector	v( nrm );
	v *= val;
	v -= *this;
	return v;
}

bool Vector::refract( Vector const& nrm, value_type n1, value_type n2, Vector* refraction ) const {
	value_type	n		= n1 / n2;
	value_type	cosI	= nrm.negated().dot( *this );
	value_type	cosT2	= static_cast<value_type>( 1.0 ) - n * n * ( static_cast<value_type>( 1.0 ) - cosI * cosI );

	if( cosT2 > 0.0 ) {

		*refraction = *this;
		*refraction	*= n;

		value_type	val = sqrt( cosT2 ) - n * cosI;
		Vector tmp( nrm );
		tmp *= val;

		*refraction	-= tmp;

		return true;
	}

	return false;
}

Vector operator*( Vector const& lhs, Vector::value_type rhs ) {
	return Vector( lhs ) *= rhs;
}

Vector operator*( Vector::value_type lhs, Vector const& rhs ) {
	return rhs * lhs;
}

Vector operator+( Vector const& lhs, Vector const& rhs ) {
	return Vector( lhs ) += rhs;
}

Vector operator-( Vector const& lhs, Vector const& rhs ) {
	return Vector( lhs ) -= rhs;
}

bool operator==( Vector const& lhs, Vector const& rhs ) {
	for( int i=0; i<4; i++ )
		if( static_cast<Vector::value_type>( fabs( lhs[i] - rhs[i] ) ) > static_cast<Vector::value_type>( EPSILON ) )
			return false;
	return true;
}

bool operator!=( Vector const& lhs, Vector const& rhs ) {
	return !( lhs == rhs );
}

std::ostream& operator<<( std::ostream& lhs, Vector const& rhs ) {
	lhs << rhs.x << " " << rhs.y << " " << rhs.z << " " << rhs.w << std::endl;
	return lhs;
}
