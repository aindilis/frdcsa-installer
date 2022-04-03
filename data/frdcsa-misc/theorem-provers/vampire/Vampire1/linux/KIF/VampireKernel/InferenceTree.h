//
// File:         InferenceTree.h
// Description:  Collecting inference trees.
// Created:      Feb 13, 2000, 18:00
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//============================================================================
#ifndef INFERENCE_TREE_H
//=============================================================================
#define INFERENCE_TREE_H 
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "GSortTree.h"
#include "Clause.h"
#include "GlobAlloc.h"
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_INFERENCE_TREE
 #define DEBUG_NAMESPACE "InferenceTree"
#endif
#include "debugMacros.h"
//============================================================================
namespace VK
{
  using namespace BK;
class InferenceTree
{
 public:
  InferenceTree() 
  {
   CALL("constructor InferenceTree()");
  };
  InferenceTree(const Clause* root) : collected(), iter()
  {
   CALL("constructor InferenceTree(const Clause* root)");
   collect(root);
  };
  ~InferenceTree() { CALL("destructor ~InferenceTree()"); };
  void collect(const Clause* root);
  void resetIter() { iter.reset(collected); };
  const Clause* getNextClause() 
  {
   if (iter)
    {
     const Clause* res = iter.value().clause();
     iter.next();
     return res; 
    }; 
   return (const Clause*)0; 
  };
 private:
  class ClWrapper
  {
   public:
    ClWrapper(const Clause* c) : cl(c) {};
    ~ClWrapper() {};
    const Clause* clause() const { return cl; };
    bool operator==(const ClWrapper& cw) const
    {
     return cl->Number() == cw.clause()->Number();
    };
    bool operator<(const ClWrapper& cw) const
    {
     return cl->Number() < cw.clause()->Number();
    };
    bool operator>(const ClWrapper& cw) const
    {
     return cl->Number() > cw.clause()->Number();
    };
   private:
    ClWrapper() { ERROR("Default constructor called"); };
   private:  
    const Clause* cl; 
  };
  typedef GSortTree<GlobAlloc,ClWrapper,InferenceTree> ClBag;
 private:
  void collectInf(const Clause* root);
 private:
  ClBag collected;
  ClBag::LeftRightIter iter;    
}; //class InferenceTree 

}; // namespace VK

//======================================================================
#undef DEBUG_NAMESPACE
//======================================================================
#endif
