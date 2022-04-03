//
// File:         StandardKBOrdering.h
// Description:  Standard Knuth-Bendix ordering.  
// Created:      Jun 28, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//=================================================================
#ifndef STANDARD_KB_ORDERING_H
#define STANDARD_KB_ORDERING_H
//===================================================================
#ifndef NO_DEBUG_VIS
#include <iostream.h>
#endif
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "SimplificationOrdering.h"
#include "Flatterm.h"
#include "Term.h"
#include "prefix.h"
#include "WeightPolynomial.h"
#include "Comparison.h"
#include "GlobAlloc.h"
#include "ObjectPool.h"
#include "WeightPolynomialSpecialisedComparisonCommand.h"
#include "ForwardMatchingSubstitutionCache.h"
#include "InstanceRetrievalForDemodulation.h"
#include "TermWeightType.h"
//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_STANDARD_KB_ORDERING
 #define DEBUG_NAMESPACE "StandardKBOrdering"
#endif
#include "debugMacros.h"
//===================================================================

namespace VK
{
  using namespace BK;
class StandardKBOrdering : public SimplificationOrdering
{
 public:
  StandardKBOrdering() : 
    _specialisedComparisonPool((SpecialisedComparison*)0),
    _constraintCheckOnUnifierPool((ConstraintCheckOnUnifier*)0),
    _constraintCheckOnForwardMatchingSubstPool((ConstraintCheckOnForwardMatchingSubst*)0),
    _compiledConstraintOnUnifiersPool((CompiledConstraintForUnifiers*)0),
    _compiledConstraintForInstanceRetrievalPool((CompiledConstraintForInstanceRetrieval*)0)
    {    
      setType(SimplificationOrdering::StandardKB);
    };
  ~StandardKBOrdering();
  void* operator new(size_t);
  void operator delete(void* obj);
  void destroyThoroughly();
  Comparison compare(const Flatterm* term1,
		     const WeightPolynomial& weight1,
		     const Flatterm* term2,
		     const WeightPolynomial& weight2);

  Comparison compareAtoms(const Flatterm* lit1,
			  const WeightPolynomial& weight1,
			  const Flatterm* lit2,
			  const WeightPolynomial& weight2);

  Comparison compareOnCurrentUnifier(PREFIX_SYM* complexTerm1,
				     PREFIX_SYM* complexTerm2);

  Comparison compareOnCurrentUnifier(const TERM& term1,
				     const TERM& term2,
				     ulong variableIndex);

  Comparison compare(const TERM* complexTerm1,
		     const TERM* complexTerm2,
		     const InstanceRetrievalForDemodulation::Substitution* subst);

  Comparison compare(const TERM& term1,
		     const TERM& term2,
		     const ForwardMatchingSubstitutionCache* subst);

  SimplificationOrdering::SpecialisedComparison* reserveSpecialisedComparison();

  void releaseSpecialisedComparison(SimplificationOrdering::SpecialisedComparison* sc);

  SimplificationOrdering::StoredConstraint*
    storedConstraintGreater(SimplificationOrdering::SpecialisedComparison* sc);
  SimplificationOrdering::StoredConstraint*
    storedConstraintGreaterOrEqual(SimplificationOrdering::SpecialisedComparison* sc);
  SimplificationOrdering::StoredConstraint*
    storedConstraintLess(SimplificationOrdering::SpecialisedComparison* sc);
  SimplificationOrdering::StoredConstraint*
    storedConstraintLessOrEqual(SimplificationOrdering::SpecialisedComparison* sc);

  SimplificationOrdering::StoredConstraint* copy(SimplificationOrdering::StoredConstraint* constr);

  void releaseConstraint(SimplificationOrdering::StoredConstraint* constr);

  SimplificationOrdering::ConstraintCheckOnUnifier* reserveConstraintCheckOnUnifier();
  void releaseConstraintCheckOnUnifier(SimplificationOrdering::ConstraintCheckOnUnifier* ccu);

 
  SimplificationOrdering::ConstraintCheckOnForwardMatchingSubst* 
    reserveConstraintCheckOnForwardMatchingSubst();

  void 
    releaseConstraintCheckOnForwardMatchingSubst(SimplificationOrdering::ConstraintCheckOnForwardMatchingSubst* ccfms);
  

 SimplificationOrdering::CompiledConstraintForUnifiers* reserveCompiledConstraintForUnifiers();
 void releaseCompiledConstraintForUnifiers(SimplificationOrdering::CompiledConstraintForUnifiers* cc);

 SimplificationOrdering::CompiledConstraintForInstanceRetrieval* reserveCompiledConstraintForInstanceRetrieval();
 void releaseCompiledConstraintForInstanceRetrieval(SimplificationOrdering::CompiledConstraintForInstanceRetrieval* cc);



 Comparison compareWithTransposition(const Flatterm* term);

 
 
 private:
  class SpecialisedComparison;
  class ConstraintCheckOnUnifier;
  class ConstraintCheckOnForwardMatchingSubst;
  class StoredConstraint;
  class CompiledConstraintForUnifiers;
  class CompiledConstraintForInstanceRetrieval;
  typedef ObjectPool<GlobAlloc,PREFIX::ITER> PrefixIterPool;
  typedef ObjectPool<GlobAlloc,ST_TERM_WITH_SUBST> StTermWithSubstPool;
 private:

  Comparison compare(const Flatterm* term1,
		     const Flatterm* term2);

  Comparison compareLexicographically(const Flatterm* term1,
				      const Flatterm* term2);

  Comparison compareAtomsLexicographically(const Flatterm* lit1,
					   const Flatterm* lit2);

  Comparison compareLexicographically(const TERM* complexTerm1,
				      const TERM* complexTerm2);

  Comparison compareLexicographically(const TERM& term1,
				      const TERM& term2);

  bool termWithUnifierContainsVariable(PREFIX_SYM* complexTerm,
				       Unifier::Variable* var);

  Comparison compareLexOnCurrentUnifier(PREFIX_SYM* complexTerm1,
					PREFIX_SYM* complexTerm2);

  bool termWithUnifierContainsVariable(const TERM* complexTerm,
				       ulong variableIndex,
				       Unifier::Variable* var);

  bool termWithUnifierContainsVariable(const TERM& term,
				       ulong variableIndex,
				       Unifier::Variable* var);

  Comparison compareOnCurrentUnifier(PREFIX_SYM* complexTerm1,
				     const TERM* complexTerm2,
				     ulong variableIndex);

  Comparison compareLexOnCurrentUnifier(PREFIX_SYM* complexTerm1,
					const TERM* complexTerm2,
					ulong variableIndex);

  Comparison compareLexOnCurrentUnifier(PREFIX_SYM* complexTerm1,
					const TERM& term2,
					ulong variableIndex);


  Comparison compareLexOnCurrentUnifier(const TERM& term1,
					const TERM& term2,
					ulong variableIndex);

  Comparison compareLexicographically(const TERM* complexTerm1,
				      const InstanceRetrievalForDemodulation::Substitution* subst,
				      const TERM* complexTerm2);

  Comparison compare(const TERM* complexTerm1,
		     const InstanceRetrievalForDemodulation::Substitution* subst,
		     const TERM& term2);

  Comparison compare(const TERM* complexTerm1,const TERM* complexTerm2);

  Comparison compare(const TERM& term1,const TERM& term2);
		     

  Comparison compare(const TERM& term1,
		     const TERM& term2,
		     const InstanceRetrievalForDemodulation::Substitution* subst);

  Comparison compareLexicographically(const TERM* complexTerm1,
				      const TERM* complexTerm2,
				      const InstanceRetrievalForDemodulation::Substitution* subst);

  Comparison compare(const TERM& term1,
		     const ForwardMatchingSubstitutionCache* subst,
		     const Flatterm* term2);

  Comparison compareLexicographically(const TERM* complexTerm1,
				      const ForwardMatchingSubstitutionCache* subst,
				      const Flatterm* term2);

  Comparison compareLexicographically(const TERM& term1,
				      const TERM& term2,
				      const ForwardMatchingSubstitutionCache* subst);
 private:

  SpecialisedComparison* _specialisedComparisonPool;

  ConstraintCheckOnUnifier* _constraintCheckOnUnifierPool;
  
  ConstraintCheckOnForwardMatchingSubst* _constraintCheckOnForwardMatchingSubstPool;

  CompiledConstraintForUnifiers* _compiledConstraintOnUnifiersPool;

  CompiledConstraintForInstanceRetrieval* _compiledConstraintForInstanceRetrievalPool;

  WeightPolynomial _weight1;
  WeightPolynomial _weight2;

  PrefixIterPool _prefixIterPool;
  StTermWithSubstPool _stTermWithSubstPool;

  friend class SpecialisedComparison;
  friend class ConstraintCheckOnUnifier;
  friend class ConstraintCheckOnForwardMatchingSubst;
  friend class CompiledConstraintForUnifiers;
}; // class StandardKBOrdering : public SimplificationOrdering

}; // namespace VK

//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_STANDARD_KB_ORDERING
 #define DEBUG_NAMESPACE "StandardKBOrdering::SpecialisedComparison"
#endif
#include "debugMacros.h"
//===================================================================

namespace VK
{
  using namespace BK;

class StandardKBOrdering::SpecialisedComparison :
public SimplificationOrdering::SpecialisedComparison
{
 public:
  void load(const TERM& lhs,const TERM& rhs);
  bool lhsVarInstMayOrder();
  bool rhsVarInstMayOrder();
  void specialiseGreater();
  void specialiseGreaterOrEqual();
  void specialiseLess();
  void specialiseLessOrEqual();


#ifndef NO_DEBUG_VIS
  ostream& output(ostream& str) const;
#endif

 private:
  SpecialisedComparison(StandardKBOrdering* ord) : 
    _ordering(ord),
    _nextInPool((SpecialisedComparison*)0),
    _constraintGreater((StoredConstraint*)0),
    _constraintGreaterOrEqual((StoredConstraint*)0),
    _constraintLess((StoredConstraint*)0),
    _constraintLessOrEqual((StoredConstraint*)0)
    {
    };
  ~SpecialisedComparison();
  void* operator new(size_t)
    {
      return GlobAlloc::allocate(sizeof(SpecialisedComparison));
    };
  void operator delete(void* obj)
    {
      return GlobAlloc::deallocate(obj,sizeof(SpecialisedComparison));
    };
  SpecialisedComparison* getNextInPool() { return _nextInPool; };
  void setNextInPool(StandardKBOrdering::SpecialisedComparison* sc) { _nextInPool = sc; };
  void specialise();
  StandardKBOrdering::StoredConstraint* storedConstraintGreater();
  StandardKBOrdering::StoredConstraint* storedConstraintGreaterOrEqual();
  StandardKBOrdering::StoredConstraint* storedConstraintLess();
  StandardKBOrdering::StoredConstraint* storedConstraintLessOrEqual();
  const WeightPolynomial::SpecialisedComparison& weightComparison() const
    {
      return _weightComparison;
    };
  
  Comparison lexicographicComparisonResult()
    {
      return _lexicographicComparisonResult;
    };

 private:
  StandardKBOrdering* _ordering;
  TERM _lhs;
  TERM _rhs;
  WeightPolynomial _lhsWeight;
  WeightPolynomial _rhsWeight;
  bool _specialised;
  WeightPolynomial::SpecialisedComparison _weightComparison;
  Comparison _lexicographicComparisonResult;
  SpecialisedComparison* _nextInPool;
  StandardKBOrdering::StoredConstraint* _constraintGreater;
  StandardKBOrdering::StoredConstraint* _constraintGreaterOrEqual;
  StandardKBOrdering::StoredConstraint* _constraintLess;
  StandardKBOrdering::StoredConstraint* _constraintLessOrEqual;
  friend class StandardKBOrdering;
  friend class StandardKBOrdering::CompiledConstraintForUnifiers;
  friend class StandardKBOrdering::CompiledConstraintForInstanceRetrieval;
}; // class StandardKBOrdering::SpecialisedComparison

}; // namespace VK

#ifndef NO_DEBUG_VIS

inline
ostream& operator<<(ostream& str,
		    const VK::StandardKBOrdering::SpecialisedComparison& sc)
{
  return sc.output(str);
};

inline
ostream& operator<<(ostream& str,
		    const VK::StandardKBOrdering::SpecialisedComparison* sc)
{
  return sc->output(str);
};

#endif



//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_SIMPLIFICATION_ORDERING
#define DEBUG_NAMESPACE "StandardKBOrdering::ConstraintCheckOnUnifier"
#endif
#include "debugMacros.h"
//===================================================================

namespace VK
{
  using namespace BK;


class StandardKBOrdering::ConstraintCheckOnUnifier :
public SimplificationOrdering::ConstraintCheckOnUnifier
{
 public:
  bool holds(const SimplificationOrdering::StoredConstraint* constr,
	     ulong variableIndex);
 private:
  ConstraintCheckOnUnifier(StandardKBOrdering* ord) : 
    _ordering(ord),
    _nextInPool((ConstraintCheckOnUnifier*)0) 
    {
    };
  ~ConstraintCheckOnUnifier();   
  void* operator new(size_t)
    {
      return GlobAlloc::allocate(sizeof(ConstraintCheckOnUnifier));
    };
  void operator delete(void* obj)
    {
      return GlobAlloc::deallocate(obj,sizeof(ConstraintCheckOnUnifier));
    };  
  ConstraintCheckOnUnifier* getNextInPool() { return _nextInPool; };
  void setNextInPool(StandardKBOrdering::ConstraintCheckOnUnifier* ccu) { _nextInPool = ccu; };
  
  bool holdsGreater(const StoredConstraint* constr);
  bool holdsGreaterOrEqual(const StoredConstraint* constr);
  bool holdsLess(const StoredConstraint* constr);
  bool holdsLessOrEqual(const StoredConstraint* constr);

  FunctionComparison 
    checkGreaterOrEqual(const WeightPolynomial::SpecialisedComparison::Stored& weightComparison);
  FunctionComparison 
    checkLessOrEqual(const WeightPolynomial::SpecialisedComparison::Stored& weightComparison);

  bool lexicorgaphicallyGreater(const TERM& lhs,const TERM& rhs);
  bool lexicorgaphicallyGreaterOrEqual(const TERM& lhs,const TERM& rhs);
  bool lexicorgaphicallyLess(const TERM& lhs,const TERM& rhs);
  bool lexicorgaphicallyLessOrEqual(const TERM& lhs,const TERM& rhs);

  
 private:
  ConstraintCheckOnUnifier() 
    {
      CALL("constructor ConstraintCheckOnUnifier()");
      ICP("ICP0");
    };
 private:
  StandardKBOrdering* _ordering;
  ConstraintCheckOnUnifier* _nextInPool;
  ulong _currentVariableIndex;
  ST_TERM_WITH_SUBST _lhsIter;
  ST_TERM_WITH_SUBST _rhsIter;
  WeightPolynomial _weightPositivePartInstance;
  WeightPolynomial _weightNegativePartInstance;
  friend class StandardKBOrdering;
}; // class SimplificationOrdering::ConstraintCheckOnUnifier

}; // namespace VK



//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_SIMPLIFICATION_ORDERING
#define DEBUG_NAMESPACE "StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst"
#endif
#include "debugMacros.h"
//===================================================================

namespace VK
{
  using namespace BK;

class StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst :
public SimplificationOrdering::ConstraintCheckOnForwardMatchingSubst
{
 public:
  bool holds(const SimplificationOrdering::StoredConstraint* constr,
	     const ForwardMatchingSubstitutionCache* subst);
 private:
  ConstraintCheckOnForwardMatchingSubst(StandardKBOrdering* ord) : 
    _ordering(ord),
    _nextInPool((ConstraintCheckOnForwardMatchingSubst*)0)
    {
    };
  ~ConstraintCheckOnForwardMatchingSubst();
  void* operator new(size_t)
    {
      return GlobAlloc::allocate(sizeof(ConstraintCheckOnForwardMatchingSubst));
    };
  void operator delete(void* obj)
    {
      return GlobAlloc::deallocate(obj,sizeof(ConstraintCheckOnForwardMatchingSubst));
    };  
  ConstraintCheckOnForwardMatchingSubst* getNextInPool() { return _nextInPool; };
  void setNextInPool(StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst* ccfms) 
    { 
      _nextInPool = ccfms; 
    };
  
  bool holdsGreater(const StoredConstraint* constr);
  bool holdsGreaterOrEqual(const StoredConstraint* constr);
  bool holdsLess(const StoredConstraint* constr);
  bool holdsLessOrEqual(const StoredConstraint* constr);

  FunctionComparison 
    checkGreaterOrEqual(const WeightPolynomial::SpecialisedComparison::Stored& weightComparison);
  FunctionComparison 
    checkLessOrEqual(const WeightPolynomial::SpecialisedComparison::Stored& weightComparison);

  bool lexicorgaphicallyGreater(const TERM& lhs,const TERM& rhs);
  bool lexicorgaphicallyGreaterOrEqual(const TERM& lhs,const TERM& rhs);
  bool lexicorgaphicallyLess(const TERM& lhs,const TERM& rhs);
  bool lexicorgaphicallyLessOrEqual(const TERM& lhs,const TERM& rhs);


 private:
  ConstraintCheckOnForwardMatchingSubst() 
    {
      CALL("constructor ConstraintCheckOnForwardMatchingSubst()");
      ICP("ICP0");
    };

 private:
  StandardKBOrdering* _ordering;
  ConstraintCheckOnForwardMatchingSubst* _nextInPool;
  const ForwardMatchingSubstitutionCache* _subst;
  ForwardMatchingSubstitutionCache::Instance _lhsInstance;
  ForwardMatchingSubstitutionCache::Instance _rhsInstance;
  WeightPolynomial _weightPositivePartInstance;
  WeightPolynomial _weightNegativePartInstance;
  friend class StandardKBOrdering;
}; // class StandardKBOrdering::ConstraintCheckOnForwardMatchingSubst

}; // namespace VK

//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_STANDARD_KB_ORDERING
 #define DEBUG_NAMESPACE "StandardKBOrdering::StoredConstraint"
#endif
#include "debugMacros.h"
//===================================================================

namespace VK
{
  using namespace BK;

class StandardKBOrdering::StoredConstraint
{
 public:
  enum TargetRelation { Greater, GreaterOrEqual, Less, LessOrEqual };
 public:
  StoredConstraint(TargetRelation tr,
		   const WeightPolynomial::SpecialisedComparison& wc,
		   Comparison lc,
		   const TERM& lhs,
		   const TERM& rhs) :
    _referenceCounter(0L),
    _targetRelation(tr),
    _weightComparison(wc),
    _lexicographicComparisonResult(lc),
    _lhs(lhs),
    _rhs(rhs)
    {
      CALL("constructor StoredConstraint(TargetRelation tr,..)");
    };
    
  ~StoredConstraint() 
    {
      CALL("destructor ~StoredConstraint()");
      ASSERT(!referenceCounter());
    };

  void* operator new(size_t)
    {
      return GlobAlloc::allocate(sizeof(StoredConstraint));
    };  
  void operator delete(void* obj)
    {
      return GlobAlloc::deallocate(obj,sizeof(StoredConstraint));
    };

  const long& referenceCounter() const { return _referenceCounter; };
  void incReferenceCounter() 
    {
      CALL("incReferenceCounter()");
      ++_referenceCounter; 
    };
  void decReferenceCounter() 
    {
      CALL("decReferenceCounter()");
      --_referenceCounter;
      ASSERT(_referenceCounter >= 0L);
    };

  const TargetRelation& targetRelation() const { return _targetRelation; };
  const WeightPolynomial::SpecialisedComparison::Stored& weightComparison() const
    {
      return _weightComparison;
    };
  const Comparison& lexicographicComparisonResult() const 
    {
      return _lexicographicComparisonResult;
    };
  const TERM& lhs() const { return _lhs; };
  const TERM& rhs() const { return _rhs; };

#ifndef NO_DEBUG_VIS
  ostream& output(ostream& str) const
    {
      switch (targetRelation())
	{
	case Greater:        str << "Greater        "; break;
	case GreaterOrEqual: str << "GreaterOrEqual "; break;
	case Less:           str << "Less           "; break;
	case LessOrEqual:    str << "LessOrEqual    "; break;
	default:             str << "?????????????? ";
	};
      str << lhs() << " vs " << rhs() << " --> " << weightComparison();
      str << " LEX = " << lexicographicComparisonResult();
      return str;
    };
#endif

 private:
  StoredConstraint() 
    {
      CALL("constructor StoredConstraint()");
      ICP("ICP0");
    };
  StoredConstraint(const StoredConstraint&) 
    { 
      CALL("constructor StoredConstraint(const StoredConstraint&)");
      ICP("ICP0");
    };
  StoredConstraint& operator=(const StoredConstraint&) 
  {
    CALL("operator=(const StoredConstraint&)");
    ICP("ICP0");
    return *this;
  };
 private:
  long _referenceCounter;
  TargetRelation _targetRelation;
  WeightPolynomial::SpecialisedComparison::Stored _weightComparison;
  Comparison _lexicographicComparisonResult;
  TERM _lhs;
  TERM _rhs;
}; // class StandardKBOrdering::StoredConstraint


}; // namespace VK

#ifndef NO_DEBUG_VIS

inline
ostream& operator<<(ostream& str,
		    const VK::StandardKBOrdering::StoredConstraint& constr)
{
  return constr.output(str);
};

inline
ostream& operator<<(ostream& str,
		    const VK::StandardKBOrdering::StoredConstraint* constr)
{
  return constr->output(str);
};

#endif

//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_STANDARD_KB_ORDERING
 #define DEBUG_NAMESPACE "StandardKBOrdering::CompiledConstraintForUnifiers"
#endif
#include "debugMacros.h"
//===================================================================

namespace VK
{
  using namespace BK;

class StandardKBOrdering::CompiledConstraintForUnifiers :
public SimplificationOrdering::CompiledConstraintForUnifiers
{
 public:
  CompiledConstraintForUnifiers(StandardKBOrdering* ord) :
    _ordering(ord),
    _nextInPool((CompiledConstraintForUnifiers*)0),
    _weightComparisonCode(DOP("StandardKBOrdering::CompiledConstraintForUnifiers::_weightComparisonCode")),
    _mainCode(DOP("StandardKBOrdering::CompiledConstraintForUnifiers::_mainCode"))
    {
      DOP(_weightComparisonCode.freeze());
      _mainCode.expand();
      DOP(_mainCode.freeze());
    };
  ~CompiledConstraintForUnifiers();  
  void* operator new(size_t)
    {
      return GlobAlloc::allocate(sizeof(CompiledConstraintForUnifiers));
    };
  void operator delete(void* obj)
    {
      return GlobAlloc::deallocate(obj,sizeof(CompiledConstraintForUnifiers));
    }; 
  CompiledConstraintForUnifiers* getNextInPool() const { return _nextInPool; };
  void setNextInPool(CompiledConstraintForUnifiers* cc) { _nextInPool = cc; };

  void load(PREFIX_SYM* lhs,PREFIX_SYM* rhs);
  bool lhsVarInstMayOrder();
  bool rhsVarInstMayOrder();
  void compileGreaterOrEqual();
  bool holdsOnCurrentUnifier();

#ifndef NO_DEBUG_VIS
  ostream& output(ostream& str) const;
#endif


 private:
  class Command
    {
    public:
      enum Tag
      {
	RetSuccess,
	RetFailure,

	IfWCFailRetFail_IfWCAlwGrRetSucc,
	/*
	  if WC(@) = FailureToCompare return Failure;
	  if WC(@) = AlwaysGreater return Success;
	  ASSERT(WC(@) = AlwaysEqual || WC(@) = CanBeGreaterOrEqual);
	*/
		
	
	IfLexSTEqIncRetFail_IfLexSTEqLeRetFail_RetSucc
	/*
	  ASSERT(top(S) = top(T));
	  if lex(S@,T@) = Incomparable return Failure;
	  if lex(S@,T@) = Less return Failure;
	  return Success;
	 */

	


		
      };
    public:
      Command() {};
      ~Command() {};
      const Tag& tag() const { return _tag; };
      void setTag(const Tag& t) 
	{ 
	  _tag = t;
	};
      bool isTerminal() const;
      Unifier::Variable* var1() const { return _var1; };
      void setVar1(Unifier::Variable* v) { _var1 = v; };
      Unifier::Variable* var2() const { return _var2; };
      void setVar2(Unifier::Variable* v) { _var2 = v; };
      PREFIX_SYM* complexTerm1() const { return _complexTerm1; };
      void setComplexTerm1(PREFIX_SYM* term) { _complexTerm1 = term; };
      PREFIX_SYM* complexTerm2() const { return _complexTerm2; };
      void setComplexTerm2(PREFIX_SYM* term) { _complexTerm2 = term; };
      bool requiresWeightComparison() const
	{
	  return (tag() == IfWCFailRetFail_IfWCAlwGrRetSucc);
	};
#ifndef NO_DEBUG_VIS
      ostream& output(ostream& str) const;
#endif
    private:
      Tag _tag;
      Unifier::Variable* _var1;
      Unifier::Variable* _var2;
      PREFIX_SYM* _complexTerm1;
      PREFIX_SYM* _complexTerm2;
    }; // class Command

 private:
  typedef WeightPolynomial::SpecialisedComparison::Command WeightComparisonCommand;
  
#ifndef NO_DEBUG
  enum TargetRelation
  {
    GreaterOrEqual
  };
#endif

 private:
  CompiledConstraintForUnifiers()
    {
      CALL("constructor CompiledConstraintForUnifiers()");
      ICP("ICP0");
    };
  bool compileLexGreaterOrEqual(Command*& code,ulong maxCodeSize);
  Comparison compareLexicographically(PREFIX_SYM* term1,PREFIX_SYM* term2);
  Comparison compareLexicographically(Unifier::Variable* var,PREFIX_SYM* term);
  Comparison compareLexicographically(Unifier::Variable* var1,Unifier::Variable* var2);
  FunctionComparison interpretWeightComparisonCode();

 private:
  StandardKBOrdering* _ordering;
  StandardKBOrdering::CompiledConstraintForUnifiers* _nextInPool;

#ifndef NO_DEBUG
  TargetRelation _targetRelation;
#endif

  PREFIX_SYM* _lhs;
  PREFIX_SYM* _rhs;
  
  PREFIX::ITER _lhsIter;
  PREFIX::ITER _rhsIter;

  WeightPolynomial _lhsWeight;
  WeightPolynomial _rhsWeight;
  WeightPolynomial::SpecialisedComparison _weightComparison;

  Array<GlobAlloc,
    WeightComparisonCommand,
    32UL,
    CompiledConstraintForUnifiers> _weightComparisonCode;
  
  Array<GlobAlloc,Command,32UL,CompiledConstraintForUnifiers> _mainCode;

  // weight comparison abstract machine

  bool _grnd;
  TermWeightType _acc;
  WeightPolynomial _posinst;
  WeightPolynomial _neginst;

}; // class StandardKBOrdering::CompiledConstraintForUnifiers


}; // namespace VK



//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_STANDARD_KB_ORDERING
 #define DEBUG_NAMESPACE "StandardKBOrdering::CompiledConstraintForInstanceRetrieval"
#endif
#include "debugMacros.h"
//===================================================================

namespace VK
{
  using namespace BK;

class StandardKBOrdering::CompiledConstraintForInstanceRetrieval :
public SimplificationOrdering::CompiledConstraintForInstanceRetrieval
{
 public:
  CompiledConstraintForInstanceRetrieval(StandardKBOrdering* ord) :
    _ordering(ord),
    _nextInPool((CompiledConstraintForInstanceRetrieval*)0),
    _weightComparisonCode(DOP("StandardKBOrdering::CompiledConstraintForInstanceRetrieval::_weightComparisonCode")),
    _mainCode(DOP("StandardKBOrdering::CompiledConstraintForInstanceRetrieval::_mainCode"))
    {
      DOP(_weightComparisonCode.freeze());
      _mainCode.expand();
      DOP(_mainCode.freeze());
    };
  ~CompiledConstraintForInstanceRetrieval();  
  void* operator new(size_t)
    {
      return GlobAlloc::allocate(sizeof(CompiledConstraintForInstanceRetrieval));
    };
  void operator delete(void* obj)
    {
      return GlobAlloc::deallocate(obj,sizeof(CompiledConstraintForInstanceRetrieval));
    }; 
  CompiledConstraintForInstanceRetrieval* getNextInPool() const { return _nextInPool; };
  void setNextInPool(CompiledConstraintForInstanceRetrieval* cc) { _nextInPool = cc; };

  void loadGreater(const TERM* lhs,const TERM* rhs);
  bool lhsVarInstMayOrder();
  bool rhsVarInstMayOrder();
  void compile(const InstanceRetrievalForDemodulation::Substitution* subst);
  bool holds();

#ifndef NO_DEBUG_VIS
  ostream& output(ostream& str) const;
#endif


 private:
  class Command
    {
    public:
      enum Tag
      {
	RetSuccess,
	RetFailure,

	IfWCFailRetFail_IfWCAlwGrRetSucc,
	/*
	  if WC(@) = FailureToCompare return Failure;
	  if WC(@) = AlwaysGreater return Success;
	  ASSERT(WC(@) = AlwaysEqual || WC(@) = CanBeGreaterOrEqual);
	*/
	
	IfLexSTEqIncRetFail_IfLexSTEqLeRetFail_RetSucc
	/*
	  ASSERT(top(S) = top(T));
	  if lex(S@,T@) = Incomparable return Failure;
	  if lex(S@,T@) = Less return Failure;
	  return Success;
	 */
      };
    public:
      Command() {};
      ~Command() {};
      const Tag& tag() const { return _tag; };
      void setTag(const Tag& t) 
	{ 
	  _tag = t;
	};
      bool isTerminal() const;
      const TERM* var1() const { return _var1; };
      void setVar1(const TERM* v) { _var1 = v; };
      const TERM* var2() const { return _var2; };
      void setVar2(const TERM* v) { _var2 = v; };
#ifndef NO_DEBUG
      ulong varNum1() const { return _varNum1; };
      void setVarNum1(ulong v) { _varNum1 = v; };
      ulong varNum2() const { return _varNum2; };
      void setVarNum2(ulong v) { _varNum2 = v; };
#endif

      const TERM* complexTerm1() const { return _complexTerm1; };
      void setComplexTerm1(const TERM* term) { _complexTerm1 = term; };
      const TERM* complexTerm2() const { return _complexTerm2; };
      void setComplexTerm2(const TERM* term) { _complexTerm2 = term; };
      bool requiresWeightComparison() const
	{
	  return (tag() == IfWCFailRetFail_IfWCAlwGrRetSucc);
	};
#ifndef NO_DEBUG_VIS
      ostream& output(ostream& str) const;
#endif
    private:
      Tag _tag;      
#ifndef NO_DEBUG
      ulong _varNum1;
      ulong _varNum2;
#endif
      const TERM* _var1;
      const TERM* _var2;
      const TERM* _complexTerm1;
      const TERM* _complexTerm2;
    }; // class Command

 private:
  typedef WeightPolynomial::SpecialisedComparison::Command WeightComparisonCommand;
  
  enum TargetRelation
  {
    Greater
  };

 private:
  CompiledConstraintForInstanceRetrieval()
    {
      CALL("constructor CompiledConstraintForInstanceRetrieval()");
      ICP("ICP0");
    };
  bool compileLexGreater(Command*& code,ulong maxCodeSize);
  FunctionComparison interpretWeightComparisonCode();

 private:
  StandardKBOrdering* _ordering;
  StandardKBOrdering::CompiledConstraintForInstanceRetrieval* _nextInPool;
  TargetRelation _targetRelation;
  const InstanceRetrievalForDemodulation::Substitution* _subst;

  const TERM* _lhs;
  const TERM* _rhs;
  
  TERM::Iterator _lhsIter;
  TERM::Iterator _rhsIter;

  InstanceRetrievalForDemodulation::Substitution::Instance _instance1;
  InstanceRetrievalForDemodulation::Substitution::Instance _instance2;


  WeightPolynomial _lhsWeight;
  WeightPolynomial _rhsWeight;
  WeightPolynomial::SpecialisedComparison _weightComparison;

  Array<GlobAlloc,
    WeightComparisonCommand,
    32UL,
    CompiledConstraintForInstanceRetrieval> _weightComparisonCode;
  
  Array<GlobAlloc,Command,32UL,CompiledConstraintForInstanceRetrieval> _mainCode;

  // weight comparison abstract machine

  bool _grnd;
  TermWeightType _acc;
  WeightPolynomial _posinst;
  WeightPolynomial _neginst;
}; // class StandardKBOrdering::CompiledConstraintForInstanceRetrieval


}; // namespace VK

//===================================================================

#endif
