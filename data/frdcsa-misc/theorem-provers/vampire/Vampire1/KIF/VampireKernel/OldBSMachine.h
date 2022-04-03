//
// File:         OldBSMachine.h
// Description:  Core of backward subsumption based on discrimination trees. 
// Created:      Feb 26, 2000, 20:20
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//============================================================================
#ifndef OLD_BS_MACHINE_H  
//=============================================================================
#define OLD_BS_MACHINE_H 
#ifndef NO_DEBUG_VIS
 #include <iostream.h>
#endif
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "VampireKernelConst.h"
#include "Stack.h"
#include "OldBSIndex.h"
#include "Term.h"
#include "Clause.h"
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_OLD_BS_MACHINE
 #define DEBUG_NAMESPACE "OldBSMachine"
#endif
#include "debugMacros.h"
//============================================================================
namespace VK
{
  using namespace BK;
  using namespace Gematogen;
class OldBSMachine
{
 public: 
  typedef OldBSIndex::CSkipList CSkipList;
 public:
  OldBSMachine() {};
  ~OldBSMachine() { CALL("destructor ~OldBSMachine()"); };
  bool subsumeNextSetMode(ulong& subsumedClNum);
  bool subsumeNextMultisetMode(ulong& subsumedClNum);
  void resetQuery(const Clause* cl) 
  { 
   _clause = cl;
  };
  bool endOfQuery() 
  {
   return false; // dummy 
  }; 
  bool queryPropLit(const CSkipList* occList) 
  {  
   return false; // dummy 
  }; 
  void queryNextLit() 
  { 
  }; 
  bool queryEndOfLit() 
  {      
    return false; // dummy 
  };

 private:
  const Clause* _clause;
}; // class OldBSMachine 

}; // namespace VK


//======================================================================
#undef DEBUG_NAMESPACE
//======================================================================
#endif
