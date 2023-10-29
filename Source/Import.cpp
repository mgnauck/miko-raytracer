
#include "Import.h"
#include <iostream>
#include "Renderer.h"
#include "ImageIO.h"
#include "OmniLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "SkyLight.h"
#include "PhongMaterial.h"
#include "PhongShader.h"
#include "LambertShader.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Mesh.h"
#include "BoundingSphere.h"

using namespace std;

Import::Import( std::string const& filename, Scene* s, Renderer* r ) : doc( filename ), scene( s ), renderer( r ) {

	nodes.reserve( 40 );

	// scene properties
	backgroundColor = Vector( 0.0, 0.0, 0.0 );
	globalAmbient	= Vector( 0.1, 0.1, 0.1 );

	// load from file
	if ( !doc.LoadFile() ) {
		cout << "could not load " << filename << "!!" << endl;
		cout << "terminating.." << endl;
		exit( 0 );
	}

	// get root element
	TiXmlNode*	rootNode = doc.RootElement();
	
	if( !rootNode ) {
		cout << "error parsing file!" << endl;
		cout << "terminating.." << endl;
		exit( 0 );
	}

	// assign nodes
	sceneNode		= rootNode->FirstChild( "scene" );
	scenegraphNode	= rootNode->FirstChild( "scenegraph" );
	shapeSetNode	= rootNode->FirstChild( "shapeset" );
	shaderSetNode	= rootNode->FirstChild( "shaderset" );

	assert( sceneNode );
	assert( scenegraphNode );
	assert( shapeSetNode );
	assert( shaderSetNode );

	// load scene data
	loadScene();
	// load transform nodes
	loadScenegraph();
	// load shapes
	loadShapeset();

	// add temp skylight
	//scene->add( new SkyLight( Vector( 1.0, 1.0, 1.0 ) ) );

	// finalize scene
	scene->transform();
}

Import::~Import() {
	nodes.clear();
}

vector<int> chop2Int( string const& s ) {

	// should be easier/cleaner with some stl functions
	vector<int>	values;
	string		tmp = "";
	int			i	= 0;

	while( i < static_cast<int>( s.size() ) ) {
		if( s[ i ] != '|' )
			tmp.push_back( s[ i ] );
		else {
			values.push_back( atoi( tmp.c_str() ) );
			tmp.clear();
		}
		i++;
	}

	values.push_back( atoi( tmp.c_str() ) );
	tmp.clear();

	return values;
}

vector<double> chop2Double( string const& s ) {

	// should be easier/cleaner with some stl functions
	vector<double>	values;
	string			tmp = "";
	int				i	= 0;

	while( i < static_cast<int>( s.size() ) ) {
		if( s[ i ] != '|' )
			tmp.push_back( s[ i ] );
		else {
			values.push_back( atof( tmp.c_str() ) );
			tmp.clear();
		}
		i++;
	}

	values.push_back( atof( tmp.c_str() ) );
	tmp.clear();

	return values;
}

Vector chopVector( string const& s ) {
	
	vector<double>	tmp( chop2Double( s ) );
	int				cnt	= static_cast<int>( tmp.size() );
	Vector			res( 0.0, 0.0, 0.0 );

	cnt = ( cnt > 3 ) ? 3 : cnt;

	for( int i=0; i<cnt; i++ )
		res[ i ] = tmp[ i ];

	tmp.clear();
	return res;
}

Index chopIndex( string const& s ) {
	
	vector<int>	tmp( chop2Int( s ) );
	int			cnt	= static_cast<int>( tmp.size() );
	Index		res( 0, 0, 0 );

	assert( cnt == 3 );

	res.a = tmp[ 0 ];
	res.b = tmp[ 1 ];
	res.c = tmp[ 2 ];

	tmp.clear();
	return res;
}

Matrix compose( Vector const trans, Vector const& rot, Vector const& scale, Vector const& scaleRot ) {

	// translation
	Matrix t;
	t.makeTranslation( trans );

	// rotation
	Matrix r;
	r.makeRotationZ( rot.z * DEG2RAD );
	r *= Matrix().makeRotationY( rot.y * DEG2RAD );
	r *= Matrix().makeRotationX( rot.x * DEG2RAD );

	// scale
	Matrix s;
	s.makeScale( scale );

	// scale rotation (rotation before scale)
	Matrix sr;
	sr.makeRotationZ( scaleRot.z * DEG2RAD );
	sr *= Matrix().makeRotationY( scaleRot.y * DEG2RAD );
	sr *= Matrix().makeRotationX( scaleRot.x * DEG2RAD );

	// translate <- rotate <- eliminate scale rotation <- scale <- scale rotate
	t *= r;
	t *= sr.inverted();
	t *= s;
	t *= sr;

	return t;
}

void Import::loadScene() {

	for( TiXmlNode* child=sceneNode->FirstChild(); child; child=child->NextSiblingElement() ) {
		
		string name = child->ToElement()->Attribute( "name" );

		if( name == "backgroundcolor" )
			backgroundColor = chopVector( child->FirstChild()->Value() );

		if( name == "ambientlightcolor" )
			globalAmbient = chopVector( child->FirstChild()->Value() );
    }

	// set background col + global ambient light color
	scene->setBackgroundColor( backgroundColor );
	scene->setGlobalAmbient( globalAmbient );
}

Matrix Import::loadXformNode( TiXmlNode* n, string* shapeID, Matrix const& parent ) {

	assert( n );

	// affine parts
	Vector	trans( 0.0, 0.0, 0.0 );
	Vector	rot( 0.0, 0.0, 0.0 );
	Vector	scale( 1.0, 1.0, 1.0 );
	Vector	scaleRot( 0.0, 0.0, 0.0 );

	// attribs eines objekts
	for( TiXmlNode* child=n->FirstChild(); child; child=child->NextSibling( "attr" ) ) {

		string name	= child->ToElement()->Attribute( "name" );

		if( name == "rotation" )
			rot = chopVector( child->FirstChild()->Value() );

		if( name == "scale_rotation" )
			scaleRot = chopVector( child->FirstChild()->Value() );

		if( name == "scale" )
			scale = chopVector( child->FirstChild()->Value() );

		if( name == "translation" )
			trans = chopVector( child->FirstChild()->Value() );

		if( name == "shape" )
			*shapeID = string( child->FirstChild()->Value() );
	}

	// compose matrix from affine parts & mul matrix with parent node matrix
	Matrix xform( compose( trans, rot, scale, scaleRot ).transposed() * parent );

	// recurse in deeper node
	TiXmlNode* node = n->FirstChild()->NextSibling( "node" );
	if( node )
		xform = loadXformNode( node, shapeID, xform );

	return xform;
}

void Import::loadScenegraph() {

	// load all transform nodes
	for( TiXmlNode* child=scenegraphNode->FirstChild(); child; child=child->NextSiblingElement() ) {

		assert( child );

		string type = child->ToElement()->Attribute( "type" );
		if( type == "transform" ) {

			TransNode node;
	
			// calculate matrix -> down the hierarchy
			node.mat = loadXformNode( child, &node.id, Matrix().makeIdentity() ).transposed();
			nodes.push_back( node );
		}
	}
}

void Import::loadOmnilight( TiXmlNode* n, TransNode const& tn ) {

	assert( n );

	Vector	color( 1.0, 1.0, 1.0 );
	double	intensity = 1.0;
	bool	shadowing = true;

	for( TiXmlNode* child=n->FirstChild(); child; child=child->NextSiblingElement() ) {

		string name	= child->ToElement()->Attribute( "name" );

		if( name == "color" )
			color = chopVector( child->FirstChild()->Value() );

		if( name == "intensity" )
			intensity = atof( child->FirstChild()->Value() );

		if( name == "shadow_enabled" )
			if( atoi( child->FirstChild()->Value() ) == 0 )
				shadowing = false;
	}

	// add the light
	OmniLight* omni = new OmniLight( tn.mat, color * intensity );
	omni->setShadowing( shadowing );
	scene->add( omni );
}

void Import::loadDirectionallight( TiXmlNode* n, TransNode const& tn ) {

	assert( n );

	Vector	color( 1.0, 1.0, 1.0 );
	double	intensity = 1.0;
	bool	shadowing = true;

	for( TiXmlNode* child=n->FirstChild(); child; child=child->NextSiblingElement() ) {

		string name	= child->ToElement()->Attribute( "name" );

		if( name == "color" )
			color = chopVector( child->FirstChild()->Value() );

		if( name == "intensity" )
			intensity = atof( child->FirstChild()->Value() );

		if( name == "shadow_enabled" )
			if( atoi( child->FirstChild()->Value() ) == 0 )
				shadowing = false;
	}

	// create and rotate forward (= light dir) vector with nodes matrix
	Vector forward( 0.0, 0.0, 1.0, 0.0 );
	forward = tn.mat * forward;

	// set forward + translation
	Matrix mat;
	mat.makeIdentity();
	mat.setRow( 2, forward );
	mat.setTranslation( tn.mat.getTranslation() );

	// add the directional light
	DirectionalLight* dir = new DirectionalLight( mat, color * intensity );
	dir->setShadowing( shadowing );
	scene->add( dir );
}

void Import::loadSpotlight( TiXmlNode* n, TransNode const& tn ) {

	assert( n );

	Vector	color( 1.0, 1.0, 1.0 );
	double	intensity	= 1.0;
	double	inner		= 10.0;
	double	outer		= 40.0;
	bool	shadowing	= true;

	for( TiXmlNode* child=n->FirstChild(); child; child=child->NextSiblingElement() ) {

		string name	= child->ToElement()->Attribute( "name" );

		if( name == "color" )
			color = chopVector( child->FirstChild()->Value() );

		if( name == "intensity" )
			intensity = atof( child->FirstChild()->Value() );

		if( name == "innerangle" )
			inner = atof( child->FirstChild()->Value() );

		if( name == "outerangle" )
			outer = atof( child->FirstChild()->Value() );

		if( name == "shadow_enabled" )
			if( atoi( child->FirstChild()->Value() ) == 0 )
				shadowing = false;
	}

	// create and rotate forward (= light dir) vector with nodes matrix
	Vector forward( 0.0, 0.0, 1.0, 0.0 );
	forward = tn.mat * forward;

	// set forward + translation
	Matrix mat;
	mat.makeIdentity();
	mat.setRow( 2, forward );
	mat.setTranslation( tn.mat.getTranslation() );

	// add the light
	SpotLight* spot = new SpotLight( mat, color * intensity, outer, inner );
	spot->setShadowing( shadowing );
	scene->add( spot );
}

void Import::loadCamera( TiXmlNode* n, TransNode const& tn ) {

	assert( n );

	double aspect	= 4.0 / 3.0;
	double fov		= 60.0;

	for( TiXmlNode* child=n->FirstChild(); child; child=child->NextSiblingElement() ) {

		string name	= child->ToElement()->Attribute( "name" );

		if( name == "fov" )
			fov = atof( child->FirstChild()->Value() );

		if( name == "aspectratio" )
			aspect = atof( child->FirstChild()->Value() );
	}

	// initial values
	Vector eye( 0.0, 0.0, 0.0, 1.0 );
	Vector tgt( 0.0, 0.0, 1.0, 1.0 );
	Vector up( 0.0, 1.0, 0.0, 0.0 );

	// transform with node transformation
	eye = tn.mat * eye;
	tgt = tn.mat * tgt;
	up	= tn.mat * up;

	// calc distance to viewplane from fov
	double dist = 1.0 / tan( fov );

	// setup camera
	Camera cam;
	cam.set( eye, tgt, up );
	cam.setDistToViewplane( dist );

	// add camera
	scene->add( cam );
	scene->setActiveCamera( 0 );
}

void Import::loadSphere( TiXmlNode* n, TransNode const& tn ) {

	assert( n );

	double	radius		= 0.0;
	string	shaderName	= "";

	for( TiXmlNode* child=n->FirstChild(); child; child=child->NextSiblingElement() ) {

		string name	= child->ToElement()->Attribute( "name" );

		if( name == "radius" )
			radius = atof( child->FirstChild()->Value() );

		if( name == "shader" )
			shaderName = child->FirstChild()->Value();
	}

	// create new sphere
	Sphere*		sphere		= new Sphere( tn.mat, radius );
	Material*	material	= 0;
	Shader*		shader		= 0;

	// find and load respective shader
	loadShader( findShader( shaderName ), &material, &shader );

	// add stuff to sphere
	sphere->setMaterial( material );
	sphere->setShader( shader );
	sphere->setTiling( Vector( 1.0, 1.0, 0.0 ) );
	
	// add sphere to scene
	scene->add( sphere );
}

void Import::loadPlane( TiXmlNode* n, TransNode const& tn ) {

	assert( n );

	double	length		= 0.0;
	double	width		= 0.0;
	string	shaderName	= "";

	for( TiXmlNode* child=n->FirstChild(); child; child=child->NextSiblingElement() ) {

		string name	= child->ToElement()->Attribute( "name" );

		if( name == "length" )
			length = atof( child->FirstChild()->Value() );

		if( name == "width" )
			width = atof( child->FirstChild()->Value() );

		if( name == "shader" )
			shaderName = child->FirstChild()->Value();
	}

	// create new plane
	Plane*		plane		= new Plane( tn.mat, length, width );
	Material*	material	= 0;
	Shader*		shader		= 0;

	// find and load respective shader
	loadShader( findShader( shaderName ), &material, &shader );

	// add stuff to plane
	plane->setMaterial( material );
	plane->setShader( shader );
	plane->setTiling( Vector( 5.0, 5.0, 0.0 ) );
	
	// add plane to scene
	scene->add( plane );
}

void Import::loadBox( TiXmlNode* n, TransNode const& tn ) {

	assert( n );

	double	length		= 0.0;
	double	width		= 0.0;
	double	height		= 0.0;
	string	shaderName	= "";

	for( TiXmlNode* child=n->FirstChild(); child; child=child->NextSiblingElement() ) {

		string name	= child->ToElement()->Attribute( "name" );

		if( name == "length" )
			length = atof( child->FirstChild()->Value() );

		if( name == "width" )
			width = atof( child->FirstChild()->Value() );

		if( name == "height" )
			height = atof( child->FirstChild()->Value() );

		if( name == "shader" )
			shaderName = child->FirstChild()->Value();
	}

	// extends around 0/0/0
	double w = width * 0.5;
	double l = length * 0.5;
	double h = height * 0.5;

	// create new box
	Box*		box			= new Box( tn.mat, Vector( -w, -h, -l ), Vector( w, h, l ) );
	Material*	material	= 0;
	Shader*		shader		= 0;

	// find and load respective shader
	loadShader( findShader( shaderName ), &material, &shader );

	// add stuff to box
	box->setMaterial( material );
	box->setShader( shader );
	box->setTiling( Vector( 1.0, 1.0, 0.0 ) );
	
	// add box to scene
	scene->add( box );
}

void Import::loadMesh( TiXmlNode* n, TransNode const& tn ) {

	assert( n );

	vector<Vector>		normals;
	vector<Vector>		vertices;
	vector<Vector>		texels;
	vector<Index>		indices;
	vector<string>		shaderNames;
	vector<string>		shaderUses;

	for( TiXmlNode* child=n->FirstChild(); child; child=child->NextSiblingElement() ) {

		string name	= child->ToElement()->Attribute( "name" );

		if( name == "faceformat" ) {

			string facefmt( child->FirstChild()->Value() );

			// do we have correct faceformat?
			if( facefmt != "v|n|t0" ) {
				cout << "can only read v|n|t0 faceformat files! sorry.." << endl;
				cout << "terminating" << endl;
				exit( 0 );
			}
		}

		if( name == "faceset" )
			if( !child->FirstChild()->FirstChild() ) {
				// only one element
				indices.push_back( chopIndex( child->FirstChild()->Value() ) );
			} else {
				for( TiXmlNode* fc=child->FirstChild(); fc; fc=fc->NextSiblingElement() )
					indices.push_back( chopIndex( fc->FirstChild()->Value() ) ); 
			}

		if( name == "normalset" )
			if( !child->FirstChild()->FirstChild() ) {
				// only one element
				normals.push_back( chopVector( child->FirstChild()->Value() ) );
			} else {
				for( TiXmlNode* fc=child->FirstChild(); fc; fc=fc->NextSiblingElement() )
					normals.push_back( chopVector( fc->FirstChild()->Value() ) );
			}

		if( name == "texcoordset" )
			if( !child->FirstChild()->FirstChild() ) {
				// only one element
				texels.push_back( chopVector( child->FirstChild()->Value() ) );
			} else {
				for( TiXmlNode* fc=child->FirstChild(); fc; fc=fc->NextSiblingElement() )
					texels.push_back( chopVector( fc->FirstChild()->Value() ) );
			}

		if( name == "vertexset" )
			if( !child->FirstChild()->FirstChild() ) {
				// only one element
				vertices.push_back( chopVector( child->FirstChild()->Value() ) );
			} else {
				for( TiXmlNode* fc=child->FirstChild(); fc; fc=fc->NextSiblingElement() )
					vertices.push_back( chopVector( fc->FirstChild()->Value() ) );
			}

		if( name == "shaderset" ) {
			for( TiXmlNode* fc=child->FirstChild(); fc; fc=fc->NextSiblingElement() ) {
				shaderNames.push_back( fc->FirstChild()->Value() );
				fc=fc->NextSiblingElement();
				shaderUses.push_back( fc->FirstChild()->Value() );
			}
		}
	}

	// for every material add seperate mesh (= submesh)
	for( int j=0; j<static_cast<int>( shaderNames.size() ); j++ ) {

		// create mesh
		Mesh*		mesh = new Mesh( tn.mat );
		vector<int>	usage( chop2Int( shaderUses[ j ] ) );

		Material*	material	= 0;
		Shader*		shader		= 0;

		// find and load respective material/shader for submesh
		loadShader( findShader( shaderNames[ j ] ), &material, &shader );

		// fill mesh with data
		for( int i=0; i<static_cast<int>( usage.size() ); i++ ) {

			// index into faceset array (v|n|t0)
			int index = i * 3;

			Vertex v0( vertices[ indices[ index ].a ], normals[ indices[ index + 1 ].a ], texels[ indices[ index + 2 ].a ] );
			Vertex v1( vertices[ indices[ index ].b ], normals[ indices[ index + 1 ].b ], texels[ indices[ index + 2 ].b ] );
			Vertex v2( vertices[ indices[ index ].c ], normals[ indices[ index + 1 ].c ], texels[ indices[ index + 2 ].c ] );

			// add 3 vertices
			mesh->add( v0 );
			mesh->add( v1 );
			mesh->add( v2 );

			// add triangle structure
			mesh->add( Triangle( index, index + 1, index + 2 ) );
		}

		usage.clear();

		// set material / shader / tiling
		mesh->setMaterial( material );
		mesh->setShader( shader );
		mesh->setTiling( Vector( 1.0, 1.0, 0.0 ) );

		// create bounding volume
		mesh->setBoundingVolume( new BoundingSphere() );

		// add mesh
		scene->add( mesh );
	}
}

void Import::loadPhong( TiXmlNode* n, PhongMaterial** mat ) {

	double specularLevel = 1.0;

	for( TiXmlNode* child=n->FirstChild(); child; child=child->NextSiblingElement() ) {

		string name = child->ToElement()->Attribute( "name" );

		if( name == "alpha" )
			( *mat )->opacity = atof( child->FirstChild()->Value() );

		if( name == "ambient_color" )
			( *mat )->ambient = chopVector( child->FirstChild()->Value() );

		if( name == "diffuse_color" )
			( *mat )->diffuse = chopVector( child->FirstChild()->Value() );

		if( name == "specular_color" )
			( *mat )->specular = chopVector( child->FirstChild()->Value() );

		if( name == "specular_exponent" )
			( *mat )->exponent = atof( child->FirstChild()->Value() );

		if( name == "specular_level" )
			specularLevel = atof( child->FirstChild()->Value() );

		if( name == "emissive_color" )
			( *mat )->emission = chopVector( child->FirstChild()->Value() );

		if( name == "reflection_amount" )
			( *mat )->reflection = atof( child->FirstChild()->Value() );

		if( name == "refraction_amount" )
			( *mat )->refraction = atof( child->FirstChild()->Value() );

		if( name == "index_of_refraction" )
			( *mat )->refractiveIndex = atof( child->FirstChild()->Value() );

		if( name == "diffuse_map" ) {
			string shaderName = child->FirstChild()->Value();
			loadShader( findShader( shaderName ), 0, 0 );
			( *mat )->setTextureMapID( shaderName );
		}

		if( name == "bump_map" ) {
			string shaderName = child->FirstChild()->Value();
			loadShader( findShader( shaderName ), 0, 0 );
			( *mat )->setNormalMapID( shaderName );
		}
	}

	// specular scaled by specular level
	( *mat )->specular *= specularLevel;

	// adjust exponent a bit to match 3ds max lighting
	( *mat )->exponent *= 0.25;

	// !! (max stylee)
	if( ( *mat )->hasTextureMap() )
		( *mat )->diffuse = Vector( 1.0, 1.0, 1.0 );
}

void Import::loadShader( TiXmlNode* n, Material** mat, Shader** sh ) {

	string id	= n->ToElement()->Attribute( "id" );
	string type = n->ToElement()->Attribute( "type" );

	if( type == "phong" ) {

		*mat	= new PhongMaterial();
		*sh		= new PhongShader();
	
		PhongMaterial* phong = dynamic_cast<PhongMaterial*>( *mat );
		loadPhong( n, &phong );
	}

	if( type == "bitmap" ) {

		// check if we have "url name"-type
		string name = n->FirstChild()->ToElement()->Attribute( "name" );

		if( name == "url" ) {
			// get name of map
			string mapName( n->FirstChild()->FirstChild()->Value() );

			// already loaded? (shader id is map identifier)
			if( !renderer->getMapManager()->exists( id ) )
				renderer->getMapManager()->addMap( id, ImageIO::load( mapName ) );
		}
	}

	//if( type == "checker" )
	//if( type == "noise" ) {
	//if( type == "marble" ) {
	//if( type == "wood" ) {
}

void Import::loadShapeset() {

	for( vector<TransNode>::iterator it=nodes.begin(); it!=nodes.end(); ++it ) {

		// xml3d bug? there are nodes without shapeID (can't load them!)
		if( it->id == "" )
			continue;

		TiXmlNode*	child	= findShape( it->id );
		string		type	= child->ToElement()->Attribute( "type" );
		string		id		= child->ToElement()->Attribute( "id" );

		// load respective object
		if( type == "omnilight" )
			loadOmnilight( child, *it );
		if( type == "dirlight" )
			loadDirectionallight( child, *it );
		if( type == "spotlight" )
			loadSpotlight( child, *it );
		if( type == "camera" )
			loadCamera( child, *it );
		if( type == "sphere" )
			loadSphere( child, *it );
		if( type == "plane" )
			loadPlane( child, *it );
		if( type == "box" )
			loadBox( child, *it );
		if( type == "poly" )
			loadMesh( child, *it );
	}
}

TiXmlNode* Import::findShape( string const& id ) {
	for( TiXmlNode* child=shapeSetNode->FirstChild(); child; child=child->NextSiblingElement() ) {
		string s = child->ToElement()->Attribute( "id" );
		if( s == id )
			return child;
	}
	return 0;
}

TiXmlNode* Import::findShader( string const& id ) {
	for( TiXmlNode* child=shaderSetNode->FirstChild(); child; child=child->NextSiblingElement() ) {
		string s = child->ToElement()->Attribute( "id" );
		if( s == id )
			return child;
	}
	return 0;
}