//
// File:         GlobalStopFlag.cpp
// Description:  Soft interrupt.
// Created:      Jun 12, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk 
//============================================================================
#include "GlobalStopFlag.h"
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_GLOBAL_STOP_FLAG
 #define DEBUG_NAMESPACE "GlobalStopFlag::HandlerActivator"
#endif
#include "debugMacros.h"
//============================================================================

using namespace BK;
bool GlobalStopFlag::_stop = false;

GlobalStopFlag::HandlerActivator* 
GlobalStopFlag::_currentHandlerActivator = (GlobalStopFlag::HandlerActivator*)0;

void GlobalStopFlag::HandlerActivator::handle()
{
  CALL("handle()");
  if (!this) throw DefaultException();
  _handler();
  _previous->handle();
}; // void GlobalStopFlag::HandlerActivator::handle()


//============================================================================
