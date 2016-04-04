
#ifndef __BLINN_SHADER_H__
#define __BLINN_SHADER_H__

#include "Shader.h"

// TODO: microfacets..fresnel..

class BlinnShader : public Shader {
public:
	Vector evaluate( ShaderProperties const& sp ) const;
};

#endif