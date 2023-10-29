
#include "BlinnShader.h"
#include "ShaderProperties.h"
#include "PhongMaterial.h"
#include "Renderer.h"
#include "Light.h"
#include "Map.h"

Vector BlinnShader::evaluate( ShaderProperties const& sp ) const {

	// material fetch
	PhongMaterial*	mat	= dynamic_cast<PhongMaterial*>( sp.material );

	// calc diffuse intensity
	double diff	= sp.lightVector.dot( sp.normal );

	// not facing -> return zero
	if( diff < 0.0 )
		return Vector( 0.0, 0.0, 0.0 );

	// resulting color
	Vector finalCol( 0.0, 0.0, 0.0 );

	// add diffuse term
	finalCol += mat->diffuse * diff;

////// TODO: microfacets..fresnel ..

	// specular mit half vec
	// ( half*nrm )^exp
	Vector refl( ( sp.lightVector + sp.viewVector ) * 0.5 );
	refl.normalize();
	double spec = refl.dot( sp.viewVector );

	// add specular term
	if( spec > 0.0 )
		finalCol += mat->specular * pow( spec, mat->exponent );

	// multiply with lights color
	return finalCol.modulated( sp.lightColor );

}