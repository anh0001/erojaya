#ifndef TIMER_H
#define TIMER_H

// #define MONITOR

#include <time.h>
#include <timer/monitor/timermonitor.h>

class MotionTimer
{
public:
	MotionTimer(double rate);
	virtual ~MotionTimer();

	long unsigned int sleep();

private:
    int m_timerfd;


#ifdef MONITOR
	TimerMonitor m_monitor;
#endif
};

#endif
