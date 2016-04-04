
#ifndef __PHONG_SHADER_H__
#define __PHONG_SHADER_H__

#include "Shader.h"

class PhongShader : public Shader {
public:
	void evaluate( Raytracer* rt, HitProperties* hp );
};

#endif
