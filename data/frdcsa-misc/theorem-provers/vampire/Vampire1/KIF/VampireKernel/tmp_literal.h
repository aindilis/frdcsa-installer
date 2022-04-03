// Revised:  Apr 11, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//           1) Temporary code replaced by permanent 
//           in TMP_LITERAL::KBCompareEqOO(TMP_LITERAL& lit).
//           2) Minor changes in TMP_LITERAL::KBCompareEqOU(TMP_LITERAL& lit)
//              and TMP_LITERAL::KBCompareEqUU(TMP_LITERAL& lit)
//              to relax requirements on the reduction ordering 
//              when applied to terms with varibles. Namely,
//              we do not assume anymore that 
//              s > t and t > u contradicts s * u.   
// Revised:  Apr 19, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//           1) better names for members;
//           2) arbitrary simplification ordering.
//  
// Revised:  May 31, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//           arg1() and arg2() respect the ordering now. 
//===========================================================
#ifndef TMP_LITERAL_H
//===========================================================
#define TMP_LITERAL_H
#include <iostream.h>
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "VampireKernelConst.h"
#include "GlobAlloc.h"
#include "Term.h"
#include "Multiset.h"
#include "WeightPolynomial.h"
#include "TermWeighting.h"
#include "SimplificationOrdering.h"
#include "Flatterm.h"
#include "Signature.h"
#include "DestructionMode.h"
#include "TermWeightType.h"
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_NEW_CLAUSE
#define DEBUG_NAMESPACE "TMP_LITERAL"
#endif 
#include "debugMacros.h"
//=================================================
namespace VK
{
#ifndef NO_DEBUG
class TmpLitList;
#endif
class TMP_LITERAL
{
 public:
  typedef MultisetWithMinMax<ulong,VampireKernelConst::MaxTermDepth + 1> VarDepthProfile;
  // ^ "+ 1" is essential!
  class InitStatic
    {
    public: 
      InitStatic() 
	{
	  if (!_count) TMP_LITERAL::initStatic(); 
	  _count++;    
	};
      ~InitStatic() 
	{
	  _count--;
	  if (!_count) TMP_LITERAL::destroyStatic();
	};
    private:
      static long _count;
    }; // class InitStatic

 public:   
  TMP_LITERAL() :
    _size(0), 
    _weightCollected(false),
    _variableOccurencesCollected(false),
    _inherentlySelected(false)
    {
      DOP(_debugLiteralComplete = false);
      DOP(_arg1 = (Flatterm*)0);
      DOP(_arg2 = (Flatterm*)0);
      DOP(_arg1Weight = (WeightPolynomial*)1);
      DOP(_arg2Weight = (WeightPolynomial*)1);
    };
  ~TMP_LITERAL() { CALL("destructor ~TMP_LITERAL()"); };
  static void initStatic(); 
  static void destroyStatic();
  static void resetStatic()
    {
      _freeLiterals = (TMP_LITERAL*)0;
    };

  const TMP_LITERAL* previous() const { return _previous; };
  TMP_LITERAL*& previous() { return _previous; };

  const TMP_LITERAL* next() const { return _next; };
  TMP_LITERAL*& next() { return _next; };

#ifndef NO_DEBUG
  TmpLitList*& currentList() { return _currentList; };
  TmpLitList* currentList() const { return _currentList; };
#endif

  static TMP_LITERAL* freshLiteral()
    {
      CALL("freshLiteral()");
      TMP_LITERAL* res;
      if (_freeLiterals)
	{
	  res = _freeLiterals;
	  _freeLiterals = _freeLiterals->_nextFree; 
	  res->reset();
	}
      else
	{
	  res = new TMP_LITERAL();       
	};

#ifndef NO_DEBUG
      res->currentList() = (TmpLitList*)0;
#endif
      return res;
    }; // TMP_LITERAL* freshLiteral()

  void recycle()
    {
      CALL("recycle()");
      ASSERT(_freeLiterals != this);
      ASSERT(!_currentList);
      _nextFree = _freeLiterals;
      _freeLiterals = this;
    }; // void recycle()


  void reset()
    {
      _openFlatterm.Reset();
      _size = 0;
      _weightCollected = false;
      _variableOccurencesCollected = false;
      DOP(_debugLiteralComplete = false);
      DOP(_arg1 = (Flatterm*)0);
      DOP(_arg2 = (Flatterm*)0);
      DOP(_arg1Weight = (WeightPolynomial*)1);
      DOP(_arg2Weight = (WeightPolynomial*)1);
      _inherentlySelected = false;
    };

  void setInherentlySelected(bool fl) 
    {
      CALL("setInherentlySelected(bool fl)");
      _inherentlySelected = fl; 
    };

  bool isInherentlySelected() const { return _inherentlySelected; };
  bool isAnswerLiteral() const { return header().isAnswerHeader(); };
  const Flatterm* flatterm() const 
    {
      CALL("flatterm() const");
      ASSERT(_debugLiteralComplete);
      return _openFlatterm.Term(); 
    };
 
  Flatterm* flatterm() 
    {
      CALL("flatterm()");

      ASSERT(_debugLiteralComplete);
      return _openFlatterm.Term(); 
    }; 

  const TERM& header() const { return _header; };
  ulong arity() const { return _header.arity(); };
  unsigned long depth() { CALL("depth()"); return ((Flatterm*)flatterm())->Depth(); };
  ulong size() const { return _size; };
  ulong weightBase() const // conceptually different from weight()! 
    {
      return (isAnswerLiteral()) ? 0UL : size(); 
    };
  

  const WeightPolynomial& weight()
    {
      CALL("weight()");
      if (!_weightCollected) collectWeight();
      return _weight; 
    };
  
  const WeightPolynomial::Monomials& variableOccurences() 
    {
      CALL("variableOccurences()");
      if (!_variableOccurencesCollected) collectVariableOccurences();
      return _variableOccurences;
    }; // const WeightPolynomial::Monomials& variableOccurences() 
 
  TermWeightType numberOfVariableOccurences()
    {
      CALL("numberOfVariableOccurences()");
      return variableOccurences().sumOfCoefficients();
    }; // TermWeightType numberOfVariableOccurences()

  void variables(const ulong*& begin,const ulong*& end)
    {
      CALL("variables(const ulong*& begin,const ulong*& end)");
      if (_weightCollected)
	{
	  begin = _weight.monomials().begin();
	  end = _weight.monomials().end();
	}
      else
	if (_variableOccurencesCollected)
	  {
	    begin = _variableOccurences.begin();
	    end = _variableOccurences.end();
	  }
	else
	  {
	    collectWeight();
	    begin = _weight.monomials().begin();
	    end = _weight.monomials().end();
	  };
    }; // void variables(const ulong*& begin,const ulong*& end)

  ulong numOfDifferentVariables() 
    {
      CALL("numOfDifferentVariables()");
      ASSERT(_debugLiteralComplete);
      if (_weightCollected)
	{
	  return _weight.monomials().length();
	}
      else
	if (_variableOccurencesCollected)
	  {
	    return _variableOccurences.length();
	  }
	else
	  {
	    collectWeight();
	    return _weight.monomials().length();
	  };
    }; // ulong numOfDifferentVariables()


  bool containsVariable(ulong var)
    {
      CALL("containsVariable(ulong var)");
      ASSERT(_debugLiteralComplete);
      if (_weightCollected)
	{
	  return (bool)_weight.monomials().coefficient(var);
	}
      else
	if (_variableOccurencesCollected)
	  {
	    return (bool)_variableOccurences.coefficient(var);
	  }
	else
	  {
	    collectWeight();
	    return (bool)_weight.monomials().coefficient(var);
	  };
    }; // bool containsVariable(ulong var)


  const WeightPolynomial& arg1Weight()
    {
      CALL("arg1Weight()");
      ASSERT(!isAnswerLiteral());
      if (!_arg1Weight) collectArg1Weight();
      return *_arg1Weight; 
    };
  const WeightPolynomial& arg2Weight()
    { 
      CALL("arg2Weight()");
      ASSERT(!isAnswerLiteral());
      if (!_arg2Weight) collectArg2Weight();
      return *_arg2Weight; 
    };

  bool arg1ContainsVariable(ulong var)
    {
      CALL("arg1ContainsVariable(ulong var)");
      return (bool)arg1Weight().monomials().coefficient(var);
    };
  
  bool arg2ContainsVariable(ulong var)
    {
      CALL("arg2ContainsVariable(ulong var)");
      return (bool)arg2Weight().monomials().coefficient(var);
    };

  bool arg1IsGround()
    {
      CALL("arg1IsGround()");
      return arg1Weight().monomials().isZero();
    };

  bool arg2IsGround()
    {
      CALL("arg2IsGround()");
      return arg2Weight().monomials().isZero();
    };

  bool isEquality() const { return _header.IsEquality(); };
  bool isOrderedEquality() const { return _header.IsOrderedEq(); };
  bool isUnorderedEquality() const { return _header.IsUnorderedEq(); };
  bool isPositive() const { return _header.Positive(); };
  bool isNegative() const { return _header.Negative(); }; 
  TERM::Polarity polarity() const { return _header.polarity(); };

  const Flatterm* arg1() const 
    {
      CALL("arg1() const");
      ASSERT(_debugLiteralComplete);
      return _arg1;
    };

  Flatterm* arg1() 
    {
      CALL("arg1()");
      ASSERT(_debugLiteralComplete);
      return _arg1;
    };

  const Flatterm* arg2() const 
    { 
      CALL("arg2() const");
      ASSERT(_debugLiteralComplete);
      return _arg2;
    };

  Flatterm* arg2()
    { 
      CALL("arg2()");
      ASSERT(_debugLiteralComplete);
      return _arg2;
    };


  void linkEqArgs() 
    { 
      CALL("linkEqArgs()");
      ASSERT(_debugLiteralComplete);
      ASSERT(flatterm() < (flatterm()->after()));
      ((Flatterm*)(flatterm()->after()))->SetBackJump(flatterm() + 1); 
    };



   
  void makeEqUnordered()
    {
      CALL("makeEqUnordered()");
      _header.MakeUnordered();
      ((Flatterm*)flatterm())->symbolRef() = _header;
      resetArguments();
      resetArgWeights(); 
    };

  ulong minPosEqLHSSize() const
    {
      CALL("minPosEqLHSSize() const");
      ASSERT(_debugLiteralComplete);
      ASSERT(isEquality() && isPositive());
      if (isOrderedEquality())
	{
	  return arg2()->size();
	}
      else // unordered 
	{
	  long w1 = arg1()->size();
	  long w2 = arg2()->size();
	  return (w1 > w2) ? w2 : w1;
	}; 
    };
   
  void pushVar(const ulong& v)
    {
      CALL("pushVar(const ulong& v)");
      ASSERT(!_debugLiteralComplete);
      _openFlatterm.PushVar(TERM(TermVar,v));
    };

  void pushVar(const TERM& v) 
    {
      CALL("pushVar(const TERM& v)");
      ASSERT(!_debugLiteralComplete);
      _openFlatterm.PushVar(v); 
    };

  void pushConst(const TERM& c) 
    {
      CALL("pushConst(const TERM& c)");
      ASSERT(!_debugLiteralComplete);
      _openFlatterm.PushConst(c); 
    };

  bool pushNonconstFun(const TERM& f) 
    {
      CALL("pushNonconstFun(const TERM& f)");
      ASSERT(!_debugLiteralComplete);
      if (_openFlatterm.Depth() < VampireKernelConst::MaxTermDepth)
	{ 
	  _openFlatterm.PushNonconstFun(f); 
	  _size += f.arity();
	  return true;
	};
      return false;   
    };
   
  bool pushSym(const TERM& sym)
    {
      CALL("pushSym(const TERM& sym)");
      if (sym.isVariable()) { pushVar(sym); return true; };
      if (sym.arity()) return pushNonconstFun(sym);
      pushConst(sym);
      return true;   
    };

  bool pushTerm(const Flatterm* term)
    {
      CALL("pushTerm(const Flatterm* term)");
      ASSERT(!_debugLiteralComplete);
      Flatterm::Iterator iter(term); 
      do
	{
	  if (!pushSym(iter.CurrPos()->symbolRef())) return false;
	  iter.NextSym();
	}
      while (iter.NotEndOfTerm());
      return true;
    };   

  void pushNonpropHeader(const TERM& hd)
    {
      CALL("pushNonpropHeader(const TERM& hd)");
      ASSERT(!_debugLiteralComplete);
      _size = hd.arity() + 1;
      _openFlatterm.PushNonconstFun(hd);
      _header = hd;
    };
   
  void pushPropLit(const TERM& lit)
    {
      CALL("pushPropLit(const TERM& lit)");
      ASSERT(!_debugLiteralComplete);
      _size = 1;
      _openFlatterm.PushConst(lit);
      _header = lit;
      ASSERT(!_weightCollected);
      DOP(_debugLiteralComplete = true);
    };
   
  void endOfLiteral()
    {
      CALL("endOfLiteral()");
      DOP(_debugLiteralComplete = true);
      ASSERT(!_weightCollected);
      if (header().IsEquality())
	{
	  resetArguments();
	  resetArgWeights();
	};
    };




  bool argsAreEqual()
    {
      CALL("argsAreEqual()");
      ASSERT(isEquality());
      return arg1()->equal(arg2());
    };
    
  bool argsAreEqual(const Clause* const * commutativity_table)
    {
      CALL("argsAreEqual(const Clause* const * commutativity_table)");
      ASSERT(isEquality());
      return arg1()->equal(arg2(),commutativity_table);
    };

  bool equal(TMP_LITERAL& lit) const 
    {
      CALL("equal(TMP_LITERAL& lit) const");
      return flatterm()->equal(lit.flatterm()); 
    };

  bool equal(TMP_LITERAL& lit,const Clause* const * commutativity_table) const 
    { 
      CALL("equal(TMP_LITERAL& lit,const Clause* const * commutativity_table) const");
      return flatterm()->equal(lit.flatterm(),commutativity_table); 
    };

  bool isComplementary(TMP_LITERAL& lit) const 
    {
      CALL("isComplementary(TMP_LITERAL& lit) const"); 
      return flatterm()->Complementary(lit.flatterm()); 
    };
  bool isComplementary(TMP_LITERAL& lit,const Clause* const * commutativity_table) const 
    { 
      CALL("isComplementary(TMP_LITERAL& lit,const Clause* const * commutativity_table) const");
      return flatterm()->Complementary(lit.flatterm(),commutativity_table); 
    };

  bool complementaryEqLit(TMP_LITERAL& lit) const;
  bool complementaryEqLit(TMP_LITERAL& lit,const Clause* const * commutativity_table) const;  
  bool equalEqLit(TMP_LITERAL& lit) const;
  bool equalEqLit(TMP_LITERAL& lit,const Clause* const * commutativity_table) const;

  bool instantiationPossible(ulong var,unsigned long substDepth,long substSize)
    {
      CALL("instantiationPossible(ulong var,unsigned long substDepth,long substSize)");
      ASSERT(substDepth);
      ulong occs = (ulong)(variableOccurences().coefficient(var));
      if (occs)
	{     
	  ulong newSize = size() + (substSize * occs) - occs;
	  return (newSize <= VampireKernelConst::MaxTermSize) && 
	    (depth() + substDepth <= VampireKernelConst::MaxTermDepth);
	}
      else 
	return true;
    }; // instantiationPossible(ulong var,unsigned long substDepth,long substSize)


  bool instantiateVar(TMP_LITERAL& new_lit,ulong var,Flatterm* subst_term);

  void renameVariables(INT_VAR_RENAMING& var_renaming)
    {
      if (isEquality())
	{
	  arg1()->renameVariables(var_renaming);
	  arg2()->renameVariables(var_renaming);
	}
      else flatterm()->renameVariables(var_renaming);
    };

  bool preferredNegative(TMP_LITERAL& lit);


  Comparison compare(TMP_LITERAL& lit);
  Comparison compareWithVarWeighting(TMP_LITERAL& lit,const VAR_WEIGHTING& var_weighting);

  void orderEqArgs();

  
  void collectVarDepthProfile(VarDepthProfile& profile)
    {
      CALL("collectVarDepthProfile(VarDepthProfile& profile)");
      profile.reset();    
      if (_weightCollected)
	{
	  if (_weight.monomials().isZero()) return;
	}
      else
	if (_variableOccurencesCollected)
	  {
	    if (_variableOccurences.isZero()) return; 
	  };
         
      Flatterm::Iterator iter(flatterm());
      SmartDepthCounter<VampireKernelConst::MaxTermDepth> depth_counter;
      depth_counter.Reset(1UL); 
      while (iter.NotEndOfTerm())
	{
	  depth_counter.NextSym(iter.CurrPos()->symbolRef());  
	  if (iter.CurrPos()->isVariable())
	    { 
	      profile.add(depth_counter.Depth());      
	    };
   
	  iter.NextSym();
	};
    }; // void collectVarDepthProfile(VarDepthProfile& profile)
 

  ostream& output(ostream& str) const;

 private:
  void* operator new(size_t);
  void operator delete(void*,size_t);
  
  const Flatterm* rawArg1() const
    {
      CALL("rawArg1() const");
      ASSERT(_debugLiteralComplete);
      return flatterm() + 1;
    };  

  Flatterm* rawArg1()
    {
      CALL("rawArg1()");
      ASSERT(_debugLiteralComplete);
      return flatterm() + 1;
    };
  
  const Flatterm* rawArg2() const
    {
      CALL("rawArg2() const");
      ASSERT(_debugLiteralComplete);
      return rawArg1()->after();
    };   

  Flatterm* rawArg2()
    {
      CALL("rawArg2()");
      ASSERT(_debugLiteralComplete);
      return rawArg1()->after();
    };   

  bool argumentsSwapped() const { return _arg1 == rawArg2(); };

  void swapArguments() 
    { 
      CALL("swapArguments()");
      ASSERT(_arg1 == rawArg1());
      ASSERT(_arg2 == rawArg2());
      _arg1 = rawArg2(); 
      _arg2 = rawArg1(); 

      ASSERT((!_arg1Weight) || (_arg1Weight == &_rawArg1Weight));
      ASSERT((!_arg2Weight) || (_arg2Weight == &_rawArg2Weight));
      WeightPolynomial* tmp = _arg1Weight;
      _arg1Weight = _arg2Weight;
      _arg2Weight = tmp;
    }; 

  void resetArguments() 
    { 
      _arg1 = rawArg1();
      _arg2 = rawArg2();
    };

  void resetArgWeights()
    {
      _arg1Weight = (WeightPolynomial*)0;
      _arg2Weight = (WeightPolynomial*)0;
    };

  void makeEqOrdered()
    {
      CALL("makeEqOrdered()");
      ASSERT(isEquality());
      _header.MakeOrdered();
      ((Flatterm*)flatterm())->symbolRef() = _header;
    };

  void collectWeight() 
    {
      CALL("collectWeight()");
      ASSERT(!_weightCollected);
      _weight.reset();
      TermWeighting::current()->collectWeight(_weight,flatterm());
      _weightCollected = true;
    };

  void collectVariableOccurences()
    {
      CALL("collectVariableOccurences()");
      ASSERT(!_variableOccurencesCollected);
      _variableOccurences.reset();
      for (Flatterm::Iterator iter(flatterm());
	   iter.NotEndOfTerm();
	   iter.NextSym())
	{
	  if (iter.CurrPos()->symbolRef().isVariable())
	    {
	      _variableOccurences.add1(iter.CurrPos()->symbolRef().var());
	    };
	};
      _variableOccurencesCollected = true;
    };
  

  void collectArg1Weight()
    {
      CALL("collectArg1Weight()");
      ASSERT(!_arg1Weight);
      if (argumentsSwapped()) { _arg1Weight = &_rawArg2Weight; }
      else
	_arg1Weight = &_rawArg1Weight;

      _arg1Weight->reset();
      TermWeighting::current()->collectWeight(*_arg1Weight,arg1());
    };  
  
  void collectArg2Weight()
    {
      CALL("collectArg2Weight()");
      ASSERT(!_arg2Weight);
      if (argumentsSwapped()) { _arg2Weight = &_rawArg1Weight; }
      else
	_arg2Weight = &_rawArg2Weight;
      _arg2Weight->reset();
      TermWeighting::current()->collectWeight(*_arg2Weight,arg2());
    };


  Comparison compareEq(TMP_LITERAL& lit);
  Comparison compareEqOO(TMP_LITERAL& lit);
  Comparison compareEqOU(TMP_LITERAL& lit);
  Comparison compareEqUU(TMP_LITERAL& lit);
  

  bool preferredNegativeEq(TMP_LITERAL& lit);
  bool preferredNegativeNoneq(TMP_LITERAL& lit);

 private:
  TERM _header;
  Flatterm::OpenWithMemory _openFlatterm;
  ulong _size;
   
  bool _weightCollected;
  WeightPolynomial _weight;
  bool _variableOccurencesCollected;
  WeightPolynomial::Monomials _variableOccurences;

  Flatterm* _arg1;
  Flatterm* _arg2;
  WeightPolynomial* _arg1Weight;
  WeightPolynomial* _arg2Weight;

  WeightPolynomial _rawArg1Weight;
  WeightPolynomial _rawArg2Weight;

#ifdef DEBUG_NAMESPACE
  bool _debugLiteralComplete;
#endif

  bool _inherentlySelected;

  TMP_LITERAL* _previous;
  TMP_LITERAL* _next;

#ifndef NO_DEBUG
  TmpLitList* _currentList;
#endif
   
  TMP_LITERAL* _nextFree;
  static TMP_LITERAL* _freeLiterals;
}; // class TMP_LITERAL 
 
}; // namespace VK

inline ostream& operator<<(ostream& str,const VK::TMP_LITERAL& lit) 
{
  return lit.output(str);
};

inline ostream& operator<<(ostream& str,const VK::TMP_LITERAL* lit) 
{
  return lit->output(str);
};


//******************* Definitions for TMP_LITERAL: ************************** 


namespace VK
{

inline void TMP_LITERAL::orderEqArgs()
{
  CALL("orderEqArgs()");
  ASSERT(isEquality());

  if (isOrderedEquality()) return; // already ordered  
  
  ASSERT(arg1() == rawArg1());
  ASSERT(arg2() == rawArg2());
  
  switch (SimplificationOrdering::current()->compare(arg1(),arg1Weight(),
					  arg2(),arg2Weight()))
    {
    case Less:
      swapArguments();
      makeEqOrdered();
      return;
  
    case Equal: return;
        
    case Greater:
      makeEqOrdered();
      return;

    case Incomparable: return;
    };


#ifdef DEBUG_NAMESPACE
  ICP("ICP0");  
  return;          
#else
#ifdef _SUPPRESS_WARNINGS_
  return;
#endif
#endif
           
}; // void TMP_LITERAL::orderEqArgs()

inline bool TMP_LITERAL::equalEqLit(TMP_LITERAL& lit) const 
{
  CALL("equalEqLit(TMP_LITERAL& lit) const");
  // both this and lit must be equational literals
  if (polarity() != lit.polarity()) return false;

  if (arg1()->equal(lit.arg1()))
    {   
      return arg2()->equal(lit.arg2());
    }
  else
    {
      if ((isUnorderedEquality()) || (lit.isUnorderedEquality()))
	{
	  return (arg1()->equal(lit.arg2())) && (arg2()->equal(lit.arg1()));
	}
      else return false; 
    }; 
}; // bool TMP_LITERAL::equalEqLit(TMP_LITERAL& lit) const 
    
inline bool TMP_LITERAL::equalEqLit(TMP_LITERAL& lit,const Clause* const * commutativity_table) const 
{
  CALL("equalEqLit(TMP_LITERAL& lit,const Clause* const * commutativity_table) const");
  // both this and lit must be equational literals
  if (polarity() != lit.polarity()) return false;
  if (arg1()->equal(lit.arg1(),commutativity_table))
    {
      return arg2()->equal(lit.arg2(),commutativity_table);
    }
  else
    {
      if ((isUnorderedEquality()) || (lit.isUnorderedEquality()))
	{
	  return (arg1()->equal(lit.arg2(),commutativity_table)) 
	    && (arg2()->equal(lit.arg1(),commutativity_table));
	}
      else return false; 
    }; 
}; // bool TMP_LITERAL::equalEqLit(TMP_LITERAL& lit,const Clause* const * commutativity_table) const 


inline bool TMP_LITERAL::complementaryEqLit(TMP_LITERAL& lit) const 
{
  CALL("complementaryEqLit(TMP_LITERAL& lit) const");
  // both this and lit must be equational literals
  if (polarity() == lit.polarity()) return false;
  if (arg1()->equal(lit.arg1()))
    {
      return arg2()->equal(lit.arg2());
    }
  else
    {
      if ((isOrderedEquality()) && (lit.isOrderedEquality()))
	{
	  return false;
	}
      else return (arg1()->equal(lit.arg2())) && (arg2()->equal(lit.arg1()));
    };
}; // bool TMP_LITERAL::complementaryEqLit(TMP_LITERAL& lit) const 
 

inline bool TMP_LITERAL::complementaryEqLit(TMP_LITERAL& lit,const Clause* const * commutativity_table) const 
{
  CALL("complementaryEqLit(TMP_LITERAL& lit,const Clause* const * commutativity_table) const");
  // both this and lit must be equational literals
  if (polarity() == lit.polarity()) return false;
  if (arg1()->equal(lit.arg1(),commutativity_table))
    {
      return arg2()->equal(lit.arg2(),commutativity_table);
    }
  else
    {
      if ((isOrderedEquality()) && (lit.isOrderedEquality()))
	{
	  return false;
	}
      else return (arg1()->equal(lit.arg2(),commutativity_table)) && (arg2()->equal(lit.arg1(),commutativity_table));
    };
}; // bool TMP_LITERAL::complementaryEqLit(TMP_LITERAL& lit,const Clause* const * commutativity_table) const 



inline
Comparison TMP_LITERAL::compare(TMP_LITERAL& lit)
{
  CALL("compare(TMP_LITERAL& lit) const");
  
  if (isEquality())
    {
      if ((lit.isEquality())
	  && (polarity() == lit.polarity()))
	return compareEq(lit); 
      return (isPositive()) ? Less : Greater;
    }
  else
    if (lit.isEquality())
      {
	return (lit.isPositive()) ? Greater : Less;
      };

  // both are not equations
 
  return SimplificationOrdering::current()->compareAtoms(flatterm(),
					      weight(),
					      lit.flatterm(),
					      lit.weight());

}; // Comparison TMP_LITERAL::compare(TMP_LITERAL& lit)



inline 
Comparison TMP_LITERAL::compareEq(TMP_LITERAL& lit)
{
  CALL("compareEq(TMP_LITERAL& lit)");
  ASSERT(isEquality()); 
  ASSERT(lit.isEquality());
  ASSERT(header().polarity() == lit.header().polarity());

  if (isOrderedEquality())
    {
      if (lit.isOrderedEquality()) return compareEqOO(lit); 
      return compareEqOU(lit);
    }; 
  // this is unordered  

  if (lit.isOrderedEquality()) return inverse(lit.compareEqOU(*this)); 
  // both are unordered
  return compareEqUU(lit);  
}; // Comparison TMP_LITERAL::compareEq(TMP_LITERAL& lit)


inline 
Comparison TMP_LITERAL::compareEqOO(TMP_LITERAL& lit)
{
  CALL("compareEqOO(TMP_LITERAL& lit)");
  ASSERT(isEquality());
  ASSERT(isOrderedEquality());
  ASSERT(header() == lit.header());



  Comparison cmp11 = 
    SimplificationOrdering::current()->compare(arg1(),arg1Weight(),
				    lit.arg1(),lit.arg1Weight()); 
  if (cmp11 != Equal) return cmp11;
  return SimplificationOrdering::current()->compare(arg2(),arg2Weight(),
					 lit.arg2(),lit.arg2Weight()); 
}; // Comparison TMP_LITERAL::compareEqOO(TMP_LITERAL& lit)




inline 
Comparison TMP_LITERAL::compareEqOU(TMP_LITERAL& lit)
{
  CALL("compareEqOU(TMP_LITERAL& lit)");
  // in the comments below * means "incomparable"
  ASSERT(isEquality());
  ASSERT(lit.isEquality());
  ASSERT(isOrderedEquality());
  ASSERT(lit.isUnorderedEquality());
  ASSERT(isPositive() == lit.isPositive());

  // arg1() > arg2()
  switch (SimplificationOrdering::current()->compare(arg1(),arg1Weight(),
					  lit.arg1(),lit.arg1Weight()))
    {
    case Less:
      // arg2() < arg1() < lit.arg1()
      return Less;
	  
    case Equal: 
      // arg1() = lit.arg1() * lit.arg2(), compare arg2() with lit.arg2()
      return SimplificationOrdering::current()->compare(arg2(),arg2Weight(),
					     lit.arg2(),lit.arg2Weight());	
    case Greater: 
      // arg1() > lit.arg1(), compare arg1() with lit.arg2()
      return SimplificationOrdering::current()->compare(arg1(),arg1Weight(),
					     lit.arg2(),lit.arg2Weight());
    case Incomparable:         
      // compare arg2() with lit.arg1() and lit.arg2()
      switch (SimplificationOrdering::current()->compare(arg2(),arg2Weight(),
					      lit.arg1(),lit.arg1Weight()))
	{
	case Less: 
	  if (SimplificationOrdering::current()->lessOrEqual(arg1(),arg1Weight(),
						  lit.arg2(),lit.arg2Weight()))
	    {
	      return Less;
	    }
	  else
	    return Incomparable;

	case Equal:
	  return SimplificationOrdering::current()->compare(arg1(),arg1Weight(),
						 lit.arg2(),lit.arg2Weight());

	case Greater:
	  // arg1() * lit.arg1(), arg2() > lit.arg1()
	  switch (SimplificationOrdering::current()->compare(arg1(),arg1Weight(),
						  lit.arg2(),lit.arg2Weight()))
	    {
	    case Less: return Less;
	    case Equal: return Greater;
	    case Greater: return Greater;
	    case Incomparable: 
	      if (SimplificationOrdering::current()->greaterOrEqual(arg2(),arg2Weight(),
							 lit.arg2(),lit.arg2Weight()))
		{
		  return Greater;
		}
	      else
		return Incomparable;
	    };

	case Incomparable:
	  // arg1() * lit.arg1(), arg2() * lit.arg1()
	  if (SimplificationOrdering::current()->less(arg1(),arg1Weight(),
					   lit.arg2(),lit.arg2Weight()))
	    {
	      return Less;
	    }
	  else
	    return Incomparable;
	};

    };


#ifdef DEBUG_NAMESPACE 
  ERROR("DE100");
  return Incomparable; 
#else
#ifdef _SUPPRESS_WARNINGS_
  return Incomparable; 
#endif  
#endif 
}; // Comparison TMP_LITERAL::compareEqOU(TMP_LITERAL& lit)





inline 
Comparison TMP_LITERAL::compareEqUU(TMP_LITERAL& lit)
{
  CALL("compareEqUU(TMP_LITERAL& lit)");
  // compare {arg1(), arg2()} with {lit.arg1(), lit.arg2()}
  // "*" in the comments means "incomparable"
  ASSERT(isEquality());
  ASSERT(lit.isEquality());
  ASSERT(isUnorderedEquality());
  ASSERT(lit.isUnorderedEquality());
  ASSERT(isPositive() == lit.isPositive());

  Comparison cmp = SimplificationOrdering::current()->compare(arg1(),arg1Weight(),lit.arg1(),lit.arg1Weight());
  switch (cmp)
    {
    case Greater:
      // arg1() > lit.arg1()
      cmp = SimplificationOrdering::current()->compare(arg1(),arg1Weight(),lit.arg2(),lit.arg2Weight()); 
      switch (cmp)
	{
	case Greater:
	  // arg1() > lit.arg1(), arg1() > lit.arg2()
	  return Greater;
        
	case Less:
	  // arg1() > lit.arg1(), arg1() < lit.arg2(). Not an error since v2.73
	  return SimplificationOrdering::current()->compare(arg2(),arg2Weight(),lit.arg2(),lit.arg2Weight()); 
       
	case Incomparable:
	  // arg1() > lit.arg1(), arg1() * lit.arg2()
	  cmp = SimplificationOrdering::current()->compare(arg2(),arg2Weight(),lit.arg2(),lit.arg2Weight()); 
	  switch (cmp)
	    {
	    case Greater:
	      // arg1() > lit.arg1(), arg1() * lit.arg2(), arg2() > lit.arg2()
	      return Greater;
            
	    case Less:
	      // arg1() > lit.arg1(), arg1() * lit.arg2(), arg2() < lit.arg2()
	      return Incomparable;
           
	    case Incomparable:
	      // arg1() > lit.arg1(), arg1() * lit.arg2(), arg2() * lit.arg2()
	      return Incomparable;
           
	    case Equal:
	      // arg1() > lit.arg1(), arg1() * lit.arg2(), arg2() = lit.arg2()
	      return Greater;
	    };
        
	case Equal:
	  // arg1() > lit.arg1(), arg1() = lit.arg2() : error because lit.arg1() * lit.arg2()
#ifdef DEBUG_NAMESPACE 
	  ERROR("DE20"); 
#endif
	  return Incomparable;
	};

    case Less:
      // arg1() < lit.arg1()
      cmp = SimplificationOrdering::current()->compare(arg2(),arg2Weight(),lit.arg1(),lit.arg1Weight()); 
      switch (cmp)
	{
	case Greater:
	  // arg1() < lit.arg1(), arg2() > lit.arg1(). Not an error since v2.73
	  return SimplificationOrdering::current()->compare(arg2(),arg2Weight(),lit.arg2(),lit.arg2Weight()); 
        
	case Less:
	  // arg1() < lit.arg1(), arg2() < lit.arg1()
	  return Less; // was Greater in the CASC-16 version (obvious incompleteness)
        
	case Incomparable:
	  // arg1() < lit.arg1(), arg2() * lit.arg1()
	  cmp = SimplificationOrdering::current()->compare(arg2(),arg2Weight(),lit.arg2(),lit.arg2Weight()); 
	  switch (cmp)
	    {
	    case Greater:
	      // arg1() < lit.arg1(), arg2() * lit.arg1(), arg2() > lit.arg2()
	      return Incomparable;
           
	    case Less:
	      // arg1() < lit.arg1(), arg2() * lit.arg1(), arg2() < lit.arg2()
	      return Less;
           
	    case Incomparable:
	      // arg1() < lit.arg1(), arg2() * lit.arg1(), arg2() * lit.arg2()
	      return Incomparable;
           
	    case Equal:
	      // arg1() < lit.arg1(), arg2() * lit.arg1(), arg2() = lit.arg2()
	      return Less; 
	    }; 
        
	case Equal:
	  // arg1() < lit.arg1(), arg2() = lit.arg1() : error because arg1() * arg2()
#ifdef DEBUG_NAMESPACE 
	  ERROR("DE40");
#endif
	  return Incomparable;
	};
      
    case Incomparable:
      // arg1() * lit.arg1()
      cmp = SimplificationOrdering::current()->compare(arg1(),arg1Weight(),lit.arg2(),lit.arg2Weight()); 
      switch (cmp)
	{
	case Greater:
	  // arg1() * lit.arg1(), arg1() > lit.arg2()
	  cmp = SimplificationOrdering::current()->compare(arg2(),arg2Weight(),lit.arg1(),lit.arg1Weight()); 
	  switch (cmp)
	    {
	    case Greater:
	      // arg1() * lit.arg1(), arg1() > lit.arg2(), arg2() > lit.arg1()
	      return Greater; 
           
	    case Less:
	      // arg1() * lit.arg1(), arg1() > lit.arg2(), arg2() < lit.arg1()
	      return Incomparable;
           
	    case Incomparable:
	      // arg1() * lit.arg1(), arg1() > lit.arg2(), arg2() * lit.arg1()
	      return Incomparable;
           
	    case Equal:
	      // arg1() * lit.arg1(), arg1() > lit.arg2(), arg2() = lit.arg1()
	      return Greater;
	    };
          
	case Less:
	  // arg1() * lit.arg1(), arg1() < lit.arg2()
	  cmp = SimplificationOrdering::current()->compare(arg2(),arg2Weight(),lit.arg1(),lit.arg1Weight()); 
	  switch (cmp)
	    {
	    case Greater:
	      // arg1() * lit.arg1(), arg1() < lit.arg2(), arg2() > lit.arg1()
	      cmp = SimplificationOrdering::current()->compare(arg2(),arg2Weight(),lit.arg2(),lit.arg2Weight());
	      ASSERT(cmp != Equal);
	      return cmp;
          
	    case Less:
	      // arg1() * lit.arg1(), arg1() < lit.arg2(), arg2() < lit.arg1()
	      return Less;
            
	    case Incomparable:
	      // arg1() * lit.arg1(), arg1() < lit.arg2(), arg2() * lit.arg1()
	      cmp = SimplificationOrdering::current()->compare(arg2(),arg2Weight(),lit.arg2(),lit.arg2Weight()); 
	      if (cmp == Less) return Less;
	      return Incomparable;
            
	    case Equal:
	      // arg1() * lit.arg1(), arg1() < lit.arg2(), arg2() = lit.arg1()
	      return Less; 
	    }; 
         

	case Incomparable:
	  // arg1() * lit.arg1(), arg1() * lit.arg2()
	  cmp = SimplificationOrdering::current()->compare(arg2(),arg2Weight(),lit.arg1(),lit.arg1Weight()); 
	  switch (cmp)
	    {
	    case Greater:
	      // arg1() * lit.arg1(), arg1() * lit.arg2(), arg2() > lit.arg1()
	      cmp = SimplificationOrdering::current()->compare(arg2(),arg2Weight(),lit.arg2(),lit.arg2Weight()); 
	      if (cmp == Greater) return Greater;
	      ASSERT(cmp != Equal);
	      return Incomparable;
           
	    case Less:
	      // arg1() * lit.arg1(), arg1() * lit.arg2(), arg2() < lit.arg1()
	      return Incomparable;
           
	    case Incomparable:
	      // arg1() * lit.arg1(), arg1() * lit.arg2(), arg2() * lit.arg1()
	      return Incomparable;
           
	    case Equal:
	      // arg1() * lit.arg1(), arg1() * lit.arg2(), arg2() = lit.arg1()
	      return Incomparable;
	    }; 
        
	case Equal:
	  // arg1() * lit.arg1(), arg1() = lit.arg2()
	  cmp = SimplificationOrdering::current()->compare(arg2(),arg2Weight(),lit.arg1(),lit.arg1Weight()); 
	  return cmp;
	};
      
    case Equal:
      // arg1() = lit.arg1()
      cmp = SimplificationOrdering::current()->compare(arg2(),arg2Weight(),lit.arg2(),lit.arg2Weight()); 
      return cmp;
    };
#ifdef DEBUG_NAMESPACE 
  ICP("DE100");
  return Incomparable; 
#else
#ifdef _SUPPRESS_WARNINGS_
  return Incomparable; 
#endif  
#endif
   
}; // Comparison TMP_LITERAL::compareEqUU(TMP_LITERAL& lit)






inline bool TMP_LITERAL::preferredNegativeEq(TMP_LITERAL& lit)
{
  CALL("preferredNegativeEq(TMP_LITERAL& lit)");
  ASSERT(isNegative());
  ASSERT(isEquality());
  ASSERT(lit.isNegative());
  ASSERT(lit.isEquality());
  ulong size1;
  ulong size2;
  if (isOrderedEquality()) { size1 = arg1()->size(); }
  else size1 = size() - 1; 
  if (lit.isOrderedEquality()) { size2 = lit.arg1()->size(); }
  else size2 = lit.size() - 1;   
  if (size1 > size2) return false;
  if (size1 < size2) return true;
  // size1 == size2
  return true; // can be refined  
}; // bool TMP_LITERAL::preferredNegativeEq(TMP_LITERAL& lit)

inline bool TMP_LITERAL::preferredNegativeNoneq(TMP_LITERAL& lit)
{
  CALL("preferredNegativeNoneq(TMP_LITERAL& lit)");
  ASSERT(_debugLiteralComplete);
  ASSERT(lit._debugLiteralComplete);
  ASSERT(!isEquality());
  ASSERT(!lit.isEquality());
  ulong size1;
  ulong size2;
  Comparison lexCmp;
  // switch (GlobalHeaderWeighting.Compare(header(),lit.header()))
  switch (Signature::current()->compareHeaderPrecedences(header().polarity(),
						header().functor(),
						lit.header().polarity(),
						lit.header().functor()))
    {
    case Greater: return true;
    case Less: return false;
    case Incomparable: ICP("ICP0"); return false;
    case Equal: 
      size1 = size();
      size2 = lit.size();
      if (size1 > size2) return false;
      if (size1 < size2) return true;
      lexCmp = flatterm()->CompareLex(lit.flatterm());
      if (lexCmp == Less) return true;
      if (lexCmp == Greater) return false;
      return true; // can be refined
    };
#ifdef DEBUG_NAMESPACE
  ICP("ICP1");
  return false;
#else
#ifdef _SUPPRESS_WARNINGS_
  return false;
#endif
#endif
}; // bool TMP_LITERAL::preferredNegativeNoneq(TMP_LITERAL& lit) 


inline bool TMP_LITERAL::preferredNegative(TMP_LITERAL& lit)
{
  CALL("preferredNegative(TMP_LITERAL& lit)"); 
  ASSERT(_debugLiteralComplete);
  ASSERT(lit._debugLiteralComplete);
  if (isEquality())
    { 
      ASSERT(isNegative());
      if (lit.isEquality())
	{
	  ASSERT(lit.isNegative());
	  // both are negative equalities
	  return preferredNegativeEq(lit);     
	}
      else return true;
    }
  else
    if (lit.isEquality()) { return false; }
    else // both are not equalities
      {
	return preferredNegativeNoneq(lit);
      };
}; // bool TMP_LITERAL::preferredNegative(TMP_LITERAL& lit)
   
 
inline Comparison TMP_LITERAL::compareWithVarWeighting(TMP_LITERAL& lit,const VAR_WEIGHTING& var_weighting)
{
  CALL("compareWithVarWeighting(TMP_LITERAL& lit,const VAR_WEIGHTING& var_weighting)");
  ASSERT(_debugLiteralComplete);
  ASSERT(lit._debugLiteralComplete);

  Comparison hd_cmp = Signature::current()->compareHeaderPrecedences(header().polarity(),
							    header().functor(),
							    lit.header().polarity(),
							    lit.header().functor());

  if (hd_cmp != Equal) return hd_cmp; 
  
  // here the headers are equal
  if (size() > lit.size()) return Greater; 
  if (size() < lit.size()) return Less; 
  
  /********* Was before v3.05 :
  // here the sizes are equal
  
  if (_vars.Size() < lit._vars.Size()) return Greater; 
  if (_vars.Size() > lit._vars.Size()) return Less; 
  
  // here both terms have same number of variable occurences
  if (_vars.NumOfDifferent() < lit._vars.NumOfDifferent()) return Greater; 
  if (_vars.NumOfDifferent() > lit._vars.NumOfDifferent()) return Less; 
  *******************/

  // compare lexicographically
  Comparison var_cmp;
  const Flatterm* fst = flatterm() + 1;
  const Flatterm* const end_of_fst = flatterm()->after();
  const Flatterm* snd = lit.flatterm();
 check_end:
  if (fst == end_of_fst) return Incomparable;
  snd++;
  if (fst->Symbol() != snd->Symbol())
    {
      if (fst->Symbol().isVariable()) 
	{
	  if (snd->Symbol().isVariable()) 
	    {
	      // different variables
	      var_cmp = var_weighting.Compare(fst->Symbol().var(),snd->Symbol().var());
	      if (var_cmp != Equal) { return var_cmp; };
	    }
	  else 
	    return Less; // a variable is less than a functor
	}
      else 
	{
	  if (snd->Symbol().isVariable()) return Greater;
	  // both are functors
	  return fst->Symbol().comparePrecedence(snd->Symbol()); 
	}; 
    };
  fst++;
  goto check_end;
}; // Comparison TMP_LITERAL::compareWithVarWeighting(TMP_LITERAL& lit,const VAR_WEIGHTING& var_weighting)

static TMP_LITERAL::InitStatic tmpLiteralInitStatic;

}; // namespace VK



//===========================================================
#endif
