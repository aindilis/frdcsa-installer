//
// File:         InferenceDispatcher.h
// Description:  Interface to several inference engines.
// Created:      Mar 5, 2000, 20:40
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//====================================================
#ifndef INFERENCE_DISPATCHER_H
//====================================================
#define INFERENCE_DISPATCHER_H
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "or_index.h"
#include "binary_resolution.h"
#include "forward_superposition.h"
#include "eq_factoring.h"
#include "backward_superposition.h"
#include "DestructionMode.h"
namespace VK
{
  using namespace BK;
  using namespace Gematogen;
class Clause;
class NewClause;
}; // namespace VK
//====================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_INFERENCE_DISPATCHER
 #define DEBUG_NAMESPACE "InferenceDispatcher"
#endif
#include "debugMacros.h"
//====================================================
namespace VK
{
  using namespace BK;
  using namespace Gematogen;
class InferenceDispatcher
{
 public:
  InferenceDispatcher(NewClause* freshClause);
  ~InferenceDispatcher() 
    {
    };
  void destroy();

#ifndef NO_DEBUG  
  bool clean() const;
#endif

  void setParamodulationFlag(bool fl) 
    {
      _paramodulationFlag = fl;
      _ordResIndex.SetParamodulation(fl); 
    }; // void setParamodulationFlag(bool fl) 

  void setSimBackSup(long degree)
    {
      _backwardSuperposition.setSimultaneous(degree);
    };
  void setNoInfBetweenRulesFlag(bool fl)
    {
      _noInfBetweenRulesFlag = fl;
      _binRes.setNoInfBetweenRulesFlag(fl);
      _forwardSuperposition.setNoInfBetweenRulesFlag(fl);
      _backwardSuperposition.setNoInfBetweenRulesFlag(fl);
    };
  void newQuery(const Clause* cl) 
    { 
      _query = cl; 
      _continuation = FirstBinRes; 
    };
  bool nextInference() { return nextInf(); };
  void integrateDefinition(Clause* cl,const TERM& definedHeader);
  void removeDefinition(Clause* cl);
  void makeActive(Clause* cl);
  void remove(Clause* cl);
 public:
  bool binResFlag;
 private:
  enum Continuation 
  { 
    FirstBinRes, 
    NextBinRes, 
    FirstForwSup, 
    NextForwSup,
    FirstEqFactor,
    NextEqFactor,
    FirstBackSup,
    NextBackSup 
  };
 private:
  bool nextInf();
 private:
  bool _paramodulationFlag;
  bool _noInfBetweenRulesFlag;
  OR_INDEX _ordResIndex;
  BIN_ORD_RES _binRes;
  FORWARD_SUPERPOSITION _forwardSuperposition;
  EQ_FACTORING _equalityFactoring;
  BACKWARD_SUPERPOSITION _backwardSuperposition;
  const Clause* _query;
  NewClause* _freshClause;
  Continuation _continuation;
}; // class InferenceDispatcher


}; // namespace VK
//====================================================
#endif
