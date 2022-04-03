//
// File:         ClauseNumIndex.h
// Description:  Searching clauses by numbers.
// Created:      Apr 08, 2000, 22:00
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//============================================================================
#ifndef CLAUSE_NUM_INDEX_H
//============================================================================
#define CLAUSE_NUM_INDEX_H
#include "jargon.h"
#include "VampireKernelDebugFlags.h"    
#include "SkipList.h"
#include "GlobAlloc.h"
#include "DestructionMode.h"
namespace VK
{
class Clause;
}; // namespace VK
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_CLAUSE_NUM_INDEX
 #define DEBUG_NAMESPACE "ClauseNumIndex"
#endif
#include "debugMacros.h"
//============================================================================
namespace VK
{
  using namespace BK;
class ClauseNumIndex
{
 public:
  ClauseNumIndex() : _list(), _surfer() 
  {
   CALL("constructor ClauseNumIndex()");
  };
  ~ClauseNumIndex() 
    { 
      CALL("destructor ~ClauseNumIndex()"); 
      if (DestructionMode::isThorough()) reset(); 
    };
  void insert(Clause* cl);
  void remove(Clause* cl);
  const Clause* find(ulong num);
  void reset()
  {
   _list.reset();  
  };
  #ifndef NO_DEBUG
   bool clean() const { return !_list; };
  #endif
 private:
  typedef SkipList<GlobAlloc,const Clause*,ulong,2,28,ClauseNumIndex> List;
 private:
  List _list;
  List::Surfer _surfer;
}; // class ClauseNumIndex

}; // namespace VK

//======================================================================
#undef DEBUG_NAMESPACE
//======================================================================
#endif
