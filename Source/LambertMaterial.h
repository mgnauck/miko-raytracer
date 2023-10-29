
#ifndef __LAMBERT_MATERIAL_H__
#define __LAMBERT_MATERIAL_H__

#include "Material.h"
#include "Vector.h"

class LambertMaterial : public Material {
public:
	LambertMaterial() : Material() {
		ambient		= Vector( 0.0, 0.0, 0.0 );
		diffuse		= Vector( 0.0, 0.0, 0.0 );
		emission	= Vector( 0.0, 0.0, 0.0 );
	}
	virtual ~LambertMaterial() {}

	Vector	ambient;
	Vector	diffuse;
	Vector	emission;
};

#endif
