//
// File:         ForwardMatchingIndex.h
// Description:  Interface for the code trees for matching.
// Created:      ???
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
// Note:         This is a part of the Gematogen library.
// Revised:      Dec 25, 2001. 
//               ForwardMatchingIndex<..> freed from MaxNumOfFunctors
//               by replacing the top level array by an extendable one.
//====================================================
#ifndef FORWARD_MATCHING_INDEX_H
//==================================================================
#define FORWARD_MATCHING_INDEX_H
#ifndef NO_DEBUG_VIS
#include <iostream.h>
#endif
#include "jargon.h"
#include "GematogenDebugFlags.h" 
#include "MultisetOfVariables.h"
#include "Array.h"
#include "ForwardMatchingIndexTree.h"
#include "DestructionMode.h"
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_MATCHING_INDEX
#define DEBUG_NAMESPACE "ForwardMatchingIndex<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>"
#endif 
#include "debugMacros.h"
//=================================================

namespace Gematogen
{

template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId>
class ForwardMatchingIndex
{ 
 public: 
  typedef ForwardMatchingIndexTreeInstruction<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId> Instruction;
  typedef ForwardMatchingIndexTreeLeaf<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId> LeafInstr;
 public:
  ForwardMatchingIndex() 
    : 
#ifdef NO_DEBUG 
    tree((Instruction*)0,(int)0),
#else
    tree((Instruction*)0,"ForwardMatchingIndex<..>::tree"),
#endif
    integrator(subst), 
    removal(subst), 
    retrieval(), 
    _output()
    {
      CALL("constructor ForwardMatchingIndex()");
      DOP(tree.freeze());
      _output.SetSubst(subst);     
    };
  ~ForwardMatchingIndex() 
    { 
      CALL("destructor ~ForwardMatchingIndex()"); 
      if (DestructionMode::isThorough()) Destroy();
    }; 
   
  inline void integrationReset(const Symbol& fun)
    {
      CALL("integrationReset(const Symbol& fun)");
      ASSERT(fun.isComplex());
      DOP(tree.unfreeze());
      integrator.Reset(&(tree.sub(fun.functor())));
      DOP(tree.freeze());
    };    
  inline void integrationVar(const Symbol& var) 
    {
      CALL("integrationVar(const Symbol& var)");
      ASSERT(var.isVariable());
      integrator.PushVar(var);
    }; 
  inline void integrationFunc(const Symbol& fun)
    {
      CALL("integrationFunc(const Symbol& fun)");
      ASSERT(fun.isComplex());
      integrator.PushFunc(fun);
    };
  LeafInstr* integrationGetLeaf(bool& newLeaf)  
    {
      CALL("integrationGetLeaf()");
      return integrator.Leaf(newLeaf);
    };   
  

  MultisetOfVariables<MaxNumOfVariables>& integrationGetVariables() { return integrator.Vars(); };
  const Flatterm** integrationMapVariable(ulong v) { return integrator.MapVar(v); };

  bool removalReset(const Symbol& fun)
    {
      CALL("removalReset(const Symbol& fun)");
      ASSERT(fun.isComplex());
      if (fun.functor() >= tree.size()) return false;
      Instruction** tree_addr = &(tree[fun.functor()]);  
      if (!(*tree_addr)) return false;
      removal.Reset(tree_addr);
      return true;
    };
   
  bool removalVar(const Symbol& var) 
    {
      CALL("removalVar(const Symbol& var)");
      ASSERT(var.isVariable());
      return removal.SkipVar(var);
    };
   
  bool removalFunc(const Symbol& fun) 
    {
      CALL("removalFunc(const Symbol& fun)");
      ASSERT(fun.isComplex());
      return removal.SkipFunc(fun);
    };
 
  bool removalFinish(LeafInstr*& leaf,Instruction**& branch_to_destroy)
    {
      CALL("removalGetLeaf(LeafInstr*& leaf,Instruction**& branch_to_destroy)");
      if (!removal.SkipComps()) return false;
      leaf = removal.Leaf();
      ASSERT(leaf->isLeaf());   
      branch_to_destroy = removal.BranchToDestroy();
      return true;
    };

  LeafInstr* Match(const Flatterm* query);
  LeafInstr* MatchAgain() 
    { 
      CALL("MatchAgain()"); 
      return retrieval.MatchAgain(); 
    };

  void Destroy()       
    {
      CALL("Destroy()");
      for (ulong f = 0; f < tree.size(); f++) 
	{
	  if (tree[f])
	    { 
	      ForwardMatchingIndexTree<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::Destroy(tree[f]);
	      tree[f] = (Instruction*)0;     
	    };
	};
    }; // void Destroy()    
 
#ifndef NO_DEBUG
  bool clean() const
    {
      CALL("clean() const");
      for (ulong f = 0; f < tree.size(); f++)  
	if (tree[f]) return false;
      return true; 
    };
  const ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>& debugIntegrator() { return integrator; };
#endif
#ifndef NO_DEBUG_VIS
  ostream& output(ostream& str) const;
#endif


 private:

  Array<Alloc,Instruction*,128UL,ForwardMatchingIndex> tree;


  const Flatterm* subst[MaxTermSize];
  ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId> integrator;
  ForwardMatchingIndexTreeRemoval<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId> removal;
  ForwardMatchingIndexTreeRetrieval<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId> retrieval;
  ForwardMatchingIndexTreeOutput<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId> _output;

}; // template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> class ForwardMatchingIndex

template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId>
inline ForwardMatchingIndexTreeLeaf<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>* ForwardMatchingIndex<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::Match(const Flatterm* query)
{
  CALL("Match(const Flatterm* query)");
  ASSERT(query->symbolRef().isComplex());
  ulong funcNum = query->symbolRef().functor();
  if (funcNum >= tree.size()) 
    return (ForwardMatchingIndexTreeLeaf<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>*)0;
  return retrieval.Match(query + 1,tree[funcNum]);    
}; // LeafInstr* ForwardMatchingIndex<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::Match(const Flatterm* query) 


#ifndef NO_DEBUG_VIS
template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId>
ostream& ForwardMatchingIndex<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::output(ostream& str) const
{
  for (ulong f = 0; f < tree.size(); f++)
    {
      const Instruction* t = tree[f];
      if (t)
	{
	  Symbol functor(0UL,f,Symbol::PositivePolarity);
	  str << "HEADER " << functor << '\n';
	  _output.Tree(str,t) << "\nEND OF TREE FOR " << functor << "\n\n";
	};  
    };  
  return str;
}; // ostream& ForwardMatchingIndex<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::output(ostream& str) const
#endif

}; // namespace Gematogen

//================================================
#undef DEBUG_NAMESPACE  
//==================================================================
#endif





















