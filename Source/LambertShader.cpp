
#include "LambertShader.h"
#include "LambertMaterial.h"
#include "Renderer.h"
#include "Light.h"
#include "Primitive.h"

void LambertShader::evaluate( Raytracer* rt, HitProperties* hp ) {

	// material fetch
	LambertMaterial* mat = dynamic_cast<LambertMaterial*>( dynamic_cast<Primitive*>( hp->objectPtr )->getMaterial() );

	// init our final color we will calculate with black
	Vector col;

	// calc ambient term
	Vector sceneAmbient( rt->getScene()->getGlobalAmbient() );
	col.x = sceneAmbient.x * mat->ambient.x;
	col.y = sceneAmbient.y * mat->ambient.y;
	col.z = sceneAmbient.z * mat->ambient.z;

	// add materials emission
	col += mat->emission;

	std::vector<Light*> const& lights = rt->getScene()->getLightPtrs();

	// calc influence of every lightsource
	for( std::vector<Light*>::const_iterator it=lights.begin(); it!=lights.end(); ++it ) {

		// calc light vector
		Vector lightVec( ( *it )->getLightVector( *hp ) );
		Vector tCol( 0.0, 0.0, 0.0 );

		// calc diffuse intensity
		double diff = lightVec.dot( hp->normal );

		// no diffuse amount -> skip this lightsource
		if( diff < 0.0 )
			continue;

		// add diffuse (if normal map use pertubed normal)
		tCol += mat->diffuse * diff;

		// calculate lights color contribution
		Vector lightCol( ( *it )->calcIntensity( rt, *hp ) );

		// combine intensity with light color
		tCol.x *= lightCol.x;
		tCol.y *= lightCol.y;
		tCol.z *= lightCol.z;

		// add to final color
		col += tCol;
	}

	// material transparent -> trace non-deflected ray
	if( mat->opacity < 1.0 ) {
		Vector opacity( rt->traceRay( Ray( hp->point + hp->ray.direction * EPSILON, hp->ray.direction ) ) );
		col = opacity * ( 1.0 - mat->opacity ) + col * mat->opacity;
	}

	// evaluate reflection contributions
	if( ( mat->reflection > 0.0 ) && rt->recursionValid() ) {	
		Ray ray( Ray().makeReflectionRay( *hp ) );
		rt->increaseRecursion();
		col += rt->traceRay( ray ) * mat->reflection;
		rt->decreaseRecursion();
	}

	// evaluate transmitted contributions
	if( ( mat->refraction > 0.0 ) && rt->recursionValid() ) {
		Ray ray( Ray().makeRefractionRay( *hp ) );
		rt->increaseRecursion();
		col = rt->traceRay( ray ) * mat->refraction + col * ( 1.0 - mat->refraction );
		rt->decreaseRecursion();
	}

	// clamp color
	col.clamp( 0.0, 1.0 );

	// set to output
	hp->outColor = col;
}
