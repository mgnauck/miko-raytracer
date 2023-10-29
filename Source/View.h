
#ifndef __VIEW_H__
#define __VIEW_H__

struct View {

	View( int w, int h ) : width( w ), height( h ) {}

	double getAspect() const {
		return width / static_cast<double>( height );
	}

	int	width;
	int	height;
};

#endif
