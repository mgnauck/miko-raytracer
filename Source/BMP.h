
#ifndef __BMP_H__
#define __BMP_H__

#include <string>
#include "Bitmap.h"

class BMP {
	public:
		static BitMap*	load( std::string const& filename );
		static bool		save( std::string const& filename, BitMap const& map );

	private:
		BMP();
		BMP( BMP const& );
		BMP& operator=( BMP const& );
};

#endif