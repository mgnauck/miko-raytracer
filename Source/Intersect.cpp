
#include "Intersect.h"
#include "Ray.h"

bool Intersect::RayPlane( Ray const& r, double d, Vector const& nrm, double* t ) {
	
	double alpha = nrm.dot( r.direction );

	// if not ray parallel to plane (alpha=0) or backfacing (alpha>0)
	if( alpha < -EPSILON ) {

		*t = -( nrm.dot( r.origin ) + d ) / alpha;	// calculate distance
		if( *t > EPSILON )							// eliminate plane behind ray cases
			return true;
	}

	return false;
}

bool Intersect::RayPlane( Ray const& r, double d, Vector const& nrm, double dist, double* t ) {

	double alpha = nrm.dot( r.direction );

	// if not ray parallel to plane (alpha=0) or backfacing (alpha>0)
	if( alpha < -EPSILON ) {

		*t = -( nrm.dot( r.origin ) + d ) / alpha;	// calculate distance
		if( ( *t > EPSILON ) && ( *t < dist ) )		// eliminate plane behind ray + greater dist cases
			return true;
	}

	return false;
}

bool Intersect::RaySphere( Ray const& r, Vector const& center, double radius, double* tnear, double* tfar, bool* in ) {

	Vector	oc	= r.origin - center;
	double	b	= oc.dot( r.direction );
	double	c	= oc.magnitudeSquared() - ( radius * radius );
	double	det	= ( b * b ) - c;

	if( det > 0.0 ) {						// solvable?

		det			= sqrt( det );
		double	t1	= -b - det;				// 2 solutions
		double	t2	= -b + det;

		if( t2 > EPSILON ) {				// find nearest, (t2<0) = sphere behind ray

			if( t1 > EPSILON ) {			// ray from outside
				*tnear = t1;
				*tfar = t2;
				*in = false;
			} else {						// ray from inside
				*tnear = t2;
				*tfar = t1;
				*in = true;
			}

			return true;
		}
	}

	return false;
}

bool Intersect::RaySphere( Ray const& r, Vector const& center, double radius, double dist ) {

	Vector	oc	= r.origin - center;
	double	b	= oc.dot( r.direction );
	double	c	= oc.magnitudeSquared() - ( radius * radius );
	double	det	= ( b * b ) - c;

	if( det > 0.0 ) {						// solvable?

		det			= sqrt( det );
		double	t1	= -b - det;				// 2 solutions
		double	t2	= -b + det;

		if( t2 > EPSILON ) {				// find nearest, (t2<0) = sphere behind ray

			if( t1 > EPSILON ) {			// ray from outside
				if( t1 < dist )				// check if in distance
					return true;
			} else {						// ray from inside
				if( t2 < dist )				// check if in distance
					return true;
			}
		}
	}

	return false;
}

bool Intersect::RayCylinder( Ray const& r, Vector const& center, double radius, double* tnear, double* tfar, bool* in ) {

	double	x	= r.origin.x - center.x;
	double	z	= r.origin.z - center.z;

	double	a	= ( r.direction.x * r.direction.x ) + ( r.direction.z * r.direction.z );
	double	b	= ( x * r.direction.x ) + ( z * r.direction.z );
	double	c	= ( x * x ) + ( z * z ) - ( radius * radius );

	double	det	= ( b * b ) - ( a * c );

	if( det > 0.0 ) {										// solvable?

		det			= sqrt( det );
		double	t1	= ( -b - det ) / a;						// 2 solutions
		double	t2	= ( -b + det ) / a;

		if( t2 > EPSILON ) {								// find nearest, (t2<0) cylinder behind ray

			if( t1 > EPSILON ) {							// ray from outside
				*tnear = t1;
				*tfar = t2;
				*in = false;
			} else {										// ray from inside
				*tnear = t2;
				*tfar = t1;
				*in = true;
			}

			return true;
		}
	}

	return false;
}

bool Intersect::RayCylinder( Ray const& r, Vector const& center, double radius, double dist ) {

	double	x	= r.origin.x - center.x;
	double	z	= r.origin.z - center.z;

	double	a	= ( r.direction.x * r.direction.x ) + ( r.direction.z * r.direction.z );
	double	b	= ( x * r.direction.x ) + ( z * r.direction.z );
	double	c	= ( x * x ) + ( z * z ) - ( radius * radius );

	double	det	= ( b * b ) - ( a * c );

	if( det > 0.0 ) {										// solvable?

		det			= sqrt( det );
		double	t1	= ( -b - det ) / a;						// 2 solutions
		double	t2	= ( -b + det ) / a;

		if( t2 > EPSILON ) {								// find nearest, (t2<0) cylinder behind ray

			if( t1 > EPSILON ) {							// ray from outside
				if( t1 < dist )								// in dist
					return true;
			} else {										// ray from inside
				if( t2 < dist )								// in dist
					return true;
			}
		}
	}

	return false;
}

bool Intersect::RayBox( Ray const& r, Vector const& center, Vector const& min, Vector const& max, double* tnear, double* tfar, bool* inside, Vector* nrm ) {

	Vector	bounds[ 2 ]	= { center + min, center + max };

	*tnear	= -1000000.0;
	*tfar	= 1000000.0;

	for( int i=0; i<3; i++ ) {

		Vector actNrm( 0.0, 0.0, 0.0, 0.0 );
		actNrm[ i ] = -1.0;

		double alpha = actNrm.dot( r.direction );

		// ray direction parallel to plane?
		if( ( alpha > -EPSILON ) && ( alpha < EPSILON ) ) {

			// ray origin not between slabs?
			if( ( center[ i ] < bounds[ 0 ][ i ] ) && ( center[ i ] > bounds[ 1 ][ i ] ) )
				return false;

		} else {

			// precalc
			alpha = 1.0 / alpha;

			// Intersect:: with the two planes (= the two slabs)
			double dot	= actNrm.dot( r.origin );
			double t1	= -( dot + min[ i ] ) * alpha;
			double t2	= -( dot + max[ i ] ) * alpha;

			bool swap = false;

			if( t1 > t2 ) {
				double tmp = t1;
				t1	= t2;
				t2	= tmp;
				swap= true;
			}

			// find biggest tnear
			if( t1 > *tnear ) {
				
				*tnear = t1;

				// respective normal
				if( !swap )
					*nrm = actNrm;
				else
					*nrm = actNrm.negated();
			}

			// find smallest tfar
			if( t2 < *tfar )
				*tfar = t2;

			// check
			if( ( *tnear > *tfar ) || ( *tfar < EPSILON ) )
				return false;
		}
	}

	// ray origin inside the box..
	if( *tnear < EPSILON ) {
		double tmp = *tnear;
		*tnear = *tfar;
		*tfar = tmp;
		*inside	= true;
		//*nrm = nrm->negated();
	}

	return true;
}

bool Intersect::RayBox( Ray const& r, Vector const& center, Vector const& min, Vector const& max, double dist ) {

	Vector	bounds[ 2 ]	= { center + min, center + max };
	double	tnear		= -1000000.0;
	double	tfar		= 1000000.0;

	for( int i=0; i<3; i++ ) {

		Vector actNrm( 0.0, 0.0, 0.0, 0.0 );
		actNrm[ i ] = -1.0;

		double alpha = actNrm.dot( r.direction );

		// ray direction parallel to plane?
		if( ( alpha > -EPSILON ) && ( alpha < EPSILON ) ) {

			// ray origin not between slabs?
			if( ( center[ i ] < bounds[ 0 ][ i ] ) && ( center[ i ] > bounds[ 1 ][ i ] ) )
				return false;

		} else {

			// precalc
			alpha = 1.0 / alpha;

			// Intersect:: with the two planes (= the two slabs)
			double dot	= actNrm.dot( r.origin );
			double t1	= -( dot + min[ i ] ) * alpha;
			double t2	= -( dot + max[ i ] ) * alpha;

			if( t1 > t2 ) {
				double tmp = t1;
				t1	= t2;
				t2	= tmp;
			}

			// find biggest tnear
			if( t1 > tnear )			
				tnear = t1;

			// find smallest tfar
			if( t2 < tfar )
				tfar = t2;

			// check
			if( ( tnear > tfar ) || ( tfar < EPSILON ) )
				return false;
		}
	}

	// check if in dist
	if( tnear >= dist )
		return false;

	if( ( tnear < EPSILON ) && ( tfar >= dist ) )
		return false;

	return true;
}

bool Intersect::RayTriangle( Ray const& r, Vector const& v0, Vector const& v1, Vector const& v2, double* t, double* u, double* v ) {

	Vector a = v1 - v0;
	Vector b = v2 - v0;
	Vector c = r.origin - v0;
	Vector d = r.direction;
	Vector p = d.cross( b );

	double detA = a.dot( p );							// calc determinant

	if( ( detA > -EPSILON ) && ( detA < EPSILON ) )		// could not invert matrix (detA=0)
		return false;									

	detA = 1.0 / detA;

	double detA1 = detA * c.dot( p );					// = u
	if( ( detA1 < 0.0 ) || ( detA1 > 1.0 ) )			// outside of triangle (u<0 || u>1)
		return false;

	Vector q = c.cross( a );

	double detA2 = detA * d.dot( q );					// = v
	if( ( detA2 < 0.0 ) || ( detA2 > 1.0 ) )			// outside of tri (v<0 || v>1)
		return false;

	if( ( detA1 + detA2 ) > 1.0 )						// outside of tri (u+v>1)
		return false;

	double detA0 = detA * b.dot( q );					// = t
	if( detA0 < EPSILON )								// triangle behind ray origin
		return false;

	*t = detA0;
	*u = detA1;
	*v = detA2;

	return true;
}

bool Intersect::RayTriangle( Ray const& r, Vector const& v0, Vector const& v1, Vector const& v2, double dist ) {

	Vector a = v1 - v0;
	Vector b = v2 - v0;
	Vector c = r.origin - v0;
	Vector d = r.direction;
	Vector p = d.cross( b );

	double detA = a.dot( p );							// calc determinat

	if( ( detA > -EPSILON ) && ( detA < EPSILON ) )		// could not invert matrix (detA=0)
		return false;									

	detA = 1.0 / detA;

	double detA1 = detA * c.dot( p );					// = u
	if( ( detA1 < 0.0 ) || ( detA1 > 1.0 ) )			// outside of triangle (u<0 || u>1)
		return false;

	Vector q = c.cross( a );

	double detA2 = detA * d.dot( q );					// = v
	if( ( detA2 < 0.0 ) || ( detA2 > 1.0 ) )			// outside of tri (v<0 || v>1)
		return false;

	if( ( detA1 + detA2 ) > 1.0 )						// outside of tri (u+v>1)
		return false;

	double detA0 = detA * b.dot( q );					// = t
	if( ( detA0 < EPSILON ) || ( detA0 >= dist ) )		// triangle behind ray origin or not in dist
		return false;

	return true;
}
