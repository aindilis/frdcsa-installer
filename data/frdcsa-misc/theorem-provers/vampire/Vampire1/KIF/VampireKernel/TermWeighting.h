//
// File:         TermWeighting.h
// Description:  Term weighting abstraction.  
// Created:      Apr 18, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//=================================================================
#ifndef TERM_WEIGHTING_H
#define TERM_WEIGHTING_H
//=================================================================
#include <iostream.h>
#include "WeightPolynomial.h"
#include "Flatterm.h"
#include "Term.h"
#include "InstanceRetrievalForDemodulation.h"
#include "TermWeightType.h"
//=================================================================

namespace VK
{

  class PREFIX_SYM;
  class ForwardMatchingSubstitutionCache;

  class TermWeighting
    {
    public:
      enum Scheme
      {
	NonuniformConstant,
	Uniform
      };
    public:
      TermWeighting() : _scheme(Uniform) {};
      static TermWeighting* current() { return _current; };
      static void setCurrent(TermWeighting* tw) { _current = tw; };
      virtual ~TermWeighting() = 0;
      Scheme scheme() const { return _scheme; };
      virtual void collectWeight(WeightPolynomial& weight,const Flatterm* term) = 0;
      virtual void collectWeight(WeightPolynomial& weight,TermWeightType coefficient,const Flatterm* term) = 0;

      virtual TermWeightType computeMinimalInstanceWeight(const Flatterm* term,
							  bool& containsVariables) = 0;
		
      virtual TermWeightType computeWeightIfGroundAndLessOrEqual(TermWeightType coefficient,
								 const Flatterm* term,
								 TermWeightType weightLimit) = 0;
      // ^ returns 0 if nonground and -1L if too heavy
    
      virtual void collectWeight(WeightPolynomial& weight,const TERM& term) = 0;
      virtual void collectWeight(WeightPolynomial& weight,TermWeightType coefficient,const TERM& term) = 0;



      virtual void collectWeight(WeightPolynomial& weight,TermWeightType coefficient,const PREFIX_SYM* term) = 0;
      virtual void collectWeight(WeightPolynomial& weight,const PREFIX_SYM* term) = 0;
  
      virtual void collectWeight(WeightPolynomial& weight,const TERM& term,ulong variableIndex) = 0;


      virtual 
	void 
	collectWeight(WeightPolynomial& weight,
		      TermWeightType coefficient,
		      const TERM* complexTerm,
		      const InstanceRetrievalForDemodulation::Substitution* subst) = 0;
      virtual 
	void 
	collectWeight(WeightPolynomial& weight,
		      const TERM* complexTerm,
		      const InstanceRetrievalForDemodulation::Substitution* subst) = 0;

      virtual void collectWeight(WeightPolynomial& weight,
				 const TERM& term,
				 const ForwardMatchingSubstitutionCache* subst) = 0;



      virtual TermWeightType computeMinimalInstanceWeight(const PREFIX_SYM* term,
								       bool& containsVariables) = 0;

      virtual TermWeightType computeWeightIfGroundAndLessOrEqual(TermWeightType coefficient,
									      const PREFIX_SYM* term,
									      TermWeightType weightLimit) = 0;
      // ^ returns 0 if nonground and -1L if too heavy

      virtual TermWeightType computeWeightIfGroundAndLessOrEqual(const PREFIX_SYM* term,
									      TermWeightType weightLimit) = 0;
      // ^ returns 0 if nonground and -1L if too heavy

      virtual TermWeightType computeMinimalInstanceWeightIfLessOrEqual(TermWeightType coefficient,
										    const PREFIX_SYM* term,
										    TermWeightType weightLimit,
										    bool& ground) = 0;
      // ^ returns -1 if too heavy

      virtual TermWeightType computeMinimalInstanceWeightIfLessOrEqual(const PREFIX_SYM* term,
										    TermWeightType weightLimit,
										    bool& ground) = 0;
      // ^ returns -1 if too heavy





      virtual TermWeightType computeMinimalInstanceWeight(const TERM* term,
								       bool& containsVariables) = 0;

      virtual TermWeightType computeWeightIfGroundAndLessOrEqual(TermWeightType coefficient,
									      const TERM* term,
									      TermWeightType weightLimit) = 0;
      // ^ returns 0 if nonground and -1L if too heavy

      virtual TermWeightType computeWeightIfGroundAndLessOrEqual(const TERM* term,
									      TermWeightType weightLimit) = 0;
      // ^ returns 0 if nonground and -1L if too heavy

      virtual TermWeightType computeMinimalInstanceWeightIfLessOrEqual(TermWeightType coefficient,
										    const TERM* term,
										    TermWeightType weightLimit,
										    bool& ground) = 0;
      // ^ returns -1 if too heavy

      virtual TermWeightType computeMinimalInstanceWeightIfLessOrEqual(const TERM* term,
										    TermWeightType weightLimit,
										    bool& ground) = 0;
      // ^ returns -1 if too heavy

  


    protected:
      void setScheme(const Scheme& s) { _scheme = s; };
    private:
      Scheme _scheme;
      static TermWeighting* _current;
    }; // class TermWeighting

}; // namespace VK

inline
ostream& operator<<(ostream& str,const VK::TermWeighting::Scheme& s)
{
  switch (s)
    {
    case VK::TermWeighting::NonuniformConstant:
      str << "nonuniform_constant";
      break;
    case VK::TermWeighting::Uniform:
      str << "uniform";
      break;
    default: str << "unknown_term_weighting_scheme";
    };
  return str;
}; // ostream& operator<<(ostream& str,const TermWeighting::Scheme s&)


//=================================================================
#endif



