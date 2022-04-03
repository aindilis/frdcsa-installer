//
// File:         premise_factoring.h
// Description:  OR premises with factoring.
// Created:      Feb 15, 2000
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
// Revised:      Dec 10, 2001. 
//               The arrays OR_PREMISE::{max_literal,mark} replaced by 
//               extending arrays OR_PREMISE::{_maxLiteral,_litIsMarked}
// Revised:      Dec 11, 2001. 
//               The stack OR_PREMISE_WITH_FACTORING::factored replaced by 
//               extending stack OR_PREMISE_WITH_FACTORING::_factoredLits
//====================================================
#ifndef PREMISE_FACTORING_H
//======================================================================
#define PREMISE_FACTORING_H
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "or_premise.h"
#include "ExpandingStack.h"
#include "GlobAlloc.h"
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_PREMISE_FACTORING
 #define DEBUG_NAMESPACE "OR_PREMISE_WITH_FACTORING"
#endif
#include "debugMacros.h"
//=================================================
namespace VK
{
class NewClause;

class OR_PREMISE_WITH_FACTORING : public OR_PREMISE
 {
  private:
   ExpandingStack<GlobAlloc,ulong,32UL, OR_PREMISE_WITH_FACTORING> _factoredLits;
   
   PREFIX_SYM* cut_literal; // the literal itself in prefix representation
   
   ulong first_factor;
   ulong last_factor;
   bool was_factored;
  public:
   OR_PREMISE_WITH_FACTORING();
   void init();
   OR_PREMISE_WITH_FACTORING(NewClause* res);
   void init(NewClause* res);
   ~OR_PREMISE_WITH_FACTORING() { CALL("destructor ~OR_PREMISE_WITH_FACTORING()"); };
   void Destroy() 
   {
    CALL("Destroy()");
   };
   #ifndef NO_DEBUG
    bool clean() const 
    {
     return true;
    };
   #endif   

   void assignResolvent(NewClause* res) { OR_PREMISE::setResolvent(res); };
   
   void Load(ulong ind,const Clause* c)
   {
    CALL("Load(ulong ind,const Clause* c)");
    OR_PREMISE::Load(ind,c);

    DOP(_factoredLits.unfreeze());
    _factoredLits.expand(c->numOfAllLiterals());    
    DOP(_factoredLits.freeze());
   };
   void LoadOptimized(ulong ind,const Clause* c)
   {
    CALL("LoadOptimized(ulong ind,const Clause* c)");
    OR_PREMISE::LoadOptimized(ind,c);

    DOP(_factoredLits.unfreeze());
    _factoredLits.expand(c->numOfAllLiterals());    
    DOP(_factoredLits.freeze());
   };
   void Load(ulong ind,RES_PAIR_LIST* res_pair)
   {
    CALL("Load(ulong ind,RES_PAIR_LIST* res_pair)");
    OR_PREMISE::Load(ind,res_pair);

    DOP(_factoredLits.unfreeze());
    _factoredLits.expand(res_pair->TheClause()->numOfAllLiterals());    
    DOP(_factoredLits.freeze());
   };
   void LoadOptimized(ulong ind,RES_PAIR_LIST* res_pair)
   {
    CALL("LoadOptimized(ulong ind,RES_PAIR_LIST* res_pair)");
    OR_PREMISE::LoadOptimized(ind,res_pair);

    DOP(_factoredLits.unfreeze());
    _factoredLits.expand(res_pair->TheClause()->numOfAllLiterals());    
    DOP(_factoredLits.freeze());
   };

   void ResetFactoring()
    {
     CALL("ResetFactoring()");
     was_factored = false;
     if (FactoringPossible())
      {
       _factoredLits.reset();
       cut_literal = OR_PREMISE::_maxLiteral[cut_lit]; // max_literal[cut_lit];
      };
    };
   bool WasFactored() { return was_factored; };
   bool FactoringPossible() { return (bool)LastMax(); };
   bool GetFirstFactor(ulong first_candidate);
   bool GetNextFactor();
  private: // aux. methods
   bool FindFirstFactor(ulong& candidate);
 }; // class OR_PREMISE_WITH_FACTORING

}; // namespace VK

//======================================================================
#endif
