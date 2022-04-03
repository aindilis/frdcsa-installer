//
// File:         InferenceDispatcher.cpp
// Description:  Interface to several inference engines.
// Created:      Mar 5, 2000, 20:40
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//====================================================
#include "InferenceDispatcher.h"
#include "Clause.h"
//====================================================
 
namespace VK
{
  using namespace BK;
  using namespace Gematogen;
InferenceDispatcher::InferenceDispatcher(NewClause* freshClause)
  : binResFlag(false),
    _paramodulationFlag(false),
    _noInfBetweenRulesFlag(false),  
    _ordResIndex(),
    _binRes(freshClause,&_ordResIndex), 
    _forwardSuperposition(freshClause,&_ordResIndex),
    _equalityFactoring(freshClause), 
    _backwardSuperposition(freshClause,&_ordResIndex),
    _query((Clause*)0),
    _freshClause(freshClause)
{
  CALL("constructor InferenceDispatcher(NewClause* freshClause)");
}; // InferenceDispatcher::InferenceDispatcher(NewClause* freshClause)


void InferenceDispatcher::destroy() 
{
  _ordResIndex.Destroy();
}; // void InferenceDispatcher::destroy() 

#ifndef NO_DEBUG  
bool InferenceDispatcher::clean() const
{
  return true
    && _ordResIndex.clean()         
    ;  
}; // bool InferenceDispatcher::clean() const
#endif



bool InferenceDispatcher::nextInf()
{
  CALL("nextInf()");

  switch (_continuation)
    { 
    case FirstBinRes:
      COP("FirstBinRes");
      if (!binResFlag) goto first_forw_sup;   
      ASSERT(Unifier::current()->NoBoundVariables());
      _continuation = NextBinRes;  
      _binRes.Load(_query);
      // no break here, proceed with the next case 

    case NextBinRes:
      COP("NextBinRes");
      if (_binRes.NextInference()) return true;
      Unifier::current()->UnbindAll(); 
      // no break here, try forward superposition now 
       
    case FirstForwSup:
    first_forw_sup:
    COP("FirstForwSup");
    ASSERT(Unifier::current()->NoBoundVariables());
    if (!_paramodulationFlag) return false; // the rules below deal with equality
    if ((!_forwardSuperposition.Load((Clause*)_query))
	|| (!_forwardSuperposition.NextInference()))
      goto first_eq_factor;
    _continuation = NextForwSup;
    return true; 

    case NextForwSup:
      COP("NextForwSup");
      ASSERT(_paramodulationFlag);
      if (_forwardSuperposition.NextInference()) return true;
      Unifier::current()->UnbindAll(); 
      // no break here, try equality factoring here

    case FirstEqFactor:
    first_eq_factor:
    COP("first_eq_factor");
    ASSERT(_paramodulationFlag);
    ASSERT(Unifier::current()->NoBoundVariables());
    if (!_equalityFactoring.Load((Clause*)_query)) goto first_back_sup; 
    _continuation = NextEqFactor;            
    // no break here, proceed with the next case

    case NextEqFactor: 
      COP("NextEqFactor");
      ASSERT(_paramodulationFlag);
      if (_equalityFactoring.NextInference()) return true;
      Unifier::current()->UnbindAll();   
      // no break here, try backward superposition

    case FirstBackSup:
    first_back_sup:
    COP("FirstBackSup");
    ASSERT(_paramodulationFlag);
    ASSERT(Unifier::current()->NoBoundVariables());
    if (!_backwardSuperposition.SetQuery((Clause*)_query)) 
      {       
	return false; // temporary
      };     
    _continuation = NextBackSup;      
    // no break here, proceed with the next case 

    case NextBackSup:
      COP("NextBackSup");
      ASSERT(_paramodulationFlag);
      if (_backwardSuperposition.NextInference()) return true;
      Unifier::current()->UnbindAll();
      return false; // temporary
    };
  ICP("ICP1"); 
  return false;   
}; // bool InferenceDispatcher::nextInf()

void InferenceDispatcher::makeActive(Clause* cl)
{
  CALL("makeActive(Clause* cl)");
  ASSERT(!cl->flag(Clause::FlagInInferenceDispatcherAsActive));
  ASSERT(!cl->flag(Clause::FlagInInferenceDispatcherAsDefinition));
  _ordResIndex.Integrate(cl);
  cl->setFlag(Clause::FlagInInferenceDispatcherAsActive);
}; // void InferenceDispatcher::makeActive(Clause* cl)

void InferenceDispatcher::remove(Clause* cl) 
{
  CALL("remove(Clause* cl)");
  ASSERT(cl->flag(Clause::FlagInInferenceDispatcherAsActive));
  ASSERT(!cl->flag(Clause::FlagInInferenceDispatcherAsDefinition));
  _ordResIndex.Remove(cl);
  cl->clearFlag(Clause::FlagInInferenceDispatcherAsActive);
}; // void InferenceDispatcher::remove(Clause* cl) 


void InferenceDispatcher::integrateDefinition(Clause* cl,const TERM& definedHeader)
{
  CALL("integrateDefinition(Clause* cl,const TERM& definedHeader)");
  ASSERT(!cl->flag(Clause::FlagInInferenceDispatcherAsActive));
  ASSERT(!cl->flag(Clause::FlagInInferenceDispatcherAsDefinition));
  _ordResIndex.Integrate(cl); // temporary
  cl->setFlag(Clause::FlagInInferenceDispatcherAsDefinition);
}; // void InferenceDispatcher::integrateDefinition(Clause* cl,const TERM& definedHeader)

void InferenceDispatcher::removeDefinition(Clause* cl)
{
  CALL("removeDefinition(Clause* cl)");
  ASSERT(!cl->flag(Clause::FlagInInferenceDispatcherAsActive));
  ASSERT(cl->flag(Clause::FlagInInferenceDispatcherAsDefinition));
  _ordResIndex.Remove(cl); // temporary
  cl->clearFlag(Clause::FlagInInferenceDispatcherAsDefinition);
}; // void InferenceDispatcher::removeDefinition(Clause* cl)



}; // namespace VK


//====================================================




