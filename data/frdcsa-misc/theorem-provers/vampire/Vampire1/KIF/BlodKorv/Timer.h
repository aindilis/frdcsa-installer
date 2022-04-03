//
// File:         Timer.h
// Description:  Interface for "clock".
// Created:      Feb 16, 2000, 15:20
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//============================================================================
#ifndef TIMER_H
//=============================================================================
#define TIMER_H 
#include <time.h>
#include "jargon.h"
#include "BlodKorvDebugFlags.h"
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_TIMER
 #define DEBUG_NAMESPACE "Timer"
#endif
#include "debugMacros.h"
//============================================================================
 
namespace BK 
{

class Timer
{
 public:
  Timer() 
  {
   CALL("constructor Timer()"); 
   reset(); 
  };
  ~Timer() {}; 
  void reset()
  {
   stopped = true;
   elapsedSec = 0;
  };
  void start()
  {
   if (stopped)   
    {
     stopped = false;
     startTime = clock();     
    };
  };
  void stop()
  {
   if (!stopped)
    {
     stopped = true;
     clock_t currTime = clock();
     if (currTime > startTime)      
       elapsedSec += (((float)(currTime - startTime))/CLOCKS_PER_SEC);
    };
  };

    
  float elapsed()
  {
   if (stopped) return elapsedSec; 
   clock_t currTime = clock();     
   if (currTime > startTime) 
    return elapsedSec + (((float)(currTime - startTime))/CLOCKS_PER_SEC); 
   return elapsedSec;
  };
 private:   
  bool stopped;
  float elapsedSec;
  clock_t startTime;
}; // class Timer

}; // namespace BK
//======================================================================
#undef DEBUG_NAMESPACE
//======================================================================
#endif
