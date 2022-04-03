//
// File:         BuiltInTheory.h
// Description:  Built-in theory abstraction.  
// Created:      Jul 06, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//==================================================================
#ifndef BUILT_IN_THEORY_H
#define BUILT_IN_THEORY_H
//==================================================================
#include <stddef.h>
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_BUILT_IN_THEORY
 #define DEBUG_NAMESPACE "BuiltInTheory"
#endif
#include "debugMacros.h"
//===================================================================

namespace VK
{
  class Clause;
  class TMP_LITERAL;
  class BuiltInTheory
    {
    public:
      BuiltInTheory(const char* name) : _name(name)
	{
	};
      const char* name() { return _name; };
      virtual ~BuiltInTheory();
      virtual bool interpret(ulong inputSymbolId,
			     const char* nativeSymbol) = 0;
      virtual Clause* simplify(TMP_LITERAL* lit,TMP_LITERAL* newLit) = 0;
    private:
      void* operator new(size_t);
      void operator delete(void* obj);
    protected:
      BuiltInTheory() : _name((const char*)0) {};
      
    private:
      const char* _name;
    }; // class BuiltInTheory

}; // namespace VK


//=================================================================
#endif
