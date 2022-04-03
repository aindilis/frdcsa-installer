//
// File:         LinearPolynomial.h
// Description:  Small linear polynomials
//               with integer coefficients.
// Created:      Apr 11, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//===================================================================
#ifndef LINEAR_POLYNOMIAL_H
#define LINEAR_POLYNOMIAL_H
//===================================================================
#ifndef NO_DEBUG_VIS
#include <iostream.h>
#endif
#include "jargon.h"
#include "BlodKorvDebugFlags.h"
#include "LinearCombination.h"
//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_LINEAR_POLYNOMIAL
#define DEBUG_NAMESPACE "LinearPolynomial<CoeffType,MaxNumOfVariables>"
#endif
#include "debugMacros.h"
//===================================================================

namespace BK 
{

template <class CoeffType,ulong MaxNumOfVariables>
class LinearPolynomial
{
 public:
  LinearPolynomial() :
    _monomials(),
    _freeMember((CoeffType)0)
    {
    };
  ~LinearPolynomial() {};
  void reset() { _monomials.reset(); _freeMember = (CoeffType)0; };
  LinearCombination<CoeffType,MaxNumOfVariables>& monomials() 
    { 
      return _monomials; 
    };
  const LinearCombination<CoeffType,MaxNumOfVariables>& monomials() const 
    { 
      return _monomials; 
    };
  CoeffType& freeMember() { return _freeMember; };
  const CoeffType& freeMember() const { return _freeMember; };
  
  bool containsVariableWhichIsNotIn(const LinearPolynomial& lp) const
    {
      return monomials().containsVariableWhichIsNotIn(lp.monomials());
    };
  
 private:
  LinearCombination<CoeffType,MaxNumOfVariables> _monomials;
  CoeffType _freeMember;
}; // class LinearPolynomial<class CoeffType,ulong MaxNumOfVariables>

}; // namespace BK



#ifndef NO_DEBUG_VIS

template <class CoeffType,ulong MaxNumOfVariables>
inline
ostream& operator<<(ostream& str,
		    const BK::LinearPolynomial<CoeffType,MaxNumOfVariables>& lp)
{
  str << lp.freeMember();
  if (!lp.monomials().isZero())
    str << " + " << lp.monomials();
  return str;
}; 
#endif




//===================================================================
#endif
