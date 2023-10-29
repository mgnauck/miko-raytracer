
#ifndef __SHADER_H__
#define __SHADER_H__

class Raytracer;
class HitProperties;
struct RecursionProperties;

class Shader {
public:
	virtual ~Shader() {}
	virtual void evaluate( Raytracer* rt, HitProperties* hp ) = 0;
};

#endif