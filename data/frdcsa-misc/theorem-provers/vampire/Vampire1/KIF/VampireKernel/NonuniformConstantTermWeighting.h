//
// File:         NonuniformConstantTermWeighting.h
// Description:  A term weighting scheme which assumes
//               that symbols may have different weights.
//               Not argument sensitive.    
// Created:      May 09, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//===================================================================
#ifndef NONUNIFORM_CONSTANT_TERM_WEIGHTING_H
#define NONUNIFORM_CONSTANT_TERM_WEIGHTING_H
//===================================================================
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "TermWeighting.h"
#include "TermWeightType.h"
//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_NONUNIFORM_CONSTANT_TERM_WEIGHTING
#define DEBUG_NAMESPACE "NonuniformConstantTermWeighting"
#endif
#include "debugMacros.h"
//===================================================================

namespace VK
{
  using namespace BK;
  using namespace Gematogen;
  class NonuniformConstantTermWeighting : public TermWeighting
    {
    public: 
      NonuniformConstantTermWeighting() 
	{
	  setScheme(NonuniformConstant);
	};    
      ~NonuniformConstantTermWeighting();
      void* operator new(size_t);
      void operator delete(void* obj);

      void collectWeight(WeightPolynomial& weight,const Flatterm* term);
      void collectWeight(WeightPolynomial& weight,TermWeightType coefficient,const Flatterm* term);
      TermWeightType computeMinimalInstanceWeight(const Flatterm* term,
							       bool& containsVariables);
      TermWeightType computeWeightIfGroundAndLessOrEqual(TermWeightType coefficient,
								      const Flatterm* term,
								      TermWeightType weightLimit);
      // ^ returns 0 if nonground and -1L if too heavy

      void collectWeight(WeightPolynomial& weight,const TERM& term);
      void collectWeight(WeightPolynomial& weight,TermWeightType coefficient,const TERM& term);
      void collectWeight(WeightPolynomial& weight,TermWeightType coefficient,const PREFIX_SYM* term);
      void collectWeight(WeightPolynomial& weight,const PREFIX_SYM* term);
      void collectWeight(WeightPolynomial& weight,const TERM& term,ulong variableIndex);

      void 
	collectWeight(WeightPolynomial& weight,
		      TermWeightType coefficient,
		      const TERM* complexTerm,
		      const InstanceRetrievalForDemodulation::Substitution* subst);
      void 
	collectWeight(WeightPolynomial& weight,
		      const TERM* complexTerm,
		      const InstanceRetrievalForDemodulation::Substitution* subst);

      void collectWeight(WeightPolynomial& weight,
			 const TERM& term,
			 const ForwardMatchingSubstitutionCache* subst);
      TermWeightType computeMinimalInstanceWeight(const PREFIX_SYM* term,bool& containsVariables);
      TermWeightType computeWeightIfGroundAndLessOrEqual(TermWeightType coefficient,const PREFIX_SYM* term,TermWeightType weightLimit);
      // returns 0 if nonground and -1L if too heavy
      TermWeightType computeWeightIfGroundAndLessOrEqual(const PREFIX_SYM* term,TermWeightType weightLimit);
      // returns 0 if nonground and -1L if too heavy
      TermWeightType computeMinimalInstanceWeightIfLessOrEqual(TermWeightType coefficient,
									    const PREFIX_SYM* term,
									    TermWeightType weightLimit,
									    bool& ground);
      // ^ returns -1 if too heavy

      TermWeightType computeMinimalInstanceWeightIfLessOrEqual(const PREFIX_SYM* term,
									    TermWeightType weightLimit,
									    bool& ground);
      // ^ returns -1 if too heavy
  



      TermWeightType computeMinimalInstanceWeight(const TERM* term,bool& containsVariables);
      TermWeightType computeWeightIfGroundAndLessOrEqual(TermWeightType coefficient,
								      const TERM* term,
								      TermWeightType weightLimit);
      // returns 0 if nonground and -1L if too heavy

      TermWeightType computeWeightIfGroundAndLessOrEqual(const TERM* term,TermWeightType weightLimit);
      // returns 0 if nonground and -1L if too heavy

      TermWeightType computeMinimalInstanceWeightIfLessOrEqual(TermWeightType coefficient,
									    const TERM* term,
									    TermWeightType weightLimit,
									    bool& ground);
      // ^ returns -1 if too heavy

      TermWeightType computeMinimalInstanceWeightIfLessOrEqual(const TERM* term,
									    TermWeightType weightLimit,
									    bool& ground);
      // ^ returns -1 if too heavy


    private:
  
      InstanceRetrievalForDemodulation::Substitution::Instance _instance;
      TERM::Iterator _iter;


    }; // class NonuniformConstantTermWeighting : public TermWeighting


}; // namespace VK

//===================================================================
#endif

