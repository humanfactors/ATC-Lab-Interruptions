#ifndef __TIMER_HDR__
#define __TIMER_HDR__


#ifdef WIN32
	// disable - decorated name length exceeded
	#pragma warning(disable:4503)
	// disable 'identifier' truncation warning
	#pragma warning(disable:4786)
#endif

#include <windows.h>

namespace atc
{	
	class Timer
	{
	public:
		static Timer* instance();
		static void destroy();
		void    start();
        double  elapsed_time();

	private:
		Timer();
		~Timer();

		static Timer*   m_instance;
        LARGE_INTEGER   m_frequency;
        LARGE_INTEGER   m_tickStart;
        int             m_hpcExists;
	};
}; 

#endif 