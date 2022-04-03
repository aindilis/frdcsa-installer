//
// File:         BSMachine.cpp
// Description:  Core of backward subsumption.
// Created:      Feb 26, 2000, 20:20
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//============================================================================
#include "BSMachine.h"
//============================================================================
#undef DEBUG_NAMESPACE 
#ifdef DEBUG_BS_MACHINE
 #define DEBUG_NAMESPACE "BSMachine"
#endif
#include "debugMacros.h"
//============================================================================

namespace VK
{
  using namespace BK;
  using namespace Gematogen;
bool BSMachine::subsumeNextSetMode(ulong& subsumedClNum)
{
 CALL("subsumeNextSetMode(ulong& subsumedClNum)");
 while (_clauseEqualizer.next())
  {
   ulong i;	  
   for (i = 0; i < _nextLit; i++) _litEqualizer[i].load(); 
   for (i = 0; i < _nextSymLit; i++) _symLitEqualizer[i].load();
   if (equalizeLiterals())
    {
     if (_substJoin.joinInSetMode()) { subsumedClNum = _clauseEqualizer.currClauseNum(); return true; };
    };
  };
 return false;         
}; // bool BSMachine::subsumeNextSetMode(ulong& subsumedClNum)

bool BSMachine::subsumeNextMultisetMode(ulong& subsumedClNum)
{
 CALL("subsumeNextMultisetMode(ulong& subsumedClNum)");
 while (_clauseEqualizer.next())
  {
   ulong i;	  
   for (i = 0; i < _nextLit; i++) _litEqualizer[i].load(); 
   for (i = 0; i < _nextSymLit; i++) _symLitEqualizer[i].load();
   if (equalizeLiterals())
    {
     if (_substJoin.joinInMultisetMode()) { subsumedClNum = _clauseEqualizer.currClauseNum(); return true; };
    };
  };
 return false;         
}; // bool BSMachine::subsumeNextMultisetMode(ulong& subsumedClNum)



}; // namespace VK

//============================================================================
