//
// File:         LiteralSharing.h
// Description:  Implements literal sharing.
// Created:      Dec 13, 2001 
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
// Note:         Was LITERAL_INDEX in clause_index.h.
// Revised:      Dec 25, 2001. 
//               Freed from HellConst::MaxNumOfFunctors.
//============================================================================
#ifndef LITERAL_SHARING_H
//=============================================================================
#define LITERAL_SHARING_H 
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "DestructionMode.h"
#include "GlobAlloc.h"
#include "Array.h"
#include "Term.h"
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_LITERAL_SHARING
 #define DEBUG_NAMESPACE "LiteralSharing"
#endif
#include "debugMacros.h"
//============================================================================
namespace VK
{
  using namespace BK;
class Flatterm;
class DTREE_NODE;
class SHARING_INDEX;
class LiteralSharing
{
 public:
  LiteralSharing(SHARING_INDEX* si) 
   : 
     #ifdef NO_DEBUG
      _propositional((TERM*)0,(int)0),
     #else
      _propositional((TERM*)0,"LiteralSharing::_propositional"),  
     #endif 

    _sharingIndex(si),

     #ifdef NO_DEBUG
      _discTree((DTREE_NODE*)0,(int)0)
     #else
      _discTree((DTREE_NODE*)0,"LiteralSharing::_discTree")
     #endif 

  {
   CALL("constructor LiteralSharing(SHARING_INDEX* si)");
   
   DOP(_propositional.freeze());  
   DOP(_discTree.freeze()); 
  }; 

  ~LiteralSharing() 
    { 
      CALL("destructor ~LiteralSharing()"); 
      if (DestructionMode::isThorough())
	{
	  for (ulong i = 0; i < _propositional.size(); ++i)
	    if (_propositional[i]) _propositional[i]->DeleteLit();
	};
      
    };
  void Destroy(); 
   
  #ifndef NO_DEBUG   
   bool clean() const;
  #endif
    
  DTREE_NODE** Tree(const TERM& header)
  {
   CALL("Tree(const TERM& header)");
   DOP(_discTree.unfreeze());
   DTREE_NODE** res = &(_discTree.sub(header.HeaderNum()));
   DOP(_discTree.freeze());
   return res;
  };

  DTREE_NODE* Integrate(TERM* lit); // returns the leaf
  
  DTREE_NODE* Integrate(const Flatterm* lit); // returns the leaf 
  DTREE_NODE* IntegrateEq(const TERM& header,
                          const Flatterm* arg1,
                          const Flatterm* arg2); // returns the leaf  
   
  DTREE_NODE* FindLeaf(TERM* lit) // returns the leaf
  {
   // temporary version
   return Integrate(lit); 
  };

  void InfoForDeletion(TERM* lit,
                       DTREE_NODE**& branch_to_destroy,
                       DTREE_NODE*& leaf,
                       DTREE_NODE*& last_fork); // for nonpropositional literals


 private:
 
  static void DestroyLeafContent(DTREE_NODE* leaf);
  TERM* ShareSplittingBranchName(const TERM& prop);
  TERM* ShareProp(const TERM& prop);

 private:

  Array<GlobAlloc,TERM*,128UL,LiteralSharing> _propositional;  

  SHARING_INDEX* _sharingIndex;
  Array<GlobAlloc,DTREE_NODE*,128UL,LiteralSharing> _discTree;
}; // class LiteralSharing

}; // namespace VK
//============================================================================
#endif
