
#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>
#include "Intersectable.h"
#include "Camera.h"

class Light;
class Renderable;
class GlobalIlluminationLight;

class Scene : public Intersectable {
public:
	Scene();
	~Scene();

	void							add( Renderable* r );
	void							add( Camera const& cam );

	void							setBackgroundColor( Vector const& c );
	Vector const&					getBackgroundColor() const;

	void							setGlobbalIlluminationLight( GlobalIlluminationLight* gil );
	GlobalIlluminationLight*		getGlobalIlluminationLight() const;

	void							setGlobalAmbient( Vector const& col );
	Vector const&					getGlobalAmbient() const;

	void							setActiveCamera( int num );
	Camera const&					getActiveCamera() const;
	std::vector<Light*> const&		getLightPtrs() const;

	void							transform();

	bool							intersect( Ray const& r, HitProperties* hp ) const;
	bool							intersect( Ray const& r, double dist ) const;

private:
	Scene( Scene const& rhs );
	Scene& operator=( Scene const& rhs );

	Vector							globalAmbient;
	Vector							backgroundColor;

	std::vector<Light*>				lightPtrs;
	std::vector<Renderable*>		objects;
	std::vector<Camera>				cameras;

	GlobalIlluminationLight*		globalIlluminationLight;

	int								activeCamera;
	mutable Renderable const*		shadowCache;
};

#endif
