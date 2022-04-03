//
// File:         GlobalClock.cpp
// Description:  A handy wrapper for the virtual process timer.
// Created:      Jan 30, 2000, Alexandre Riazanov, riazanov@cs.man.ac.uk 
//============================================================================
#include <sys/time.h>
#include <signal.h>
#include "GlobalClock.h"
//============================================================================

using namespace BK;
long GlobalClock::InitStatic::_count; // implicitely initialised to 0L
GlobalClock::AlarmHandler* GlobalClock::_currentAlarmHandler; 
int GlobalClock::_elapsedDeciseconds;



GlobalClock::InitStatic::InitStatic()
{
  if (!_count)
    {
      GlobalClock::_currentAlarmHandler = (AlarmHandler*)0;
      GlobalClock::_elapsedDeciseconds = 0;
      static struct itimerval timerSettings;
      timerSettings.it_interval.tv_sec = 0;
      timerSettings.it_interval.tv_usec = 100000; // 1 decisecond
      timerSettings.it_value.tv_sec = 0;
      timerSettings.it_value.tv_usec = 100000; // 1 decisecond
      signal(SIGVTALRM,GlobalClock::alarmSignalHandler);
      setitimer(ITIMER_VIRTUAL,&timerSettings,(struct itimerval*)0);
    };
  ++_count;
}; //GlobalClock::InitStatic::InitStatic()

GlobalClock::InitStatic::~InitStatic()
{
  --_count;
  if (!_count)
    {
		  
    };
}; // GlobalClock::InitStatic::~InitStatic()

void GlobalClock::alarmSignalHandler(int)
{
  signal(SIGVTALRM,GlobalClock::alarmSignalHandler);
  ++_elapsedDeciseconds;
  if (_currentAlarmHandler)
    {
      if (_currentAlarmHandler->_handlingFunction)
	{
	  _currentAlarmHandler->_handlingFunction(_elapsedDeciseconds);
	};
    };
}; // void GlobalClock::alarmSignalHandler(int)

//============================================================================
