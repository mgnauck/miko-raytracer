
#ifndef __PHONG_MATERIAL_H__
#define __PHONG_MATERIAL_H__

#include "LambertMaterial.h"

class PhongMaterial : public LambertMaterial {
public:
	PhongMaterial() : LambertMaterial() {
		specular	= Vector( 0.0, 0.0, 0.0 );
		exponent	= 0.0;
	}
	virtual ~PhongMaterial() {}

	Vector	specular;
	double	exponent;

};

#endif