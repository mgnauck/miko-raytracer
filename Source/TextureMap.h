
#ifndef __TEXTURE_MAP_H__
#define __TEXTURE_MAP_H__

#include "Vector.h"

class TextureMap {
public:
	virtual ~TextureMap() {}
	virtual Vector getTexel( Vector const& pos ) const = 0;
};

#endif