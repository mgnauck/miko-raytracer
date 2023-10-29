
#include "Scene.h"
#include "Renderable.h"
#include "HitProperties.h"
#include "Light.h"

Scene::Scene() : Intersectable(), backgroundColor( 0.0, 0.0, 0.0 ), globalAmbient( 0.0, 0.0, 0.0 ) {
	lightPtrs.reserve( 10 );
	objects.reserve( 30 );
	shadowCache = 0;
	activeCamera = 0;

	globalIlluminationLight = 0;
}

Scene::~Scene() {
	for( std::vector<Renderable*>::iterator it=objects.begin(); it!=objects.end(); ++it )
		delete *it;

	objects.clear();
	lightPtrs.clear();
	cameras.clear();

	delete globalIlluminationLight;
}

void Scene::add( Renderable* r ) {
	if( r->isLight() )
		lightPtrs.push_back( dynamic_cast<Light*>( r ) );
	objects.push_back( r );
}

void Scene::add( Camera const& cam ) {
	cameras.push_back( cam );
}

void Scene::setBackgroundColor( Vector const& c ) {
	backgroundColor = c;
}

Vector const& Scene::getBackgroundColor() const {
	return backgroundColor;
}

void Scene::setGlobalAmbient( Vector const& col ) {
	globalAmbient = col;
}

Vector const& Scene::getGlobalAmbient() const {
	return globalAmbient;
}

void Scene::setGlobbalIlluminationLight( GlobalIlluminationLight* gil ) {
	globalIlluminationLight = gil;
}

GlobalIlluminationLight* Scene::getGlobalIlluminationLight() const {
	return globalIlluminationLight;
}

void Scene::setActiveCamera( int num ) {
	if( ( num >= 0 ) && ( num < static_cast<int>( cameras.size() ) ) )
		activeCamera = num;
	else {
		assert( 0 );
		activeCamera = 0;
	}
}

Camera const& Scene::getActiveCamera() const {
	return cameras[ activeCamera ];
}

std::vector<Light*> const& Scene::getLightPtrs() const {
	return lightPtrs;
}

void Scene::transform() {
	for( std::vector<Renderable*>::const_iterator it=objects.begin(); it!=objects.end(); ++it )
		( *it )->transform();
}

bool Scene::intersect( Ray const& r, HitProperties* hp ) const {

	bool firstHit = false;

	// intersect with every renderable object and find nearest
	for( std::vector<Renderable*>::const_iterator it=objects.begin(); it!=objects.end(); ++it ) {

		HitProperties acthp;

		if( ( *it )->intersect( r, &acthp ) ) {

			if( !firstHit || ( acthp.distance < hp->distance ) ) {
				*hp			= acthp;
				firstHit	= true;
			}
		}
	}

	return firstHit;
}

bool Scene::intersect( Ray const& r, double dist ) const {

	// quick intersection test for shadows
	// check last shadowing object first..

	if( shadowCache )
		if( shadowCache->intersect( r, dist ) )
			return true;

	// do only intersect with actual primitives (no lights!), stop at first hit within dist
	for( std::vector<Renderable*>::const_iterator it=objects.begin(); it!=objects.end(); ++it )
		if( !( *it )->isLight() && ( shadowCache != *it ) && ( *it )->intersect( r, dist ) ) {
			shadowCache = *it;
			return true;
		}

	return false;
}
