//
// File:         SimplificationOrdering.h
// Description:  Simplification ordering abstraction.  
// Created:      Apr 16, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//==================================================================
#ifndef SIMPLIFICATION_ORDERING_H
#define SIMPLIFICATION_ORDERING_H
//==================================================================
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "Comparison.h"
#include "RuntimeError.h"
#include "WeightPolynomial.h"
#include "Term.h"
#include "InstanceRetrievalForDemodulation.h"
//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_SIMPLIFICATION_ORDERING
 #define DEBUG_NAMESPACE "SimplificationOrdering"
#endif
#include "debugMacros.h"
//===================================================================

namespace VK
{
class PREFIX_SYM;
class Flatterm;
class ForwardMatchingSubstitutionCache;

class SimplificationOrdering
{
 public:  
  enum Type 
  {
    NonrecursiveKB,
    StandardKB
  };
  class SpecialisedComparison;
  class ConstraintCheckOnUnifier;
  class ConstraintCheckOnForwardMatchingSubst;
  class StoredConstraint;
  class CompiledConstraintForUnifiers;
  class CompiledConstraintForInstanceRetrieval;
 public:
  SimplificationOrdering() : _type(NonrecursiveKB) {};
  virtual ~SimplificationOrdering();

  Type type() const { return _type; };

  static void setCurrent(SimplificationOrdering* so) { _current = so; };
  static SimplificationOrdering* current() 
    {
      CALL("current()");
      return _current; 
    };
  

  // Basic comparison operations

  virtual Comparison compare(const Flatterm* term1,
			     const WeightPolynomial& weight1,
			     const Flatterm* term2,
			     const WeightPolynomial& weight2) = 0;


  virtual Comparison compareAtoms(const Flatterm* lit1,
				  const WeightPolynomial& weight1,
				  const Flatterm* lit2,
				  const WeightPolynomial& weight2) = 0;


  virtual Comparison compareOnCurrentUnifier(PREFIX_SYM* complexTerm1,
					     PREFIX_SYM* complexTerm2) = 0;

  virtual Comparison compareOnCurrentUnifier(const TERM& term1,
					     const TERM& term2,
					     ulong variableIndex) = 0; 

  virtual Comparison compare(const TERM* complexTerm1,
			     const TERM* complexTerm2,
			     const InstanceRetrievalForDemodulation::Substitution* subst) = 0;

  virtual Comparison compare(const TERM& term1,
			     const TERM& term2,
			     const ForwardMatchingSubstitutionCache* subst) = 0; 

  bool greaterOrEqual(const Flatterm* term1,
		      const WeightPolynomial& weight1,
		      const Flatterm* term2,
		      const WeightPolynomial& weight2);  

  bool greater(const Flatterm* term1,
	       const WeightPolynomial& weight1,
	       const Flatterm* term2,
	       const WeightPolynomial& weight2);  


 
  bool lessOrEqual(const Flatterm* term1,
		   const WeightPolynomial& weight1,
		   const Flatterm* term2,
		   const WeightPolynomial& weight2);

  
  bool less(const Flatterm* term1,
	    const WeightPolynomial& weight1,
	    const Flatterm* term2,
	    const WeightPolynomial& weight2);




  bool atomGreaterOrEqual(const Flatterm* lit1,
			  const WeightPolynomial& weight1,
			  const Flatterm* lit2,
			  const WeightPolynomial& weight2);
  
  bool atomGreater(const Flatterm* lit1,
		   const WeightPolynomial& weight1,
		   const Flatterm* lit2,
		   const WeightPolynomial& weight2);

  bool atomLessOrEqual(const Flatterm* lit1,
		       const WeightPolynomial& weight1,
		       const Flatterm* lit2,
		       const WeightPolynomial& weight2);

  bool atomLess(const Flatterm* lit1,
		const WeightPolynomial& weight1,
		const Flatterm* lit2,
		const WeightPolynomial& weight2);


  
  bool greaterOrEqualWithCurrentUnifier(PREFIX_SYM* complexTerm1,
					PREFIX_SYM* complexTerm2);

  
  bool greater(const TERM* complexTerm1,
	       const TERM* complexTerm2,
	       const InstanceRetrievalForDemodulation::Substitution* subst);

  // Specialised comparisons
  
  virtual SpecialisedComparison* reserveSpecialisedComparison() = 0;
  virtual void releaseSpecialisedComparison(SpecialisedComparison* sc) = 0;

  // Stored constraints
  virtual 
    SimplificationOrdering::StoredConstraint*
    storedConstraintGreater(SpecialisedComparison* sc) = 0;
  virtual 
    SimplificationOrdering::StoredConstraint*
    storedConstraintGreaterOrEqual(SpecialisedComparison* sc) = 0;
  virtual 
    SimplificationOrdering::StoredConstraint*
    storedConstraintLess(SpecialisedComparison* sc) = 0;
  virtual 
    SimplificationOrdering::StoredConstraint*
    storedConstraintLessOrEqual(SpecialisedComparison* sc) = 0;
  virtual 
    SimplificationOrdering::StoredConstraint* copy(SimplificationOrdering::StoredConstraint* constr) = 0;
  virtual void releaseConstraint(StoredConstraint* constr) = 0;


  // Compiled constraints
  
  virtual 
    SimplificationOrdering::CompiledConstraintForUnifiers* reserveCompiledConstraintForUnifiers() = 0;
  virtual 
    void releaseCompiledConstraintForUnifiers(SimplificationOrdering::CompiledConstraintForUnifiers* cc) = 0;


  virtual 
    SimplificationOrdering::CompiledConstraintForInstanceRetrieval* reserveCompiledConstraintForInstanceRetrieval() = 0;
  virtual 
    void releaseCompiledConstraintForInstanceRetrieval(SimplificationOrdering::CompiledConstraintForInstanceRetrieval* cc) = 0;


  // Checking stored constraints on substitutions

  virtual ConstraintCheckOnUnifier* reserveConstraintCheckOnUnifier() = 0;
  virtual void releaseConstraintCheckOnUnifier(ConstraintCheckOnUnifier* ccu) = 0;

  
  virtual 
    ConstraintCheckOnForwardMatchingSubst* 
    reserveConstraintCheckOnForwardMatchingSubst() = 0;

  virtual 
    void 
    releaseConstraintCheckOnForwardMatchingSubst(ConstraintCheckOnForwardMatchingSubst* ccfms) = 0;


  
  // Miscelaneous
  
  virtual Comparison compareWithTransposition(const Flatterm* term) = 0;



  
 protected:
  void setType(const Type& t) { _type = t; };
  
 private:
  static SimplificationOrdering* _current;
  Type _type;
  friend class InitStatic;
}; // class SimplificationOrdering


}; // namespace VK
//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_SIMPLIFICATION_ORDERING
#define DEBUG_NAMESPACE "SimplificationOrdering::SpecialisedComparison"
#endif
#include "debugMacros.h"
//===================================================================


namespace VK
{
class SimplificationOrdering::SpecialisedComparison
{
 public:
  enum Category
  {
    AlwaysLess,
    AlwaysEqual,
    AlwaysGreater,
    CanBeLessOrEqual,
    CanBeGreaterOrEqual,
    VolatileButNearlyAlwaysLess,
    VolatileButNearlyAlwaysGreater,
    AbsolutelyVolatile 
  };
 public:
  virtual void load(const TERM& lhs,const TERM& rhs) = 0;
  virtual bool lhsVarInstMayOrder() = 0;
  virtual bool rhsVarInstMayOrder() = 0;
  virtual void specialiseGreater() = 0;
  virtual void specialiseGreaterOrEqual() = 0;
  virtual void specialiseLess() = 0;
  virtual void specialiseLessOrEqual() = 0;
  const Category& category() const { return _category; }; 

#ifndef NO_DEBUG_VIS
  virtual ostream& output(ostream& str) const = 0;
#endif
  
 protected:
  SpecialisedComparison() {};
  virtual ~SpecialisedComparison();
 protected:
  Category _category;
}; // class SimplificationOrdering::SpecialisedComparison

}; // namespace VK

#ifndef NO_DEBUG_VIS
inline
ostream& operator<<(ostream& str,
		    const VK::SimplificationOrdering::SpecialisedComparison::Category& cat)
{
  switch (cat)
    {
    case VK::SimplificationOrdering::SpecialisedComparison::AlwaysLess:
      return str << "AlwaysLess";
    case VK::SimplificationOrdering::SpecialisedComparison::AlwaysEqual:
      return str << "AlwaysEqual";
    case VK::SimplificationOrdering::SpecialisedComparison::AlwaysGreater:
      return str << "AlwaysGreater";
    case VK::SimplificationOrdering::SpecialisedComparison::CanBeLessOrEqual:
      return str << "CanBeLessOrEqual";
    case VK::SimplificationOrdering::SpecialisedComparison::CanBeGreaterOrEqual:
      return str << "CanBeGreaterOrEqual";
    case VK::SimplificationOrdering::SpecialisedComparison::VolatileButNearlyAlwaysLess:
      return str << "VolatileButNearlyAlwaysLess";
    case VK::SimplificationOrdering::SpecialisedComparison::VolatileButNearlyAlwaysGreater:
      return str << "VolatileButNearlyAlwaysGreater";
    case VK::SimplificationOrdering::SpecialisedComparison::AbsolutelyVolatile:
      return str << "AbsolutelyVolatile";
    };
  return str << "????????";
};
#endif


//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_SIMPLIFICATION_ORDERING
#define DEBUG_NAMESPACE "SimplificationOrdering::ConstraintCheckOnUnifier"
#endif
#include "debugMacros.h"
//===================================================================

namespace VK
{

class SimplificationOrdering::ConstraintCheckOnUnifier
{
 public:
  virtual bool holds(const SimplificationOrdering::StoredConstraint* constr,
		     ulong variableIndex) = 0;
 protected:
  ConstraintCheckOnUnifier() {};
  virtual ~ConstraintCheckOnUnifier();  
}; // class SimplificationOrdering::ConstraintCheckOnUnifier

}; // namespace VK

//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_SIMPLIFICATION_ORDERING
#define DEBUG_NAMESPACE "SimplificationOrdering::ConstraintCheckOnForwardMatchingSubst"
#endif
#include "debugMacros.h"
//===================================================================
  
namespace VK
{
class SimplificationOrdering::ConstraintCheckOnForwardMatchingSubst
{
 public:
  virtual bool holds(const SimplificationOrdering::StoredConstraint* constr,
		     const ForwardMatchingSubstitutionCache* subst) = 0;
 protected:
  ConstraintCheckOnForwardMatchingSubst() {};
  virtual ~ConstraintCheckOnForwardMatchingSubst();
}; // class SimplificationOrdering::ConstraintCheckOnForwardMatchingSubst

}; // namespace VK
//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_SIMPLIFICATION_ORDERING
 #define DEBUG_NAMESPACE "SimplificationOrdering::StoredConstraint"
#endif
#include "debugMacros.h"
//===================================================================

namespace VK
{
class SimplificationOrdering::StoredConstraint
{
 private:
  StoredConstraint() {};
  ~StoredConstraint() {};
  friend class SimplificationOrdering; // to suppress warnings
}; // class SimplificationOrdering::StoredConstraint

}; // namespace VK
//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_SIMPLIFICATION_ORDERING
 #define DEBUG_NAMESPACE "SimplificationOrdering::CompiledConstraintForUnifiers"
#endif
#include "debugMacros.h"
//===================================================================

namespace VK
{
class SimplificationOrdering::CompiledConstraintForUnifiers
{
 public:
  virtual void load(PREFIX_SYM* lhs,PREFIX_SYM* rhs) = 0;
  virtual bool lhsVarInstMayOrder() = 0;
  virtual bool rhsVarInstMayOrder() = 0;
  virtual void compileGreaterOrEqual() = 0;
  virtual bool holdsOnCurrentUnifier() = 0;
 protected:
  CompiledConstraintForUnifiers() {};
  virtual ~CompiledConstraintForUnifiers();
  friend class SimplificationOrdering; // to suppress warnings
}; // class SimplificationOrdering::CompiledConstraintForUnifiers

}; // namespace VK
//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_SIMPLIFICATION_ORDERING
 #define DEBUG_NAMESPACE "SimplificationOrdering::CompiledConstraintForInstanceRetrieval"
#endif
#include "debugMacros.h"
//===================================================================

namespace VK
{
class SimplificationOrdering::CompiledConstraintForInstanceRetrieval
{
 public:
  enum Category
  {
    Tautology,
    NearlyTautology,
    Volatile,
    NearlyContradiction,
    Contradiction,
    FailureToCompare
  };
 public:
  virtual void loadGreater(const TERM* lhs,const TERM* rhs) = 0;
  virtual bool lhsVarInstMayOrder() = 0;
  virtual bool rhsVarInstMayOrder() = 0;
  const Category& category() const { return _category; };
  virtual 
    void 
    compile(const InstanceRetrievalForDemodulation::Substitution* subst) = 0;
  virtual bool holds() = 0;
 protected:
  CompiledConstraintForInstanceRetrieval() {};
  virtual ~CompiledConstraintForInstanceRetrieval();
  void setCategory(const Category& cat) { _category = cat; };
 private:
  Category _category;
  friend class SimplificationOrdering; // to suppress warnings  
}; // class SimplificationOrdering::CompiledConstraintForInstanceRetrieval

}; // namespace VK

//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_SIMPLIFICATION_ORDERING
 #define DEBUG_NAMESPACE "SimplificationOrdering"
#endif
#include "debugMacros.h"
//===================================================================

namespace VK
{
inline
bool SimplificationOrdering::greaterOrEqual(const Flatterm* term1,
					    const WeightPolynomial& weight1,
					    const Flatterm* term2,
					    const WeightPolynomial& weight2)
{
  Comparison cmp = compare(term1,weight1,term2,weight2);
  return (cmp == Equal) || (cmp == Greater);
};  

 
inline 
bool SimplificationOrdering::greater(const Flatterm* term1,
				     const WeightPolynomial& weight1,
				     const Flatterm* term2,
				     const WeightPolynomial& weight2)
{
  return compare(term1,weight1,term2,weight2) == Greater;
};  


  
inline
bool SimplificationOrdering::lessOrEqual(const Flatterm* term1,
					 const WeightPolynomial& weight1,
					 const Flatterm* term2,
					 const WeightPolynomial& weight2)
{
  Comparison cmp = compare(term1,weight1,term2,weight2);
  return (cmp == Equal) || (cmp == Less);
};

 
inline 
bool SimplificationOrdering::less(const Flatterm* term1,
				  const WeightPolynomial& weight1,
				  const Flatterm* term2,
				  const WeightPolynomial& weight2)
{
  return compare(term1,weight1,term2,weight2) == Less;
};




inline
bool SimplificationOrdering::atomGreaterOrEqual(const Flatterm* lit1,
						const WeightPolynomial& weight1,
						const Flatterm* lit2,
						const WeightPolynomial& weight2)
{
  Comparison cmp = compareAtoms(lit1,weight1,lit2,weight2);
  return (cmp == Equal) || (cmp == Greater);
};


inline  
bool SimplificationOrdering::atomGreater(const Flatterm* lit1,
					 const WeightPolynomial& weight1,
					 const Flatterm* lit2,
					 const WeightPolynomial& weight2)
{
  return compareAtoms(lit1,weight1,lit2,weight2) == Greater;
};

inline
bool SimplificationOrdering::atomLessOrEqual(const Flatterm* lit1,
					     const WeightPolynomial& weight1,
					     const Flatterm* lit2,
					     const WeightPolynomial& weight2)
{
  Comparison cmp = compareAtoms(lit1,weight1,lit2,weight2);
  return (cmp == Equal) || (cmp == Less);
};


inline
bool SimplificationOrdering::atomLess(const Flatterm* lit1,
				      const WeightPolynomial& weight1,
				      const Flatterm* lit2,
				      const WeightPolynomial& weight2)
{
  return compareAtoms(lit1,weight1,lit2,weight2) == Less;
};

inline
bool SimplificationOrdering::greaterOrEqualWithCurrentUnifier(PREFIX_SYM* complexTerm1,
							     PREFIX_SYM* complexTerm2)
{
  Comparison cmp = compareOnCurrentUnifier(complexTerm1,complexTerm2);
  return (cmp == Greater) || (cmp == Equal); 
}; 

inline
bool SimplificationOrdering::greater(const TERM* complexTerm1,
				     const TERM* complexTerm2,
				     const InstanceRetrievalForDemodulation::Substitution* subst)
{
  return compare(complexTerm1,complexTerm2,subst) == Greater;
};
}; // namespace VK

//=================================================================
#endif
