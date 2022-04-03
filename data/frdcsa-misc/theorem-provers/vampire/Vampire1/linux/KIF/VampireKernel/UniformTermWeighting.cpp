//
// File:         UniformTermWeighting.cpp
// Description:  Fast and simple term weighting.  
// Created:      Apr 18, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//===================================================================
#include "UniformTermWeighting.h"
#include "prefix.h"
#include "ForwardMatchingSubstitutionCache.h"
#include "GlobAlloc.h"
#include "TermWeightType.h"
//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_UNIFORM_TERM_WEIGHTING
 #define DEBUG_NAMESPACE "UniformTermWeighting"
#endif
#include "debugMacros.h"
//===================================================================

namespace VK
{
UniformTermWeighting uniformTermWeighting;


UniformTermWeighting::~UniformTermWeighting() {};

void* UniformTermWeighting::operator new(size_t)
{
  return GlobAlloc::allocate(sizeof(UniformTermWeighting));
}; 

void UniformTermWeighting::operator delete(void* obj)
{
  GlobAlloc::deallocate(obj,sizeof(UniformTermWeighting));
};

void UniformTermWeighting::collectWeight(WeightPolynomial& weight,const Flatterm* term)
{
  CALL("collectWeight(WeightPolynomial& weight,const Flatterm* term)");
  Flatterm::Iterator iter(term);
  do
    {
      if (iter.CurrPos()->isVariable())
	{
	  weight.add1(iter.CurrPos()->Var());
	}
      else
	weight.add1();
      iter.NextSym();
    }
  while (iter.NotEndOfTerm());
}; //  void UniformTermWeighting::collectWeight(WeightPolynomial& weight,const Flatterm* term)



void UniformTermWeighting::collectWeight(WeightPolynomial& weight,TermWeightType coefficient,const Flatterm* term)
{
  CALL("collectWeight(WeightPolynomial& weight,TermWeightType coefficient,const Flatterm* term)");
  Flatterm::Iterator iter(term);
  do
    {
      if (iter.CurrPos()->isVariable())
	{
	  weight.add(coefficient,iter.CurrPos()->Var());
	}
      else
	weight.add(coefficient);
      iter.NextSym();
    }
  while (iter.NotEndOfTerm());
}; //  void UniformTermWeighting::collectWeight(WeightPolynomial& weight,TermWeightType coefficient,const Flatterm* term)




TermWeightType UniformTermWeighting::computeMinimalInstanceWeight(const Flatterm* term,
							bool& containsVariables)
{
  CALL("computeMinimalInstanceWeight(const Flatterm* term,bool& containsVariables)");
  TermWeightType result = 0;
  containsVariables = false;
  
  Flatterm::Iterator iter(term);
  do
    {
      if (iter.CurrPos()->isVariable()) containsVariables = true;
      ++result;
      iter.NextSym();
    }
  while (iter.NotEndOfTerm());  
  return result;
}; // TermWeightType UniformTermWeighting::computeMinimalInstanceWeight(const Flatterm* term,bool& containsVariables)


TermWeightType UniformTermWeighting::computeWeightIfGroundAndLessOrEqual(TermWeightType coefficient,
							       const Flatterm* term,
							       TermWeightType weightLimit)
{
  CALL("computeWeightIfGroundAndLessOrEqual(TermWeightType coefficient,const Flatterm* term,TermWeightType weightLimit)");
    
  TermWeightType result = 0;
  Flatterm::Iterator iter(term);
  do
    {
      if (iter.CurrPos()->isVariable()) return 0L; // nonground
      result += coefficient;
      if (result > weightLimit) return (TermWeightType)(-1); // too heavy      
      iter.NextSym();
    }
  while (iter.NotEndOfTerm());
  ASSERT(result > 0L);
  return result;

}; // TermWeightType UniformTermWeighting::computeWeightIfGroundAndLessOrEqual(TermWeightType coefficient,const Flatterm* term,TermWeightType weightLimit)




void UniformTermWeighting::collectWeight(WeightPolynomial& weight,const TERM& term)
{
  CALL("collectWeight(WeightPolynomial& weight,const TERM& term");
  if (term.isVariable())
    {
      weight.add1(term.var());
    }
  else
    {
      ASSERT(term.IsReference());
      TERM::Iterator iter(term.First());
      do
	{
	  if (iter.CurrentSymIsVar())
	    {
	      weight.add1(iter.symbolRef().var());
	    }
	  else
	    weight.add1();
	}
      while (iter.Next());
    };
}; // void UniformTermWeighting::collectWeight(WeightPolynomial& weight,const TERM& term)



void UniformTermWeighting::collectWeight(WeightPolynomial& weight,TermWeightType coefficient,const TERM& term)
{
  CALL("collectWeight(WeightPolynomial& weight,TermWeightType coefficient,const TERM& term");
  if (term.isVariable())
    {
      weight.add(coefficient,term.var());
    }
  else
    {
      ASSERT(term.IsReference());
      TERM::Iterator iter(term.First());
      do
	{
	  if (iter.CurrentSymIsVar())
	    {
	      weight.add(coefficient,iter.symbolRef().var());
	    }
	  else
	    weight.add(coefficient);
	}
      while (iter.Next());
    };
}; // void UniformTermWeighting::collectWeight(WeightPolynomial& weight,TermWeightType coefficient,const TERM& term)





void UniformTermWeighting::collectWeight(WeightPolynomial& weight,TermWeightType coefficient,const PREFIX_SYM* term)
{
  CALL("collectWeight(WeightPolynomial& weight,TermWeightType coefficient,const PREFIX_SYM* term)");
  if (term->Head().isVariable())
    {
      Unifier::Variable* var = term->Head().var();
      bool freeVar;
      void* instance = var->Unref(freeVar);
      if (freeVar)
	{
	  weight.add(coefficient,((Unifier::Variable*)instance)->absoluteVarNum());
	  return;
	}
      else
	{
	  term = (const PREFIX_SYM*)instance;
	};
    };
  ASSERT(term->Head().IsFunctor());
  PREFIX::ITER iter((PREFIX_SYM*)term);
  do
    {
      if (iter.CurrVar())
	{
	  weight.add(coefficient,iter.CurrVar()->absoluteVarNum());
	}
      else
	{
	  weight.add(coefficient);
	};
      iter.SkipSym();
    }
  while (iter.NotEmpty());
  
}; // void UniformTermWeighting::collectWeight(WeightPolynomial& weight,TermWeightType coefficient,const PREFIX_SYM* term)



void UniformTermWeighting::collectWeight(WeightPolynomial& weight,const PREFIX_SYM* term)
{
  CALL("collectWeight(WeightPolynomial& weight,const PREFIX_SYM* term)");
  if (term->Head().isVariable())
    {
      Unifier::Variable* var = term->Head().var();
      bool freeVar;
      void* instance = var->Unref(freeVar);
      if (freeVar)
	{
	  weight.add1(((Unifier::Variable*)instance)->absoluteVarNum());
	  return;
	}
      else
	{
	  term = (const PREFIX_SYM*)instance;
	};
    };
  ASSERT(term->Head().IsFunctor());
  PREFIX::ITER iter((PREFIX_SYM*)term);
  do
    {
      if (iter.CurrVar())
	{
	  weight.add1(iter.CurrVar()->absoluteVarNum());
	}
      else
	{
	  weight.add1();
	};
      iter.SkipSym();
    }
  while (iter.NotEmpty());
  
}; // void UniformTermWeighting::collectWeight(WeightPolynomial& weight,const PREFIX_SYM* term)


void UniformTermWeighting::collectWeight(WeightPolynomial& weight,const TERM& term,ulong variableIndex)
{
  CALL("collectWeight(WeightPolynomial& weight,const TERM& term,ulong variableIndex)");
  static ST_TERM_WITH_SUBST iter;
  iter.Reset(variableIndex,(TERM&)term);
  do
    {
      if (iter.CurrSym().isVariable())
	{
	  weight.add1(iter.CurrSym().var()->absoluteVarNum());
	}
      else
	{
	  weight.add1();
	};
      iter.SkipSym();
    }
  while (!iter.Empty());

}; // void UniformTermWeighting::collectWeight(WeightPolynomial& weight,const TERM& term,ulong variableIndex)



void 
UniformTermWeighting::collectWeight(WeightPolynomial& weight,
				    TermWeightType coefficient,
				    const TERM* complexTerm,
				    const InstanceRetrievalForDemodulation::Substitution* subst)
{
  CALL("collectWeight(WeightPolynomial& weight,TermWeightType coefficient,const TERM* complexTerm,const InstanceRetrievalForDemodulation::Substitution* subst)");
   
  static InstanceRetrievalForDemodulation::Substitution::Instance inst;
  inst.reset(complexTerm,subst);
  do
    {
      if (inst.symbol().isVariable())
	{
	  weight.add(coefficient,inst.symbol().var());
	}
      else
	weight.add(coefficient);
    }
  while (inst.next()); 
}; // void UniformTermWeighting::collectWeight(WeightPolynomial& weight,TermWeightType coefficient,const TERM* complexTerm,const InstanceRetrievalForDemodulation::Substitution* subst)




void 
UniformTermWeighting::collectWeight(WeightPolynomial& weight,
				    const TERM* complexTerm,
				    const InstanceRetrievalForDemodulation::Substitution* subst)
{
  CALL("collectWeight(WeightPolynomial& weight,const TERM* complexTerm,const InstanceRetrievalForDemodulation::Substitution* subst)");
   
  static InstanceRetrievalForDemodulation::Substitution::Instance inst;
  inst.reset(complexTerm,subst);
  do
    {
      if (inst.symbol().isVariable())
	{
	  weight.add1(inst.symbol().var());
	}
      else
	weight.add1();
    }
  while (inst.next()); 
}; // void UniformTermWeighting::collectWeight(WeightPolynomial& weight,const TERM* complexTerm,const InstanceRetrievalForDemodulation::Substitution* subst)



void UniformTermWeighting::collectWeight(WeightPolynomial& weight,
					 const TERM& term,
					 const ForwardMatchingSubstitutionCache* subst)
{
  CALL("collectWeight(WeightPolynomial& weight,const TERM& term,const ForwardMatchingSubstitutionCache* subst)");
  
  static ForwardMatchingSubstitutionCache::Instance inst;
  inst.reset(term,subst);
  do
    {
      if (inst.symbol().isVariable())
	{
	  weight.add1(inst.symbol().var());
	}
      else
	weight.add1();
    }
  while (inst.next());
}; // void UniformTermWeighting::collectWeight(WeightPolynomial& weight,const TERM& term,const ForwardMatchingSubstitutionCache* subst)







TermWeightType UniformTermWeighting::computeMinimalInstanceWeight(const PREFIX_SYM* term,bool& containsVariables)
{
  CALL("computeMinimalInstanceWeight(const PREFIX_SYM* term,bool& containsVariables)");

  if (term->Head().isVariable())
    {
      Unifier::Variable* var = term->Head().var();
      bool freeVar;
      void* instance = var->Unref(freeVar);
      if (freeVar)
	{
	  containsVariables = true;
	  return 1L; // minimal term weight
	}
      else
	{
	  term = (const PREFIX_SYM*)instance;
	};
    };

  ASSERT(term->Head().IsFunctor());  
  TermWeightType result = 0;
  containsVariables = false;
  PREFIX::ITER iter((PREFIX_SYM*)term);
  do
    {
      if (iter.CurrVar()) containsVariables = true;
      ++result;
      iter.SkipSym();
    }
  while (iter.NotEmpty());
  return result;
}; // TermWeightType UniformTermWeighting::computeMinimalInstanceWeight(const PREFIX_SYM* term,bool& containsVariables)



TermWeightType UniformTermWeighting::computeWeightIfGroundAndLessOrEqual(TermWeightType coefficient,const PREFIX_SYM* term,TermWeightType weightLimit)
{
  CALL("computeWeightIfGroundAndLessOrEqual(TermWeightType coefficient,const PREFIX_SYM* term,TermWeightType weightLimit)");  
  // returns 0 if nonground and -1 if too heavy
  if (term->Head().isVariable())
    {
      Unifier::Variable* var = term->Head().var();
      bool freeVar;
      void* instance = var->Unref(freeVar);
      if (freeVar)
	{
	  return 0L;  // nonground
	}
      else
	{
	  term = (const PREFIX_SYM*)instance;
	};
    };

  ASSERT(term->Head().IsFunctor());  
  TermWeightType result = 0;
  PREFIX::ITER iter((PREFIX_SYM*)term);
  do
    {
      if (iter.CurrVar()) return 0L; // nonground
      result += coefficient;
      if (result > weightLimit) return (TermWeightType)(-1); // too heavy
      iter.SkipSym();
    }
  while (iter.NotEmpty());
  ASSERT(result > 0L);
  return result;
}; // TermWeightType UniformTermWeighting::computeWeightIfGroundAndLessOrEqual(TermWeightType coefficient,const PREFIX_SYM* term,TermWeightType weightLimit)


TermWeightType UniformTermWeighting::computeWeightIfGroundAndLessOrEqual(const PREFIX_SYM* term,TermWeightType weightLimit)
{
  CALL("computeWeightIfGroundAndLessOrEqual(const PREFIX_SYM* term,TermWeightType weightLimit)");  
  // returns 0 if nonground and -1 if too heavy
  if (term->Head().isVariable())
    {
      Unifier::Variable* var = term->Head().var();
      bool freeVar;
      void* instance = var->Unref(freeVar);
      if (freeVar)
	{
	  return 0L;  // nonground
	}
      else
	{
	  term = (const PREFIX_SYM*)instance;
	};
    };

  ASSERT(term->Head().IsFunctor());  
  TermWeightType result = 0;
  PREFIX::ITER iter((PREFIX_SYM*)term);
  do
    {
      if (iter.CurrVar()) return 0L; // nonground
      ++result;
      if (result > weightLimit) return (TermWeightType)(-1); // too heavy
      iter.SkipSym();
    }
  while (iter.NotEmpty());
  ASSERT(result > 0L);
  return result;
}; // TermWeightType UniformTermWeighting::computeWeightIfGroundAndLessOrEqual(const PREFIX_SYM* term,TermWeightType weightLimit)


TermWeightType 
UniformTermWeighting::computeMinimalInstanceWeightIfLessOrEqual(TermWeightType coefficient,
								const PREFIX_SYM* term,
								TermWeightType weightLimit,
								bool& ground)
{
  CALL("computeMinimalInstanceWeightIfLessOrEqual(TermWeightType coefficient,const PREFIX_SYM* term,..)");
  // ^ returns -1 if too heavy

  if (term->Head().isVariable())
    {
      Unifier::Variable* var = term->Head().var();
      bool freeVar;
      void* instance = var->Unref(freeVar);
      if (freeVar)
	{
	  ground = false;
	  return (coefficient > weightLimit) ? (TermWeightType)(-1) : coefficient;
	}
      else
	{
	  term = (const PREFIX_SYM*)instance;
	};
    };

  ASSERT(term->Head().IsFunctor());  
  TermWeightType result = 0;
  ground = true;
  PREFIX::ITER iter((PREFIX_SYM*)term);
  do
    {
      if (iter.CurrVar()) ground = false;
      result += coefficient;
      if (result > weightLimit) return (TermWeightType)(-1); // too heavy
      iter.SkipSym();
    }
  while (iter.NotEmpty());
  ASSERT(result > 0L);
  return result;

}; // TermWeightType UniformTermWeighting::computeMinimalInstanceWeightIfLessOrEqual(TermWeightType coefficient,const PREFIX_SYM* term,..)




TermWeightType 
UniformTermWeighting::computeMinimalInstanceWeightIfLessOrEqual(const PREFIX_SYM* term,
								TermWeightType weightLimit,
								bool& ground)
{
  CALL("computeMinimalInstanceWeightIfLessOrEqual(const PREFIX_SYM* term,..)");
  // ^ returns -1 if too heavy
  if (weightLimit <= 0L) return (TermWeightType)(-1);
  if (term->Head().isVariable())
    {
      Unifier::Variable* var = term->Head().var();
      bool freeVar;
      void* instance = var->Unref(freeVar);
      if (freeVar)
	{
	  ground = false;
	  return 1L; 
	}
      else
	{
	  term = (const PREFIX_SYM*)instance;
	};
    };

  ASSERT(term->Head().IsFunctor());  
  TermWeightType result = 0;
  ground = true;
  PREFIX::ITER iter((PREFIX_SYM*)term);
  do
    {
      if (iter.CurrVar()) ground = false;
      ++result;
      if (result > weightLimit) return (TermWeightType)(-1); // too heavy
      iter.SkipSym();
    }
  while (iter.NotEmpty());
  ASSERT(result > 0L);
  return result;

}; // TermWeightType UniformTermWeighting::computeMinimalInstanceWeightIfLessOrEqual(const PREFIX_SYM* term,..)





TermWeightType 
UniformTermWeighting::computeMinimalInstanceWeight(const TERM* term,bool& containsVariables)
{
  CALL("computeMinimalInstanceWeight(const TERM* term,bool& containsVariables)");
  ASSERT(term->isComplex());
  _iter.Reset(term);
  TermWeightType result = 0L;
  do
    {
      if (_iter.symbolRef().isVariable()) containsVariables = true;
      ++result;
    }
  while (_iter.Next());
  return result;
}; // TermWeightType UniformTermWeighting::computeMinimalInstanceWeight(const TERM* term,bool& containsVariables)



TermWeightType 
UniformTermWeighting::computeWeightIfGroundAndLessOrEqual(TermWeightType coefficient,
							  const TERM* term,
							  TermWeightType weightLimit)
{
  CALL("computeWeightIfGroundAndLessOrEqual(TermWeightType coefficient,const TERM* term,TermWeightType weightLimit)");  
  // returns 0 if nonground and -1 if too heavy
  ASSERT(term->isComplex());
  _iter.Reset(term);
  TermWeightType result = 0L;
  do
    {
      if (_iter.symbolRef().isVariable()) return 0L; // nonground
      result += coefficient;
      if (result > weightLimit) return (TermWeightType)(-1); // too heavy
    }
  while (_iter.Next());
  return result;

}; // TermWeightType UniformTermWeighting::computeWeightIfGroundAndLessOrEqual(TermWeightType coefficient,const TERM* term,TermWeightType weightLimit)


TermWeightType 
UniformTermWeighting::computeWeightIfGroundAndLessOrEqual(const TERM* term,TermWeightType weightLimit)
{
  CALL("computeWeightIfGroundAndLessOrEqual(const TERM* term,TermWeightType weightLimit)");  
  // returns 0 if nonground and -1 if too heavy
  ASSERT(term->isComplex());
  _iter.Reset(term);
  TermWeightType result = 0L;
  do
    {
      if (_iter.symbolRef().isVariable()) return 0L; // nonground
      ++result;
      if (result > weightLimit) return (TermWeightType)(-1); // too heavy
    }
  while (_iter.Next());
  return result;
}; // TermWeightType UniformTermWeighting::computeWeightIfGroundAndLessOrEqual(const TERM* term,TermWeightType weightLimit)


TermWeightType 
UniformTermWeighting::computeMinimalInstanceWeightIfLessOrEqual(TermWeightType coefficient,
								const TERM* term,
								TermWeightType weightLimit,
								bool& ground)
{
  CALL("computeMinimalInstanceWeightIfLessOrEqual(TermWeightType coefficient,const TERM* term,..)");
  // ^ returns -1 if too heavy

  ASSERT(term->isComplex());
  _iter.Reset(term);
  TermWeightType result = 0L;
  ground = true;
  do
    {
      if (_iter.symbolRef().isVariable()) ground = false; 
      result += coefficient;
      if (result > weightLimit) return (TermWeightType)(-1); // too heavy
    }
  while (_iter.Next());
  return result;
}; // TermWeightType UniformTermWeighting::computeMinimalInstanceWeightIfLessOrEqual(TermWeightType coefficient,const TERM* term,..)




TermWeightType 
UniformTermWeighting::computeMinimalInstanceWeightIfLessOrEqual(const TERM* term,
								TermWeightType weightLimit,
								bool& ground)
{
  CALL("computeMinimalInstanceWeightIfLessOrEqual(const TERM* term,..)");
  // ^ returns -1 if too heavy

  ASSERT(term->isComplex());

  _iter.Reset(term);
  TermWeightType result = 0L;
  ground = true;
  do
    {
      if (_iter.symbolRef().isVariable()) ground = false; 
      ++result;
      if (result > weightLimit) return (TermWeightType)(-1); // too heavy
    }
  while (_iter.Next());

  return result;
}; // TermWeightType UniformTermWeighting::computeMinimalInstanceWeightIfLessOrEqual(const TERM* term,..)


}; // namespace VK

//===================================================================

