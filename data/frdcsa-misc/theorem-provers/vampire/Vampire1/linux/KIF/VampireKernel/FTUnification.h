//
// File:         FTUnification.h
// Description:  Unification for flatterms.
// Created:      Jun 6, 2000, 15:00
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//====================================================
#ifndef FT_UNIFICATION_H
//====================================================
#define FT_UNIFICATION_H
#ifndef NO_DEBUG_VIS
 #include <iostream.h>
#endif
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "VampireKernelConst.h"
#include "Term.h" 
#include "Flatterm.h" 
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FT_UNIFICATION
 #define DEBUG_NAMESPACE "FTUnification"
#endif 
#include "debugMacros.h"
//================================================= 

namespace VK
{
  using namespace BK;
  using namespace Gematogen;
class FTUnification
{
 public:
  FTUnification() 
   : _bound("_bound"),
     _borderVar("_borderVar"),
     _borderTerm("_borderTerm")
  { 
   init();
  };
  void init() 
  {
   for (ulong v = 0; v < 2*VampireKernelConst::MaxNumOfVariables; v++) _subst[v] = (const Flatterm*)0;
  };
  ~FTUnification() {};
  bool unify(const Flatterm* t1,const Flatterm* t2); 
  
  const Flatterm* subst(ulong var) const { return _subst[var]; };

  #ifndef NO_DEBUG
   bool equalWithSubst(const Flatterm* t1,const Flatterm* t2);
  #endif

 private: 
  void reset()
  {
   CALL("reset()");
   _borderVar.reset();
   _borderTerm.reset();
   while (_bound) _subst[_bound.pop()] = (const Flatterm*)0;
  };
  bool occurs(ulong var,const Flatterm* term);
  void bind(ulong var,const Flatterm* term);
 private:
  const Flatterm* _subst[2*VampireKernelConst::MaxNumOfVariables];
  Stack<ulong,2*VampireKernelConst::MaxNumOfVariables> _bound;
  Stack<ulong,VampireKernelConst::MaxTermSize> _borderVar;
  Stack<const Flatterm*,VampireKernelConst::MaxTermSize> _borderTerm;
}; // class FTUnification

inline bool FTUnification::unify(const Flatterm* t1,const Flatterm* t2)
{
 CALL("unify(const Flatterm* t1,const Flatterm* t2)");
 DOP(const Flatterm* debugT1 = t1);
 DOP(const Flatterm* debugT2 = t2);
 reset();
 Flatterm::Iterator iter1(t1);
 Flatterm::Iterator iter2(t2);
 ulong bv;
 compute_border:
  do
   { 
    if (iter1.CurrPos()->symbolRef() == iter2.CurrPos()->symbolRef()) 
     { 
      iter1.NextSym();
      iter2.NextSym();
     }
    else
     if (iter1.CurrPos()->symbolRef().isVariable())
      {
       _borderVar.push(iter1.CurrPos()->symbolRef().var());
       _borderTerm.push(iter2.CurrPos()); 
       iter1.NextSym();
       iter2.after();
      }
     else 
      if (iter2.CurrPos()->symbolRef().isVariable())
       {
        _borderVar.push(iter2.CurrPos()->symbolRef().var());
        _borderTerm.push(iter1.CurrPos()); 
        iter2.NextSym();
        iter1.after();
       } 
      else 
       {
        return false; // function conflict
       };
   }
  while (iter1.NotEndOfTerm()); 
 
  ASSERT(iter2.EndOfTerm());

  process_border:
   if (_borderVar.empty()) 
    {
     ASSERT(equalWithSubst(debugT1,debugT2));
     return true;
    };
   bv = _borderVar.pop();
   t2 = _borderTerm.pop();
   t1 = _subst[bv];
   if (t1) 
    {
     iter1.Reset(t1);
     iter2.Reset(t2);
     goto compute_border;
    };
   // variable bv is free 
   if (occurs(bv,t2)) 
    {
     return false;
    };   
   bind(bv,t2);
   goto process_border;
}; // bool FTUnification::unify(const Flatterm* t1,const Flatterm* t2)

inline bool FTUnification::occurs(ulong var,const Flatterm* term)
{
 CALL("occurs(ulong var,const Flatterm* term)");
 ASSERT(term->symbolRef().isComplex() || (term->symbolRef().var() != var));
 Stack<const Flatterm*,VampireKernelConst::MaxTermSize> termsToCheck;
 Flatterm::Iterator iter(term);
 check_term:
  do 
   {
    if (iter.CurrPos()->symbolRef().isVariable())
     {
      if (iter.CurrPos()->symbolRef().var() == var) return true;
      if (_subst[iter.CurrPos()->symbolRef().var()])
       termsToCheck.push(_subst[iter.CurrPos()->symbolRef().var()]); 
     };
    iter.NextSym();
   }
  while (iter.NotEndOfTerm());
  
  if (termsToCheck.empty()) return false;
  iter.Reset(termsToCheck.pop());
  goto check_term;
}; // bool FTUnification::occurs(ulong var,const Flatterm* term)

inline void FTUnification::bind(ulong var,const Flatterm* term)
{
 CALL("bind(ulong var,const Flatterm* term)");
 ASSERT(!_subst[var]);
 _subst[var] = term;
 _bound.push(var);
}; // void FTUnification::bind(ulong var,const Flatterm* term)

#ifndef NO_DEBUG
 inline bool FTUnification::equalWithSubst(const Flatterm* t1,const Flatterm* t2)
 {
  CALL("equalWithSubst(const Flatterm* t1,const Flatterm* t2)");
  Flatterm::Iterator iter1(t1);
  Flatterm::Iterator iter2(t2);
  do
   { 
    if (iter1.CurrPos()->symbolRef() == iter2.CurrPos()->symbolRef()) 
     { 
      iter1.NextSym();
      iter2.NextSym();
     }
    else
     if (iter1.CurrPos()->symbolRef().isVariable())
      {
       if (_subst[iter1.CurrPos()->symbolRef().var()])
	{
	 if (!equalWithSubst(_subst[iter1.CurrPos()->symbolRef().var()],iter2.CurrPos())) return false;      
         iter1.NextSym(); 
         iter2.after();
        }
       else // iter1.CurrPos()->symbolRef() is a free variable 
        {
         if (!iter2.CurrPos()->symbolRef().isVariable()) return false;  
         if (!_subst[iter2.CurrPos()->symbolRef().var()]) return false;  
         if (!equalWithSubst(iter1.CurrPos(),_subst[iter2.CurrPos()->symbolRef().var()])) return false;
         iter1.NextSym();
         iter2.NextSym();
        };
      }
     else 
      if (iter2.CurrPos()->symbolRef().isVariable())
       {
        if (_subst[iter2.CurrPos()->symbolRef().var()])
	 {
	  if (!equalWithSubst(_subst[iter2.CurrPos()->symbolRef().var()],iter1.CurrPos())) return false;      
          iter2.NextSym(); 
          iter1.after();
         }
        else // iter2.CurrPos()->symbolRef() is a free variable 
	 return false;
       } 
      else return false; // function conflict   
   }
  while (iter1.NotEndOfTerm());
  ASSERT(iter2.EndOfTerm());
  return true;
 }; // bool FTUnification::equalWithSubst(const Flatterm* t1,const Flatterm* t2)
#endif


}; // namespace VK



//================================================
#undef DEBUG_NAMESPACE
//================================================= 
#endif
