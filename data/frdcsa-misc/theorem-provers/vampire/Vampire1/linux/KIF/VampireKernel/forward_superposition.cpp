//
// File:         forward_superposition.cpp
// Description:  Representation of stored terms.
// Created:      Feb 14, 2000, Alexandre Riazanov, riazanov@cs.man.ac.uk
// Revised:      Dec 04, 2001, Alexandre Riazanov, riazanov@cs.man.ac.uk 
//               The array FPARA_PREMISE::_inherentlySelected
//               has been made extendable.
// Revised:      Apr 28, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//               SimplificationOrdering::StoredConstraint 
//               replaced SharedKBConstr
// Revised:      May 09, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//               SimplificationOrdering::CompiledConstraintForUnifiers
//               replaced ORD_CONSTR
//====================================================
#include "forward_superposition.h"

//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_SUPERPOSITION
#define DEBUG_NAMESPACE "FORWARD_SUPERPOSITION"
#endif
#include "debugMacros.h"
//=================================================

using namespace VK;

bool FORWARD_SUPERPOSITION::Load(Clause* cl)
{
  CALL("Load(Clause* cl)");
  fpara_premise.Load(cl);
  fpara_machine.setNoRulesFlag(_noInfBetweenRulesFlag && cl->IsRule());
  PREFIX_SYM* lit;
  bool inherentlySelected;
 try_next_lit:
  lit = fpara_premise.NextMaxLit(inherentlySelected);
  if (!lit) return false;
  if (fpara_machine.LoadLiteral(lit,inherentlySelected)) { _continuation = ContFSup; return true; };
  goto try_next_lit; 
}; // bool FORWARD_SUPERPOSITION::Load(Clause* cl)
 
bool FORWARD_SUPERPOSITION::NextFSupInference()
{
  if (fpara_machine.Paramodulate()) return true;  
  PREFIX_SYM* lit;
  bool inherentlySelected;
 try_next_lit:
  lit = fpara_premise.NextMaxLit(inherentlySelected);
  if (!lit) return false;
  if ((fpara_machine.LoadLiteral(lit,inherentlySelected)) && (fpara_machine.Paramodulate())) 
    return true;
  goto try_next_lit; 
}; // bool FORWARD_SUPERPOSITION::NextFSupInference()

bool FORWARD_SUPERPOSITION::NextEqRes()
{
  PREFIX_SYM* lit; 
 try_next_lit:
  lit = fpara_premise.NextMaxLitMem();
  if (!lit) return false;
      
  TERM header =  lit->Head().Func();
  if (header.Negative() && header.IsEquality() && header.IsUnorderedEq())
    {
      PREFIX_SYM* arg1 = lit+1;
      PREFIX_SYM* arg2 = arg1->Bracket();
      if (UNIFICATION::Unify(arg1,arg2)) return true;
    };
  goto try_next_lit; 
}; // bool FORWARD_SUPERPOSITION::NextEqRes()

bool FORWARD_SUPERPOSITION::CollectResult()
{
  CALL("CollectResult()");
  if (!fpara_premise.Query()->Active()) return false;
  Clause* rule = fpara_machine.Candidate()->TheClause(); 
  if (!rule->Active()) return false;
 

  const SimplificationOrdering::StoredConstraint* constr = 
    fpara_machine.Candidate()->orderingConstraint();
  if (constr)
    {
      if (!_constraintCheck)
	_constraintCheck = SimplificationOrdering::current()->reserveConstraintCheckOnUnifier();
      ASSERT(_constraintCheck);
      if (_constraintCheck->holds(constr,fpara_machine.GetIndex()))
	return false;
    };


  if (fpara_premise.MinResultWeight() 
      + rule->MinResWeight() 
      > _resolvent->MaxWeight())
    {
      _resolvent->AnotherTooHeavyClause();
      return false;
    }; 
      
  _resolvent->Reset();
  if (fpara_premise.CollectLiterals() && fpara_machine.CollectResult())
    {
      //_resolvent->SetBackground(ClauseBackground::RESOL);
      _resolvent->UsedRule(ClauseBackground::ForwSup);
      _resolvent->PushAncestor(fpara_premise.Query());
      _resolvent->PushAncestor(rule);
      return true;
    }
  else return false; 
}; // bool FORWARD_SUPERPOSITION::CollectResult()   
 
bool FORWARD_SUPERPOSITION::CollectEqResResult()
{
  CALL("CollectEqResResult()");
  if (!fpara_premise.Query()->Active()) return false;
  _resolvent->Reset();
  if (fpara_premise.CollectLiterals())
    {
      //_resolvent->SetBackground(ClauseBackground::RESOL);
      _resolvent->UsedRule(ClauseBackground::EqRes);
      _resolvent->PushAncestor(fpara_premise.Query());
      return true;
    }
  else return false; 
}; // bool FORWARD_SUPERPOSITION::CollectEqResResult() 



//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_SUPERPOSITION
#define DEBUG_NAMESPACE "FPARA_PREMISE"
#endif
#include "debugMacros.h"
//=================================================
void FPARA_PREMISE::Load(Clause* cl)
{
  CALL("Load(Clause* cl)");

  DOP(_maxLiterals.unfreeze());
  _maxLiterals.expand(cl->numOfAllLiterals());
  DOP(_maxLiterals.freeze());   


  DOP(_inherentlySelected.unfreeze());
  _inherentlySelected.expand(cl->numOfAllLiterals());
  DOP(_inherentlySelected.freeze());   


  eq_res_applicable = false;
  clause = cl;
  _allLiterals = cl->LitList();
  num_of_max_literals = _allLiterals->numOfSelectedLits();
  _firstNonselectedLit = _allLiterals->nth(num_of_max_literals);


  LiteralList::Iterator iter(_allLiterals);  
  _maxLiterals.reset();
  _freeMaxLitMemory = max_literal;
  for (ulong ln = 0; ln < num_of_max_literals; ln++)
    {
      ASSERT(iter.notEnd());
      _maxLiterals.push(_freeMaxLitMemory);
      ::StandardTermToPrefix(index,iter.currentLiteral(),_freeMaxLitMemory);
      ASSERT(_freeMaxLitMemory - max_literal <= (long)VampireKernelConst::MaxClauseSize);
      _inherentlySelected[ln] = iter.currentElement()->isInherentlySelected();
      if ((iter.currentLiteral()->Negative())
	  && (iter.currentLiteral()->IsEquality())
	  && (iter.currentLiteral()->IsUnorderedEq()))
	eq_res_applicable = true;
       
      iter.next();
    };
     
  CreateWeightFormula();
     
  ResetIter();
}; // void FPARA_PREMISE::Load(Clause* cl) 

bool FPARA_PREMISE::CollectLiterals()
{
  CALL("CollectLiterals()");
  // all the selected literals except the rewritten one
  for (ulong ln = num_of_max_literals; ln; ln--)
    if ((ln - 1) != next_max_lit)
      if (!_resolvent->PushPrefixLit(MaxLitMem(ln-1),_inherentlySelected[ln-1])) return false;     
     
  // all nonselected
  for(LiteralList::Iterator iter(_allLiterals,_firstNonselectedLit);iter.notEnd();iter.next())
    {
      ASSERT(!(iter.currentElement()->isInherentlySelected()));
      if (!_resolvent->PushStandardLit(index,iter.currentLiteral(),false)) return false; 
    }; 
  return true; 
}; // bool FPARA_PREMISE::CollectLiterals()

//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_SUPERPOSITION
#define DEBUG_NAMESPACE "FPARA_MACHINE"
#endif
#include "debugMacros.h"
//=================================================

bool FPARA_MACHINE::LoadLiteral(PREFIX_SYM* lit,bool inhSel)
{
  CALL("LoadLiteral(PREFIX_SYM* lit,bool inhSel)");

  if (!lit->Head().Func().arity()) return false; // propositional
  // here lit must be a plain nonpropositional literal with all brackets set
  literal = lit;
  complex_positions.reset();
  if (lit->Head().Func().IsEquality())
    {
      _eqArg1 = lit + 1;
      _eqArg2 = _eqArg1->Bracket();

      if (lit->Head().Func().IsOrderedEq())
	{
	  _currentConstraint = (SimplificationOrdering::CompiledConstraintForUnifiers*)0;

	  PREFIX_SYM* end_of_arg = _eqArg1->Bracket();
	  for (PREFIX_SYM* pos = lit + 1; pos < end_of_arg; pos++)
	    if (pos->Head().IsFunctor()) 
	      {
		complex_positions.push(pos);
	      };

	  //eq_second_arg = (PREFIX_SYM*)0;
      
	  _swapArguments = false;

	}
      else // unordered equality
	{ 
	  if (_eqArg1->Head().isVariable() || _eqArg2->Head().isVariable())
	    {   
	      _currentConstraint = (SimplificationOrdering::CompiledConstraintForUnifiers*)0;


	      PREFIX_SYM* end_of_lit = lit->Bracket();
	      for (PREFIX_SYM* pos = _eqArg1; pos < end_of_lit; pos++)
		if (pos->Head().IsFunctor()) complex_positions.push(pos);

	      //eq_second_arg = (PREFIX_SYM*)0;

	      _swapArguments = false;

	    }
	  else // both arguments are complex 
	    {

	      if (!_compiledConstraint)
		_compiledConstraint = SimplificationOrdering::current()->reserveCompiledConstraintForUnifiers();
	      ASSERT(_compiledConstraint);
	      _compiledConstraint->load(_eqArg2,_eqArg1);
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


	      PREFIX_SYM* end_of_arg1 = _eqArg1->Bracket();
	      for (PREFIX_SYM* pos = _eqArg1; pos < end_of_arg1; pos++)
		if (pos->Head().IsFunctor()) complex_positions.push(pos);

	      //eq_second_arg = _eqArg2;

	      _swapArguments = true;
        
	    };
	};
    }
  else // noneq. literal
    {
      _currentConstraint = (SimplificationOrdering::CompiledConstraintForUnifiers*)0;

      PREFIX_SYM* end_of_lit = lit->Bracket();
      for (PREFIX_SYM* pos = lit + 1; pos < end_of_lit; pos++)
	if (pos->Head().IsFunctor()) complex_positions.push(pos);

      //eq_second_arg = (PREFIX_SYM*)0;

      _swapArguments = false;

    };
 
  if (complex_positions.empty()) return false;

  FPARA_MACH::LoadTermToRewrite(complex_positions.pop());
  inherentlySelected = inhSel;
  return true; 
}; // bool FPARA_MACHINE::LoadLiteral(PREFIX_SYM* lit,bool inhSel)


bool FPARA_MACHINE::LoadRestOfLit()
{
  CALL("LoadRestOfLit()");

  if (!_swapArguments) return false;
  _swapArguments = false;

  if (!_compiledConstraint)
    _compiledConstraint = SimplificationOrdering::current()->reserveCompiledConstraintForUnifiers();
  ASSERT(_compiledConstraint);
  _compiledConstraint->load(_eqArg1,_eqArg2);
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




  PREFIX_SYM* end_of_arg2 = _eqArg2->Bracket();
  for (PREFIX_SYM* pos = _eqArg2; pos < end_of_arg2; pos++)
    if (pos->Head().IsFunctor()) complex_positions.push(pos);
    
  //eq_second_arg = (PREFIX_SYM*)0;
  
  return (!complex_positions.empty());
}; // bool FPARA_MACHINE::LoadRestOfLit()


bool FPARA_MACHINE::Paramodulate() 
{
  CALL("Paramodulate()");
 try_another_candidate:
  candidate = FPARA_MACH::NextCandidate(_currentConstraint);
  if (candidate) 
    {
      if (_noRulesFlag && candidate->TheClause()->IsRule()) goto try_another_candidate;
      return true; 
    }
  else // try another position
    {
    try_another_position:
      if (complex_positions.empty()) 
	{
	  if (!LoadRestOfLit()) return false;
	};
      ASSERT(!complex_positions.empty());
    
      PREFIX_SYM* term_to_rewrite = complex_positions.pop();
      FPARA_MACH::LoadTermToRewrite(term_to_rewrite);
      candidate = FPARA_MACH::NextCandidate(_currentConstraint);
      if (candidate) 
	{
	  if (_noRulesFlag && candidate->TheClause()->IsRule()) goto try_another_candidate;
	  return true;
	};
      goto try_another_position;
    };
}; // bool FPARA_MACHINE::Paramodulate() 

bool FPARA_MACHINE::CollectRestOfTheRewritingClause()
{
  CALL("CollectRestOfTheRewritingClause()");
  unsigned ln = 0;
  unsigned used_lit_num = candidate->LitNum();
  for (LiteralList::Iterator iter(candidate->TheClause()->LitList());iter.notEnd();iter.next())
    {
      if (ln != used_lit_num) 
	if (!_resolvent->PushStandardLit(index,iter.currentLiteral(),iter.currentElement()->isInherentlySelected())) return false;
      ln++;
    }; 
  return true;
}; // bool FPARA_MACHINE::CollectRestOfTheRewritingClause()
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_SUPERPOSITION
#define DEBUG_NAMESPACE "FPARA_MACH"
#endif
#include "debugMacros.h"
//=================================================


RES_PAIR_LIST* 
FPARA_MACH::NextCandidate(SimplificationOrdering::CompiledConstraintForUnifiers* constr) 
{
  CALL("NextCandidate(SimplificationOrdering::CompiledConstraintForUnifiers* constr)");
  RES_PAIR_LIST* result;
  switch (state)
    {
    case INIT:
      if (para_mach.FindFirstLeaf(*(_unifIndex->FParaTree(goal->Head().Func()))))
	{
	  state = PARA_FROM_COMPLEX;

	  if (constr && constr->holdsOnCurrentUnifier()) goto try_next_leaf;

	  para_mach.ResetCandidatesFromLeaf();
	  result = para_mach.NextCandidateFromTheLeaf();
	  ASSERT_IN(result,"A10");
	  return result;
	};
      goto try_para_from_var; 
     
    case PARA_FROM_COMPLEX:
      result = para_mach.NextCandidateFromTheLeaf();
      if (result) return result;
    try_next_leaf:
      if (para_mach.FindNextLeaf())
	{

	  if (constr && constr->holdsOnCurrentUnifier()) goto try_next_leaf;

	  para_mach.ResetCandidatesFromLeaf();
	  result = para_mach.NextCandidateFromTheLeaf();
	  ASSERT_IN(result,"A20");
	  return result;
	};
      goto try_para_from_var;
        
    case PARA_FROM_VAR:
      result = para_mach.NextCandidateFromTheLeaf();
      if (result) return result;
      if (para_mach.FindNextLeaf())
	{
	  para_mach.ResetCandidatesFromLeaf();
	  result = para_mach.NextCandidateFromTheLeaf();
	  ASSERT_IN(result,"A30"); 
	  return result;
	};
      return (RES_PAIR_LIST*)0;
    };
     
 try_para_from_var:
  state = PARA_FROM_VAR;
  para_mach.CompilePseudoLiteral(pseudo_header,goal);
      
  if (para_mach.FindFirstLeaf(*(_unifIndex->FParaFromVarTree())))
    {
      para_mach.ResetCandidatesFromLeaf();
      result = para_mach.NextCandidateFromTheLeaf();
      ASSERT_IN(result,"A20");
      return result;
    };
  return (RES_PAIR_LIST*)0; 
}; // RES_PAIR_LIST* FPARA_MACH::NextCandidate(SimplificationOrdering::CompiledConstraintForUnifiers* constr)  

//=======================================================================
