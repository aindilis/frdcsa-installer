//
// File:         GlobalClock.h
// Description:  A handy wrapper for the virtual process timer.
// Created:      Jan 30, 2000, Alexandre Riazanov, riazanov@cs.man.ac.uk 
//============================================================================
#ifndef GLOBAL_CLOCK_H
#define GLOBAL_CLOCK_H
//============================================================================
namespace BK
{
  class GlobalClock
    {
    public:
      class InitStatic
	{
	public:
	  InitStatic();
	  ~InitStatic();
	  
	private:
	  static long _count;
	}; // class InitStatic
      class AlarmHandler
	{
	public:
	  AlarmHandler(void (*handlingFunction)(int)) : 
	    _handlingFunction(handlingFunction),
	    _previous(GlobalClock::_currentAlarmHandler)
	    {
	      GlobalClock::_currentAlarmHandler = this;
	    };
	  ~AlarmHandler() 
	    {
	      if (GlobalClock::_currentAlarmHandler == this)
		{
		  GlobalClock::_currentAlarmHandler = _previous;
		};
	    };
	private:
	  AlarmHandler() {};
	private:
	  void (*_handlingFunction)(int);
	  AlarmHandler* _previous;
	  friend class GlobalClock;
	}; // class AlarmHandler

      class Timer
	{
	public:
	  Timer()
	    {
	      reset();
	    };
	  ~Timer()
	    {
	    };
	  void reset() { _stopped = true; _elapsedDeciseconds = 0; };
	  void start() 
	    {
	      if (_stopped)
		{
		  _stopped = false;
		  _startTime = GlobalClock::elapsedDeciseconds();
		};
	    };
	  void stop() 
	    {
	      if (!_stopped)
		{
		  _stopped = true;
		  _elapsedDeciseconds += (GlobalClock::elapsedDeciseconds() - _startTime);
		};
	    };
	  int elapsedDeciseconds() const
	    { 
	      if (_stopped) return _elapsedDeciseconds;
	      return _elapsedDeciseconds + 
		(GlobalClock::elapsedDeciseconds() - _startTime);
	    };
	  float elapsedSeconds() const { return ((float)elapsedDeciseconds())/10; };
	  bool isTicking() const { return !_stopped; };
	private:
	  bool _stopped;
	  int _startTime;
	  int _elapsedDeciseconds;
	}; // class Timer
    public:
      static int elapsedDeciseconds() { return _elapsedDeciseconds; };
      static float elapsedSeconds() { return ((float)_elapsedDeciseconds)/10; };
    private: 
      static void alarmSignalHandler(int);
    private:
      static AlarmHandler* _currentAlarmHandler;
      static int _elapsedDeciseconds;
      friend class InitStatic;
      friend class AlarmHandler;
    }; // class GlobalClock

  static GlobalClock::InitStatic globalClockInitStatic;

}; // namespace BK



//============================================================================
#endif
