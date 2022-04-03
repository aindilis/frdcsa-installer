//
// File:         ForwardDemodulation.h
// Description:  Implements forward demodulation. 
// Created:      ???, Alexandre Riazanov, riazanov@cs.man.ac.uk
// Revised:      Dec 24, 2001, Alexandre Riazanov, riazanov@cs.man.ac.uk
//               The definition 
//                typedef Multiset<..> SetOfSplittingBranches
// 
//               replaced by 
// 
//                typedef ExpandingMultiset<..> SetOfSplittingBranches
//                       
// Revised:      Apr 29, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//               KBConstraint and SharedKBConstraint are no longer used. 
//               Generic SimplificationOrdering is used instead. 
// Revised:      May 15, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//               RewriteByPreordered, IntegratePreordered and RemovePreordered gone.
//====================================================
#ifndef FORWARD_DEMODULATION_H
//==================================================================
#define FORWARD_DEMODULATION_H
#ifndef NO_DEBUG_VIS
 #include <iostream.h>
#endif 
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "VampireKernelInterceptMacros.h"
#include "VampireKernelConst.h"
#include "GlobAlloc.h"
#include "GList.h"
#include "DestructionMode.h"
#include "ForwardMatchingIndexTree.h"
#include "ForwardMatchingIndex.h"
#include "MultisetOfVariables.h"
#include "ExpandingMultiset.h"
#include "Term.h"
#include "Flatterm.h"
#include "ForwardMatchingSubstitutionCache.h"
#include "Tuple.h"
#include "SimplificationOrdering.h"
namespace VK
{
  using namespace BK;
  using namespace Gematogen;
class Clause;
}; // namespace VK

//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_DEMODULATION
 #define DEBUG_NAMESPACE "ForwardDemodulation"
#endif 
#include "debugMacros.h"
//=================================================

namespace VK
{
  using namespace BK;
  using namespace Gematogen;
class ForwardDemodulation
 {
  public:
   typedef ExpandingMultiset<GlobAlloc,ulong,32UL,ForwardDemodulation> SetOfSplittingBranches;
   class Index 
    {
     public:
      typedef Tuple2<ulong,const Flatterm* const *> VarInstanceLocationPair;
      typedef GList<GlobAlloc,VarInstanceLocationPair,ForwardDemodulation::Index> StoredSubstitution;
      
      class ConstrRule
       {
        public:
	 ConstrRule() : 
	   clause((Clause*)0), 
	   constr((SimplificationOrdering::StoredConstraint*)0),
	   subst((StoredSubstitution*)0),
	   lhs((TERM*)0)
	   {
	   };
	   
	 
	 void init(Clause* cl,SimplificationOrdering::StoredConstraint* con,
		   StoredSubstitution* sub,TERM* t) 
	   {
	     CALL("init(Clause* cl,SimplificationOrdering::StoredConstraint* con,StoredSubstitution* sub,TERM* t)");
	     clause = cl;
	     subst = sub;
	     lhs = t;
	     if (con) { constr = SimplificationOrdering::current()->copy(con); }
	     else constr = (SimplificationOrdering::StoredConstraint*)0;
	   };

	 ~ConstrRule() 
	   {
	     CALL("desructor ~ConstrRule()");
	     if (DestructionMode::isThorough())
	       {
		 if (constr) SimplificationOrdering::current()->releaseConstraint(constr);
		 StoredSubstitution::destroyList(subst);
	       };
	   };
         Clause* clause;
         SimplificationOrdering::StoredConstraint* constr;
         StoredSubstitution* subst;  
         TERM* lhs;  
       private:
	 ConstrRule(const ConstrRule& cr) 
	   {
	     CALL("ConstrRule::constructor ConstrRule(const ConstrRule& cr)");
	     ICP("ICP0");	     
	   };
	 ConstrRule& operator=(const ConstrRule&)
	 {
	   CALL("operator=(const ConstrRule&)");
	   ICP("ICP0");
	   return *this;
	 };
      };
   
      typedef GList<GlobAlloc,ConstrRule,ForwardDemodulation::Index> ConstrRuleList; 
      

      
     public:
      class LeafInfo 
      {
       public:
        void init() { _rules = (ConstrRuleList*)0; }; 
	void destroy() 
	  {
	    CALL("LeafInfo::destroy()");
	    ConstrRuleList::destroyList(_rules);
	    _rules = (ConstrRuleList*)0;
	  };
        ConstrRuleList*& rules() { return _rules; };
       private:
	ConstrRuleList* _rules; 
      };

     public:
      Index() : degree(0L)
       {
	CALL("constructor Index::Index()");
       };
      ~Index() 
	{ 
	  CALL("ForwardDemodulation::destructor ~Index"); 
	  if (DestructionMode::isThorough()) 
	    {
	      Destroy();
	    }; 
	}; 
      void SetDegree(long d) { degree = d; }; 

      void Integrate(Clause* cl);
      void Remove(Clause* cl);
      ForwardMatchingIndexTreeLeaf<VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables,GlobAlloc,TERM,Flatterm,LeafInfo,Index>* Match(const Flatterm* query);
      ForwardMatchingIndexTreeLeaf<VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables,GlobAlloc,TERM,Flatterm,LeafInfo,Index>* MatchAgain();


      void Destroy() 
      { 
	CALL("Destroy()");
       _matchingIndex.Destroy(); 
      };
      #ifndef NO_DEBUG
       bool clean() const { return  _matchingIndex.clean(); };
      #endif
      #ifndef NO_DEBUG_VIS       
       ostream&  output(ostream& str) const { return _matchingIndex.output(str); }; 
      #endif     

     private:
      ForwardMatchingIndexTreeLeaf<VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables,GlobAlloc,TERM,Flatterm,LeafInfo,Index>* Integrate(TERM* t);
      bool Remove(TERM* t,ForwardMatchingIndexTreeLeaf<VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables,GlobAlloc,TERM,Flatterm,LeafInfo,Index>*& leaf,ForwardMatchingIndexTreeInstruction<VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables,GlobAlloc,TERM,Flatterm,LeafInfo,Index>**& branch_to_destroy);
      void IntegrateConstrained(Clause* cl);
      
      void IntegrateConstrained(Clause* cl,
				TERM* t,
				SimplificationOrdering::StoredConstraint* constr);
      void RemoveConstrained(Clause* cl);
      void RemoveConstrained(Clause* cl,TERM* lhs);

      bool appropriateDegreeForGreater(const SimplificationOrdering::SpecialisedComparison::Category& cat) const;
      bool appropriateDegreeForLess(const SimplificationOrdering::SpecialisedComparison::Category& cat) const;

      StoredSubstitution* createStoredSubstitution();

     private:  
      ForwardMatchingIndex<VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables,GlobAlloc,TERM,Flatterm,LeafInfo,Index> _matchingIndex;

      long degree;
      
    }; // class Index




  public:
   ForwardDemodulation() : 
    fDOnSplittingBranchesFlag(false)
   {
    CALL("constructor ForwardDemodulation()");
    _constrCheck = (SimplificationOrdering::ConstraintCheckOnForwardMatchingSubst*)0;
   };
   ~ForwardDemodulation() 
     {
       CALL("destructor ~ForwardDemodulation()");
       if (DestructionMode::isThorough()) Destroy(); 
     };
   void SetDegree(long d) { degree = d; index.SetDegree(d); };
   void SetFDOnSplittingBranchesFlag(bool fl) 
   {
    if (fDOnSplittingBranchesFlag && (!fl)) RuntimeError::report("Switching fDOnSplittingBranchesFlag off.");
    fDOnSplittingBranchesFlag = fl; 
   }; 
   void Integrate(Clause* cl);
   void Remove(Clause* cl);

   static Index::ConstrRuleList* removeConstrRule(Clause* cl,TERM* t,Index::ConstrRuleList* lst);
   static Index::ConstrRuleList* insertConstrRule(Clause* cl,SimplificationOrdering::StoredConstraint* constr,Index::StoredSubstitution* subst,TERM* t,Index::ConstrRuleList* lst);

   bool Rewrite(const Flatterm* query,const SetOfSplittingBranches& splittingBranches)
   {
    CALL("Rewrite(const Flatterm* query,const SetOfSplittingBranches& splittingBranches)");
    ASSERT(degree > 0); 
    branchesUsed = false;
    return RewriteByConstrained(query,splittingBranches);
   };

   Clause* Rule() const { return rule; };  
   bool RHSNext() { return _ruleRHSInstance.next(); };    
   TERM RHSSymbol() { return _ruleRHSInstance.symbol(); };   
   bool SplittingBranchesUsed() const { return branchesUsed; }; 

   void Destroy();

   #ifndef NO_DEBUG
    bool clean() const { return index.clean(); };
   #endif

   #ifndef NO_DEBUG_VIS 
    ostream& outputIndex(ostream& str) { return index.output(str); };
   #endif

  private:   

   bool CanBeRewritingRule(const Clause* cl);   
   bool CheckSplittingBranches(const Clause* cl,const SetOfSplittingBranches& splittingBranches);
   bool RewriteByConstrained(const Flatterm* query,const SetOfSplittingBranches& splittingBranches);

   bool checkConstraint(const SimplificationOrdering::StoredConstraint* constr);



   void collectSubstCache(const Index::StoredSubstitution* storedSubst);

  private: // data
   Index index;
   Clause* rule;
   
   ForwardMatchingSubstitutionCache::Instance _ruleRHSInstance;

   long degree;
   bool fDOnSplittingBranchesFlag;
   bool branchesUsed;
   ForwardMatchingSubstitutionCache _substCache;
   SimplificationOrdering::ConstraintCheckOnForwardMatchingSubst* _constrCheck;
 }; // class ForwardDemodulation

}; // namespace VK

//=================================================================
#endif
