//
// File:         SimplificationOrdering.cpp
// Description:  Simplification ordering abstraction.  
// Created:      Apr 19, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//=================================================================
#include "SimplificationOrdering.h"
#include "NonrecursiveKBOrdering.h"
//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_SIMPLIFICATION_ORDERING
 #define DEBUG_NAMESPACE "SimplificationOrdering"
#endif
#include "debugMacros.h"
//===================================================================

namespace VK
{

SimplificationOrdering* SimplificationOrdering::_current;

SimplificationOrdering::~SimplificationOrdering()
{
  CALL("destructor ~SimplificationOrdering()");
};

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

SimplificationOrdering::SpecialisedComparison::~SpecialisedComparison()
{
  CALL("destructor ~SpecialisedComparison()");
};
}; // namespace VK


//===================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_SIMPLIFICATION_ORDERING
#define DEBUG_NAMESPACE "SimplificationOrdering::ConstraintCheckOnUnifier"
#endif
#include "debugMacros.h"
//===================================================================

namespace VK
{
SimplificationOrdering::ConstraintCheckOnUnifier::~ConstraintCheckOnUnifier()
{
  CALL("destructor ~ConstraintCheckOnUnifier()");
};

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
SimplificationOrdering::ConstraintCheckOnForwardMatchingSubst::~ConstraintCheckOnForwardMatchingSubst()
{
  CALL("destructor ~ConstraintCheckOnForwardMatchingSubst()");
};
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
SimplificationOrdering::CompiledConstraintForUnifiers::~CompiledConstraintForUnifiers()
{  
  CALL("destructor ~CompiledConstraintForUnifiers()");
};

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

SimplificationOrdering::CompiledConstraintForInstanceRetrieval::~CompiledConstraintForInstanceRetrieval()
{
  CALL("destructor ~CompiledConstraintForInstanceRetrieval()");
};
}; // namespace VK
//=================================================================
