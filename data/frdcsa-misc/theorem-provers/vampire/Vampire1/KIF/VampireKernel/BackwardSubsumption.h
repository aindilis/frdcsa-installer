//
// File:         BackwardSubsumption.h
// Description:  Interface for several BS related modules.
// Created:      Feb 26, 2000, 20:20
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//============================================================================
#ifndef BACKWARD_SUBSUMPTION_H
//=============================================================================
#define BACKWARD_SUBSUMPTION_H 
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "VampireKernelInterceptMacros.h"
#include "BSIndex.h"
#include "BSMachine.h"
//#include "ClauseNumIndex.h"
//#include "Clause.h"
#include "Term.h"
#ifndef NO_DEBUG
#include "SubsumptionCheck.h" 
#endif
namespace VK
{
  using namespace BK;
  using namespace Gematogen;
class ClauseNumIndex;
class Clause;
}; // namespace VK

//===========================================================================

// This is for experimental comparison with the old discrimination tree based implementation

//#include "OldBackwardSubsumption.h" 

//#define OLD_BACKWARD_SUBSUMPTION_IMPLEMENTATION
#undef OLD_BACKWARD_SUBSUMPTION_IMPLEMENTATION

#ifdef OLD_BACKWARD_SUBSUMPTION_IMPLEMENTATION
#define OLD_IMPL(act) act
#else
#define OLD_IMPL(act)
#endif

//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_BACKWARD_SUBSUMPTION
#define DEBUG_NAMESPACE "BackwardSubsumption"
#endif
#include "debugMacros.h"
//============================================================================

namespace VK
{
  using namespace BK;
  using namespace Gematogen;

class BackwardSubsumption
{
 public:
  typedef BSIndex::LTPair LTPair;  
  typedef BSIndex::LAPair LAPair;
  typedef BSIndex::LATTriple LATTriple;
  typedef BSIndex::LList LList;
  typedef BSIndex::LTList LTList;
  typedef BSIndex::LAList LAList;
  typedef BSIndex::LATList LATList;
  typedef BSIndex::CSkipList CSkipList;
  typedef BSIndex::CLSkipList CLSkipList;
  typedef BSIndex::CLTSkipList CLTSkipList;
  typedef BSIndex::CLASkipList CLASkipList;
  typedef BSIndex::CLATSkipList CLATSkipList;
 public:
  BackwardSubsumption(ClauseNumIndex* clauseNumIndex) 
    : _index(),
    _integrator(&_index),
    _removal(&_index),
    _retrieval(&_index),
    _machine(),        
    _termTraversal(DOP("BackwardSubsumption::_termTraversal")),
    _clauseNumIndex(clauseNumIndex),
    _setMode(false),
    _optimisedMultisetMode(false)
#ifdef DEBUG_NAMESPACE
    , _debugSubsumptionCheck()
#endif 
#ifdef OLD_BACKWARD_SUBSUMPTION_IMPLEMENTATION
    , _oldImplementation(clauseNumIndex)
#endif
    {
      CALL("constructor BackwardSubsumption(ClauseNumIndex* clauseNumIndex)");
    };
  ~BackwardSubsumption() 
    {
      CALL("destructor ~BackwardSubsumption()");      
    };
  bool subsumptionAllowedInSetMode(const Clause* subsumingCl,const Clause* subsumedCl)
    {
      return subsumingCl->NumOfPosEq() <= subsumedCl->NumOfPosEq(); 
    };
  bool subsumptionAllowedInMultisetMode(const Clause* subsumingCl,const Clause* subsumedCl)
    {
      return true; 
    };
  void destroyIndex() { CALL("destroyIndex()"); _index.destroy(); };
#ifndef NO_DEBUG
  bool clean() const { return _index.clean(); };
#endif        
  void integrate(const Clause* cl);
  void remove(const Clause* cl);
  bool query(const Clause* cl);
  bool subsumeNext() 
    {
      CALL("subsumeNext()");
      bool res;
      if (_setMode) 
	{
	  res = subsumeNextSetMode();
	}
      else 
	if (_optimisedMultisetMode && _queryClause->allMaximalSelected())
	  {
	    res = subsumeNextSetMode();
	  }
	else 
	  res = subsumeNextMultisetMode();
  
#ifdef INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING
      if (!res) { INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_RETRIEVAL_TOTAL; };
#endif 

      return res;
    };
  bool subsumeNextSetMode();
  bool subsumeNextMultisetMode();
  const Clause* subsumed() const { return _recentlySubsumedCl; };
  void setSetMode(bool fl) { _setMode = fl; }; 
  void setOptimisedMultisetMode(bool fl) { _optimisedMultisetMode = fl; }; 
 private:
  bool preproQuery(const Clause* cl);
  bool preproQueryLit(const TERM* lit);
  bool preproQuerySymLit(const TERM* lit);
  bool preproQueryComplexTerm(const TERM* t);
  bool preproQueryComplexTermArguments(const TERM* t);
  bool preproQuerySymLitComplexTerm(const TERM* t);
  bool preproQuerySymLitComplexTermArguments(const TERM* t);
 private:
  BSIndex _index;
  BSIndex::Integrator _integrator;
  BSIndex::Removal _removal;
  BSIndex::Retrieval _retrieval;
  BSMachine _machine; 
 public: 
  TERM::Traversal _termTraversal;
 private: 
  ClauseNumIndex* _clauseNumIndex;
  const Clause* _recentlySubsumedCl;
  const Clause* _queryClause; 
  bool _setMode;
  bool _optimisedMultisetMode;
#ifdef DEBUG_NAMESPACE
  SubsumptionCheck _debugSubsumptionCheck;
#endif
#ifdef OLD_BACKWARD_SUBSUMPTION_IMPLEMENTATION
  OldBackwardSubsumption _oldImplementation;
#endif
}; // class BackwardSubsumption

}; // namespace VK
//======================================================================
#undef DEBUG_NAMESPACE
//======================================================================
#endif
