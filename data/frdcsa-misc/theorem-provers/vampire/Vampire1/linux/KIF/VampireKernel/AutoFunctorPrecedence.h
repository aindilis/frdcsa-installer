//
// File:         AutoFunctorPrecedence.h
// Description:  Automatic assignment of precedences to the functors and predicates of the signature.
// Created:      Apr 24, 2001, Alexandre Riazanov, riazanov@cs.man.ac.uk
// Revised:      May 20, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//               Bug fix in
//               headerPrecedence(bool negative,ulong predNum,long& precedence)
//====================================================
#ifndef AUTO_FUNCTOR_PRECEDENCE_H
//=================================================
#define AUTO_FUNCTOR_PRECEDENCE_H
#ifndef NO_DEBUG_VIS
 #include <iostream.h>
#endif
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "VampireKernelConst.h"
#include "Array.h"
#include "GlobAlloc.h"
#include "Tuple.h"
#include "GSortTree.h"
//=================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_AUTO_FUNCTOR_PRECEDENCE
 #define DEBUG_NAMESPACE "AutoFunctorPrecedence"
#endif 
#include "debugMacros.h"
//=================================================  

namespace VK
{
  using namespace BK;
class AutoFunctorPrecedence
{
 public: 
  class InitStatic
  {
   public: 
    InitStatic()
    { 
     if (!_count) 
      {
       _count++;
      };  
    };
    ~InitStatic() {};
   private:
    static long _count;   
  }; // class InitStatic   

 public:
  AutoFunctorPrecedence();
  ~AutoFunctorPrecedence();
    
  void setDependenceOnArity(long typeOfDependence) { _dependenceOnArity = typeOfDependence; };

  void setInversePolarityFlag(bool flag) { _inversePolarity = flag; };

  void setKinkyHeaderPrecedenceFlag(bool flag) { _kinkyHeaderPrecedence = flag; };

  bool precedenceIsFixed(ulong funNum) { return _precedenceIsFixed.sub(funNum); };  


  bool precedenceIsReserved(long precedence,ulong& funNum);

  void assignFixed(ulong funNum,long precedence);

  void assignMaximalPrecedence(ulong funNum);

  static long maximalPrecedence()
    {
      return LONG_MAX/8;
    };

  void assignArity(ulong funNum,ulong arity);
  
  void assignAllPrecedences(); 

  bool symbolPrecedence(ulong funNum,long& precedence)
  {  
    if (!_registeredFunctor.sub(funNum)) return false;
    precedence = _precedence.sub(funNum);
    return true; 
  };
  
  bool headerPrecedence(bool negative,ulong predNum,long& precedence) 
  {
   CALL("headerPrecedence(bool negative,ulong predNum,long& precedence)"); 
   if (!_registeredFunctor.sub(predNum)) return false;

   if (VampireKernelConst::isEquality(predNum)) 
     {
       long prec;
       ALWAYS(symbolPrecedence(predNum,prec));
       if (negative) { precedence = prec + 1L; } else precedence = prec;
       return true;
     };

   // not an equality
   if (_inversePolarity) negative = !negative;
   if (negative)
    {
     long prec;
     ALWAYS(symbolPrecedence(predNum,prec));
     if (_kinkyHeaderPrecedence) 
       {
	 precedence = -prec; 
       }
     else
       precedence = prec + 1;
     return true;
    }
   else 
     return symbolPrecedence(predNum,precedence);
  };


 private: 
  typedef Array<GlobAlloc,bool,128UL,AutoFunctorPrecedence> BoolArray;
  typedef Array<GlobAlloc,long,128UL,AutoFunctorPrecedence> LongArray;
  typedef Array<GlobAlloc,ulong,128UL,AutoFunctorPrecedence> ULongArray;
  typedef Tuple2<ulong,ulong> ArityFuncPair; 
  typedef GSortTree<GlobAlloc,ArityFuncPair,AutoFunctorPrecedence> ArityFuncSortTree;  
 private: 
  long normalisedPrecedence(long inputPrecedence) { return inputPrecedence*2; };
  long previousNormalisedPrecedence(long normalisedPrecedence) { return normalisedPrecedence - 2; };
  void assignSmallerPrecedence(ulong funNum); 
  static long abs(long num) { if (num >= 0) return num; return -num; }; 
  static long max(long x,long y) { if (x > y) return x; return y; };
 private: 
  long _dependenceOnArity;
  bool _inversePolarity;
  bool _kinkyHeaderPrecedence;
  long _somePrecedenceSet;
  long _minimalFixedPrecedence;
  long _maximalFixedPrecedence;
  ulong _minFunctorNumber;
  ulong _maxFunctorNumber;
  LongArray _precedence; 
  BoolArray _precedenceIsFixed;
  ULongArray _arity;
  BoolArray _registeredFunctor;
}; // class AutoFunctorPrecedence


static AutoFunctorPrecedence::InitStatic autoFunctorPrecedenceInitStatic;
}; // namespace VK

//================================================
#undef DEBUG_NAMESPACE   
//==================================================
#endif
