//
// File:         backward_superposition.cpp
// Description:  Classes dealing with backward superposition.
// Created:      May 06, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
// Revised:      May 09, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//               SimplificationOrdering::CompiledConstraintForUnifiers
//               replaced ORD_CONSTR
// Revised:      May 16, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//               Bug fix in RecollectMaxWeightForm().
//==================================================================
#include "backward_superposition.h"
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_BACKWARD_SUPERPOSITION
#define DEBUG_NAMESPACE "BACKWARD_SUPERPOSITION"
#endif
#include "debugMacros.h"
//================================================


namespace VK
{

bool BACKWARD_SUPERPOSITION::SetQuery(Clause* cl) 
{ 
  bpara_machine.setNoRulesFlag(_noInfBetweenRulesFlag && cl->IsRule());
  bpara_machine.SetQuery(cl);
  bpara_premise.Load(cl);
  _currentPosEq = bpara_premise.NextEq();
  if (!_currentPosEq) return false;
  bpara_machine.LoadEq(_currentPosEq,bpara_premise.GetIndex());
  return true; 
}; // bool BACKWARD_SUPERPOSITION::SetQuery(Clause* cl) 

bool BACKWARD_SUPERPOSITION::CollectResult()
{
  CALL("CollectResult()");
  if ((!bpara_premise.TheClause()->Active()) 
      || (!bpara_machine.candClause()->Active()))
    return false;
      
  if (bpara_premise.MinResultWeight()
      + bpara_machine.candClause()->MinResWeight()
      > _resolvent->MaxWeight())
    {
      _resolvent->AnotherTooHeavyClause();
      return false;
    };            
     
  _resolvent->Reset();
  if (bpara_machine.CollectLiterals() && bpara_premise.CollectLiterals())
    {
      _resolvent->UsedRule(ClauseBackground::BackSup);
      _resolvent->PushAncestor(bpara_premise.TheClause());
      _resolvent->PushAncestor(bpara_machine.candClause());
      return true;
    }
  else return false;
}; // bool BACKWARD_SUPERPOSITION::CollectResult()

bool BACKWARD_SUPERPOSITION::NextInference()
{
 process_literal:
  while (bpara_machine.Paramodulate())
    { 
      if (CollectResult()) return true;
    };
  // try next literal
  _currentPosEq = bpara_premise.NextEq();       
  if (!_currentPosEq) return false;
  bpara_machine.LoadEq(_currentPosEq,bpara_premise.GetIndex());
  goto process_literal;
}; // bool BACKWARD_SUPERPOSITION::NextInference()


}; // namespace VK

//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_BACKWARD_SUPERPOSITION
#define DEBUG_NAMESPACE "BPARA_PREMISE"
#endif
#include "debugMacros.h"
//================================================

namespace VK
{

inline void BPARA_PREMISE::CollectNonmaxWeightForm()
{
  CALL("CollectNonmaxWeightForm()");
  nonmax_weight_form.Reset();

  for (LiteralList::Iterator iter(allLiterals,firstNonmaxLit);
       iter.notEnd();
       iter.next())
    if (!iter.currentLiteral()->isAnswerLiteral())
      nonmax_weight_form.Compile(iter.currentLiteral());
}; // void BPARA_PREMISE::CollectNonmaxWeightForm()


inline void BPARA_PREMISE::RecollectMaxWeightForm()
{
  CALL("RecollectMaxWeightForm()");
  max_weight_form.Reset();

  for (LiteralList::Iterator iter(allLiterals); 
       iter.currentElement() != firstNonmaxLit;
       iter.next())
    {
      ASSERT(!iter.currentLiteral()->isAnswerLiteral());
      if (iter.currentElement() == _currentMaxLitElement)
	{              
	  ASSERT(iter.currentElement()->isEquality());
	  ASSERT(iter.currentElement()->positive());
	  if (iter.currentElement()->isOrderedEq())
	    max_weight_form.Compile(*(iter.currentElement()->arg2()));
	}
      else 
	max_weight_form.Compile(iter.currentElement()->literal());
    }; 
}; // void BPARA_PREMISE::RecollectMaxWeightForm()


inline void BPARA_PREMISE::Load(Clause* cl) 
{
  CALL("Load(Clause* cl)");
  clause = cl;
  allLiterals = cl->LitList();
  firstNonmaxLit = allLiterals->nth(allLiterals->numOfSelectedLits());
  maxLitIter.reset(allLiterals);
}; // void BPARA_PREMISE::Load(Clause* cl) 


inline const TERM* BPARA_PREMISE::NextEq()
{
  CALL("NextEq()");
  const TERM* lit;
 check_lit:
  if (maxLitIter.currentElement() == firstNonmaxLit) return (TERM*)0;  
  _currentMaxLitElement = maxLitIter.currentElement();
  lit = maxLitIter.currentLiteral();
  maxLitIter.next();
  if (lit->Positive() && lit->IsEquality()) 
    {
      RecollectMaxWeightForm();
      return lit;
    }; 
  goto check_lit;
}; //  const TERM* BPARA_PREMISE::NextEq()


inline bool BPARA_PREMISE::CollectLiterals() 
{
  CALL("CollectLiterals()");
  // collect max. literals except the used equality

  //cout << "REWR RULE " << maxLitIter.currentLiteral() << "\n";

  for (LiteralList::Iterator iter(allLiterals); iter.currentElement() != firstNonmaxLit;iter.next())
    if (iter.currentElement()->next() != maxLitIter.currentElement())
      {      

	//cout << "PUSH MAX " << iter.currentLiteral() << "\n";

	if (!_resolvent->PushStandardLit(index,iter.currentLiteral(),iter.currentElement()->isInherentlySelected())) return false; 

	//_resolvent->output(cout << "Success, RES3 = ") << "\n";

      };      

  // collect nonmax. literals
  for (LiteralList::Iterator iter(allLiterals,firstNonmaxLit);iter.notEnd();iter.next())
    {
      ASSERT(!(iter.currentElement()->isInherentlySelected()));
      if (!_resolvent->PushStandardLit(index,iter.currentLiteral(),false)) return false;   
    };  
  return true;
}; // bool BPARA_PREMISE::CollectLiterals() 


}; // namespace VK

//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_BACKWARD_SUPERPOSITION
#define DEBUG_NAMESPACE "BPARA_MACHINE"
#endif
#include "debugMacros.h"
//================================================= 

namespace VK
{

inline void BPARA_MACHINE::LoadEq(const TERM* eq_lit,ulong eq_ind)
{
  CALL("LoadEq(const TERM* eq_lit,ulong eq_ind)");
  ASSERT(eq_lit->IsEquality());
  ASSERT(eq_lit->Positive());
  eq_index = eq_ind; 
  const TERM* arg_ref = eq_lit->Args();
  PREFIX_SYM* mem = fst_arg_mem + 1;
  StandardTermToPrefix(eq_index,*arg_ref,mem);
  fst_arg_mem[0].SetBracket(mem);
     
  // another argument
  arg_ref = arg_ref->Next();
  mem = snd_arg_mem + 1;
  StandardTermToPrefix(eq_index,*arg_ref,mem);
  snd_arg_mem[0].SetBracket(mem);
     
  bpara_mach.LoadLHS(fst_arg_mem);
  rhs = snd_arg_mem + 1;
  unordered_eq = eq_lit->IsUnorderedEq();
  _swapArguments = unordered_eq;

  if (unordered_eq)
    {
      if (!_compiledConstraint)
	_compiledConstraint = SimplificationOrdering::current()->reserveCompiledConstraintForUnifiers();
      ASSERT(_compiledConstraint);
      _compiledConstraint->load(snd_arg_mem,fst_arg_mem);
      if (_compiledConstraint->rhsVarInstMayOrder())
	{
	  _compiledConstraint->compileGreaterOrEqual();
	  _currentConstraint = _compiledConstraint;
	}
      else
	{
	  _currentConstraint = 
	    (SimplificationOrdering::CompiledConstraintForUnifiers*)0;
	};
    }
  else
    {
      _currentConstraint = 
	(SimplificationOrdering::CompiledConstraintForUnifiers*)0;
    };
     
}; //  void BPARA_MACHINE::LoadEq(const TERM* eq_lit,ulong eq_ind)



inline bool BPARA_MACHINE::Paramodulate()
{
  CALL("Paramodulate()");
 try_next_cand:
  if (bpara_mach.NextCandidate(_currentConstraint,_candClause,_candLitRedexPair)) 
    {
      if (_noRulesFlag && _candClause->IsRule()) goto try_next_cand;
      return true;
    };
  if (_swapArguments)
    {
      _swapArguments = false;
      bpara_mach.LoadLHS(snd_arg_mem);
      rhs = fst_arg_mem + 1;


      if (!_compiledConstraint)
	_compiledConstraint = 
	  SimplificationOrdering::current()->reserveCompiledConstraintForUnifiers();
      ASSERT(_compiledConstraint);
      _compiledConstraint->load(fst_arg_mem,snd_arg_mem);
      if (_compiledConstraint->rhsVarInstMayOrder())
	{
	  _compiledConstraint->compileGreaterOrEqual();
	  _currentConstraint = _compiledConstraint;
	}
      else
	{
	  _currentConstraint = 
	    (SimplificationOrdering::CompiledConstraintForUnifiers*)0;
	};

      goto try_next_cand;          
    };
  return false; 
}; // bool BPARA_MACHINE::Paramodulate()

inline bool BPARA_MACHINE::CollectLiterals() 
{
  CALL("CollectLiterals()");

  const SimplificationOrdering::StoredConstraint* constr = 
    _candLitRedexPair->constraint();
  if (constr) 
    {
      if (!_constraintCheck)
	_constraintCheck = SimplificationOrdering::current()->reserveConstraintCheckOnUnifier();
      ASSERT(_constraintCheck);
      if (_constraintCheck->holds(constr,index))
	return false;
    };
    
  ulong rewritten_lit_num = _candLitRedexPair->litNum();

  LiteralList::Element* rewritten_lit = _candClause->LitList()->nth(rewritten_lit_num);
  for (LiteralList::Iterator iter(_candClause->LitList());iter.notEnd();iter.next())
    {
      if (iter.currentElement() == rewritten_lit)
	{        
	  if (!_resolvent->PushRewrittenLit(index,iter.currentLiteral(),_candLitRedexPair->redex(),rhs,iter.currentElement()->isInherentlySelected()))
	    return false;
	}
      else
	{
	  if ((_simultaneousness > 0) && (iter.currentElement()->arity()))
	    {
	      if (!_resolvent->PushRewrittenLit(index,iter.currentLiteral(),_candLitRedexPair->redex(),rhs,iter.currentElement()->isInherentlySelected()))
		return false;   
	    }
	  else
	    if (!_resolvent->PushStandardLit(index,iter.currentElement()->literal(),iter.currentElement()->isInherentlySelected())) return false;  
	};  
    };    

  RARE((query->PreorderedPositiveUnitEquality() 
	&& (!query->flag(Clause::FlagContainsAnswerLiterals))
	&& (!bpara_mach.VariablesFromIndexAffected())
	&& (query != _candClause)));      

  return true;
};  // bool BPARA_MACHINE::CollectLiterals()

}; // namespace VK
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_BACKWARD_SUPERPOSITION
#define DEBUG_NAMESPACE "BPARA_MACH"
#endif
#include "debugMacros.h"
//=================================================

namespace VK
{

            
inline 
bool 
BPARA_MACH::NextCandidate(SimplificationOrdering::CompiledConstraintForUnifiers* constr,
			  Clause*& candCl,
			  BackParaCandidates::LitRedexPair*& candLitRedexPair)
{
  CALL("NextCandidate(..)"); 

  if (init_state)
    {
      init_state = false;
      if (para_mach.FindFirstLeaf(*(_unifIndex->BParaTree())))
	{	       
	  if (constr && constr->holdsOnCurrentUnifier()) goto try_next_leaf;
	  _candidates.reset(*(para_mach.BPCandidatesFromLeaf()));            
	  ALWAYS(_candidates.next(candCl,candLitRedexPair));
	  return true;
	}
      else return false;
    }
  else // !init_state
    {
      if (_candidates.next(candCl,candLitRedexPair)) 
	{
	  return true;
	};
    try_next_leaf: 

      if (para_mach.FindNextLeaf())
	{	             
	  if (constr && constr->holdsOnCurrentUnifier()) goto try_next_leaf;
	  _candidates.reset(*(para_mach.BPCandidatesFromLeaf()));            
	  ALWAYS(_candidates.next(candCl,candLitRedexPair));
	  return true;
	}
      else 
	{
	  return false;  
	};
    };     
}; // bool BPARA_MACH::NextCandidate(..) 

}; // namespace VK

//==================================================================
