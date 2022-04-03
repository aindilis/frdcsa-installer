//
// File:         ForwardDemodulation.cpp
// Description:  
// Created:      ???, Alexandre Riazanov, riazanov@cs.man.ac.uk
// Revised:      Apr 29, 2001, Alexandre Riazanov, riazanov@cs.man.ac.uk
//               KBConstraint and SharedKBConstraint are no longer used. 
//               Generic SimplificationOrdering is used instead. 
//====================================================
#include "ForwardDemodulation.h"
#include "Clause.h"
#include "SimplificationOrdering.h"
//=================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_DEMODULATION
#define DEBUG_NAMESPACE "ForwardDemodulation"
#endif 
#include "debugMacros.h"
//=================================================

using namespace VK;
using namespace BK;
using namespace Gematogen;

#ifdef DEBUG_ALLOC_OBJ_TYPE
#include "ClassDesc.h"
ClassDesc ForwardMatchingIndexTreeInstruction<VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables,GlobAlloc,TERM,Flatterm,ForwardDemodulation::Index::LeafInfo,ForwardDemodulation::Index>::_classDesc("ForwardMatchingIndexInstruction<VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables,GlobAlloc,TERM,Flatterm,ForwardDemodulation::Index::LeafInfo,ForwardDemodulation::Index>");
 
ClassDesc ForwardDemodulation::Index::StoredSubstitution::_classDesc("ForwardDemodulation::Index::StoredSubstitution",
								     ForwardDemodulation::Index::StoredSubstitution::minSize(),
								     ForwardDemodulation::Index::StoredSubstitution::maxSize());
ClassDesc ForwardDemodulation::Index::ConstrRuleList::_classDesc("ForwardDemodulation::Index::ConstrRuleList",
								 ForwardDemodulation::Index::ConstrRuleList::minSize(),
								 ForwardDemodulation::Index::ConstrRuleList::maxSize());

#endif


void ForwardDemodulation::Integrate(Clause* cl) 
{ 
  if (CanBeRewritingRule(cl)) index.Integrate(cl); 
}; 

void ForwardDemodulation::Remove(Clause* cl) 
{ 
  if (CanBeRewritingRule(cl)) index.Remove(cl); 
};


bool ForwardDemodulation::RewriteByConstrained(const Flatterm* query,const SetOfSplittingBranches& splittingBranches)
{
  CALL("RewriteByConstrained(const Flatterm* query,const SetOfSplittingBranches& splittingBranches)");

  ASSERT(query->Symbol().isComplex());


  for (ForwardMatchingIndexTreeLeaf<VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables,GlobAlloc,TERM,Flatterm,Index::LeafInfo,Index>* 
	 leaf = index.Match(query); 
       leaf; 
       leaf = index.MatchAgain())
    {
      ASSERT(leaf->isLeaf());  
      Index::ConstrRuleList* crl =  leaf->leafInfo().rules(); 
      ASSERT(crl);  
      do
	{
#ifdef DEBUG_ALLOC
	  ASSERT(crl->checkObj());
#endif
	  rule = crl->hd().clause;
	  if (rule->Active())
	    {
	      // check the constraint
	      SimplificationOrdering::StoredConstraint* constr = crl->hd().constr;
	      collectSubstCache(crl->hd().subst);

	      if (!constr) // preordered 
		{
		  ASSERT(rule->PreorderedPositiveUnitEquality() || fDOnSplittingBranchesFlag);
		  ASSERT(crl->hd().lhs == rule->LitList()->firstLit()->Arg1()->First()); 

		  if (CheckSplittingBranches(rule,splittingBranches))
		    {
		      TERM rule_rhs_term = *(rule->LitList()->firstLit()->Arg2());
		      _ruleRHSInstance.reset(rule_rhs_term,&_substCache);
		      return true;
		    };
		}
	      else
		{
		  // constrained rule, check the constraint and splitting branches
		  if ((checkConstraint(constr)) && 
		      (CheckSplittingBranches(rule,splittingBranches)))
		    {
		      const TERM* lhs = crl->hd().lhs;
		      ASSERT(lhs);
		      ASSERT(lhs->isComplex());
		      TERM rule_rhs_term;
		      if (rule->LitList()->firstLit()->Arg1()->First() == lhs)
			{
			  rule_rhs_term = *(rule->LitList()->firstLit()->Arg2());
			}          
		      else
			{
			  ASSERT(rule->LitList()->firstLit()->Arg2()->First() == lhs);
			  rule_rhs_term = *(rule->LitList()->firstLit()->Arg1());
			  ASSERT(rule_rhs_term.First()->isComplex());
			};

		      _ruleRHSInstance.reset(rule_rhs_term,&_substCache);  

		      return true;      
		    };              
		};
	    };
	  crl = crl->tl();
	}
      while (crl);
    };
  return false;      
}; // bool ForwardDemodulation::RewriteByConstrained(const Flatterm* query,const SetOfSplittingBranches& splittingBranches) 

ForwardDemodulation::Index::ConstrRuleList* ForwardDemodulation::removeConstrRule(Clause* cl,TERM* t,Index::ConstrRuleList* lst)
{
  CALL("removeConstrRule(Clause* cl,TERM* t,Index::ConstrRuleList* lst)");
  if (!lst) return (Index::ConstrRuleList*)0;
  if ((lst->hd().clause == cl) && (lst->hd().lhs == t))
    {
      Index::ConstrRuleList* res = lst->tl();
   
      //Index::StoredSubstitution::destroyList(lst->hd().subst);

      delete lst;
      return res;
    };
  lst->tl() = removeConstrRule(cl,t,lst->tl());
  return lst;
}; // ForwardDemodulation::Index::ConstrRuleList* ForwardDemodulation::removeConstrRule(Clause* cl,TERM* t,Index::ConstrRuleList* lst)

ForwardDemodulation::Index::ConstrRuleList* 
ForwardDemodulation::insertConstrRule(Clause* cl,
				      SimplificationOrdering::StoredConstraint* constr,
				      Index::StoredSubstitution* subst,
				      TERM* t,
				      Index::ConstrRuleList* lst)
{
  CALL("insertConstrRule(..)");
  //return new Index::ConstrRuleList(Index::ConstrRule(cl,constr,subst,t),lst); 
  Index::ConstrRuleList* res = new Index::ConstrRuleList(lst); 
  res->hd().init(cl,constr,subst,t); 
  return res;
}; // ForwardDemodulation::Index::ConstrRuleList* ForwardDemodulation::insertConstrRule(..)

void ForwardDemodulation::Destroy() 
{
  CALL("Destroy()");
  index.Destroy(); 
  if (_constrCheck)
    {
      SimplificationOrdering::current()->releaseConstraintCheckOnForwardMatchingSubst(_constrCheck);
      _constrCheck = (SimplificationOrdering::ConstraintCheckOnForwardMatchingSubst*)0;
    };
}; // void ForwardDemodulation::Destroy() 

inline bool ForwardDemodulation::CanBeRewritingRule(const Clause* cl)
{
  CALL("CanBeRewritingRule(const Clause* cl)");
  // cl is either unit positive equality or a clause with exactly one selected positive equality literal
  // and a number of literals that are splitting branches. 
  // At the momemnt no answer literals are allowed in rewriting rules.
  if (cl->flag(Clause::FlagContainsAnswerLiterals)) return false;
  TERM* lit; 
  if (cl->Unit())
    {
      lit = cl->LitList()->firstLit();
      return (lit->Positive() && lit->IsEquality());
    }
  else  
    {
      if (!fDOnSplittingBranchesFlag) return false;
      lit = cl->LitList()->firstLit();
      if (lit->Negative() || (!lit->IsEquality()) || (cl->LitList()->numOfSelectedLits() != 1)) return false;
      LiteralList::Iterator iter(cl->LitList());
      iter.next();
      ASSERT(iter.notEnd());
      do
	{
	  if (!iter.currentLiteral()->IsSplitting()) return false;
	  iter.next();
	}
      while (iter.notEnd());
      return true;
    };
}; // bool ForwardDemodulation::CanBeRewritingRule(const Clause* cl) 


inline bool ForwardDemodulation::CheckSplittingBranches(const Clause* cl,const SetOfSplittingBranches& splittingBranches)
{
  CALL("CheckSplittingBranches(const Clause* cl,const SetOfSplittingBranches& splittingBranches)");
  if ((!fDOnSplittingBranchesFlag) || cl->Unit())
    {
      ASSERT(cl->Unit());
      return true; 
    }; 
  LiteralList::Iterator iter(cl->LitList());
  iter.next(); 
  while (iter.notEnd()) 
    {
      if (!splittingBranches.safeScoreOf(iter.currentLiteral()->HeaderNum())) return false;
      iter.next();
    };
  branchesUsed = true;
  return true;
}; // bool ForwardDemodulation::CheckSplittingBranches(const Clause* cl,const SetOfSplittingBranches& splittingBranches)



inline
bool 
ForwardDemodulation::checkConstraint(const SimplificationOrdering::StoredConstraint* constr)
{
  CALL("checkConstraint(..)");
  ASSERT(constr);
  if (!_constrCheck)
    _constrCheck = SimplificationOrdering::current()->reserveConstraintCheckOnForwardMatchingSubst();
  ASSERT(_constrCheck);
  return _constrCheck->holds(constr,&_substCache);
}; // bool ForwardDemodulation::checkConstraint(const SimplificationOrdering::StoredConstraint* constr)


inline
void 
ForwardDemodulation::collectSubstCache(const ForwardDemodulation::Index::StoredSubstitution* storedSubst)
{
  CALL("collectSubstCache(const ForwardDemodulation::Index::StoredSubstitution* storedSubst)");
  _substCache.reset(); 
  while (storedSubst) 
    {
#ifdef DEBUG_ALLOC
  ASSERT(storedSubst->checkObj());
#endif
      _substCache.associate(storedSubst->hd().el1,*(storedSubst->hd().el2));
      storedSubst = storedSubst->tl();
    }; 
  _substCache.endOfCollection();
}; // void ForwardDemodulation::collectSubstCache(const ForwardDemodulation::Index::StoredSubstitution* storedSubst)


//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_DEMODULATION
#define DEBUG_NAMESPACE "ForwardDemodulation::Index"
#endif 
#include "debugMacros.h"
//=================================================


void ForwardDemodulation::Index::IntegrateConstrained(Clause* cl)
{
  CALL("IntegrateConstrained(Clause* cl)");

#ifdef DEBUG_NAMESPACE
  if (cl->numOfNonanswerLiterals() > 1)
    {
      LiteralList::Iterator iter(cl->LitList());
      iter.next(); 
      do
	{
	  if ((!iter.currentLiteral()->IsSplitting()) &&
	      (!iter.currentLiteral()->isAnswerLiteral()))
	    {
	      ERROR("E1");
	    };
	  iter.next();
	}
      while (iter.notEnd());
    };  
#endif          
  
  ASSERT(cl->LitList()->numOfSelectedLits() == 1); 

  TERM* lit = cl->LitList()->firstLit();

  ASSERT(lit->Positive());
  ASSERT(lit->IsEquality());


  if (lit->IsOrderedEq())
    {
      ASSERT(lit->Args()->IsReference());
      TERM* lhs = lit->Args()->First();
      ForwardMatchingIndexTreeLeaf<VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables,GlobAlloc,TERM,Flatterm,Index::LeafInfo,Index>* leaf = Integrate(lhs); 
      INTERCEPT_FORWARD_MATCHING_INDEXING_INTEGRATE(lhs);    

      Index::ConstrRuleList* crl = leaf->leafInfo().rules();   

#ifdef DEBUG_ALLOC
      ASSERT((!crl) || crl->checkObj());
#endif


      crl = new Index::ConstrRuleList(crl); 

      crl->hd().init(cl,
		     (SimplificationOrdering::StoredConstraint*)0,
		     createStoredSubstitution(),
		     lhs);

      leaf->leafInfo().rules() = crl;
    }      
  else
    {
      TERM* arg1ref = lit->Args();
      TERM* arg2ref = arg1ref->Next();
      if ((arg1ref->isVariable()) || (arg2ref->isVariable())) return;        
    
      SimplificationOrdering::SpecialisedComparison* specialisedArgComparison = 
	SimplificationOrdering::current()->reserveSpecialisedComparison();
      
      ASSERT(specialisedArgComparison);
      specialisedArgComparison->load(*arg1ref,*arg2ref);
      if (specialisedArgComparison->lhsVarInstMayOrder())
	{
	  specialisedArgComparison->specialiseGreater();
	  if (appropriateDegreeForGreater(specialisedArgComparison->category()))
	    {
	      SimplificationOrdering::StoredConstraint* constraint = 
		SimplificationOrdering::current()->storedConstraintGreater(specialisedArgComparison);
	      if (constraint)
		{
		  IntegrateConstrained(cl,arg1ref->First(),constraint);
		  SimplificationOrdering::current()->releaseConstraint(constraint);
		};
	    };

	};
      if (specialisedArgComparison->rhsVarInstMayOrder())
	{
	  specialisedArgComparison->specialiseLess();
	  if (appropriateDegreeForLess(specialisedArgComparison->category()))
	    {
	      SimplificationOrdering::StoredConstraint* constraint = 
		SimplificationOrdering::current()->storedConstraintLess(specialisedArgComparison);
	      if (constraint)
		{
		  IntegrateConstrained(cl,arg2ref->First(),constraint);
		  SimplificationOrdering::current()->releaseConstraint(constraint);
		};
	    };
	};

      SimplificationOrdering::current()->releaseSpecialisedComparison(specialisedArgComparison);
    };
}; // void ForwardDemodulation::Index::IntegrateConstrained(Clause* cl)

void ForwardDemodulation::Index::IntegrateConstrained(Clause* cl,
						      TERM* t,
						      SimplificationOrdering::StoredConstraint* constr)
{
  CALL("IntegrateConstrained(Clause* cl,..)");
  
  ForwardMatchingIndexTreeLeaf<VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables,GlobAlloc,TERM,Flatterm,Index::LeafInfo,Index>* 
    leaf = Integrate(t);
  INTERCEPT_FORWARD_MATCHING_INDEXING_INTEGRATE(t);

  Index::ConstrRuleList* crl = leaf->leafInfo().rules();  

#ifdef DEBUG_ALLOC
      ASSERT((!crl) || crl->checkObj());
#endif 
  crl = insertConstrRule(cl,constr,createStoredSubstitution(),t,crl);

  leaf->leafInfo().rules() = crl;
}; //  void ForwardDemodulation::Index::IntegrateConstrained(Clause* cl,..)


void ForwardDemodulation::Index::RemoveConstrained(Clause* cl) 
{ 
  CALL("RemoveConstrained(Clause* cl)"); 
  
#ifdef DEBUG_NAMESPACE
  if (cl->numOfNonanswerLiterals() > 1)
    {
      LiteralList::Iterator iter(cl->LitList());
      iter.next(); 
      do
	{
	  if ((!iter.currentLiteral()->IsSplitting()) &&
	      (!iter.currentLiteral()->isAnswerLiteral()))
	    {
	      ERROR("E1");
	    };
	  iter.next();
	}
      while (iter.notEnd());
    };  
#endif          
  
  ASSERT(cl->LitList()->numOfSelectedLits() == 1); 

  TERM* lit = cl->LitList()->firstLit();

  ASSERT(lit->Positive());
  ASSERT(lit->IsEquality());

  if (lit->IsOrderedEq())
    { 
      RemoveConstrained(cl,lit->Args()->First());
    }
  else
    {       
      TERM* arg1ref = lit->Arg1();
      TERM* arg2ref = lit->Arg2();
      if (arg1ref->isVariable() || arg2ref->isVariable()) return;
      TERM* arg1 = arg1ref->First();
      TERM* arg2 = arg2ref->First();
      RemoveConstrained(cl,arg1);
      RemoveConstrained(cl,arg2);    
    };  
}; // void ForwardDemodulation::Index::RemoveConstrained(Clause* cl)
 
void ForwardDemodulation::Index::RemoveConstrained(Clause* cl,TERM* lhs) 
{ 
  CALL("RemoveConstrained(Clause* cl,TERM* lhs)");
  ForwardMatchingIndexTreeLeaf<VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables,GlobAlloc,TERM,Flatterm,Index::LeafInfo,Index>* leaf;
  ForwardMatchingIndexTreeInstruction<VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables,GlobAlloc,TERM,Flatterm,Index::LeafInfo,Index>** branch_to_destroy;
  if (Remove(lhs,leaf,branch_to_destroy))
    { 
      ASSERT(leaf->isLeaf());
      INTERCEPT_FORWARD_MATCHING_INDEXING_REMOVE(lhs);
      Index::ConstrRuleList* crl = leaf->leafInfo().rules();      
      ASSERT(crl);   
      crl = removeConstrRule(cl,lhs,crl);
      leaf->leafInfo().rules() = crl;
      if (!crl) // the branch must be destroyed
	{
	  ForwardMatchingIndexTree<VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables,GlobAlloc,TERM,Flatterm,Index::LeafInfo,Index>::DestroyBranch(branch_to_destroy);
	};
    };
}; // void ForwardDemodulation::Index::RemoveConstrained(Clause* cl,TERM* lhs)



inline bool ForwardDemodulation::Index::appropriateDegreeForGreater(const SimplificationOrdering::SpecialisedComparison::Category& cat) const 
{
  CALL("appropriateDegreeForGreater(const SimplificationOrdering::SpecialisedComparison::Category& cat) const");
  switch (cat)
    {
    case SimplificationOrdering::SpecialisedComparison::AlwaysLess:
      // same as below
    case SimplificationOrdering::SpecialisedComparison::AlwaysEqual:
      return false;
    case SimplificationOrdering::SpecialisedComparison::AlwaysGreater:
      return true;
    case SimplificationOrdering::SpecialisedComparison::CanBeLessOrEqual:
      return (degree >= 4);
    case SimplificationOrdering::SpecialisedComparison::CanBeGreaterOrEqual:
      return (degree >= 2);
    case SimplificationOrdering::SpecialisedComparison::VolatileButNearlyAlwaysLess:
      return (degree >= 4);
    case SimplificationOrdering::SpecialisedComparison::VolatileButNearlyAlwaysGreater:
      return (degree >= 2);
    case SimplificationOrdering::SpecialisedComparison::AbsolutelyVolatile:
      return (degree >= 3);
    };

  ICP("ICP1");

#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE)
  return false;
#endif
}; // bool ForwardDemodulation::Index::appropriateDegreeForGreater(const SimplificationOrdering::SpecialisedComparison::Category& cat) const 



inline bool ForwardDemodulation::Index::appropriateDegreeForLess(const SimplificationOrdering::SpecialisedComparison::Category& cat) const 
{
  CALL("appropriateDegreeForLess(const SimplificationOrdering::SpecialisedComparison::Category& cat) const");
  switch (cat)
    {
    case SimplificationOrdering::SpecialisedComparison::AlwaysLess:
      return true;
    case SimplificationOrdering::SpecialisedComparison::AlwaysEqual:
      // same as below
    case SimplificationOrdering::SpecialisedComparison::AlwaysGreater:
      return false;
    case SimplificationOrdering::SpecialisedComparison::CanBeLessOrEqual:
      return (degree >= 2);
    case SimplificationOrdering::SpecialisedComparison::CanBeGreaterOrEqual:
      return (degree >= 4);
    case SimplificationOrdering::SpecialisedComparison::VolatileButNearlyAlwaysLess:
      return (degree >= 2);
    case SimplificationOrdering::SpecialisedComparison::VolatileButNearlyAlwaysGreater:
      return (degree >= 4);
    case SimplificationOrdering::SpecialisedComparison::AbsolutelyVolatile:
      return (degree >= 3);
    };

  ICP("ICP1");

#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE)
  return false;
#endif
}; // bool ForwardDemodulation::Index::appropriateDegreeForLess(const SimplificationOrdering::SpecialisedComparison::Category& cat) const 


inline ForwardMatchingIndexTreeLeaf<VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables,GlobAlloc,TERM,Flatterm,ForwardDemodulation::Index::LeafInfo,ForwardDemodulation::Index>* ForwardDemodulation::Index::Integrate(TERM* t)
{
  CALL("Integrate(TERM* t)");
  ASSERT(t->isComplex()); 
  _matchingIndex.integrationReset(*t);
  TERM::Iterator iter(t); 
  while (iter.Next()) 
    if (iter.CurrentSymIsVar()) 
      { _matchingIndex.integrationVar(iter.symbolRef()); }
    else _matchingIndex.integrationFunc(iter.symbolRef());

  bool newLeaf;
  ForwardMatchingIndexTreeLeaf<VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables,GlobAlloc,TERM,Flatterm,ForwardDemodulation::Index::LeafInfo,Index>* res =  _matchingIndex.integrationGetLeaf(newLeaf);
  if (newLeaf) res->leafInfo().init();
  return res;
}; // ForwardMatchingIndexTreeLeaf<VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables,GlobAlloc,TERM,Flatterm,ForwardDemodulation::Index::LeafInfo,ForwardDemodulation::Index>* ForwardDemodulation::Index::Integrate(TERM* t)

inline bool ForwardDemodulation::Index::Remove(TERM* t,ForwardMatchingIndexTreeLeaf<VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables,GlobAlloc,TERM,Flatterm,ForwardDemodulation::Index::LeafInfo,ForwardDemodulation::Index>*& leaf,ForwardMatchingIndexTreeInstruction<VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables,GlobAlloc,TERM,Flatterm,ForwardDemodulation::Index::LeafInfo,ForwardDemodulation::Index>**& branch_to_destroy)
{
  CALL("Remove(TERM* t,ForwardMatchingIndexTreeLeaf<VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables,GlobAlloc,TERM,Flatterm,ForwardDemodulation::Index::LeafInfo,ForwardDemodulation::Index>*& leaf,ForwardMatchingIndexTreeInstruction<VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables,GlobAlloc,TERM,Flatterm,ForwardDemodulation::Index::LeafInfo,ForwardDemodulation::Index>**& branch_to_destroy)");
  if (!_matchingIndex.removalReset(*t)) return false;
  TERM::Iterator iter(t);
  while (iter.Next())  
    {    
      if (iter.CurrentSymIsVar())
	{
	  if (!_matchingIndex.removalVar(iter.symbolRef())) return false;
	}
      else if (!_matchingIndex.removalFunc(iter.symbolRef())) return false;
    };

  return _matchingIndex.removalFinish(leaf,branch_to_destroy);
}; // bool ForwardDemodulation::Index::Remove(TERM* t,ForwardMatchingIndexTreeLeaf<VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables,GlobAlloc,TERM,Flatterm,ForwardDemodulation::Index::LeafInfo,ForwardDemodulation::Index>*& leaf,ForwardMatchingIndexTreeInstruction<VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables,GlobAlloc,TERM,Flatterm,ForwardDemodulation::Index::LeafInfo,ForwardDemodulation::Index>**& branch_to_destroy) 


inline void ForwardDemodulation::Index::Integrate(Clause* cl)
{
  IntegrateConstrained(cl);
};

inline void ForwardDemodulation::Index::Remove(Clause* cl)
{
  RemoveConstrained(cl);
};    

inline ForwardMatchingIndexTreeLeaf<VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables,GlobAlloc,TERM,Flatterm,ForwardDemodulation::Index::LeafInfo,ForwardDemodulation::Index>* ForwardDemodulation::Index::Match(const Flatterm* query) 
{ 
  return INTERCEPT_FORWARD_MATCHING_INDEXING_RETRIEVAL_RESULT(_matchingIndex.Match(INTERCEPT_FORWARD_MATCHING_INDEXING_RETRIEVAL_QUERY(query))); 
}; 

inline ForwardMatchingIndexTreeLeaf<VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables,GlobAlloc,TERM,Flatterm,ForwardDemodulation::Index::LeafInfo,ForwardDemodulation::Index>* ForwardDemodulation::Index::MatchAgain() 
{ 
  return _matchingIndex.MatchAgain();    
}; 

inline
ForwardDemodulation::Index::StoredSubstitution* 
ForwardDemodulation::Index::createStoredSubstitution()
{
  CALL("createStoredSubstitution()");
  StoredSubstitution* result = (StoredSubstitution*)0;
  _matchingIndex.integrationGetVariables().ResetIterator();
  for (const ulong* v = _matchingIndex.integrationGetVariables().NextVar(); 
       v; 
       v = _matchingIndex.integrationGetVariables().NextVar())
    {
      result = 
	new StoredSubstitution(VarInstanceLocationPair(*v,_matchingIndex.integrationMapVariable(*v)),
			       result);
    };
  return result;
}; // ForwardDemodulation::Index::StoredSubstitution* ForwardDemodulation::Index::createStoredSubstitution()


//=================================================














