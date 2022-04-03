#ifndef NEW_CLAUSE_H
//=========================================================================
#define NEW_CLAUSE_H
#include <iostream.h>
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "VampireKernelConst.h"
#include "Term.h"
#include "prefix.h"
#include "variables.h"
#include "OpenNewClause.h"
#include "ClauseBackground.h"
#include "ForwardDemodulation.h"
#include "ClauseQueue.h"
#include "PagedPassiveClauseStorage.h"
#include "Splitting.h"
#include "Clause.h"
namespace VK
{
  using namespace BK;
  using namespace Gematogen;
class TMP_LITERAL;
class ForwardSubsumptionFrontEnd;
class ForwardDemodulation;
class ComponentName;
class ClauseAssembler;
}; // namespace VK
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_NEW_CLAUSE
#define DEBUG_NAMESPACE "NewClause"
#endif 
#include "debugMacros.h"
//=================================================

namespace VK
{
  using namespace BK;
  using namespace Gematogen;
class NewClause
{
 public:
  class Statistics
    {
    public:
      Statistics() { reset(); };
      ~Statistics() {};
      void reset() 
	{
	  generated = 0;
	  forwardSubsumed = 0;
	  simplifiedByFSR = 0;
	  simplifiedByDemod = 0;
	  simplifiedByBuiltInTheories = 0;
	  fDOnSplittingBranches = 0;
	  simplifiedByEqRes = 0;
	  tooDeepInferences = 0;
	};
      ulong generated;
      ulong forwardSubsumed;
      ulong simplifiedByFSR;
      ulong simplifiedByDemod;
      ulong simplifiedByBuiltInTheories;
      ulong fDOnSplittingBranches;
      ulong simplifiedByEqRes;
      ulong tooDeepInferences;
    };

 public:
  NewClause() 
    : _statistics(),
    _openClause(), 
    _splitting(&_ancestors),
    _clauseNumber(0L),
    _ancestors(), 
    _varRenaming(), 
    _intVarRenaming(),
    _clauseAssembler((ClauseAssembler*)0),
    _forwardSubsumption((ForwardSubsumptionFrontEnd*)0),
    _forwardSimplificationByUEq((ForwardDemodulation*)0),
    _fDOnSplittingBranchesFlag(false), // default
    _splittingBranches(DOP("NewClause::_splittingBranches")),
    _forwardSubsumptionResolution(0L), // default
    _forwardDemodulation(0), // default
    _commutativityTable((const SymbolInfo*)0), // default 
    _simplificationByEqRes(false), // default
    _splittingFlag(false), // default
    _negEqSplitting(0L), // off, default
    _equalityPossible(false), // default
    _maxWeight(VampireKernelConst::MaxClauseSize),
    _previousMaxWeight(VampireKernelConst::MaxClauseSize),
    _maxInferenceDepth(LONG_MAX)
    {
      CALL("constructor NewClause()");
      DOP(_splittingBranches.freeze());
    };

  ~NewClause() { CALL("destructor ~NewClause()"); };     
  void Reset()
    {               
      _statistics.generated++;
      _openClause.Reset();
      _ancestors.Reset();
    };

  // setting options and parameters
  void SetClauseIndex(ClauseAssembler* ca) 
    { 
      _clauseAssembler = ca;
      _splitting.setClauseAssembler(ca); 
    }; 
  void SetComponentName(ComponentName* cn) 
    { 
      //_compName = cn;
      _splitting.setComponentNamingService(cn); 
    };
  void SetForwardSubsumption(ForwardSubsumptionFrontEnd* fs) 
    { 
      _splitting.setForwardSubsumption(fs);
      _forwardSubsumption = fs; 
    };
  void SetSimplificationByUEq(ForwardDemodulation* fsueq) { _forwardSimplificationByUEq = fsueq; };
  void SetFDOnSplittingBranchesFlag(bool fl) { _fDOnSplittingBranchesFlag = fl; };
  void SetNormalizationByCommutativity(const SymbolInfo* com_tab) { _commutativityTable = com_tab; }; 
  //void SetDefHeaderTable(const DefinedHeaderTable* t) { _defHeaderTab = t; }; 
   
  void SetForwardSubsumptionFlag(bool fl) 
    {
      _splitting.setForwardSubsumptionFlag(fl); 
      _forwardSubsumptionFlag = fl; 
    };

  void SetSimplificationByFS(long fl) 
    {
      if (fl > 0)
	{
	  _forwardSubsumptionResolution = (ulong)fl; 
	  _splitting.setForwardSubsumptionResolutionFlag(true);
	}
      else 
	{
	  _forwardSubsumptionResolution = 0;
	  _splitting.setForwardSubsumptionResolutionFlag(false);
	};
    };
  void SetSimplificationByUEq(long fl) { _forwardDemodulation = fl; };
  void SetSimplificationByEqRes(bool fl) { _simplificationByEqRes = fl; };
  void SetSplitting(bool fl) 
    { 
      _splittingFlag = fl;
    };
  void SetSplittingStyle(long num) 
    { 
      //_splittingStyle = num;
      _splitting.setStyle(num);
    };
  void SetSplittingWithNaming(bool fl) 
    { 
      _splitting.setSplittingWithNamingFlag(fl);
      //_splittingWithNamingFlag = fl; 
    };

  void SetNegEqSplitting(long w) 
    { 
      _negEqSplitting = w;
      if (w > 0L) 
	{
	  _splitting.setNegEqSplittingMinEligibleTermSize(w + 1);
	}
      else 
	{
	  _splitting.setNegEqSplittingMinEligibleTermSize(LONG_MAX);
	}; 
    };

  void SetEqualityPossible(bool fl) { _equalityPossible = fl; };
  void SetMaxWeight(ulong mw) 
    {
      _previousMaxWeight = _maxWeight;
      _maxWeight = (mw > VampireKernelConst::MaxClauseSize) ? VampireKernelConst::MaxClauseSize : mw;
      if (_maxWeight < VampireKernelConst::MinMaxWeight) _maxWeight = VampireKernelConst::MinMaxWeight;
      _openClause.SetMaxWeight(_maxWeight);
    };
  void setMaxInferenceDepth(long maxInferenceDepth)
    {
      _maxInferenceDepth = maxInferenceDepth;
    };
  void SetMaxLength(long ml) { _openClause.SetMaxLength(ml); };
  void SetMaxLitWeight(long mlw) { _openClause.SetMaxLitWeight(mlw); };

  // getting options and parameters 

  long MaxWeight() const { return _maxWeight; };
  long PreviousMaxWeight() const { return _previousMaxWeight; };


  // keeping statistics 

  void AnotherForwardSubsumed() { _statistics.forwardSubsumed++; };

  void AnotherSimplifiedByFSR() { _statistics.simplifiedByFSR++; };
  void AnotherSimplifiedByDemod() { _statistics.simplifiedByDemod++; };  
  void anotherSimplifiedByBuiltInTheories() { _statistics.simplifiedByBuiltInTheories++; };  
  void AnotherFDOnSplittingBranches() { _statistics.fDOnSplittingBranches++; };
  void AnotherSimplifiedByEqRes() { _statistics.simplifiedByDemod++; };  
  void AnotherTooHeavyClause() { _openClause.AnotherTooHeavyClause(); };
  void anotherClauseWithTooDeepInference()
    {
      ++_statistics.tooDeepInferences;
    };

  // getting statistics 


  ulong statTooDeepInferences() const
    {
      return _statistics.tooDeepInferences;
    };

  ulong statTooHeavy() const 
    { 
      return _openClause.statistics.tooHeavy; 
    };
  ulong statTooHeavyLiterals() const 
    { 
      return _openClause.statistics.tooHeavyLiterals;
    };
  ulong statTooManyVariables() const 
    { 
      return _openClause.statistics.tooManyVariables; 
    };
  ulong statTooLong() const 
    { 
      return _openClause.statistics.tooLong; 
    };
  ulong statTooDeep() const 
    { 
      return _openClause.statistics.tooDeep; 
    };
  ulong statEqTautologies() const 
    { 
      return _openClause.statistics.eqTautologies; 
    };
  ulong statPropTautologies() const 
    { 
      return _openClause.statistics.propTautologies; 
    };
  ulong statGenerated() const 
    { 
      return _statistics.generated; 
    };
  ulong statForwardSubsumed() const 
    { 
      return _statistics.forwardSubsumed + 
	_splitting.statForwardSubsumed(); 
    };
  ulong statSimplifiedByFSR() const 
    { 
      return _statistics.simplifiedByFSR +
	_splitting.statSimplifiedByFSR(); 
    };
  ulong statSimplifiedByDemod() const { return _statistics.simplifiedByDemod; };
  ulong statSimplifiedByBuiltInTheories() const
    {
      return _statistics.simplifiedByBuiltInTheories;
    };
  ulong statFDOnSplittingBranches() const { return _statistics.fDOnSplittingBranches; };
  ulong statSimplifiedByEqRes()  const { return _statistics.simplifiedByEqRes; };
  ulong statSplit() const 
    { 
      return _splitting.statSplit(); 
    };
  ulong statComponents() const 
    { 
      return _splitting.statComponents(); 
    };

  ulong statDifferentComponents() const 
    { 
      return _splitting.statDifferentComponents(); 
    };
   
  const bool& WasTautology() const { return _openClause.WasTautology(); };   

 public: // collecting a clause 

  void MakeVIP() { _ancestors.MakeVIP(); };
  bool PushSymbol(const TERM& sym)
    {
      if (sym.isVariable()) { PushVar(sym); return true; };
      if (sym.arity()) return PushNonconstFun(sym);
      PushConst(sym);
      return true;
    };   
  bool PushFunc(const TERM& f)
    {
      CALL("PushFunc(const TERM& f)");
      ASSERT(f.isComplex());
      if (f.arity()) return PushNonconstFun(f);
      PushConst(f); 
      return true;
    };
 
  bool OpenNonpropLiteral(const TERM& header,bool inherentlySelected) 
    { 
      return _openClause.OpenNonpropLiteral(header,inherentlySelected); 
    };
  bool CloseLiteral() { return _openClause.CloseLiteral(); };
  bool PushPropLit(const TERM& lit,bool inherentlySelected) 
    { 
      return _openClause.PushPropLit(lit,inherentlySelected); 
    }; 

  bool PushRewrittenLit(ulong index,TERM* lit,TERM* redex,PREFIX_SYM* replacing_term,bool inherentlySelected);
  bool PushRewrittenLit(PREFIX_SYM* lit,
			PREFIX_SYM* redex,
			TERM replacing_term,
			ulong index,
			bool inherentlySelected);
  bool PushPrefixLit(PREFIX_SYM* word,bool inherentlySelected); 
  bool PushPrefixEqLit(TERM header,PREFIX_SYM* arg1,PREFIX_SYM* arg2,bool inherentlySelected);
  bool PushPrefixComplexTerm(PREFIX_SYM* word);
  bool PushPrefixTerm(PREFIX_SYM* t);
  bool PushStandardLit(ulong index,TERM* t,bool inherentlySelected); 
  bool PushStandardTerm(ulong index,TERM t);
  bool PushStandardComplexTerm(ulong index,TERM* t);
  bool PushLiteral(TERM* t,bool inherentlySelected);   
  bool LoadClause(const Clause* c);
  bool LoadClause(PagedPassiveClauseStorage::ClauseLiterals& literals);

  void setClauseNumber(long num) { _clauseNumber = num; }; 
  void UsedRule(ClauseBackground::Rule r) { _ancestors.UsedRule(r); }; 
  void CancelRule(ClauseBackground::Rule r) { _ancestors.CancelRule(r); }; 
  void UsedRules(const ClauseBackground::Rules& rs) { _ancestors.UsedRules(rs); }; 
  void PushAncestor(Clause* cl) 
    {
      CALL("PushAncestor(Clause* cl)");
      if (cl->isSubgoal()) _ancestors.markAsSubgoal();
      _ancestors.pushAncestor(cl,cl->inferenceDepth()); 
    };
  void InverseAncestorOrder() { _ancestors.InverseOrder(); };
  void markAsSupport() { _ancestors.markAsSupport(); };
 public:  // processing the collected clause
  Clause* FSubsume(Clause* cl);
  void ProcessCollected(ClauseQueue& newClauses);
  void ProcessCollected2(ClauseQueue& newClauses);
  void ProcessCollected3(Clause*& emptyClause,
			 PagedPassiveClauseStorage& storage,
			 ulong& numOfNewClauses,
			 ulong& numOfRejectedNewClauses,
			 Clause::NewNumber* clauseNumberGenerator,
			 bool showNewClauses); 
  Clause* Reanimate();

  bool transferToPagedPassive(PagedPassiveClauseStorage& storage,bool showNewClause);

 public: // output
  ostream& output(ostream& str) const;
  ostream& outputAncestors(ostream& str) const;
   
 private:  // collecting a clause 
  bool PushVar(const ulong v) { return _openClause.PushVar(v); };
  void PushVar(const TERM& v) { _openClause.PushVar(v); };  
  void PushConst(const TERM& c) { _openClause.PushConst(c); };
  bool PushNonconstFun(const TERM& f) { return _openClause.PushNonconstFun(f); };

 private: // processing collected clause
  void Simplify(bool& redundant);
  Clause* FSubsume();
  ulong SimplifyByFS();
  Clause* RewriteByUEq(TMP_LITERAL* lit,TMP_LITERAL* new_lit);
  bool SimplifyByUEq(TMP_LITERAL*& lit,TMP_LITERAL*& spare_lit);
  bool SimplifyByUEq();
  bool simplifyByBuiltInTheories(TMP_LITERAL*& lit,TMP_LITERAL*& spareLit);
  bool simplifyByBuiltInTheories();
  bool SimplifyByEqRes();
   
  bool NormalizeByCommutativity();  
  bool NormalizeByCommutativity(TMP_LITERAL* lit);
 
 private: // structure

  Statistics _statistics;  

  OpenNewClause _openClause;

  Splitting _splitting;

  long _clauseNumber;
  OpenClauseBackground _ancestors;
  VAR_TO_INT_RENAMING _varRenaming;
  INT_VAR_RENAMING _intVarRenaming;
  ClauseAssembler* _clauseAssembler;

  ForwardSubsumptionFrontEnd* _forwardSubsumption;
  ForwardDemodulation* _forwardSimplificationByUEq;
  bool _fDOnSplittingBranchesFlag;
  ForwardDemodulation::SetOfSplittingBranches _splittingBranches;

 private: // options and parameters
  bool _forwardSubsumptionFlag; 
  ulong _forwardSubsumptionResolution;
  long _forwardDemodulation;
  const SymbolInfo* _commutativityTable;
  bool _simplificationByEqRes;
  bool _splittingFlag;
  long _negEqSplitting;  
  bool _equalityPossible;
  long _maxWeight; 
  long _previousMaxWeight;
  long _maxInferenceDepth;
}; // class NewClause 

//=================================================================

inline ostream& operator<<(ostream& str,const NewClause& cl) 
{
  return cl.output(str);
};

inline ostream& operator<<(ostream& str,const NewClause* cl) 
{
  return cl->output(str);
};


inline bool NewClause::PushRewrittenLit(ulong index,TERM* lit,TERM* redex,PREFIX_SYM* replacing_term,bool inherentlySelected)
{
  CALL("PushRewrittenLit(ulong index,TERM* lit,TERM* redex,PREFIX_SYM* replacing_term,bool inherentlySelected)");
  TERM header = *lit;
  ASSERT(header.arity()); 
  if (header.IsEquality()) header.MakeUnordered();
  if (!(OpenNonpropLiteral(header,inherentlySelected))) return false;
  
  ST_TERM_WITH_SUBST iter;
  iter.Reset(index,lit);
  iter.SkipSym();
  PREF_SYM sym;
  
  while (!(iter.Empty()))
    {
      if (iter.CurrPosInBase() == redex)
	{
	  if (!PushPrefixTerm(replacing_term)) return false;
	  iter.SkipTerm();
	}
      else
	{ 
	  sym = iter.CurrSym();
	  if (sym.isVariable()) // free variable, must be renamed
	    {
	      if (!(PushVar(_varRenaming.Rename(sym.var())))) { return false; };
	    }
	  else
	    {
	      if (sym.Func().arity())
		{
		  if (!(PushNonconstFun(sym.Func()))) return false;
		}
	      else PushConst(sym.Func()); 
	    };
	  iter.SkipSym();
	};  
    };
   
  return CloseLiteral();
}; // bool NewClause::PushRewrittenLit(ulong index,TERM* lit,TERM* redex,PREFIX_SYM* replacing_term,bool inherentlySelected)


inline bool NewClause::PushPrefixLit(PREFIX_SYM* word,bool inherentlySelected)
{
  CALL("PushPrefixLit(PREFIX_SYM* word,bool inherentlySelected)");
  ASSERT(PREFIX::CorrectComplexTerm(word));
  TERM header = word->Head().Func();
  if (!(header.arity())) { return PushPropLit(header,inherentlySelected); };
  
  if (!(OpenNonpropLiteral(header,inherentlySelected))) return false;
  
  PREFIX::ITER iter;
  iter.Reset(word);
  TERM sym;
  iter.SkipSym(); // skipping the header
  do
    {
      if (iter.CurrVar()) // free variable
	{ 
	  if (!(PushVar(_varRenaming.Rename(iter.CurrVar())))) { return false; };  
	}
      else // functor
	{
	  sym = iter.CurrSubterm()->Head().Func();
	  if (sym.arity())
	    {
	      if (!(PushNonconstFun(sym))) return false;
	    }
	  else // constant
	    PushConst(sym);
	};
      iter.SkipSym();
    }
  while (!(iter.Empty()));
  return CloseLiteral();
}; // bool NewClause::PushPrefixLit(PREFIX_SYM* word,bool inherentlySelected)



inline bool NewClause::PushPrefixEqLit(TERM header,PREFIX_SYM* arg1,PREFIX_SYM* arg2,bool inherentlySelected)
{
  CALL("PushPrefixEqLit(TERM header,PREFIX_SYM* arg1,PREFIX_SYM* arg2,bool inherentlySelected)");
  ASSERT(header.IsEquality());   
  
  return OpenNonpropLiteral(header,inherentlySelected)
    && PushPrefixTerm(arg1)
    && PushPrefixTerm(arg2)
    && CloseLiteral();
}; // bool NewClause::PushPrefixEqLit(TERM header,PREFIX_SYM* arg1,PREFIX_SYM* arg2,bool inherentlySelected)


inline bool NewClause::PushPrefixComplexTerm(PREFIX_SYM* word)
{
  CALL("PushPrefixComplexTerm(PREFIX_SYM* word)");
  PREFIX::ITER iter;
  iter.Reset(word);
  TERM sym;
  do
    {
      if (iter.CurrVar()) // free variable
	{ 
	  if (!(PushVar(_varRenaming.Rename(iter.CurrVar())))) return false; 
	}
      else // functor
	{
	  sym = iter.CurrSubterm()->Head().Func();
	  if (sym.arity())
	    {
	      if (!(PushNonconstFun(sym))) return false;
	    }
	  else // constant
	    PushConst(sym);
	};
      iter.SkipSym();
    }
  while (!(iter.Empty()));
  return true;
}; // bool NewClause::PushPrefixComplexTerm(PREFIX_SYM* word)

inline bool NewClause::PushPrefixTerm(PREFIX_SYM* t)
{
  CALL("PushPrefixTerm(PREFIX_SYM* t)");
  if (t->Head().isVariable())
    {
      bool free_var;
      void* bind = t->Head().var()->Unref(free_var);
      if (free_var) return PushVar(_varRenaming.Rename((Unifier::Variable*)bind));
      return PushPrefixComplexTerm((PREFIX_SYM*)bind); 
    };
  return PushPrefixComplexTerm(t);
}; // bool NewClause::PushPrefixTerm(PREFIX_SYM* t)

inline bool NewClause::PushStandardTerm(ulong index,TERM t)
{
  CALL("PushStandardTerm(ulong index,TERM t)");
  // t is variable or reference 
  if (t.isVariable())
    {
      bool free_var;
      void* bind = Unifier::current()->variable(t.var(),index)->Unref(free_var);
    
      if (free_var) return PushVar(_varRenaming.Rename((Unifier::Variable*)bind));
      return PushPrefixComplexTerm((PREFIX_SYM*)bind);
    };
  // t is reference to a complex term
  return PushStandardComplexTerm(index,t.First());
}; // bool NewClause::PushStandardTerm(ulong index,TERM t)

inline bool NewClause::PushStandardComplexTerm(ulong index,TERM* t)
{
  CALL("PushStandardComplexTerm(ulong index,TERM* t)");
  ST_TERM_WITH_SUBST term;
  term.Reset(index,t);
  PREF_SYM sym;
  do
    {
      sym = term.CurrSym();
      if (sym.isVariable()) // free variable, must be renamed
	{
	  if (!(PushVar(_varRenaming.Rename(sym.var())))) return false; 
	}
      else
	{
	  if (sym.Func().arity())
	    {
	      if (!(PushNonconstFun(sym.Func()))) return false;
	    }
	  else PushConst(sym.Func()); 
	};
      term.SkipSym();
    }
  while (!(term.Empty()));
  return true; 
}; // inline bool NewClause::PushStandardComplexTerm(ulong index,TERM* t)

inline bool NewClause::PushStandardLit(ulong index,TERM* t,bool inherentlySelected)
{
  CALL("PushStandardLit(ulong index,TERM* t,bool inherentlySelected)");

  // Note: this function uses the global substitution to substitute the variables!
  TERM header = *t;

  if (!(header.arity())) { return PushPropLit(header,inherentlySelected); };

  if (!(OpenNonpropLiteral(header,inherentlySelected))) return false;
 
  ST_TERM_WITH_SUBST term;
  term.Reset(index,t);
  term.SkipSym();
  PREF_SYM sym;
  while (!(term.Empty()))
    {
      sym = term.CurrSym();
      if (sym.isVariable()) // free variable, must be renamed
	{
	  if (!(PushVar(_varRenaming.Rename(sym.var())))) { return false; };
	}
      else
	{
	  if (sym.Func().arity())
	    {
	      if (!(PushNonconstFun(sym.Func()))) return false;
	    }
	  else 
	    {
	      PushConst(sym.Func());
	    }; 
	};
      term.SkipSym();
    };
   
  return CloseLiteral();  
}; // bool NewClause::PushStandardLit(ulong index,TERM* t,bool inherentlySelected)

}; // namespace VK

//===========================================================================
#undef DEBUG_NAMESPACE
//=========================================================================
#endif
