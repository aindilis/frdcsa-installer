#include "prefix.h"
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_PREFIX
#define DEBUG_NAMESPACE "PREFIX"
#endif
#include "debugMacros.h"
//================================================= 

namespace VK
{
#ifndef NO_DEBUG_VIS
ostream& PREFIX::outputWord(ostream& str,const PREFIX_SYM* word)
{
  SIMPLE_ITER w_iter;
  w_iter.Reset((PREFIX_SYM*)word);
  while (!(w_iter.Empty()))
    {
      str << '<' << (unsigned long)w_iter.CurrPos() << "> ";
      w_iter.CurrPos()->output(str) << '\n';
      w_iter.SkipSym();
    };     
  return str;
}; // ostream& PREFIX::outputWord(ostream& str,const PREFIX_SYM* word)  

ostream& PREFIX::outputWordWithSubst(ostream& str,const PREFIX_SYM* word)
{
  ITER w_iter;
  w_iter.Reset((PREFIX_SYM*)word);
  while (!(w_iter.Empty()))
    {
      if (w_iter.CurrVar())
	{
	  w_iter.CurrVar()->output(str);
	}
      else w_iter.CurrSubterm()->output(str);
      str << '\n';
      w_iter.SkipSym();
    };
  return str;
}; // ostream& PREFIX::outputWordWithSubst(ostream& str,const PREFIX_SYM* word)
  
ostream& PREFIX::outputTerm(ostream& str,const PREFIX_SYM* word)
{
  if (word->Head().isVariable()) return word->Head().output(str);
  SIMPLE_ITER w_iter;
 
  ExpandingBracketCounter bracketCounter; // temporary solution, can not make it static now
  bracketCounter.reset();    

  w_iter.Reset((PREFIX_SYM*)word);
  while (!(w_iter.Empty()))
    {
      w_iter.CurrPos()->Head().output(str);
      if ((w_iter.CurrPos()->Head().IsFunctor()) 
	  && (w_iter.CurrPos()->Head().Func().arity()))
	{
	  bracketCounter.openSafe(w_iter.CurrPos()->Head().Func().arity());
	  str << '(';
	  w_iter.SkipSym();
	}
      else
	{
	  ulong closed = bracketCounter.close();
	  while (closed) { str << ')';  closed--; };
	  w_iter.SkipSym();
	  if (!(w_iter.Empty())) str << ',';
	};
    };       
  return str;
}; // ostream& PREFIX::outputTerm(ostream& str,const PREFIX_SYM* word)
#endif

}; // namespace VK



//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_PREFIX
#define DEBUG_NAMESPACE "(PREFIX)"
#endif
#include "debugMacros.h"
//=================================================  


namespace VK
{

void StandardTermToPrefix(ulong index,TERM* t,PREFIX_SYM*& memory)
  // t must be a complex term here
{
  CALL("StandardTermToPrefix(ulong index,TERM* t,PREFIX_SYM*& memory)");

  //DF; cout << "STT -> PREF\n" << t->Content() << " flag = " << (ulong)(t->Flag()) 
  //    << "  reserved = " << t->Reserved() << "\n";
  DOP(PREFIX_SYM* debugMemory = memory);
  ASSERT(t->isComplex());
  TERM::Iterator t_iter;
  BracketCounter<VampireKernelConst::MaxTermDepth> brack_counter;
  PREFIX_SYM* left_bracket[VampireKernelConst::MaxTermSize];
  PREFIX_SYM** next_left_bracket = left_bracket;
  t_iter.Reset(t);
  TERM sym;
  unsigned closed;
  unsigned var_offset = Unifier::variableOffset(index);
 next_sym:
  sym = t_iter.Symbol();
  if (sym.isVariable())
    {
      memory->Head().MkVar(Unifier::current()->variableBase(sym.var())+var_offset);
      memory->SetBracket(memory+1);
      closed = brack_counter.Close();    
      while (closed)
	{
	  next_left_bracket--;
	  (*next_left_bracket)->SetBracket(memory+1);
	  closed--;
	};
    }
  else // functor
    {
      memory->Head().MkFunc(sym);
      if (sym.arity())
	{
	  brack_counter.Open(sym.arity());
	  *next_left_bracket = memory;
	  next_left_bracket++;
	}
      else // constant
	{
	  memory->SetBracket(memory+1);
	  unsigned closed = brack_counter.Close();    
	  while (closed)
	    {
	      next_left_bracket--;
	      (*next_left_bracket)->SetBracket(memory+1);
	      closed--;
	    };  
	};
    };
  memory++;
  if (t_iter.Next())
    {
      goto next_sym;
    };
  
  ASSERT(next_left_bracket == left_bracket); 
  ASSERT(PREFIX::CorrectComplexTerm(debugMemory)); 
}; // void StandardTermToPrefix(ulong index,TERM* t,PREFIX_SYM*& memory)
 


}; // namespace VK


//================================================= 
