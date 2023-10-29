
#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "Raytracer.h"
#include "MapManager.h"
#include "View.h"

class BitMap;

class Renderer {
	public:
		Renderer();
		virtual ~Renderer();

		static MapManager*	getMapManager();

		void				init( Raytracer* r, View* v );
		BitMap*				render() const;

	protected:
		Renderer( Renderer const& rhs );				// no duplicates!
		Renderer& operator=( Renderer const& rhs );

		static MapManager*	mapManager;
		Raytracer*			raytracer;
		View*				view;
};

#endif