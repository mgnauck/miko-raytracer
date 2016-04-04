
/////////////////////////////////////////////////////////////////////////////////////////// trash

void savePicAsHeightMap( std::string const& infile, std::string const& outfile ) {

	ImageIO::init();

	BitMap*	in	= ImageIO::load( infile );
	BitMap* out	= new BitMap( in->getWidth(), in->getHeight() );

	for( int j=0; j<in->getHeight(); j++ ) {
		for( int i=0; i<in->getWidth(); i++ ) {
			unsigned int col = in->get( i, j );
			int r = ( col >> 16 ) & 0xff;
			int g = ( col >> 8 ) & 0xff;
			int b = col & 0xff;
			int v = (int)( (double)r * 0.2125 + (double)g * 0.7154 + (double)b * 0.0721 ) & 0xff;
			out->set( i, j, ( 0xff << 24 ) | ( v << 16 ) | ( v << 8 ) | v );
		}
	}

	ImageIO::save( outfile, *out );

	delete out;
	delete in;
}

//////////////////////// main scene generation

Scene*		scene = initScene();
Renderer	renderer;
loadTextures( &renderer );


//////////////////////// map load + scene generation

#include "numbers.h"
#include "View.h"
#include "Plane.h"
#include "Sphere.h"
#include "Box.h"
#include "LambertMaterial.h"
#include "LambertShader.h"
#include "LambertNormalShader.h"
#include "PhongMaterial.h"
#include "PhongShader.h"
#include "PhongNormalShader.h"
#include "BlinnShader.h"
#include "OmniLight.h"
#include "DirectionalLight.h"
#include "SkyLight.h"
#include "SpotLight.h"
#include "AreaLight.h"
#include "CheckerMap.h"
#include "NoiseMap.h"
#include "MarbleMap.h"
#include "WoodMap.h"

void loadTextures( Renderer* r ) {

	r->getMapManager()->addTextureMap( new MarbleMap() );
	r->getMapManager()->addTextureMap( new CheckerMap( 0.3, 0.3, Vector( 0.6, 0.6, 0.6 ), Vector( 0.8, 0.8, 0.8 ) ) );
	r->getMapManager()->addTextureMap( new WoodMap() );
	r->getMapManager()->addBumpMap( ImageIO::load( "data/height1.bmp" ) );
}

Scene* initScene() {

	Scene* scene = new Scene();

	// camera gedrösel
	Camera cam;
	cam.set( Vector( 0.0, 10.0, 17.0 ), Vector( 0.0, 4.0, 0.0 ), Vector( 0.0, 1.0, 0.0 ) );
	cam.setDistToViewplane( 1.0 );
	scene->add( cam );
	scene->setActiveCamera( 0 );

	Matrix matrix;
	matrix.makeIdentity();

	// sphere
/*	matrix.setCol( 3, Vector( 0.0, 4.0, 0.0 ) );
	Sphere* sphere = new Sphere( matrix, 4.0 );
	sphere->setMaterial( new PhongMaterial( Vector( 0.05, 0.05, 0.05 ), Vector( 0.8, 0.8, 0.8 ), Vector( 1.0, 1.0, 1.0 ), Vector( 0.0, 0.0, 0.0 ), 80.0, 1.0, 0.0, 0.0, 0.0, 0, -1 ) );
	sphere->setShader( new PhongShader() );
	scene->add( sphere );
*/

	matrix.setCol( 3, Vector( 0.0, 4.0, 0.0 ) );
	Sphere* sphere = new Sphere( matrix, 5.0 );
	sphere->setMaterial( new PhongMaterial( Vector( 0.05, 0.05, 0.05 ), Vector( 0.8, 0.8, 0.8 ), Vector( 1.0, 1.0, 1.0 ), Vector( 0.0, 0.0, 0.0 ), 50.0, 1.0, 0.0, 0.0, 0.0, -1, 0 ) );
	sphere->setShader( new PhongNormalShader() );
	scene->add( sphere );

	matrix.makeIdentity();

	matrix.setCol( 3, Vector( -5.0, 2.0, 5.0 ) );
	Sphere* sphere2 = new Sphere( matrix, 2.0 );
	sphere2->setMaterial( new PhongMaterial( Vector( 0.05, 0.05, 0.05 ), Vector( 0.8, 0.8, 0.8 ), Vector( 1.0, 1.0, 1.0 ), Vector( 0.0, 0.0, 0.0 ), 80.0, 1.0, 0.0, 0.0, 0.0, 2, -1 ) );
	sphere2->setShader( new PhongShader() );
	scene->add( sphere2 );

	matrix.makeIdentity();

	/// plane
	matrix.setCol( 3, Vector( 0.0, 0.0, 0.0 ) );
	Plane* plane = new Plane( matrix, 200.0, 200.0 );
	plane->setMaterial( new PhongMaterial( Vector( 0.2, 0.2, 0.2 ), Vector( 0.8, 0.8, 0.8 ), Vector( 1.0, 1.0, 1.0 ), Vector( 0.0, 0.0, 0.0 ), 80.0, 1.0, 0.0, 0.0, 0.0, 0, -1 ) );
	plane->setShader( new PhongShader() );
	plane->setTiling( Vector( 2.05, 2.05, 0.0 ) );
	scene->add( plane );

	matrix.makeIdentity();

	/// light
	matrix.setCol( 3, Vector( 30.0, 60.0, 45.0 ) );
	matrix *= Matrix().makeRotationZ( 180.0 * DEG2RAD );
	scene->add( new AreaLight( matrix, Vector( 1.0, 1.0, 1.0 ), Vector( 6.0, 6.0, 0.0 ) ) );
	//scene->add( new OmniLight( matrix, Vector( 0.8, 0.8, 0.8 ) ) );

	scene->transform();
	return scene;
}
