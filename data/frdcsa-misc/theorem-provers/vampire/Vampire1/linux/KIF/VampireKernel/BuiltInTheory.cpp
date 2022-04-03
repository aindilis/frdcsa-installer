//
// File:         BuiltInTheory.cpp
// Description:  Built-in theory abstraction.  
// Created:      Jul 06, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//==================================================================
#include "RuntimeError.h"
#include "BuiltInTheory.h"
//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_BUILT_IN_THEORY
 #define DEBUG_NAMESPACE "BuiltInTheory"
#endif
#include "debugMacros.h"
//===================================================================

using namespace VK;
using namespace BK;

BuiltInTheory::~BuiltInTheory()
{
};

void* BuiltInTheory::operator new(size_t)
{
  CALL("operator new(size_t)");
  RuntimeError::report("Call to BuiltInTheory::operator new(size_t)");
  return (void*)0;
}; // void* BuiltInTheory::operator new(size_t)

void BuiltInTheory::operator delete(void* obj)
{
  CALL("operator delete(void* obj)");
  RuntimeError::report("Call to BuiltInTheory::operator delete(void* obj)");
}; // void BuiltInTheory::operator delete(void* obj)

//==================================================================
