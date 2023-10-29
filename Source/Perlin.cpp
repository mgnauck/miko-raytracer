
#include "Perlin.h"
#include <cmath>
#include "numbers.h"

Perlin::Perlin( int perlinTableSize ) : tabSize( perlinTableSize ) {

	tab = new value_type [ tabSize * tabSize * tabSize ];
	for( int i=0; i<tabSize*tabSize*tabSize; i++ )
		tab[ i ] = rnd( -1.0, 1.0 );
}

Perlin::~Perlin() {
	delete [] tab;
}

value_type Perlin::interpolate( value_type a, value_type b, double t ) const {
//	return cos( ( a * ( 1.0 - t ) + b * t ) * 0.25 * PI + PI * 0.5 );
	return ( a * ( 1.0 - t ) + b * t );
}

value_type Perlin::getTab( int u, int v ) const {

	// 3x3 boxfilter
	int iu = u % tabSize;
	int iu_1 = abs( u - 1 ) % tabSize;
	int iu1 = ( u + 1 ) % tabSize;
	
	int iv = v % tabSize;
	int iv_1 = abs( v - 1 ) % tabSize;
	int iv1 = ( v + 1 ) % tabSize;

	// corners with 1/16 importance
	value_type v1 = (	tab[ tabSize * iv1 + iu1 ] +
						tab[ tabSize * iv1 + iu_1 ] +
						tab[ tabSize * iv_1 + iu1 ] +
						tab[ tabSize * iv_1 + iu_1 ]	) * 0.0625;

	// direct neighbours with 1/8
	value_type v2 = (	tab[ tabSize * iv + iu1 ] + tab[ tabSize * iv + iu_1 ] +
						tab[ tabSize * iv1 + iu ] + tab[ tabSize * iv_1 + iu ] ) * 0.125;


	value_type v3 = tab[ tabSize * iv + iu ] * 0.25;
	
	return v1 + v2 + v3;
	//return tab[ tabSize * iv + iu ];
}

value_type Perlin::Perlin1D( value_type u, int octaves, value_type persistence ) const {

	// persistence defines steepness
	// octaves defines how many levels we want to sum up

	value_type	out = 0.0;
	value_type	amp = 1.0;	// amplitude

	for( int i=0; i<octaves; i++ ) {

		// fractional part for interpolation
		value_type frac = u - floor( u );

		// get 2x noise
		value_type v1 = tab[ static_cast<int>( u ) % tabSize ];
		value_type v2 = tab[ ( static_cast<int>( u ) + 1 ) % tabSize ];

		// interpolate values linear (no noise smoothing done)
		out += interpolate( v1, v2, frac ) * amp;

		// new amplitude (amp = persistence^i)
		amp = pow( amp, persistence );

		// inc frequency (= 2^i)
		u *= 2.0;
	}

	return out;
}

value_type Perlin::Perlin2D( value_type u, value_type v, int octaves, value_type persistence ) const {

	// persistence defines steepness
	// octaves defines how many levels we want to sum up

	value_type	out = 0.0;
	value_type	amp = 1.0;	// amplitude

	for( int i=0; i<octaves; i++ ) {

		// fractional parts for interpolation
		value_type fracu = u - floor( u );
		value_type fracv = v - floor( v );

		// integer
		int iu	= static_cast<int>( u );
		int iv	= static_cast<int>( v );

		// get 4x noise
		value_type v1 = getTab( iu, iv );
		value_type v2 = getTab( iu + 1, iv );
		value_type v3 = getTab( iu, iv + 1 );
		value_type v4 = getTab( iu + 1, iv + 1 );

		// interpolate values bilinear
		value_type tmp1 = interpolate( v1, v2, fracu );
		value_type tmp2 = interpolate( v3, v4, fracu );
		out += interpolate( tmp1, tmp2, fracv ) * amp;

		// new amplitude (amp = persistence^i)
		amp = pow( amp, persistence );

		// inc frequency (= 2^i)
		u *= 2.0;
		v *= 2.0;
	}

	return out;
}

value_type Perlin::Perlin3D( value_type u, value_type v, value_type w, int octaves, value_type persistence ) const {

	// persistence defines steepness
	// octaves defines how many levels we want to sum up

	value_type	out = 0.0;
	value_type	amp = 1.0;	// amplitude

	for( int i=0; i<octaves; i++ ) {

		// fractional parts for interpolation
		value_type fracu = u - floor( u );
		value_type fracv = v - floor( v );
		value_type fracw = w - floor( w );

		int iu	= static_cast<int>( u ) % tabSize;
		int iv	= static_cast<int>( v ) % tabSize;
		int iw	= static_cast<int>( w ) % tabSize;
		int iu1	= ( iu + 1 ) % tabSize;
		int iv1	= ( iv + 1 ) % tabSize;
		int iw1	= ( iw + 1 ) % tabSize;

		int tabSize2 = tabSize * tabSize;

		// get 8x noise
		value_type v1 = tab[ tabSize2 * iw + tabSize * iv + iu ];
		value_type v2 = tab[ tabSize2 * iw + tabSize * iv + iu1 ];
		value_type v3 = tab[ tabSize2 * iw + tabSize * iv1 + iu ];
		value_type v4 = tab[ tabSize2 * iw + tabSize * iv1 + iu1 ];
		value_type v5 = tab[ tabSize2 * iw1 + tabSize * iv + iu ];
		value_type v6 = tab[ tabSize2 * iw1 + tabSize * iv + iu1 ];
		value_type v7 = tab[ tabSize2 * iw1 + tabSize * iv1 + iu ];
		value_type v8 = tab[ tabSize2 * iw1 + tabSize * iv1 + iu1 ];

		// interpolate values trilinear (no noise smoothing done)
		value_type tmp1 = interpolate( v1, v2, fracu );
		value_type tmp2 = interpolate( v3, v4, fracu );
		value_type tmp3 = interpolate( v5, v6, fracu );
		value_type tmp4 = interpolate( v7, v8, fracu );
		value_type tmp5 = interpolate( tmp1, tmp2, fracv );
		value_type tmp6 = interpolate( tmp3, tmp4, fracv );
		out += interpolate( tmp5, tmp6, fracw ) * amp;

		// new amplitude (amp = persistence^i)
		amp = pow( amp, persistence );

		// inc frequency (= 2^i)
		u *= 2.0;
		v *= 2.0;
		w *= 2.0;
	}

	return out;
}