//
// File:         forward_superposition.h
// Description:  Representation of stored terms.
// Created:      Feb 14, 2000, Alexandre Riazanov, riazanov@cs.man.ac.uk
// Revised:      Dec 04, 2001, Alexandre Riazanov, riazanov@cs.man.ac.uk 
//               The array FPARA_PREMISE::_inherentlySelected
//               has been made extendable.
// Revised:      Dec 22, 2001, Alexandre Riazanov, riazanov@cs.man.ac.uk
//               In FPARA_PREMISE it was implicitely assumed that 
//               <num of lits>*HellConst::MaxTermSize <= HellConst::MaxClauseSize
//               which is now wrong since HellConst::MaxClauseSize is
//               not defined via HellConst::MaxNumOfLiterals.
// Revised:      Apr 28, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//               SimplificationOrdering::StoredConstraint 
//               replaced SharedKBConstr
// Revised:      May 09, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//               SimplificationOrdering::CompiledConstraintForUnifiers
//               replaced ORD_CONSTR

//====================================================
#ifndef FORWARD_SUPERPOSITION_H
//============================================================
#define FORWARD_SUPERPOSITION_H
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "VampireKernelConst.h"
#include "Stack.h"
#include "para_machine.h"
#include "NewClause.h"
#include "weight_formula.h"
#include "Array.h"
#include "ExpandingStack.h"
#include "GlobAlloc.h"
#include "SimplificationOrdering.h"
#include "DestructionMode.h"
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_SUPERPOSITION
#define DEBUG_NAMESPACE "FPARA_MACH"
#endif
#include "debugMacros.h"
//=================================================
namespace VK
{
class OR_INDEX;
class FPARA_MACH 
{
 private:
  enum STATE { INIT,  
	       PARA_FROM_COMPLEX, PARA_FROM_VAR };
 private:   
  STATE state;
  PARA_MACH para_mach;
  PREFIX_SYM* goal;
  TERM pseudo_header; 
  OR_INDEX* _unifIndex;
 public: 
  FPARA_MACH(OR_INDEX* unifIndex) 
    : pseudo_header((TERM::Arity)1,(TERM::Functor)0,TERM::PositivePolarity),
    _unifIndex(unifIndex)
    {};
  ~FPARA_MACH() { CALL("destructor ~FPARA_MACH()"); };
  void SetIndex(ulong ind) 
    {
      CALL("SetIndex(ulong ind)");
      para_mach.SetIndex(ind); 
    };
  ulong GetIndex() const { return para_mach.GetIndex(); };
  PREFIX_SYM* Goal() const { return goal; };
  void LoadTermToRewrite(PREFIX_SYM* t)
    {
      CALL("LoadTermToRewrite(PREFIX_SYM* t)");
      // t must be a plain complex term with all brackets set
      goal = t;
      para_mach.CompileComplexTerm(goal);
      state = INIT;
    };
    
  RES_PAIR_LIST* NextCandidate(SimplificationOrdering::CompiledConstraintForUnifiers* constr);
}; // class FPARA_MACH 
}; // namespace VK


//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_SUPERPOSITION
#define DEBUG_NAMESPACE "FPARA_MACHINE"
#endif
#include "debugMacros.h"
//================================================= 
 
namespace VK
{

class FPARA_MACHINE : public FPARA_MACH
{
 public:
  FPARA_MACHINE(NewClause* res,OR_INDEX* unifIndex) 
    : FPARA_MACH(unifIndex), 
    complex_positions("complex_positions"),
    _resolvent(res)
    {
      _compiledConstraint = (SimplificationOrdering::CompiledConstraintForUnifiers*)0; 
    };
  ~FPARA_MACHINE() 
    { 
      CALL("destructor ~FPARA_MACHINE()"); 
      if (DestructionMode::isThorough())
	{
	  if (_compiledConstraint)
	    {
	      SimplificationOrdering::current()->releaseCompiledConstraintForUnifiers(_compiledConstraint);
	    };      
	};
    };
  void setNoRulesFlag(bool fl) { _noRulesFlag = fl; };
  void SetIndex(ulong ind) 
    {
      CALL("SetIndex(ulong ind)");
      index = ind; 
      FPARA_MACH::SetIndex(ind); 
    };
  const ulong& GetIndex() const { return index; };
  RES_PAIR_LIST* Candidate() const { return candidate; };
  bool LoadLiteral(PREFIX_SYM* lit,bool inhSel);
  bool LoadRestOfLit();
  bool Paramodulate();

  bool CollectResult()
    {
      CALL("CollectResult()");
      return CollectRewrittenLiteral() && CollectRestOfTheRewritingClause();
    };
 private:
  bool CollectRewrittenLiteral()
    {
      CALL("CollectRewrittenLiteral()");
      return _resolvent->PushRewrittenLit(literal,
					  FPARA_MACH::Goal(),
					  candidate->ReplacingTerm(),
					  index,
					  inherentlySelected);
    };  
    
  bool CollectRestOfTheRewritingClause();  
 
 private:
  Stack<PREFIX_SYM*,VampireKernelConst::MaxTermSize> complex_positions;
  PREFIX_SYM* literal;
  bool inherentlySelected;
  RES_PAIR_LIST* candidate;
  ulong index;

   
  SimplificationOrdering::CompiledConstraintForUnifiers* _compiledConstraint;

  SimplificationOrdering::CompiledConstraintForUnifiers* _currentConstraint;


  PREFIX_SYM* _eqArg1;
  PREFIX_SYM* _eqArg2;
  bool _swapArguments;

  NewClause* _resolvent;
  bool _noRulesFlag;
}; // class FPARA_MACHINE : public FPARA_MACH


}; // namespace VK

 
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_SUPERPOSITION
#define DEBUG_NAMESPACE "FPARA_PREMISE"
#endif
#include "debugMacros.h"
//=================================================
namespace VK
{
class FPARA_PREMISE
{
 private:
  Clause* clause;
  ulong index;
  PREFIX_SYM max_literal[VampireKernelConst::MaxClauseSize];
 
  PREFIX_SYM* _freeMaxLitMemory; 

  ExpandingStack<GlobAlloc,PREFIX_SYM*,32UL,FPARA_PREMISE> _maxLiterals;
    

  Array<GlobAlloc,bool,32UL,FPARA_PREMISE> _inherentlySelected;


  ulong num_of_max_literals;
   
  LiteralList* _allLiterals;
  LiteralList::Element* _firstNonselectedLit;

  ulong next_max_lit;
  bool eq_res_applicable;
  WEIGHT_FORMULA weight_form;
  NewClause* _resolvent;
 public:
  FPARA_PREMISE(NewClause* res) 
    : _maxLiterals(DOP("FPARA_PREMISE::_maxLiterals")),
    _inherentlySelected(DOP("FPARA_PREMISE::_inherentlySelected")),
    weight_form(), 
    _resolvent(res) 
    {
      CALL("constructor FPARA_PREMISE(NewClause* res)");
      DOP(_maxLiterals.freeze());
      DOP(_inherentlySelected.freeze());
    };
  ~FPARA_PREMISE() { CALL("destructor ~FPARA_PREMISE()"); };
  void SetIndex(ulong ind) 
    {
      CALL("SetIndex(ulong ind)");
      index = ind; 
      weight_form.SetIndex(ind); 
    };
  Clause* Query() const { return clause; };
  void ResetIter() { next_max_lit = num_of_max_literals; };
  bool EqResApplicable() const { return eq_res_applicable; };
  void CreateWeightFormula()
    {
      CALL("CreateWeightFormula()");
      weight_form.Reset();

      for(LiteralList::Iterator iter(_allLiterals,_firstNonselectedLit);
	  iter.notEnd();
	  iter.next())
	if (!iter.currentLiteral()->isAnswerLiteral())
	  weight_form.Compile(iter.currentLiteral());


     
      ResetIter();
      for (PREFIX_SYM* m_lit = NextMaxLitMem(); m_lit; m_lit = NextMaxLitMem())
	{
	  TERM header = m_lit->Head().Func();
	  if (header.IsEquality() && header.IsOrderedEq()) weight_form.Compile(m_lit->Arg2()); 
	};
      
    };
   
  void Load(Clause* cl);

  PREFIX_SYM* NextMaxLit(bool& inherentlySelected)
    {
      if (next_max_lit)
	{
	  next_max_lit--;
	  return MaxLit(next_max_lit,inherentlySelected);
	};
      return (PREFIX_SYM*)0;
    }; 

  PREFIX_SYM* NextMaxLitMem()
    {
      if (next_max_lit)
	{
	  next_max_lit--;
	  return MaxLitMem(next_max_lit);
	};
      return (PREFIX_SYM*)0;
    }; 

  long MinResultWeight()
    {
      return weight_form.Compute();
    };
    
  bool CollectLiterals();
 private:
  PREFIX_SYM* MaxLit(ulong n,bool& inherentlySelected) 
    { 
      CALL("MaxLit(ulong n,bool& inherentlySelected)");
      inherentlySelected = _inherentlySelected[n];
      return _maxLiterals.nth(n);
      //return max_literal + (n*VampireKernelConst::MaxTermSize); 
    };
  PREFIX_SYM* MaxLitMem(ulong n) 
    { 
      CALL("MaxLitMem(ulong n)");
      return _maxLiterals.nth(n);
      //return max_literal + (n*VampireKernelConst::MaxTermSize); 
    };
}; // class FPARA_PREMISE
}; // namespace VK

//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_SUPERPOSITION
#define DEBUG_NAMESPACE "FORWARD_SUPERPOSITION"
#endif
#include "debugMacros.h"
//=================================================
namespace VK
{

class FORWARD_SUPERPOSITION
{
 public:
  FORWARD_SUPERPOSITION(NewClause* res,OR_INDEX* unifIndex)
    : fpara_machine(res,unifIndex),
    fpara_premise(res),
    _resolvent(res),
    _noInfBetweenRulesFlag(false)
    {
      CALL("constructor FORWARD_SUPERPOSITION(NewClause* res,OR_INDEX* unifIndex)");
      fpara_premise.SetIndex(0);
      fpara_machine.SetIndex(1);
      _constraintCheck = (SimplificationOrdering::ConstraintCheckOnUnifier*)0;
    }; 
  ~FORWARD_SUPERPOSITION() 
    { 
      CALL("destructor ~FORWARD_SUPERPOSITION()"); 
      if (DestructionMode::isThorough())
	{
	  if (_constraintCheck)
	    SimplificationOrdering::current()->releaseConstraintCheckOnUnifier(_constraintCheck);
	  _constraintCheck = (SimplificationOrdering::ConstraintCheckOnUnifier*)0;
	};
    };
  void setNoInfBetweenRulesFlag(bool fl)
    {
      _noInfBetweenRulesFlag = fl;     
    };
  bool Load(Clause* cl);
  bool NextInference()
    { 
      CALL("NextInference()");
      switch (_continuation)
	{
	case ContFSup:
	  while (NextFSupInference())
	    if (CollectResult()) return true;
	  // no more superpositions, start equality resolution 
	  if (!fpara_premise.EqResApplicable()) return false;
	  fpara_premise.ResetIter();
	  _continuation = ContEqRes;
	  // no break here, proceed with the next case
	case ContEqRes:
	  while (NextEqRes())
	    {
	      if (CollectEqResResult())
		{
		  UndoEqRes();         
		  return true;
		};
	      UndoEqRes(); 
	    }; 
	  // no more inferences
	  return false;
	};
      ICP("ICP1");
      return false;
    };
 private:
  bool NextFSupInference();
  void UndoEqRes()
    {
      Unifier::current()->UnbindLastSection();
    }; 
  bool NextEqRes();
  bool CollectResult();
  bool CollectEqResResult();
  
 private: 
  enum Continuation { ContFSup, ContEqRes };
 private:
  FPARA_MACHINE fpara_machine;
  FPARA_PREMISE fpara_premise;
  NewClause* _resolvent;
  Continuation _continuation;
  SimplificationOrdering::ConstraintCheckOnUnifier* _constraintCheck;
  bool _noInfBetweenRulesFlag;
}; // class FORWARD_SUPERPOSITION


}; // namespace VK

//================================================
#undef DEBUG_NAMESPACE 
//=================================================================
#endif
