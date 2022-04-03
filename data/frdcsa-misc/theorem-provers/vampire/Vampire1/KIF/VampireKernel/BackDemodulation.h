//
// File:         BackDemodulation.h
// Description:  Back simplification by ordered unit equalities.
// Created:      Apr 16, 2000, Alexandre Riazanov, riazanov@cs.man.ac.uk
// Revised:      Dec 23, 2001, Alexandre Riazanov, riazanov@cs.man.ac.uk
//               typedef Multiset<ulong,HellConst::MaxNumOfDifferentHeaders> 
//                 SetOfSplittingBranches
//                
//               replaced by 
// 
//               typedef ExpandingMultiset<GlobAlloc,ulong,1UL,BackDemodulation> 
//                 SetOfSplittingBranches
// Revised:      May 13, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//               1) Indexing for backward matching is now implemented 
//               with parh indexing and joins.
//               2) Ordering constraints reimplemented. 
//====================================================
#ifndef BACK_DEMODULATION_H
//====================================================
#define BACK_DEMODULATION_H
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "ExpandingMultiset.h"
#include "Term.h"
#include "NewClause.h"   
#include "MultisetOfVariables.h"
#include "InstanceRetrievalForDemodulation.h"
#include "LiteralList.h"
#include "RuntimeError.h"
#include "DestructionMode.h"
//====================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_BACK_DEMODULATION
#define DEBUG_NAMESPACE "BackDemodulation"
#endif
#include "debugMacros.h"
//====================================================

namespace VK
{
  using namespace BK;
  using namespace Gematogen;

class Clause;
class ClauseNumIndex;
class SHARING_INDEX;
class BackDemodulation
{
 public:
  typedef ExpandingMultiset<GlobAlloc,ulong,32UL,BackDemodulation> SetOfSplittingBranches;

 public:
  BackDemodulation(NewClause* freshClause,SHARING_INDEX* sharingIndex,ClauseNumIndex* clauseNumIndex) 
    : _bDOnSplittingBranchesFlag(false),
    _resolvent(freshClause),
    _clauseNumIndex(clauseNumIndex),
    _splittingBranches(DOP("BackDemodulation::_splittingBranches")),
    _numOfBDOnSplittingBranches(0UL)
    {
      CALL("constructor BackDemodulation(NewClause* freshClause,SHARING_INDEX* sharingIndex,ClauseNumIndex* clauseNumIndex)");
      DOP(_splittingBranches.freeze());
      _compiledConstraint = 
	(SimplificationOrdering::CompiledConstraintForInstanceRetrieval*)0; 
    };
  ~BackDemodulation() 
    { 
      CALL("destructor ~BackDemodulation()");  
      if (DestructionMode::isThorough()) destroy(); 
    };
  void destroy() 
    {
      if (_compiledConstraint)
	{
	  SimplificationOrdering::current()->releaseCompiledConstraintForInstanceRetrieval(_compiledConstraint);
	  _compiledConstraint = (SimplificationOrdering::CompiledConstraintForInstanceRetrieval*)0;
	};
      _index.destroy();
    }; 
  void setDegree(long d) { _degree = d; };
  void setBDOnSplittingBranchesFlag(bool fl) 
    {
      if (_bDOnSplittingBranchesFlag && (!fl)) RuntimeError::report("Switching _bDOnSplittingBranchesFlag off.");
      _bDOnSplittingBranchesFlag = fl; 
    };
  bool query(const Clause* cl);
  bool nextSimplified();
  bool collectInference();
  
  Clause* simplified() { return _recentlySimplified; }; 
  void integrate(Clause* cl);
  void remove(Clause* cl);
  ulong statBDOnSplittingBranches() const { return _numOfBDOnSplittingBranches; };
  void resetStat() { _numOfBDOnSplittingBranches = 0UL; };
 private:
  BackDemodulation()
    {
      CALL("constructor BackDemodulation()");
      ICP("ICP0");
    };

  bool query(const TERM* lhsref,const TERM* rhsref);
   
  bool appropriateDegree(const SimplificationOrdering::CompiledConstraintForInstanceRetrieval::Category& cat)
    {
      switch (cat)
	{	  
	case SimplificationOrdering::CompiledConstraintForInstanceRetrieval::NearlyTautology:
	  return  (_degree >= 2);
	case SimplificationOrdering::CompiledConstraintForInstanceRetrieval::Volatile:
	  return (_degree >= 3);
	case SimplificationOrdering::CompiledConstraintForInstanceRetrieval::NearlyContradiction:
	  return (_degree >= 4);
	default: return false;
	};
    };

  bool rewrite(const TERM::OccList::Node* occ);
  bool rewriteLit(TERM* lit,bool inhSel);  

  bool collectRHSInstance();

 private:
  bool _swapQuerySides;
  long _degree;
  bool _bDOnSplittingBranchesFlag;
  NewClause* _resolvent;

  InstanceRetrievalForDemodulation _index;
  InstanceRetrievalForDemodulation::Substitution::Instance _rhsInstance;

  SimplificationOrdering::CompiledConstraintForInstanceRetrieval* _compiledConstraint;

  SimplificationOrdering::CompiledConstraintForInstanceRetrieval* _currentConstraint;



  ClauseNumIndex* _clauseNumIndex;
  const Clause* _query;
  ulong _queryNum;
  const TERM* _queryLHS;
  TERM _queryRHS;
  TERM::Iterator _termIter;
  const TERM::OccList::Node* _candidates; 
  Clause* _recentlySimplified;
  const TERM* _redex;
  TERM::Iterator _rewrittenLitIter;
  TERM::Iterator _RHSIter;
  SetOfSplittingBranches _splittingBranches;
  ulong _numOfBDOnSplittingBranches;
}; // class BackDemodulation


}; // namespace VK
//====================================================
#undef DEBUG_NAMESPACE
//====================================================
#endif
