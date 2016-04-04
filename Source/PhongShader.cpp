
#include "PhongShader.h"
#include "PhongMaterial.h"
#include "Renderer.h"
#include "Light.h"
#include "GlobalIlluminationLight.h"
#include "Primitive.h"
#include "Map.h"

void PhongShader::evaluate( Raytracer* rt, HitProperties* hp ) {

	// material fetch
	PhongMaterial* mat = dynamic_cast<PhongMaterial*>( dynamic_cast<Primitive*>( hp->objectPtr )->getMaterial() );

	// get global illumination light object (0 if no gi!)
	GlobalIlluminationLight* gi = rt->getScene()->getGlobalIlluminationLight();

	// texture fetch
	Vector textureCol( 1.0, 1.0, 1.0 );
	if( mat->hasTextureMap() )
		textureCol = Renderer::getMapManager()->getMap( mat->getTextureMapID() )->get( hp->texel );

	// normal map fetch
	Vector deriv( 1.0, 1.0, 1.0 );
	if( mat->hasNormalMap() )
		deriv = Renderer::getMapManager()->getMap( mat->getNormalMapID() )->getDerived( hp->texel ); 

	// if normal mapping -> calc pertubed normal
	if( mat->hasNormalMap() )
		hp->normal = ( hp->normal + ( ( deriv.x * hp->tangent ) - ( deriv.y * hp->binormal ) ) ).normalized();

	// calculate view vector (normalize for safety)
	Vector viewVec( hp->ray.direction.negated().normalized() );

	// output
	Vector col( 0.0, 0.0, 0.0 );

	// calc ambient/global term
	if( !gi ) {

		// calc ambient sceneambient * ambient coeff * texture color
		Vector sceneAmbient( rt->getScene()->getGlobalAmbient() );
		sceneAmbient = Vector( 0.3, 0.3, 0.3 );

		col.x += sceneAmbient.x * mat->ambient.x * textureCol.x;
		col.y += sceneAmbient.y * mat->ambient.y * textureCol.y;
		col.z += sceneAmbient.z * mat->ambient.z * textureCol.z;
	}

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

		// combine with texture color
		tCol.x *= textureCol.x;
		tCol.y *= textureCol.y;
		tCol.z *= textureCol.z;

		// calc specular intensity
		Vector refl( lightVec.reflect( hp->normal ) );
		refl.normalize();
		double spec = refl.dot( viewVec );

		// add specular term
		if( spec > 0.0 )
			tCol += mat->specular * pow( spec, mat->exponent );

		// calculate lights color contribution
		Vector lightCol( ( *it )->calcIntensity( rt, *hp ) );

		// combine intensity with light color
		tCol.x *= lightCol.x;
		tCol.y *= lightCol.y;
		tCol.z *= lightCol.z;

		// add to final color
		col += tCol;
	}

	// if gi is enabled samples hemisphere
	if( gi ) {
		// eval global term
		Vector global( gi->calcIntensity( rt, *hp ) );

		// combine with textureCol and add to output color
		col.x += global.x * textureCol.x;
		col.y += global.y * textureCol.x;
		col.z += global.z * textureCol.x;
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
