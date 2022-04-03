//
// File:         DefinedHeaderTable.h
// Description:  Set of defined headers for stratified resolution.
// Created:      Dec 24, 2001.
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//============================================================================
#ifndef DEFINED_HEADER_TABLE_H
//============================================================================

#define DEFINED_HEADER_TABLE_H
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "GlobAlloc.h"
#include "Array.h"
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_DEFINED_HEADER_TABLE
 #define DEBUG_NAMESPACE "DefinedHeaderTable"
#endif
#include "debugMacros.h"
//============================================================================
namespace VK
{
  using namespace BK;
class DefinedHeaderTable
{
 public:
  DefinedHeaderTable() 
  {
   CALL("DefinedHeaderTable()");
   init();
  };
  ~DefinedHeaderTable() {};
  void init()
  {
   CALL("init()");
    #ifdef NO_DEBUG 
     _defined.init(false,(int)0);
    #else
     _defined.init(false,"DefinedHeaderTable::_defined");
    #endif
   DOP(_defined.freeze());
  };

  void registerDefined(ulong headerNum) 
  {
   CALL("registerDefined(ulong headerNum)");
   DOP(_defined.unfreeze());
   _defined.sub(headerNum) = true;
   DOP(_defined.freeze());
  };

  bool isDefined(ulong headerNum) const 
  {
   CALL("isDefined(ulong headerNum) const");
   if (headerNum >= _defined.size()) return false;
   return _defined[headerNum];
  };  

 private:
  Array<GlobAlloc,bool,128UL,DefinedHeaderTable> _defined;
}; // class DefinedHeaderTable

}; // namespace VK
//======================================================================
#endif
