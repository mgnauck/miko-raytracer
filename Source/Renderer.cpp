
#include "Renderer.h"
#include "View.h"
#include "defines.h"
#include "BitMap.h"

MapManager* Renderer::mapManager = new MapManager();

Renderer::Renderer() : raytracer( 0 ), view( 0 ) {
}

Renderer::~Renderer() {
	delete raytracer;
	delete view;
	if( mapManager ) {
		delete mapManager;
		mapManager = 0;
	}
}

MapManager* Renderer::getMapManager() {
	return mapManager;
}

void Renderer::init( Raytracer* r, View* v ) {
	raytracer	= r;
	view		= v;
}

BitMap* Renderer::render() const {

	assert( view );
	assert( raytracer );

	BitMap*		pic		= new BitMap( view->width, view->height );

	double		dx		= view->getAspect() / static_cast<double>( view->width );
	double		dy		= 1.0 / static_cast<double>( view->height );

	double	startX		= -view->getAspect() / 2.0 + 0.5 * dx;
	double	startY		= -0.5 + 0.5 * dy;

#ifndef __NO_PROGRESS__
	double	deltaProg	= 100.0 / view->height;
	double	progress	= 0.0;
#endif

	double y = startY;
	for( int j=0; j<view->height; j++ ) {

		double x = startX;
		for( int i=0; i<view->width; i++ ) {

			Ray						ray;
	
			ray.makePrimaryRay( x, y, raytracer->getScene()->getActiveCamera() );

			Vector color( raytracer->traceRay( ray ) );
			pic->set( i, j, color.toRGB32() );

			raytracer->resetRecursion();
			x += dx;
		}

#ifndef __NO_PROGRESS__
		std::cout << "progress: " << progress << "% \r";
		progress += deltaProg;
#endif
		y += dy;
	}

	return pic;
}
