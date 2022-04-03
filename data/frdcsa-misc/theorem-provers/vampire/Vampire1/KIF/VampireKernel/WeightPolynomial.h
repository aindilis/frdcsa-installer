//
// File:         WeightPolynomial.h
// Description:  Operations on weights of terms.
// Created:      Apr 11, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//===================================================================
#ifndef WEIGHT_POLYNOMIAL_H
#define WEIGHT_POLYNOMIAL_H
//===================================================================
#ifndef NO_DEBUG_VIS
#include <iostream.h>
#endif
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "VampireKernelConst.h"
#include "RuntimeError.h"
#include "LinearPolynomial.h"
#include "Comparison.h"
#include "SharedLinearPolynomial.h"
#include "GlobAlloc.h"
#include "DestructionMode.h"
#include "TermWeightType.h"
//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_WEIGHT_POLYNOMIAL
 #define DEBUG_NAMESPACE "WeightPolynomial"
#endif
#include "debugMacros.h"
//===================================================================

namespace VK
{
  using namespace BK;

class WeightPolynomial
{
 public:
  typedef LinearCombination<TermWeightType,VampireKernelConst::MaxNumOfVariables * VampireKernelConst::MaxNumOfIndices> Monomials;
  typedef SharedLinearPolynomial<GlobAlloc,TermWeightType,VampireKernelConst::MaxNumOfVariables * VampireKernelConst::MaxNumOfIndices> Stored;
  typedef LinearPolynomialSharing<GlobAlloc,TermWeightType,VampireKernelConst::MaxNumOfVariables * VampireKernelConst::MaxNumOfIndices> Sharing;
  class SpecialisedComparison;
 public:
  WeightPolynomial() {};
  ~WeightPolynomial() {};
  static void setSharing(Sharing* s) 
    { 
      _sharing = s; 
    };
  void reset() { _linearPolynomial.reset(); };
  Monomials& monomials() { return _linearPolynomial.monomials(); };
  const Monomials& monomials() const { return _linearPolynomial.monomials(); };
  const TermWeightType& coefficient(ulong var) const 
    { 
      return monomials().coefficient(var); 
    };
  bool containsVariable(ulong var) const { return (bool)coefficient(var); };
  bool isConstant() const { return monomials().isZero(); };
  bool isZero() const { return isConstant() && (!freeMember()); };
  ulong numberOfVariables() const { return monomials().length(); };

  TermWeightType& freeMember() { return _linearPolynomial.freeMember(); };
  const TermWeightType& freeMember() const { return _linearPolynomial.freeMember(); };
  TermWeightType minimalValue() const
    {
      // here we assume that the minimal term weight is 1
      return freeMember() + monomials().sumOfCoefficients();
    };
  FunctionComparison compare(const WeightPolynomial& p) const;
  FunctionComparison compareForGreaterOrEqual(const WeightPolynomial& p) const;
  FunctionComparison compareForLessOrEqual(const WeightPolynomial& p) const;
  bool isAlwaysGreater(const WeightPolynomial& p) const;
  bool isNeverLess(const WeightPolynomial& p) const;
  bool isAlwaysLess(const WeightPolynomial& p) const
    {
      CALL("isAlwaysLess(const WeightPolynomial& p) const");
      return p.isAlwaysGreater(*this);
    };
  
  bool isNeverGreater(const WeightPolynomial& p) const
    {
      CALL("isNeverGreater(const WeightPolynomial& p) const");
      return p.isNeverLess(*this);
    };
  bool containsVariableWhichIsNotIn(const WeightPolynomial& wp) const
    {
      return _linearPolynomial.containsVariableWhichIsNotIn(wp._linearPolynomial);
    };

  void add(const TermWeightType& constant) { freeMember() += constant; };
  void add1() { ++freeMember(); };
  void add(const TermWeightType& coeff,ulong var) { monomials().add(coeff,var); };
  void add1(ulong var) { monomials().add1(var); };

  Stored* createStoredCopy() const;
  static void destroyStoredCopy(Stored* polynomial);
#ifndef NO_DEBUG_VIS
  ostream& output(ostream& str) const
    {
      return str << _linearPolynomial;
    };
#endif
  

 private:
  LinearPolynomial<TermWeightType,VampireKernelConst::MaxNumOfVariables * VampireKernelConst::MaxNumOfIndices> _linearPolynomial;
  friend class SpecialisedComparison;
  static Sharing* _sharing;
}; // class WeightPolynomial 
}; // namespace VK

#ifndef NO_DEBUG_VIS

inline
ostream& operator<<(ostream& str,
		    const VK::WeightPolynomial& wp)
{
  return wp.output(str);
};

#endif

//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_WEIGHT_POLYNOMIAL
 #define DEBUG_NAMESPACE "WeightPolynomial::SpecialisedComparison"
#endif
#include "debugMacros.h"
//===================================================================

namespace VK
{
class WeightPolynomial::SpecialisedComparison
{
 public:
  class Stored;
  class Command;
 public:
  SpecialisedComparison() {};
  ~SpecialisedComparison() {};

  void compare(const WeightPolynomial& weight1,const WeightPolynomial& weight2);
  const FunctionComparison& category() const { return _category; };  
  const WeightPolynomial& positivePart() const { return _positivePart; };
  const WeightPolynomial& negativePart() const { return _negativePart; };
  bool compileGreaterOrEqual(Command*& code,ulong maxCodeSize) const
    {
      return compileGreaterOrEqual(category(),positivePart(),negativePart(),
				   code,maxCodeSize);
    };
 private:
  static bool compileGreaterOrEqual(const FunctionComparison& cat,
				    const WeightPolynomial& posPart,
				    const WeightPolynomial& negPart,
				    Command*& code,ulong maxCodeSize);
 private:
  FunctionComparison _category;
  WeightPolynomial _positivePart;
  WeightPolynomial _negativePart;
}; // class WeightPolynomial::SpecialisedComparison

}; // namespace VK


#ifndef NO_DEBUG_VIS

inline
ostream& operator<<(ostream& str,
		    const VK::WeightPolynomial::SpecialisedComparison& sc)
{
  str << "{" << sc.positivePart() << "} vs {"
      << sc.negativePart() << "} ~~ " << sc.category();
  return str;
};

#endif

//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_WEIGHT_POLYNOMIAL
 #define DEBUG_NAMESPACE "WeightPolynomial::SpecialisedComparison::Stored"
#endif
#include "debugMacros.h"
//===================================================================

namespace VK
{

class WeightPolynomial::SpecialisedComparison::Stored
{
 public:
  Stored() 
    { 

      init(); 
    };
  Stored(const WeightPolynomial::SpecialisedComparison& raw)
    {

      init(raw);
    };
  Stored(const Stored& sc) 
    {

      init(sc); 
    };
  ~Stored()
    {
      CALL("destructor ~Stored()");
      if (DestructionMode::isThorough()) destroy();
    };
  void init() 
    {
      CALL("init()");
      _positivePart = (WeightPolynomial::Stored*)0;
      _negativePart = (WeightPolynomial::Stored*)0;
    };
  void init(const WeightPolynomial::SpecialisedComparison& raw)
    {
      CALL("init(const WeightPolynomial::SpecialisedComparison& raw)");
      _category = raw.category();
      if (strict(_category))
	{	  
          _positivePart = (WeightPolynomial::Stored*)0;
          _negativePart = (WeightPolynomial::Stored*)0;
	}
      else
	{
	  _positivePart = raw.positivePart().createStoredCopy();
	  ASSERT(_positivePart);
	  _negativePart = raw.negativePart().createStoredCopy();
	  ASSERT(_negativePart);
	};
    };
  void init(const Stored& sc)
    {
      CALL("init(const Stored& sc)");
      _category = sc.category();
      _positivePart = sc.positivePart();
      if (_positivePart) _positivePart->incReferenceCounter();
      _negativePart = sc.negativePart();
      if (_negativePart) _negativePart->incReferenceCounter();
    };
  void destroy()
    {
      CALL("destroy()");
      WeightPolynomial::destroyStoredCopy(_positivePart);
      _positivePart = (WeightPolynomial::Stored*)0;
      WeightPolynomial::destroyStoredCopy(_negativePart);
      _negativePart = (WeightPolynomial::Stored*)0;
    };

  Stored& operator=(const Stored& sc)
  {
    CALL("operator=(const Stored& sc)");
    destroy();
    init(sc);
    return *this;
  };


  const FunctionComparison& category() const 
    { 
      return _category; 
    };
  WeightPolynomial::Stored* positivePart() const { return _positivePart; };
  WeightPolynomial::Stored* negativePart() const { return _negativePart; };


 private:
  void* operator new(size_t) { CALL("operator new(size_t)"); ICP("ICP0"); return (void*)0; };
  void operator delete(void* obj) { CALL("operator delete(void* obj)"); ICP("ICP0"); };
 private:
  FunctionComparison _category;
  WeightPolynomial::Stored* _positivePart;
  WeightPolynomial::Stored* _negativePart;
}; // class WeightPolynomial::SpecialisedComparison::Stored

}; // namespace VK

#ifndef NO_DEBUG_VIS


inline
ostream& operator<<(ostream& str,
		    const VK::WeightPolynomial::SpecialisedComparison::Stored& comp)
{
  return
    str << "{" << comp.positivePart() << "} vs {" 
	<< comp.negativePart() << "} ~~ " << comp.category();
};

inline
ostream& operator<<(ostream& str,
		    const VK::WeightPolynomial::SpecialisedComparison::Stored* comp)
{	
  return str << *comp;
};
#endif


//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_WEIGHT_POLYNOMIAL
 #define DEBUG_NAMESPACE "WeightPolynomial"
#endif
#include "debugMacros.h"
//===================================================================

namespace VK
{
inline
FunctionComparison 
WeightPolynomial::compare(const WeightPolynomial& p) const
{
  CALL("compare(const WeightPolynomial& p) const");

  TermWeightType minVal = minimalValue();
  TermWeightType pMinVal = p.minimalValue();
  
  if (minVal < pMinVal)
    {
      // the result is either AlwaysLess or AbsolutelyVolatile
      if (p.monomials().isGreaterOrEqual(monomials()))
	{
	  return AlwaysLess;
	}
      else 
	return AbsolutelyVolatile;
    };
  
  if (minVal > pMinVal)
    {
      // the result is either AlwaysGreater or AbsolutelyVolatile
      if (monomials().isGreaterOrEqual(p.monomials()))
	{
	  return AlwaysGreater;
	}
      else 
	return AbsolutelyVolatile;
    };

  // minVal == pMinVal
  // the result can only be AlwaysEqual, AbsolutelyVolatile, 
  // CanBeLessOrEqual or CanBeGreaterOrEqual
  
  switch (monomials().compare(p.monomials()))
    {
    case Less: return CanBeLessOrEqual;
    case Equal: return AlwaysEqual;
    case Greater: return CanBeGreaterOrEqual; 
    case Incomparable: return AbsolutelyVolatile;
    };
  
  ICP("ICP0");
 
#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE)
  return FailureToCompare;
#endif

}; // FunctionComparison WeightPolynomial::compare(const WeightPolynomial& p) const


inline
FunctionComparison 
WeightPolynomial::compareForGreaterOrEqual(const WeightPolynomial& p) const
{
  CALL("compareForGreaterOrEqual(const WeightPolynomial& p) const");

  TermWeightType minVal = minimalValue();
  TermWeightType pMinVal = p.minimalValue();
  
  if (minVal < pMinVal)
    {
      // either AlwaysLess or AbsolutelyVolatile
      return FailureToCompare; // sufficient
    };
  
  if (minVal > pMinVal)
    {
      // the result is either AlwaysGreater or AbsolutelyVolatile
      if (monomials().isGreaterOrEqual(p.monomials()))
	{
	  return AlwaysGreater;
	}
      else 
	return FailureToCompare;
    };

  // minVal == pMinVal
  // the result can only be AlwaysEqual, AbsolutelyVolatile, 
  // CanBeLessOrEqual or CanBeGreaterOrEqual
  
  switch (monomials().compare(p.monomials()))
    {
    case Less: return FailureToCompare;
    case Equal: return AlwaysEqual;
    case Greater: return CanBeGreaterOrEqual; 
    case Incomparable: return FailureToCompare;
    };
  
  ICP("ICP0");
 
#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE)
  return FailureToCompare;
#endif

}; // FunctionComparison WeightPolynomial::compareForGreaterOrEqual(const WeightPolynomial& p) const




inline
FunctionComparison 
WeightPolynomial::compareForLessOrEqual(const WeightPolynomial& p) const
{
  CALL("compareForLessOrEqual(const WeightPolynomial& p) const");

  TermWeightType minVal = minimalValue();
  TermWeightType pMinVal = p.minimalValue();
  if (minVal < pMinVal)
    {
      // the result is either AlwaysLess or AbsolutelyVolatile
      if (p.monomials().isGreaterOrEqual(monomials()))
	{
	  return AlwaysLess;
	}
      else 
	return FailureToCompare;
    };
  
  if (minVal > pMinVal)
    {
      // either AlwaysGreater or AbsolutelyVolatile
      return FailureToCompare; // sufficient
    };

  // minVal == pMinVal
  // the result can only be AlwaysEqual, AbsolutelyVolatile, 
  // CanBeLessOrEqual or CanBeGreaterOrEqual
  
  switch (monomials().compare(p.monomials()))
    {
    case Less: return CanBeLessOrEqual;
    case Equal: return AlwaysEqual;
    case Greater: return FailureToCompare;
    case Incomparable: return FailureToCompare;
    };
  
  ICP("ICP0");
 
#if (defined _SUPPRESS_WARNINGS_) || (defined DEBUG_NAMESPACE)
  return FailureToCompare;
#endif

}; // FunctionComparison WeightPolynomial::compareForLessOrEqual(const WeightPolynomial& p) const






inline
bool WeightPolynomial::isAlwaysGreater(const WeightPolynomial& p) const
{
  CALL("isAlwaysGreater(const WeightPolynomial& p)");
  if (minimalValue() <= p.minimalValue()) return false;
  // minimalValue() > p.minimalValue()
  return monomials().isGreaterOrEqual(p.monomials());
}; // bool WeightPolynomial::isAlwaysGreater(const WeightPolynomial& p) const


inline
bool WeightPolynomial::isNeverLess(const WeightPolynomial& p) const
{
  CALL("isNeverLess(const WeightPolynomial& p) const");
  if (minimalValue() < p.minimalValue()) return false;
  return monomials().isGreaterOrEqual(p.monomials());
}; // bool WeightPolynomial::isNeverLess(const WeightPolynomial& p) const

inline
WeightPolynomial::Stored* WeightPolynomial::createStoredCopy() const 
{
  CALL("createStoredCopy() const");

  if (isZero()) return (Stored*)0;

  _sharing->resetIntegration();


  if (freeMember()) _sharing->integrConst(freeMember());

  const ulong* endOfVars = monomials().end();
  for (const ulong* v = monomials().begin(); v != endOfVars; ++v)
    { 
      _sharing->integrVar(coefficient(*v),*v);
    };


  Stored* res = _sharing->integrate();

  ASSERT(res->referenceCounter() >= 1L);

  res->incReferenceCounter();

  return res;
}; // WeightPolynomial::Stored* WeightPolynomial::createStoredCopy() const 

inline
void WeightPolynomial::destroyStoredCopy(Stored* polynomial)
{
  CALL("destroyStoredCopy(Stored* polynomial)");
  if (polynomial) 
    {
      ASSERT(polynomial->referenceCounter() >= 1L);
      polynomial->decReferenceCounter();
      if (polynomial->referenceCounter() == 1L) // only in the sharing index	
	_sharing->remove(polynomial);
    };
};
}; // namespace VK

//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_WEIGHT_POLYNOMIAL
 #define DEBUG_NAMESPACE "WeightPolynomial::SpecialisedComparison"
#endif
#include "debugMacros.h"
//===================================================================


namespace VK
{

inline
void 
WeightPolynomial::SpecialisedComparison::compare(const WeightPolynomial& weight1,
						 const WeightPolynomial& weight2)
{
  CALL("compare(..)");
  _positivePart.reset();
  _negativePart.reset();
  const ulong* endOfVars = weight1.monomials().end();
  for (const ulong* v = weight1.monomials().begin();
       v != endOfVars;
       ++v)
    {
      TermWeightType coeffDiff = weight1.coefficient(*v) - weight2.coefficient(*v);
      if (coeffDiff > (TermWeightType)0)
	{
	  _positivePart.add(coeffDiff,*v);
	}
      else
	if (coeffDiff < (TermWeightType)0)
	  {
	    _negativePart.add(-coeffDiff,*v);
	  };
    };

  endOfVars = weight2.monomials().end();
  for (const ulong* v = weight2.monomials().begin();
       v != endOfVars;
       ++v)
    {
      if (!weight1.containsVariable(*v))
	{
	  _negativePart.add(weight2.coefficient(*v),*v);
	};
    };

  if (weight1.freeMember() > weight2.freeMember())
    {
      _positivePart.add(weight1.freeMember() - weight2.freeMember());
    }
  else
    {
      _negativePart.add(weight2.freeMember() - weight1.freeMember());
    };

  // summarise
  if (_positivePart.isConstant())
    { 
      if (_negativePart.isConstant())
	{ 
	  if (_positivePart.freeMember() == _negativePart.freeMember())
	    {
	      ASSERT(!_positivePart.freeMember());
	      _category = AlwaysEqual;
	    }
	  else
	    if (_positivePart.freeMember())
	      {
		ASSERT(!_negativePart.freeMember());
		_category = AlwaysGreater;
	      }
	    else
	      {
		ASSERT(_negativePart.freeMember());
		_category = AlwaysLess;
	      };
	}
      else
	{
	  TermWeightType negMinVal = _negativePart.minimalValue();
	  if (_positivePart.freeMember() < negMinVal)
	    {
	      _category = AlwaysLess;
	    }
	  else
	    if (_positivePart.freeMember() == negMinVal)
	      {
		_category = CanBeLessOrEqual;
	      }
	    else
	      _category = VolatileButNearlyAlwaysLess;
	};
    }
  else
    if (_negativePart.isConstant())
      {    
	TermWeightType posMinVal = _positivePart.minimalValue();
	if (posMinVal > _negativePart.freeMember())
	  {
	    _category = AlwaysGreater;
	  }
	else
	  if (posMinVal == _negativePart.freeMember())
	    {
	      _category = CanBeGreaterOrEqual;
	    }
	  else
	    _category = VolatileButNearlyAlwaysGreater;
      }
    else
      _category = AbsolutelyVolatile;
}; // void WeightPolynomial::SpecialisedComparison::compare(..)


}; // namespace VK

//===================================================================
#endif
