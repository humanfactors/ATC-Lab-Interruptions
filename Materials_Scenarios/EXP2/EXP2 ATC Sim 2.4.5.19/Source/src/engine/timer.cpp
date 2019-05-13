#include "Timer.h"

using namespace atc;

Timer* Timer::m_instance = 0;

Timer* 
Timer::instance() 
{
	if (!m_instance)
    {
        m_instance = new Timer;
    }

	return m_instance;
}

void 
Timer::destroy()
{
	if (m_instance)
    {
		delete (m_instance);
		m_instance = 0;
	}
}

//------------------------------------------------------------------------------
// Construction/Destruction
//
Timer::Timer()
{
	// Get frequency of the high-resolution performance counter
    m_hpcExists = QueryPerformanceFrequency(&m_frequency);
}

Timer::~Timer()
{
}

void
Timer::start()
{
    if (m_hpcExists)
    {
        // Get the new reference tick count
        QueryPerformanceCounter(&m_tickStart);
     }
}

double
Timer::elapsed_time()
{
    double ret = -1.0;
    LARGE_INTEGER ticksNow;

    if (m_hpcExists)
    {
        QueryPerformanceCounter(&ticksNow);

        ret = (ticksNow.QuadPart - m_tickStart.QuadPart) * 1000.0 / m_frequency.QuadPart;
    }

    return ret;
}
