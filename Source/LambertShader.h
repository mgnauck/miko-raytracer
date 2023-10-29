
#ifndef __LAMBERT_SHADER_H__
#define __LAMBERT_SHADER_H__

#include "Shader.h"

class LambertShader : public Shader {
public:
	void evaluate( Raytracer* rt, HitProperties* hp );
};

#endif