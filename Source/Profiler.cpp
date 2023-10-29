
#include "Profiler.h"
#include <iostream>
#include <cmath>
#include "defines.h"

#ifndef __NO_TIMING__
	#include <windows.h>
#endif

Profiler::Profiler( std::string const& tag ) : profileTag( tag ) {

#ifndef __NO_TIMING__
	startTime = GetTickCount();
#endif

#ifndef __NO_MESSAGES__
	std::cout << "started: " << profileTag << std::endl;
#endif
}

Profiler::~Profiler() {

#ifndef __NO_MESSAGES__
	std::cout << "stopped: " << profileTag << std::endl;
#endif
	stopProfiling();
}

void Profiler::stopProfiling() {

#ifndef __NO_TIMING__
	double						duration		= ( GetTickCount() - startTime ) / 60000.0;
	std::ios_base::fmtflags		originalFlags	= std::cout.flags();

	std::cout.precision( 3 );
	std::cout << "duration: " << floor( duration ) << " mins ";
	std::cout << (int)( ( duration - floor( duration ) ) * 60.0 ) << " secs" << std::endl;
	std::cout.flags( originalFlags );
#endif
}
