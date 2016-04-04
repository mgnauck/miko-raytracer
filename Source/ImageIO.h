
#ifndef __IMAGEIO_H__
#define __IMAGEIO_H__

#include <string>
#include "BitMap.h"
#include "defines.h"

#ifndef __NO_DEVIL__
	#include "../devIL/il.h"
#else
	#include "BMP.h"
#endif


class ImageIO {
	public:

#ifndef __NO_DEVIL__

		static void init() {
			ilInit();
			ilEnable( IL_CONV_PAL );
		}

		static BitMap* load( std::string const& name ) {

			ILuint	pic;

			ilGenImages( 1, &pic );
			ilBindImage( pic );

			ilEnable( IL_ORIGIN_SET );
			ilOriginFunc( IL_ORIGIN_UPPER_LEFT );

			ilLoadImage( const_cast<char *>( name.c_str() ) );
			ilConvertImage( IL_RGBA, IL_UNSIGNED_BYTE );

			int				w	= ilGetInteger( IL_IMAGE_WIDTH );
			int				h	= ilGetInteger( IL_IMAGE_HEIGHT );
			unsigned int*	ptr	= new unsigned int [ w * h ];

			ilCopyPixels( 0, 0, 0, w, h, 1, IL_RGBA, IL_UNSIGNED_BYTE, ptr );

			BitMap*			m		= new BitMap( w, h );

			// wrap picture
			for( int j=0; j<h; j++ )
				for( int i=0; i<w; i++ )
					m->set( i, j, ptr[ w * ( h - j - 1 ) + i ] );

			delete [] ptr;

			ilDisable( IL_ORIGIN_SET );
			ilDeleteImages( 1, &pic );

			if( ilGetError() == IL_NO_ERROR )
				return m;

			std::cout << "ERROR loading bitmap!" << std::endl;

			return 0;
		}

		static bool save( std::string const& name, BitMap const& m ) {

			ILuint	pic;

			ilGenImages( 1, &pic );
			ilBindImage( pic );

			int				w	= m.getWidth();
			int				h	= m.getHeight();
			unsigned int*	ptr	= new unsigned int [ w * h ];

			// wrap picture
			for( int j=0; j<h; j++ )
				for( int i=0; i<w; i++ )
					ptr[ w * ( h - j - 1 ) + i ] = m.get( i, j );

			ilTexImage( w, h, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, ptr );

			delete [] ptr;

			ilEnable( IL_FILE_OVERWRITE );
			ilSaveImage( const_cast<char *>( name.c_str() ) );
			ilDisable( IL_FILE_OVERWRITE );

			ilDeleteImages( 1, &pic );
		
			if( ilGetError() == IL_NO_ERROR )
				return true;

			std::cout << "ERROR saving bitmap!" << std::endl;

			return false;
		}
#else

		static BitMap* load( std::string const& name ) {
			return BMP::load( name );
		}

		static bool save( std::string const& name, BitMap const& m ) {
			return BMP::save( name, m );
		}

#endif
	
	private:
		ImageIO();
		ImageIO( ImageIO const& );
		ImageIO& operator=( ImageIO const& );
};

#endif
