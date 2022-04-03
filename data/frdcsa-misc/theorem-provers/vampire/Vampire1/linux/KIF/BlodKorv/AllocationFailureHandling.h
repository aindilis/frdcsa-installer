//
// File:         AllocationFailureHandling.h
// Description:  Primitives for flexible and extendable 
//               handling of allocation failures.  
// Created:      Nov 15, 2001
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//====================================================
#ifndef ALLOCATION_FAILURE_HANDLING_H
//====================================================
#define ALLOCATION_FAILURE_HANDLING_H
#include "jargon.h"
#include "Exit.h"
#include "DestructionMode.h"
#include "RuntimeError.h"
#include "BlodKorvDebugFlags.h"
#include "Debug.h"
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_ALLOCATION_FAILURE_HANDLING
 #define DEBUG_NAMESPACE "AllocationFailureHandling<InstanceId>"
#endif 
#include "debugMacros.h"
//=================================================

namespace BK 
{

template <class InstanceId> 
class AllocationFailureHandling
{
 public: 
  class Activator
  {
   public:
    Activator(AllocationFailureHandling* handler)
    {
     CALL("Activator::constructor Activator(AllocationFailureHandling* handler)");
     _handler = handler;
     _handler->activate();
    };
    ~Activator() 
    {
     CALL("Activator::destructor ~Activator()"); 
     if (DestructionMode::isThorough()) _handler->deactivate();
    };
   private: 
    Activator() 
    {
     CALL("Activator::constructor Activator()");
     ICP("ICP0");
    };
    AllocationFailureHandling* _handler;
  };
 public:
  static void initStatic() 
  {
   _currentHandler = (AllocationFailureHandling*)0;
  }; 
  AllocationFailureHandling() 
   : _inheritMemoryReleaseRequest(false),
     _memoryReleaseRequest(0),
     _inheritUnrecoverableAllocationFailureHandler(false),
     _unrecoverableAllocationFailureHandler(0),
     _previousHandler((AllocationFailureHandling*)0)
  {
  };
  ~AllocationFailureHandling()
  {
  };
  void activate() 
  {
   CALL("activate()");
   if (_previousHandler)
    {
     RuntimeError::report("activating allocation failure handler which is active");
     Exit::exit(1);  
    }
   else
    {
     _previousHandler = _currentHandler;
     _currentHandler = this;
    };
  };
  void deactivate()
  {
   CALL("deactivate()");
   _currentHandler = _previousHandler;
   _previousHandler = (AllocationFailureHandling*)0;
  };
    
  bool& inheritMemoryReleaseRequest() { return _inheritMemoryReleaseRequest; };
  bool inheritMemoryReleaseRequest() const { return _inheritMemoryReleaseRequest; };
  void setMemoryReleaseRequest(bool (*memoryReleaseRequest)(ulong size))
  {
   _memoryReleaseRequest = memoryReleaseRequest;
  };
   
  bool& inheritUnrecoverableAllocationFailureHandler() 
  { 
   return _inheritUnrecoverableAllocationFailureHandler;
  };
  bool inheritUnrecoverableAllocationFailureHandler() const
  { 
   return _inheritUnrecoverableAllocationFailureHandler;
  };

  void setUnrecoverableAllocationFailureHandler(void (*unrecoverableAllocationFailureHandler)(ulong size))
  {
   _unrecoverableAllocationFailureHandler = unrecoverableAllocationFailureHandler;
  };

  static bool requestMemoryRelease(ulong size)
  {
   CALL("requestMemoryRelease(ulong size)");
   return _currentHandler->callMemoryRelease(size);
  };

  static void reportUnrecoverableAllocationFailure(ulong size)
  {
   CALL("reportUnrecoverableAllocationFailure(ulong size)");
   _currentHandler->reportUltimateFailure(size);
  };
 
 private:  
  bool callMemoryRelease(ulong size)
  {
   return this     
          && ((_inheritMemoryReleaseRequest) ? 
              _previousHandler->callMemoryRelease(size)
               :
              (_memoryReleaseRequest && _memoryReleaseRequest(size)));         
  };
 
  void reportUltimateFailure(ulong size)
  {
   if (this)
    {
     if (_inheritUnrecoverableAllocationFailureHandler)
      {
       _previousHandler->reportUltimateFailure(size);
      }
     else
      if (_unrecoverableAllocationFailureHandler)
       _unrecoverableAllocationFailureHandler(size); 
    };
   RuntimeError::report("unhandled unrecoverable allocation failure");
   Exit::exit(1);    
  };
 private:
  bool _inheritMemoryReleaseRequest;
  bool (*_memoryReleaseRequest)(ulong size);
  bool _inheritUnrecoverableAllocationFailureHandler;
  void (*_unrecoverableAllocationFailureHandler)(ulong size);
  AllocationFailureHandling* _previousHandler;
  static AllocationFailureHandling* _currentHandler;

}; // template <class InstanceId> class AllocationFailureHandling

}; // namespace BK

//====================================================
#endif



