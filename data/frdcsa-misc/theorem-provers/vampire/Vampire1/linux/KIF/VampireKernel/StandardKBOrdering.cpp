//
// File:         StandardKBOrdering.cpp
// Description:  Standard Knuth-Bendix ordering.  
// Created:      Jun 28, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//=================================================================
#include "StandardKBOrdering.h"
#include "TermWeighting.h"
#include "variables.h"
#include "ForwardMatchingSubstitutionCache.h"
#include "DestructionMode.h"
#include "TermWeightType.h"
//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_STANDARD_KB_ORDERING
#define DEBUG_NAMESPACE "StandardKBOrdering"
#endif
#include "debugMacros.h"
//===================================================================


using namespace VK;

using namespace BK;

StandardKBOrdering::~StandardKBOrdering()
{
  CALL("destructor ~StandardKBOrdering()");
  if (DestructionMode::isThorough()) destroyThoroughly();
};


void* StandardKBOrdering::operator new(size_t)
{
  return GlobAlloc::allocate(sizeof(StandardKBOrdering));
};

void StandardKBOrdering::operator delete(void* obj)
{
  GlobAlloc::deallocate(obj,sizeof(StandardKBOrdering));
}; 

void StandardKBOrdering::destroyThoroughly()
{
  CALL("destroyThoroughly()");
  while (_specialisedComparisonPool)
    {
      SpecialisedComparison* tmp = _specialisedComparisonPool;
      _specialisedComparisonPool = _specialisedComparisonPool->getNextInPool();
      delete tmp;
    };

  while (_constraintCheckOnUnifierPool)
    {
      ConstraintCheckOnUnifier* tmp = _constraintCheckOnUnifierPool;
      _constraintCheckOnUnifierPool = _constraintCheckOnUnifierPool->getNextInPool();
      delete tmp;
    };
  

  while (_constraintCheckOnForwardMatchingSubstPool)
    {
      ConstraintCheckOnForwardMatchingSubst* tmp = _constraintCheckOnForwardMatchingSubstPool;
      _constraintCheckOnForwardMatchingSubstPool = _constraintCheckOnForwardMatchingSubstPool->getNextInPool();
      delete tmp;
    };

  while (_compiledConstraintOnUnifiersPool)
    {
      CompiledConstraintForUnifiers* tmp = _compiledConstraintOnUnifiersPool;
      _compiledConstraintOnUnifiersPool = _compiledConstraintOnUnifiersPool->getNextInPool();      
      delete tmp;
    };


  while (_compiledConstraintForInstanceRetrievalPool)
    {
      CompiledConstraintForInstanceRetrieval* tmp = _compiledConstraintForInstanceRetrievalPool;

      _compiledConstraintForInstanceRetrievalPool = _compiledConstraintForInstanceRetrievalPool->getNextInPool();  
    
      delete tmp;
    };  

}; // void StandardKBOrdering::destroyThoroughly()

Comparison StandardKBOrdering::compare(const Flatterm* term1,
				       const WeightPolynomial& weight1,
				       const Flatterm* term2,
				       const WeightPolynomial& weight2)
{
  CALL("compare(..)");
  Comparison lexCmp;
  switch (weight1.compare(weight2))
    {
    case BK::FailureToCompare:
      return Incomparable;
    case BK::AlwaysLess:
      return Less;
    case BK::AlwaysEqual:
      return compareLexicographically(term1,term2);
    case BK::AlwaysGreater:
      return Greater;
    case BK::CanBeLessOrEqual:
      lexCmp = compareLexicographically(term1,term2);
      if (lexCmp == Less) return Less;
      ASSERT(lexCmp != Equal);
      return Incomparable;
    case BK::CanBeGreaterOrEqual:
      lexCmp = compareLexicographically(term1,term2);
      if (lexCmp == Greater) return Greater;
      ASSERT(lexCmp != Equal);
      return Incomparable;
    case BK::VolatileButNearlyAlwaysLess:
      // same as below
    case BK::VolatileButNearlyAlwaysGreater:
      // same as below
    case BK::AbsolutelyVolatile:
      return Incomparable;
    };
  ICP("ICP0");

#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE)
  return Incomparable;
#endif

}; // Comparison StandardKBOrdering::compare(..)




Comparison StandardKBOrdering::compareAtoms(const Flatterm* lit1,
					    const WeightPolynomial& weight1,
					    const Flatterm* lit2,
					    const WeightPolynomial& weight2)
{
  CALL("compareAtoms(..)");
  Comparison lexCmp;
  switch (weight1.compare(weight2))
    {
    case BK::FailureToCompare:
      return Incomparable;
    case BK::AlwaysLess:
      return Less;
    case BK::AlwaysEqual:
      return compareAtomsLexicographically(lit1,lit2);
    case BK::AlwaysGreater:
      return Greater;
    case BK::CanBeLessOrEqual:
      lexCmp = compareAtomsLexicographically(lit1,lit2);
      if (lexCmp == Less) return Less;
      ASSERT(lexCmp != Equal);
      return Incomparable;
    case BK::CanBeGreaterOrEqual:
      lexCmp = compareAtomsLexicographically(lit1,lit2);
      if (lexCmp == Greater) return Greater;
      ASSERT(lexCmp != Equal);
      return Incomparable;
    case BK::VolatileButNearlyAlwaysLess:
      // same as below
    case BK::VolatileButNearlyAlwaysGreater:
      // same as below
    case BK::AbsolutelyVolatile:
      return Incomparable;
    };

  ICP("ICP0");
#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE)
  return Incomparable;
#endif

}; // Comparison StandardKBOrdering::compareAtoms(..)



Comparison StandardKBOrdering::compareOnCurrentUnifier(PREFIX_SYM* complexTerm1,
						       PREFIX_SYM* complexTerm2)
{
  CALL("compareOnCurrentUnifier(PREFIX_SYM* complexTerm1,PREFIX_SYM* complexTerm2)");

  //DF; cout << "COMPARE ";
  //DF; PREFIX::outputWordWithSubst(cout,complexTerm1);
  //DF; cout << " WITH ";
  //DF; PREFIX::outputWordWithSubst(cout,complexTerm2) << "\n";
  //DF; Unifier::current()->output(cout << "SUBST ",PREFIX::outputTerm) << "\n\n\n";  

  _weight1.reset();
  _weight2.reset();
  TermWeighting::current()->collectWeight(_weight1,complexTerm1);
  TermWeighting::current()->collectWeight(_weight2,complexTerm2);
  Comparison lexCmp;
  switch (_weight1.compare(_weight2))
    {
    case BK::FailureToCompare:
      return Incomparable;
    case BK::AlwaysLess:
      return Less;
    case BK::AlwaysEqual:
      return compareLexOnCurrentUnifier(complexTerm1,complexTerm2);
    case BK::AlwaysGreater:
      return Greater;
    case BK::CanBeLessOrEqual:
      lexCmp = compareLexOnCurrentUnifier(complexTerm1,complexTerm2);
      if (lexCmp == Less) return Less;
      ASSERT(lexCmp != Equal);
      return Incomparable;
      
    case BK::CanBeGreaterOrEqual:
      lexCmp = compareLexOnCurrentUnifier(complexTerm1,complexTerm2);
      if (lexCmp == Greater) return Greater;
      ASSERT(lexCmp != Equal);
      return Incomparable;

    case BK::VolatileButNearlyAlwaysLess:
      // same as below
    case BK::VolatileButNearlyAlwaysGreater:
      // same as below
    case BK::AbsolutelyVolatile:
      return Incomparable;
    };

  ICP("ICP0");
#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE)
  return Incomparable;
#endif

}; // Comparison StandardKBOrdering::compareOnCurrentUnifier(..)


Comparison 
StandardKBOrdering::compareOnCurrentUnifier(const TERM& term1,
					    const TERM& term2,
					    ulong variableIndex)
{
  CALL("compareOnCurrentUnifier(const TERM& term1,const TERM& term2,ulong variableIndex)");
  _weight1.reset();
  _weight2.reset();
  TermWeighting::current()->collectWeight(_weight1,term1,variableIndex);
  TermWeighting::current()->collectWeight(_weight2,term2,variableIndex);
  Comparison lexCmp;
  switch (_weight1.compare(_weight2))
    {
    case BK::FailureToCompare:
      return Incomparable;
    case BK::AlwaysLess:
      return Less;
    case BK::AlwaysEqual:
      return compareLexOnCurrentUnifier(term1,term2,variableIndex);
    case BK::AlwaysGreater:
      return Greater;
    case BK::CanBeLessOrEqual:

      //DF; cout << "********WEIGHT COMP = CanBeLessOrEqual\n";

      lexCmp = compareLexOnCurrentUnifier(term1,term2,variableIndex);

      //DF; cout << "*********LEX CMP = " << lexCmp << "\n";

      if (lexCmp == Less) return Less;
      ASSERT(lexCmp != Equal);
      return Incomparable;
      
    case BK::CanBeGreaterOrEqual:
      lexCmp = compareLexOnCurrentUnifier(term1,term2,variableIndex);
      if (lexCmp == Greater) return Greater;
      ASSERT(lexCmp != Equal);
      return Incomparable;

    case BK::VolatileButNearlyAlwaysLess:
      // same as below
    case BK::VolatileButNearlyAlwaysGreater:
      // same as below
    case BK::AbsolutelyVolatile:
      return Incomparable;
    };

  ICP("ICP0");
#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE)
  return Incomparable;
#endif

}; // Comparison StandardKBOrdering::compareOnCurrentUnifier(const TERM& term1,const TERM& term2,ulong variableIndex)



Comparison StandardKBOrdering::compare(const TERM* complexTerm1,
				       const TERM* complexTerm2,
				       const InstanceRetrievalForDemodulation::Substitution* subst)
{
  CALL("compare(const TERM* complexTerm1,const TERM* complexTerm2,const InstanceRetrievalForDemodulation::Substitution* subst)");
  _weight1.reset();
  _weight2.reset();
  
  TermWeighting::current()->collectWeight(_weight1,complexTerm1,subst);
  TermWeighting::current()->collectWeight(_weight2,complexTerm2,subst);
  Comparison lexCmp;
  switch (_weight1.compare(_weight2))
    {
    case BK::FailureToCompare:
      return Incomparable;
    case BK::AlwaysLess:
      return Less;
    case BK::AlwaysEqual:
      return compareLexicographically(complexTerm1,complexTerm2,subst);
    case BK::AlwaysGreater:
      return Greater;
    case BK::CanBeLessOrEqual:
      lexCmp = compareLexicographically(complexTerm1,complexTerm2,subst);
      if (lexCmp == Less) return Less;
      ASSERT(lexCmp != Equal);
      return Incomparable;
      
    case BK::CanBeGreaterOrEqual:
      lexCmp = compareLexicographically(complexTerm1,complexTerm2,subst);
      if (lexCmp == Greater) return Greater;
      ASSERT(lexCmp != Equal);
      return Incomparable;

    case BK::VolatileButNearlyAlwaysLess:
      // same as below
    case BK::VolatileButNearlyAlwaysGreater:
      // same as below
    case BK::AbsolutelyVolatile:
      return Incomparable;
    };

  ICP("ICP0");
#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE)
  return Incomparable;
#endif
  

}; // Comparison StandardKBOrdering::compare(const TERM* complexTerm1,const TERM* complexTerm2,const InstanceRetrievalForDemodulation::Substitution* subst)







Comparison StandardKBOrdering::compare(const TERM& term1,
				       const TERM& term2,
				       const ForwardMatchingSubstitutionCache* subst)
{
  CALL("compare(const TERM& term1,const TERM& term2,const ForwardMatchingSubstitutionCache* subst)");
  _weight1.reset();
  _weight2.reset();
  TermWeighting::current()->collectWeight(_weight1,term1,subst);
  TermWeighting::current()->collectWeight(_weight2,term2,subst);
  Comparison lexCmp;
  switch (_weight1.compare(_weight2))
    {
    case BK::FailureToCompare:
      return Incomparable;
    case BK::AlwaysLess:
      return Less;
    case BK::AlwaysEqual:
      return compareLexicographically(term1,term2,subst);
    case BK::AlwaysGreater:
      return Greater;
    case BK::CanBeLessOrEqual:
      lexCmp = compareLexicographically(term1,term2,subst);
      if (lexCmp == Less) return Less;
      ASSERT(lexCmp != Equal);
      return Incomparable;
      
    case BK::CanBeGreaterOrEqual:
      lexCmp = compareLexicographically(term1,term2,subst);
      if (lexCmp == Greater) return Greater;
      ASSERT(lexCmp != Equal);
      return Incomparable;

    case BK::VolatileButNearlyAlwaysLess:
      // same as below
    case BK::VolatileButNearlyAlwaysGreater:
      // same as below
    case BK::AbsolutelyVolatile:
      return Incomparable;
    };

  ICP("ICP0");
#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE)
  return Incomparable;
#endif

}; // Comparison StandardKBOrdering::compare(const TERM& term1,const TERM& term2,const ForwardMatchingSubstitutionCache* subst)



SimplificationOrdering::SpecialisedComparison* 
StandardKBOrdering::reserveSpecialisedComparison()
{
  CALL("reserveSpecialisedComparison()");
  SpecialisedComparison* result = _specialisedComparisonPool;
  if (result)
    {
      _specialisedComparisonPool = _specialisedComparisonPool->getNextInPool();
    }
  else
    {
      result = new SpecialisedComparison(this);
    };
  
  return (SimplificationOrdering::SpecialisedComparison*)result;
}; // SimplificationOrdering::SpecialisedComparison* StandardKBOrdering::reserveSpecialisedComparison()

void StandardKBOrdering::releaseSpecialisedComparison(SimplificationOrdering::SpecialisedComparison* sc)
{
  CALL("releaseSpecialisedComparison(SimplificationOrdering::SpecialisedComparison* sc)");
  ((StandardKBOrdering::SpecialisedComparison*)sc)->setNextInPool(_specialisedComparisonPool);
  _specialisedComparisonPool = (StandardKBOrdering::SpecialisedComparison*)sc;
}; //  StandardKBOrdering::releaseSpecialisedComparison(..)



SimplificationOrdering::StoredConstraint*
StandardKBOrdering::storedConstraintGreater(SimplificationOrdering::SpecialisedComparison* sc)
{
  CALL("storedConstraintGreater(SimplificationOrdering::SpecialisedComparison* sc)");
  StoredConstraint* result = ((SpecialisedComparison*)sc)->storedConstraintGreater();
  result->incReferenceCounter();
  ASSERT(result->referenceCounter() >= 2L);
  return (SimplificationOrdering::StoredConstraint*)result;
}; // SimplificationOrdering::StoredConstraint* StandardKBOrdering::storedConstraintGreater(SimplificationOrdering::SpecialisedComparison* sc)

SimplificationOrdering::StoredConstraint*
StandardKBOrdering::storedConstraintGreaterOrEqual(SimplificationOrdering::SpecialisedComparison* sc)
{
  CALL("storedConstraintGreaterOrEqual(SimplificationOrdering::SpecialisedComparison* sc)");
  StoredConstraint* result = ((SpecialisedComparison*)sc)->storedConstraintGreaterOrEqual();
  result->incReferenceCounter();
  ASSERT(result->referenceCounter() >= 2L);
  return (SimplificationOrdering::StoredConstraint*)result;
}; // SimplificationOrdering::StoredConstraint* StandardKBOrdering::storedConstraintGreaterOrEqual(SimplificationOrdering::SpecialisedComparison* sc)

SimplificationOrdering::StoredConstraint*
StandardKBOrdering::storedConstraintLess(SimplificationOrdering::SpecialisedComparison* sc)
{
  CALL("storedConstraintLess(SimplificationOrdering::SpecialisedComparison* sc)");
  StoredConstraint* result = ((SpecialisedComparison*)sc)->storedConstraintLess();
  result->incReferenceCounter();
  ASSERT(result->referenceCounter() >= 2L);
  return (SimplificationOrdering::StoredConstraint*)result;
}; // SimplificationOrdering::StoredConstraint* StandardKBOrdering::storedConstraintLess(SimplificationOrdering::SpecialisedComparison* sc)

SimplificationOrdering::StoredConstraint*
StandardKBOrdering::storedConstraintLessOrEqual(SimplificationOrdering::SpecialisedComparison* sc)
{
  CALL("storedConstraintLessOrEqual(SimplificationOrdering::SpecialisedComparison* sc)");
  StoredConstraint* result = ((SpecialisedComparison*)sc)->storedConstraintLessOrEqual();
  result->incReferenceCounter();
  ASSERT(result->referenceCounter() >= 2L);
  return (SimplificationOrdering::StoredConstraint*)result;
}; // SimplificationOrdering::StoredConstraint* StandardKBOrdering::storedConstraintLessOrEqual(SimplificationOrdering::SpecialisedComparison* sc)

SimplificationOrdering::StoredConstraint* StandardKBOrdering::copy(SimplificationOrdering::StoredConstraint* constr)
{
  CALL("copy(SimplificationOrdering::StoredConstraint* constr)");
  ASSERT(constr);
  ((StoredConstraint*)constr)->incReferenceCounter();
  return constr;
}; // SimplificationOrdering::StoredConstraint* StandardKBOrdering::copy(SimplificationOrdering::StoredConstraint* constr)


void StandardKBOrdering::releaseConstraint(SimplificationOrdering::StoredConstraint* constr)
{
  CALL("releaseConstraint(SimplificationOrdering::StoredConstraint* constr)");
  ASSERT(constr);
  ((StoredConstraint*)constr)->decReferenceCounter();
  if (!((StoredConstraint*)constr)->referenceCounter())
    delete ((StoredConstraint*)constr);
}; // void StandardKBOrdering::releaseConstraint(SimplificationOrdering::StoredConstraint* constr)



SimplificationOrdering::ConstraintCheckOnUnifier* StandardKBOrdering::reserveConstraintCheckOnUnifier()
{
  CALL("reserveConstraintCheckOnUnifier()");  
  ConstraintCheckOnUnifier* result = _constraintCheckOnUnifierPool;
  if (result)
    {
      _constraintCheckOnUnifierPool = _constraintCheckOnUnifierPool->getNextInPool();
    }
  else
    {
      result = new ConstraintCheckOnUnifier(this);
    };
  return (SimplificationOrdering::ConstraintCheckOnUnifier*)result;
}; // SimplificationOrdering::ConstraintCheckOnUnifier* StandardKBOrdering::reserveConstraintCheckOnUnifier()

void StandardKBOrdering::releaseConstraintCheckOnUnifier(SimplificationOrdering::ConstraintCheckOnUnifier* ccu)
{
  CALL("releaseConstraintCheckOnUnifier(SimplificationOrdering::ConstraintCheckOnUnifier* ccu)");

  ((StandardKBOrdering::ConstraintCheckOnUnifier*)ccu)->setNextInPool(_constraintCheckOnUnifierPool);
  _constraintCheckOnUnifierPool = (StandardKBOrdering::ConstraintCheckOnUnifier*)ccu;
}; // void StandardKBOrdering::releaseConstraintCheckOnUnifier(SimplificationOrdering::ConstraintCheckOnUnifier* ccu)



SimplificationOrdering::ConstraintCheckOnForwardMatchingSubst* 
StandardKBOrdering::reserveConstraintCheckOnForwardMatchingSubst()
{
  CALL("reserveConstraintCheckOnForwardMatchingSubst()");
  ConstraintCheckOnForwardMatchingSubst* result = _constraintCheckOnForwardMatchingSubstPool;
  if (result)
    {
      _constraintCheckOnForwardMatchingSubstPool = 
	_constraintCheckOnForwardMatchingSubstPool->getNextInPool();
    }
  else
    {
      result = new ConstraintCheckOnForwardMatchingSubst(this);
    };
  return (SimplificationOrdering::ConstraintCheckOnForwardMatchingSubst*)result;
}; // SimplificationOrdering::ConstraintCheckOnForwardMatchingSubst* StandardKBOrdering::reserveConstraintCheckOnForwardMatchingSubst()

void 
StandardKBOrdering::releaseConstraintCheckOnForwardMatchingSubst(SimplificationOrdering::ConstraintCheckOnForwardMatchingSubst* ccfms)
{
  CALL("releaseConstraintCheckOnForwardMatchingSubst(..)");
  ((StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst*)ccfms)->
    setNextInPool(_constraintCheckOnForwardMatchingSubstPool);
  _constraintCheckOnForwardMatchingSubstPool = 
    (StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst*)ccfms;
}; // StandardKBOrdering::releaseConstraintCheckOnForwardMatchingSubst(SimplificationOrdering::ConstraintCheckOnForwardMatchingSubst* ccfms)


SimplificationOrdering::CompiledConstraintForUnifiers* 
StandardKBOrdering::reserveCompiledConstraintForUnifiers()
{
  CALL("reserveCompiledConstraintForUnifiers()");
  StandardKBOrdering::CompiledConstraintForUnifiers* result = _compiledConstraintOnUnifiersPool;
  if (result)
    {
      _compiledConstraintOnUnifiersPool = result->getNextInPool();
    }
  else
    {
      result = new CompiledConstraintForUnifiers(this);
    };
  ASSERT(result);
  return (SimplificationOrdering::CompiledConstraintForUnifiers*)result;
}; // SimplificationOrdering::CompiledConstraintForUnifiers* StandardKBOrdering::reserveCompiledConstraintForUnifiers()

void 
StandardKBOrdering::releaseCompiledConstraintForUnifiers(SimplificationOrdering::CompiledConstraintForUnifiers* cc)
{
  CALL("releaseCompiledConstraintForUnifiers(SimplificationOrdering::CompiledConstraintForUnifiers* cc)");
  
  ((StandardKBOrdering::CompiledConstraintForUnifiers*)cc)->setNextInPool(_compiledConstraintOnUnifiersPool);
  _compiledConstraintOnUnifiersPool = (StandardKBOrdering::CompiledConstraintForUnifiers*)cc;
}; // void StandardKBOrdering::releaseCompiledConstraintForUnifiers(SimplificationOrdering::CompiledConstraintForUnifiers* cc)



SimplificationOrdering::CompiledConstraintForInstanceRetrieval* 
StandardKBOrdering::reserveCompiledConstraintForInstanceRetrieval()
{
  CALL("reserveCompiledConstraintForInstanceRetrieval()");
  StandardKBOrdering::CompiledConstraintForInstanceRetrieval* result = _compiledConstraintForInstanceRetrievalPool;
  if (result)
    {
      _compiledConstraintForInstanceRetrievalPool = result->getNextInPool();
    }
  else
    {
      result = new CompiledConstraintForInstanceRetrieval(this);
    };
  ASSERT(result);
  return (SimplificationOrdering::CompiledConstraintForInstanceRetrieval*)result;
}; // SimplificationOrdering::CompiledConstraintForInstanceRetrieval* StandardKBOrdering::reserveCompiledConstraintForInstanceRetrieval()

void 
StandardKBOrdering::releaseCompiledConstraintForInstanceRetrieval(SimplificationOrdering::CompiledConstraintForInstanceRetrieval* cc)
{
  CALL("releaseCompiledConstraintForInstanceRetrieval(SimplificationOrdering::CompiledConstraintForInstanceRetrieval* cc)");

  ((StandardKBOrdering::CompiledConstraintForInstanceRetrieval*)cc)->setNextInPool(_compiledConstraintForInstanceRetrievalPool);
  _compiledConstraintForInstanceRetrievalPool = (StandardKBOrdering::CompiledConstraintForInstanceRetrieval*)cc;
}; // void StandardKBOrdering::releaseCompiledConstraintForInstanceRetrieval(SimplificationOrdering::CompiledConstraintForInstanceRetrieval* cc)




Comparison StandardKBOrdering::compareWithTransposition(const Flatterm* term)
{  CALL("compareWithTransposition(const Flatterm* term)");
 ASSERT(term->isComplex());
 ASSERT(term->arity() == 2);
 const Flatterm* arg1 = term->next();
 const Flatterm* arg2 = arg1->after();
 return compare(arg1,arg2);
}; // Comparison StandardKBOrdering::compareWithTransposition(const Flatterm* term)

inline
Comparison StandardKBOrdering::compare(const Flatterm* term1,
				       const Flatterm* term2)
{
  CALL("compare(const Flatterm* term1,const Flatterm* term2)");
 _weight1.reset();
 _weight2.reset();
 TermWeighting::current()->collectWeight(_weight1,term1);
 TermWeighting::current()->collectWeight(_weight2,term2);
 return compare(term1,_weight1,term2,_weight2);
}; // Comparison StandardKBOrdering::compare(const Flatterm* term1,const Flatterm* term2)



inline
Comparison 
StandardKBOrdering::compareLexicographically(const Flatterm* term1,
					     const Flatterm* term2)
{
  CALL("compareLexicographically(const Flatterm* term1,..)");
  if (term1->symbolRef().isVariable())
    {
      if (term1->symbolRef() == term2->symbolRef()) return Equal;
      if (term2->containsSymbol(term1->symbolRef()))
	{
	  return Less;
	};
      return Incomparable;
    }
  else
    if (term2->symbolRef().isVariable())
      {
        if (term1->containsSymbol(term2->symbolRef()))
	  {
	    return Greater;
	  };
	return Incomparable;
      }
    else
      {
	// both are complex
	if (term1->symbolRef() != term2->symbolRef())
	  return term1->symbolRef().comparePrecedence(term2->symbolRef());
	Flatterm::Iterator iter1(term1);
	Flatterm::Iterator iter2(term2);
	iter1.NextSym();
	iter2.NextSym();
        while (iter1.NotEndOfTerm())
	  {
	    Comparison cmp = compare(iter1.CurrPos(),iter2.CurrPos());
	    if (cmp != Equal) return cmp;
	    iter1.after();
	    iter2.after();
	  };
	ASSERT(iter2.EndOfTerm());
	return Equal;
      };

}; // Comparison StandardKBOrdering::compareLexicographically(..)

inline
Comparison 
StandardKBOrdering::compareAtomsLexicographically(const Flatterm* lit1,
						  const Flatterm* lit2)
{
  CALL("compareAtomsLexicographically(..)");
  if (lit1->functor() != lit2->functor()) 
    return lit1->symbolRef().comparePrecedence(lit2->symbolRef());

  Flatterm::Iterator iter1(lit1);
  Flatterm::Iterator iter2(lit2);
  iter1.NextSym();
  iter2.NextSym();
  while (iter1.NotEndOfTerm())
    {
      Comparison cmp = compare(iter1.CurrPos(),iter2.CurrPos());
      if (cmp != Equal) return cmp;
      iter1.after();
      iter2.after();
    };
  ASSERT(iter2.EndOfTerm());
  return Equal;
}; // Comparison StandardKBOrdering::compareAtomsLexicographically(..)



inline
Comparison StandardKBOrdering::compareLexicographically(const TERM* complexTerm1,
							const TERM* complexTerm2)
{
  CALL("compareLexicographically(const TERM* complexTerm1,const TERM* complexTerm2)");

  if ((*complexTerm1) != (*complexTerm2)) 
    return complexTerm1->comparePrecedence(*complexTerm2);
  const TERM* arg1 = complexTerm1->Args();
  const TERM* arg2 = complexTerm2->Args();
  while (!arg1->IsNull())
    {
      Comparison cmp = 
	compare(*arg1,*arg2);
      if (cmp != Equal) return cmp;
      arg1 = arg1->Next();
      arg2 = arg2->Next();
    };
  return Equal;

}; // Comparison StandardKBOrdering::compareLexicographically(..)

inline
Comparison StandardKBOrdering::compareLexicographically(const TERM& term1,
							const TERM& term2)
{
  CALL("compareLexicographically(const TERM& term1,..)");
  if (term1 == term2) return Equal;
  if (term1.isVariable())
    {
      if (term2.isVariable()) return Incomparable;
      if (term2.First()->containsSymbol(term1)) return Less;
      return Incomparable;
    }
  else
    if (term2.isVariable())
      {
	if (term1.First()->containsSymbol(term2)) return Greater;	
	return Incomparable;
      }
    else // both are references
      {
	ASSERT(term1.IsReference() && term2.IsReference());
	return compareLexicographically(term1.First(),term2.First());
      };
}; // Comparison StandardKBOrdering::compareLexicographically(const TERM& term1,..)


inline
bool 
StandardKBOrdering::termWithUnifierContainsVariable(PREFIX_SYM* complexTerm,
						    Unifier::Variable* var)
{ 
  CALL("termWithUnifierContainsVariable(PREFIX_SYM* complexTerm,Unifier::Variable* var)");
  PREFIX::CorrectBracket(complexTerm);
  PrefixIterPool::AutoObject iter(_prefixIterPool);
  iter.object().Reset(complexTerm);
  iter.object().SkipSym();
  while (iter.object().NotEmpty())
    {
      if (iter.object().CurrVar() == var) return true;
      iter.object().SkipSym();
    };
  return false;
}; // bool StandardKBOrdering::termWithUnifierContainsVariable(PREFIX_SYM* complexTerm,Unifier::Variable* var)

inline
Comparison 
StandardKBOrdering::compareLexOnCurrentUnifier(PREFIX_SYM* complexTerm1,
					       PREFIX_SYM* complexTerm2)
{
  CALL("compareLexOnCurrentUnifier(PREFIX_SYM* complexTerm1,PREFIX_SYM* complexTerm2)");

  //DF; cout << "COMP LEX\n";
  //DF; PREFIX::outputWordWithSubst(cout,complexTerm1) << "\n   WITH\n";
  //DF; PREFIX::outputWordWithSubst(cout,complexTerm2) << "\n";

  if (complexTerm1->Head() != complexTerm2->Head())
    return complexTerm1->Head().Func().comparePrecedence(complexTerm2->Head().Func());  
  if (!complexTerm1->Head().Func().arity()) return Equal;
  
  PrefixIterPool::AutoObject iter1(_prefixIterPool);
  PrefixIterPool::AutoObject iter2(_prefixIterPool);
  PREFIX::CorrectBracket(complexTerm1);
  PREFIX::CorrectBracket(complexTerm2);
  iter1.object().Reset(complexTerm1);
  iter2.object().Reset(complexTerm2);
  iter1.object().SkipSym();
  iter2.object().SkipSym();
  do
    {
      Unifier::Variable* var1 = iter1.object().CurrVar();
      Unifier::Variable* var2 = iter2.object().CurrVar();
      
      if (var1)
	{
	  if (var2)
	    {
	      if (var1 != var2) return Incomparable;
	      iter1.object().SkipSym();
	      iter2.object().SkipSym();
	    }
	  else
	    {
	      // check if iter2.object().CurrSubterm() contains var1
	      if (termWithUnifierContainsVariable(iter2.object().CurrSubterm(),var1))
		return Less;
	      return Incomparable;
	    };
	}
      else
	if (var2)
	  {
	    // check if iter1.object().CurrSubterm() contains var2
	    if (termWithUnifierContainsVariable(iter1.object().CurrSubterm(),var2))
	      return Greater;
	    return Incomparable;
	  }
	else // both subterms are complex
	  {
	    PREFIX::CorrectBracket(iter1.object().CurrSubterm());
	    PREFIX::CorrectBracket(iter2.object().CurrSubterm());
	    Comparison cmp = compareOnCurrentUnifier(iter1.object().CurrSubterm(),
						     iter2.object().CurrSubterm());
	    if (cmp != Equal) return cmp;
	    iter1.object().SkipTerm();
	    iter2.object().SkipTerm();
	  };
    }
  while (iter1.object().NotEmpty());
  ASSERT(iter2.object().Empty());
  return Equal;
}; // Comparison StandardKBOrdering::compareLexOnCurrentUnifier(..)


inline
bool 
StandardKBOrdering::termWithUnifierContainsVariable(const TERM* complexTerm,
						    ulong variableIndex,
						    Unifier::Variable* var)
{
  CALL("termWithUnifierContainsVariable(const TERM* complexTerm,ulong variableIndex,Unifier::Variable* var)");

  StTermWithSubstPool::AutoObject iter(_stTermWithSubstPool);
  iter.object().Reset(variableIndex,(TERM*)complexTerm);
  iter.object().SkipSym();
  while (!iter.object().Empty())
    {
      if (iter.object().CurrSym().isVariable() &&
	  (iter.object().CurrSym().var() == var))
	return true;
      iter.object().SkipSym();
    };
  return false;
}; // bool StandardKBOrdering::termWithUnifierContainsVariable(..)


inline
bool 
StandardKBOrdering::termWithUnifierContainsVariable(const TERM& term,
						    ulong variableIndex,
						    Unifier::Variable* var)
{
  CALL("termWithUnifierContainsVariable(const TERM& term,ulong variableIndex,Unifier::Variable* var)");
  if (term.isVariable())
    {
      Unifier::Variable* var = 
	Unifier::current()->variable(term.var(),variableIndex);
      bool freeVar;
      void* instance = var->Unref(freeVar);
      if (freeVar)
	{
	  return ((Unifier::Variable*)instance) == var;
	}
      else
	{
	  return termWithUnifierContainsVariable((PREFIX_SYM*)instance,var);
	};

    } 
  else
    {
      return termWithUnifierContainsVariable(term.First(),
					     variableIndex,
					     var);
    };
  
}; // bool StandardKBOrdering::termWithUnifierContainsVariable(..)


inline
Comparison 
StandardKBOrdering::compareOnCurrentUnifier(PREFIX_SYM* complexTerm1,
					    const TERM* complexTerm2,
					    ulong variableIndex)
{
  CALL("compareOnCurrentUnifier(PREFIX_SYM* complexTerm1,const TERM* complexTerm2,ulong variableIndex)");
  _weight1.reset();
  _weight2.reset();
  
  TermWeighting::current()->collectWeight(_weight1,complexTerm1);
  TermWeighting::current()->collectWeight(_weight2,
					  TERM((TERM*)complexTerm2),
					  variableIndex);

  Comparison lexCmp;
  switch (_weight1.compare(_weight2))
    {
    case BK::FailureToCompare:
      return Incomparable;
    case BK::AlwaysLess:
      return Less;
    case BK::AlwaysEqual:
      return compareLexOnCurrentUnifier(complexTerm1,complexTerm2,variableIndex);
    case BK::AlwaysGreater:
      return Greater;
    case BK::CanBeLessOrEqual:
      lexCmp = compareLexOnCurrentUnifier(complexTerm1,complexTerm2,variableIndex);
      if (lexCmp == Less) return Less;
      ASSERT(lexCmp != Equal);
      return Incomparable;
      
    case BK::CanBeGreaterOrEqual:
      lexCmp = compareLexOnCurrentUnifier(complexTerm1,complexTerm2,variableIndex);
      if (lexCmp == Greater) return Greater;
      ASSERT(lexCmp != Equal);
      return Incomparable;

    case BK::VolatileButNearlyAlwaysLess:
      // same as below
    case BK::VolatileButNearlyAlwaysGreater:
      // same as below
    case BK::AbsolutelyVolatile:
      return Incomparable;
    };

  ICP("ICP0");
#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE)
  return Incomparable;
#endif
}; // Comparison StandardKBOrdering::compareOnCurrentUnifier(..)


inline
Comparison 
StandardKBOrdering::compareLexOnCurrentUnifier(PREFIX_SYM* complexTerm1,
					       const TERM* complexTerm2,
					       ulong variableIndex)
{
  CALL("compareLexOnCurrentUnifier(PREFIX_SYM* complexTerm1,const TERM* complexTerm2,ulong variableIndex)");

  if (complexTerm1->Head().Func() != (*complexTerm2))
    return complexTerm1->Head().Func().comparePrecedence(*complexTerm2);
  
  PREFIX::CorrectBracket(complexTerm1);
  PrefixIterPool::AutoObject iter1(_prefixIterPool);
  iter1.object().Reset(complexTerm1);
  iter1.object().SkipSym();
  const TERM* arg2 = complexTerm2->Args();
  while (iter1.object().NotEmpty())
    {
      Unifier::Variable* var1 = iter1.object().CurrVar();
      if (var1)
	{
          if (arg2->isVariable())
	    {
	      Unifier::Variable* var2 = 
		Unifier::current()->variable(arg2->var(),variableIndex);
	      bool freeVar2;
	      void* instance2 = var2->Unref(freeVar2);
	      if (freeVar2)
		{
		  if (((Unifier::Variable*)instance2) == var1)
		    return Less;
		}
	      else
		{
		  if (termWithUnifierContainsVariable((PREFIX_SYM*)instance2,
						      var1))
		    return Less;
		};	    
	      return Incomparable;  
	    }
	  else
	    {
	      if (termWithUnifierContainsVariable(arg2->First(),
						  variableIndex,
						  var1))
		return Less;
	      return Incomparable;
	    };	    
	}
      else // iter1.object().CurrSubterm() is complex
	{
	  PREFIX::CorrectBracket(iter1.object().CurrSubterm());
	  if (arg2->isVariable())
	    {
	      Unifier::Variable* var2 = 
		Unifier::current()->variable(arg2->var(),variableIndex);
	      bool freeVar2;
	      void* instance2 = var2->Unref(freeVar2);
	      if (freeVar2)
		{
		  if (termWithUnifierContainsVariable(iter1.object().CurrSubterm(),
						      (Unifier::Variable*)instance2))
		    return Greater;
		  return Incomparable;
		}
	      else
		{
		  Comparison cmp = 
		    compareOnCurrentUnifier(iter1.object().CurrSubterm(),
					    (PREFIX_SYM*)instance2);
		  if (cmp != Equal) return cmp;
		};	

	    }
	  else // both arguments are complex
	    {
	      Comparison cmp = 
		compareOnCurrentUnifier(iter1.object().CurrSubterm(),
					arg2->First(),
					variableIndex); 
	      if (cmp != Equal) return cmp;
	    };
	};

      iter1.object().SkipTerm();
      arg2 = arg2->Next();
    };
  return Equal;
}; // Comparison StandardKBOrdering::compareLexOnCurrentUnifier(..)



inline
Comparison 
StandardKBOrdering::compareLexOnCurrentUnifier(PREFIX_SYM* complexTerm1,
					       const TERM& term2,
					       ulong variableIndex)
{
  CALL("compareLexOnCurrentUnifier(PREFIX_SYM* complexTerm1,const TERM& term2,ulong variableIndex)");
  if (term2.isVariable())
    {
      
      Unifier::Variable* var2 = 
	Unifier::current()->variable(term2.var(),variableIndex);
      bool freeVar2;
      void* instance2 = var2->Unref(freeVar2);
      if (freeVar2)
	{
	  if (termWithUnifierContainsVariable(complexTerm1,
					      (Unifier::Variable*)instance2))
	    return Greater;
	  return Incomparable;
	}
      else
	{
	  return compareLexOnCurrentUnifier(complexTerm1,
					    (PREFIX_SYM*)instance2);
	};
    }
  else
    return compareLexOnCurrentUnifier(complexTerm1,
				      term2.First(),
				      variableIndex);
}; // Comparison StandardKBOrdering::compareLexOnCurrentUnifier(..)



inline
Comparison 
StandardKBOrdering::compareLexOnCurrentUnifier(const TERM& term1,
					       const TERM& term2,
					       ulong variableIndex)
{
  CALL("compareLexOnCurrentUnifier(const TERM& term1,const TERM& term2,ulong variableIndex)");

  if (term1 == term2) return Equal;
  if (term1.isVariable())
    {
      Unifier::Variable* var1 = 
	Unifier::current()->variable(term1.var(),variableIndex);
      bool freeVar1;
      void* instance1 = var1->Unref(freeVar1);
      if (freeVar1)
	{
	  if (term2.isVariable())
	    {
	      Unifier::Variable* var2 = 
		Unifier::current()->variable(term2.var(),variableIndex);
	      bool freeVar2;
	      void* instance2 = var2->Unref(freeVar2);
	      if (freeVar2)
		{
		  return (instance1 == instance2) ? Equal : Incomparable;
		}
	      else
		{
		  if (termWithUnifierContainsVariable((PREFIX_SYM*)instance2,
						      (Unifier::Variable*)instance1))
		    return Less;
		  return Incomparable;
		};

	    }
	  else // term2 is a reference
	    {
	      if (termWithUnifierContainsVariable(term2.First(),
						  variableIndex,
						  (Unifier::Variable*)instance1))
		return Less;
	      return Incomparable;
	    };
	}
      else
	{
	  return 
	    compareLexOnCurrentUnifier((PREFIX_SYM*)instance1,
				       term2,
				       variableIndex);
	};
    }
  else // term1 is a reference
    if (term2.isVariable())
      {
	Unifier::Variable* var2 = 
	  Unifier::current()->variable(term2.var(),variableIndex);
	bool freeVar2;
	void* instance2 = var2->Unref(freeVar2);
	if (freeVar2)
	  {
	    if (termWithUnifierContainsVariable(term1.First(),
						variableIndex,
						(Unifier::Variable*)instance2))
	      return Greater;
	    return Incomparable;
	  }
	else
	  {
	    Comparison cmp =
	      compareLexOnCurrentUnifier((PREFIX_SYM*)instance2,
					 term1.First(),
					 variableIndex);
	    return BK::inverse(cmp);
	  };
      }
    else // both are references
      {
	if ((*term1.First()) != (*term2.First()))
	  {
	    return term1.First()->comparePrecedence(*term2.First());
	  };
	const TERM* arg1 = term1.First()->Args();
	const TERM* arg2 = term2.First()->Args();
	while (!arg1->IsNull())
	  {
	    Comparison cmp = 
	      compareOnCurrentUnifier(*arg1,*arg2,variableIndex);
	    if (cmp != Equal) return cmp;
	    arg1 = arg1->Next();
	    arg2 = arg2->Next();
	  };
	return Equal;
      };
}; // Comparison StandardKBOrdering::compareLexOnCurrentUnifier(const TERM& term1,const TERM& term2,ulong variableIndex)

inline
Comparison 
StandardKBOrdering::compareLexicographically(const TERM* complexTerm1,
					     const InstanceRetrievalForDemodulation::Substitution* subst,
					     const TERM* complexTerm2)
{
  CALL("compareLexicographically(const TERM* complexTerm1,const InstanceRetrievalForDemodulation::Substitution* subst,const TERM* complexTerm2)");
  if ((*complexTerm1) != (*complexTerm2))
    return complexTerm1->comparePrecedence(*complexTerm2);
  const TERM* arg1 = complexTerm1->Args();
  const TERM* arg2 = complexTerm2->Args();
  while (!arg1->IsNull())
    {
      if (arg1->isVariable())
	{
	  TERM instance = subst->map(arg1->var());
	  Comparison cmp = compare(instance,*arg2);
	  if (cmp != Equal) return cmp;
	}
      else
	{
	  Comparison cmp = compare(arg1->First(),subst,*arg2);
	  if (cmp != Equal) return cmp;
	};
      arg1 = arg1->Next();      
      arg2 = arg2->Next();      
    };
  ASSERT(arg2->IsNull());
  return Equal;
}; // Comparison StandardKBOrdering::compareLexicographically(..)



inline
Comparison 
StandardKBOrdering::compare(const TERM* complexTerm1,
			    const InstanceRetrievalForDemodulation::Substitution* subst,
			    const TERM& term2)
{
  CALL("compare(const TERM* complexTerm1,const InstanceRetrievalForDemodulation::Substitution* subst,const TERM& term2)");
  if (term2.isVariable())
    {
      // check if complexTerm1 contains the variable
      InstanceRetrievalForDemodulation::Substitution::Instance iter1;
      iter1.reset(complexTerm1,subst);
      while (iter1.next())
	{
	  if (iter1.symbol() == term2) return Greater;
	};
      return Incomparable;
    }
  else
    {
      _weight1.reset();
      _weight2.reset();
      TermWeighting::current()->collectWeight(_weight1,complexTerm1,subst);
      TermWeighting::current()->collectWeight(_weight2,term2);
      Comparison lexCmp;
      switch (_weight1.compare(_weight2))
	{
	case BK::FailureToCompare:
	  return Incomparable;
	case BK::AlwaysLess:
	  return Less;
	case BK::AlwaysEqual:
	  return compareLexicographically(complexTerm1,subst,term2.First());
	case BK::AlwaysGreater:
	  return Greater;
	case BK::CanBeLessOrEqual:
	  lexCmp = compareLexicographically(complexTerm1,subst,term2.First());
	  if (lexCmp == Less) return Less;
	  ASSERT(lexCmp != Equal);
	  return Incomparable;
      
	case BK::CanBeGreaterOrEqual:
	  lexCmp = compareLexicographically(complexTerm1,subst,term2.First());
	  if (lexCmp == Greater) return Greater;
	  ASSERT(lexCmp != Equal);
	  return Incomparable;

	case BK::VolatileButNearlyAlwaysLess:
	  // same as below
	case BK::VolatileButNearlyAlwaysGreater:
	  // same as below
	case BK::AbsolutelyVolatile:
	  return Incomparable;
	};
 
    };
  ICP("ICP0");
#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE)
  return Incomparable;
#endif

}; // Comparison StandardKBOrdering::compare(..)


inline 
Comparison 
StandardKBOrdering::compare(const TERM* complexTerm1,const TERM* complexTerm2)
{
  CALL("compare(const TERM* complexTerm1,const TERM* complexTerm2)");
  _weight1.reset();
  _weight2.reset();
  TermWeighting::current()->collectWeight(_weight1,TERM((TERM*)complexTerm1));
  TermWeighting::current()->collectWeight(_weight2,TERM((TERM*)complexTerm2));
  
  Comparison lexCmp;
  switch (_weight1.compare(_weight2))
    {
    case BK::FailureToCompare:
      return Incomparable;
    case BK::AlwaysLess:
      return Less;
    case BK::AlwaysEqual:
      return compareLexicographically(complexTerm1,complexTerm2);
    case BK::AlwaysGreater:
      return Greater;
    case BK::CanBeLessOrEqual:
      lexCmp = compareLexicographically(complexTerm1,complexTerm2);
      if (lexCmp == Less) return Less;
      ASSERT(lexCmp != Equal);
      return Incomparable;
      
    case BK::CanBeGreaterOrEqual:
      lexCmp = compareLexicographically(complexTerm1,complexTerm2);
      if (lexCmp == Greater) return Greater;
      ASSERT(lexCmp != Equal);
      return Incomparable;
      
    case BK::VolatileButNearlyAlwaysLess:
      // same as below
    case BK::VolatileButNearlyAlwaysGreater:
      // same as below
    case BK::AbsolutelyVolatile:
      return Incomparable;
    };

  ICP("ICP0");
#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE)
  return Incomparable;
#endif
}; // Comparison StandardKBOrdering::compare(..)


Comparison 
StandardKBOrdering::compare(const TERM& term1,
			    const TERM& term2)
{
  CALL("compare(const TERM& term1,const TERM& term2)");
  if (term1.isVariable())
    {
      if (term2.isVariable())
	{
	  if (term1 == term2) return Equal;
	}
      else
	{
	  if (term2.First()->containsSymbol(term1)) return Less;
	};
      return Incomparable;
    }
  else
    if (term2.isVariable())
      {
	if (term1.First()->containsSymbol(term2)) return Greater;
	return Incomparable;
      }
    else // both are references 
      return compare(term1.First(),term2.First());
}; // Comparison StandardKBOrdering::compare(const TERM& term1,const TERM& term2)


inline 
Comparison 
StandardKBOrdering::compare(const TERM& term1,
			    const TERM& term2,
			    const InstanceRetrievalForDemodulation::Substitution* subst)
{
  CALL("compare(const TERM& term1,const TERM& term2,const InstanceRetrievalForDemodulation::Substitution* subst)");
  if (term1.isVariable())
    {
      if (term2.isVariable())
	{
	  return compare(subst->map(term1.var()),subst->map(term2.var()));
	}
      else
	{
	  Comparison cmp =
	    compare(term2.First(),subst,subst->map(term1.var()));
	  return BK::inverse(cmp);
	};
    }
  else
    if (term2.isVariable())
      {
	return compare(term1.First(),subst,subst->map(term2.var()));
      }
    else
      return compare(term1.First(),term2.First(),subst);
      
}; // Comparison StandardKBOrdering::compare(..)


inline 
Comparison 
StandardKBOrdering::compareLexicographically(const TERM* complexTerm1,
					     const TERM* complexTerm2,
					     const InstanceRetrievalForDemodulation::Substitution* subst)
{
  CALL("compareLexicographically(const TERM* complexTerm1,const TERM* complexTerm2,const InstanceRetrievalForDemodulation::Substitution* subst)");
  if ((*complexTerm1) != (*complexTerm2))
    return complexTerm1->comparePrecedence(*complexTerm2);
  const TERM* arg1 = complexTerm1->Args();      
  const TERM* arg2 = complexTerm2->Args();
  while (!arg1->IsNull())
    {
      Comparison cmp = compare(*arg1,*arg2,subst);
      if (cmp != Equal) return cmp;
      arg1 = arg1->Next();
      arg2 = arg2->Next();
    };
  ASSERT(arg2->IsNull());
  return Equal;
}; // Comparison StandardKBOrdering::compareLexicographically(const TERM* complexTerm1,const TERM* complexTerm2,const InstanceRetrievalForDemodulation::Substitution* subst)

inline
Comparison 
StandardKBOrdering::compare(const TERM& term1,
			    const ForwardMatchingSubstitutionCache* subst,
			    const Flatterm* term2)
{
  CALL("compare(const TERM& term1,const ForwardMatchingSubstitutionCache* subst,const Flatterm* term2)");

  //DF; cout << "COMPARE " << term1 << "\n";
  //DF; cout << "WITH " << term2 << "\n";
  //DF; cout << "AFTER SUBST " << subst << "\n";

  
  if (term1.isVariable())
    {
      return compare(subst->map(term1.var()),term2);
    }
  else
    {
      if (term2->isVariable())
	{
	  ForwardMatchingSubstitutionCache::Instance iter1;
	  iter1.reset(term1.First(),subst);
	  while (iter1.next())
	    {
	      if (iter1.symbol() == term2->symbolRef())
		return Greater;
	    };
	  return Incomparable;
	}
      else
	{
	  const TERM* complexTerm1 = term1.First();
	  _weight1.reset();
	  _weight2.reset();
	  TermWeighting::current()->collectWeight(_weight1,term1,subst);
	  TermWeighting::current()->collectWeight(_weight2,term2);
	  Comparison lexCmp;
	  switch (_weight1.compare(_weight2))
	    {
	    case BK::FailureToCompare:
	      return Incomparable;
	    case BK::AlwaysLess:
	      return Less;
	    case BK::AlwaysEqual:
	      return compareLexicographically(complexTerm1,subst,term2);
	    case BK::AlwaysGreater:
	      return Greater;
	    case BK::CanBeLessOrEqual:
	      lexCmp = compareLexicographically(complexTerm1,subst,term2);
	      if (lexCmp == Less) return Less;
	      ASSERT(lexCmp != Equal);
	      return Incomparable;
	      
	    case BK::CanBeGreaterOrEqual:
	      lexCmp = compareLexicographically(complexTerm1,subst,term2);
	      if (lexCmp == Greater) return Greater;
	      ASSERT(lexCmp != Equal);
	      return Incomparable;
	      
	    case BK::VolatileButNearlyAlwaysLess:
	      // same as below
	    case BK::VolatileButNearlyAlwaysGreater:
	      // same as below
	    case BK::AbsolutelyVolatile:
	      return Incomparable;
	    };  	  
	};
    };
  ICP("ICP0");
#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE)
  return Incomparable;
#endif
}; // Comparison StandardKBOrdering::compare(..)


inline
Comparison 
StandardKBOrdering::compareLexicographically(const TERM* complexTerm1,
					     const ForwardMatchingSubstitutionCache* subst,
					     const Flatterm* term2)
{
  CALL("compareLexicographically(const TERM* complexTerm1,const ForwardMatchingSubstitutionCache* subst,const Flatterm* term2)");
  if (term2->isVariable())
    {
      ForwardMatchingSubstitutionCache::Instance iter1;
      iter1.reset(complexTerm1,subst);
      while (iter1.next())
	{
	  if (iter1.symbol() == term2->symbolRef()) return Greater;
	};
      return Incomparable;
    }
  else
    {
      if ((*complexTerm1) != term2->symbolRef())
	return complexTerm1->comparePrecedence(term2->symbolRef());
      const TERM* arg1 = complexTerm1->Args();
      Flatterm::Iterator iter2(term2);
      iter2.NextSym();
      while (!arg1->IsNull())
	{
	  Comparison cmp = compare(*arg1,subst,iter2.CurrPos());
	  if (cmp != Equal) return cmp;
	  arg1 = arg1->Next();
	  iter2.after();
	};
      ASSERT(iter2.EndOfTerm());
      return Equal;
    };
}; // Comparison StandardKBOrdering::compareLexicographically(..)



inline
Comparison 
StandardKBOrdering::compareLexicographically(const TERM& term1,
					     const TERM& term2,
					     const ForwardMatchingSubstitutionCache* subst)
{
  CALL("compareLexicographically(const TERM& term1,const TERM& term2,const ForwardMatchingSubstitutionCache* subst)");
  if (term1.isVariable())
    {
      if (term2.isVariable())
	{
	  return compareLexicographically(subst->map(term1.var()),
					  subst->map(term2.var()));
	}
      else
	{
	  Comparison cmp = compareLexicographically(term2.First(),
						    subst,
						    subst->map(term1.var()));
	  return BK::inverse(cmp);
	};
    }
  else
    if (term2.isVariable())
      {
	return compareLexicographically(term1.First(),
					subst,
					subst->map(term2.var()));
      }
    else
      {
	const TERM* complexTerm1 = term1.First();
	const TERM* complexTerm2 = term2.First();
	if ((*complexTerm1) != (*complexTerm2))
	  return complexTerm1->comparePrecedence(*complexTerm2);
	const TERM* arg1 = complexTerm1->Args();
	const TERM* arg2 = complexTerm2->Args();
	while (!arg1->IsNull())
	  {
	    Comparison cmp = compare(*arg1,*arg2,subst);
	    if (cmp != Equal) return cmp;
	    arg1 = arg1->Next();
	    arg2 = arg2->Next();
	  };
	ASSERT(arg2->IsNull());
	return Equal;
      };     
}; // Comparison StandardKBOrdering::compareLexicographically(const TERM& term1,const TERM& term2,const ForwardMatchingSubstitutionCache* subst)



//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_STANDARD_KB_ORDERING
#define DEBUG_NAMESPACE "StandardKBOrdering::SpecialisedComparison"
#endif
#include "debugMacros.h"
//===================================================================

StandardKBOrdering::SpecialisedComparison::~SpecialisedComparison()
{ 
  CALL("destructor ~SpecialisedComparison()");
  
  if (DestructionMode::isThorough())
    {  
      if (_constraintGreater)
	{
	  _constraintGreater->decReferenceCounter();
	  if (!_constraintGreater->referenceCounter())
	    delete _constraintGreater;
	};
      if (_constraintGreaterOrEqual)
	{
	  _constraintGreaterOrEqual->decReferenceCounter();
	  if (!_constraintGreaterOrEqual->referenceCounter())
	    delete _constraintGreaterOrEqual;
	};

      if (_constraintLess)
	{
	  _constraintLess->decReferenceCounter();
	  if (!_constraintLess->referenceCounter())
	    delete _constraintLess;
	};  
      if (_constraintLessOrEqual)
	{
	  _constraintLessOrEqual->decReferenceCounter();
	  if (!_constraintLessOrEqual->referenceCounter())
	    delete _constraintLessOrEqual;
	};
    };
}; // StandardKBOrdering::SpecialisedComparison::~SpecialisedComparison()

void StandardKBOrdering::SpecialisedComparison::load(const TERM& lhs,const TERM& rhs)
{
  CALL("load(const TERM& lhs,const TERM& rhs)");
  _lhs = lhs;
  _rhs = rhs;
  _lhsWeight.reset();
  _rhsWeight.reset();
  TermWeighting::current()->collectWeight(_lhsWeight,_lhs);
  TermWeighting::current()->collectWeight(_rhsWeight,_rhs);
  _specialised = false;

  if (_constraintGreater)
    {
      _constraintGreater->decReferenceCounter();
      if (!_constraintGreater->referenceCounter())
	delete _constraintGreater;
      _constraintGreater = (StoredConstraint*)0;
    };
  
  if (_constraintGreaterOrEqual)
    {
      _constraintGreaterOrEqual->decReferenceCounter();
      if (!_constraintGreaterOrEqual->referenceCounter())
	delete _constraintGreaterOrEqual;
      _constraintGreaterOrEqual = (StoredConstraint*)0;
    };

  if (_constraintLess)
    {
      _constraintLess->decReferenceCounter();
      if (!_constraintLess->referenceCounter())
	delete _constraintLess;
      _constraintLess = (StoredConstraint*)0;
    };
  
  if (_constraintLessOrEqual)
    {
      _constraintLessOrEqual->decReferenceCounter();
      if (!_constraintLessOrEqual->referenceCounter())
	delete _constraintLessOrEqual;
      _constraintLessOrEqual = (StoredConstraint*)0;
    };

  //DF; cout << "LOADED: " << *this << "\n";


}; // void StandardKBOrdering::SpecialisedComparison::load(const TERM& lhs,const TERM& rhs)

bool StandardKBOrdering::SpecialisedComparison::lhsVarInstMayOrder()
{
  CALL("lhsVarInstMayOrder()");
  return !_rhsWeight.containsVariableWhichIsNotIn(_lhsWeight);
}; // bool StandardKBOrdering::SpecialisedComparison::lhsVarInstMayOrder()

bool StandardKBOrdering::SpecialisedComparison::rhsVarInstMayOrder()
{
  CALL("rhsVarInstMayOrder()");
  return !_lhsWeight.containsVariableWhichIsNotIn(_rhsWeight);
}; // bool StandardKBOrdering::SpecialisedComparison::rhsVarInstMayOrder()


void StandardKBOrdering::SpecialisedComparison::specialiseGreater()
{
  specialise();
};

void StandardKBOrdering::SpecialisedComparison::specialiseGreaterOrEqual()
{
  specialise();
};

void StandardKBOrdering::SpecialisedComparison::specialiseLess()
{
  specialise();
};

void StandardKBOrdering::SpecialisedComparison::specialiseLessOrEqual()
{
  specialise();
};




inline
StandardKBOrdering::StoredConstraint* StandardKBOrdering::SpecialisedComparison::storedConstraintGreater()
{
  CALL("storedConstraintGreater()");
  if (!_constraintGreater) 
    {
      specialise();
      _constraintGreater = 
	new StoredConstraint(StoredConstraint::Greater,
			     _weightComparison,
			     _lexicographicComparisonResult,
			     _lhs,
			     _rhs);
      _constraintGreater->incReferenceCounter();      
    };

  return _constraintGreater;
}; // StandardKBOrdering::StoredConstraint* StandardKBOrdering::SpecialisedComparison::storedConstraintGreater() 


inline
StandardKBOrdering::StoredConstraint* StandardKBOrdering::SpecialisedComparison::storedConstraintGreaterOrEqual()
{
  CALL("storedConstraintGreaterOrEqual()");
  if (!_constraintGreaterOrEqual) 
    {
      specialise();
      _constraintGreaterOrEqual = 
	new StoredConstraint(StoredConstraint::GreaterOrEqual,
			     _weightComparison,
			     _lexicographicComparisonResult,
			     _lhs,
			     _rhs);
      _constraintGreaterOrEqual->incReferenceCounter();      
    };

  //DF; cout << "SPECIALISED (GREATER OR EQUAL): " << *this << "\n";
  //DF; cout << "STORED CONSTR (GREATER OR EQUAL): " << _constraintGreaterOrEqual << "\n";


  return _constraintGreaterOrEqual;
}; // StandardKBOrdering::StoredConstraint* StandardKBOrdering::SpecialisedComparison::storedConstraintGreaterOrEqual()


inline
StandardKBOrdering::StoredConstraint* 
StandardKBOrdering::SpecialisedComparison::storedConstraintLess()
{
  CALL("storedConstraintLess()");
  if (!_constraintLess) 
    {
      specialise();
      _constraintLess = 
	new StoredConstraint(StoredConstraint::Less,
			     _weightComparison,
			     _lexicographicComparisonResult,
			     _lhs,
			     _rhs);
      _constraintLess->incReferenceCounter();      
    };
  return _constraintLess;
}; // StandardKBOrdering::StoredConstraint* StandardKBOrdering::SpecialisedComparison::storedConstraintLess()

inline
StandardKBOrdering::StoredConstraint* 
StandardKBOrdering::SpecialisedComparison::storedConstraintLessOrEqual()
{
  CALL("storedConstraintLessOrEqual()");  
  if (!_constraintLessOrEqual) 
    {
      specialise();
      _constraintLessOrEqual = 
	new StoredConstraint(StoredConstraint::LessOrEqual,
			     _weightComparison,
			     _lexicographicComparisonResult,
			     _lhs,
			     _rhs);
      _constraintLessOrEqual->incReferenceCounter();      
    };

  //DF; cout << "SPECIALISED (LESS OR EQUAL): " << *this << "\n";
  //DF; cout << "STORED CONSTR (LESS OR EQUAL): " << _constraintLessOrEqual << "\n";

  return _constraintLessOrEqual;
}; // StandardKBOrdering::StoredConstraint* StandardKBOrdering::SpecialisedComparison::storedConstraintLessOrEqual()


#ifndef NO_DEBUG_VIS
ostream& StandardKBOrdering::SpecialisedComparison::output(ostream& str) const
{
  str << _lhs << " VS " << _rhs << "\n";
  if (_specialised)
    {
      str << "CATEGORY: " << category() << "\n";
      str << " WEIGHT: " << _weightComparison << "\n"
	  << " LEX: ";
      if (::strict(_weightComparison.category()))
	{
	  str << "IRRELEVANT";
	}
      else
	{
	  str << _lexicographicComparisonResult;
	}
      
    }
  else
    {
      str << " NOT SPECIALISED\n";
      str << "LHS WEIGHT: " << _lhsWeight << "\n";
      str << "RHS WEIGHT: " << _rhsWeight;
    };
  return str;
};
#endif



inline
void StandardKBOrdering::SpecialisedComparison::specialise()
{
  CALL("specialise()");
  if (_specialised) return;

  //DF; cout << "SPECIALISE " << _lhs << " VS " << _rhs << "\n";

  _weightComparison.compare(_lhsWeight,_rhsWeight);
  switch (_weightComparison.category())
    {
    case BK::FailureToCompare:
      ICP("ICP10");
      break;
    case BK::AlwaysLess: 
      _category = SimplificationOrdering::SpecialisedComparison::AlwaysLess;
      break;

    case BK::AlwaysEqual:
      // purely lexicographic
      _lexicographicComparisonResult = 
	_ordering->compareLexicographically(_lhs,_rhs);
      switch (_lexicographicComparisonResult)
	{
	case Less:
	  _category = SimplificationOrdering::SpecialisedComparison::AlwaysLess;
	  break;
	case Equal:
	  _category = SimplificationOrdering::SpecialisedComparison::AlwaysEqual;
	  break;
	case Greater:
	  _category = SimplificationOrdering::SpecialisedComparison::AlwaysGreater;
	  break;
	case Incomparable:
	  _category = SimplificationOrdering::SpecialisedComparison::AbsolutelyVolatile;
	  break;
	};
      break;

    case BK::AlwaysGreater:
      _category = SimplificationOrdering::SpecialisedComparison::AlwaysGreater;
      break;

    case BK::CanBeLessOrEqual:

      //DF; cout << "WEIGHT COMP = CanBeLessOrEqual\n";

      _lexicographicComparisonResult = 
	_ordering->compareLexicographically(_lhs,_rhs);

      //DF; cout << "LEX COMP = " << _lexicographicComparisonResult << "\n";

      switch (_lexicographicComparisonResult)
	{
	case Less:
	  _category = SimplificationOrdering::SpecialisedComparison::AlwaysLess;
	  break;
	case Equal:
	  ICP("ICP0");
	  break;
	case Greater:
	  _category = 
	    SimplificationOrdering::SpecialisedComparison::VolatileButNearlyAlwaysLess;
	  break;
	case Incomparable:
	  _category = 
	    SimplificationOrdering::SpecialisedComparison::VolatileButNearlyAlwaysLess;
	  break;
	};
      break;

    case BK::CanBeGreaterOrEqual:
      _lexicographicComparisonResult = 
	_ordering->compareLexicographically(_lhs,_rhs);
      
      switch (_lexicographicComparisonResult)
	{
	case Less:
	  _category = 
	    SimplificationOrdering::SpecialisedComparison::VolatileButNearlyAlwaysGreater;
	  break;
	case Equal:
	  ICP("ICP1");
	  break;
	case Greater:
	  _category = SimplificationOrdering::SpecialisedComparison::AlwaysGreater;
	  break;
	case Incomparable:
	  _category = 
	    SimplificationOrdering::SpecialisedComparison::VolatileButNearlyAlwaysGreater;
	  break;
	};
      break;

    case BK::VolatileButNearlyAlwaysLess:
      _lexicographicComparisonResult = 
	_ordering->compareLexicographically(_lhs,_rhs);
      _category = 
	SimplificationOrdering::SpecialisedComparison::VolatileButNearlyAlwaysLess;
      break;

    case BK::VolatileButNearlyAlwaysGreater:
      _lexicographicComparisonResult = 
	_ordering->compareLexicographically(_lhs,_rhs);
      _category = 
	SimplificationOrdering::SpecialisedComparison::VolatileButNearlyAlwaysGreater;
      break;

    case BK::AbsolutelyVolatile:
      _lexicographicComparisonResult = 
	_ordering->compareLexicographically(_lhs,_rhs);
      _category = 
	SimplificationOrdering::SpecialisedComparison::AbsolutelyVolatile;
      break;         
    };      
  _specialised = true;
}; // void StandardKBOrdering::SpecialisedComparison::specialise()






//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_SIMPLIFICATION_ORDERING
#define DEBUG_NAMESPACE "StandardKBOrdering::ConstraintCheckOnUnifier"
#endif
#include "debugMacros.h"
//===================================================================

StandardKBOrdering::ConstraintCheckOnUnifier::~ConstraintCheckOnUnifier()
{
};

bool StandardKBOrdering::ConstraintCheckOnUnifier::holds(const SimplificationOrdering::StoredConstraint* constr,
							 ulong variableIndex)
{
  CALL("holds(..)");

//DF; cout << "HOLDS? " << ((const StoredConstraint*)constr) << "\n";
//DF; Unifier::current()->output(cout << "SUBST ",PREFIX::outputTerm) << "\n\n\n";  

  
  ASSERT(constr);
  _currentVariableIndex = variableIndex;
  bool result;
#if (defined DEBUG_NAMESPACE) || (defined _SUPPRESS_WARNINGS_)
  result = false;
#endif
  DOP(Comparison debugCmp = \
      _ordering->compareOnCurrentUnifier(((const StoredConstraint*)constr)->lhs(),\
								 ((const StoredConstraint*)constr)->rhs(),\
								 variableIndex));

  
//DF; cout << "DEBUG CMP = " << debugCmp << "\n";



  switch (((const StoredConstraint*)constr)->targetRelation())
    {
    case StoredConstraint::Greater:
      result = holdsGreater((const StoredConstraint*)constr);
      ASSERT(result == (debugCmp == Greater));
      break;
    case StoredConstraint::GreaterOrEqual:
      result = holdsGreaterOrEqual((const StoredConstraint*)constr);
      ASSERT(result == ((debugCmp == Greater) || (debugCmp == Equal)));
      break;
    case StoredConstraint::Less:
      result = holdsLess((const StoredConstraint*)constr);
      ASSERT(result == (debugCmp == Less));
      break;
    case StoredConstraint::LessOrEqual:
      result = holdsLessOrEqual((const StoredConstraint*)constr);

//DF; cout << "RESULT = " << result << "\n";

      ASSERT(result == ((debugCmp == Less) || (debugCmp == Equal)));
      break;
    };
  return result;
}; // bool StandardKBOrdering::ConstraintCheckOnUnifier::holds(..)



inline
bool StandardKBOrdering::ConstraintCheckOnUnifier::holdsGreater(const StandardKBOrdering::StoredConstraint* constr)
{
  CALL("holdsGreater(..)");
  switch (checkGreaterOrEqual(constr->weightComparison()))
    {
    case BK::FailureToCompare:
      return false;

    case BK::AlwaysLess:
      ICP("ICP10");
      return false; 

    case BK::AlwaysEqual:
      // check lexicographically
      switch (constr->lexicographicComparisonResult())
	{
	case Less: return false;
	case Equal: return false;
	case Greater: return true;
	case Incomparable: return lexicorgaphicallyGreater(constr->lhs(),constr->rhs());
	};	      	      
      ICP("ICP20");
      RuntimeError::report("Illegal control point in StandardKBOrdering::ConstraintCheckOnUnifier::...");

    case BK::AlwaysGreater:
      return true;

    case BK::CanBeLessOrEqual:
      ICP("ICP30");
      return false;

    case BK::CanBeGreaterOrEqual:
      // check lexicographically
      switch (constr->lexicographicComparisonResult())
	{
	case Less: return false;
	case Equal: ICP("ICP1"); 
	case Greater: return true;
	case Incomparable: return lexicorgaphicallyGreater(constr->lhs(),constr->rhs());
	};	      	      
      ICP("ICP40");
      RuntimeError::report("Illegal control point in StandardKBOrdering::ConstraintCheckOnUnifier::...");

    case BK::VolatileButNearlyAlwaysLess:
      // same as below
    case BK::VolatileButNearlyAlwaysGreater:
      // same as below
    case BK::AbsolutelyVolatile:
      ICP("ICP70");
      return false; 
    };
  ICP("ICP80");
#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE)
  return false;
#endif
}; // bool StandardKBOrdering::ConstraintCheckOnUnifier::holdsGreater(const StandardKBOrdering::StoredConstraint* constr)


inline
bool StandardKBOrdering::ConstraintCheckOnUnifier::holdsGreaterOrEqual(const StandardKBOrdering::StoredConstraint* constr)
{
  CALL("holdsGreaterOrEqual(..)");

  switch (checkGreaterOrEqual(constr->weightComparison()))
    {
    case BK::FailureToCompare:
      return false;

    case BK::AlwaysLess:
      ICP("ICP10");
      return false; 

    case BK::AlwaysEqual:
      // check lexicographically
      switch (constr->lexicographicComparisonResult())
	{
	case Less: return false;
	case Equal: return true;
	case Greater: return true;
	case Incomparable: return lexicorgaphicallyGreaterOrEqual(constr->lhs(),constr->rhs());
	};	      	      
      ICP("ICP20");
      RuntimeError::report("Illegal control point in StandardKBOrdering::ConstraintCheckOnUnifier::...");

    case BK::AlwaysGreater:
      return true;

    case BK::CanBeLessOrEqual:	      	      
      ICP("ICP30");
      return false;

    case BK::CanBeGreaterOrEqual:
      // check lexicographically
      switch (constr->lexicographicComparisonResult())
	{
	case Less: return false;
	case Equal: ICP("ICP1"); 
	case Greater: return true;
	case Incomparable: return lexicorgaphicallyGreaterOrEqual(constr->lhs(),constr->rhs());
	};	      	      
      ICP("ICP40");
      RuntimeError::report("Illegal control point in StandardKBOrdering::ConstraintCheckOnUnifier::...");

    case BK::VolatileButNearlyAlwaysLess:
      // same as below
    case BK::VolatileButNearlyAlwaysGreater:
      // same as below
    case BK::AbsolutelyVolatile:      	      
      ICP("ICP50");
      return false; 
    };
  ICP("ICP3");
#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE)
  return false;
#endif

}; // bool StandardKBOrdering::ConstraintCheckOnUnifier::holdsGreaterOrEqual(const StandardKBOrdering::StoredConstraint* constr)


inline
bool StandardKBOrdering::ConstraintCheckOnUnifier::holdsLess(const StandardKBOrdering::StoredConstraint* constr)
{
  CALL("holdsLess(..)");
  switch (checkLessOrEqual(constr->weightComparison()))
    {
    case BK::FailureToCompare:
      return false;

    case BK::AlwaysLess:
      return true; 

    case BK::AlwaysEqual:
      // check lexicographically
      switch (constr->lexicographicComparisonResult())
	{
	case Less: return true;
	case Equal: return false;
	case Greater: return false;
	case Incomparable: return lexicorgaphicallyLess(constr->lhs(),constr->rhs());
	};	      	      
      ICP("ICP10");
      RuntimeError::report("Illegal control point in StandardKBOrdering::ConstraintCheckOnUnifier::...");

    case BK::AlwaysGreater:      	      	      
      ICP("ICP20");
      return false;

    case BK::CanBeLessOrEqual:
      // check lexicographically
      switch (constr->lexicographicComparisonResult())
	{
	case Less: return true;
	case Equal: ICP("ICP1"); 
	case Greater: return false;
	case Incomparable: return lexicorgaphicallyLess(constr->lhs(),constr->rhs());
	};	      	      
      ICP("ICP1");
      RuntimeError::report("Illegal control point in StandardKBOrdering::ConstraintCheckOnUnifier::...");

    case BK::CanBeGreaterOrEqual: 
      // same as below
    case BK::VolatileButNearlyAlwaysLess:
      // same as below
    case BK::VolatileButNearlyAlwaysGreater:
      // same as below
    case BK::AbsolutelyVolatile:    	      	      
      ICP("ICP30");
      return false; 
    };
  ICP("ICP40");
#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE)
  return false;
#endif

}; // bool StandardKBOrdering::ConstraintCheckOnUnifier::holdsLess(const StandardKBOrdering::StoredConstraint* constr)



inline
bool StandardKBOrdering::ConstraintCheckOnUnifier::holdsLessOrEqual(const StandardKBOrdering::StoredConstraint* constr)
{
  CALL("holdsLessOrEqual(..)");


  switch (checkLessOrEqual(constr->weightComparison()))
    {
    case BK::FailureToCompare:
      return false;

    case BK::AlwaysLess:
      return true; 

    case BK::AlwaysEqual:
      // check lexicographically
      switch (constr->lexicographicComparisonResult())
	{
	case Less: return true;
	case Equal: return true;
	case Greater: return false;
	case Incomparable: return lexicorgaphicallyLessOrEqual(constr->lhs(),constr->rhs());
	};	      	      
      ICP("ICP10");
      RuntimeError::report("Illegal control point in StandardKBOrdering::ConstraintCheckOnUnifier::...");

    case BK::AlwaysGreater:         
      ICP("ICP20");
      return false;

    case BK::CanBeLessOrEqual:
      // check lexicographically
      switch (constr->lexicographicComparisonResult())
	{
	case Less: return true;
	case Equal: ICP("ICP1"); 
	case Greater: return false;
	case Incomparable: return lexicorgaphicallyLessOrEqual(constr->lhs(),constr->rhs());
	};	      	      
      ICP("ICP30");
      RuntimeError::report("Illegal control point in StandardKBOrdering::ConstraintCheckOnUnifier::...");

    case BK::CanBeGreaterOrEqual:
      // same as below
    case BK::VolatileButNearlyAlwaysLess:
      // same as below
    case BK::VolatileButNearlyAlwaysGreater: 
      // same as below    
    case BK::AbsolutelyVolatile: 	      
      ICP("ICP40");
      return false;  	
    };
  ICP("ICP50");
#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE)
  return false;
#endif

}; // bool StandardKBOrdering::ConstraintCheckOnUnifier::holdsLessOrEqual(const StandardKBOrdering::StoredConstraint* constr)


inline
FunctionComparison 
StandardKBOrdering::ConstraintCheckOnUnifier::checkGreaterOrEqual(const WeightPolynomial::SpecialisedComparison::Stored& weightComparison)
{
  CALL("checkGreaterOrEqual(const WeightPolynomial::SpecialisedComparison::Stored& weightComparison)");

  switch (weightComparison.category())
    {       
    case BK::FailureToCompare:
      return BK::FailureToCompare;
    case BK::AlwaysLess:
      return BK::FailureToCompare;
    case BK::AlwaysEqual:
      // no break, same as below
    case BK::AlwaysGreater:
      return weightComparison.category(); 
    case BK::CanBeLessOrEqual:
      // check if the substitution made it strictly equal
      ASSERT(weightComparison.positivePart());
      ASSERT(weightComparison.negativePart()); 
      ASSERT(weightComparison.positivePart()->hd().isConstant());
      ASSERT(weightComparison.positivePart()->hd().constant() > (TermWeightType)0);
      ASSERT(!weightComparison.positivePart()->tl());
      {
	TermWeightType weightPosInst = weightComparison.positivePart()->hd().constant();
	
	for (WeightPolynomial::Stored* mem = weightComparison.negativePart();
	     mem;
	     mem = mem->tl())
	  {
	    if (weightPosInst == (TermWeightType)0) 
	      return BK::FailureToCompare;
	    
	    ASSERT(mem->hd().isVariable() && (mem->hd().coeff() > (TermWeightType)0));
	    TermWeightType coeff = mem->hd().coeff();
	    Unifier::Variable* var = Unifier::current()->variable(mem->hd().var(),_currentVariableIndex);
	    bool freeVar;
	    void* varInstance = var->Unref(freeVar);
	    if (freeVar)
	      {
		return BK::FailureToCompare;
	      }
	    else
	      {		
		TermWeightType w = 
		  TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(coeff,
										(const PREFIX_SYM*)varInstance,
										weightPosInst);
		if (w <= (TermWeightType)0) return BK::FailureToCompare;
		ASSERT(w <= weightPosInst);
		weightPosInst -= w;
	      };
	  };
	ASSERT(weightPosInst == (TermWeightType)0);
	return BK::AlwaysEqual;
      };

    case BK::CanBeGreaterOrEqual:
      // try to make it BK::AlwaysGreater 
      ASSERT(weightComparison.positivePart());
      ASSERT(weightComparison.negativePart()); 
      ASSERT(weightComparison.negativePart()->hd().isConstant());
      ASSERT(weightComparison.negativePart()->hd().constant() > (TermWeightType)0);
      ASSERT(!weightComparison.negativePart()->tl());
      {
	TermWeightType weightNegInst = weightComparison.negativePart()->hd().constant();
	bool ground = true;
	for (WeightPolynomial::Stored* mem = weightComparison.positivePart();
	     mem;
	     mem = mem->tl())
	  {
	    
	    ASSERT(mem->hd().isVariable() && (mem->hd().coeff() > (TermWeightType)0));

	    TermWeightType coeff = mem->hd().coeff();
	    Unifier::Variable* var = Unifier::current()->variable(mem->hd().var(),_currentVariableIndex);
	    bool freeVar;
	    void* varInstance = var->Unref(freeVar);
	    if (freeVar)
	      {
		weightNegInst -= coeff;
		ground = false;
	      }
	    else
	      {		
		bool instanceContainsVariables;
		weightNegInst -= 
		  (coeff * 
		   TermWeighting::current()->computeMinimalInstanceWeight((const PREFIX_SYM*)varInstance,
									  instanceContainsVariables));
		if (instanceContainsVariables) ground = false;
	      };	   
	    if (weightNegInst < (TermWeightType)0) 
	      return BK::AlwaysGreater;
	  };
	ASSERT(weightNegInst == (TermWeightType)0); 
	if (ground)
	  return BK::AlwaysEqual;
	return BK::CanBeGreaterOrEqual;
      };


    case BK::VolatileButNearlyAlwaysLess:
      ASSERT(weightComparison.positivePart());
      ASSERT(weightComparison.negativePart()); 
      
      ASSERT(weightComparison.positivePart()->hd().isConstant());
      ASSERT(weightComparison.positivePart()->hd().constant() > (TermWeightType)0);
      ASSERT(!weightComparison.positivePart()->tl());
      {
	TermWeightType weightPosInst = weightComparison.positivePart()->hd().constant();
	
	for (WeightPolynomial::Stored* mem = weightComparison.negativePart();
	     mem;
	     mem = mem->tl())
	  {
	    if (weightPosInst == (TermWeightType)0) 
	      return BK::FailureToCompare;
	    
	    ASSERT(mem->hd().isVariable() && (mem->hd().coeff() > (TermWeightType)0));
	    TermWeightType coeff = mem->hd().coeff();
	    Unifier::Variable* var = Unifier::current()->variable(mem->hd().var(),_currentVariableIndex);
	    bool freeVar;
	    void* varInstance = var->Unref(freeVar);
	    if (freeVar)
	      {
		return BK::FailureToCompare;
	      }
	    else
	      {		
		TermWeightType w = 
		  TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(coeff,
										(const PREFIX_SYM*)varInstance,
										weightPosInst);
		if (w <= 0) return BK::FailureToCompare;
		ASSERT(w <= weightPosInst);
		weightPosInst -= w;
	      };
	  };
	ASSERT(weightPosInst >= (TermWeightType)0);
	// the substitution made both parts ground
	if (weightPosInst == (TermWeightType)0) 
	  return BK::AlwaysEqual;
	return BK::AlwaysGreater;
      };

    case BK::VolatileButNearlyAlwaysGreater:
      ASSERT(weightComparison.positivePart());
      ASSERT(weightComparison.negativePart()); 
      
      ASSERT(weightComparison.negativePart()->hd().isConstant());
      ASSERT(weightComparison.negativePart()->hd().constant() > (TermWeightType)0);
      ASSERT(!weightComparison.negativePart()->tl());
      {
	TermWeightType weightNegInst = weightComparison.negativePart()->hd().constant();
	bool ground = true;
	for (WeightPolynomial::Stored* mem = weightComparison.positivePart();
	     mem;
	     mem = mem->tl())
	  {
	    ASSERT(mem->hd().isVariable() && (mem->hd().coeff() > (TermWeightType)0));
	    TermWeightType coeff = mem->hd().coeff();
	    Unifier::Variable* var = 
	      Unifier::current()->variable(mem->hd().var(),_currentVariableIndex);
	    bool freeVar;
	    void* varInstance = var->Unref(freeVar);
	    if (freeVar)
	      {
		weightNegInst -= coeff;
		ground = false;
	      }
	    else
	      {		
		bool instanceContainsVariables;
		weightNegInst -= 
		  (coeff * 
		   TermWeighting::current()->computeMinimalInstanceWeight((const PREFIX_SYM*)varInstance,
									  instanceContainsVariables));
		if (instanceContainsVariables) ground = false;
	      };
	    if (weightNegInst < (TermWeightType)0) 
	      return BK::AlwaysGreater;
	  };
	
	ASSERT(weightNegInst >= (TermWeightType)0);
	if (weightNegInst == (TermWeightType)0)
	  {
	    if (ground) 
	      return BK::AlwaysEqual;
	    return BK::CanBeGreaterOrEqual;
	  }
	else
	  return BK::FailureToCompare;
      };

    case BK::AbsolutelyVolatile:
      ASSERT(weightComparison.positivePart());
      ASSERT(weightComparison.negativePart()); 
      // First, collect the instance of the positive part
      _weightPositivePartInstance.reset();
      for (WeightPolynomial::Stored* mem = weightComparison.positivePart();
	   mem;
	   mem = mem->tl())
	{
	  if (mem->hd().isConstant())
	    {
	      _weightPositivePartInstance.add(mem->hd().constant());
	    }
	  else // monomial
	    {
	      ASSERT(mem->hd().isVariable() && (mem->hd().coeff() > (TermWeightType)0));
	      TermWeightType coeff = mem->hd().coeff();
	      Unifier::Variable* var = 
		Unifier::current()->variable(mem->hd().var(),_currentVariableIndex);
	      bool freeVar;
	      void* varInstance = var->Unref(freeVar);
	      if (freeVar)
		{
		  ulong varNum;
		  ulong varIndex;
		  ((Unifier::Variable*)varInstance)->NumAndIndex(varNum,varIndex);	    
		  _weightPositivePartInstance.add(coeff,
						  varNum + 
						  (varIndex*VampireKernelConst::MaxNumOfVariables));
		}
	      else
		{
		  TermWeighting::current()->collectWeight(_weightPositivePartInstance,
							  coeff,
							  (const PREFIX_SYM*)varInstance);
		};
	    }; 
	};
      // The instance of the positive part collected.
      // Now, collect the instance of the negative part. 
      {
	
	_weightNegativePartInstance.reset();
	WeightPolynomial::Stored* mem = weightComparison.negativePart();
      next_member:
	ASSERT(mem);
	if (mem->hd().isConstant())
	  {
	    _weightNegativePartInstance.add(mem->hd().constant());
	  }
	else // monomial
	  {
	    ASSERT(mem->hd().isVariable() && (mem->hd().coeff() > (TermWeightType)0));
	    TermWeightType coeff = mem->hd().coeff();
	    Unifier::Variable* var = 
	      Unifier::current()->variable(mem->hd().var(),_currentVariableIndex);
	    bool freeVar;
	    void* varInstance = var->Unref(freeVar);
	    if (freeVar)
	      {
		ulong varNum;
		ulong varIndex;
		((Unifier::Variable*)varInstance)->NumAndIndex(varNum,varIndex);	    
		_weightNegativePartInstance.add(coeff,
						varNum + (varIndex*VampireKernelConst::MaxNumOfVariables));
	      }
	    else
	      {
		TermWeighting::current()->collectWeight(_weightNegativePartInstance,
							coeff,
							(const PREFIX_SYM*)varInstance);
	      };
	    
	  };

	mem = mem->tl();
	if (mem) 
	  {
	    if (!_weightPositivePartInstance.isAlwaysGreater(_weightNegativePartInstance))
	      return BK::FailureToCompare;
	    goto next_member;
	  }
	else
	  {
	    // The instance of the negative part collected.
	    FunctionComparison cmp =
	      _weightPositivePartInstance.compareForGreaterOrEqual(_weightNegativePartInstance);
	    switch (cmp)
	      {
	      case BK::FailureToCompare: 
		return BK::FailureToCompare;
	      case BK::AlwaysLess:
		ICP("ICP10");
		return BK::FailureToCompare;
	      case BK::AlwaysEqual:
		return BK::AlwaysEqual;
	      case BK::AlwaysGreater:
		return BK::AlwaysGreater;
	      case BK::CanBeLessOrEqual:
		ICP("ICP20");
		return BK::FailureToCompare;
	      case BK::CanBeGreaterOrEqual:
		return BK::CanBeGreaterOrEqual;
	      case BK::VolatileButNearlyAlwaysLess:
		// same as below
	      case BK::VolatileButNearlyAlwaysGreater:
		// same as below
	      case BK::AbsolutelyVolatile:
		ICP("ICP30");
		return BK::FailureToCompare;
	      };
	  };	
      };
    }; // switch (weightComparison.category())

  ICP("END");
#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE) 
  return BK::FailureToCompare; 
#endif
}; // StandardKBOrdering::ConstraintCheckOnUnifier::checkGreaterOrEqual(..)





inline
FunctionComparison 
StandardKBOrdering::ConstraintCheckOnUnifier::checkLessOrEqual(const WeightPolynomial::SpecialisedComparison::Stored& weightComparison)
{
  CALL("checkLessOrEqual(const WeightPolynomial::SpecialisedComparison::Stored& weightComparison)");
  switch (weightComparison.category())
    {       
    case BK::FailureToCompare:
      return BK::FailureToCompare;
    case BK::AlwaysLess:
      // no break, same as below
    case BK::AlwaysEqual:
      return weightComparison.category(); 
    case BK::AlwaysGreater:
      return BK::FailureToCompare;

    case BK::CanBeLessOrEqual:
      // try to make it BK::AlwaysLess 
      ASSERT(weightComparison.positivePart());
      ASSERT(weightComparison.negativePart()); 
      ASSERT(weightComparison.positivePart()->hd().isConstant());
      ASSERT(weightComparison.positivePart()->hd().constant() > (TermWeightType)0);
      ASSERT(!weightComparison.positivePart()->tl());
      {
	TermWeightType weightPosInst = weightComparison.positivePart()->hd().constant();
	bool ground = true;
	for (WeightPolynomial::Stored* mem = weightComparison.negativePart();
	     mem;
	     mem = mem->tl())
	  {
	    ASSERT(mem->hd().isVariable() && (mem->hd().coeff() > (TermWeightType)0));
	    TermWeightType coeff = mem->hd().coeff();
	    Unifier::Variable* var = Unifier::current()->variable(mem->hd().var(),_currentVariableIndex);
	    bool freeVar;
	    void* varInstance = var->Unref(freeVar);
	    if (freeVar)
	      {
		weightPosInst -= coeff;
		ground = false;
	      }
	    else
	      {		
		bool instanceContainsVariables;
		weightPosInst -= 
		  (coeff * 
		   TermWeighting::current()->computeMinimalInstanceWeight((const PREFIX_SYM*)varInstance,
									  instanceContainsVariables));
		if (instanceContainsVariables) ground = false;
	      };	   
	    if (weightPosInst < (TermWeightType)0) 
	      return BK::AlwaysLess;
	  };
	ASSERT(weightPosInst == (TermWeightType)0); 
	if (ground)
	  return BK::AlwaysEqual;
	return BK::CanBeLessOrEqual;
      };

    case BK::CanBeGreaterOrEqual:
      // check if the substitution made it strictly equal
      ASSERT(weightComparison.positivePart());
      ASSERT(weightComparison.negativePart()); 
      ASSERT(weightComparison.negativePart()->hd().isConstant());
      ASSERT(weightComparison.negativePart()->hd().constant() > (TermWeightType)0);
      ASSERT(!weightComparison.negativePart()->tl());
      {
	TermWeightType weightNegInst = weightComparison.negativePart()->hd().constant();
	
	for (WeightPolynomial::Stored* mem = weightComparison.positivePart();
	     mem;
	     mem = mem->tl())
	  {
	    if (weightNegInst == (TermWeightType)0) 
	      return BK::FailureToCompare;
	    
	    ASSERT(mem->hd().isVariable() && (mem->hd().coeff() > (TermWeightType)0));
	    TermWeightType coeff = mem->hd().coeff();
	    Unifier::Variable* var = Unifier::current()->variable(mem->hd().var(),_currentVariableIndex);
	    bool freeVar;
	    void* varInstance = var->Unref(freeVar);
	    if (freeVar)
	      {
		return BK::FailureToCompare;
	      }
	    else
	      {		
		TermWeightType w = TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(coeff,
												 (const PREFIX_SYM*)varInstance,
												 weightNegInst);
		if (w <= 0) return BK::FailureToCompare;
		ASSERT(w <= weightNegInst);
		weightNegInst -= w;
	      };
	  };
	ASSERT(weightNegInst == (TermWeightType)0);
	return BK::AlwaysEqual;
      };


    case BK::VolatileButNearlyAlwaysLess:
      ASSERT(weightComparison.positivePart());
      ASSERT(weightComparison.negativePart()); 
      ASSERT(weightComparison.positivePart()->hd().isConstant());
      ASSERT(weightComparison.positivePart()->hd().constant() > (TermWeightType)0);
      ASSERT(!weightComparison.positivePart()->tl());
      {
	TermWeightType weightPosInst = weightComparison.positivePart()->hd().constant();
	bool ground = true;
	for (WeightPolynomial::Stored* mem = weightComparison.negativePart();
	     mem;
	     mem = mem->tl())
	  {
	    ASSERT(mem->hd().isVariable() && (mem->hd().coeff() > (TermWeightType)0));
	    TermWeightType coeff = mem->hd().coeff();
	    Unifier::Variable* var = Unifier::current()->variable(mem->hd().var(),_currentVariableIndex);
	    bool freeVar;
	    void* varInstance = var->Unref(freeVar);
	    if (freeVar)
	      {
		weightPosInst -= coeff;
		ground = false;
	      }
	    else
	      {		
		bool instanceContainsVariables;
		weightPosInst -= 
		  (coeff * 
		   TermWeighting::current()->computeMinimalInstanceWeight((const PREFIX_SYM*)varInstance,
									  instanceContainsVariables));
		if (instanceContainsVariables) ground = false;
	      };
	    if (weightPosInst < (TermWeightType)0) 
	      return BK::AlwaysLess;
	  };
	
	ASSERT(weightPosInst >= (TermWeightType)0);
	if (weightPosInst == (TermWeightType)0)
	  {
	    if (ground) 
	      return BK::AlwaysEqual;
	    return BK::CanBeLessOrEqual;
	  }
	else
	  return BK::FailureToCompare;
      };



    case BK::VolatileButNearlyAlwaysGreater:
      ASSERT(weightComparison.positivePart());
      ASSERT(weightComparison.negativePart()); 
      ASSERT(weightComparison.negativePart()->hd().isConstant());
      ASSERT(weightComparison.negativePart()->hd().constant() > (TermWeightType)0);
      ASSERT(!weightComparison.negativePart()->tl());
      {
	TermWeightType weightNegInst = weightComparison.negativePart()->hd().constant();
	
	for (WeightPolynomial::Stored* mem = weightComparison.positivePart();
	     mem;
	     mem = mem->tl())
	  {
	    if (weightNegInst == (TermWeightType)0) 
	      return BK::FailureToCompare;
	    
	    ASSERT(mem->hd().isVariable() && (mem->hd().coeff() > (TermWeightType)0));
	    TermWeightType coeff = mem->hd().coeff();
	    Unifier::Variable* var = Unifier::current()->variable(mem->hd().var(),_currentVariableIndex);
	    bool freeVar;
	    void* varInstance = var->Unref(freeVar);
	    if (freeVar)
	      {
		return BK::FailureToCompare;
	      }
	    else
	      {		
		TermWeightType w = TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(coeff,
												 (const PREFIX_SYM*)varInstance,
												 weightNegInst);
		if (w <= 0) return BK::FailureToCompare;
		ASSERT(w <= weightNegInst);
		weightNegInst -= w;
	      };
	  };
	ASSERT(weightNegInst >= (TermWeightType)0);
	// the substitution made both parts ground
	if (weightNegInst == (TermWeightType)0) 
	  return BK::AlwaysEqual;
	return BK::AlwaysLess;
      };


    case BK::AbsolutelyVolatile:
      ASSERT(weightComparison.positivePart());
      ASSERT(weightComparison.negativePart()); 
      // First, collect the instance of the positive part
      _weightPositivePartInstance.reset();
      for (WeightPolynomial::Stored* mem = weightComparison.positivePart();
	   mem;
	   mem = mem->tl())
	{
	  if (mem->hd().isConstant())
	    {
	      _weightPositivePartInstance.add(mem->hd().constant());
	    }
	  else // monomial
	    {
	      ASSERT(mem->hd().isVariable() && (mem->hd().coeff() > (TermWeightType)0));
	      TermWeightType coeff = mem->hd().coeff();
	      Unifier::Variable* var = Unifier::current()->variable(mem->hd().var(),_currentVariableIndex);
	      bool freeVar;
	      void* varInstance = var->Unref(freeVar);
	      if (freeVar)
		{
		  ulong varNum;
		  ulong varIndex;
		  ((Unifier::Variable*)varInstance)->NumAndIndex(varNum,varIndex);	    
		  _weightPositivePartInstance.add(coeff,
						  varNum + (varIndex*VampireKernelConst::MaxNumOfVariables));
		}
	      else
		{
		  TermWeighting::current()->collectWeight(_weightPositivePartInstance,
							  coeff,
							  (const PREFIX_SYM*)varInstance);
		};
	    }; 
	};
      
      
      // The instance of the positive part collected.
      // Now, collect the instance of the negative part. 
      {	
	_weightNegativePartInstance.reset();
	WeightPolynomial::Stored* mem = weightComparison.negativePart();
      next_member:
	ASSERT(mem);
	if (mem->hd().isConstant())
	  {
	    _weightNegativePartInstance.add(mem->hd().constant());
	  }
	else // monomial
	  {
	    ASSERT(mem->hd().isVariable() && (mem->hd().coeff() > (TermWeightType)0));
	    TermWeightType coeff = mem->hd().coeff();
	    Unifier::Variable* var = Unifier::current()->variable(mem->hd().var(),_currentVariableIndex);
	    bool freeVar;
	    void* varInstance = var->Unref(freeVar);
	    if (freeVar)
	      {
		ulong varNum;
		ulong varIndex;
		((Unifier::Variable*)varInstance)->NumAndIndex(varNum,varIndex);	    
		_weightNegativePartInstance.add(coeff,
						varNum + (varIndex*VampireKernelConst::MaxNumOfVariables));
	      }
	    else
	      {
		TermWeighting::current()->collectWeight(_weightNegativePartInstance,
							coeff,
							(const PREFIX_SYM*)varInstance);
	      };
	    
	  };

	mem = mem->tl();

	if (mem) 
	  {
	    if (_weightPositivePartInstance.isNeverGreater(_weightNegativePartInstance))
	      {
		return BK::AlwaysLess;
	      };
	    goto next_member;
	  }
	else
	  {
	    // The instance of the negative part collected.

	    FunctionComparison cmp =
	      _weightPositivePartInstance.compareForLessOrEqual(_weightNegativePartInstance);

	    switch (cmp)
	      {
	      case BK::FailureToCompare:		
		return BK::FailureToCompare;
	      case BK::AlwaysLess:
		return BK::AlwaysLess;
	      case BK::AlwaysEqual:
		return BK::AlwaysEqual;
	      case BK::AlwaysGreater:
		ICP("ICP10");
		return BK::FailureToCompare;
	      case BK::CanBeLessOrEqual:
		return BK::CanBeLessOrEqual;
	      case BK::CanBeGreaterOrEqual:
		// same as below
	      case BK::VolatileButNearlyAlwaysLess:
		// same as below
	      case BK::VolatileButNearlyAlwaysGreater:
		// same as below
	      case BK::AbsolutelyVolatile:
		ICP("ICP20");
		return BK::FailureToCompare;
	      };
	  };	
      };
    }; // switch (weightComparison.category())

  ICP("END");
#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE) 
  return BK::FailureToCompare; 
#endif

}; // StandardKBOrdering::ConstraintCheckOnUnifier::checkLessOrEqual(..)




inline
bool StandardKBOrdering::ConstraintCheckOnUnifier::lexicorgaphicallyGreater(const TERM& lhs,const TERM& rhs)
{
  CALL("lexicorgaphicallyGreater(const TERM& lhs,const TERM& rhs)");
  Comparison cmp = 
    _ordering->compareLexOnCurrentUnifier(lhs,rhs,_currentVariableIndex);
  return (cmp == Greater);
}; // bool StandardKBOrdering::ConstraintCheckOnUnifier::lexicorgaphicallyGreater(const TERM& lhs,const TERM& rhs)

inline
bool StandardKBOrdering::ConstraintCheckOnUnifier::lexicorgaphicallyGreaterOrEqual(const TERM& lhs,const TERM& rhs)
{
  CALL("lexicorgaphicallyGreaterOrEqual(const TERM& lhs,const TERM& rhs)");
  Comparison cmp = 
    _ordering->compareLexOnCurrentUnifier(lhs,rhs,_currentVariableIndex);
  return (cmp == Greater) || (cmp == Equal);   
}; // bool StandardKBOrdering::ConstraintCheckOnUnifier::lexicorgaphicallyGreaterOrEqual(const TERM& lhs,const TERM& rhs)


inline
bool 
StandardKBOrdering::ConstraintCheckOnUnifier::lexicorgaphicallyLess(const TERM& lhs,const TERM& rhs)
{
  return lexicorgaphicallyGreater(rhs,lhs);
};

inline
bool 
StandardKBOrdering::ConstraintCheckOnUnifier::lexicorgaphicallyLessOrEqual(const TERM& lhs,const TERM& rhs)
{
  return lexicorgaphicallyGreaterOrEqual(rhs,lhs);
};


//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_SIMPLIFICATION_ORDERING
#define DEBUG_NAMESPACE "StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst"
#endif
#include "debugMacros.h"
//===================================================================

StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst::~ConstraintCheckOnForwardMatchingSubst()
{
};

bool 
StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst::holds(const SimplificationOrdering::StoredConstraint* constr,
								 const ForwardMatchingSubstitutionCache* subst)
{
  CALL("holds(..)");
  
  //DF; cout << "HOLDS? " << (StandardKBOrdering::StoredConstraint*)constr << "\n";
  //DF; cout << "SUBST:\n" << subst << "\n";

  ASSERT(constr);
  bool result;
  DOP(Comparison debugCmp = \
      _ordering->compare(((const StoredConstraint*)constr)->lhs(),\
			 ((const StoredConstraint*)constr)->rhs(),\
			 subst));

  //DF; cout << "DEBUG CMP = " << debugCmp << "\n";

  _subst = subst;
  switch (((const StoredConstraint*)constr)->targetRelation())
    {
    case StoredConstraint::Greater:
      result = holdsGreater((const StoredConstraint*)constr);

      //DF; cout << "GREATER = " << result << "\n";

      ASSERT(result == (debugCmp == Greater));
      return result;
    case StoredConstraint::GreaterOrEqual:
      result = holdsGreaterOrEqual((const StoredConstraint*)constr);
      ASSERT(result == ((debugCmp == Greater) || (debugCmp == Equal)));
      return result;
    case StoredConstraint::Less:
      result = holdsLess((const StoredConstraint*)constr);

      //DF; cout << "LESS = " << result << "\n";

      ASSERT(result == (debugCmp == Less));
      return result;
    case StoredConstraint::LessOrEqual:
      result = holdsLessOrEqual((const StoredConstraint*)constr);
      ASSERT(result == ((debugCmp == Less) || (debugCmp == Equal)));
      return result;
    };
  ICP("ICP0");
#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE)
  return false;
#endif
}; // bool StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst::holds(..)

 
inline
bool 
StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst::holdsGreater(const StoredConstraint* constr)
{
  CALL("holdsGreater(..)");

  //DF; cout << "HOLDS GREATER? " << constr << "\n";
  //DF; cout << "SUBST:\n" << _subst << "\n";

  switch (checkGreaterOrEqual(constr->weightComparison()))
    {
    case BK::FailureToCompare:
      return false;

    case BK::AlwaysLess:
      ICP("ICP10");
      return false; 

    case BK::AlwaysEqual:
      // check lexicographically
      switch (constr->lexicographicComparisonResult())
	{
	case Less: return false;
	case Equal: return false;
	case Greater: return true;
	case Incomparable: return lexicorgaphicallyGreater(constr->lhs(),constr->rhs());
	};	      	      
      ICP("ICP20");
      RuntimeError::report("Illegal control point in StandardKBOrdering::ConstraintCheckOnUnifier::...");

    case BK::AlwaysGreater:
      return true;

    case BK::CanBeLessOrEqual:
      ICP("ICP30");
      return false;

    case BK::CanBeGreaterOrEqual:
      // check lexicographically
      switch (constr->lexicographicComparisonResult())
	{
	case Less: return false;
	case Equal: ICP("ICP1"); 
	case Greater: return true;
	case Incomparable: return lexicorgaphicallyGreater(constr->lhs(),constr->rhs());
	};	      	      
      ICP("ICP40");
      RuntimeError::report("Illegal control point in StandardKBOrdering::ConstraintCheckOnUnifier::...");

    case BK::VolatileButNearlyAlwaysLess:
      // same as below
    case BK::VolatileButNearlyAlwaysGreater:
      // same as below
    case BK::AbsolutelyVolatile:     	      
      ICP("ICP50");
      return false;
    };
  ICP("ICP60");
#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE)
  return false;
#endif
}; // bool StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst::holdsGreater(const StoredConstraint* constr)




inline
bool 
StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst::holdsGreaterOrEqual(const StoredConstraint* constr)
{
  CALL("holdsGreaterOrEqual(..)");

  switch (checkGreaterOrEqual(constr->weightComparison()))
    {
    case BK::FailureToCompare:
      return false;

    case BK::AlwaysLess:
      ICP("ICP10");
      return false; 

    case BK::AlwaysEqual:
      // check lexicographically
      switch (constr->lexicographicComparisonResult())
	{
	case Less: return false;
	case Equal: return true;
	case Greater: return true;
	case Incomparable: return lexicorgaphicallyGreaterOrEqual(constr->lhs(),constr->rhs());
	};	      	      
      ICP("ICP20");
      RuntimeError::report("Illegal control point in StandardKBOrdering::ConstraintCheckOnUnifier::...");

    case BK::AlwaysGreater:
      return true;

    case BK::CanBeLessOrEqual:
      ICP("ICP30");
      return false;

    case BK::CanBeGreaterOrEqual:
      // check lexicographically
      switch (constr->lexicographicComparisonResult())
	{
	case Less: return false;
	case Equal: ICP("ICP1"); 
	case Greater: return true;
	case Incomparable: return lexicorgaphicallyGreaterOrEqual(constr->lhs(),constr->rhs());
	};	      	      
      ICP("ICP40");
      RuntimeError::report("Illegal control point in StandardKBOrdering::ConstraintCheckOnUnifier::...");

    case BK::VolatileButNearlyAlwaysLess:
      // same as below
    case BK::VolatileButNearlyAlwaysGreater:
      // same as below
    case BK::AbsolutelyVolatile:
      ICP("ICP50");
      return false; 
    };
  ICP("ICP60");
#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE)
  return false;
#endif

}; // bool StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst::holdsGreaterOrEqual(const StoredConstraint* constr)

inline
bool 
StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst::holdsLess(const StoredConstraint* constr)
{
  CALL("holdsLess(..)");

  //DF; cout << "HOLDS LESS? "; //<< constr << "\n";
  //DF; cout << "SUBST:\n" << _subst << "\n";

  FunctionComparison weightComp = 
    checkLessOrEqual(constr->weightComparison());

  //DF; cout << "WEIGHT COMP = " << weightComp << "\n";

  switch (weightComp)
    {
    case BK::FailureToCompare:
      return false;

    case BK::AlwaysLess:
      return true; 

    case BK::AlwaysEqual:
      // check lexicographically
      switch (constr->lexicographicComparisonResult())
	{
	case Less: return true;
	case Equal: return false;
	case Greater: return false;
	case Incomparable: return lexicorgaphicallyLess(constr->lhs(),constr->rhs());
	};	      	      
      ICP("ICP10");
      RuntimeError::report("Illegal control point in StandardKBOrdering::ConstraintCheckOnUnifier::...");

    case BK::AlwaysGreater:    	      
      ICP("ICP20");
      return false;

    case BK::CanBeLessOrEqual:

      // check lexicographically
      switch (constr->lexicographicComparisonResult())
	{
	case Less: return true;
	case Equal: ICP("ICP1"); 
	case Greater: return false;
	case Incomparable: return lexicorgaphicallyLess(constr->lhs(),constr->rhs());
	};	      	      
      ICP("ICP30");
      RuntimeError::report("Illegal control point in StandardKBOrdering::ConstraintCheckOnUnifier::...");

    case BK::CanBeGreaterOrEqual:
      // same as below
    case BK::VolatileButNearlyAlwaysLess:
      // same as below
    case BK::VolatileButNearlyAlwaysGreater:
      // same as below
    case BK::AbsolutelyVolatile:            	      
      ICP("ICP40");
      return false; 
    };
  ICP("ICP50");
#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE)
  return false;
#endif

}; // bool StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst::holdsLess(const StoredConstraint* constr)

inline
bool 
StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst::holdsLessOrEqual(const StoredConstraint* constr)
{
  CALL("holdsLessOrEqual(..)");

  switch (checkLessOrEqual(constr->weightComparison()))
    {
    case BK::FailureToCompare:
      return false;

    case BK::AlwaysLess:
      return true; 

    case BK::AlwaysEqual:
      // check lexicographically
      switch (constr->lexicographicComparisonResult())
	{
	case Less: return true;
	case Equal: return true;
	case Greater: return false;
	case Incomparable: return lexicorgaphicallyLessOrEqual(constr->lhs(),constr->rhs());
	};	      	      
      ICP("ICP10");
      RuntimeError::report("Illegal control point in StandardKBOrdering::ConstraintCheckOnUnifier::...");

    case BK::AlwaysGreater:   	      
      ICP("ICP20");
      return false;

    case BK::CanBeLessOrEqual:
      // check lexicographically
      switch (constr->lexicographicComparisonResult())
	{
	case Less: return true;
	case Equal: ICP("ICP1"); 
	case Greater: return false;
	case Incomparable: return lexicorgaphicallyLessOrEqual(constr->lhs(),constr->rhs());
	};	      	      
      ICP("ICP30");
      RuntimeError::report("Illegal control point in StandardKBOrdering::ConstraintCheckOnUnifier::...");

    case BK::CanBeGreaterOrEqual:
    case BK::VolatileButNearlyAlwaysLess:
    case BK::VolatileButNearlyAlwaysGreater:
    case BK::AbsolutelyVolatile:    	      
      ICP("ICP40");
      return false; 
    };
  ICP("ICP50");
#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE)
  return false;
#endif
}; // bool StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst::holdsLessOrEqual(const StoredConstraint* constr)
  
inline
FunctionComparison 
StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst::checkGreaterOrEqual(const WeightPolynomial::SpecialisedComparison::Stored& weightComparison)
{
  CALL("checkGreaterOrEqual(..)");

  switch (weightComparison.category())
    {       
    case BK::FailureToCompare:
      return BK::FailureToCompare;

    case BK::AlwaysLess:
      return BK::FailureToCompare;
    case BK::AlwaysEqual:
      // no break, same as below
    case BK::AlwaysGreater:
      // no break, same as below
      return weightComparison.category(); 
    case BK::CanBeLessOrEqual:
      // check if the substitution made it strictly equal
      ASSERT(weightComparison.positivePart());
      ASSERT(weightComparison.negativePart()); 
      ASSERT(weightComparison.positivePart()->hd().isConstant());
      ASSERT(weightComparison.positivePart()->hd().constant() > (TermWeightType)0);
      ASSERT(!weightComparison.positivePart()->tl());
      {
	TermWeightType weightPosInst = weightComparison.positivePart()->hd().constant();
	
	for (WeightPolynomial::Stored* mem = weightComparison.negativePart();
	     mem;
	     mem = mem->tl())
	  {
	    if (weightPosInst == (TermWeightType)0) 
	      return BK::FailureToCompare;
	    
	    ASSERT(mem->hd().isVariable() && (mem->hd().coeff() > (TermWeightType)0));
	    TermWeightType coeff = mem->hd().coeff();
	    const Flatterm* varInstance = _subst->map(mem->hd().var());
	    if (varInstance->isVariable())
	      {
		return BK::FailureToCompare;
	      }
	    else
	      {			
		TermWeightType w = TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(coeff,
												 varInstance,
												 weightPosInst);
		if (w <= 0) return BK::FailureToCompare;
		ASSERT(w <= weightPosInst);
		weightPosInst -= w;
	      };
	  };
	ASSERT(weightPosInst == (TermWeightType)0);
	return BK::AlwaysEqual;
      };

    case BK::CanBeGreaterOrEqual:
      // try to make it BK::AlwaysGreater 
      ASSERT(weightComparison.positivePart());
      ASSERT(weightComparison.negativePart()); 
      ASSERT(weightComparison.negativePart()->hd().isConstant());
      ASSERT(weightComparison.negativePart()->hd().constant() > (TermWeightType)0);
      ASSERT(!weightComparison.negativePart()->tl());
      {
	TermWeightType weightNegInst = weightComparison.negativePart()->hd().constant();
	bool ground = true;
	for (WeightPolynomial::Stored* mem = weightComparison.positivePart();
	     mem;
	     mem = mem->tl())
	  {
	    ASSERT(mem->hd().isVariable() && (mem->hd().coeff() > (TermWeightType)0));
	    TermWeightType coeff = mem->hd().coeff();
	    const Flatterm* varInstance = _subst->map(mem->hd().var());
	    if (varInstance->isVariable())
	      {
		weightNegInst -= coeff;
		ground = false;
	      }
	    else
	      {
		bool instanceContainsVariables;
		weightNegInst -= 
		  (coeff * 
		   TermWeighting::current()->computeMinimalInstanceWeight(varInstance,
									  instanceContainsVariables));
		if (instanceContainsVariables) ground = false;
	      };	
	    if (weightNegInst < (TermWeightType)0) 
	      return BK::AlwaysGreater;
	  };
	ASSERT(weightNegInst == (TermWeightType)0); 
	if (ground)
	  return BK::AlwaysEqual;
	return BK::CanBeGreaterOrEqual;
      };


    case BK::VolatileButNearlyAlwaysLess:
      ASSERT(weightComparison.positivePart());
      ASSERT(weightComparison.negativePart()); 
      ASSERT(weightComparison.positivePart()->hd().isConstant());
      ASSERT(weightComparison.positivePart()->hd().constant() > (TermWeightType)0);
      ASSERT(!weightComparison.positivePart()->tl());
      {
	TermWeightType weightPosInst = weightComparison.positivePart()->hd().constant();
	
	for (WeightPolynomial::Stored* mem = weightComparison.negativePart();
	     mem;
	     mem = mem->tl())
	  {
	    if (weightPosInst == (TermWeightType)0) 
	      return BK::FailureToCompare;
	    
	    ASSERT(mem->hd().isVariable() && (mem->hd().coeff() > (TermWeightType)0));
	    TermWeightType coeff = mem->hd().coeff();
	    const Flatterm* varInstance = _subst->map(mem->hd().var());
	    if (varInstance->isVariable())
	      {
		return BK::FailureToCompare;
	      }
	    else
	      {		
		TermWeightType w = TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(coeff,
												 varInstance,
												 weightPosInst);
		if (w <= 0) return BK::FailureToCompare;
		ASSERT(w <= weightPosInst);
		weightPosInst -= w;
	      };
	  };
	ASSERT(weightPosInst >= (TermWeightType)0);
	// the substitution made both terms ground
	if (weightPosInst == (TermWeightType)0) 
	  return BK::AlwaysEqual;
	return BK::AlwaysGreater;
      };

    case BK::VolatileButNearlyAlwaysGreater:
      ASSERT(weightComparison.positivePart());
      ASSERT(weightComparison.negativePart()); 
      ASSERT(weightComparison.negativePart()->hd().isConstant());
      ASSERT(weightComparison.negativePart()->hd().constant() > (TermWeightType)0);
      ASSERT(!weightComparison.negativePart()->tl());
      {
	TermWeightType weightNegInst = weightComparison.negativePart()->hd().constant();
	bool ground = true;
	for (WeightPolynomial::Stored* mem = weightComparison.positivePart();
	     mem;
	     mem = mem->tl())
	  {
	    ASSERT(mem->hd().isVariable() && (mem->hd().coeff() > (TermWeightType)0));
	    TermWeightType coeff = mem->hd().coeff();
	    const Flatterm* varInstance = _subst->map(mem->hd().var());
	    if (varInstance->isVariable())
	      {
		weightNegInst -= coeff;
		ground = false;
	      }
	    else
	      {		
		bool instanceContainsVariables;
		weightNegInst -= 
		  (coeff * 
		   TermWeighting::current()->computeMinimalInstanceWeight(varInstance,
									  instanceContainsVariables));
		if (instanceContainsVariables) ground = false;
	      };
	    if (weightNegInst < (TermWeightType)0) 
	      return BK::AlwaysGreater;
	  };
	
	ASSERT(weightNegInst >= (TermWeightType)0);
	if (weightNegInst == (TermWeightType)0)
	  {
	    if (ground) 
	      return BK::AlwaysEqual;
	    return BK::CanBeGreaterOrEqual;
	  }
	else
	  return BK::FailureToCompare;
      };

    case BK::AbsolutelyVolatile:
      ASSERT(weightComparison.positivePart());
      ASSERT(weightComparison.negativePart()); 
      // First, collect the instance of the positive part
      _weightPositivePartInstance.reset();
      for (WeightPolynomial::Stored* mem = weightComparison.positivePart();
	   mem;
	   mem = mem->tl())
	{
	  if (mem->hd().isConstant())
	    {
	      _weightPositivePartInstance.add(mem->hd().constant());
	    }
	  else // monomial
	    {
	      ASSERT(mem->hd().isVariable() && (mem->hd().coeff() > (TermWeightType)0));
	      TermWeightType coeff = mem->hd().coeff();
	      const Flatterm* varInstance = _subst->map(mem->hd().var());

	      if (varInstance->isVariable())
		{ 
		  _weightPositivePartInstance.add(coeff,varInstance->Var());
		}
	      else
		{
		  TermWeighting::current()->collectWeight(_weightPositivePartInstance,
							  coeff,
							  varInstance);
		};
	    }; 
	};
      // The instance of the positive part collected.
      // Now, collect the instance of the negative part. 
      {
	
	_weightNegativePartInstance.reset();
	WeightPolynomial::Stored* mem = weightComparison.negativePart();
      next_member:
	ASSERT(mem);
	if (mem->hd().isConstant())
	  {
	    _weightNegativePartInstance.add(mem->hd().constant());
	  }
	else // monomial
	  {
	    ASSERT(mem->hd().isVariable() && (mem->hd().coeff() > (TermWeightType)0));
	    TermWeightType coeff = mem->hd().coeff();
	    const Flatterm* varInstance = _subst->map(mem->hd().var());

	    if (varInstance->isVariable())
	      {    
		_weightNegativePartInstance.add(coeff,varInstance->Var());
	      }
	    else
	      {
		TermWeighting::current()->collectWeight(_weightNegativePartInstance,
							coeff,
							varInstance);
	      };
	    
	  };

	mem = mem->tl();
	if (mem) 
	  {
	    if (!_weightPositivePartInstance.isAlwaysGreater(_weightNegativePartInstance))
	      return BK::FailureToCompare;
	    goto next_member;
	  }
	else
	  {
	    // The instance of the negative part collected.
	    FunctionComparison cmp =
	      _weightPositivePartInstance.compareForGreaterOrEqual(_weightNegativePartInstance);
	    switch (cmp)
	      {
	      case BK::FailureToCompare:
		return BK::FailureToCompare;
	      case BK::AlwaysLess:
		ICP("ICP10");
		return BK::FailureToCompare;
	      case BK::AlwaysEqual:
		return BK::AlwaysEqual;
	      case BK::AlwaysGreater:
		return BK::AlwaysGreater;
	      case BK::CanBeLessOrEqual:
		ICP("ICP20");
		return BK::FailureToCompare;
	      case BK::CanBeGreaterOrEqual:
		return BK::CanBeGreaterOrEqual;
	      case BK::VolatileButNearlyAlwaysLess:
		// same as below
	      case BK::VolatileButNearlyAlwaysGreater:
		// same as below
	      case BK::AbsolutelyVolatile:
		ICP("ICP30");
		return BK::FailureToCompare;
	      };
	  };	
      };
    }; // switch (weightComparison.category())

  ICP("END");
#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE) 
  return BK::FailureToCompare; 
#endif
}; // FunctionComparison StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst::checkGreaterOrEqual(const WeightPolynomial::SpecialisedComparison::Stored& weightComparison)


inline
FunctionComparison 
StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst::checkLessOrEqual(const WeightPolynomial::SpecialisedComparison::Stored& weightComparison)
{
  CALL("checkLessOrEqual(..)");

  switch (weightComparison.category())
    {       
    case BK::FailureToCompare:
      return BK::FailureToCompare;

    case BK::AlwaysLess:
      // no break, same as below
    case BK::AlwaysEqual:
      return weightComparison.category();
      
    case BK::AlwaysGreater:
      return BK::FailureToCompare;

    case BK::CanBeLessOrEqual:
      // try to make it BK::AlwaysLess 
      COP("case BK::CanBeLessOrEqual");
      ASSERT(weightComparison.positivePart());
      ASSERT(weightComparison.negativePart()); 
      ASSERT(weightComparison.positivePart()->hd().isConstant());
      ASSERT(weightComparison.positivePart()->hd().constant() > (TermWeightType)0);
      ASSERT(!weightComparison.positivePart()->tl());
      {
	TermWeightType weightPosInst = weightComparison.positivePart()->hd().constant();
	bool ground = true;
	for (WeightPolynomial::Stored* mem = weightComparison.negativePart();
	     mem;
	     mem = mem->tl())
	  {
	    ASSERT(mem->hd().isVariable() && (mem->hd().coeff() > (TermWeightType)0));
	    TermWeightType coeff = mem->hd().coeff();
	    const Flatterm* varInstance = _subst->map(mem->hd().var());

	    if (varInstance->isVariable())
	      {
		weightPosInst -= coeff;
		ground = false;
	      }
	    else
	      {		
		bool instanceContainsVariables;
		weightPosInst -= 
		  (coeff * 
		   TermWeighting::current()->computeMinimalInstanceWeight(varInstance,
									  instanceContainsVariables));
		if (instanceContainsVariables) ground = false;
	      };	   
	    if (weightPosInst < (TermWeightType)0) 
	      return BK::AlwaysLess;
	  };
	ASSERT(weightPosInst == (TermWeightType)0); 
	if (ground)
	  return BK::AlwaysEqual;
	return BK::CanBeLessOrEqual;
      };

    case BK::CanBeGreaterOrEqual:
      // check if the substitution made it strictly equal
      COP("case BK::CanBeGreaterOrEqual");
      ASSERT(weightComparison.positivePart());
      ASSERT(weightComparison.negativePart()); 
      ASSERT(weightComparison.negativePart()->hd().isConstant());
      ASSERT(weightComparison.negativePart()->hd().constant() > (TermWeightType)0);
      ASSERT(!weightComparison.negativePart()->tl());
      {
	TermWeightType weightNegInst = weightComparison.negativePart()->hd().constant();
	
	for (WeightPolynomial::Stored* mem = weightComparison.positivePart();
	     mem;
	     mem = mem->tl())
	  {
	    if (weightNegInst == (TermWeightType)0) 
	      return BK::FailureToCompare;
	    
	    ASSERT(mem->hd().isVariable() && (mem->hd().coeff() > (TermWeightType)0));
	    TermWeightType coeff = mem->hd().coeff();
	    const Flatterm* varInstance = _subst->map(mem->hd().var());
	    if (varInstance->isVariable())
	      {
		return BK::FailureToCompare;
	      }
	    else
	      {		
		TermWeightType w = TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(coeff,
												 varInstance,
												 weightNegInst);
		if (w <= 0) return BK::FailureToCompare;
		ASSERT(w <= weightNegInst);
		weightNegInst -= w;
	      };
	  };
	ASSERT(weightNegInst == (TermWeightType)0);
	return BK::AlwaysEqual;
      };


    case BK::VolatileButNearlyAlwaysLess:
      COP("case BK::VolatileButNearlyAlwaysLess");
      ASSERT(weightComparison.positivePart());
      ASSERT(weightComparison.negativePart()); 
      ASSERT(weightComparison.positivePart()->hd().isConstant());
      ASSERT(weightComparison.positivePart()->hd().constant() > (TermWeightType)0);
      ASSERT(!weightComparison.positivePart()->tl());
      {
	TermWeightType weightPosInst = weightComparison.positivePart()->hd().constant();
	bool ground = true;
	for (WeightPolynomial::Stored* mem = weightComparison.negativePart();
	     mem;
	     mem = mem->tl())
	  {
	    ASSERT(mem->hd().isVariable() && (mem->hd().coeff() > (TermWeightType)0));
	    TermWeightType coeff = mem->hd().coeff();
	    const Flatterm* varInstance = _subst->map(mem->hd().var());
	    if (varInstance->isVariable())
	      {
		weightPosInst -= coeff;
		ground = false;
	      }
	    else
	      {		
		bool instanceContainsVariables;
		weightPosInst -= 
		  (coeff * 
		   TermWeighting::current()->computeMinimalInstanceWeight(varInstance,
									  instanceContainsVariables));
		if (instanceContainsVariables) ground = false;
	      };
	    if (weightPosInst < (TermWeightType)0) 
	      return BK::AlwaysLess;
	  };
	
	ASSERT(weightPosInst >= (TermWeightType)0);
	if (weightPosInst == (TermWeightType)0)
	  {
	    if (ground) 
	      return BK::AlwaysEqual;
	    return BK::CanBeLessOrEqual;
	  }
	else
	  return BK::FailureToCompare;
      };



    case BK::VolatileButNearlyAlwaysGreater:
      COP("case BK::VolatileButNearlyAlwaysGreater");
      ASSERT(weightComparison.positivePart());
      ASSERT(weightComparison.negativePart()); 
      ASSERT(weightComparison.negativePart()->hd().isConstant());
      ASSERT(weightComparison.negativePart()->hd().constant() > (TermWeightType)0);
      ASSERT(!weightComparison.negativePart()->tl());
      {
	TermWeightType weightNegInst = weightComparison.negativePart()->hd().constant();
	
	for (WeightPolynomial::Stored* mem = weightComparison.positivePart();
	     mem;
	     mem = mem->tl())
	  {
	    if (weightNegInst == (TermWeightType)0) 
	      return BK::FailureToCompare;
	    
	    ASSERT(mem->hd().isVariable() && (mem->hd().coeff() > (TermWeightType)0));
	    TermWeightType coeff = mem->hd().coeff();
	    const Flatterm* varInstance = _subst->map(mem->hd().var());
	    if (varInstance->isVariable())
	      {
		return BK::FailureToCompare;
	      }
	    else
	      {		
		TermWeightType w = TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(coeff,
												 varInstance,
												 weightNegInst);
		if (w <= 0) return BK::FailureToCompare;
		ASSERT(w <= weightNegInst);
		weightNegInst -= w;
	      };
	  };
	ASSERT(weightNegInst >= (TermWeightType)0);
	// the substitution made both parts ground
	if (weightNegInst == (TermWeightType)0) 
	  return BK::AlwaysEqual;
	return BK::AlwaysLess;
      };


    case BK::AbsolutelyVolatile:
      COP("case BK::AbsolutelyVolatile");
      ASSERT(weightComparison.positivePart());
      ASSERT(weightComparison.negativePart()); 
      // First, collect the instance of the positive part
      _weightPositivePartInstance.reset();
      for (WeightPolynomial::Stored* mem = weightComparison.positivePart();
	   mem;
	   mem = mem->tl())
	{
	  if (mem->hd().isConstant())
	    {
	      _weightPositivePartInstance.add(mem->hd().constant());
	    }
	  else // monomial
	    {
	      ASSERT(mem->hd().isVariable() && (mem->hd().coeff() > (TermWeightType)0));
	      TermWeightType coeff = mem->hd().coeff();
	      const Flatterm* varInstance = _subst->map(mem->hd().var());
	      if (varInstance->isVariable())
		{    
		  _weightPositivePartInstance.add(coeff,varInstance->Var());
		}
	      else
		{
		  TermWeighting::current()->collectWeight(_weightPositivePartInstance,
							  coeff,
							  varInstance);
		};
	    }; 
	};
      // The instance of the positive part collected.
      // Now, collect the instance of the negative part. 
      {
	
	_weightNegativePartInstance.reset();
	WeightPolynomial::Stored* mem = weightComparison.negativePart();
      next_member:
	ASSERT(mem);
	if (mem->hd().isConstant())
	  {
	    _weightNegativePartInstance.add(mem->hd().constant());
	  }
	else // monomial
	  {
	    ASSERT(mem->hd().isVariable() && (mem->hd().coeff() > (TermWeightType)0));
	    TermWeightType coeff = mem->hd().coeff();
	    const Flatterm* varInstance = _subst->map(mem->hd().var());
	    if (varInstance->isVariable())
	      {    
		_weightNegativePartInstance.add(coeff,varInstance->Var());
	      }
	    else
	      {
		TermWeighting::current()->collectWeight(_weightNegativePartInstance,
							coeff,
							varInstance);
	      };
	    
	  };

	mem = mem->tl();
	if (mem) 
	  {
	    if (_weightPositivePartInstance.isNeverGreater(_weightNegativePartInstance))
	      return BK::AlwaysLess;
	    goto next_member;
	  }
	else
	  {
	    // The instance of the negative part collected.
	    FunctionComparison cmp =
	      _weightPositivePartInstance.compareForLessOrEqual(_weightNegativePartInstance);
	    switch (cmp)
	      {
	      case BK::FailureToCompare:
		return BK::FailureToCompare;		
	      case BK::AlwaysLess:
		return BK::AlwaysLess;
	      case BK::AlwaysEqual:
		return BK::AlwaysEqual;
	      case BK::AlwaysGreater:
		ICP("ICP10");
		return BK::FailureToCompare;
	      case BK::CanBeLessOrEqual:
		return BK::CanBeLessOrEqual;
	      case BK::CanBeGreaterOrEqual:
		// same as below
	      case BK::VolatileButNearlyAlwaysLess:
		// same as below
	      case BK::VolatileButNearlyAlwaysGreater:
		// same as below
	      case BK::AbsolutelyVolatile:
		ICP("ICP20");
		return BK::FailureToCompare;		
	      };
	  };	
      };
    }; // switch (weightComparison.category())

  ICP("END");
#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE) 
  return BK::FailureToCompare; 
#endif

}; // FunctionComparison StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst::checkLessOrEqual(const WeightPolynomial::SpecialisedComparison::Stored& weightComparison)




inline
bool StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst::lexicorgaphicallyGreater(const TERM& lhs,const TERM& rhs)
{
  CALL("lexicorgaphicallyGreater(const TERM& lhs,const TERM& rhs)");
  Comparison cmp =
    _ordering->compareLexicographically(lhs,rhs,_subst);
  return (cmp == Greater);
}; // bool StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst::lexicorgaphicallyGreater(const TERM& lhs,const TERM& rhs)

inline
bool StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst::lexicorgaphicallyGreaterOrEqual(const TERM& lhs,const TERM& rhs)
{
  CALL("lexicorgaphicallyGreaterOrEqual(const TERM& lhs,const TERM& rhs)");  
  Comparison cmp =
    _ordering->compareLexicographically(lhs,rhs,_subst);
  return (cmp == Greater) || (cmp == Equal);
}; // bool StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst::lexicorgaphicallyGreaterOrEqual(const TERM& lhs,const TERM& rhs)

inline
bool 
StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst::lexicorgaphicallyLess(const TERM& lhs,const TERM& rhs)
{
  return lexicorgaphicallyGreater(rhs,lhs);
};

inline
bool 
StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst::lexicorgaphicallyLessOrEqual(const TERM& lhs,const TERM& rhs)
{
  return lexicorgaphicallyGreaterOrEqual(rhs,lhs);
};



//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_STANDARD_KB_ORDERING
#define DEBUG_NAMESPACE "StandardKBOrdering::CompiledConstraintForUnifiers"
#endif
#include "debugMacros.h"
//===================================================================


StandardKBOrdering::CompiledConstraintForUnifiers::~CompiledConstraintForUnifiers() 
{
};  

void StandardKBOrdering::CompiledConstraintForUnifiers::load(PREFIX_SYM* lhs,PREFIX_SYM* rhs)
{
  CALL("load(PREFIX_SYM* lhs,PREFIX_SYM* rhs)");
  ASSERT(lhs->Head().IsFunctor());
  ASSERT(rhs->Head().IsFunctor());
  _lhs = lhs;
  _rhs = rhs;
  _lhsWeight.reset();
  _rhsWeight.reset();
  TermWeighting::current()->collectWeight(_lhsWeight,(TermWeightType)1,_lhs);
  TermWeighting::current()->collectWeight(_rhsWeight,(TermWeightType)1,_rhs);
}; // void StandardKBOrdering::CompiledConstraintForUnifiers::load(PREFIX_SYM* lhs,PREFIX_SYM* rhs)

bool StandardKBOrdering::CompiledConstraintForUnifiers::lhsVarInstMayOrder()
{
  CALL("lhsVarInstMayOrder()");
  return !_rhsWeight.containsVariableWhichIsNotIn(_lhsWeight);
}; // bool StandardKBOrdering::CompiledConstraintForUnifiers::lhsVarInstMayOrder()

 
bool StandardKBOrdering::CompiledConstraintForUnifiers::rhsVarInstMayOrder()
{
  CALL("rhsVarInstMayOrder()");
  return !_lhsWeight.containsVariableWhichIsNotIn(_rhsWeight);
}; // bool StandardKBOrdering::CompiledConstraintForUnifiers::rhsVarInstMayOrder()

void StandardKBOrdering::CompiledConstraintForUnifiers::compileGreaterOrEqual()
{
  CALL("compileGreaterOrEqual()");
#ifndef NO_DEBUG
  _targetRelation = GreaterOrEqual;
#endif
  _weightComparison.compare(_lhsWeight,_rhsWeight);
  Command* command = _mainCode.memory();
  bool compileWeightComparison = false;
  switch (_weightComparison.category())
    {
    case BK::FailureToCompare:
      command->setTag(Command::RetFailure);
      return;
    case BK::AlwaysLess:
      command->setTag(Command::RetFailure);
      return;
    case BK::AlwaysEqual:
      break;
    case BK::AlwaysGreater:
      command->setTag(Command::RetSuccess);
      return;
    case BK::CanBeLessOrEqual:
      compileWeightComparison = true;
      break;
    case BK::CanBeGreaterOrEqual:
      compileWeightComparison = true;
      break;
    case BK::VolatileButNearlyAlwaysLess:
      compileWeightComparison = true;
      break;
    case BK::VolatileButNearlyAlwaysGreater:
      compileWeightComparison = true;
      break;
    case BK::AbsolutelyVolatile:
      compileWeightComparison = true;
      break;
    };

  if (compileWeightComparison)
    {

      if (!_weightComparisonCode.size())
	{
	  DOP(_weightComparisonCode.unfreeze());
	  _weightComparisonCode.expand();
	  DOP(_weightComparisonCode.freeze());
	};
      WeightComparisonCommand* endOfWeightCode = 
	_weightComparisonCode.memory();
      while (!_weightComparison.compileGreaterOrEqual(endOfWeightCode,
						      _weightComparisonCode.size()))
	{
	  DOP(_weightComparisonCode.unfreeze());
	  _weightComparisonCode.expand();
	  DOP(_weightComparisonCode.freeze());
	  endOfWeightCode = _weightComparisonCode.memory();
	};      
      // naturalise the variables
      for (WeightComparisonCommand* c = _weightComparisonCode.memory();
	   c < endOfWeightCode;
	   ++c)
	if (c->requiresVar())
	  {
	    c->setNativeVar((void*)Unifier::current()->variable(c->var()));
	  };

    };
  
  // main code
 
  ulong maxSize;

 compile_main:
  command = _mainCode.memory();
  maxSize = _mainCode.size();
  if (compileWeightComparison)
    {
      command->setTag(Command::IfWCFailRetFail_IfWCAlwGrRetSucc);
      ++command;
      --maxSize;
    };
   
  if (!compileLexGreaterOrEqual(command,maxSize))
    {
      DOP(_mainCode.unfreeze());
      _mainCode.expand();
      DOP(_mainCode.freeze());
      goto compile_main;
    };

  /***********
	      DF; cout << "CHECK: ";
	      DF; PREFIX::outputTerm(cout,_lhs) << " >= ";
	      DF; PREFIX::outputTerm(cout,_rhs) << "\n";
	      DF; cout << "WEIGHT COMP:\n" << _weightComparison << "\n";
	      DF; output(cout << "\n\n") << "\n\n";
  ************/

}; // void StandardKBOrdering::CompiledConstraintForUnifiers::compileGreaterOrEqual()


bool StandardKBOrdering::CompiledConstraintForUnifiers::holdsOnCurrentUnifier()
{
  CALL("holdsOnCurrentUnifier()");

  //DF; output(cout << "\n\n\nHOLDS?\n") << "\n\n";   
  //DF; Unifier::current()->output(cout << "SUBST ",PREFIX::outputTerm) << "\n\n\n";  

  Command* command = _mainCode.memory();
  FunctionComparison weightComp;
  Comparison lexComp;

 next_command:
  switch (command->tag())
    {
    case Command::RetSuccess:
      goto return_true; 
    case Command::RetFailure:
      goto return_false;

    case Command::IfWCFailRetFail_IfWCAlwGrRetSucc:
      /*
	if WC(@) = BK::FailureToCompare return Failure;
	if WC(@) = BK::AlwaysGreater return Success;
	ASSERT(WC(@) = BK::AlwaysEqual || WC(@) = BK::CanBeGreaterOrEqual);
      */

      weightComp = interpretWeightComparisonCode();

      //DF; cout << "\n\n\nWEIGHT COMPARISON RETURNS: " << weightComp << "\n\n\n"; 

      if (weightComp == BK::FailureToCompare) goto return_false;
      if (weightComp == BK::AlwaysGreater) goto return_true;
      ASSERT((weightComp == BK::CanBeGreaterOrEqual) || (weightComp == BK::AlwaysEqual));
      ++command;
      goto next_command;

    case Command::IfLexSTEqIncRetFail_IfLexSTEqLeRetFail_RetSucc:
      /*
	ASSERT(top(S) = top(T));
	if lex(S@,T@) = Incomparable return Failure;
	if lex(S@,T@) = Less return Failure;
	return Success;
      */
      lexComp = compareLexicographically(command->complexTerm1(),
					 command->complexTerm2());
      if ((lexComp == BK::Greater) || (lexComp == BK::Equal))
	goto return_true;
      goto return_false;
     
#ifdef DEBUG_NAMESPACE
    default: ICP("ICP10"); goto return_false;
#else
#ifdef _SUPPRESS_WARNINGS_
    default: goto return_false;
#endif
#endif
    };
  
 return_true:

  ASSERT((_targetRelation != GreaterOrEqual) || SimplificationOrdering::current()->greaterOrEqualWithCurrentUnifier(_lhs,_rhs));

  return true;

 return_false:
  // v Very strong assumption!  
  ASSERT((_targetRelation != GreaterOrEqual) || (!SimplificationOrdering::current()->greaterOrEqualWithCurrentUnifier(_lhs,_rhs)));
  return false;
}; // bool StandardKBOrdering::CompiledConstraintForUnifiers::holdsOnCurrentUnifier()




#ifndef NO_DEBUG_VIS
ostream& StandardKBOrdering::CompiledConstraintForUnifiers::output(ostream& str) const
{
  PREFIX::outputTerm(str << "CONSTRAINT: ",_lhs);
  switch (_targetRelation)
    {
    case GreaterOrEqual: str << " >= "; break;
    default: str << "???";
    };
  PREFIX::outputTerm(str,_rhs) << "\n";
  DF; cout << "WEIGHT COMPARISON:\n" << _weightComparison << "\n";
  str << "********* MAIN CODE: *********************\n";
  const Command* command = _mainCode.memory();
  bool printWC = false;
 print_command:
  command->output(str);
  printWC = printWC || command->requiresWeightComparison();
  if (!command->isTerminal())
    {
      ++command;
      goto print_command;
    };

  if (printWC)
    {
      str << "********* WEIGHT COMPARISON CODE: ********\n";
      WeightPolynomial::SpecialisedComparison::Command::outputCode(str,_weightComparisonCode.memory()) 
	<< "\n";
    };
  str << "*********** END OF CODE ********\n";
  return str;
};
#endif



bool StandardKBOrdering::CompiledConstraintForUnifiers::compileLexGreaterOrEqual(Command*& code,ulong maxCodeSize)
{
  CALL("compileLexGreaterOrEqual(Command*& code,ulong maxCodeSize)");
  ASSERT(maxCodeSize);
  ASSERT(_lhs->Head().IsFunctor());
  ASSERT(_rhs->Head().IsFunctor());
  
  if (_lhs->Head() == _rhs->Head())
    {
      code->setTag(Command::IfLexSTEqIncRetFail_IfLexSTEqLeRetFail_RetSucc);
      code->setComplexTerm1(_lhs);
      code->setComplexTerm2(_rhs);
    }
  else
    {
      if (_lhs->Head().Func().hasGreaterPrecedenceThan(_rhs->Head().Func()))
	{
	  code->setTag(Command::RetSuccess);
	}
      else	
	code->setTag(Command::RetFailure); 
    };

  ++code; 
  return true;
}; // bool StandardKBOrdering::CompiledConstraintForUnifiers::compileLexGreaterOrEqual(Command*& code,ulong maxCodeSize)


inline 
Comparison 
StandardKBOrdering::CompiledConstraintForUnifiers::compareLexicographically(PREFIX_SYM* term1,PREFIX_SYM* term2)
{
  CALL("compareLexicographically(PREFIX_SYM* term1,PREFIX_SYM* term2)");
  ASSERT(term1->Head().IsFunctor());
  ASSERT(term2->Head().IsFunctor());
  ASSERT(term1->Head() == term2->Head()); 
  return _ordering->compareLexOnCurrentUnifier(term1,term2);
}; // Comparison StandardKBOrdering::CompiledConstraintForUnifiers::compareLexicographically(PREFIX_SYM* term1,PREFIX_SYM* term2)


inline 
Comparison 
StandardKBOrdering::CompiledConstraintForUnifiers::compareLexicographically(Unifier::Variable* var,PREFIX_SYM* term)
{
  CALL("compareLexicographically(Unifier::Variable* var,PREFIX_SYM* term)");
  ASSERT(term->Head().IsFunctor());
  return Incomparable; // temporary
}; // Comparison StandardKBOrdering::CompiledConstraintForUnifiers::compareLexicographically(Unifier::Variable* var,PREFIX_SYM* term)

inline
Comparison StandardKBOrdering::CompiledConstraintForUnifiers::compareLexicographically(Unifier::Variable* var1,Unifier::Variable* var2)
{
  CALL("compareLexicographically(Unifier::Variable* var1,Unifier::Variable* var2)");
  
  return Incomparable; // temporary

}; // Comparison StandardKBOrdering::CompiledConstraintForUnifiers::compareLexicographically(Unifier::Variable* var1,Unifier::Variable* var2)


inline
FunctionComparison 
StandardKBOrdering::CompiledConstraintForUnifiers::interpretWeightComparisonCode()
{
  CALL("interpretWeightComparisonCode()");
  const WeightComparisonCommand* command = _weightComparisonCode.memory();

 next_command:

  //DF; command->outputShort(cout << "DO ") << "\n";

  switch (command->tag())
    {
      
    case WeightComparisonCommand::RetFailureToCompare: return BK::FailureToCompare;
    case WeightComparisonCommand::RetAlwaysLess: ICP("ICP10"); return BK::FailureToCompare;
    case WeightComparisonCommand::RetAlwaysEqual: return BK::AlwaysEqual;
    case WeightComparisonCommand::RetAlwaysGreater: return BK::AlwaysGreater;
    case WeightComparisonCommand::RetCanBeLessOrEqual: ICP("ICP20"); return BK::FailureToCompare;
    case WeightComparisonCommand::RetCanBeGreaterOrEqual: return BK::CanBeGreaterOrEqual;
    case WeightComparisonCommand::RetVolatileButNearlyAlwaysLess: ICP("ICP30");  return BK::FailureToCompare;
    case WeightComparisonCommand::RetVolatileButNearlyAlwaysGreater: ICP("ICP40"); return BK::FailureToCompare;
    case WeightComparisonCommand::RetAbsolutelyVolatile: ICP("ICP50"); return BK::FailureToCompare;

      //----------------------------------------

    case WeightComparisonCommand::IfXGr1RetFail_IfGrndRetAlwEq_RetFail:
      /*
	if min|X@| > 1 return BK::FailureToCompare;
	if ground(X@) return BK::AlwaysEqual;
	return BK::FailureToCompare;
      */
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar) return BK::FailureToCompare; // nonground
	TermWeightType w = TermWeighting::current()->computeWeightIfGroundAndLessOrEqual((PREFIX_SYM*)instance,(TermWeightType)1);
	if (w <= 0) return BK::FailureToCompare;
	return BK::AlwaysEqual;
      };

    case WeightComparisonCommand::IfXGr1RetFail_IfNotGrndRetFail:
      /*
	if min|X@| > 1 return BK::FailureToCompare;
	if !ground(X@) return BK::FailureToCompare;
      */
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar) return BK::FailureToCompare; // nonground
	TermWeightType w = TermWeighting::current()->computeWeightIfGroundAndLessOrEqual((PREFIX_SYM*)instance,(TermWeightType)1);
	if (w <= 0) return BK::FailureToCompare;
      };
      break;


      //----------------------------------------



    case WeightComparisonCommand::IfXGr1RetAlwGr_IfGrndRetAlwEq_RetCanBeGrOrEq:
      /*
	if min|X@| > 1 return BK::AlwaysGreater;
	if ground(X@) return BK::AlwaysEqual;
	return BK::CanBeGreaterOrEqual;
      */
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar) return BK::CanBeGreaterOrEqual;
	
	bool ground;
	TermWeightType w = TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual((PREFIX_SYM*)instance,
											       (TermWeightType)1,
											       ground);
	if (w < (TermWeightType)0) return BK::AlwaysGreater;
	ASSERT(w == (TermWeightType)1);
	if (ground) return BK::AlwaysEqual;
	return BK::CanBeGreaterOrEqual;
      };

    case WeightComparisonCommand::IfXGr1RetAlwGr_GrndToGRND:
      /*
	if min|X@| > 1 return BK::AlwaysGreater;
	GRND := ground(X@);
      */
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _grnd = false;
	  }
	else
	  {
	    bool ground;
	    TermWeightType w = TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual((PREFIX_SYM*)instance,
												   (TermWeightType)1,
												   ground);
	    if (w < (TermWeightType)0) return BK::AlwaysGreater;
	    _grnd = ground;
	  };
      };
      break;
      

    case WeightComparisonCommand::IfXGr1RetAlwGr_UpdateGRND:
      /*
	if min|X@| > 1 return BK::AlwaysGreater;
	GRND := GRND && ground(X@);
      */
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _grnd = false;
	  }
	else
	  {
	    bool ground;
	    TermWeightType w = TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual((PREFIX_SYM*)instance,
												   (TermWeightType)1,
												   ground);
	    if (w < (TermWeightType)0) return BK::AlwaysGreater;
	    _grnd = _grnd && ground;
	  };
      };
      break;

    case WeightComparisonCommand::IfXGr1RetAlwGr_UpdateGRND_IfGRNDRetAlwEq_RetCanBeGrOrEq:
      /*
	if min|X@| > 1 return BK::AlwaysGreater;
	GRND := GRND && ground(X@);
	if GRND return BK::AlwaysEqual;
	return BK::CanBeGreaterOrEqual;
      */
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    return BK::CanBeGreaterOrEqual;
	  }
	else
	  {
	    bool ground;
	    TermWeightType w = TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual((PREFIX_SYM*)instance,
												   (TermWeightType)1,
												   ground);
	    if (w < (TermWeightType)0) return BK::AlwaysGreater;
	    _grnd = _grnd && ground;
	    if (_grnd) return BK::AlwaysEqual;
	    return BK::CanBeGreaterOrEqual;
	  };
      };



      //----------------------------------------


    case WeightComparisonCommand::IfNotGrndRetFail_IfXGrWRetFail_IfXLeWRetAlwGr_RetAlwEq:
      /*
	if !ground(X@) return BK::FailureToCompare;
	if min|X@| > W return BK::FailureToCompare;
	if min|X@| < W return BK::AlwaysGreater;
	return BK::AlwaysEqual;
      */
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    return BK::FailureToCompare;
	  }
	else
	  {	    
	    TermWeightType w = TermWeighting::current()->computeWeightIfGroundAndLessOrEqual((PREFIX_SYM*)instance,
											     command->weight());
	    if (w <= (TermWeightType)0) return BK::FailureToCompare;
	    if (w < command->weight()) 
	      {
		return BK::AlwaysGreater;
	      }
	    else
	      {
		ASSERT(w == command->weight());
		return BK::AlwaysEqual;
	      };
	    
	  };
      };


    case WeightComparisonCommand::IfNotGrndRetFail_IfCXGrWRetFail_IfCXLeWRetAlwGr_RetAlwEq:
      /*
	if !ground(X@) return BK::FailureToCompare;
	if C*min|X@| > W return BK::FailureToCompare;
	if C*min|X@| < W return BK::AlwaysGreater;
	return BK::AlwaysEqual;
      */
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    return BK::FailureToCompare;
	  }
	else
	  {	    
	    TermWeightType w = TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(command->coefficient(),
											     (PREFIX_SYM*)instance,
											     command->weight());
	    if (w <= (TermWeightType)0) return BK::FailureToCompare;
	    if (w < command->weight()) 
	      {
		return BK::AlwaysGreater;
	      }
	    else
	      {
		ASSERT(w == command->weight());
		return BK::AlwaysEqual;
	      };
	    
	  };
      };



    case WeightComparisonCommand::IfNotGrndRetFail_WMinusXToACC_IfACCLe0RetFail:
      /*
	if !ground(X@) return BK::FailureToCompare;
	ACC := W - min|X@|;
	if ACC < 0 return BK::FailureToCompare;      
      */
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    return BK::FailureToCompare;
	  }
	else
	  {	
	    TermWeightType w = TermWeighting::current()->computeWeightIfGroundAndLessOrEqual((PREFIX_SYM*)instance,
											     command->weight());
	    if (w <= (TermWeightType)0) return BK::FailureToCompare;
	    _acc = command->weight() - w;
	    ASSERT(_acc >= (TermWeightType)0);
	  };
      };
      break;



    case WeightComparisonCommand::IfNotGrndRetFail_WMinusCXToACC_IfACCLe0RetFail:
      /*
	if !ground(X@) return BK::FailureToCompare;
	ACC := W - C*min|X@|;
	if ACC < 0 return BK::FailureToCompare;      
      */
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    return BK::FailureToCompare;
	  }
	else
	  {	
	    TermWeightType w = TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(command->coefficient(),
											     (PREFIX_SYM*)instance,
											     command->weight());
	    if (w <= (TermWeightType)0) return BK::FailureToCompare;
	    _acc = command->weight() - w;
	    ASSERT(_acc >= (TermWeightType)0);
	  };
      };
      break;


    case WeightComparisonCommand::IfNotGrndRetFail_ACCMinusXPlus1ToACC_IfACCLe0RetFail:
      /*
	if !ground(X@) return BK::FailureToCompare;
	ACC := ACC - min|X@| + 1;
	if ACC < 0 return BK::FailureToCompare;
      */    
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    return BK::FailureToCompare;
	  }
	else
	  {	
	    ++_acc;
	    TermWeightType w = TermWeighting::current()->computeWeightIfGroundAndLessOrEqual((PREFIX_SYM*)instance,
											     _acc);
	    if (w <= (TermWeightType)0) return BK::FailureToCompare;
	    _acc -= w;
	    ASSERT(_acc >= (TermWeightType)0);
	  };
      };
      break;
    

    case WeightComparisonCommand::IfNotGrndRetFail_ACCMinusCXPlusCToACC_IfACCLe0RetFail:
      /*
	if !ground(X@) return BK::FailureToCompare;
	ACC := ACC - C*min|X@| + C;
	if ACC < 0 return BK::FailureToCompare; 
      */    
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    return BK::FailureToCompare;
	  }
	else
	  {	
	    _acc += command->coefficient();
	    TermWeightType w = TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(command->coefficient(),
											     (PREFIX_SYM*)instance,
											     _acc);
	    if (w <= (TermWeightType)0) return BK::FailureToCompare;
	    _acc -= w;
	    ASSERT(_acc >= (TermWeightType)0);
	  };
      };
      break;
      


    case WeightComparisonCommand::IfNotGrndRetFail_ACCMinusXPlus1ToACC_IfACCLe0RetFail_IfACCEq0RetAlwEq_RetAlwGr:
      /*
	if !ground(X@) return BK::FailureToCompare;
	ACC := ACC - min|X@| + 1;
	if ACC < 0 return BK::FailureToCompare; 
	if ACC = 0 return BK::AlwaysEqual;
	return BK::AlwaysGreater;
      */    
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    return BK::FailureToCompare;
	  }
	else
	  {	
	    ++_acc;
	    TermWeightType w = TermWeighting::current()->computeWeightIfGroundAndLessOrEqual((PREFIX_SYM*)instance,
											     _acc);
	    if (w <= (TermWeightType)0) return BK::FailureToCompare;
	    if (_acc == w) return BK::AlwaysEqual;
	    return BK::AlwaysGreater;
	  };
      };


    case WeightComparisonCommand::IfNotGrndRetFail_ACCMinusCXPlusCToACC_IfACCLe0RetFail_IfACCEq0RetAlwEq_RetAlwGr:
      /*
	if !ground(X@) return BK::FailureToCompare;
	ACC := ACC - C*min|X@| + C;
	if ACC < 0 return BK::FailureToCompare; 
	if ACC = 0 return BK::AlwaysEqual;
	return BK::AlwaysGreater;
      */    
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    return BK::FailureToCompare;
	  }
	else
	  {	
	    _acc += command->coefficient(); 
	    TermWeightType w = TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(command->coefficient(),
											     (PREFIX_SYM*)instance,
											     _acc);
	    if (w <= (TermWeightType)0) return BK::FailureToCompare;
	    if (_acc == w) return BK::AlwaysEqual;
	    return BK::AlwaysGreater;
	  };
      };


      //----------------------------------------



    case WeightComparisonCommand::IfXGrWRetAlwGr_IfXLeWRetFail_IfGrndRetAlwEq_RetCanBeGrOrEq:
      /*
	ASSERT(W > 0L);
	if min|X@| > W return BK::AlwaysGreater;
	if min|X@| < W return BK::FailureToCompare;
	if ground(X@) return BK::AlwaysEqual;
	return BK::CanBeGreaterOrEqual;
      */
      {
	ASSERT(command->weight() > (TermWeightType)0);
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    // min|X@| == 1, ground(X@) == false
	    if ((TermWeightType)1 < command->weight()) return BK::FailureToCompare;
	    return BK::CanBeGreaterOrEqual;
	  }
	else
	  {
	    bool ground;
	    TermWeightType w = 
	      TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual((PREFIX_SYM*)instance,
										  command->weight(),
										  ground);
	    if (w < (TermWeightType)0) return BK::AlwaysGreater;
	    ASSERT((w > (TermWeightType)0) && (w <= command->weight()));
	    if (w < command->weight()) return BK::FailureToCompare;
	    if (ground) return BK::AlwaysEqual;
	    return BK::CanBeGreaterOrEqual;
	  };
      };
      
    
    case WeightComparisonCommand::IfCXGrWRetAlwGr_IfCXLeWRetFail_IfGrndRetAlwEq_RetCanBeGrOrEq:
      /*
	ASSERT(W > (TermWeightType)0);
	if C*min|X@| > W return BK::AlwaysGreater;
	if C*min|X@| < W return BK::FailureToCompare;
	if ground(X@) return BK::AlwaysEqual;
	return BK::CanBeGreaterOrEqual;
      */
      {
	ASSERT(command->weight() > (TermWeightType)0);
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    // min|X@| == 1, ground(X@) == false
	    if (command->coefficient() < command->weight()) return BK::FailureToCompare;
	    return BK::CanBeGreaterOrEqual;
	  }
	else
	  {
	    bool ground;
	    TermWeightType w = 
	      TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual(command->coefficient(),
										  (PREFIX_SYM*)instance,
										  command->weight(),
										  ground);
	    if (w < (TermWeightType)0) return BK::AlwaysGreater;
	    ASSERT((w > (TermWeightType)0) && (w <= command->weight()));
	    if (w < command->weight()) return BK::FailureToCompare;
	    if (ground) return BK::AlwaysEqual;
	    return BK::CanBeGreaterOrEqual;
	  };
      };


    case WeightComparisonCommand::WMinusXToACC_IfACCLe0RetAlwGr_GrndToGRND:
      /*
	ACC := W - min|X@|;
	if ACC < 0 return BK::AlwaysGreater;
	GRND := ground(X@);
      */
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _acc = command->weight() - (TermWeightType)1;
	    if (_acc < (TermWeightType)0) return BK::AlwaysGreater;
	    _grnd = false;
	  }
	else
	  {
	    bool ground;
	    TermWeightType w = 
	      TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual((PREFIX_SYM*)instance,
										  command->weight(),
										  ground);
	    if (w < (TermWeightType)0) return BK::AlwaysGreater;
	    _acc = command->weight() - w;
	    _grnd = ground;
	  };
	
      };
      break;

    case WeightComparisonCommand::WMinusCXToACC_IfACCLe0RetAlwGr_GrndToGRND:
      /*
	ACC := W - C*min|X@|;
	if ACC <= 0 return BK::AlwaysGreater;
	GRND := ground(X@);
      */
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _acc = command->weight() - command->coefficient();
	    if (_acc < (TermWeightType)0) return BK::AlwaysGreater;
	    _grnd = false;
	  }
	else
	  {
	    bool ground;
	    TermWeightType w = 
	      TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual(command->coefficient(),
										  (PREFIX_SYM*)instance,
										  command->weight(),
										  ground);
	    if (w < (TermWeightType)0) return BK::AlwaysGreater;
	    _acc = command->weight() - w;
	    _grnd = ground;
	  };
	
      };
      break;
    
    case WeightComparisonCommand::ACCMinusXPlus1ToACC_IfACCLe0RetAlwGr_UpdateGRND:
      /*
	ACC := ACC - min|X@| + 1;
	if ACC < 0 return BK::AlwaysGreater;
	GRND := GRND && ground(X@);
      */    
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    if (_acc < (TermWeightType)0) return BK::AlwaysGreater;
	    _grnd = false;
	  }
	else
	  {
	    ++_acc;
	    bool ground;
	    TermWeightType w = 
	      TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual((PREFIX_SYM*)instance,
										  _acc,
										  ground);
	    if (w < (TermWeightType)0) return BK::AlwaysGreater;
	    _acc -= w;
	    ASSERT(_acc >= (TermWeightType)0);
	    _grnd = _grnd && ground;
	  };
      };
      break;

    
    case WeightComparisonCommand::ACCMinusCXPlusCToACC_IfACCLe0RetAlwGr_UpdateGRND:
      /*
	ACC := ACC - C*min|X@| + C;
	if ACC < 0 return BK::AlwaysGreater;
	GRND := GRND && ground(X@);
      */    
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    if (_acc < (TermWeightType)0) return BK::AlwaysGreater;
	    _grnd = false;
	  }
	else
	  {
	    _acc += command->coefficient();
	    bool ground;
	    TermWeightType w = 
	      TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual(command->coefficient(),
										  (PREFIX_SYM*)instance,
										  _acc,
										  ground);
	    if (w < (TermWeightType)0) return BK::AlwaysGreater;
	    _acc -= w;
	    ASSERT(_acc >= (TermWeightType)0);
	    _grnd = _grnd && ground;
	  };
      };
      break;


    case WeightComparisonCommand::IfXGrACCPlus1RetAlwGr_IfXLeACCPlus1RetFail_UpdateGRND_IfGRNDRetAlwEq_RetCanBeGrOrEq:
      /*
	if min|X@| > ACC + 1 return BK::AlwaysGreater;
	if min|X@| < ACC + 1 return BK::FailureToCompare;      
	GRND := GRND && ground(X@);
	if GRND return BK::AlwaysEqual;
	return BK::CanBeGreaterOrEqual;
      */   
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    if (_acc < (TermWeightType)0) return BK::AlwaysGreater;
	    if (_acc > (TermWeightType)0) return BK::FailureToCompare;  
	    return BK::CanBeGreaterOrEqual;
	  }
	else
	  {
	    ++_acc;
	    bool ground;
	    TermWeightType w = 
	      TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual((PREFIX_SYM*)instance,
										  _acc,
										  ground);
	    if (w < (TermWeightType)0) return BK::AlwaysGreater;
	    if (w < _acc) return BK::FailureToCompare; 
	    ASSERT(w == _acc);
	    if (_grnd && ground) return BK::AlwaysEqual;
	    return BK::CanBeGreaterOrEqual;
	  };
      };
    
    case WeightComparisonCommand::IfCXGrACCPlusCRetAlwGr_IfCXLeACCPlusCRetFail_UpdateGRND_IfGRNDRetAlwEq_RetCanBeGrOrEq:
      /*
	if C*min|X@| > ACC + C return BK::AlwaysGreater;
	if C*min|X@| < ACC + C return BK::FailureToCompare;      
	GRND := GRND && ground(X@);
	if GRND return BK::AlwaysEqual;
	return BK::CanBeGreaterOrEqual;
      */   
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    if (_acc < (TermWeightType)0) return BK::AlwaysGreater;
	    if (_acc > (TermWeightType)0) return BK::FailureToCompare;  
	    return BK::CanBeGreaterOrEqual;
	  }
	else
	  {
	    _acc += command->coefficient();
	    bool ground;
	    TermWeightType w = 
	      TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual(command->coefficient(),
										  (PREFIX_SYM*)instance,
										  _acc,
										  ground);
	    if (w < (TermWeightType)0) return BK::AlwaysGreater;
	    if (w < _acc) return BK::FailureToCompare; 
	    ASSERT(w == _acc);
	    if (_grnd && ground) return BK::AlwaysEqual;
	    return BK::CanBeGreaterOrEqual;
	  };
      };
    
      //----------------------------------------

    case WeightComparisonCommand::XPlusWToPOSINST_IfConstPOSINSAndPOSINSTLe0RetFail_IfConstPOSINSTGotoL:
      /*
	POSINST := |X@| + W;
	if constant(POSINST) && POSINST < 0 return BK::FailureToCompare; 
	if constant(POSINST) goto L; 
      */
      {
	_posinst.reset();
	_posinst.add(command->weight());
	
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _posinst.add1(((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {	    
	    TermWeighting::current()->collectWeight(_posinst,(const PREFIX_SYM*)instance);
	    if (_posinst.isConstant())
	      {
		if (_posinst.freeMember() < (TermWeightType)0) return BK::FailureToCompare;  
		command = command->label();
		goto next_command;
	      };
	  };
      };
      break;


    case WeightComparisonCommand::CXPlusWToPOSINST_IfConstPOSINSAndPOSINSTLe0RetFail_IfConstPOSINSTGotoL:
      /*
	POSINST := C*|X@| + W;
	if constant(POSINST) && POSINST < 0 return BK::FailureToCompare; 
	if constant(POSINST) goto L;
      */
      {
	_posinst.reset();
	_posinst.add(command->weight());
	
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _posinst.add(command->coefficient(),((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {	    
	    TermWeighting::current()->collectWeight(_posinst,command->coefficient(),(const PREFIX_SYM*)instance);
	    if (_posinst.isConstant())
	      {
		if (_posinst.freeMember() < (TermWeightType)0) return BK::FailureToCompare;  
		command = command->label();
		goto next_command;
	      };
	  };
      };
      break;


    case WeightComparisonCommand::XPlusWToPOSINST:
      /*
	POSINST := |X@| + W;
      */
      {
	_posinst.reset();
	_posinst.add(command->weight());
	
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _posinst.add1(((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {	    
	    TermWeighting::current()->collectWeight(_posinst,(const PREFIX_SYM*)instance);
	  };
      };
      break;
    
    case WeightComparisonCommand::CXPlusWToPOSINST:
      /*
	POSINST := C*|X@| + W;
      */
      {
	_posinst.reset();
	_posinst.add(command->weight());
	
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _posinst.add(command->coefficient(),((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {	    
	    TermWeighting::current()->collectWeight(_posinst,command->coefficient(),(const PREFIX_SYM*)instance);
	  };
      };
      break;

    case WeightComparisonCommand::POSINSTPlusXToPOSINST:
      /*
	POSINST := POSINST + |X@|;
      */
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _posinst.add1(((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {	    
	    TermWeighting::current()->collectWeight(_posinst,(const PREFIX_SYM*)instance);
	  };
      };
      break;
    
    case WeightComparisonCommand::POSINSTPlusCXToPOSINST:
      /*
	POSINST := POSINST + C*|X@|;
      */
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _posinst.add(command->coefficient(),((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {	    
	    TermWeighting::current()->collectWeight(_posinst,command->coefficient(),(const PREFIX_SYM*)instance);
	  };
      };
      break;

    
    case WeightComparisonCommand::POSINSTPlusXToPOSINST_IfConstPOSINSTAndPOSINSTLe0RetFail_IfConstPOSINSTGotoL:
      /*
	POSINST := POSINST + |X@|;
	if constant(POSINST) && POSINST < 0 return BK::FailureToCompare; 
	if constant(POSINST) goto L;
      */
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _posinst.add1(((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {	    
	    TermWeighting::current()->collectWeight(_posinst,(const PREFIX_SYM*)instance);
	  };
	if (_posinst.isConstant())
	  {
	    if (_posinst.freeMember() < (TermWeightType)0) return BK::FailureToCompare; 
	    command = command->label();
	    goto next_command;
	  };
      };
      break;

    case WeightComparisonCommand::POSINSTPlusCXToPOSINST_IfConstPOSINSTAndPOSINSTLe0RetFail_IfConstPOSINSTGotoL:
      /*
	POSINST := POSINST + C*|X@|;
	if constant(POSINST) && POSINST < 0 return BK::FailureToCompare; 
	if constant(POSINST) goto L;
      */
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _posinst.add(command->coefficient(),((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {	    
	    TermWeighting::current()->collectWeight(_posinst,command->coefficient(),(const PREFIX_SYM*)instance);
	  };
	if (_posinst.isConstant())
	  {
	    if (_posinst.freeMember() < (TermWeightType)0) return BK::FailureToCompare; 
	    command = command->label();
	    goto next_command;
	  };
      };
      break;

    case WeightComparisonCommand::XMinus1ToNEGINSTRetCompForGrOrEqPOSINSTvNEGINST:
      /*
	NEGINST := |X@| - 1;
	return compareForGreaterOrEqual(POSINST,NEGINST);
      */
      {
	_neginst.reset();
	_neginst.add((TermWeightType)(-1));
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _neginst.add1(((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {
	    TermWeighting::current()->collectWeight(_neginst,(const PREFIX_SYM*)instance);
	  };
	return _posinst.compareForGreaterOrEqual(_neginst);
      };
    
    case WeightComparisonCommand::CXMinusCToNEGINSTRetCompForGrOrEqPOSINSTvNEGINST:
      /*
	NEGINST := C*|X@| - C;
	return compareForGreaterOrEqual(POSINST,NEGINST);
      */
      {
	_neginst.reset();
	_neginst.add(-(command->coefficient()));
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _neginst.add(command->coefficient(),((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {
	    TermWeighting::current()->collectWeight(_neginst,command->coefficient(),(const PREFIX_SYM*)instance);
	  };
	return _posinst.compareForGreaterOrEqual(_neginst);
      };


    case WeightComparisonCommand::XMinus1ToNEGINST_IfCompForGrOrEqPOSINSTvNEGINSTEqFailRetFail:
      /*
	NEGINST := |X@| - 1;
	if compareForGreaterOrEqual(POSINST,NEGINST) == BK::FailureToCompare return BK::FailureToCompare;
      */
      {
	_neginst.reset();
	_neginst.add((TermWeightType)(-1));
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _neginst.add1(((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {
	    TermWeighting::current()->collectWeight(_neginst,(const PREFIX_SYM*)instance);
	  };
	if (_posinst.compareForGreaterOrEqual(_neginst) == BK::FailureToCompare) 
	  return BK::FailureToCompare;
      };
      break;

    case WeightComparisonCommand::CXMinusCToNEGINST_IfCompForGrOrEqPOSINSTvNEGINSTEqFailRetFail:
      /*
	NEGINST := C*|X@| - C;
	if compareForGreaterOrEqual(POSINST,NEGINST) == BK::FailureToCompare return BK::FailureToCompare;
      */
      {
	_neginst.reset();
	_neginst.add(-(command->coefficient()));
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _neginst.add(command->coefficient(),((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {
	    TermWeighting::current()->collectWeight(_neginst,command->coefficient(),(const PREFIX_SYM*)instance);
	  };
	if (_posinst.compareForGreaterOrEqual(_neginst) == BK::FailureToCompare) 
	  return BK::FailureToCompare;
      };
      break;

    case WeightComparisonCommand::NEGINSTPlusXMinus1ToNEGINST_IfCompForGrOrEqPOSINSTvNEGINSTEqFailRetFail:
      /*
	NEGINST := NEGINST + |X@| - 1;
	if compareForGreaterOrEqual(POSINST,NEGINST) == BK::FailureToCompare return BK::FailureToCompare;
      */
      {
	_neginst.add((TermWeightType)(-1));
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _neginst.add1(((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {
	    TermWeighting::current()->collectWeight(_neginst,(const PREFIX_SYM*)instance);
	  };
	if (_posinst.compareForGreaterOrEqual(_neginst) == BK::FailureToCompare) 
	  return BK::FailureToCompare;
      };
      break;

    case WeightComparisonCommand::NEGINSTPlusCXMinusCToNEGINST_IfCompForGrOrEqPOSINSTvNEGINSTEqFailRetFail:
      /*
	NEGINST := NEGINST + C*|X@| - C;
	if compareForGreaterOrEqual(POSINST,NEGINST) == BK::FailureToCompare return BK::FailureToCompare;
      */
      {
	_neginst.add(-(command->coefficient()));
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _neginst.add(command->coefficient(),((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {
	    TermWeighting::current()->collectWeight(_neginst,command->coefficient(),(const PREFIX_SYM*)instance);
	  };
	if (_posinst.compareForGreaterOrEqual(_neginst) == BK::FailureToCompare) 
	  return BK::FailureToCompare;
      };
      break;

    case WeightComparisonCommand::NEGINSTPlusXMinus1ToNEGINST_RetCompForGrOrEqPOSINSTvNEGINST:
      /*
	NEGINST := NEGINST + |X@| - 1;
	return compareForGreaterOrEqual(POSINST,NEGINST);
      */
      {
	_neginst.add((TermWeightType)(-1));
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _neginst.add1(((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {
	    TermWeighting::current()->collectWeight(_neginst,(const PREFIX_SYM*)instance);
	  };
	return _posinst.compareForGreaterOrEqual(_neginst);
      };

    case WeightComparisonCommand::NEGINSTPlusCXMinusCToNEGINST_RetCompForGrOrEqPOSINSTvNEGINST:
      /*
	NEGINST := NEGINST + C*|X@| - C;
	return compareForGreaterOrEqual(POSINST,NEGINST);
      */
      {
	_neginst.add(-(command->coefficient()));
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _neginst.add(command->coefficient(),((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {
	    TermWeighting::current()->collectWeight(_neginst,command->coefficient(),(const PREFIX_SYM*)instance);
	  };
	return _posinst.compareForGreaterOrEqual(_neginst);
      };


    case WeightComparisonCommand::IfNotGrndRetFail_IfXMinus1GrPOSINSTRetFail_IfXMinus1LePOSINSTRetAlwGr_RetAlwEq:
      /*
	ASSERT(constant(POSINST));
	ASSERT(POSINST >= 0);
	if !ground(X@) return BK::FailureToCompare;
	if min|X@| - 1 > POSINST return BK::FailureToCompare;
	if min|X@| - 1 < POSINST return BK::AlwaysGreater;
	return BK::AlwaysEqual;
      */
      {
	ASSERT(_posinst.isConstant());
	ASSERT(_posinst.freeMember() >= (TermWeightType)0);
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    return BK::FailureToCompare;
	  }
	else
	  {
	    _posinst.add1();
	    TermWeightType w = 
	      TermWeighting::current()->computeWeightIfGroundAndLessOrEqual((const PREFIX_SYM*)instance,
									    _posinst.freeMember());
	    if (w <= (TermWeightType)0) return BK::FailureToCompare;
	    ASSERT(w <= _posinst.freeMember());
	    if (w < _posinst.freeMember()) return BK::AlwaysGreater;
	    return BK::AlwaysEqual;
	  };
      };

    case WeightComparisonCommand::IfNotGrndRetFail_IfCXMinusCGrPOSINSTRetFail_IfCXMinusCLePOSINSTRetAlwGr_RetAlwEq:
      /*
	ASSERT(constant(POSINST));
	ASSERT(POSINST >= 0);
	if !ground(X@) return BK::FailureToCompare;
	if C*min|X@| - C > POSINST return BK::FailureToCompare;
	if C*min|X@| - C < POSINST return BK::AlwaysGreater;
	return BK::AlwaysEqual;
      */
      {
	ASSERT(_posinst.isConstant());
	ASSERT(_posinst.freeMember() >= (TermWeightType)0);
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    return BK::FailureToCompare;
	  }
	else
	  {
	    _posinst.add(command->coefficient());
	    TermWeightType w = 
	      TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(command->coefficient(),
									    (const PREFIX_SYM*)instance,
									    _posinst.freeMember());
	    if (w <= (TermWeightType)0) return BK::FailureToCompare;
	    ASSERT(w <= _posinst.freeMember());
	    if (w < _posinst.freeMember()) return BK::AlwaysGreater;
	    return BK::AlwaysEqual;
	  };
      };


    case WeightComparisonCommand::IfNotGrndRetFail_POSINSTPlus1MinusXToACC_IfACCLe0RetFail:
      /*
	ASSERT(constant(POSINST));
	ASSERT(POSINST >= 0);
	if !ground(X@) return BK::FailureToCompare;
	ACC := POSINST + 1 - min|X@|;      
	if ACC < 0 return BK::FailureToCompare; 
      */
      {	
	ASSERT(_posinst.isConstant());
	ASSERT(_posinst.freeMember() >= (TermWeightType)0);
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    return BK::FailureToCompare;
	  }
	else
	  {	    
	    _acc = _posinst.freeMember() + 1;
	    TermWeightType w = 
	      TermWeighting::current()->computeWeightIfGroundAndLessOrEqual((const PREFIX_SYM*)instance,
									    _acc);
	    if (w <= (TermWeightType)0) return BK::FailureToCompare;
	    _acc -= w;
	    ASSERT(_acc >= (TermWeightType)0);
	  };
      };
      break;

    case WeightComparisonCommand::IfNotGrndRetFail_POSINSTPlusCMinusCXToACC_IfACCLe0RetFail:
      /*
	ASSERT(constant(POSINST));
	ASSERT(POSINST >= 0);
	if !ground(X@) return BK::FailureToCompare;
	ACC := POSINST + C - C*min|X@|;      
	if ACC < 0 return BK::FailureToCompare; 
      */
      {	
	ASSERT(_posinst.isConstant());
	ASSERT(_posinst.freeMember() >= (TermWeightType)0);
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    return BK::FailureToCompare;
	  }
	else
	  {	    
	    _acc = _posinst.freeMember() + command->coefficient();
	    TermWeightType w = 
	      TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(command->coefficient(),
									    (const PREFIX_SYM*)instance,
									    _acc);
	    if (w <= (TermWeightType)0) return BK::FailureToCompare;
	    _acc -= w;
	    ASSERT(_acc >= (TermWeightType)0);
	  };
      };
      break;




    case WeightComparisonCommand::XPlusWToNEGINST_IfConstNEGINSTAndNEGINSTLe0RetAlwGr_IfConstNEGINSTGotoL:
      /*
	NEGINST := |X@| + W;
	if constant(NEGINST) && NEGINST < 0 return BK::AlwaysGreater;
	if constant(NEGINST) goto L;
      */
      {
	_neginst.reset();
	_neginst.add(command->weight());
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _neginst.add1(((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {
	    TermWeighting::current()->collectWeight(_neginst,(const PREFIX_SYM*)instance);
	    if (_neginst.isConstant())
	      {
		if (_neginst.freeMember() < (TermWeightType)0) return BK::AlwaysGreater;
		command = command->label();
		goto next_command;
	      };
	  };
      };
      break;
     


    case WeightComparisonCommand::CXPlusWToNEGINST_IfConstNEGINSTAndNEGINSTLe0RetAlwGr_IfConstNEGINSTGotoL:
      /*
	NEGINST := C*|X@| + W;
	if constant(NEGINST) && NEGINST < 0 return BK::AlwaysGreater;
	if constant(NEGINST) goto L;
      */
      {
	_neginst.reset();
	_neginst.add(command->weight());
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _neginst.add(command->coefficient(),((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {
	    TermWeighting::current()->collectWeight(_neginst,command->coefficient(),(const PREFIX_SYM*)instance);
	    if (_neginst.isConstant())
	      {
		if (_neginst.freeMember() < (TermWeightType)0) return BK::AlwaysGreater;
		command = command->label();
		goto next_command;
	      };
	  };
      };
      break;


    case WeightComparisonCommand::XPlusWToNEGINST:
      /*
	NEGINST := |X@| + W;
      */
      {
	_neginst.reset();
	_neginst.add(command->weight());
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _neginst.add1(((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {
	    TermWeighting::current()->collectWeight(_neginst,(const PREFIX_SYM*)instance);
	  };
      };
      break;

    
    case WeightComparisonCommand::CXPlusWToNEGINST:
      /*
	NEGINST := C*|X@| + W;
      */
      {
	_neginst.reset();
	_neginst.add(command->weight());
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _neginst.add(command->coefficient(),((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {
	    TermWeighting::current()->collectWeight(_neginst,command->coefficient(),(const PREFIX_SYM*)instance);
	  };
      };
      break;

        
    case WeightComparisonCommand::NEGINSTPlusXToNEGINST:
      /*
	NEGINST := NEGINST + |X@|;
      */
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _neginst.add1(((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {
	    TermWeighting::current()->collectWeight(_neginst,(const PREFIX_SYM*)instance);
	  };
      };
      break;

    case WeightComparisonCommand::NEGINSTPlusCXToNEGINST:
      /*
	NEGINST := NEGINST + C*|X@|;
      */
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _neginst.add(command->coefficient(),((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {
	    TermWeighting::current()->collectWeight(_neginst,command->coefficient(),(const PREFIX_SYM*)instance);
	  };
      };
      break;
    
    case WeightComparisonCommand::NEGINSTPlusXToNEGINST_IfConstNEGINSTAndNEGINSTLe0RetAlwGr_IfConstNEGINSTGotoL:
      /*
	NEGINST := NEGINST + |X@|;
	if constant(NEGINST) && NEGINST < 0 return BK::AlwaysGreater;
	if constant(NEGINST) goto L;
      */
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _neginst.add1(((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {
	    TermWeighting::current()->collectWeight(_neginst,(const PREFIX_SYM*)instance);
	    if (_neginst.isConstant())
	      {
		if (_neginst.freeMember() < 0) return BK::AlwaysGreater;
		command = command->label();
		goto next_command;
	      };
	  };
      };
      break;
        
    case WeightComparisonCommand::NEGINSTPlusCXToNEGINST_IfConstNEGINSTAndNEGINSTLe0RetAlwGr_IfConstNEGINSTGotoL:
      /*
	NEGINST := NEGINST + C*|X@|;
	if constant(NEGINST) && NEGINST < 0 return BK::AlwaysGreater;
	if constant(NEGINST) goto L;
      */
      {
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _neginst.add(command->coefficient(),((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {
	    TermWeighting::current()->collectWeight(_neginst,command->coefficient(),(const PREFIX_SYM*)instance);
	    if (_neginst.isConstant())
	      {
		if (_neginst.freeMember() < 0) return BK::AlwaysGreater;
		command = command->label();
		goto next_command;
	      };
	  };
      };
      break;
    

    case WeightComparisonCommand::XMinus1ToPOSINST_IfCompForGrOrEqPOSINSTvNEGINSTEqAlwGrRetAlwGr:
      /*
	POSINST := |X@| - 1;
	if compareForGreaterOrEqual(POSINST,NEGINST) == BK::AlwaysGreater return BK::AlwaysGreater;
      */
      {
	_posinst.reset();
	_posinst.add((TermWeightType)(-1));
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _posinst.add1(((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {
	    TermWeighting::current()->collectWeight(_posinst,(const PREFIX_SYM*)instance);
	  };
	
	if (_posinst.compareForGreaterOrEqual(_neginst) == BK::AlwaysGreater)
	  return BK::AlwaysGreater;
      };
      break;

    case WeightComparisonCommand::CXMinusCToPOSINST_IfCompForGrOrEqPOSINSTvNEGINSTEqAlwGrRetAlwGr:
      /*
	POSINST := C*|X@| - C;
	if compareForGreaterOrEqual(POSINST,NEGINST) == BK::AlwaysGreater return BK::AlwaysGreater;
      */
      {
	_posinst.reset();
	_posinst.add(-(command->coefficient()));
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _posinst.add(command->coefficient(),((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {
	    TermWeighting::current()->collectWeight(_posinst,command->coefficient(),(const PREFIX_SYM*)instance);
	  };
	if (_posinst.compareForGreaterOrEqual(_neginst) == BK::AlwaysGreater)
	  return BK::AlwaysGreater;
      };
      break;

    case WeightComparisonCommand::POSINSTPlusXMinus1ToPOSINST_IfCompForGrOrEqPOSINSTvNEGINSTEqAlwGrRetAlwGr:
      /*
	POSINST := POSINST + |X@| - 1;
	if compareForGreaterOrEqual(POSINST,NEGINST) == BK::AlwaysGreater return BK::AlwaysGreater;
      */
      {
	_posinst.add((TermWeightType)(-1));
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _posinst.add1(((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {
	    TermWeighting::current()->collectWeight(_posinst,(const PREFIX_SYM*)instance);
	  };
	if (_posinst.compareForGreaterOrEqual(_neginst) == BK::AlwaysGreater)
	  return BK::AlwaysGreater;
      };
      break;

    case WeightComparisonCommand::POSINSTPlusCXMinusCToPOSINST_IfCompForGrOrEqPOSINSTvNEGINSTEqAlwGrRetAlwGr:
      /*
	POSINST := POSINST + C*|X@| - C;
	if compareForGreaterOrEqual(POSINST,NEGINST) == BK::AlwaysGreater return BK::AlwaysGreater;
      */
      {
	_posinst.add(-(command->coefficient()));
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _posinst.add(command->coefficient(),((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {
	    TermWeighting::current()->collectWeight(_posinst,command->coefficient(),(const PREFIX_SYM*)instance);
	  };
	if (_posinst.compareForGreaterOrEqual(_neginst) == BK::AlwaysGreater)
	  return BK::AlwaysGreater;
      };
      break;

    case WeightComparisonCommand::POSINSTPlusXMinus1ToPOSINST_RetCompForGrOrEqPOSINSTvNEGINST:
      /*
	POSINST := POSINST + |X@| - 1;
	return compareForGreaterOrEqual(POSINST,NEGINST);
      */
      {
	_posinst.add((TermWeightType)(-1));
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _posinst.add1(((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {
	    TermWeighting::current()->collectWeight(_posinst,(const PREFIX_SYM*)instance);
	  };
	return _posinst.compareForGreaterOrEqual(_neginst);
      };

    case WeightComparisonCommand::POSINSTPlusCXMinusCToPOSINST_RetCompForGrOrEqPOSINSTvNEGINST:
      /*
	POSINST := POSINST + C*|X@| - C;
	return compareForGreaterOrEqual(POSINST,NEGINST);
      */
      {
	_posinst.add(-(command->coefficient()));
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _posinst.add(command->coefficient(),((Unifier::Variable*)instance)->absoluteVarNum());
	  }
	else
	  {
	    TermWeighting::current()->collectWeight(_posinst,command->coefficient(),(const PREFIX_SYM*)instance);
	  };
	return _posinst.compareForGreaterOrEqual(_neginst);
      };


    case WeightComparisonCommand::NEGINSTPlus1MinusXToACC_IfACCLe0RetAlwGr_GrndToGRND:
      /*
	ASSERT(constant(NEGINST));
	ASSERT(NEGINST >= 0);
	ACC := NEGINST + 1 - min|X@|;
	if ACC < 0 return BK::AlwaysGreater;
	GRND := ground(X@);
      */  
      {
	ASSERT(_neginst.isConstant());
	ASSERT(_neginst.freeMember() >= (TermWeightType)0);
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _acc = _neginst.freeMember();
	    _grnd = false;
	  }
	else
	  {
	    _acc = _neginst.freeMember() + 1;
	    bool ground;
	    TermWeightType w = 
	      TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual((const PREFIX_SYM*)instance,
										  _acc,
										  ground);

	    if (w < (TermWeightType)0) return BK::AlwaysGreater;
	    _acc -= w;
	    _grnd = ground;
	  };

      };
      break;

    case WeightComparisonCommand::NEGINSTPlusCMinusCXToACC_IfACCLe0RetAlwGr_GrndToGRND:
      /*
	ASSERT(constant(NEGINST));
	ASSERT(NEGINST >= 0);
	ACC := NEGINST + C - C*min|X@|;
	if ACC < 0 return BK::AlwaysGreater;
	GRND := ground(X@);
      */  
      {
	ASSERT(_neginst.isConstant());
	ASSERT(_neginst.freeMember() >= (TermWeightType)0);
	bool freeVar;
	void* instance = ((Unifier::Variable*)(command->nativeVar()))->Unref(freeVar);
	if (freeVar)
	  {
	    _acc = _neginst.freeMember();
	    _grnd = false;
	  }
	else
	  {
	    _acc = _neginst.freeMember() + command->coefficient();
	    bool ground;
	    TermWeightType w = 
	      TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual(command->coefficient(),
										  (const PREFIX_SYM*)instance,
										  _acc,
										  ground);
	    if (w < (TermWeightType)0) return BK::AlwaysGreater;
	    _acc -= w;
	    _grnd = ground;
	  };

      };
      break;


#ifdef DEBUG_NAMESPACE
    default: ICP("Bad instruction tag");
#endif
    };
  ASSERT(!command->isTerminal());
  ++command;
  goto next_command;

  ICP("END");
}; // FunctionComparison StandardKBOrdering::CompiledConstraintForUnifiers::interpretWeightComparisonCode()



//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_STANDARD_KB_ORDERING
#define DEBUG_NAMESPACE "StandardKBOrdering::CompiledConstraintForUnifiers::Command"
#endif
#include "debugMacros.h"
//===================================================================



inline
bool 
StandardKBOrdering::CompiledConstraintForUnifiers::Command::isTerminal() const
{
  CALL("isTerminal() const");
  switch (tag())
    {
    case RetSuccess:
    case RetFailure:
    case IfLexSTEqIncRetFail_IfLexSTEqLeRetFail_RetSucc:
      return true;
    default: return false;
    };
}; // bool StandardKBOrdering::CompiledConstraintForUnifiers::Command::isTerminal() const


#ifndef NO_DEBUG_VIS

ostream& StandardKBOrdering::CompiledConstraintForUnifiers::Command::output(ostream& str) const
{
  switch (tag())
    {
    case RetSuccess: return str << "RetSuccess;\n";
    case RetFailure: return str << "RetFailure;\n";

    case IfWCFailRetFail_IfWCAlwGrRetSucc:
      str << "IfWCFailRetFail_IfWCAlwGrRetSucc\n"
	  << " {\n"
	  << "   if WC(@) = FailureToCompare return Failure;\n"
	  << "   if WC(@) = AlwaysGreater return Success;\n"
          << "   ASSERT(WC(@) = AlwaysEqual || WC(@) = CanBeGreaterOrEqual);\n"
	  << " }\n";
      return str;

    case IfLexSTEqIncRetFail_IfLexSTEqLeRetFail_RetSucc:
      str << "IfLexSTEqIncRetFail_IfLexSTEqLeRetFail_RetSucc\n";
      PREFIX::outputTerm(str << " S = ",complexTerm1()) << "\n";   
      PREFIX::outputTerm(str << " T = ",complexTerm2()) << "\n";   
      str << " {\n"
	  << "   ASSERT(top(S) = top(T));\n"
          << "   if lex(S@,T@) = Incomparable return Failure;\n"
	  << "   if lex(S@,T@) = Less return Failure;\n"
          << "   return Success;\n"
	  << " }\n";
      return str;
		
    default: return str << "????????????;\n";
    };
  return str;
}; // ostream& StandardKBOrdering::CompiledConstraintForUnifiers::Command::output(ostream& str) const
#endif





//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_STANDARD_KB_ORDERING
#define DEBUG_NAMESPACE "StandardKBOrdering::CompiledConstraintForInstanceRetrieval"
#endif
#include "debugMacros.h"
//===================================================================

StandardKBOrdering::CompiledConstraintForInstanceRetrieval::~CompiledConstraintForInstanceRetrieval()
{
};


void 
StandardKBOrdering::CompiledConstraintForInstanceRetrieval::loadGreater(const TERM* lhs,const TERM* rhs)
{
  CALL("loadGreater(const TERM* lhs,const TERM* rhs)");

  //DF; cout << "LOAD " << lhs << " > " << rhs << "\n";

  ASSERT(lhs->isComplex());
  ASSERT(rhs->isComplex());
  _targetRelation = Greater;
  _lhs = lhs;
  _rhs = rhs;
  _lhsWeight.reset();
  _rhsWeight.reset();

  TermWeighting::current()->collectWeight(_lhsWeight,TERM((TERM*)_lhs));
  TermWeighting::current()->collectWeight(_rhsWeight,TERM((TERM*)_rhs));
  _weightComparison.compare(_lhsWeight,_rhsWeight);
  switch (_weightComparison.category())
    {
    case BK::FailureToCompare: 
      setCategory(SimplificationOrdering::CompiledConstraintForInstanceRetrieval::FailureToCompare); 
      break;
    case BK::AlwaysLess:
      setCategory(SimplificationOrdering::CompiledConstraintForInstanceRetrieval::Contradiction); 
      break;
    case BK::AlwaysEqual:
      setCategory(SimplificationOrdering::CompiledConstraintForInstanceRetrieval::Volatile); 
      break;
    case BK::AlwaysGreater:
      setCategory(SimplificationOrdering::CompiledConstraintForInstanceRetrieval::Tautology); 
      break;
    case BK::CanBeLessOrEqual:
      setCategory(SimplificationOrdering::CompiledConstraintForInstanceRetrieval::NearlyContradiction); 
      break;
    case BK::CanBeGreaterOrEqual:
      setCategory(SimplificationOrdering::CompiledConstraintForInstanceRetrieval::NearlyTautology); 
      break;
    case BK::VolatileButNearlyAlwaysLess:
      setCategory(SimplificationOrdering::CompiledConstraintForInstanceRetrieval::NearlyContradiction); 
      break;
    case BK::VolatileButNearlyAlwaysGreater:
      setCategory(SimplificationOrdering::CompiledConstraintForInstanceRetrieval::NearlyTautology); 
      break;
    case BK::AbsolutelyVolatile:
      setCategory(SimplificationOrdering::CompiledConstraintForInstanceRetrieval::Volatile); 
      break;
    };
}; // void StandardKBOrdering::CompiledConstraintForInstanceRetrieval::loadGreater(const TERM* lhs,const TERM* rhs)

bool StandardKBOrdering::CompiledConstraintForInstanceRetrieval::lhsVarInstMayOrder()
{
  CALL("lhsVarInstMayOrder()");
  return !_rhsWeight.containsVariableWhichIsNotIn(_lhsWeight);
}; // bool StandardKBOrdering::CompiledConstraintForInstanceRetrieval::lhsVarInstMayOrder()

 
bool StandardKBOrdering::CompiledConstraintForInstanceRetrieval::rhsVarInstMayOrder()
{
  CALL("rhsVarInstMayOrder()");
  return !_lhsWeight.containsVariableWhichIsNotIn(_rhsWeight);
}; // bool StandardKBOrdering::CompiledConstraintForInstanceRetrieval::rhsVarInstMayOrder()

void StandardKBOrdering::CompiledConstraintForInstanceRetrieval::compile(const InstanceRetrievalForDemodulation::Substitution* subst)
{
  CALL("compile(const InstanceRetrievalForDemodulation::Substitution* subst)");
  ASSERT(_targetRelation == Greater)
    _subst = subst;
  Command* command = _mainCode.memory();
  bool compileWeightComparison = false;
  switch (_weightComparison.category())
    {
    case BK::FailureToCompare:
      command->setTag(Command::RetFailure);
      return;
    case BK::AlwaysLess:
      command->setTag(Command::RetFailure);
      return;
    case BK::AlwaysEqual:
      break;
    case BK::AlwaysGreater:
      command->setTag(Command::RetSuccess);
      return;
    case BK::CanBeLessOrEqual:
      compileWeightComparison = true;
      break;
    case BK::CanBeGreaterOrEqual:
      compileWeightComparison = true;
      break;
    case BK::VolatileButNearlyAlwaysLess:
      compileWeightComparison = true;
      break;
    case BK::VolatileButNearlyAlwaysGreater:
      compileWeightComparison = true;
      break;
    case BK::AbsolutelyVolatile:
      compileWeightComparison = true;
      break;
    };

  if (compileWeightComparison)
    {

      if (!_weightComparisonCode.size())
	{
	  DOP(_weightComparisonCode.unfreeze());
	  _weightComparisonCode.expand();
	  DOP(_weightComparisonCode.freeze());
	};
      WeightComparisonCommand* endOfWeightCode = 
	_weightComparisonCode.memory();
      while (!_weightComparison.compileGreaterOrEqual(endOfWeightCode,
						      _weightComparisonCode.size()))
	{
	  DOP(_weightComparisonCode.unfreeze());
	  _weightComparisonCode.expand();
	  DOP(_weightComparisonCode.freeze());
	  endOfWeightCode = _weightComparisonCode.memory();
	};      


      // naturalise the variables
      for (WeightComparisonCommand* c = _weightComparisonCode.memory();
	   c < endOfWeightCode;
	   ++c)
	if (c->requiresVar())
	  {
	    c->setNativeVar((void*)_subst->nativeVar(c->var()));
	  };

    };
  
  // main code
 
  ulong maxSize;

 compile_main:
  command = _mainCode.memory();
  maxSize = _mainCode.size();
  if (compileWeightComparison)
    {
      command->setTag(Command::IfWCFailRetFail_IfWCAlwGrRetSucc);
      ++command;
      --maxSize;
    };
   
  if (!compileLexGreater(command,maxSize))
    {
      DOP(_mainCode.unfreeze());
      _mainCode.expand();
      DOP(_mainCode.freeze());
      goto compile_main;
    };

  /***********
	      DF; cout << "CHECK: ";
	      DF; cout << _lhs << " > ";
	      DF; cout << _rhs << "\n";
	      DF; cout << "WEIGHT COMP:\n" << _weightComparison << "\n";
	      DF; output(cout << "\n\n") << "\n\n";
  ************/

}; // void StandardKBOrdering::CompiledConstraintForInstanceRetrieval::compile(const InstanceRetrievalForDemodulation::Substitution* subst)



bool StandardKBOrdering::CompiledConstraintForInstanceRetrieval::holds()
{
  CALL("holds()");

  //DF; output(cout << "\n\n\nHOLDS?\n") << "\n\n\n";   
  //DF; cout << "SUBST " << _subst << "\n"; 

  Command* command = _mainCode.memory();
  FunctionComparison weightComp;
  Comparison lexComp;

 next_command:
  switch (command->tag())
    {
    case Command::RetSuccess:
      goto return_true; 
    case Command::RetFailure:
      goto return_false;

    case Command::IfWCFailRetFail_IfWCAlwGrRetSucc:
      /*
	if WC(@) = BK::FailureToCompare return Failure;
	if WC(@) = BK::AlwaysGreater return Success;
	ASSERT(WC(@) = BK::AlwaysEqual || WC(@) = BK::CanBeGreaterOrEqual);
      */

      weightComp = interpretWeightComparisonCode();

      //DF; cout << "\n\n\nWEIGHT COMPARISON RETURNS: " << weightComp << "\n\n\n"; 

      if (weightComp == BK::FailureToCompare) goto return_false;
      if (weightComp == BK::AlwaysGreater) goto return_true;
      ASSERT((weightComp == BK::CanBeGreaterOrEqual) || (weightComp == BK::AlwaysEqual));
      ++command;
      goto next_command;
	
    case Command::IfLexSTEqIncRetFail_IfLexSTEqLeRetFail_RetSucc:
      /*
	ASSERT(top(S) = top(T));
	if lex(S@,T@) = Incomparable return Failure;
	if lex(S@,T@) = Less return Failure;
	return Success;
      */
      lexComp = _ordering->compare(command->complexTerm1(),
				   command->complexTerm2(),
				   _subst);
      if (lexComp == BK::Greater) goto return_true;
      goto return_false;
      
#ifdef DEBUG_NAMESPACE
    default: ICP("ICP10"); goto return_false;
#else
#ifdef _SUPPRESS_WARNINGS_
    default: goto return_false;
#endif
#endif
    };
  
 return_true:
  ASSERT((_targetRelation != Greater) || SimplificationOrdering::current()->greater(_lhs,_rhs,_subst));

  return true;

 return_false:
  // v Very strong assumption!  
  ASSERT((_targetRelation != Greater) || (!SimplificationOrdering::current()->greater(_lhs,_rhs,_subst)));
  return false;
}; // bool StandardKBOrdering::CompiledConstraintForInstanceRetrieval::holds()




#ifndef NO_DEBUG_VIS
ostream& StandardKBOrdering::CompiledConstraintForInstanceRetrieval::output(ostream& str) const
{
  str << "CONSTRAINT: " << _lhs;
  switch (_targetRelation)
    {
    case Greater: str << " > "; break;
    default: str << "???";
    };
  str << _rhs << "\n";
  cout << "WEIGHT COMPARISON:\n" << _weightComparison << "\n";
  str << "********* MAIN CODE: *********************\n";
  const Command* command = _mainCode.memory();
  bool printWC = false;
 print_command:
  command->output(str);
  printWC = printWC || command->requiresWeightComparison();
  if (!command->isTerminal())
    {
      ++command;
      goto print_command;
    };

  if (printWC)
    {
      str << "********* WEIGHT COMPARISON CODE: ********\n";
      WeightPolynomial::SpecialisedComparison::Command::outputCode(str,_weightComparisonCode.memory()) 
	<< "\n";
    };
  str << "*********** END OF CODE ********\n";
  return str;
};
#endif



bool StandardKBOrdering::CompiledConstraintForInstanceRetrieval::compileLexGreater(Command*& code,ulong maxCodeSize)
{
  CALL("compileLexGreater(Command*& code,ulong maxCodeSize)");
  ASSERT(maxCodeSize);
  ASSERT(_lhs->isComplex());
  ASSERT(_rhs->isComplex());
  if ((*_lhs) == (*_rhs))
    {
      code->setTag(Command::IfLexSTEqIncRetFail_IfLexSTEqLeRetFail_RetSucc);
      code->setComplexTerm1(_lhs);
      code->setComplexTerm2(_rhs);
    }
  else
    if (_lhs->hasGreaterPrecedenceThan(*_rhs))
      {
	code->setTag(Command::RetSuccess);
      }
    else
      code->setTag(Command::RetFailure);
  ++code; 
  return true;

}; // bool StandardKBOrdering::CompiledConstraintForInstanceRetrieval::compileLexGreater(Command*& code,ulong maxCodeSize)



inline
FunctionComparison 
StandardKBOrdering::CompiledConstraintForInstanceRetrieval::interpretWeightComparisonCode()
{
  CALL("interpretWeightComparisonCode()");
  const WeightComparisonCommand* command = _weightComparisonCode.memory();

 next_command:

  //DF; command->outputShort(cout << "DO ") << "\n"; 

  switch (command->tag())
    {
      
    case WeightComparisonCommand::RetFailureToCompare: return BK::FailureToCompare;
    case WeightComparisonCommand::RetAlwaysLess: ICP("ICP10"); return BK::FailureToCompare;
    case WeightComparisonCommand::RetAlwaysEqual: return BK::AlwaysEqual;
    case WeightComparisonCommand::RetAlwaysGreater: return BK::AlwaysGreater;
    case WeightComparisonCommand::RetCanBeLessOrEqual: ICP("ICP20"); return BK::FailureToCompare;
    case WeightComparisonCommand::RetCanBeGreaterOrEqual: return BK::CanBeGreaterOrEqual;
    case WeightComparisonCommand::RetVolatileButNearlyAlwaysLess: ICP("ICP30");  return BK::FailureToCompare;
    case WeightComparisonCommand::RetVolatileButNearlyAlwaysGreater: ICP("ICP40"); return BK::FailureToCompare;
    case WeightComparisonCommand::RetAbsolutelyVolatile: ICP("ICP50"); return BK::FailureToCompare;

      //----------------------------------------

    case WeightComparisonCommand::IfXGr1RetFail_IfGrndRetAlwEq_RetFail:
      /*
	if min|X@| > 1 return BK::FailureToCompare;
	if ground(X@) return BK::AlwaysEqual;
	return BK::FailureToCompare;
      */
      {
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  return BK::FailureToCompare; // nonground
	
	
	ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	TermWeightType w = 
	  TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(((const TERM*)(command->nativeVar()))->First(),
							     
									(TermWeightType)1);
	if (w <= 0) return BK::FailureToCompare;
	return BK::AlwaysEqual;
      };

    case WeightComparisonCommand::IfXGr1RetFail_IfNotGrndRetFail:
      /*
	if min|X@| > 1 return BK::FailureToCompare;
	if !ground(X@) return BK::FailureToCompare;
      */
      {
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  return BK::FailureToCompare; // nonground
	ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	TermWeightType w = 
	  TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(((const TERM*)(command->nativeVar()))->First(),
							     
									(TermWeightType)1);
	if (w <= 0) return BK::FailureToCompare;
      };
      break;


      //----------------------------------------



    case WeightComparisonCommand::IfXGr1RetAlwGr_IfGrndRetAlwEq_RetCanBeGrOrEq:
      /*
	if min|X@| > 1 return BK::AlwaysGreater;
	if ground(X@) return BK::AlwaysEqual;
	return BK::CanBeGreaterOrEqual;
      */
      {
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  return BK::CanBeGreaterOrEqual;
	
	bool ground;
	ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	TermWeightType w = 
	  TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual(((const TERM*)(command->nativeVar()))->First(),
								   
									      (TermWeightType)1,
									      ground);
	if (w < (TermWeightType)0) return BK::AlwaysGreater;
	ASSERT(w == (TermWeightType)1);
	if (ground) return BK::AlwaysEqual;
	return BK::CanBeGreaterOrEqual;
      };

    case WeightComparisonCommand::IfXGr1RetAlwGr_GrndToGRND:
      /*
	if min|X@| > 1 return BK::AlwaysGreater;
	GRND := ground(X@);
      */
      {
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _grnd = false;
	  }
	else
	  {
	    bool ground;
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    TermWeightType w = 
	      TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual(((const TERM*)(command->nativeVar()))->First(),
								       
										  (TermWeightType)1,
										  ground);
	    if (w < (TermWeightType)0) return BK::AlwaysGreater;
	    _grnd = ground;
	  };
      };
      break;
      

    case WeightComparisonCommand::IfXGr1RetAlwGr_UpdateGRND:
      /*
	if min|X@| > 1 return BK::AlwaysGreater;
	GRND := GRND && ground(X@);
      */
      {
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _grnd = false;
	  }
	else
	  {
	    bool ground;
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    TermWeightType w = 
	      TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual(((const TERM*)(command->nativeVar()))->First(),
								       
										  (TermWeightType)1,
										  ground);
	    if (w < (TermWeightType)0) return BK::AlwaysGreater;
	    _grnd = _grnd && ground;
	  };
      };
      break;

    case WeightComparisonCommand::IfXGr1RetAlwGr_UpdateGRND_IfGRNDRetAlwEq_RetCanBeGrOrEq:
      /*
	if min|X@| > 1 return BK::AlwaysGreater;
	GRND := GRND && ground(X@);
	if GRND return BK::AlwaysEqual;
	return BK::CanBeGreaterOrEqual;
      */
      {

	if (((const TERM*)(command->nativeVar()))->isVariable())
	  return BK::CanBeGreaterOrEqual;

	bool ground;
	ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	TermWeightType w = 
	  TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual(((const TERM*)(command->nativeVar()))->First(),
								   
									      (TermWeightType)1,
									      ground);
	if (w < (TermWeightType)0) return BK::AlwaysGreater;
	_grnd = _grnd && ground;
	if (_grnd) return BK::AlwaysEqual;
	return BK::CanBeGreaterOrEqual;
      };



      //----------------------------------------


    case WeightComparisonCommand::IfNotGrndRetFail_IfXGrWRetFail_IfXLeWRetAlwGr_RetAlwEq:
      /*
	if !ground(X@) return BK::FailureToCompare;
	if min|X@| > W return BK::FailureToCompare;
	if min|X@| < W return BK::AlwaysGreater;
	return BK::AlwaysEqual;
      */
      {
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  return BK::FailureToCompare;

	ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	TermWeightType w = 
	  TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(((const TERM*)(command->nativeVar()))->First(),
							     
									command->weight());
	if (w <= (TermWeightType)0) return BK::FailureToCompare;
	if (w < command->weight()) 
	  {
	    return BK::AlwaysGreater;
	  }
	else
	  {
	    ASSERT(w == command->weight());
	    return BK::AlwaysEqual;
	  };
      };


    case WeightComparisonCommand::IfNotGrndRetFail_IfCXGrWRetFail_IfCXLeWRetAlwGr_RetAlwEq:
      /*
	if !ground(X@) return BK::FailureToCompare;
	if C*min|X@| > W return BK::FailureToCompare;
	if C*min|X@| < W return BK::AlwaysGreater;
	return BK::AlwaysEqual;
      */
      {
	
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  return BK::FailureToCompare;

	ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	TermWeightType w = 
	  TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(command->coefficient(),
									((const TERM*)(command->nativeVar()))->First(),
							     
									command->weight());
	if (w <= (TermWeightType)0) return BK::FailureToCompare;
	if (w < command->weight()) 
	  {
	    return BK::AlwaysGreater;
	  }
	else
	  {
	    ASSERT(w == command->weight());
	    return BK::AlwaysEqual;
	  };
      };



    case WeightComparisonCommand::IfNotGrndRetFail_WMinusXToACC_IfACCLe0RetFail:
      /*
	if !ground(X@) return BK::FailureToCompare;
	ACC := W - min|X@|;
	if ACC < 0 return BK::FailureToCompare;      
      */
      {
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  return BK::FailureToCompare;

	ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	TermWeightType w = 
	  TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(((const TERM*)(command->nativeVar()))->First(),
							     
									command->weight());
	if (w <= (TermWeightType)0) return BK::FailureToCompare;
	_acc = command->weight() - w;
	ASSERT(_acc >= (TermWeightType)0);
      };
      break;



    case WeightComparisonCommand::IfNotGrndRetFail_WMinusCXToACC_IfACCLe0RetFail:
      /*
	if !ground(X@) return BK::FailureToCompare;
	ACC := W - C*min|X@|;
	if ACC < 0 return BK::FailureToCompare;      
      */
      {
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  return BK::FailureToCompare;

	ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	TermWeightType w = 
	  TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(command->coefficient(),
									((const TERM*)(command->nativeVar()))->First(),
							     
									command->weight());
	if (w <= (TermWeightType)0) return BK::FailureToCompare;
	_acc = command->weight() - w;
	ASSERT(_acc >= (TermWeightType)0);
      };
      break;


    case WeightComparisonCommand::IfNotGrndRetFail_ACCMinusXPlus1ToACC_IfACCLe0RetFail:
      /*
	if !ground(X@) return BK::FailureToCompare;
	ACC := ACC - min|X@| + 1;
	if ACC < 0 return BK::FailureToCompare;
      */    
      {
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  return BK::FailureToCompare;

	++_acc;
	ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	TermWeightType w = 
	  TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(((const TERM*)(command->nativeVar()))->First(),
							     
									_acc);
	if (w <= (TermWeightType)0) return BK::FailureToCompare;
	_acc -= w;
	ASSERT(_acc >= (TermWeightType)0);
      };
      break;
    

    case WeightComparisonCommand::IfNotGrndRetFail_ACCMinusCXPlusCToACC_IfACCLe0RetFail:
      /*
	if !ground(X@) return BK::FailureToCompare;
	ACC := ACC - C*min|X@| + C;
	if ACC < 0 return BK::FailureToCompare; 
      */    
      {
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  return BK::FailureToCompare;

	_acc += command->coefficient();
	ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	TermWeightType w = TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(command->coefficient(),
											 ((const TERM*)(command->nativeVar()))->First(),
								    
											 _acc);
	if (w <= (TermWeightType)0) return BK::FailureToCompare;
	_acc -= w;
	ASSERT(_acc >= (TermWeightType)0);
      };
      break;
      


    case WeightComparisonCommand::IfNotGrndRetFail_ACCMinusXPlus1ToACC_IfACCLe0RetFail_IfACCEq0RetAlwEq_RetAlwGr:
      /*
	if !ground(X@) return BK::FailureToCompare;
	ACC := ACC - min|X@| + 1;
	if ACC < 0 return BK::FailureToCompare; 
	if ACC = 0 return BK::AlwaysEqual;
	return BK::AlwaysGreater;
      */    
      {
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  return BK::FailureToCompare;

	++_acc;
	ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	TermWeightType w = 
	  TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(((const TERM*)(command->nativeVar()))->First(),
							     
									_acc);
	if (w <= (TermWeightType)0) return BK::FailureToCompare;
	if (_acc == w) return BK::AlwaysEqual;
	return BK::AlwaysGreater;
      };


    case WeightComparisonCommand::IfNotGrndRetFail_ACCMinusCXPlusCToACC_IfACCLe0RetFail_IfACCEq0RetAlwEq_RetAlwGr:
      /*
	if !ground(X@) return BK::FailureToCompare;
	ACC := ACC - C*min|X@| + C;
	if ACC < 0 return BK::FailureToCompare; 
	if ACC = 0 return BK::AlwaysEqual;
	return BK::AlwaysGreater;
      */    
      {
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  return BK::FailureToCompare;
	ASSERT(((const TERM*)(command->nativeVar()))->IsReference());


	_acc += command->coefficient(); 
	TermWeightType w = 
	  TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(command->coefficient(),
									((const TERM*)(command->nativeVar()))->First(),
							     
									_acc);
	if (w <= (TermWeightType)0) return BK::FailureToCompare;
	if (_acc == w) return BK::AlwaysEqual;
	return BK::AlwaysGreater;
      };


      //----------------------------------------



    case WeightComparisonCommand::IfXGrWRetAlwGr_IfXLeWRetFail_IfGrndRetAlwEq_RetCanBeGrOrEq:
      /*
	ASSERT(W > (TermWeightType)0);
	if min|X@| > W return BK::AlwaysGreater;
	if min|X@| < W return BK::FailureToCompare;
	if ground(X@) return BK::AlwaysEqual;
	return BK::CanBeGreaterOrEqual;
      */
      {
	ASSERT(command->weight() > (TermWeightType)0);
	
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    // min|X@| == 1, ground(X@) == false
	    if ((TermWeightType)1 < command->weight()) return BK::FailureToCompare;
	    return BK::CanBeGreaterOrEqual;
	  }
	else
	  {
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    bool ground;
	    TermWeightType w = 
	      TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual(((const TERM*)(command->nativeVar()))->First(),
								       
										  command->weight(),
										  ground);

	   
	    if (w < (TermWeightType)0) return BK::AlwaysGreater;
	    ASSERT((w > (TermWeightType)0) && (w <= command->weight()));
	    if (w < command->weight()) return BK::FailureToCompare;
	    if (ground) return BK::AlwaysEqual;
	    return BK::CanBeGreaterOrEqual;
	  };
      };
      
    
    case WeightComparisonCommand::IfCXGrWRetAlwGr_IfCXLeWRetFail_IfGrndRetAlwEq_RetCanBeGrOrEq:
      /*
	ASSERT(W > (TermWeightType)0);
	if C*min|X@| > W return BK::AlwaysGreater;
	if C*min|X@| < W return BK::FailureToCompare;
	if ground(X@) return BK::AlwaysEqual;
	return BK::CanBeGreaterOrEqual;
      */
      {
	ASSERT(command->weight() > (TermWeightType)0);

	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    // min|X@| == 1, ground(X@) == false
	    if (command->coefficient() < command->weight()) return BK::FailureToCompare;
	    return BK::CanBeGreaterOrEqual;
	  }
	else
	  {
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    bool ground;
	    TermWeightType w = 
	      TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual(command->coefficient(),
										  ((const TERM*)(command->nativeVar()))->First(),
								       
										  command->weight(),
										  ground);
	    if (w < (TermWeightType)0) return BK::AlwaysGreater;
	    ASSERT((w > (TermWeightType)0) && (w <= command->weight()));
	    if (w < command->weight()) return BK::FailureToCompare;
	    if (ground) return BK::AlwaysEqual;
	    return BK::CanBeGreaterOrEqual;
	  };
      };


    case WeightComparisonCommand::WMinusXToACC_IfACCLe0RetAlwGr_GrndToGRND:
      /*
	ACC := W - min|X@|;
	if ACC < 0 return BK::AlwaysGreater;
	GRND := ground(X@);
      */
      {
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _acc = command->weight() - (TermWeightType)1;
	    if (_acc < (TermWeightType)0) return BK::AlwaysGreater;
	    _grnd = false;
	  }
	else
	  {
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    bool ground;
	    TermWeightType w = 
	      TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual(((const TERM*)(command->nativeVar()))->First(),
								       
										  command->weight(),
										  ground);
	    if (w < (TermWeightType)0) return BK::AlwaysGreater;
	    _acc = command->weight() - w;
	    _grnd = ground;
	  };
	
      };
      break;

    case WeightComparisonCommand::WMinusCXToACC_IfACCLe0RetAlwGr_GrndToGRND:
      /*
	ACC := W - C*min|X@|;
	if ACC <= 0 return BK::AlwaysGreater;
	GRND := ground(X@);
      */
      {
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _acc = command->weight() - command->coefficient();
	    if (_acc < (TermWeightType)0) return BK::AlwaysGreater;
	    _grnd = false;
	  }
	else
	  {
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    bool ground;
	    TermWeightType w = 
	      TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual(command->coefficient(),
										  ((const TERM*)(command->nativeVar()))->First(),
								       
										  command->weight(),
										  ground);
	    if (w < (TermWeightType)0) return BK::AlwaysGreater;
	    _acc = command->weight() - w;
	    _grnd = ground;
	  };
	
      };
      break;
    
    case WeightComparisonCommand::ACCMinusXPlus1ToACC_IfACCLe0RetAlwGr_UpdateGRND:
      /*
	ACC := ACC - min|X@| + 1;
	if ACC < 0 return BK::AlwaysGreater;
	GRND := GRND && ground(X@);
      */    
      {
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    if (_acc < (TermWeightType)0) return BK::AlwaysGreater;
	    _grnd = false;
	  }
	else
	  {
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    ++_acc;
	    bool ground;
	    TermWeightType w = 
	      TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual(((const TERM*)(command->nativeVar()))->First(),
								       
										  _acc,
										  ground);
	    if (w < (TermWeightType)0) return BK::AlwaysGreater;
	    _acc -= w;
	    ASSERT(_acc >= (TermWeightType)0);
	    _grnd = _grnd && ground;
	  };
      };
      break;

    
    case WeightComparisonCommand::ACCMinusCXPlusCToACC_IfACCLe0RetAlwGr_UpdateGRND:
      /*
	ACC := ACC - C*min|X@| + C;
	if ACC < 0 return BK::AlwaysGreater;
	GRND := GRND && ground(X@);
      */    
      {
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    if (_acc < (TermWeightType)0) return BK::AlwaysGreater;
	    _grnd = false;
	  }
	else
	  {
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    _acc += command->coefficient();
	    bool ground;
	    TermWeightType w = 
	      TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual(command->coefficient(),
										  ((const TERM*)(command->nativeVar()))->First(),
								       
										  _acc,
										  ground);
	    if (w < (TermWeightType)0) return BK::AlwaysGreater;
	    _acc -= w;
	    ASSERT(_acc >= (TermWeightType)0);
	    _grnd = _grnd && ground;
	  };
      };
      break;


    case WeightComparisonCommand::IfXGrACCPlus1RetAlwGr_IfXLeACCPlus1RetFail_UpdateGRND_IfGRNDRetAlwEq_RetCanBeGrOrEq:
      /*
	if min|X@| > ACC + 1 return BK::AlwaysGreater;
	if min|X@| < ACC + 1 return BK::FailureToCompare;      
	GRND := GRND && ground(X@);
	if GRND return BK::AlwaysEqual;
	return BK::CanBeGreaterOrEqual;
      */   
      {
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    if (_acc < (TermWeightType)0) return BK::AlwaysGreater;
	    if (_acc > (TermWeightType)0) return BK::FailureToCompare;  
	    return BK::CanBeGreaterOrEqual;
	  }
	else
	  {
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    ++_acc;
	    bool ground;
	    TermWeightType w = 
	      TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual(((const TERM*)(command->nativeVar()))->First(),
								       
										  _acc,
										  ground);
	    if (w < (TermWeightType)0) return BK::AlwaysGreater;
	    if (w < _acc) return BK::FailureToCompare; 
	    ASSERT(w == _acc);
	    if (_grnd && ground) return BK::AlwaysEqual;
	    return BK::CanBeGreaterOrEqual;
	  };
      };
    
    case WeightComparisonCommand::IfCXGrACCPlusCRetAlwGr_IfCXLeACCPlusCRetFail_UpdateGRND_IfGRNDRetAlwEq_RetCanBeGrOrEq:
      /*
	if C*min|X@| > ACC + C return BK::AlwaysGreater;
	if C*min|X@| < ACC + C return BK::FailureToCompare;      
	GRND := GRND && ground(X@);
	if GRND return BK::AlwaysEqual;
	return BK::CanBeGreaterOrEqual;
      */   
      {
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    if (_acc < (TermWeightType)0) return BK::AlwaysGreater;
	    if (_acc > (TermWeightType)0) return BK::FailureToCompare;  
	    return BK::CanBeGreaterOrEqual;
	  }
	else
	  {
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    _acc += command->coefficient();
	    bool ground;
	    TermWeightType w = 
	      TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual(command->coefficient(),
										  ((const TERM*)(command->nativeVar()))->First(),
								       
										  _acc,
										  ground);
	    if (w < (TermWeightType)0) return BK::AlwaysGreater;
	    if (w < _acc) return BK::FailureToCompare; 
	    ASSERT(w == _acc);
	    if (_grnd && ground) return BK::AlwaysEqual;
	    return BK::CanBeGreaterOrEqual;
	  };
      };
    
      //----------------------------------------

    case WeightComparisonCommand::XPlusWToPOSINST_IfConstPOSINSAndPOSINSTLe0RetFail_IfConstPOSINSTGotoL:
      /*
	POSINST := |X@| + W;
	if constant(POSINST) && POSINST < 0 return BK::FailureToCompare; 
	if constant(POSINST) goto L; 
      */
      {
	_posinst.reset();
	_posinst.add(command->weight());

	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _posinst.add1(((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());	    
	    TermWeighting::current()->collectWeight(_posinst,*((const TERM*)(command->nativeVar())));
	    if (_posinst.isConstant())
	      {
		if (_posinst.freeMember() < (TermWeightType)0) return BK::FailureToCompare;  
		command = command->label();
		goto next_command;
	      };
	  };
      };
      break;


    case WeightComparisonCommand::CXPlusWToPOSINST_IfConstPOSINSAndPOSINSTLe0RetFail_IfConstPOSINSTGotoL:
      /*
	POSINST := C*|X@| + W;
	if constant(POSINST) && POSINST < 0 return BK::FailureToCompare; 
	if constant(POSINST) goto L;
      */
      {
	_posinst.reset();
	_posinst.add(command->weight());

	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _posinst.add(command->coefficient(),((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {	
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());	    
	    TermWeighting::current()->collectWeight(_posinst,
						    command->coefficient(),
						    *((const TERM*)(command->nativeVar())));
	    if (_posinst.isConstant())
	      {
		if (_posinst.freeMember() < (TermWeightType)0) return BK::FailureToCompare;  
		command = command->label();
		goto next_command;
	      };
	  };
      };
      break;


    case WeightComparisonCommand::XPlusWToPOSINST:
      /*
	POSINST := |X@| + W;
      */
      {
	_posinst.reset();
	_posinst.add(command->weight());

	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _posinst.add1(((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {		
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());	    
	    TermWeighting::current()->collectWeight(_posinst,
						    *((const TERM*)(command->nativeVar())));
	  };
      };
      break;
    
    case WeightComparisonCommand::CXPlusWToPOSINST:
      /*
	POSINST := C*|X@| + W;
      */
      {
	_posinst.reset();
	_posinst.add(command->weight());

	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _posinst.add(command->coefficient(),((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {			
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());	    
	    TermWeighting::current()->collectWeight(_posinst,
						    command->coefficient(),
						    *((const TERM*)(command->nativeVar())));
	  };
      };
      break;

    case WeightComparisonCommand::POSINSTPlusXToPOSINST:
      /*
	POSINST := POSINST + |X@|;
      */
      {
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _posinst.add1(((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {				
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());	    
	    TermWeighting::current()->collectWeight(_posinst,
						    *((const TERM*)(command->nativeVar())));
	  };
      };
      break;
    
    case WeightComparisonCommand::POSINSTPlusCXToPOSINST:
      /*
	POSINST := POSINST + C*|X@|;
      */
      {
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _posinst.add(command->coefficient(),((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {					
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());	    
	    TermWeighting::current()->collectWeight(_posinst,
						    command->coefficient(),
						    *((const TERM*)(command->nativeVar())));
	  };
      };
      break;

    
    case WeightComparisonCommand::POSINSTPlusXToPOSINST_IfConstPOSINSTAndPOSINSTLe0RetFail_IfConstPOSINSTGotoL:
      /*
	POSINST := POSINST + |X@|;
	if constant(POSINST) && POSINST < 0 return BK::FailureToCompare; 
	if constant(POSINST) goto L;
      */
      {
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _posinst.add1(((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {						
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());    
	    TermWeighting::current()->collectWeight(_posinst,
						    *((const TERM*)(command->nativeVar())));
	  };
	if (_posinst.isConstant())
	  {
	    if (_posinst.freeMember() < (TermWeightType)0) return BK::FailureToCompare; 
	    command = command->label();
	    goto next_command;
	  };
      };
      break;

    case WeightComparisonCommand::POSINSTPlusCXToPOSINST_IfConstPOSINSTAndPOSINSTLe0RetFail_IfConstPOSINSTGotoL:
      /*
	POSINST := POSINST + C*|X@|;
	if constant(POSINST) && POSINST < 0 return BK::FailureToCompare; 
	if constant(POSINST) goto L;
      */
      {
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _posinst.add(command->coefficient(),((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {							
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());      
	    TermWeighting::current()->collectWeight(_posinst,
						    command->coefficient(),
						    *((const TERM*)(command->nativeVar())));
	  };
	if (_posinst.isConstant())
	  {
	    if (_posinst.freeMember() < (TermWeightType)0) return BK::FailureToCompare; 
	    command = command->label();
	    goto next_command;
	  };
      };
      break;

    case WeightComparisonCommand::XMinus1ToNEGINSTRetCompForGrOrEqPOSINSTvNEGINST:
      /*
	NEGINST := |X@| - 1;
	return compareForGreaterOrEqual(POSINST,NEGINST);
      */
      {
	_neginst.reset();
	_neginst.add((TermWeightType)(-1));

	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _neginst.add1(((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {							
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference()); 
	    TermWeighting::current()->collectWeight(_neginst,
						    *((const TERM*)(command->nativeVar())));
	  };
	return _posinst.compareForGreaterOrEqual(_neginst);
      };
    
    case WeightComparisonCommand::CXMinusCToNEGINSTRetCompForGrOrEqPOSINSTvNEGINST:
      /*
	NEGINST := C*|X@| - C;
	return compareForGreaterOrEqual(POSINST,NEGINST);
      */
      {
	_neginst.reset();
	_neginst.add(-(command->coefficient()));

	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _neginst.add(command->coefficient(),((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {							
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference()); 
	    TermWeighting::current()->collectWeight(_neginst,
						    command->coefficient(),
						    *((const TERM*)(command->nativeVar())));
	  };
	return _posinst.compareForGreaterOrEqual(_neginst);
      };


    case WeightComparisonCommand::XMinus1ToNEGINST_IfCompForGrOrEqPOSINSTvNEGINSTEqFailRetFail:
      /*
	NEGINST := |X@| - 1;
	if compareForGreaterOrEqual(POSINST,NEGINST) == BK::FailureToCompare return BK::FailureToCompare;
      */
      {
	_neginst.reset();
	_neginst.add((TermWeightType)(-1));

	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _neginst.add1(((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {							
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference()); 
	    TermWeighting::current()->collectWeight(_neginst,*((const TERM*)(command->nativeVar())));
	  };
	if (_posinst.compareForGreaterOrEqual(_neginst) == BK::FailureToCompare) 
	  return BK::FailureToCompare;
      };
      break;

    case WeightComparisonCommand::CXMinusCToNEGINST_IfCompForGrOrEqPOSINSTvNEGINSTEqFailRetFail:
      /*
	NEGINST := C*|X@| - C;
	if compareForGreaterOrEqual(POSINST,NEGINST) == BK::FailureToCompare return BK::FailureToCompare;
      */
      {
	_neginst.reset();
	_neginst.add(-(command->coefficient()));

	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _neginst.add(command->coefficient(),((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {							
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference()); 
	    TermWeighting::current()->collectWeight(_neginst,
						    command->coefficient(),
						    *((const TERM*)(command->nativeVar())));
	  };
	if (_posinst.compareForGreaterOrEqual(_neginst) == BK::FailureToCompare) 
	  return BK::FailureToCompare;
      };
      break;

    case WeightComparisonCommand::NEGINSTPlusXMinus1ToNEGINST_IfCompForGrOrEqPOSINSTvNEGINSTEqFailRetFail:
      /*
	NEGINST := NEGINST + |X@| - 1;
	if compareForGreaterOrEqual(POSINST,NEGINST) == BK::FailureToCompare return BK::FailureToCompare;
      */
      {
	_neginst.add((TermWeightType)(-1));

	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _neginst.add1(((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {							
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference()); 
	    TermWeighting::current()->collectWeight(_neginst,
						    *((const TERM*)(command->nativeVar())));
	  };
	if (_posinst.compareForGreaterOrEqual(_neginst) == BK::FailureToCompare) 
	  return BK::FailureToCompare;
      };
      break;

    case WeightComparisonCommand::NEGINSTPlusCXMinusCToNEGINST_IfCompForGrOrEqPOSINSTvNEGINSTEqFailRetFail:
      /*
	NEGINST := NEGINST + C*|X@| - C;
	if compareForGreaterOrEqual(POSINST,NEGINST) == BK::FailureToCompare return BK::FailureToCompare;
      */
      {
	_neginst.add(-(command->coefficient()));

	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _neginst.add(command->coefficient(),((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {							
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    TermWeighting::current()->collectWeight(_neginst,
						    command->coefficient(),
						    *((const TERM*)(command->nativeVar())));
	  };
	if (_posinst.compareForGreaterOrEqual(_neginst) == BK::FailureToCompare) 
	  return BK::FailureToCompare;
      };
      break;

    case WeightComparisonCommand::NEGINSTPlusXMinus1ToNEGINST_RetCompForGrOrEqPOSINSTvNEGINST:
      /*
	NEGINST := NEGINST + |X@| - 1;
	return compareForGreaterOrEqual(POSINST,NEGINST);
      */
      {
	_neginst.add((TermWeightType)(-1));

	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _neginst.add1(((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {							
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    TermWeighting::current()->collectWeight(_neginst,
						    *((const TERM*)(command->nativeVar())));
	  };
	return _posinst.compareForGreaterOrEqual(_neginst);
      };

    case WeightComparisonCommand::NEGINSTPlusCXMinusCToNEGINST_RetCompForGrOrEqPOSINSTvNEGINST:
      /*
	NEGINST := NEGINST + C*|X@| - C;
	return compareForGreaterOrEqual(POSINST,NEGINST);
      */
      {
	_neginst.add(-(command->coefficient()));

	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _neginst.add(command->coefficient(),((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {							
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    TermWeighting::current()->collectWeight(_neginst,
						    command->coefficient(),
						    *((const TERM*)(command->nativeVar())));
	  };
	return _posinst.compareForGreaterOrEqual(_neginst);
      };


    case WeightComparisonCommand::IfNotGrndRetFail_IfXMinus1GrPOSINSTRetFail_IfXMinus1LePOSINSTRetAlwGr_RetAlwEq:
      /*
	ASSERT(constant(POSINST));
	ASSERT(POSINST >= 0);
	if !ground(X@) return BK::FailureToCompare;
	if min|X@| - 1 > POSINST return BK::FailureToCompare;
	if min|X@| - 1 < POSINST return BK::AlwaysGreater;
	return BK::AlwaysEqual;
      */
      {
	ASSERT(_posinst.isConstant());
	ASSERT(_posinst.freeMember() >= (TermWeightType)0);

	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    return BK::FailureToCompare;
	  }
	else
	  {							
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    _posinst.add1();
	    TermWeightType w = 
	      TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(((const TERM*)(command->nativeVar()))->First(),
								 
									    _posinst.freeMember());
	    if (w <= (TermWeightType)0) return BK::FailureToCompare;
	    ASSERT(w <= _posinst.freeMember());
	    if (w < _posinst.freeMember()) return BK::AlwaysGreater;
	    return BK::AlwaysEqual;
	  };
      };

    case WeightComparisonCommand::IfNotGrndRetFail_IfCXMinusCGrPOSINSTRetFail_IfCXMinusCLePOSINSTRetAlwGr_RetAlwEq:
      /*
	ASSERT(constant(POSINST));
	ASSERT(POSINST >= 0);
	if !ground(X@) return BK::FailureToCompare;
	if C*min|X@| - C > POSINST return BK::FailureToCompare;
	if C*min|X@| - C < POSINST return BK::AlwaysGreater;
	return BK::AlwaysEqual;
      */
      {
	ASSERT(_posinst.isConstant());
	ASSERT(_posinst.freeMember() >= (TermWeightType)0);

	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    return BK::FailureToCompare;
	  }
	else
	  {
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    _posinst.add(command->coefficient());
	    TermWeightType w = 
	      TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(command->coefficient(),
									    ((const TERM*)(command->nativeVar()))->First(),
								 
									    _posinst.freeMember());
	    if (w <= (TermWeightType)0) return BK::FailureToCompare;
	    ASSERT(w <= _posinst.freeMember());
	    if (w < _posinst.freeMember()) return BK::AlwaysGreater;
	    return BK::AlwaysEqual;
	  };
      };


    case WeightComparisonCommand::IfNotGrndRetFail_POSINSTPlus1MinusXToACC_IfACCLe0RetFail:
      /*
	ASSERT(constant(POSINST));
	ASSERT(POSINST >= 0);
	if !ground(X@) return BK::FailureToCompare;
	ACC := POSINST + 1 - min|X@|;      
	if ACC < 0 return BK::FailureToCompare; 
      */
      {	
	ASSERT(_posinst.isConstant());
	ASSERT(_posinst.freeMember() >= (TermWeightType)0);

	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    return BK::FailureToCompare;
	  }
	else
	  {	 
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());   
	    _acc = _posinst.freeMember() + 1;
	    TermWeightType w = 
	      TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(((const TERM*)(command->nativeVar()))->First(),
								 
									    _acc);
	    if (w <= (TermWeightType)0) return BK::FailureToCompare;
	    _acc -= w;
	    ASSERT(_acc >= (TermWeightType)0);
	  };
      };
      break;

    case WeightComparisonCommand::IfNotGrndRetFail_POSINSTPlusCMinusCXToACC_IfACCLe0RetFail:
      /*
	ASSERT(constant(POSINST));
	ASSERT(POSINST >= 0);
	if !ground(X@) return BK::FailureToCompare;
	ACC := POSINST + C - C*min|X@|;      
	if ACC < 0 return BK::FailureToCompare; 
      */
      {	
	ASSERT(_posinst.isConstant());
	ASSERT(_posinst.freeMember() >= (TermWeightType)0);

	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    return BK::FailureToCompare;
	  }
	else
	  {		 
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());       
	    _acc = _posinst.freeMember() + command->coefficient();
	    TermWeightType w = 
	      TermWeighting::current()->computeWeightIfGroundAndLessOrEqual(command->coefficient(),
									    ((const TERM*)(command->nativeVar()))->First(),
								 
									    _acc);
	    if (w <= (TermWeightType)0) return BK::FailureToCompare;
	    _acc -= w;
	    ASSERT(_acc >= (TermWeightType)0);
	  };
      };
      break;




    case WeightComparisonCommand::XPlusWToNEGINST_IfConstNEGINSTAndNEGINSTLe0RetAlwGr_IfConstNEGINSTGotoL:
      /*
	NEGINST := |X@| + W;
	if constant(NEGINST) && NEGINST < 0 return BK::AlwaysGreater;
	if constant(NEGINST) goto L;
      */
      {
	_neginst.reset();
	_neginst.add(command->weight());

	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _neginst.add1(((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {		 
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());   
	    TermWeighting::current()->collectWeight(_neginst,
						    *((const TERM*)(command->nativeVar())));
	    if (_neginst.isConstant())
	      {
		if (_neginst.freeMember() < (TermWeightType)0) return BK::AlwaysGreater;
		command = command->label();
		goto next_command;
	      };
	  };
      };
      break;
     


    case WeightComparisonCommand::CXPlusWToNEGINST_IfConstNEGINSTAndNEGINSTLe0RetAlwGr_IfConstNEGINSTGotoL:
      /*
	NEGINST := C*|X@| + W;
	if constant(NEGINST) && NEGINST < 0 return BK::AlwaysGreater;
	if constant(NEGINST) goto L;
      */
      {
	_neginst.reset();
	_neginst.add(command->weight());

	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _neginst.add(command->coefficient(),((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {		 
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());  
	    TermWeighting::current()->collectWeight(_neginst,
						    command->coefficient(),
						    *((const TERM*)(command->nativeVar())));
	    if (_neginst.isConstant())
	      {
		if (_neginst.freeMember() < (TermWeightType)0) return BK::AlwaysGreater;
		command = command->label();
		goto next_command;
	      };
	  };
      };
      break;


    case WeightComparisonCommand::XPlusWToNEGINST:
      /*
	NEGINST := |X@| + W;
      */
      {
	_neginst.reset();
	_neginst.add(command->weight());
	
	
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _neginst.add1(((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {		 
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    TermWeighting::current()->collectWeight(_neginst,
						    *((const TERM*)(command->nativeVar())));
	  };
      };
      break;

    
    case WeightComparisonCommand::CXPlusWToNEGINST:
      /*
	NEGINST := C*|X@| + W;
      */
      {
	_neginst.reset();
	_neginst.add(command->weight());
	
	
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _neginst.add(command->coefficient(),((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {		 
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    TermWeighting::current()->collectWeight(_neginst,
						    command->coefficient(),
						    *((const TERM*)(command->nativeVar())));
	  };
      };
      break;

        
    case WeightComparisonCommand::NEGINSTPlusXToNEGINST:
      /*
	NEGINST := NEGINST + |X@|;
      */
      {
	
	
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _neginst.add1(((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {		 
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    TermWeighting::current()->collectWeight(_neginst,
						    *((const TERM*)(command->nativeVar())));
	  };
      };
      break;

    case WeightComparisonCommand::NEGINSTPlusCXToNEGINST:
      /*
	NEGINST := NEGINST + C*|X@|;
      */
      {
	
	
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _neginst.add(command->coefficient(),((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {		 
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    TermWeighting::current()->collectWeight(_neginst,
						    command->coefficient(),
						    *((const TERM*)(command->nativeVar())));
	  };
      };
      break;
    
    case WeightComparisonCommand::NEGINSTPlusXToNEGINST_IfConstNEGINSTAndNEGINSTLe0RetAlwGr_IfConstNEGINSTGotoL:
      /*
	NEGINST := NEGINST + |X@|;
	if constant(NEGINST) && NEGINST < 0 return BK::AlwaysGreater;
	if constant(NEGINST) goto L;
      */
      {
	
	
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _neginst.add1(((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {		 
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    TermWeighting::current()->collectWeight(_neginst,
						    *((const TERM*)(command->nativeVar())));
	    if (_neginst.isConstant())
	      {
		if (_neginst.freeMember() < 0) return BK::AlwaysGreater;
		command = command->label();
		goto next_command;
	      };
	  };
      };
      break;
        
    case WeightComparisonCommand::NEGINSTPlusCXToNEGINST_IfConstNEGINSTAndNEGINSTLe0RetAlwGr_IfConstNEGINSTGotoL:
      /*
	NEGINST := NEGINST + C*|X@|;
	if constant(NEGINST) && NEGINST < 0 return BK::AlwaysGreater;
	if constant(NEGINST) goto L;
      */
      {
	
	
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _neginst.add(command->coefficient(),((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {		 
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    TermWeighting::current()->collectWeight(_neginst,
						    command->coefficient(),
						    *((const TERM*)(command->nativeVar())));
	    if (_neginst.isConstant())
	      {
		if (_neginst.freeMember() < 0) return BK::AlwaysGreater;
		command = command->label();
		goto next_command;
	      };
	  };
      };
      break;
    

    case WeightComparisonCommand::XMinus1ToPOSINST_IfCompForGrOrEqPOSINSTvNEGINSTEqAlwGrRetAlwGr:
      /*
	POSINST := |X@| - 1;
	if compareForGreaterOrEqual(POSINST,NEGINST) == BK::AlwaysGreater return BK::AlwaysGreater;
      */
      {
	_posinst.reset();
	_posinst.add((TermWeightType)(-1));
	
	
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _posinst.add1(((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {		 
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    TermWeighting::current()->collectWeight(_posinst,
						    *((const TERM*)(command->nativeVar())));
	  };
	
	if (_posinst.compareForGreaterOrEqual(_neginst) == BK::AlwaysGreater)
	  return BK::AlwaysGreater;
      };
      break;

    case WeightComparisonCommand::CXMinusCToPOSINST_IfCompForGrOrEqPOSINSTvNEGINSTEqAlwGrRetAlwGr:
      /*
	POSINST := C*|X@| - C;
	if compareForGreaterOrEqual(POSINST,NEGINST) == BK::AlwaysGreater return BK::AlwaysGreater;
      */
      {
	_posinst.reset();
	_posinst.add(-(command->coefficient()));
	
	
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _posinst.add(command->coefficient(),((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {		 
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    TermWeighting::current()->collectWeight(_posinst,
						    command->coefficient(),
						    *((const TERM*)(command->nativeVar())));
	  };
	if (_posinst.compareForGreaterOrEqual(_neginst) == BK::AlwaysGreater)
	  return BK::AlwaysGreater;
      };
      break;

    case WeightComparisonCommand::POSINSTPlusXMinus1ToPOSINST_IfCompForGrOrEqPOSINSTvNEGINSTEqAlwGrRetAlwGr:
      /*
	POSINST := POSINST + |X@| - 1;
	if compareForGreaterOrEqual(POSINST,NEGINST) == BK::AlwaysGreater return BK::AlwaysGreater;
      */
      {
	_posinst.add((TermWeightType)(-1));
	
	
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _posinst.add1(((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {		 
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    TermWeighting::current()->collectWeight(_posinst,
						    *((const TERM*)(command->nativeVar())));
	  };
	if (_posinst.compareForGreaterOrEqual(_neginst) == BK::AlwaysGreater)
	  return BK::AlwaysGreater;
      };
      break;

    case WeightComparisonCommand::POSINSTPlusCXMinusCToPOSINST_IfCompForGrOrEqPOSINSTvNEGINSTEqAlwGrRetAlwGr:
      /*
	POSINST := POSINST + C*|X@| - C;
	if compareForGreaterOrEqual(POSINST,NEGINST) == BK::AlwaysGreater return BK::AlwaysGreater;
      */
      {
	_posinst.add(-(command->coefficient()));
	
	
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _posinst.add(command->coefficient(),((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {		 
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    TermWeighting::current()->collectWeight(_posinst,
						    command->coefficient(),
						    *((const TERM*)(command->nativeVar())));
	  };
	if (_posinst.compareForGreaterOrEqual(_neginst) == BK::AlwaysGreater)
	  return BK::AlwaysGreater;
      };
      break;

    case WeightComparisonCommand::POSINSTPlusXMinus1ToPOSINST_RetCompForGrOrEqPOSINSTvNEGINST:
      /*
	POSINST := POSINST + |X@| - 1;
	return compareForGreaterOrEqual(POSINST,NEGINST);
      */
      {
	_posinst.add((TermWeightType)(-1));
	
	
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _posinst.add1(((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {		 
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    TermWeighting::current()->collectWeight(_posinst,
						    *((const TERM*)(command->nativeVar())));
	  };
	return _posinst.compareForGreaterOrEqual(_neginst);
      };

    case WeightComparisonCommand::POSINSTPlusCXMinusCToPOSINST_RetCompForGrOrEqPOSINSTvNEGINST:
      /*
	POSINST := POSINST + C*|X@| - C;
	return compareForGreaterOrEqual(POSINST,NEGINST);
      */
      {
	_posinst.add(-(command->coefficient()));
	
	
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _posinst.add(command->coefficient(),((const TERM*)(command->nativeVar()))->var());
	  }
	else
	  {		 
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    TermWeighting::current()->collectWeight(_posinst,
						    command->coefficient(),
						    *((const TERM*)(command->nativeVar())));
	  };
	return _posinst.compareForGreaterOrEqual(_neginst);
      };


    case WeightComparisonCommand::NEGINSTPlus1MinusXToACC_IfACCLe0RetAlwGr_GrndToGRND:
      /*
	ASSERT(constant(NEGINST));
	ASSERT(NEGINST >= 0);
	ACC := NEGINST + 1 - min|X@|;
	if ACC < 0 return BK::AlwaysGreater;
	GRND := ground(X@);
      */  
      {
	ASSERT(_neginst.isConstant());
	ASSERT(_neginst.freeMember() >= (TermWeightType)0);
	
	
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _acc = _neginst.freeMember();
	    _grnd = false;
	  }
	else
	  {		 
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    _acc = _neginst.freeMember() + 1;
	    bool ground;
	    TermWeightType w = 
	      TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual(((const TERM*)(command->nativeVar()))->First(),
								       
										  _acc,
										  ground);
	    if (w < (TermWeightType)0) return BK::AlwaysGreater;
	    _acc -= w;
	    _grnd = ground;
	  };

      };
      break;

    case WeightComparisonCommand::NEGINSTPlusCMinusCXToACC_IfACCLe0RetAlwGr_GrndToGRND:
      /*
	ASSERT(constant(NEGINST));
	ASSERT(NEGINST >= 0);
	ACC := NEGINST + C - C*min|X@|;
	if ACC < 0 return BK::AlwaysGreater;
	GRND := ground(X@);
      */  
      {
	ASSERT(_neginst.isConstant());
	ASSERT(_neginst.freeMember() >= (TermWeightType)0);
	
       
	if (((const TERM*)(command->nativeVar()))->isVariable())
	  {
	    _acc = _neginst.freeMember();
	    _grnd = false;
	  }
	else
	  {		 
	    ASSERT(((const TERM*)(command->nativeVar()))->IsReference());
	    _acc = _neginst.freeMember() + command->coefficient();
	    bool ground;
	    TermWeightType w = 
	      TermWeighting::current()->computeMinimalInstanceWeightIfLessOrEqual(command->coefficient(),
										  ((const TERM*)(command->nativeVar()))->First(),
								       
										  _acc,
										  ground);
	    if (w < (TermWeightType)0) return BK::AlwaysGreater;
	    _acc -= w;
	    _grnd = ground;
	  };

      };
      break;


#ifdef DEBUG_NAMESPACE
    default: ICP("Bad instruction tag");
#endif
    };
  ASSERT(!command->isTerminal());
  ++command;
  goto next_command;

  ICP("END");
}; // FunctionComparison StandardKBOrdering::CompiledConstraintForInstanceRetrieval::interpretWeightComparisonCode()



//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_STANDARD_KB_ORDERING
#define DEBUG_NAMESPACE "StandardKBOrdering::CompiledConstraintForInstanceRetrieval::Command"
#endif
#include "debugMacros.h"
//===================================================================

inline
bool 
StandardKBOrdering::CompiledConstraintForInstanceRetrieval::Command::isTerminal() const
{
  CALL("isTerminal() const");
  switch (tag())
    {
    case RetSuccess:
    case RetFailure:
    case IfLexSTEqIncRetFail_IfLexSTEqLeRetFail_RetSucc:
      return true;
    default: return false;
    };
}; // bool StandardKBOrdering::CompiledConstraintForInstanceRetrieval::Command::isTerminal() const


#ifndef NO_DEBUG_VIS

ostream& StandardKBOrdering::CompiledConstraintForInstanceRetrieval::Command::output(ostream& str) const
{
  switch (tag())
    {
    case RetSuccess: return str << "RetSuccess;\n";
    case RetFailure: return str << "RetFailure;\n";

    case IfWCFailRetFail_IfWCAlwGrRetSucc:
      str << "IfWCFailRetFail_IfWCAlwGrRetSucc\n"
	  << " {\n"
	  << "   if WC(@) = BK::FailureToCompare return Failure;\n"
	  << "   if WC(@) = BK::AlwaysGreater return Success;\n"
          << "   ASSERT(WC(@) = BK::AlwaysEqual || WC(@) = BK::CanBeGreaterOrEqual);\n"
	  << " }\n";
      return str;
      

    case IfLexSTEqIncRetFail_IfLexSTEqLeRetFail_RetSucc:
      str << "IfLexSTEqIncRetFail_IfLexSTEqLeRetFail_RetSucc\n";
      str << " S = " << complexTerm1() << "\n";   
      str << " T = " << complexTerm2() << "\n";         
      str << " {\n"
	  << "   ASSERT(top(S) = top(T));\n"
          << "   if lex(S@,T@) = Incomparable return Failure;\n"
	  << "   if lex(S@,T@) = Less return Failure;\n"
          << "   return Success;\n"
	  << " }\n";
      return str;

    default: return str << "????????????;\n";
    };
  return str;
}; // ostream& StandardKBOrdering::CompiledConstraintForInstanceRetrieval::Command::output(ostream& str) const
#endif
//=================================================================
