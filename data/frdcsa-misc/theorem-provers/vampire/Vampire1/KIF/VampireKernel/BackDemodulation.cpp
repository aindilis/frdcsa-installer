//
// File:         BackDemodulation.cpp
// Description:  Back simplification by ordered unit equalities.
// Created:      Apr 16, 2000.
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
// Revised:      Dec 23, 2001.
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
#include "BackDemodulation.h"
#include "ClauseNumIndex.h"
#include "Clause.h"
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
void BackDemodulation::integrate(Clause* cl)
{
  CALL("integrate(Clause* cl)");   
  ASSERT(_clauseNumIndex->find(cl->Number()) == cl);
  ulong clauseNum = cl->Number(); 
  TERM* lit;
  TERM::Iterator iter;
  TERM* term;
  ulong ln = 0;
  for(LiteralList::Iterator litIter(cl->LitList());litIter.notEnd();litIter.next())
    {
      lit = litIter.currentLiteral();
      if (lit->arity())
	{
	  iter.Reset(lit);
	  iter.Next();
	  do
	    {
	      term = iter.CurrPos();
	      if (term->isComplex()) 
		{         
#ifdef INTERCEPT_BACKWARD_MATCHING_INDEXING
		  if (!term->Occurences())
		    {
		      INTERCEPT_BACKWARD_MATCHING_INDEXING_INTEGRATE(term);
		    };
#endif
		  term->RegisterOccurence(clauseNum,ln);
	 
		  _index.integrate(term);
		};
	    }
	  while (iter.Next());
	};         
      ln++;
    };
}; // void BackDemodulation::integrate(Clause* cl)
 
void BackDemodulation::remove(Clause* cl) 
{
  CALL("remove(Clause* cl)");
  ASSERT(_clauseNumIndex->find(cl->Number()) == cl);
  ulong clauseNum = cl->Number(); 
  TERM* lit;
  TERM::Iterator iter;
  TERM* term;

  for(LiteralList::Iterator litIter(cl->LitList());litIter.notEnd();litIter.next())
    {
      lit = litIter.currentLiteral();
      if (lit->arity())
	{
	  iter.Reset(lit);
	  iter.Next();
	  do
	    {
	      term = iter.CurrPos();
	      if (term->isComplex()) 
		{
#ifdef INTERCEPT_BACKWARD_MATCHING_INDEXING
		  bool nonemptyOcc = term->Occurences() && 
		    (*term->Occurences());       
#endif        

		  term->RemoveOccurences(clauseNum);

#ifdef INTERCEPT_BACKWARD_MATCHING_INDEXING
		  if ((!term->Occurences()) && nonemptyOcc)
		    {
		      INTERCEPT_BACKWARD_MATCHING_INDEXING_REMOVE(term);
		    };
#endif
		  
		  if (!term->Occurences()) 
		    _index.remove(term);
		};
	    }
	  while (iter.Next());
	};   
    };
}; // void BackDemodulation::remove(Clause* cl)  


bool BackDemodulation::query(const Clause* cl) 
{ 
  CALL("query(const Clause* cl)");  

  //DF; cout << "TRY " << cl << "\n";

  // At the moment no answer literals are allowed in rewriting rules.
  if (cl->flag(Clause::FlagContainsAnswerLiterals)) return false; 
  if (!cl->Active()) return false;
  if (cl->Unit() || (_bDOnSplittingBranchesFlag && (cl->LitList()->numOfSelectedLits() == 1)))
    {   
      const TERM* lit = cl->LitList()->firstLit(); // the selected one
      if (lit->Positive() && lit->IsEquality())
	{
	  if (_bDOnSplittingBranchesFlag)
	    {
	      _splittingBranches.reset();
	      LiteralList::Iterator iter(cl->LitList());
	      iter.next();
	      DOP(_splittingBranches.unfreeze());
	      while (iter.notEnd())
		{
		  if (iter.currentLiteral()->IsSplitting()) 
		    {
		      ASSERT(!iter.currentLiteral()->arity());
		      _splittingBranches.addSafe(iter.currentLiteral()->HeaderNum()); 
		    }
		  else return false;
		  iter.next();
		};
	      DOP(_splittingBranches.freeze());
	    }
	  else 
	    if (!cl->Unit()) return false;

	  _query = cl;
	  _queryNum = cl->Number();
	  _candidates = (const TERM::OccList::Node*)0;
	  if (lit->IsOrderedEq())
	    {
	      _queryLHS = lit->Args()->First();
	      ASSERT(_queryLHS->isComplex());

	      if (!_index.newQuery(_queryLHS)) return false;

	      _queryRHS = *(lit->Args()->Next());
	      _swapQuerySides = false;

	      _currentConstraint = 
		(SimplificationOrdering::CompiledConstraintForInstanceRetrieval*)0;

	      return true;
	    }
	  else // not preordered
	    {
	      if (_degree <= 0) return false;
	      _swapQuerySides = true;
	      if (query(lit->Arg1(),lit->Arg2())) return true;
	      _swapQuerySides = false;
	      return query(lit->Arg2(),lit->Arg1());
	    };
	};
    };
  return false; 
}; // bool BackDemodulation::query(const Clause* cl) 

bool BackDemodulation::nextSimplified() 
{ 
  CALL("nextSimplified()");
  if (!_query->Active()) 
    { 
      INTERCEPT_BACKWARD_MATCHING_INDEXING_RETRIEVAL_ABORT;
      return false;
    };  


 check_candidates:
  if (_candidates) 
    {   

      ulong clauseNum = _candidates->key();
      if (clauseNum == _queryNum) 
	{ 
	  _candidates = _candidates->next();
	  goto check_candidates; 
	}; 
 
      _recentlySimplified = (Clause*)_clauseNumIndex->find(clauseNum);

      if (!_recentlySimplified)
	{
	  _candidates = _candidates->next();
	  goto check_candidates; 
	};

      ASSERT(_recentlySimplified);
      if (!_recentlySimplified->Active())
	{ 
	  _candidates = _candidates->next();
	  goto check_candidates; 
	}; 
      if (_bDOnSplittingBranchesFlag)
	{
	  ulong numOfBranchesToCover = _splittingBranches.numOfDiffElements();
	  if (!numOfBranchesToCover) return true;
	  if (_recentlySimplified->numOfAllLiterals() <= numOfBranchesToCover)
	    {
	      // Definitely can not be rewritten: does not have 
	      // enough splitting branches.
	      _candidates = _candidates->next();
	      goto check_candidates; 
	    };     
	  LiteralList::Iterator iter(_recentlySimplified->LitList());
	  do
	    {
	      if ((iter.currentLiteral()->IsSplitting()) 
		  && (_splittingBranches.safeScoreOf(iter.currentLiteral()->HeaderNum())))
		{
		  ASSERT(!iter.currentLiteral()->arity());          
		  ASSERT(numOfBranchesToCover);
		  numOfBranchesToCover--;
		  if (!numOfBranchesToCover) { _numOfBDOnSplittingBranches++; return true; };
		};          
	      iter.next();
	    }
	  while (iter.notEnd());
	  ASSERT(numOfBranchesToCover);
	  _candidates = _candidates->next();
	  goto check_candidates; 
	};

      return true;
    };
 try_next_match:
   
  if ((!_index.nextMatch(_redex)) ||
      (_currentConstraint && 
       (!_currentConstraint->holds())))
    { 

      if (_swapQuerySides)
	{
	  _swapQuerySides = false;
	  const TERM* lit = _query->LitList()->firstLit();
	  if (!query(lit->Arg2(),lit->Arg1())) return false;
	  goto try_next_match;  
	}
      else return false;
    };



  ASSERT(_redex && _redex->isComplex());

  const TERM::OccList* occ = _redex->Occurences();   

  if (occ)
    {
      _candidates = occ->first();
      goto check_candidates;
    }
  else goto try_next_match; 
}; // nextSimplified() 
 
bool BackDemodulation::collectInference() 
{
  CALL("collectInference()");
  ASSERT(_candidates);
  if (rewrite(_candidates)) { _candidates = _candidates->next(); return true; };
  _candidates = _candidates->next();   
  return false; 
}; // bool collectInference() 




bool BackDemodulation::query(const TERM* lhsref,const TERM* rhsref) 
{
  CALL("query(const TERM* lhsref,const TERM* rhsref)");
  if (lhsref->isVariable() || rhsref->isVariable()) return false;
  ASSERT(lhsref->IsReference());
  ASSERT(rhsref->IsReference());
  _queryLHS = lhsref->First();
  ASSERT(_queryLHS->isComplex());
  _queryRHS = *rhsref;

  if (!_compiledConstraint)
    _compiledConstraint = SimplificationOrdering::current()->reserveCompiledConstraintForInstanceRetrieval();
  ASSERT(_compiledConstraint);
  _compiledConstraint->loadGreater(_queryLHS,_queryRHS.First());
  if ((!_compiledConstraint->lhsVarInstMayOrder()) ||
      (!appropriateDegree(_compiledConstraint->category())))
    {
      _currentConstraint = 
	(SimplificationOrdering::CompiledConstraintForInstanceRetrieval*)0;
      return false;
    };

  //DF; cout << "QUERY " << _queryLHS << "\n";

  if (_index.newQuery(_queryLHS))
    {
      _compiledConstraint->compile(_index.substitution());
      _currentConstraint = _compiledConstraint;
      return true;
    }
  else
    {

      //DF; cout << "CANCELLED\n";
      return false;
    };
}; // bool BackDemodulation::query(const TERM* lhsref,const TERM* rhsref) 




bool BackDemodulation::rewrite(const TERM::OccList::Node* occ)
{
  CALL("rewrite(const TERM::OccList::Node* occ)");

  _resolvent->Reset();
  TERM::LitNumMask* litMask = occ->value();
  ASSERT(litMask);
   
  ulong litNum = 0;
  for (LiteralList::Iterator iter(_recentlySimplified->LitList());iter.notEnd();iter.next())
    {
      if (litMask->contains(litNum)) // this literal contains rewritten redex       
	{
	  if (!rewriteLit(iter.currentLiteral(),iter.currentElement()->isInherentlySelected())) return false;
	}
      else // literal remains unchanhed 
	if (!_resolvent->PushLiteral(iter.currentLiteral(),iter.currentElement()->isInherentlySelected())) return false;      
      litNum++;
    };

  _resolvent->PushAncestor((Clause*)_query);
  _resolvent->PushAncestor((Clause*)_recentlySimplified);                
  _resolvent->UsedRule(ClauseBackground::BackDemod);  
  _resolvent->MakeVIP();                               
  return true;  
}; // bool BackDemodulation::rewrite(TERM::OccList::Node* occ)

bool BackDemodulation::rewriteLit(TERM* lit,bool inhSel)             
{
  CALL("rewriteLit(TERM* lit,bool inhSel)");
  ASSERT(_redex->isComplex());
  TERM header = *lit;
  if (header.IsEquality() && header.IsOrderedEq()) header.MakeUnordered(); 
 
  if (!_resolvent->OpenNonpropLiteral(header,inhSel)) return false; 
  _rewrittenLitIter.Reset(lit);
  ALWAYS(_rewrittenLitIter.Next());
 check_position:    
  if (_rewrittenLitIter.CurrPos() == _redex)
    {
      if (!collectRHSInstance()) return false;      
      if (_rewrittenLitIter.SkipTerm()) goto check_position;
    }
  else
    {
      if (!_resolvent->PushSymbol(_rewrittenLitIter.symbolRef())) return false;
      if (_rewrittenLitIter.Next()) goto check_position;
    };
  return _resolvent->CloseLiteral();
}; // bool BackDemodulation::rewriteLit(TERM* lit,bool inhSel)  



inline bool BackDemodulation::collectRHSInstance()
{
  CALL("collectRHSInstance()");
  _rhsInstance.reset(_queryRHS,_index.substitution());
  do
    {
      if (!_resolvent->PushSymbol(_rhsInstance.symbol())) return false;
    }
  while (_rhsInstance.next());
  return true;
}; // bool BackDemodulation::collectRHSInstance()
}; // namespace VK

//====================================================
