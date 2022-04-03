//
// File:         backward_superposition.h
// Description:  Classes dealing with backward superposition.
// Created:      Mar 12, 2000, Alexandre Riazanov, riazanov@cs.man.ac.uk
// Revised:      Apr 28, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//               SimplificationOrdering::StoredConstraint 
//               replaced SharedKBConstr
// Revised:      May 09, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//               SimplificationOrdering::CompiledConstraintForUnifiers
//               replaced ORD_CONSTR
//====================================================
#ifndef BACKWARD_SUPERPOSITION_H
//==================================================================
#define BACKWARD_SUPERPOSITION_H
#include <iostream.h>
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "VampireKernelConst.h"   
#include "DestructionMode.h"
#include "para_machine.h"
#include "NewClause.h"
#include "SimplificationOrdering.h"
#include "weight_formula.h"
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_BACKWARD_SUPERPOSITION
#define DEBUG_NAMESPACE "BPARA_MACH"
#endif
#include "debugMacros.h"
//=================================================
namespace VK
{
class BPARA_MACH
{
 private:
  PARA_MACH para_mach;
  OR_INDEX* _unifIndex;
  bool init_state;
  Clause* query; // mostly for debugging
  bool unit_query;

  BackParaCandidates::Iterator _candidates;


 public:
  BPARA_MACH(OR_INDEX* unifIndex) : 
    _unifIndex(unifIndex)
    {
    };
  ~BPARA_MACH() { CALL("destructor ~BPARA_MACH()"); };
  void SetIndex(ulong ind) { para_mach.SetIndex(ind); };
  ulong GetIndex() const { return para_mach.GetIndex(); };
  void SetUnitQuery(bool fl) { unit_query = fl; };
  void SetQuery(Clause* cl) { query = cl; };
  bool VariablesFromIndexAffected() { return para_mach.VariablesFromIndexAffected(); };
   
  void LoadLHS(PREFIX_SYM* t)
    {
      para_mach.CompileComplexTerm(t);     
      init_state = true;
    };
            
  bool NextCandidate(SimplificationOrdering::CompiledConstraintForUnifiers* constr,
		     Clause*& candCl,
		     BackParaCandidates::LitRedexPair*& candLitRedexPair);     
}; // class BPARA_MACH

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
class BPARA_MACHINE 
{

 public: 
  BPARA_MACHINE(NewClause* res,OR_INDEX* unifIndex) : 
    bpara_mach(unifIndex),     
    _resolvent(res)
    {
      
      fst_arg_mem[0].Head().MkFunc(TERM((TERM::Arity)1,(TERM::Functor)0,TERM::PositivePolarity));
      snd_arg_mem[0].Head().MkFunc(TERM((TERM::Arity)1,(TERM::Functor)0,TERM::PositivePolarity));
      _constraintCheck = (SimplificationOrdering::ConstraintCheckOnUnifier*)0; 

      _compiledConstraint = (SimplificationOrdering::CompiledConstraintForUnifiers*)0; 
    };
  ~BPARA_MACHINE() 
    { 
      CALL("destructor ~BPARA_MACHINE()");   
      if (DestructionMode::isThorough())
	{
	  if (_constraintCheck)
	    {
	      SimplificationOrdering::current()->releaseConstraintCheckOnUnifier(_constraintCheck);
	    };
	  if (_compiledConstraint)
	    {
	      SimplificationOrdering::current()->releaseCompiledConstraintForUnifiers(_compiledConstraint);
	    };
	};
    };
  void setNoRulesFlag(bool fl) { _noRulesFlag = fl; };
  void setSimultaneous(long degree) { _simultaneousness = degree; };
  void SetIndex(ulong ind) { index = ind; bpara_mach.SetIndex(ind); };
  ulong GetIndex() const { return index; };    
  bool VariablesFromIndexAffected() { return bpara_mach.VariablesFromIndexAffected(); };
  void SetQuery(Clause* cl) 
    {
      query = cl;
      bpara_mach.SetQuery(cl);
      bpara_mach.SetUnitQuery(cl->Unit());
    };

  Clause* candClause() const { return _candClause; };

  void LoadEq(const TERM* eq_lit,ulong eq_ind);
  bool Paramodulate();
  bool CollectLiterals();

 private:
  BPARA_MACH bpara_mach;
  PREFIX_SYM fst_arg_mem[VampireKernelConst::MaxTermSize];
  PREFIX_SYM snd_arg_mem[VampireKernelConst::MaxTermSize];
  PREFIX_SYM* rhs;
  ulong index;
  ulong eq_index;
  bool unordered_eq;
  bool _swapArguments;

  Clause* _candClause;
  BackParaCandidates::LitRedexPair* _candLitRedexPair;

  Clause* query;
  NewClause* _resolvent;
  long _simultaneousness;
  SimplificationOrdering::ConstraintCheckOnUnifier* _constraintCheck;
  
  SimplificationOrdering::CompiledConstraintForUnifiers* _compiledConstraint;

  SimplificationOrdering::CompiledConstraintForUnifiers* _currentConstraint;

  bool _noRulesFlag;  
 
}; // class BPARA_MACHINE 
}; // namespace VK


//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_BACKWARD_SUPERPOSITION
#define DEBUG_NAMESPACE "BPARA_PREMISE"
#endif
#include "debugMacros.h"
//================================================= 

namespace VK
{
class BPARA_PREMISE
{ 
 private:
  ulong index;
  Clause* clause;

  LiteralList* allLiterals;
  LiteralList::Element* firstNonmaxLit;
  LiteralList::Iterator maxLitIter;
  LiteralList::Element* _currentMaxLitElement;

  WEIGHT_FORMULA nonmax_weight_form;
  WEIGHT_FORMULA max_weight_form;
  NewClause* _resolvent;
 public:
  BPARA_PREMISE(NewClause* res) 
    : maxLitIter(), nonmax_weight_form(), max_weight_form(), _resolvent(res)
    {}; 
  ~BPARA_PREMISE() { CALL("destructor ~BPARA_PREMISE()"); };
  void SetIndex(ulong ind) 
    {
      index = ind; 
      nonmax_weight_form.SetIndex(ind);
      max_weight_form.SetIndex(ind);
    };
  ulong GetIndex() const { return index; };
  Clause* TheClause() const { return clause; };
  void CollectNonmaxWeightForm();
  void RecollectMaxWeightForm();
  void Load(Clause* cl);
  const TERM* NextEq();
  long MinResultWeight()
    {
      return nonmax_weight_form.Compute() + max_weight_form.Compute();
    }; 

  bool CollectLiterals();
}; // class BPARA_PREMISE

}; // namespace VK

//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_BACKWARD_SUPERPOSITION
#define DEBUG_NAMESPACE "BACKWARD_SUPERPOSITION"
#endif
#include "debugMacros.h"
//================================================= 

namespace VK
{
class BACKWARD_SUPERPOSITION
{
 private:
  BPARA_PREMISE bpara_premise;
  BPARA_MACHINE bpara_machine;
  NewClause* _resolvent; 
  const TERM* _currentPosEq;
  bool _noInfBetweenRulesFlag;
 public:
  BACKWARD_SUPERPOSITION(NewClause* res,OR_INDEX* unifIndex)
    : bpara_premise(res),
    bpara_machine(res,unifIndex),
    _resolvent(res),
    _noInfBetweenRulesFlag(false)
    {
      bpara_premise.SetIndex(0);
      bpara_machine.SetIndex(1);
    }; 
  ~BACKWARD_SUPERPOSITION() { CALL("destructor ~BACKWARD_SUPERPOSITION()"); }; 
 
  void setSimultaneous(long degree) { bpara_machine.setSimultaneous(degree); };
  void setNoInfBetweenRulesFlag(bool fl)
    {
      _noInfBetweenRulesFlag = fl;     
    };
   
  bool SetQuery(Clause* cl); 
  bool CollectResult();
  bool NextInference(); 
}; // class BACKWARD_SUPERPOSITION

}; // namespace VK
//================================================
#undef DEBUG_NAMESPACE
//==============================================================
#endif 







 
