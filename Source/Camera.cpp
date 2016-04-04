
#include "Camera.h"

Camera::Camera() : Transformable() {
	distToViewplane = 1.0;
}

Camera::Camera( Matrix const& mat ) : Transformable( mat ) {
	distToViewplane = 1.0;
}

void Camera::set( Vector const& eye, Vector const& tgt, Vector const& up ) {
	
	Vector forward( tgt - eye );
	forward.normalize();

	Vector right = forward.cross( up );
	right.normalize();

	Vector realUp = forward.cross( right );
	realUp.normalize();

	Matrix rot;
	rot.makeIdentity();

	rot.setRow( 0, right );
//	rot.setRow( 1, up.normalized() );
	rot.setRow( 1, realUp );
	rot.setRow( 2, forward );

	Matrix trans;
	trans.makeTranslation( eye );

	Transformable::setMatrix( trans * rot );
}

Vector Camera::getEye() const {
	return getMatrix().getTranslation();
}

void Camera::setDistToViewplane( double d ) {
	distToViewplane = d;
}

double Camera::getDistToViewplane() const {
	return distToViewplane;
}