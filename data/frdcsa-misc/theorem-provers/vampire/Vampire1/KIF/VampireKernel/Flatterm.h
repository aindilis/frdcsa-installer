//
// File:         Flatterm.h
// Description:  Flatterms.
// Created:      Feb 13, 2000, 20:50
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//====================================================
#ifndef FLATTERM_H
//=================================================================
#define FLATTERM_H
#include <iostream.h>
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "VampireKernelConst.h"
#include "Term.h"
#include "MultisetOfVariables.h"
#include "variables.h"
#include "Stack.h"
namespace VK
{
  class Clause;
}; // namespace VK

//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FLATTERM
#define DEBUG_NAMESPACE "Flatterm"
#endif
#include "debugMacros.h"
//=================================================

namespace VK
{
  using namespace BK;
  class Flatterm    
    {
    private:
      const Flatterm* _after;
      TERM symbol;
    public:
      Flatterm() {};
      ~Flatterm() {};
      TERM Symbol() const { return symbol; };
      TERM& symbolRef() { return symbol; };
      const TERM& symbolRef() const { return symbol; };
      bool isVariable() const { return symbol.isVariable(); };
      bool isComplex() const 
	{
	  CALL("isComplex() const");
	  return symbol.isComplex(); 
	};
      ulong Var() const 
	{
	  CALL("Var() const");
	  ASSERT(isVariable());
	  return symbol.var(); 
	};
      ulong functor() const 
	{ 
	  CALL("functor() const");
	  ASSERT(isComplex());
	  return symbol.functor(); 
	};
      ulong arity() const { return symbol.arity(); };
      void SetSymbol(const TERM& sym) { symbol = sym; };
      void SetVar(ulong var) { symbol.MakeVar(var); _after = this + 1; };
      void SetVar(const TERM& var) { symbol = var; _after = this + 1; };
      void SetConst(const TERM& csym) { symbol = csym; _after = this + 1; }; 
      const Flatterm* after() const { return _after; };
      Flatterm* after() { return (Flatterm*)_after; };
      const Flatterm* next() const { return (this + 1); }; 
      Flatterm* next() { return (this + 1); };
      const Flatterm* previous() const { return (this - 1); }; 
      Flatterm* previous() { return (this - 1); };
      const Flatterm* lastSymbol() const { return after() - 1; };
      Flatterm* lastSymbol() { return after() - 1; };
      void SetNextTerm(const Flatterm* nt) { _after = nt; };
      void ShiftNextTerm(const long& shift) { _after += shift; };
      //long Weight() const { return (_after - this); };
      long size() const { return (_after - this); };
      unsigned long Depth() const;
      bool Nonground() const;
      bool containsSymbol(const TERM& sym) const;
      bool equal(const Flatterm* t) const;
      bool equal(const Flatterm* t,const Clause* const * commutativity_table) const;
      bool EqualArguments(const Flatterm* t) const;
      bool EqualArguments(const Flatterm* t,const Clause* const * commutativity_table) const;
      bool Complementary(const Flatterm* t) const;
      bool Complementary(const Flatterm* t,const Clause* const * commutativity_table) const;
      Comparison CompareLex(const Flatterm* t) const;
      Comparison CompareAtomsLex(const Flatterm* t) const; 
      static Comparison KBCompareAtoms(const Flatterm* t1,MultisetOfVariables<VampireKernelConst::MaxNumOfVariables>* vars1,const Flatterm* t2,MultisetOfVariables<VampireKernelConst::MaxNumOfVariables>* vars2);
      static Comparison KBCompare(const Flatterm* t1,MultisetOfVariables<VampireKernelConst::MaxNumOfVariables>* vars1,const Flatterm* t2,MultisetOfVariables<VampireKernelConst::MaxNumOfVariables>* vars2);
      void renameVariables(INT_VAR_RENAMING& var_renaming);
      bool IsBackJump() const { return _after < this; };
      void SetBackJump(const Flatterm* jmp) { _after = jmp; };
      static void CopyWithShift(const Flatterm* org,Flatterm* dest,const long& shift);
      void BinSwapArgs();   
    public: // iterators
      class Iterator;
      class Open;
      class OpenWithMemory;
    public:
#ifndef NO_DEBUG
      bool CheckIntegrity() { return (after() > this); };
#endif
      ostream& output(ostream& str) const { return str << symbol; };
      ostream& outputTerm(ostream& str) const;
    }; // class Flatterm
}; // namespace VK

inline ostream& operator<<(ostream& str,const VK::Flatterm* t) 
{    
  return t->outputTerm(str);
};


//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FLATTERM
#define DEBUG_NAMESPACE "Flatterm::Open"
#endif
#include "debugMacros.h"
//=================================================
namespace VK
{
  using namespace BK;
  class Flatterm::Open
    {
    private:
      Flatterm* curr_pos;
      Stack<Flatterm*,VampireKernelConst::MaxTermSize> open_parentheses;
      BracketCounter<VampireKernelConst::MaxTermDepth> bracket_counter;
      DepthCounter depth_counter;
      DOP(Flatterm* debug_fst_pos);
    private: // auxilliary methods
      void CloseNParentheses(unsigned n)
	{
	  depth_counter.Up(n);
	  while (n)
	    {
	      open_parentheses.pop()->SetNextTerm(curr_pos);
	      n--;
	    };
	}; 
    public:
      Open() 
	: open_parentheses("open_parentheses"),
	bracket_counter(),
	depth_counter()
	{};
      Open(Flatterm* mem) 
	: open_parentheses("open_parentheses"),
	bracket_counter()
	{ 
	  Reset(mem);
	};
      ~Open() {};
      void Reset(Flatterm* mem) 
	{
	  CALL("Reset(Flatterm* mem)");
	  DOP(debug_fst_pos = mem);
	  curr_pos = mem; 
	  open_parentheses.reset();
	  bracket_counter.Reset();
	  depth_counter.Reset(1UL);
	};

      unsigned long Depth() const { return depth_counter.Depth(); };
      void PushVar(ulong var)
	{
	  CALL("PushVar(ulong var)");
	  ASSERT(curr_pos - debug_fst_pos < (long)VampireKernelConst::MaxTermSize);
	  curr_pos->SetVar(var);
	  curr_pos++;
	  CloseNParentheses(bracket_counter.Close());
	}; 
    
      void PushVar(const TERM& var)
	{
	  CALL("PushVar(const TERM& var)");
	  ASSERT(curr_pos - debug_fst_pos < (long)VampireKernelConst::MaxTermSize);
	  curr_pos->SetVar(var);
	  curr_pos++;
	  CloseNParentheses(bracket_counter.Close());
	};
      void PushVar(const TERM& var,ulong varOffset)
	{
	  CALL("PushVar(const TERM& var,ulong varOffset)");
	  ASSERT(curr_pos - debug_fst_pos < (long)VampireKernelConst::MaxTermSize);
	  curr_pos->SetVar(var.var() + varOffset);
	  curr_pos++;
	  CloseNParentheses(bracket_counter.Close());
	}; 
      void PushConst(const TERM& csym)
	{
	  CALL("PushConst(const TERM& csym)");     
	  ASSERT(curr_pos - debug_fst_pos < (long)VampireKernelConst::MaxTermSize);
	  curr_pos->SetConst(csym);
	  curr_pos++;
	  CloseNParentheses(bracket_counter.Close());
	}; 
      void PushNonconstFun(const TERM& fsym)
	{
	  CALL("PushNonconstFun(const TERM& fsym)");
	  ASSERT(curr_pos - debug_fst_pos < (long)VampireKernelConst::MaxTermSize);
	  curr_pos->SetSymbol(fsym);
	  // fsym.arity() > 0 here
	  bracket_counter.Open(fsym.arity());
	  open_parentheses.push(curr_pos);
	  depth_counter.Down();
	  curr_pos++;
	}; 
      void Push(const TERM& sym) 
	{
	  if (sym.isVariable()) { PushVar(sym); }
	  else if (sym.arity()) { PushNonconstFun(sym); }
	  else PushConst(sym);
	};
      void Push(const TERM& sym,ulong varOffset)
	{
	  if (sym.isVariable()) { PushVar(sym,varOffset); }
	  else if (sym.arity()) { PushNonconstFun(sym); }
	  else PushConst(sym);
	};
      Flatterm* CurrPos() const { return curr_pos; };
   
      void PushStandardComplexTerm(const TERM* t,ulong varOffset)
	{ 
	  CALL("PushStandardComplexTerm(const TERM* t,ulong varOffset)");
	  ASSERT(t->isComplex());
	  TERM::Iterator iter(t);  
	  do
	    {    
	      Push(iter.symbolRef(),varOffset);
	    }
	  while (iter.Next()); 
	};
    }; // class Flatterm::Open

}; // namespace VK
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FLATTERM
#define DEBUG_NAMESPACE "Flatterm::OpenWithMemory"
#endif
#include "debugMacros.h"
//=================================================
namespace VK
{
  using namespace BK;
  class Flatterm::OpenWithMemory : public Flatterm::Open
    {
    private:
      Flatterm memory[VampireKernelConst::MaxTermSize+2];
    public:
      OpenWithMemory() : Open(memory) {};
      void Reset() { Open::Reset(memory); };
      const Flatterm* Term() const { return memory; }; 
      Flatterm* Term() { return memory; }; 
      long currentSize() const { return CurrPos() - memory; };
    }; // class Flatterm::OpenWithMemory : public Flatterm::Open

}; // namespace VK
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FLATTERM
#define DEBUG_NAMESPACE "Flatterm::Iterator"
#endif
#include "debugMacros.h"
//=================================================

namespace VK
{
  using namespace BK;

  class Flatterm::Iterator
    {
    private:
      const Flatterm* curr_pos;
      const Flatterm* end_of_term;
    public:
      Iterator() {};
      Iterator(const Flatterm* t) { Reset(t); };
      void Reset(const Flatterm* t)
	{
	  curr_pos = t;
	  end_of_term = t->after();
	};
      const Flatterm* CurrPos() const { return curr_pos; };
      bool EndOfTerm() const { return (curr_pos == end_of_term); };
      bool NotEndOfTerm() const { return (curr_pos < end_of_term); };
      void NextSym() { curr_pos++; };
      void after() { curr_pos = curr_pos->after(); }; 
    }; // class Flatterm::Iterator

}; // namespace VK
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FLATTERM
#define DEBUG_NAMESPACE "Flatterm"
#endif
#include "debugMacros.h"
//=================================================
namespace VK
{
  using namespace BK;

  inline unsigned long Flatterm::Depth() const 
    {
      CALL("Depth()");
      if (!(Symbol().arity())) return 1UL;
      Iterator iter(this);
      SmartDepthCounter<VampireKernelConst::MaxTermDepth> depth_counter;
      depth_counter.Reset(1UL);
      unsigned long depth = 0UL;
      do
	{
	  depth_counter.NextSym(iter.CurrPos()->symbolRef());  
	  if (depth_counter.Depth() > depth) 
	    {
	      depth = depth_counter.Depth();
	      ASSERT(depth <= VampireKernelConst::MaxTermDepth);
	    };
   
	  iter.NextSym();
	}  
      while (iter.NotEndOfTerm());
      return depth;
    }; // unsigned long Flatterm::Depth()

  inline bool Flatterm::Nonground() const 
    {
      CALL("Nonground() const");
      Iterator iter(this);
      do
	{
	  if (iter.CurrPos()->symbolRef().isVariable()) return true;
	  iter.NextSym();
	}
      while (iter.NotEndOfTerm());
      return false;
    }; // bool Flatterm::Nonground() const 

  inline bool Flatterm::containsSymbol(const TERM& sym) const 
    {
      CALL("containsSymbol(const TERM& sym) const");
      Iterator iter(this);
      do
	{
	  if (iter.CurrPos()->symbolRef() == sym) return true;
	  iter.NextSym();
	}
      while (iter.NotEndOfTerm());
      return false;

    }; // bool Flatterm::containsSymbol(const TERM& sym) const


  /*
    inline bool Flatterm::equal(const Flatterm* t) const
    {
    // can be applied to variables
    if (Symbol() != t->Symbol()) return false;
    const Flatterm* sym = this + 1;
    if (sym != _after)
    {
    t++;
    do
    {
    if (sym->Symbol() != t->Symbol()) return false;
    sym++;
    t++;
    }
    while (sym != _after);
    };  

    return true; 
    }; // bool Flatterm::equal(const Flatterm* t) const
  */

  inline bool Flatterm::equal(const Flatterm* t) const
    {
      CALL("equal(const Flatterm* t) const");
      // experimental version  

      // can be applied to variables
      if (symbolRef() != t->symbolRef()) return false; 
      register ulong n = size();
      n--;
      while (n)
	{
	  if ((this[n].symbolRef()) != (t[n].symbolRef())) return false;   
	  n--;  
	};
      return true;
    }; // bool Flatterm::equal(const Flatterm* t) const


  inline bool Flatterm::equal(const Flatterm* t,const Clause* const * commutativity_table) const
    {
      CALL("equal(const Flatterm* t,const Clause* const * commutativity_table) const");
      if (size() != t->size()) return false;

      Iterator iter1(this);

      Iterator iter2(t);
      const Flatterm* arg11;
      const Flatterm* arg21;
      const Flatterm* arg12;
      const Flatterm* arg22;

      do
	{
	  if (iter1.CurrPos()->symbolRef() != iter2.CurrPos()->symbolRef()) return false;

	  if ((iter1.CurrPos()->isComplex()) && (commutativity_table[iter1.CurrPos()->functor()]))
	    {    
	      // recursion     
	      arg11 = iter1.CurrPos() + 1;
	      arg21 = iter2.CurrPos() + 1; 
	      if (arg11->equal(arg21,commutativity_table))
		{      
		  COP("equal 1st args");
		  iter1.NextSym();
		  iter1.after();
		  iter2.NextSym();
		  iter2.after();      
		}
	      else // try to swap arguments
		{
		  arg12 = arg11->after();
		  arg22 = arg21->after();
		  if (arg12->equal(arg21,commutativity_table)
		      && arg22->equal(arg11,commutativity_table))
		    {
		      iter1.after();         
		      iter2.after();
		    }
		  else return false;
		};
	    }
	  else
	    {
	      iter1.NextSym();
	      iter2.NextSym();
	    };
	}
      while (iter1.NotEndOfTerm());
      return true;
    }; // bool Flatterm::equal(const Flatterm* t,const Clause* const * commutativity_table) const


  inline bool Flatterm::EqualArguments(const Flatterm* t) const
    {
      CALL("EqualArguments(const Flatterm* t) const");
      const Flatterm* sym1 = this + 1;
      const Flatterm* sym2 = t + 1;
      while (sym1 != _after)
	{
	  if (sym1->Symbol() != sym2->Symbol()) return false;
	  sym1++;
	  sym2++;
	};
      return true; 
    }; // bool Flatterm::EqualArguments(const Flatterm* t) const

  inline bool Flatterm::EqualArguments(const Flatterm* t,const Clause* const * commutativity_table) const
    {
      CALL("EqualArguments(const Flatterm* t,const Clause* const * commutativity_table) const");
      Iterator iter1(this);
      Iterator iter2(t);
      iter1.NextSym();
      iter2.NextSym();
      while (iter1.NotEndOfTerm())
	{
	  if (!iter1.CurrPos()->equal(iter2.CurrPos(),commutativity_table)) return false;
	  iter1.after();
	  iter2.after();
	};
      return true;

      const Flatterm* sym1 = this + 1;
      const Flatterm* sym2 = t + 1;
      while (sym1 != _after)
	{
	  if (sym1->Symbol() != sym2->Symbol()) return false;
	  sym1++;
	  sym2++;
	};
      return true; 
    }; // bool Flatterm::EqualArguments(const Flatterm* t,const Clause* const * commutativity_table) const

  inline bool Flatterm::Complementary(const Flatterm* t) const
    {
      return (Symbol().ComplementaryHeader(t->Symbol()))
	&& (EqualArguments(t));
    }; // bool Flatterm::Complementary(const Flatterm* t) const

  inline bool Flatterm::Complementary(const Flatterm* t,const Clause* const * commutativity_table) const
    {
      return (Symbol().ComplementaryHeader(t->Symbol()))
	&& (EqualArguments(t,commutativity_table));
    }; // bool Flatterm::Complementary(const Flatterm* t,const Clause* const * commutativity_table) const


  inline Comparison Flatterm::CompareLex(const Flatterm* t) const
    {
      CALL("CompareLex(const Flatterm* t) const");
      const Flatterm* sym1 = this;
      const Flatterm* sym2 = t;
  
      while (sym1 != _after)
	{

	  if (sym1->Symbol() != sym2->Symbol())
	    {
	      if ((sym1->Symbol().isVariable()) || (sym2->Symbol().isVariable()))
		{
		  return Incomparable;
		}
	      else
		{
		  return sym1->Symbol().comparePrecedence(sym2->Symbol());
		}; 
	    };
	  sym1++;
	  sym2++;
	};


      return Equal; 
    }; // Comparison Flatterm::CompareLex(const Flatterm* t) const
    

  inline Comparison Flatterm::KBCompare(const Flatterm* t1,MultisetOfVariables<VampireKernelConst::MaxNumOfVariables>* vars1,
					const Flatterm* t2,MultisetOfVariables<VampireKernelConst::MaxNumOfVariables>* vars2)
    {
      CALL("KBCompare(const Flatterm* t1,MultisetOfVariables<VampireKernelConst::MaxNumOfVariables>* vars1,const Flatterm* t2,MultisetOfVariables<VampireKernelConst::MaxNumOfVariables>* vars2)");

      if (t1->size() == t2->size())         
	{
	  if (vars1->equal(*vars2)) return t1->CompareLex(t2);
	  if (vars1->GreaterOrEqual(*vars2)) 
	    {
	      if (t1->CompareLex(t2) == Greater) 
		{
		  return  Greater; 
		};
	      return Incomparable;            
	    }
	  else
	    if (vars2->GreaterOrEqual(*vars1))
	      {
		if (t1->CompareLex(t2) == Less)      
		  {     
		    return Less;
		  };
		return Incomparable;
	      };    
	  return Incomparable;  
	};
      // different weights 
      if (t1->size() > t2->size())
	{
	  return vars1->GreaterOrEqual(*vars2) ? Greater : Incomparable;
	}
      else // t1->size() < t2->size()
	{
	  return vars2->GreaterOrEqual(*vars1) ? Less : Incomparable;
	};
    }; // Comparison Flatterm::KBCompare(const Flatterm* t1,MultisetOfVariables<VampireKernelConst::MaxNumOfVariables>* vars1,...





  inline Comparison Flatterm::CompareAtomsLex(const Flatterm* t) const
    {
      CALL("CompareAtomsLex(const Flatterm* t) const");
      const Flatterm* sym1 = this;
      const Flatterm* sym2 = t;
      if (sym1->Symbol().functor() != sym2->Symbol().functor())  
	return sym1->Symbol().comparePrecedence(sym2->Symbol());
      sym1++;
      sym2++;
      while (sym1 != _after)
	{

	  if (sym1->Symbol() != sym2->Symbol())
	    {
	      if ((sym1->Symbol().isVariable()) || (sym2->Symbol().isVariable()))
		{
		  return Incomparable;
		}
	      else
		{
		  return sym1->Symbol().comparePrecedence(sym2->Symbol());
		}; 
	    };
	  sym1++;
	  sym2++;
	};

      return Equal; 
    }; // Comparison Flatterm::CompareAtomsLex(const Flatterm* t) const


  inline Comparison Flatterm::KBCompareAtoms(const Flatterm* t1,MultisetOfVariables<VampireKernelConst::MaxNumOfVariables>* vars1,
					     const Flatterm* t2,MultisetOfVariables<VampireKernelConst::MaxNumOfVariables>* vars2)
    {
      CALL("KBCompareAtoms(const Flatterm* t1,MultisetOfVariables<VampireKernelConst::MaxNumOfVariables>* vars1,const Flatterm* t2,MultisetOfVariables<VampireKernelConst::MaxNumOfVariables>* vars2)");

      if (t1->size() == t2->size())         
	{
	  if (vars1->equal(*vars2)) return t1->CompareAtomsLex(t2);
	  if (vars1->GreaterOrEqual(*vars2)) 
	    {
	      if (t1->CompareAtomsLex(t2) == Greater) 
		{
		  return Greater; 
		};
	      return Incomparable;            
	    }
	  else
	    if (vars2->GreaterOrEqual(*vars1))
	      {
		if (t1->CompareAtomsLex(t2) == Less)      
		  {     
		    return Less;
		  };
		return Incomparable;
	      };    
	  return Incomparable;  
	};
      // different weights 
      if (t1->size() > t2->size())
	{
	  return vars1->GreaterOrEqual(*vars2) ? Greater : Incomparable;
	}
      else // t1->size() < t2->size()
	{
	  return vars2->GreaterOrEqual(*vars1) ? Less : Incomparable;
	};
    }; // Comparison Flatterm::KBCompareAtoms(const Flatterm* t1,MultisetOfVariables<VampireKernelConst::MaxNumOfVariables>* vars1,...

  inline void Flatterm::renameVariables(INT_VAR_RENAMING& var_renaming)
    {
      for (Flatterm* sym = this; sym < _after; sym++)
	if (sym->Symbol().isVariable())
	  sym->symbolRef().SetVar(var_renaming.Rename(sym->Symbol().var()));
    }; // void Flatterm::renameVariables(INT_VAR_RENAMING& var_renaming)

  inline void Flatterm::CopyWithShift(const Flatterm* org,Flatterm* dest,const long& shift)
    {
      CALL("CopyWithShift(const Flatterm* org,Flatterm* dest,const long& shift)");
      ASSERT((dest < org) || (dest >= org->after()));
      const Flatterm* endOfOrg = org->after();
      do 
	{
	  *dest = *org;   
	  dest->ShiftNextTerm(shift);  
	  org++; 
	  dest++;
	}
      while (org < endOfOrg);
    }; //void Flatterm::CopyWithShift(const Flatterm* org,Flatterm* dest,const long& shift) 


  inline void Flatterm::BinSwapArgs()
    {
      CALL("BinSwapArgs()");
      ASSERT(isComplex());
      ASSERT(arity() == 2);
      Flatterm* arg1 = this + 1;
      Flatterm* arg2 = arg1->after();
      long arg1Weight = arg2 - arg1;
      long arg2Weight = arg2->size(); 
   
      Flatterm arg1Copy[VampireKernelConst::MaxTermSize]; 
      CopyWithShift(arg1,arg1Copy,arg2Weight);
      CopyWithShift(arg2,arg1,(-1)*arg1Weight);
      arg1 += arg2Weight;
      for (long i = 0; i < arg1Weight; i++) arg1[i] = arg1Copy[i];      
    }; // void Flatterm::BinSwapArgs()


  inline ostream& Flatterm::outputTerm(ostream& str) const
    {
      Iterator iter(this);
      while (iter.NotEndOfTerm())
	{
	  iter.CurrPos()->output(str) << " ";
	  iter.NextSym();
	};   
      return str;
    }; // ostream& Flatterm::outputTerm((ostream& str) const

}; // namespace VK
//================================================
#undef DEBUG_NAMESPACE
//=============================================================
#endif
