
#include "Raytracer.h"
#include "Renderable.h"
#include "Renderer.h"
#include "HitProperties.h"
#include "Material.h"
#include "Shader.h"
#include "Light.h"
#include "Primitive.h"
#include "GlobalIlluminationLight.h"

Raytracer::Raytracer( Scene* s, RecursionProperties const& mr ) : scene( s ), maxRecursion( mr ) {
}

Raytracer::~Raytracer() {
	delete scene;
}

RecursionProperties const& Raytracer::getActRecursion() const {
	return actRecursion;
}

Scene const* Raytracer::getScene() const {
	return scene;
}

void Raytracer::resetRecursion() {
	actRecursion.depth			= 0;
	actRecursion.samples		= maxRecursion.samples;
	actRecursion.lightSamples	= maxRecursion.lightSamples;
}

void Raytracer::increaseRecursion() {
	actRecursion.depth++;
	actRecursion.samples		/= 2;
	actRecursion.lightSamples	/= 2;
}

void Raytracer::decreaseRecursion() {
	actRecursion.depth--;
	actRecursion.samples		*= 2;
	actRecursion.lightSamples	*= 2;
}

bool Raytracer::recursionValid() const {
	return ( actRecursion.depth <= maxRecursion.depth );
}

void Raytracer::enableGlobalIllumination() {
	scene->setGlobbalIlluminationLight( new GlobalIlluminationLight() );
}

Vector Raytracer::traceRay( Ray const& ray ) {

	assert( scene );

	// our actual hit properties
	HitProperties hp;

	// intersect ray with scene. find nearest hit
	if( !scene->intersect( ray, &hp ) )
		return scene->getBackgroundColor();
		
	// hit light? then cast to light and return color
	if( hp.objectPtr->isLight() )
		return dynamic_cast<Light*>( hp.objectPtr )->getColor();

	// hit primitive! cast object to primitive
	Primitive* primitive = dynamic_cast<Primitive*>( hp.objectPtr );

	// fill missing properties for hit
	primitive->completeHitProperties( &hp );

	// use primitives shader to calculate color at hitpoint
	primitive->getShader()->evaluate( const_cast<Raytracer*>( this ), &hp );

	// return calculated color
	return hp.outColor;
}
