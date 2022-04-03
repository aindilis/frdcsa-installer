//
// File:         VampireKernelConst.h
// Description:  Global constants for VampireKernel.
// Created:      Feb 12, 2000, 17:50
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//====================================================
#ifndef HELL_CONST_H
//====================================================
#define HELL_CONST_H 
#include <limits.h>
#include <stddef.h>
#include "jargon.h"
//====================================================
class VampireKernelConst
{
 public:
  // interface 
  static const char* const SkolemConstantNamePrefix = "c__";
  static const char* const SkolemFunctionNamePrefix = "f__";
  static const char* const SkolemPredicateNamePrefix = "p__";
  static const char* const CanonicalVariableNamePrefix = "X";
  // basic parameters
  static const ulong MaxlNumOfLits = 65535; // can not be greater than 2^16 - 1 NEW!
  static const ulong MaxSymbolArity = 1023; // can not be greater than 2^12 - 1, see term.h
#ifdef LARGE_TERMS
  static const ulong MaxNumOfVariables = 128;
  static const ulong MaxTermDepth = 48;
  static const ulong MaxTermSize = 256;  
  static const ulong MaxClauseSize = 512;
#else 
#ifdef HUGE_TERMS
  static const ulong MaxNumOfVariables = 256;
  static const ulong MaxTermDepth = 1024;
  static const ulong MaxTermSize = 1024; 
  static const ulong MaxClauseSize = 2048;
#else
  static const ulong MaxNumOfVariables = 64;
  static const ulong MaxTermDepth = 24;  
  static const ulong MaxTermSize = 128;   
  static const ulong MaxClauseSize = 256;
#endif
#endif
 
  static const ulong MaxlSymbolNum = 65535; 
 
  static const long MinMaxWeight = 1L;

  static const ulong MaxNumOfStandardIndices = 2; // for 2 premises in a rule

 // hard values: don't change this unless you know what you are doing!
  static const ulong UnordEqNum = 0UL;
  static const ulong OrdEqNum = 1UL;
  static const ulong FirstNoneqFun = 2UL;
  static const ulong BuiltInTrueNum = 2UL;
  static const ulong ReservedPred0 = 3UL;
  static const ulong ReservedPred1 = 4UL;
  static const ulong ReservedPred2 = 5UL;
 
  // derived values
  static const ulong MaxlHeaderNum = MaxlSymbolNum*2 + 1; 

  static const ulong MaxNumOfIndices = MaxNumOfStandardIndices + 0; 

  // checking integrity
  static bool checkIntegrity() 
    { 
      return (UnordEqNum == 0)
	&& (OrdEqNum == 1)
	&& (FirstNoneqFun == 2);
 
    };
  // functions
  static bool isEquality(const ulong& num) { return (num < FirstNoneqFun); }; 
  static bool isOrderedEq(const ulong& num) { return (bool)num; }; 
  static bool isUnorderedEq(const ulong& num) { return !num; }; 
  static bool isBuiltInTrue(const ulong& num) { return num == BuiltInTrueNum; };
  static bool isReservedPred(const ulong& num) { return (num > 1) && (num < 5); };
}; // namespace VampireKernelConst
//====================================================
#endif



