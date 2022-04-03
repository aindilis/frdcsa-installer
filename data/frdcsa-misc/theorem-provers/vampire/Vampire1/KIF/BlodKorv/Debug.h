//
// File:         Debug.h
// Description:  
// Created:      Oct 1, 1999, Alexandre Riazanov, riazanov@cs.man.ac.uk
// Revised:      Jan 17, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//               1) class InitStatic added instead of the old 
//               scheme for initilising the static part. 
//               2) nextEvent() made hookable from outside. 
//======================================================================
#ifndef NO_DEBUG
#ifndef DEBUG_H 
//======================================================================
#define DEBUG_H
#include <iostream.h>
#include <time.h>
#include "jargon.h"
#include "BlodKorvDebugFlags.h"
//=====================================================================

namespace BK 
{
class Debug
{
 public:
  class InitStatic
    {
    public: 
      InitStatic() 
	{
	  if (!_count) { Debug::initStatic(); _count++; };    
	};
      ~InitStatic() {};
    private:
      static long _count;
    }; // class InitStatic
  class Call;
  class ProgressCheck;
 public:  
  static void initStatic();
  static void assignJob(const char* jobId) { job = jobId; }; 
  static void setTracing(ostream* str,bool traceCOPs,bool traceAfterErr) 
    { 
      trace = str; 
      traceCP = traceCOPs; 
      traceAfterError = traceAfterErr;
    };
  static void tracingOff() { trace = (ostream*)0; };
  static void setNextEventHook(void (*fun)(ulong))
    {
      nextEventHook = fun;
    };
  static void setErrorHook(void (*fun)())
    {
      errorHook = fun;
    };
  static void cp(const char* cop) 
    {    
      controlPoint = cop;
      nextEvent();
      if (trace && traceCP) *trace << eventId << " COP " << controlPoint << '\n'; 
    };
  static void error();
  static void error(const char* msg);
  static void debugFeatureInternalFault(const char* msg);
  static void reportCP();
  static void history();
  static void reportSlowProgress(const char* checkName,float maxInt,float realInt);
  static ulong event() { return eventId; };                                 
  static ostream& msgStream() { return cout; };
  //private: 
 public: // made public to please VC++
  struct StrList // Debug.h must be self-contained
  {
    StrList(const char* h,StrList* t) : hd(h), tl(t) {};
    const char* hd;
    StrList* tl;
  };       
 private:
  Debug() {};       
  ~Debug() {}; 
  static void nextEvent() 
    { 
      eventId++; 
      if (nextEventHook) nextEventHook(eventId); 
    }; 
  static StrList* newStrList(const char* h,StrList* t);
  static void recycle(StrList* l);
  static void initSigHandlers();
  static void reportCP(ulong ev,const char* ns,const char* cn,const char* cp,const char* on); 
  static void handleSIGFPE(int);
  static void handleSIGILL(int);
  static void handleSIGSEGV(int);
  static void handleSIGBUS(int);
  static void handleSIGABRT(int);
  //static void handleSIGIOT(int);
  static void handleSIGTRAP(int);
 private:
  static const char* job;   
  static ulong eventId;
  static const char* controlPoint;
  static const char* callName;
  static long callDepth;
  static const char* objectName;
  static const char* nameSpace;
  static const char* errorDescription;
  static StrList* controlPoints;
  static StrList* callNames;
  static StrList* objectNames;
  static StrList* nameSpaces;
  static StrList* recycledSL;
  static ostream* trace;
  static bool traceCP;
  static bool traceAfterError;
  static void (*nextEventHook)(ulong eventNum);
  static void (*errorHook)(); 
  //static Debug _initStatic;
  friend class Call;  
}; // class Debug

class Debug::Call
{
 public:
  Call(const char* nameSp,const char* callNm);
  Call(const char* nameSp,const char* objName,const char* callNm);
  ~Call();
 private:
  Call() {};
}; // class Debug::Call

class Debug::ProgressCheck
{
 public:
  ProgressCheck(const char* name,float freq) : _name(name)
    {
      _lastInterval = 0;   
      _maxInterval = (clock_t)(freq * CLOCKS_PER_SEC);
      _lastCheckUp = clock(); 
    }; 
  ~ProgressCheck() 
    {
      check();
    };
  void check() 
    {
      clock_t tmp = clock();
      _lastInterval = tmp - _lastCheckUp;
      _lastCheckUp = tmp;
      if (_lastInterval > _maxInterval)
	{
	  Debug::reportSlowProgress(_name,(float)(_maxInterval)/CLOCKS_PER_SEC,(float)(_lastInterval)/CLOCKS_PER_SEC);
	};
    };
 private:
  ProgressCheck() {};
 private:
  const char* _name;
  clock_t _lastInterval;
  clock_t _maxInterval;
  clock_t _lastCheckUp;  
};

//==================================================================

inline Debug::StrList* Debug::newStrList(const char* h,StrList* t)
{
  if (recycledSL)
    {
      StrList* res = recycledSL;
      recycledSL = recycledSL->tl;
      res->hd = h;
      res->tl = t;
      return res;
    }
  else 
    {
      StrList* res = new StrList(h,t);
      if (!res) debugFeatureInternalFault("Failed to allocate data needed for debugging purposes");
      return res;
    };
}; //Debug::StrList* Debug::newStrList() 

inline void Debug::recycle(StrList* l)
{
  l->tl = recycledSL;
  recycledSL = l;
}; // Debug::recycle(StrList* l) 


//===================================================================

static Debug::InitStatic debugInitStatic;

}; // namespace BK


//===================================================================
#endif
#endif
