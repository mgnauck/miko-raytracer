
#ifndef __RECURSION_PROPERTIES_H__
#define __RECURSION_PROPERTIES_H__

struct RecursionProperties {

	RecursionProperties() {

		depth			= 0;
		samples			= 0;
		lightSamples	= 1;
	}

	RecursionProperties( int _depth, int _samples, int _lightSamples ) {

		depth			= _depth;
		samples			= _samples;
		lightSamples	= _lightSamples;
	}

	int	depth;
	int	samples;
	int	lightSamples;
};

#endif