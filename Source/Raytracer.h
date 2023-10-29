
#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__

#include "Ray.h"
#include "RecursionProperties.h"
#include "Scene.h"

class Raytracer {
	public:
		Raytracer( Scene* s, RecursionProperties const& mr );
		virtual ~Raytracer();

		RecursionProperties const&	getActRecursion() const;
		Scene const*				getScene() const;

		void						resetRecursion();
		void						increaseRecursion();
		void						decreaseRecursion();
		bool						recursionValid() const;

		void						enableGlobalIllumination();

		Vector						traceRay( Ray const& r );

	private:
		Raytracer( Raytracer const& rhs );					// no duplicates!
		Raytracer& operator=( Raytracer const& rhs );

		Scene*						scene;	
		RecursionProperties			maxRecursion;
		RecursionProperties			actRecursion;
};

#endif
