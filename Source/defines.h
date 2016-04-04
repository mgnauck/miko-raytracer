
#ifndef __DEFINES_H__
#define __DEFINES_H__

//#define __NO_DEVIL__

//#define __NO_PROGRESS__
//#define __NO_TIMING__
//#define __QUIET__

#ifdef __QUIET__

	#define __NO_PROGRESS__
	#define __NO_TIMING__
	#define __NO_MESSAGES__

#endif

#endif