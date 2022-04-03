//
// File:         BuiltInTheoryDispatcher.cpp
// Description:  Built-in theory abstraction.  
// Created:      Jul 06, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//==================================================================
#include <string.h>
#include "GlobAlloc.h"
#include "BuiltInTheoryDispatcher.h"
#include "BuiltInFloatingPointArithmetic.h"
//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_BUILT_IN_THEORY_DISPATCHER
 #define DEBUG_NAMESPACE "BuiltInTheoryDispatcher"
#endif
#include "debugMacros.h"
//===================================================================

using namespace VK;
using namespace BK;

BuiltInTheoryDispatcher* BuiltInTheoryDispatcher::_current;

BuiltInTheoryDispatcher::BuiltInTheoryDispatcher(Clause::NewNumber* clauseNumberGenerator) :
  _clauseNumberGenerator(clauseNumberGenerator),
  _floatingPointArithmetic((BuiltInFloatingPointArithmetic*)0)
{
};

BuiltInTheoryDispatcher::BuiltInTheoryDispatcher() 
{
};


BuiltInTheoryDispatcher::~BuiltInTheoryDispatcher()
{
  CALL("destructor ~BuiltInTheoryDispatcher()");
  if (_floatingPointArithmetic) delete _floatingPointArithmetic;
};


void* BuiltInTheoryDispatcher::operator new(size_t)
{
  CALL("operator new(size_t)");
  return GlobAlloc::allocate(sizeof(BuiltInTheoryDispatcher));
}; // void* BuiltInTheoryDispatcher::operator new(size_t)

void BuiltInTheoryDispatcher::operator delete(void* obj)
{
  CALL("operator delete(void* obj)");
  GlobAlloc::deallocate(obj,sizeof(BuiltInTheoryDispatcher));
}; // void BuiltInTheoryDispatcher::operator delete(void* obj)

bool BuiltInTheoryDispatcher::interpret(ulong inputSymbolId,
					const char* theoryName,
					const char* nativeSymbol)
{
  CALL("interpret(..)");
  BuiltInTheory* th = theory(theoryName);
  if (!th) return false;
  if (!th->interpret(inputSymbolId,nativeSymbol)) return false;
  return true;
}; // bool BuiltInTheoryDispatcher::interpret(ulong inputSymbolId,onst char* theoryName,const char* nativeSymbol)



Clause* BuiltInTheoryDispatcher::simplify(TMP_LITERAL* lit,TMP_LITERAL* newLit)
{
  CALL("simplify(TMP_LITERAL* lit,TMP_LITERAL* newLit)");  
  Clause* result = (Clause*)0;
  if (_floatingPointArithmetic)
    result = _floatingPointArithmetic->simplify(lit,newLit);
  if (result) return result;
  return (Clause*)0;
}; // Clause* BuiltInTheoryDispatcher::simplify(TMP_LITERAL* lit,TMP_LITERAL* newLit)


      
BuiltInTheory* BuiltInTheoryDispatcher::theory(const char* theoryName)
{
  CALL("theory(const char* theoryName)");
  if (!strcmp(theoryName,"floating_point_arithmetic"))
    {
      if (!_floatingPointArithmetic) 
	{
	  _floatingPointArithmetic = 
	    new BuiltInFloatingPointArithmetic("floating_point_arithmetic",_clauseNumberGenerator);
	};
      return _floatingPointArithmetic;
    };
  return (BuiltInTheory*)0; // no theory with this name
}; // BuiltInTheory* BuiltInTheoryDispatcher::theory(const char* theoryName)



//==================================================================
