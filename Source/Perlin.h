
#ifndef __PERLIN_H__
#define __PERLIN_H__

class Perlin {
public:
	typedef double value_type;

	Perlin( int perlinTableSize );
	~Perlin();

	value_type Perlin1D( value_type u, int octaves, value_type persistence ) const;
	value_type Perlin2D( value_type u, value_type v, int octaves, value_type persistence ) const;
	value_type Perlin3D( value_type u, value_type v, value_type w, int octaves, value_type persistence ) const;

private:
	value_type interpolate( value_type a, value_type b, double t ) const;
	value_type getTab( int u, int v ) const;

	int			tabSize;
	value_type*	tab;
};

#endif