
#ifndef __IMPORT_H__
#define __IMPORT_H__

#include <string>
#include <map>
#include "Scene.h"
#include "../tinyxml/tinyxml.h"

class Renderer;
class Material;
class PhongMaterial;
class Shader;

struct TransNode {
	std::string	id;
	Matrix		mat;
};

struct Index {
	Index( int x, int y, int z ) : a( x ), b( y ), c( z ) {}
	int			a, b, c;
};

class Import {
public:
	Import( std::string const& filename, Scene* s, Renderer* r );
	~Import();

private:
	void				loadScene();

	Matrix				loadXformNode( TiXmlNode* n, std::string* shapeID, Matrix const& parent );
	void				loadScenegraph();

	void				loadOmnilight( TiXmlNode* n, TransNode const& tn );
	void				loadDirectionallight( TiXmlNode* n, TransNode const& tn );
	void				loadSpotlight( TiXmlNode* n, TransNode const& tn );
	void				loadCamera( TiXmlNode* n, TransNode const& tn );
	void				loadSphere( TiXmlNode* n, TransNode const& tn );
	void				loadPlane( TiXmlNode* n, TransNode const& tn );
	void				loadBox( TiXmlNode* n, TransNode const& tn );
	void				loadMesh( TiXmlNode* n, TransNode const& tn );
	void				loadShapeset();
	
	void				loadPhong( TiXmlNode* n, PhongMaterial** mat );
	void				loadShader( TiXmlNode* n, Material** mat, Shader** sh );

	TiXmlNode*			findShader( std::string const& id );
	TiXmlNode*			findShape( std::string const& id );

	Import( Import const& );
	Import& operator=( Import const& );


	Vector					backgroundColor;
	Vector					globalAmbient;

	Scene*					scene;
	Renderer*				renderer;

	TiXmlDocument			doc;
	std::vector<TransNode>	nodes;

	TiXmlNode*				sceneNode;
	TiXmlNode*				scenegraphNode;
	TiXmlNode*				shapeSetNode;
	TiXmlNode*				shaderSetNode;
};

#endif