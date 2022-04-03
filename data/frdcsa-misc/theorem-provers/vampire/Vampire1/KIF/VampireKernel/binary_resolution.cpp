#include "binary_resolution.h"
#include "Clause.h"
#include "NewClause.h"
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_BINARY_RESOLUTION
 #define DEBUG_NAMESPACE "BIN_ORD_RES"
#endif
#include "debugMacros.h"
//=================================================

namespace VK
{
void BIN_ORD_RES::Load(const Clause* c)
 {
  // reset everithing here
  ORMachine.SetIndex(index2);
  premise1.Load(index1,c);
  CollectWeightForm(c);
  _sideClauseCanNotBeRule = _noInfBetweenRulesFlag && c->IsRule();
  currentLit = 0;
  _continuation = ContFirstLit;
 }; // inline void BIN_ORD_RES::Load(const Clause* c)   

bool BIN_ORD_RES::NextInference()
 {
  CALL("NextInference()");
  switch (_continuation)
   {
    case ContFirstLit: goto next_lit;
    case ContFirstFactor: goto first_factor;
    case ContNextFactor: goto next_factor; 
   };
  
  ICP("ICP1");

  next_lit:
   if (currentLit > premise1.LastMax()) { return false; };
 
   ASSERT(premise1.nthMaxLit(currentLit));
   ORMachine.CompileLiteral(premise1.nthMaxLit(currentLit));
   ORMachine.Inversepolarity();
   premise1.Cut(currentLit);
  
   currentLit++;
  
   Unifier::current()->UnbindAll();
   RES_PAIR_LIST* candidate;
  
   if (ORMachine.FindFirstLeaf())  
    {
     process_leaf: 
      ORMachine.ResetCandidatesFromLeaf();
      try_next_cand:
       candidate  = ORMachine.NextCandidate();
       if (!candidate) goto try_next_leaf;       
       if (_sideClauseCanNotBeRule && candidate->TheClause()->IsRule()) 
        {
         goto try_next_cand;
        };
       if (candidate->TheClause()->MinResWeight() 
           + FirstPremise()->MinResWeight() 
           > _resolvent->MaxWeight())
        {
         _resolvent->AnotherTooHeavyClause();
         goto try_next_cand;
        };   
            
       
       premise2.LoadOptimized(index2,candidate);

       // first resolvent without factoring
       
       if (!MakeNewResolvent()) 
        {
         goto first_factor;
        };
       _continuation = ContFirstFactor;
       return true;
       
       first_factor:
        ResetFactoringState();
       
        next_factor:
         if (NextFactor())
	  {
	   if (!MakeNewResolvent()) goto next_factor;            
           _continuation = ContNextFactor;
           return true;          
          };
  
         // no_more_factors:
         premise2.Unload();
         goto try_next_cand;
     
         // no more candidates in this leaf, try to find another one
         try_next_leaf:
          if (ORMachine.FindNextLeaf()) { goto process_leaf; };
     };
   
   // no more resolvents for this literal
   premise1.UnmarkCut();
   goto next_lit;
  
 }; // bool BIN_ORD_RES::NextInference()


 
bool BIN_ORD_RES::NextFactor()
 {
  CALL("NextFactor()");
  if (premise2.WasFactored())
   {
    if (premise2.GetNextFactor())
     {
      return true;
     }
    else 
     {
      premise2.ResetFactoring();
      if (premise1.WasFactored())
       {
        return premise1.GetNextFactor();
       }
      else
       {
        return premise1.GetFirstFactor(premise1.CutLit()+1);
       };
     };
   }
  else 
   {
    if (premise2.GetFirstFactor(0))
     {
      return true;
     }
    else
     {
      if (premise1.WasFactored())
       {
        return premise1.GetNextFactor();
       }
      else
       {
        return premise1.GetFirstFactor(premise1.CutLit()+1);
       };
     };
   };
 }; // bool BIN_ORD_RES::NextFactor()
 

bool BIN_ORD_RES::MakeNewResolvent()
 {
  CALL("MakeNewResolvent()");
  if (!CollectResult()) return false;

  // resolvent collected in Resolvent
  _resolvent->UsedRule(ClauseBackground::BinRes);
  _resolvent->PushAncestor((Clause*)FirstPremise());
  _resolvent->PushAncestor((Clause*)SecondPremise());
  
  return true;
  //_resolvent->ProcessCollected(); 
 }; // bool BIN_ORD_RES::MakeNewResolvent() 


inline bool BIN_ORD_RES::CollectResult() 
    { 
     CALL("CollectResult()");
     if ((!premise1.TheClause()->Active())
         || (!premise2.TheClause()->Active()))
      return false;
     if (MinResWeight() + premise2.TheClause()->MinResWeight()
         > _resolvent->MaxWeight())
      {
       _resolvent->AnotherTooHeavyClause();
       return false;
      }; 
     _resolvent->Reset();
     return premise1.CollectNonmarkedLiterals() && premise2.CollectNonmarkedLiterals();
    }; // inline bool BIN_ORD_RES::CollectResult() 


inline void BIN_ORD_RES::ResetFactoringState()
 {
  premise1.ResetFactoring();
  premise2.ResetFactoring();
 }; // void BIN_ORD_RES::ResetFactoringState() 
 
inline void BIN_ORD_RES::CollectWeightForm(const Clause* c)
 {
  weight_form.Reset();
  LiteralList* lits = c->LitList();
  for (LiteralList::Iterator iter(lits,lits->nth(lits->numOfSelectedLits()));
       iter.notEnd();
       iter.next())
    if (!iter.currentLiteral()->isAnswerLiteral())
      weight_form.Compile(iter.currentElement()->literal()); 
 }; // inline void CollectWeightForm() 
 
 
}; // namespace VK


//=============================================================
 
 
 
 
 
 
 
 
