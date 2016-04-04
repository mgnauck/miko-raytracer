
#include "BMP.h"
#include <cstdio>
#include <cassert>

BitMap* BMP::load( std::string const& filename ) {

	FILE*	f	= fopen( filename.c_str(), "rb" );

	assert( f );

	// bitmap file header
	unsigned short	bfType;
	unsigned int	bfSize;
	unsigned short	bfReserved1;
	unsigned short	bfReserved2;
	unsigned int	bfOffbits;

	fread( &bfType, 2, 1, f );
	fread( &bfSize, 4, 1, f );
	fread( &bfReserved1, 2, 1, f );
	fread( &bfReserved2, 2, 1, f );
	fread( &bfOffbits, 4, 1, f );

	// bitmap info header
    unsigned int	biSize;
    int				biWidth;
    int				biHeight;
    unsigned short	biPlanes;
    unsigned short	biBitCount;
    unsigned int	biCompression;
    unsigned int	biSizeImage;
    int				biXPelsPerMeter;
    int				biYPelsPerMeter;
    unsigned int	biClrUsed;
    unsigned int	biClrImportant;

	fread( &biSize, 4, 1, f );
	fread( &biWidth, 4, 1, f );
	fread( &biHeight, 4, 1, f );
	fread( &biPlanes, 2, 1, f );
	fread( &biBitCount, 2, 1, f );
	fread( &biCompression, 4, 1, f );
	fread( &biSizeImage, 4, 1, f );
	fread( &biXPelsPerMeter, 4, 1, f );
	fread( &biYPelsPerMeter, 4, 1, f );
	fread( &biClrUsed, 4, 1, f );
	fread( &biClrImportant, 4, 1, f );

	BitMap* m = new BitMap( biWidth, biHeight );

	// read data
	for( int j=0; j<biHeight; j++ ) {

		for( int i=0; i<biWidth; i++ ) {

			unsigned char	a, r, g, b;

			fread( &b, 1, 1, f );
			fread( &g, 1, 1, f );
			fread( &r, 1, 1, f );

			if( biBitCount == 32 )
				fread( &a, 1, 1, f );	
			else if( biBitCount == 24 )
				a = 0xff;

			m->set( i, biHeight - j - 1, ( a << 24 ) | ( b << 16 ) | ( g << 8 ) | r );
		}
	}

	fclose( f );

	return m;
}

bool BMP::save( std::string const& filename, BitMap const& map ) {

	FILE*	f	= fopen( filename.c_str(), "wb" );

	assert( f );

	int	width	= map.getWidth();
	int	height	= map.getHeight();

	// bitmap file header
	unsigned short	bfType			= 0x4D42;								// "BM"
	unsigned int	bfSize			= 54 + width * height * 3;
	unsigned short	bfReserved1		= 0;
	unsigned short	bfReserved2		= 0;
	unsigned int	bfOffbits		= 54;
	
	fwrite( &bfType, 2, 1, f );
	fwrite( &bfSize, 4, 1, f );
	fwrite( &bfReserved1, 2, 1, f );
	fwrite( &bfReserved2, 2, 1, f );
	fwrite( &bfOffbits, 4, 1, f );

	// bitmap info header
    unsigned int	biSize			= 40;							// bmih size
    int				biWidth			= width;
    int				biHeight		= height;
    unsigned short	biPlanes		= 1;
    unsigned short	biBitCount		= 24;							// RGB
    unsigned int	biCompression	= 0;							// type: RGB
    unsigned int	biSizeImage		= width * height * 3;
    int				biXPelsPerMeter	= 2925;
    int				biYPelsPerMeter	= 2925;
    unsigned int	biClrUsed		= 0;
    unsigned int	biClrImportant	= 0;

	fwrite( &biSize, 4, 1, f );
	fwrite( &biWidth, 4, 1, f );
	fwrite( &biHeight, 4, 1, f );
	fwrite( &biPlanes, 2, 1, f );
	fwrite( &biBitCount, 2, 1, f );
	fwrite( &biCompression, 4, 1, f );
	fwrite( &biSizeImage, 4, 1, f );
	fwrite( &biXPelsPerMeter, 4, 1, f );
	fwrite( &biYPelsPerMeter, 4, 1, f );
	fwrite( &biClrUsed, 4, 1, f );
	fwrite( &biClrImportant, 4, 1, f );

	// write data
	for( int j=0; j<height; j++ ) {

		for( int i=0; i<width; i++ ) {

			unsigned int	c	= map.get( i, height - j - 1 );

			unsigned char	b	= ( c >> 16 ) & 0xff;
			unsigned char	g	= ( c >> 8 ) & 0xff;
			unsigned char	r	= c & 0xff;

			fwrite( &b, 1, 1, f );
			fwrite( &g, 1, 1, f );
			fwrite( &r, 1, 1, f );
		}
	}

	fclose( f );

	return true;
}