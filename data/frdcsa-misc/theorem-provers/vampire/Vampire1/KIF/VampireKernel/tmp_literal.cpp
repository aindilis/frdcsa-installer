//
// File:         tmp_literal.cpp
// Description:  Temporary (buffer) literals.
// Created:      Dec 21, 2001 
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//============================================================================
#include "tmp_literal.h"
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_NEW_CLAUSE
 #define DEBUG_NAMESPACE "TMP_LITERAL"
#endif 
#include "debugMacros.h"
//=================================================

namespace VK
{

long TMP_LITERAL::InitStatic::_count; // implicitely initialised to 0L
TMP_LITERAL* TMP_LITERAL::_freeLiterals;
void TMP_LITERAL::initStatic() { _freeLiterals = (TMP_LITERAL*)0; };

void TMP_LITERAL::destroyStatic() 
{      
  CALL("destroyStatic()");
  if (DestructionMode::isThorough())
    {
      while (_freeLiterals)
	{
	  TMP_LITERAL* tmp = _freeLiterals;
	  _freeLiterals = _freeLiterals->_nextFree;
	  delete tmp;
	};
    }
  else
   _freeLiterals = (TMP_LITERAL*)0; 
}; // void TMP_LITERAL::destroyStatic()



ostream& TMP_LITERAL::output(ostream& str) const
{
  if (_inherentlySelected) str << "[[InhSel]] ";
  if (isEquality())
    { 
      str << '(';
      arg1()->outputTerm(str) << ')' << header() << '(';
      arg2()->outputTerm(str);
      str << ')';
    }
  else 
    flatterm()->outputTerm(str);
  return str; 
}; // ostream& TMP_LITERAL::output(ostream& str) const


void* TMP_LITERAL::operator new(size_t)
{
  CALL("operator new(size_t)");
  void* res = (void*)GlobAlloc::allocate(sizeof(TMP_LITERAL));
  return res;
}; // void* TMP_LITERAL::operator new(size_t)

void TMP_LITERAL::operator delete(void* obj,size_t)
{
  CALL("operator delete(void*,size_t)");
  GlobAlloc::deallocate(obj,sizeof(TMP_LITERAL));
}; // void TMP_LITERAL::operator delete(void*,size_t)


bool TMP_LITERAL::instantiateVar(TMP_LITERAL& new_lit,ulong var,Flatterm* subst_term)
{
  CALL("instantiateVar(TMP_LITERAL& new_lit,ulong var,Flatterm* subst_term)");
  TERM new_header = header();
  ASSERT(new_header.arity());
  if(new_header.IsEquality()) new_header.MakeUnordered();
  new_lit.reset();
  new_lit.pushNonpropHeader(new_header);
  Flatterm::Iterator iter(flatterm());
  Flatterm::Iterator subst_iter;
  iter.NextSym(); // skipping the header
  while (iter.NotEndOfTerm())
    {
      TERM sym = iter.CurrPos()->Symbol();  
      if (sym.isVariable() && sym.var() == var) 
	{
	  subst_iter.Reset(subst_term);             
	  do
	    {
	      if (!new_lit.pushSym(((Flatterm*)(subst_iter.CurrPos()))->symbolRef())) return false;             
	      subst_iter.NextSym();
	    }
	  while (subst_iter.NotEndOfTerm());
	}
      else if (!new_lit.pushSym(sym)) return false;
      iter.NextSym();
    };
  new_lit.endOfLiteral();
  return true;
}; // bool TMP_LITERAL::instantiateVar(TMP_LITERAL& new_lit,ulong var,Flatterm* subst_term)

}; // namespace VK

//============================================================================
