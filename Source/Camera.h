
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Transformable.h"

class Camera : public Transformable {
public:
	Camera();
	Camera( Matrix const& mat );

	void	set( Vector const& eye, Vector const& tgt, Vector const& up );

	Vector	getEye() const;
	
	void	setDistToViewplane( double d );
	double	getDistToViewplane() const;

private:
	double	distToViewplane;
};

#endif