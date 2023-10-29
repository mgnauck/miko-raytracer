
#ifndef __BIT_MAP_H__
#define __BIT_MAP_H__

#include <vector>
#include "Map.h"

double const NORMAL_MAP_SCALE = 33.37;

class BitMap : public Map {
	public:
		BitMap() : width( 0 ), height( 0 ) {}

		BitMap( int w, int h ) : width( w ), height( h ), data( w * h, 0x0 ) {
		}

		virtual ~BitMap() {
			data.clear();
		}

		BitMap( BitMap const& rhs ) {

			width	= rhs.getWidth();
			height	= rhs.getHeight();

			data.clear();

			for( int j=0; j<height; j++ )
				for( int i=0; i<width; i++ )
					data.push_back( rhs.get( i, j ) );
		}

		void resize( int w, int h ) {

			width	= w;
			height	= h;
			data.resize( w * h );
		}

		void set( int x, int y, unsigned int c ) {
			data[ width * y + x ] = c;
		}

		unsigned int get( int x, int y ) const {
			return data[ width * y + x ];
		}

		Vector get( Vector const& tex ) const {

			// bilinear filter (sample und lerp 4 texels)
			double tx = abs( tex.x );
			double ty = abs( tex.y );

			double fracX1 = tx - floor( tx );
			double fracY1 = ty - floor( ty );
			double fracX2 = 1.0 - fracX1;
			double fracY2 = 1.0 - fracY1;

			tx *= static_cast<double>( width );
			ty *= static_cast<double>( height );

			double t  = 1.0 / 255.0;
			double w1 = fracX2 * fracY2 * t;
			double w2 = fracX1 * fracY2 * t;
			double w3 = fracX2 * fracY1 * t;
			double w4 = fracX1 * fracY1 * t;

			int ix1 = static_cast<int>( tx ) % width;
			int iy1 = static_cast<int>( ty ) % height;
			int ix2 = ( ix1 + 1 ) % width;
			int iy2 = ( iy1 + 1 ) % height;

			int ofs1 = width * iy1;
			int ofs2 = width * iy2;

			unsigned int c1	= data[ ofs1 + ix1 ];
			unsigned int c2	= data[ ofs1 + ix2 ];
			unsigned int c3	= data[ ofs2 + ix1 ];
			unsigned int c4	= data[ ofs2 + ix2 ];

			Vector v1( c1 & 0xff, ( c1 >> 8 ) & 0xff, ( c1 >> 16 ) & 0xff );
			Vector v2( c2 & 0xff, ( c2 >> 8 ) & 0xff, ( c2 >> 16 ) & 0xff );
			Vector v3( c3 & 0xff, ( c3 >> 8 ) & 0xff, ( c3 >> 16 ) & 0xff );
			Vector v4( c4 & 0xff, ( c4 >> 8 ) & 0xff, ( c4 >> 16 ) & 0xff );

			return v1 * w1 + v2 * w2 + v3 * w3 + v4 * w4;
		}

		Vector getDerived( Vector const& tex ) const {

			// assumes grayscale heightmap
			int		x	= static_cast<int>( abs( tex.x * static_cast<double>( width ) ) ) % width;
			int		y	= static_cast<int>( abs( tex.y * static_cast<double>( height ) ) ) % height;
			
			double	inv	= 1.0 / 255.0;

			double	h1	= static_cast<double>( get( x, y ) & 0xff ) * inv;
			double	h2	= static_cast<double>( get( ( x + 1 ) % width, y ) & 0xff ) * inv;
			double	h3	= static_cast<double>( get( x, ( y + 1 ) % height ) & 0xff ) * inv;
			
			// Vec( dx, dy, .. ) * scale
			return Vector( h2 - h1, h3 - h1, 0.0, 0.0 ) * NORMAL_MAP_SCALE;
		}


		int const getWidth() const {
			return width;
		}

		int const getHeight() const {
			return height;
		}

		void* getPtr() {
			return &data[ 0 ];
		}

		unsigned int getSize() const {
			return width * height;
		}

		BitMap& operator=( BitMap const& rhs ) {
			if( &rhs == this )
				return *this;

			width	= rhs.getWidth();
			height	= rhs.getHeight();

			data.clear();

			for( int j=0; j<height; j++ )
				for( int i=0; i<width; i++ )
					data.push_back( rhs.get( i, j ) );

			return *this;
		}

	protected:
		int							width;
		int							height;
		std::vector<unsigned int>	data;
};

#endif
