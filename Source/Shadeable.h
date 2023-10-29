
#ifndef _SHADEABLE_H__
#define _SHADEABLE_H__

#include "Material.h"
#include "Shader.h"
#include "Vector.h"

class Shadeable {
public:
	Shadeable() : material( 0 ), shader( 0 ), tiling( 1.0, 1.0, 0.0 ) {}
	Shadeable( Shadeable const& rhs ) : material( rhs.getMaterial() ), shader( rhs.getShader() ), tiling( rhs.getTiling() ) {}

	virtual ~Shadeable() {
		delete material;
		delete shader;	
	}

	virtual void completeHitProperties( HitProperties* hp ) const = 0;

	void setMaterial( Material* m ) {
		material = m;
	}

	Material* getMaterial() const {
		return material;
	}

	void setShader( Shader* s ) {
		shader = s;
	}
	
	Shader* getShader() const {
		return shader;
	}

	void setTiling( Vector const& t ) {
		tiling = t;
	}

	Vector const& getTiling() const {
		return tiling;
	}

	Shadeable& operator=( Shadeable const& rhs ) {
		if( &rhs == this )
			return *this;
		material = rhs.getMaterial();
		shader = rhs.getShader();
		tiling = rhs.getTiling();
		return *this;
	}
	
protected:
	Material*	material;
	Shader*		shader;
	Vector		tiling;			// u,v = x,y
};

#endif
