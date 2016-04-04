
#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <string>

struct Vector;

class Material {
public:
	Material() {

		opacity			= 1.0;
		gloss			= 0.0;
		reflection		= 0.0;
		refraction		= 0.0;
		refractiveIndex	= 1.0;
		
		textureMapID	= "";
		normalMapID		= "";
		hasTexture		= false;
		hasNormal		= false;
	}

	virtual ~Material() {}

	void setTextureMapID( std::string const& name ) {
		hasTexture		= true;
		textureMapID	= name;
	}

	void setNormalMapID( std::string const& name ) {
		hasNormal		= true;
		normalMapID		= name;
	}

	std::string const& getTextureMapID() const {
		return textureMapID;
	}

	std::string const& getNormalMapID() const {
		return normalMapID;
	}

	bool hasTextureMap() const {
		return hasTexture;
	}

	bool hasNormalMap() const {
		return hasNormal;
	}

	double			opacity;
	double			gloss;
	double			reflection;
	double			refraction;
	double			refractiveIndex;

private:
	std::string		textureMapID;
	std::string		normalMapID;

	bool			hasTexture;
	bool			hasNormal;
};

#endif
