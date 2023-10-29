
#ifndef __PROFILER_H__
#define __PROFILER_H__

#include <string>

class Profiler {
	public:
		Profiler( std::string const& tag );
		virtual ~Profiler();
	
	private:
		virtual void	stopProfiling();

		unsigned long	startTime;
		std::string		profileTag;
};

#endif