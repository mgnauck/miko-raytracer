/*
	miko raytracer
	written by markus gnauck (dec2004-mar2005)
	markus@unik.de

	some features:
	- primitives + meshes
	- bounding volumes
	- shadow caching
	- procedural + image textures
	- normal mapping
	- spotlight, area light, skylight
	- monte carlo GI
	- XML scene format support (via TinyXML)

	known bugs/missing import:
	- resolution change influences viewport
	- import of duskshroud scene -> missing hands of figure???
	- no area light import
	- no skylight import
	- no procedural texture import
	- bump amount factor is not imported

	todo:
	- glossy reflections (use gloss factor from material for dimension)
	- reimplement antialiasing
	- octree
	- more clever sampling for gi
	- real aabb test :)
*/

#include "Renderer.h"
#include "Profiler.h"
#include "Import.h"
#include "ImageIO.h"

#include <iostream>
#include <string>
using namespace std;

string	inname;
string	outname			= "out.png";
int		xres			= 640;
int		yres			= 480;
int		recursion		= 1;
int		samples			= 0;
int		lightsamples	= 0;

bool interpretCommanline( int argc, char* argv[] ) {

	if( argc == 2 ) {
		inname = argv[ 1 ];
		return true;
	}

	if( argc == 6 ) {
		inname		= argv[ 1 ];
		outname		= argv[ 2 ];
		xres		= atoi( argv[ 3 ] );
		yres		= atoi( argv[ 4 ] );
		recursion	= atoi( argv[ 5 ] );
		return true;
	}

	if( argc == 8 ) {
		inname			= argv[ 1 ];
		outname			= argv[ 2 ];
		xres			= atoi( argv[ 3 ] );
		yres			= atoi( argv[ 4 ] );
		recursion		= atoi( argv[ 5 ] );
		samples			= atoi( argv[ 6 ] );
		lightsamples	= atoi( argv[ 7 ] );
		return true;
	}

	cout << "possible commands (missing are default):" << endl;
	cout << "1) miko.exe infile" << endl;
	cout << "2) miko.exe infile outfile res_x res_y recursion" << endl;
	cout << "3) miko.exe infile outfile res_x res_y recursion samples lightsamples" << endl;
	cout << endl;
	cout << "infile is scene file in xml3d format" << endl;
	cout << "outfile is picture with types like png/jpg/bmp" << endl;
	cout << "res_x + res_y is resolution of outfile" << endl;
	cout << "recursion is max recursion depth" << endl;
	cout << "samples is sample count for gi (if samples=0 no gi is done)" << endl;
	cout << "lightsamples is sample count for arealights/skylights" << endl;
	cout << endl;
	cout << "default parameters are: " << outname << ", " << xres << ", " << yres << ", " << recursion << ", " << samples << ", " << lightsamples << endl;
	cout << "texture/normal maps have to be in path were miko.exe resides. sorry." << endl;

	return false;
}

int main( int argc, char* argv[] ) {

	cout << endl;
	cout << "miko raytracer" << endl;
	cout << "markus@unik.de" << endl;
	cout << endl;

	if( !interpretCommanline( argc, argv ) )
		return 0;

	////
	ImageIO::init();

	Renderer	renderer;
	Scene*		scene = new Scene();
	Import		import( inname, scene, &renderer );
	Raytracer*	raytracer = new Raytracer( scene, RecursionProperties( recursion, samples, lightsamples ) );
	BitMap*		pic = 0;

	if( samples > 0 )
		raytracer->enableGlobalIllumination();

	renderer.init( raytracer, new View( xres, yres ) );

	{
		Profiler profiler( "go and trace.." );
		pic = renderer.render();
	}

	cout << endl << "picture saved to <" << outname << ">" << endl;
	ImageIO::save( outname, *pic );
	delete pic;

	return 0;
}
