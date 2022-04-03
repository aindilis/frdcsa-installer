//
// File:         BuiltInTheoryDispatcher.h
// Description:  Built-in theory abstraction.  
// Created:      Jul 06, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//==================================================================
#ifndef BUILT_IN_THEORY_DISPATCHER_H
#define BUILT_IN_THEORY_DISPATCHER_H
//==================================================================
#include "jargon.h"
#include "GList.h"
#include "GlobAlloc.h"
#include "VampireKernelDebugFlags.h"
#include "Clause.h"
//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_BUILT_IN_THEORY_DISPATCHER
 #define DEBUG_NAMESPACE "BuiltInTheoryDispatcher"
#endif
#include "debugMacros.h"
//===================================================================

namespace VK
{
  class TMP_LITERAL;
  class BuiltInFloatingPointArithmetic;
  class BuiltInTheory;
  class BuiltInTheoryDispatcher
    {
    public:
      BuiltInTheoryDispatcher(Clause::NewNumber* clauseNumberGenerator);
      ~BuiltInTheoryDispatcher();
      void* operator new(size_t);
      void operator delete(void* obj);
      static BuiltInTheoryDispatcher* current() { return _current; };
      static void setCurrent(BuiltInTheoryDispatcher* td)
	{
	  _current = td;
	};
      bool interpret(ulong inputSymbolId,
		     const char* theoryName,
		     const char* nativeSymbol);
      Clause* simplify(TMP_LITERAL* lit,TMP_LITERAL* newLit);
    private:
      BuiltInTheoryDispatcher();
      BuiltInTheory* theory(const char* theoryName);
    private:
      Clause::NewNumber* _clauseNumberGenerator;
      BuiltInFloatingPointArithmetic* _floatingPointArithmetic;      
      static BuiltInTheoryDispatcher* _current;
    }; // class BuiltInTheoryDispatcher

}; // namespace VK


//=================================================================
#endif
