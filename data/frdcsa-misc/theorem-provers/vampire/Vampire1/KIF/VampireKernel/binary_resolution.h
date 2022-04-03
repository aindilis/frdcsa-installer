#ifndef BINARY_RESOLUTION_H
//======================================================
#define BINARY_RESOLUTION_H
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "premise_factoring.h"
#include "or_machine.h"
#include "weight_formula.h"
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_BINARY_RESOLUTION
 #define DEBUG_NAMESPACE "BIN_ORD_RES"
#endif
#include "debugMacros.h"
//=================================================

namespace VK
{
class Clause;
class NewClause;

class BIN_ORD_RES
 {
  public:
   BIN_ORD_RES(NewClause* resolvent,OR_INDEX* resIndex) 
    : premise1(resolvent), 
      premise2(resolvent),
      index1(0),
      index2(1),
      ORMachine(resIndex),
      weight_form(),
      _resolvent(resolvent),
      _noInfBetweenRulesFlag(false)
    {
     weight_form.SetIndex(index1);
    };
   ~BIN_ORD_RES() { CALL("destructor ~BIN_ORD_RES()"); };
   void setNoInfBetweenRulesFlag(bool fl)
   {
    _noInfBetweenRulesFlag = fl;
   };
   void Load(const Clause* c);
   bool NextInference();  

   #ifndef NO_DEBUG 
    bool clean() const { return true; };
   #endif

  private:
   enum Continuation { ContFirstLit, ContFirstFactor, ContNextFactor }; 

  private: 
   void ResetFactoringState();
   bool NextFactor();
   bool MakeNewResolvent();
   void CollectWeightForm(const Clause* c);
   long MinResWeight() { return weight_form.Compute(); };
   const Clause* FirstPremise() const { return premise1.TheClause(); };
   const Clause* SecondPremise() const { return premise2.TheClause(); };
   inline bool CollectResult();

  private:
   OR_PREMISE_WITH_FACTORING premise1,premise2;
   const unsigned long index1,index2;
   unsigned long currentLit;
   OR_MACH ORMachine;
   WEIGHT_FORMULA weight_form;
   NewClause* _resolvent;
   Continuation _continuation;
   bool _noInfBetweenRulesFlag;
   bool _sideClauseCanNotBeRule;

 }; // class BIN_ORD_RES 
  
 
}; // namespace VK


//================================================
#undef DEBUG_NAMESPACE 
//======================================================
#endif
