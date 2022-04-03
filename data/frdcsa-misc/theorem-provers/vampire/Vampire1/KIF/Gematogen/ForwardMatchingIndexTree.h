//
// File:         ForwardMatchingIndexTree.h
// Description:  Code trees for matching.
// Created:      Feb 15, 2000, 15:20
// Revised:      Jan 10, 2001
// Revised:      Jan 22, 2001 (made generic) 
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
// Note:         This is a part of the Gematogen library. 
//====================================================
#ifndef FORWARD_MATCHING_INDEX_TREE_H
//=============================================================
#define FORWARD_MATCHING_INDEX_TREE_H
#ifndef NO_DEBUG_VIS 
#include <iostream.h>
#endif
#include <limits.h>
#include "jargon.h"
#ifdef _SUPPRESS_WARNINGS_
#include "RuntimeError.h"
#endif
#include "GlobalStopFlag.h"
#include "GematogenDebugFlags.h"
#include "Stack.h"
#include "EquivalenceRelation.h"
#include "MultisetOfVariables.h"
#include "DestructionMode.h"
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_MATCHING_INDEX_TREE
#define DEBUG_NAMESPACE "ForwardMatchingIndexTreeInstruction<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>"
#endif
#include "debugMacros.h"
//=================================================

namespace Gematogen
{
  using namespace BK;

template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
class ForwardMatchingIndexTreeInstruction
{
 public:
  enum Tag { Func, Var, Compare, Leaf };
 public:
  ForwardMatchingIndexTreeInstruction(Tag t)  : _tag(t)
    {
    };
  ~ForwardMatchingIndexTreeInstruction() 
    {
      CALL("destructor ~ForwardMatchingIndexTreeInstruction()");
      if (isLeaf())
	if (DestructionMode::isThorough())
	  {
	    leafInfo().destroy();
	  };
    };
  void operator delete(void* obj)
    { 
      CALL("operator delete(void* obj)");
#ifdef DEBUG_ALLOC_OBJ_TYPE
      switch (((ForwardMatchingIndexTreeInstruction*)obj)->_tag)  
	{
	case Func:
	  ALWAYS(_classDesc.registerDeallocated(sizeof(TagAndNextAndForkAndSymbol))); 
	  Alloc::deallocate(obj,sizeof(TagAndNextAndForkAndSymbol),&_classDesc); 
	  break;
	case Var:
	  ALWAYS(_classDesc.registerDeallocated(sizeof(TagAndNextAndForkAndVar)));  
	  Alloc::deallocate(obj,sizeof(TagAndNextAndForkAndVar),&_classDesc); 
	  break;
	case Compare:
	  ALWAYS(_classDesc.registerDeallocated(sizeof(TagAndNextAndForkAndVar1AndVar2)));  
	  Alloc::deallocate(obj,sizeof(TagAndNextAndForkAndVar1AndVar2),&_classDesc); 
	  break;
	case Leaf: 
	  ALWAYS(_classDesc.registerDeallocated(sizeof(TagAndLeafInfo))); 
	  Alloc::deallocate(obj,sizeof(TagAndLeafInfo),&_classDesc); 
	  break;
#ifdef DEBUG_NAMESPACE
	default: ICP("0");   
#else
#ifdef _SUPPRESS_WARNINGS_
        default: RuntimeError::report("ICP in ForwardMatchingIndexTreeInstruction<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::delete(void* obj)");
#endif
#endif
	};        
#else
      switch (((ForwardMatchingIndexTreeInstruction*)obj)->_tag)  
	{
	case Func: Alloc::deallocate(obj,sizeof(TagAndNextAndForkAndSymbol)); break;
	case Var: Alloc::deallocate(obj,sizeof(TagAndNextAndForkAndVar)); break;
	case Compare: Alloc::deallocate(obj,sizeof(TagAndNextAndForkAndVar1AndVar2)); break;
	case Leaf: Alloc::deallocate(obj,sizeof(TagAndLeafInfo)); break;
#ifdef DEBUG_NAMESPACE
	default: ICP("0");   
#else
#ifdef _SUPPRESS_WARNINGS_
        default: RuntimeError::report("ICP in ForwardMatchingIndexTreeInstruction<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::delete(void* obj)");
#endif
#endif
	};              
#endif
    };
  
  Tag tag() const 
    {
      CALL("tag()");
      ASSERT(checkObj()); 
      return _tag; 
    };

  bool isFunc() const 
    {
      CALL("isFunc() const"); 
      ASSERT(checkObj()); 
      return (_tag == Func); 
    };
  bool isVar() const 
    {
      CALL("isVar() const");
      ASSERT(checkObj()); 
      return (_tag == Var); 
    };
  bool isCompare() const 
    {
      CALL("isCompare() const"); 
      ASSERT(checkObj()); 
      return (_tag == Compare); 
    };
  bool isLeaf() const 
    {
      CALL("isLeaf() const");
      ASSERT(checkObj()); 
      return (_tag == Leaf); 
    };    
 
  ForwardMatchingIndexTreeInstruction** nextAddr() 
    { 
      CALL("nextAddr()");
      ASSERT(checkObj()); 
      ASSERT(_tag != Leaf);
      return (ForwardMatchingIndexTreeInstruction**)(((OnlyTag*)this) + 1); 
    };
  ForwardMatchingIndexTreeInstruction* const * nextAddr() const 
    {
      CALL("nextAddr() const"); 
      ASSERT(checkObj());
      ASSERT(checkObj());  
      return (ForwardMatchingIndexTreeInstruction**)(((OnlyTag*)this) + 1); 
    };
  ForwardMatchingIndexTreeInstruction* next() const { return *(nextAddr()); };
  void setNext(ForwardMatchingIndexTreeInstruction* n) { *(nextAddr()) = n;  };

  ForwardMatchingIndexTreeInstruction** forkAddr() 
    { 
      CALL("forkAddr()");
      ASSERT(checkObj()); 
      ASSERT(_tag != Leaf);
      return (ForwardMatchingIndexTreeInstruction**)(((TagAndNext*)this) + 1); 
    };
  ForwardMatchingIndexTreeInstruction* const * forkAddr() const 
    {
      CALL("forkAddr() const");
      ASSERT(checkObj()); 
      ASSERT(_tag != Leaf); 
      return (ForwardMatchingIndexTreeInstruction**)(((TagAndNext*)this) + 1); 
    }; 
  ForwardMatchingIndexTreeInstruction* fork() const { return *(forkAddr()); };     
  void setFork(ForwardMatchingIndexTreeInstruction* f) { *(forkAddr()) = f; };

  // Methods for Func:
  Symbol* funcAddr() 
    {
      CALL("funcAddr()");
      ASSERT(checkObj(Func)); 
      return (Symbol*)(((TagAndNextAndFork*)this) + 1); 
    };
  Symbol* const funcAddr() const 
    {
      CALL("funcAddr() const"); 
      ASSERT(checkObj(Func)); 
      return (Symbol*)(((TagAndNextAndFork*)this) + 1); 
    };
  const Symbol& func() const { return *(funcAddr()); };
  void setFunc(const Symbol& f) 
    { 
      *(funcAddr()) = f; 
    };
  static ForwardMatchingIndexTreeInstruction* newFunc(const Symbol& f)
    {
      ForwardMatchingIndexTreeInstruction* res = new (sizeof(TagAndNextAndForkAndSymbol)) ForwardMatchingIndexTreeInstruction(Func);
      res->setFunc(f);
      res->setNext((ForwardMatchingIndexTreeInstruction*)0);
      res->setFork((ForwardMatchingIndexTreeInstruction*)0);
      return res;
    };

  // Methods for Var:
  const Flatterm*** varAddr() 
    {
      CALL("varAddr()");
      ASSERT(checkObj(Var)); 
      return (const Flatterm***)(((TagAndNextAndFork*)this) + 1); 
    };
  const Flatterm** const * varAddr() const 
    {
      CALL("varAddr() const"); 
      ASSERT(checkObj(Var)); 
      return (const Flatterm***)(((TagAndNextAndFork*)this) + 1); 
    };
  const Flatterm** var() const { return *(varAddr()); };
  void setVar(const Flatterm** v) { *(varAddr()) = v; };      
  static ForwardMatchingIndexTreeInstruction* newVar(const Flatterm** v)
    {
      ForwardMatchingIndexTreeInstruction* res = new (sizeof(TagAndNextAndForkAndVar)) ForwardMatchingIndexTreeInstruction(Var);
      res->setVar(v);
      res->setNext((ForwardMatchingIndexTreeInstruction*)0);
      res->setFork((ForwardMatchingIndexTreeInstruction*)0);
      return res;
    };

  // Methods for Compare:
  const Flatterm*** var1Addr() 
    {
      CALL("var1Addr()"); 
      ASSERT(checkObj(Compare)); 
      return (const Flatterm***)(((TagAndNextAndFork*)this) + 1); 
    };
  const Flatterm** const * var1Addr() const 
    {
      CALL("var1Addr() const");  
      ASSERT(checkObj(Compare)); 
      return (const Flatterm***)(((TagAndNextAndFork*)this) + 1); 
    };
  const Flatterm** var1() const { return *(var1Addr()); };
  void setVar1(const Flatterm** v) { *(var1Addr()) = v; };    

  const Flatterm*** var2Addr() 
    {
      CALL("var2Addr()"); 
      ASSERT(checkObj(Compare)); 
      return (const Flatterm***)(((TagAndNextAndForkAndVar1*)this) + 1);
    };
  const Flatterm** const * var2Addr() const 
    {    
      CALL("var2Addr() const");  
      ASSERT(checkObj(Compare)); 
      return (const Flatterm***)(((TagAndNextAndForkAndVar1*)this) + 1);
    };
  const Flatterm** var2() const { return *(var2Addr()); };
  void setVar2(const Flatterm** v) { *(var2Addr()) = v; };

  void bothVars(const Flatterm**& v1,const Flatterm**& v2) 
    {
      CALL("bothVars(const Flatterm**& v1,const Flatterm**& v2)"); 
      ASSERT(checkObj(Compare));  
      v1 = var1(); 
      v2 = var2(); 
    };
  static ForwardMatchingIndexTreeInstruction* newCompare(const Flatterm** v1,const Flatterm** v2)
    {
      CALL("newCompare(const Flatterm** v1,const Flatterm** v2)");
      ASSERT(v1 < v2);
      ForwardMatchingIndexTreeInstruction* res = new (sizeof(TagAndNextAndForkAndVar1AndVar2)) ForwardMatchingIndexTreeInstruction(Compare);
      res->setVar1(v1);
      res->setVar2(v2);
      res->setNext((ForwardMatchingIndexTreeInstruction*)0);
      res->setFork((ForwardMatchingIndexTreeInstruction*)0);
      return res;
    }; 

  // Methods for Leaf:
  LeafInfo* leafInfoAddr() 
    {
      CALL("leafInfoAddr()");
      ASSERT(checkObj(Leaf)); 
      return (LeafInfo*)(((OnlyTag*)this) + 1); 
    };     
  const LeafInfo* leafInfoAddr() const  
    {
      CALL("leafInfoAddr() const");
      ASSERT(checkObj(Leaf)); 
      return (const LeafInfo*)(((OnlyTag*)this) + 1); 
    };    
  LeafInfo& leafInfo() 
    {
      CALL("leafInfo()");
      ASSERT(checkObj(Leaf)); 
      return *(leafInfoAddr()); 
    }; 
  const LeafInfo& leafInfo() const
    {
      CALL("leafInfo() const");
      ASSERT(checkObj(Leaf)); 
      return *(leafInfoAddr()); 
    }; 


  static ForwardMatchingIndexTreeInstruction* newLeaf()
    {
      ForwardMatchingIndexTreeInstruction* res = new (sizeof(TagAndLeafInfo)) ForwardMatchingIndexTreeInstruction(Leaf);
      return res;
    };

 public:
#ifndef NO_DEBUG_VIS 
  ostream& output(ostream& str,const Flatterm* const * subst) const;
#endif

#ifndef NO_DEBUG 
  static ulong sizeOfInstr(Tag t) 
    {
      switch (t)
	{
	case Func: return sizeof(TagAndNextAndForkAndSymbol);
	case Var: return sizeof(TagAndNextAndForkAndVar);
	case Compare: return sizeof(TagAndNextAndForkAndVar1AndVar2);
	case Leaf: return sizeof(TagAndLeafInfo);
	};  
      return ULONG_MAX;
    };   
 
  bool checkObj() const
    {
#ifndef DEBUG_ALLOC
      return true;
#else 
      ObjDesc* objDesc = Alloc::hostObj((void*)this);
      if (!objDesc) 
	{ 
	  DMSG("ForwardMatchingIndexTreeInstruction<..>::checkObj()> Null pointer to object descriptor.\n");  
	  return false; 
	}; 
      bool res = true;   
      switch (_tag) 
	{
        case Func: break;
        case Var: break;
        case Compare: break;
        case Leaf: break;
        default: 
	  DMSG("ForwardMatchingIndexTreeInstruction<..>::checkObj()> Tag out of range.\n"); 
	  res = false;
	};          

#ifdef DEBUG_ALLOC_OBJ_SIZE
      if (objDesc->size !=  sizeOfInstr(_tag)) 
        {
	  DMSG("ForwardMatchingIndexTreeInstruction<..>::checkObj()> objDesc->size !=  sizeOfInstr(_tag)\n");
	  res = false;
        };
#endif
#ifdef DEBUG_ALLOC_OBJ_STATUS
      if (objDesc->status != ObjDesc::Persistent) 
        {
	  DMSG("ForwardMatchingIndexTreeInstruction<..>::checkObj()> objDesc->status != ObjDesc::Persistent\n");
	  res = false;
        };
#endif
#ifdef DEBUG_ALLOC_OBJ_TYPE
      if (objDesc->classDesc != &_classDesc) 
        {
	  DMSG("ForwardMatchingIndexTreeInstruction<..>::checkObj()> objDesc->classDesc != &_classDesc\n");
	  res = false;
        };
#endif
      if (!res) DOP(DMSG("ForwardMatchingIndexTreeInstruction<..>::checkObj()> host object ") << *objDesc << "\n"); 
      return res;
#endif
    }; // bool checkObj() const

  bool checkObj(Tag t) const
    {
#ifndef DEBUG_ALLOC
      return true;
#else 
      ObjDesc* objDesc = Alloc::hostObj((void*)this);
      if (!objDesc) 
	{ 
	  DMSG("ForwardMatchingIndexTreeInstruction<..>::checkObj(Tag t)> Null pointer to object descriptor.\n");  
	  return false; 
	}; 
      bool res = true;   
      if (_tag != t) 
	{
	  DMSG("ForwardMatchingIndexTreeInstruction<..>::checkObj(Tag t)> _tag != t.\n"); 
	  res = false;
	};

#ifdef DEBUG_ALLOC_OBJ_SIZE
      if (objDesc->size !=  sizeOfInstr(_tag)) 
        {
	  DMSG("ForwardMatchingIndexTreeInstruction<..>::checkObj(Tag t)> objDesc->size !=  sizeOfInstr(_tag)\n");
	  res = false;
        };
#endif
#ifdef DEBUG_ALLOC_OBJ_STATUS
      if (objDesc->status != ObjDesc::Persistent) 
        {
	  DMSG("ForwardMatchingIndexTreeInstruction<..>::checkObj(Tag t)> objDesc->status != ObjDesc::Persistent\n");
	  res = false;
        };
#endif
#ifdef DEBUG_ALLOC_OBJ_TYPE
      if (objDesc->classDesc != &_classDesc) 
        {
	  DMSG("ForwardMatchingIndexTreeInstruction<..>::checkObj(Tag t)> objDesc->classDesc != &_classDesc\n");
	  res = false;
        };
#endif
      if (!res) DOP(DMSG("ForwardMatchingIndexTreeInstruction<..>::checkObj(Tag t)> host object ") << *objDesc << "\n"); 
      return res;
#endif
    }; // bool checkObj(Tag t) const

#endif   



 private: // size templates
  class OnlyTag
    {
    private: 
      Tag _tag;
    };
  class TagAndNext
    {
    private: 
      Tag _tag;
      ForwardMatchingIndexTreeInstruction* _next;
    };

  class TagAndNextAndFork
    {
    private: 
      Tag _tag;
      ForwardMatchingIndexTreeInstruction* _next;
      ForwardMatchingIndexTreeInstruction* _fork;   
    }; 

  class TagAndNextAndForkAndSymbol // Func
    {
    private: 
      Tag _tag;
      ForwardMatchingIndexTreeInstruction* _next;
      ForwardMatchingIndexTreeInstruction* _fork;   
      Symbol _sym;
    }; 
  class TagAndNextAndForkAndVar // Var
    {
    private: 
      Tag _tag;
      ForwardMatchingIndexTreeInstruction* _next;
      ForwardMatchingIndexTreeInstruction* _fork; 
      const Flatterm** _var;
    };

  class TagAndNextAndForkAndVar1
    {
    private: 
      Tag _tag;
      ForwardMatchingIndexTreeInstruction* _next;
      ForwardMatchingIndexTreeInstruction* _fork; 
      const Flatterm** _var1;
    };

  class TagAndNextAndForkAndVar1AndVar2 // Compare
    {
    private: 
      Tag _tag;
      ForwardMatchingIndexTreeInstruction* _next;
      ForwardMatchingIndexTreeInstruction* _fork; 
      const Flatterm** _var1;
      const Flatterm** _var2;
    };

  class TagAndLeafInfo // Leaf
    {
    private: 
      Tag _tag;
      LeafInfo _leafInfo;
    };



 private: 
  void* operator new(size_t,size_t size)
    {
#ifdef DEBUG_ALLOC_OBJ_TYPE
      ALWAYS(_classDesc.registerAllocated(size));
      return Alloc::allocate(size,&_classDesc);
#else
      return Alloc::allocate(size);
#endif
    };  

 private: // data
  Tag _tag;
   
#ifdef DEBUG_ALLOC_OBJ_TYPE
  static ClassDesc _classDesc;
#endif
}; // template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> class ForwardMatchingIndexTreeInstruction

}; // namespace Gematogen

//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_MATCHING_INDEX_TREE
#define DEBUG_NAMESPACE "ForwardMatchingIndexTreeLeaf<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>"
#endif
#include "debugMacros.h"
//=================================================

namespace Gematogen
{

template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
class ForwardMatchingIndexTreeLeaf : private ForwardMatchingIndexTreeInstruction<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>
{
 public:
  ForwardMatchingIndexTreeLeaf() {};
  LeafInfo* leafInfoAddr() { return ForwardMatchingIndexTreeInstruction<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::leafInfoAddr(); };
  LeafInfo& leafInfo() { return ForwardMatchingIndexTreeInstruction<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::leafInfo(); };

#ifndef NO_DEBUG
  bool isLeaf() const { return ForwardMatchingIndexTreeInstruction<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::isLeaf(); };
#endif 

}; // template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> class ForwardMatchingIndexTreeLeaf


}; // namespace Gematogen


//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_MATCHING_INDEX_TREE
#define DEBUG_NAMESPACE "ForwardMatchingIndexTree<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>"
#endif
#include "debugMacros.h"
//=================================================


namespace Gematogen
{


template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId>  
class ForwardMatchingIndexTree
{ 
 public: 
  typedef ForwardMatchingIndexTreeInstruction<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId> Instruction;
 public:
  static void Destroy(Instruction* tree);  
  static void DestroyBranch(Instruction** branch);
}; // template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> class ForwardMatchingIndexTree 


}; // namespace Gematogen



//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_MATCHING_INDEX_TREE
#define DEBUG_NAMESPACE "ForwardMatchingIndexTreeIntegrator"
#endif 
#include "debugMacros.h"
//=================================================
namespace Gematogen
{

template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
class ForwardMatchingIndexTreeIntegrator // objects must be static - too costly constructor
{
 public: 
  typedef ForwardMatchingIndexTreeInstruction<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId> Instruction;
  typedef ForwardMatchingIndexTreeLeaf<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId> LeafInstr; 
 public:
  ForwardMatchingIndexTreeIntegrator(const Flatterm** subst)
    : _constrToInsert("_constrToInsert"),
    _constrToShift("_constrToShift"),
    _constrPassed("_constrPassed"),
    _constrPassedInShift("_constrPassedInShift"),
    _constrIntegrated("_constrIntegrated"),
    _constrOld("_constrOld"),
    _matchingIndexSubstitution(subst),
    _registeredVariables()      
#ifdef DEBUG_NAMESPACE
    , _debugCodeSequence("_debugCodeSequence"), _debugCommandEval("_debugCommandEval")
#endif 
    {};
  ~ForwardMatchingIndexTreeIntegrator() {};

#ifdef DEBUG_NAMESPACE 
  void debugPosInstruction(Instruction* n) { _debugCodeSequence.push(n); _debugCommandEval.push(true); };
  void debugNegInstruction(Instruction* n) { _debugCodeSequence.push(n); _debugCommandEval.push(false); };
#ifndef NO_DEBUG_VIS
  void debugShowSequence(ostream& str) const 
    {
      for (ulong i = 0; i < _debugCodeSequence.size(); i++)
	{
	  str << ((_debugCommandEval.nth(i)) ? " +++ " : " --- "); 
	  _debugCodeSequence.nth(i)->output(str,_matchingIndexSubstitution) << '\n';           
	};
    };
  void debugShowPosSequence(ostream& str) const 
    {
      for (ulong i = 0; i < _debugCodeSequence.size(); i++)
	{
	  if (_debugCommandEval.nth(i)) _debugCodeSequence.nth(i)->output(str,_matchingIndexSubstitution) << '\n';           
	};
    };
#endif
#endif

  void Reset(Instruction** n)
    {
      _nodeAddr = n;
      _constrToInsert.Reset();
      _constrToShift.Reset();
      _constrPassed.Reset();
      //_constrPassedInShift.Reset();
      _constrIntegrated.Reset();
      _constrOld.Reset();
      _registeredVariables.Reset();
      _currentMatchingIndexVar = _matchingIndexSubstitution;
      _readModeFlag = (bool)(*_nodeAddr);
#ifdef DEBUG_NAMESPACE
      _debugCodeSequence.reset();
      _debugCommandEval.reset();
#endif
    };

  MultisetOfVariables<MaxNumOfVariables>& Vars() { return _registeredVariables; };

  const Flatterm** MapVar(ulong v) const { return _firstVariableOccurence[v]; }; 

  void Push(const Symbol& sym)
    { 
      if (sym.isVariable()) { PushVar(sym); } else PushFunc(sym); 
    };  
  void PushVar(const Symbol& sym);
  void PushFunc(const Symbol& sym);

  LeafInstr* Leaf(bool& newLeaf)
    {
      CALL("Leaf()");
      _readModeFlag = _readModeFlag && ReadCollectedComps(); 
      newLeaf = !_readModeFlag;       
      if (_readModeFlag) 
	{
	  ASSERT((*_nodeAddr)->isLeaf());
	  DOP(debugPosInstruction(*_nodeAddr));
	  return (LeafInstr*)(*_nodeAddr);
	};
      // write mode
      *_nodeAddr = Instruction::newLeaf();
      DOP(debugPosInstruction(*_nodeAddr));
      return (LeafInstr*)(*_nodeAddr);
    };
 
 private: 
  typedef EquivalenceRelation<MaxTermSize> EqualityConstraint;

 private:
  void MoveDown() { _nodeAddr = (*_nodeAddr)->nextAddr(); };
  void MoveRight() { _nodeAddr = (*_nodeAddr)->forkAddr(); };
  ulong VarNum(const Flatterm** v) const { return v - _matchingIndexSubstitution; };
  const Flatterm** Var(const ulong& var_num) const 
    {
      CALL("Var(ulong var_num) const");
      ASSERT(var_num < MaxTermSize); 
      return _matchingIndexSubstitution + var_num; 
    };
#ifdef DEBUG_NAMESPACE
  bool debugInSubst(const Flatterm** var) const 
    { 
      return (var >= _matchingIndexSubstitution) && (var < (_matchingIndexSubstitution + MaxTermSize));
    }; 
#endif

  void MakeShifted(const ulong& v1,const ulong& v2) 
    {
      CALL("MakeShifted(const ulong& v1,const ulong& v2)");
      ASSERT(v1 < v2);
      ASSERT(!_constrIntegrated.Equivalent(v1,v2));
      _constrToShift.MakeEquivalent(v1,v2);
    };

  void MakeIntegrated(const ulong& v1,const ulong& v2) 
    {
      CALL("MakeIntegrated(const ulong& v1,const ulong& v2)");
      ASSERT(v1 < v2);
      ASSERT(!_constrIntegrated.Equivalent(v1,v2));
      ASSERT(!_constrPassed.Equivalent(v1,v2));
      ASSERT(!_constrOld.Equivalent(v1,v2));
      _constrIntegrated.MakeEquivalent(v1,v2);
    };    

#ifdef DEBUG_NAMESPACE
  void CheckConstrIntegrity()
    { 
      CALL("CheckConstrIntegrity()");
      ASSERT(_constrToInsert.Implies(_constrIntegrated));      
      ASSERT(_constrPassed.Implies(_constrIntegrated));   
      ASSERT(_constrToShift.Implies(_constrOld));        
      ASSERT(_constrPassed.Implies(_constrOld));
      static EqualityConstraint debugER("debugER");  
      debugER.MakeComposition(_constrToShift,_constrPassed);
      ASSERT(_constrOld.Implies(debugER));
      debugER.MakeComposition(_constrToInsert,_constrPassed);
      ASSERT(_constrIntegrated.Implies(debugER));
    }; //void CheckConstrIntegrity() 
#endif 



  void PassCompNF(Instruction* instr)
    {
      // Trying to pass a Compare having no fork
      CALL("PassCompNF(Instruction* instr)");
      DOP(CheckConstrIntegrity());
      ASSERT(instr->isCompare());
      const Flatterm** var1 = instr->var1();
      const Flatterm** var2 = instr->var2();
      const ulong var_num1 = VarNum(var1);
      const ulong var_num2 = VarNum(var2);

      ASSERT(!(instr->fork()));
      ASSERT(!_constrToShift.Equivalent(var_num1,var_num2));
      ASSERT(!_constrOld.Equivalent(var_num1,var_num2));
      ASSERT(!_constrPassed.Equivalent(var_num1,var_num2));
   
      bool shared = _constrIntegrated.Equivalent(var_num1,var_num2);
    
      if (shared)
	{
	  // the current instruction is shared
	  _constrPassed.MakeEquivalent(var_num1,var_num2);
	  DOP(debugPosInstruction(instr));
	  _nodeAddr = instr->nextAddr();
	}
      else // ! _constrIntegrated.Equivalent(var_num1,var_num2)
	{
	  // the instruction itself must be shifted unless it can be replaced by an equivalent one 
	  *_nodeAddr = instr->next();
	  delete instr;
	};

      instr = *_nodeAddr;
      EqualityConstraint::MergedPairs mergedPairs(_constrOld,var_num1,var_num2);
      ulong merged1;
      ulong merged2;

      while (mergedPairs.nextPair(merged1,merged2))
	{
	  ASSERT(merged1 < merged2);     
	  ASSERT(!_constrOld.Equivalent(merged1,merged2));
	  if (_constrIntegrated.Equivalent(merged1,merged2) && (!_constrPassed.Equivalent(merged1,merged2)))
	    {
	      // another Compare can be shared	 
	      shared = true;
	      _constrPassed.MakeEquivalent(merged1,merged2);
	      *_nodeAddr = Instruction::newCompare(Var(merged1),Var(merged2));
	      (*_nodeAddr)->setNext(instr);
	      DOP(debugPosInstruction(*_nodeAddr));
	      MoveDown();
	    };
	};    
  
      _constrOld.MakeEquivalent(var_num1,var_num2);
      if (!shared) MakeShifted(var_num1,var_num2);
      ASSERT((*_nodeAddr) == instr);
      DOP(CheckConstrIntegrity());
    }; // void PassCompNF(Instruction* instr) 

  bool PassComp(Instruction* instr)
    {
      // Trying to pass a Compare instruction that might have fork
      CALL("PassComp(Instruction* instr)");
      DOP(CheckConstrIntegrity()); 
      ASSERT(instr->isCompare());
      const Flatterm** var1 = instr->var1();
      const Flatterm** var2 = instr->var2();
      ulong var_num1 = VarNum(var1);
      ulong var_num2 = VarNum(var2);
      ASSERT(!_constrToShift.Equivalent(var_num1,var_num2));
      ASSERT(!_constrOld.Equivalent(var_num1,var_num2)); 
      ASSERT(!_constrPassed.Equivalent(var_num1,var_num2));

      ASSERT((*_nodeAddr) == instr);

      if (_constrIntegrated.Equivalent(var_num1,var_num2))
	{
	  // this instruction can be shared
	  goto shift_and_insert_shared;      
	}
      else // try to replace this Compare by an equivalent one which is also in _constrIntegrated
	{
	  EqualityConstraint::MergedPairs mergedPairs(_constrOld,var_num1,var_num2);
	  ulong merged1;
	  ulong merged2; 
	  while (mergedPairs.nextPair(merged1,merged2))
	    {
	      ASSERT(merged1 < merged2);     
	      ASSERT(!_constrOld.Equivalent(merged1,merged2));
	      if (_constrIntegrated.Equivalent(merged1,merged2) && (!_constrPassed.Equivalent(merged1,merged2)))
		{
		  // appropriate replacement found
		  var_num1 = merged1;
		  var_num2 = merged2;
		  // modify the arguments of instr
		  instr->setVar1(Var(var_num1));
		  instr->setVar2(Var(var_num2));        
		  goto shift_and_insert_shared;
		};
	    };
      
	  // no appropriate replacement found, must follow the fork

	  DOP(debugNegInstruction(instr));
	  if (!_constrToShift.Identity())
	    {
	      _constrPassedInShift = _constrPassed;
	      _constrPassedInShift.MakeEquivalent(var_num1,var_num2);
	      CopyCompsToShift(instr->nextAddr());
	    };
	  MoveRight();  
	  DOP(CheckConstrIntegrity());
	  return false;
	};

    shift_and_insert_shared:
      COP("shift_and_insert_shared");
      // make necessary shifts into the fork
      if (instr->fork()) 
	{
	  if (!_constrToShift.Identity())
	    {
	      _constrPassedInShift = _constrPassed; 
	      CopyCompsToShift(instr->forkAddr());  
	    };      
	};
      ASSERT(instr == (*_nodeAddr));
      DOP(debugPosInstruction(instr));
      _constrPassed.MakeEquivalent(var_num1,var_num2);
      MoveDown();
      instr = *_nodeAddr; 
      // now find all CompareS that can be shared due to (var_num1 ~ var_num2) added to _constrOld
 
    
      EqualityConstraint::MergedPairs mergedPairs(_constrOld,var_num1,var_num2);
      ulong merged1;
      ulong merged2; 

      while (mergedPairs.nextPair(merged1,merged2))
	{
	  ASSERT(merged1 < merged2);     
	  ASSERT(!_constrOld.Equivalent(merged1,merged2));

	  if (_constrIntegrated.Equivalent(merged1,merged2) && (!_constrPassed.Equivalent(merged1,merged2)))
	    {
	      // another Compare can be shared	
	      _constrPassed.MakeEquivalent(merged1,merged2);
	      *_nodeAddr = Instruction::newCompare(Var(merged1),Var(merged2));
	      (*_nodeAddr)->setNext(instr);
	      DOP(debugPosInstruction(*_nodeAddr));
	      MoveDown();
	    };
	};    

      ASSERT((*_nodeAddr) == instr);

      _constrOld.MakeEquivalent(var_num1,var_num2); 

      COP("end");
      DOP(CheckConstrIntegrity());     
      return true;
    }; // bool PassComp(Instruction* instr)




  // Read mode
 private:
  bool ReadFunc(const Symbol& func);
  bool ReadVar();
  void ReadCompare(const Flatterm** v1,const Flatterm** v2);
  bool ReadCollectedComps();
    
  // Write mode 
 private:
  void EnterWriteMode();
  void WriteFunc(const Symbol& func);
  void WriteVar(const ulong& v);
  void WriteCompare(const Flatterm** v1,const Flatterm** v2);
  void InsertVar(const Flatterm** v);
  void InsertFunc(const Symbol& func);
      
  // Insertion of constraints 
 private:
  void CopyCompsToShift(Instruction** addr);
  void InsertCompsToShift(Instruction** addr);
  bool MoveCompsToInsert(Instruction**& addr); 


 private: 
#ifndef NO_DEBUG_VIS
  ostream& outputComps(ostream& str) 
    {
      _constrIntegrated.Normalize();
      _constrOld.Normalize();
      _constrPassed.Normalize();
      _constrToShift.Normalize();
      _constrToInsert.Normalize();
      return str << _constrIntegrated << '\n'
		 << _constrOld << '\n'
		 << _constrPassed << '\n'
		 << _constrToShift << '\n'
		 << _constrToInsert << '\n';      

    };
#endif

 private: // data

  Instruction** _nodeAddr;

  EqualityConstraint _constrToInsert;
  EqualityConstraint _constrToShift;
  EqualityConstraint _constrPassed;
   
  EqualityConstraint _constrPassedInShift;
  EqualityConstraint _constrIntegrated;
  EqualityConstraint _constrOld;   

  const Flatterm** _matchingIndexSubstitution;
  
  const Flatterm** _firstVariableOccurence[MaxNumOfVariables];
  MultisetOfVariables<MaxNumOfVariables> _registeredVariables; 
  const Flatterm** _currentMatchingIndexVar;
  bool _readModeFlag;
   
#ifdef DEBUG_NAMESPACE
  static const ulong _debugMaxNumOfVariables = MaxNumOfVariables;
  Stack<Instruction*,10*(MaxTermSize + _debugMaxNumOfVariables + 1)> _debugCodeSequence;
  Stack<bool,10*(MaxTermSize + _debugMaxNumOfVariables + 1)> _debugCommandEval;
#endif

}; // class ForwardMatchingIndexTreeIntegrator


}; // namespace Gematogen

//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_MATCHING_INDEX_TREE
#define DEBUG_NAMESPACE "ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>"
#endif
#include "debugMacros.h"
//=================================================
namespace Gematogen
{

template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
inline void ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::PushVar(const Symbol& sym)
{  
  CALL("PushVar(const Symbol& sym)");
  ulong var = sym.var();
  ASSERT(var < MaxNumOfVariables); 

  if (_readModeFlag)
    {
      _readModeFlag = ReadVar();
      ASSERT(debugInSubst(_currentMatchingIndexVar));
      if (_readModeFlag)
	{
	  if (_registeredVariables.Occurences(var)) { ReadCompare(_firstVariableOccurence[var],_currentMatchingIndexVar); }
	  else // first occurence of the variable
	    {
	      _registeredVariables.Register(var);
	      _firstVariableOccurence[var] = _currentMatchingIndexVar;
	    };        
	} 
      else // variable must be inserted 
	{
	  InsertVar(_currentMatchingIndexVar);
	  if (_registeredVariables.Occurences(var))
	    {
	      EnterWriteMode();
	      WriteCompare(_firstVariableOccurence[var],_currentMatchingIndexVar);
	    } 
	  else // first occurence of the variable
	    {
	      _registeredVariables.Register(var);
	      _firstVariableOccurence[var] = _currentMatchingIndexVar;
	      EnterWriteMode();
	    };     
	};
      _currentMatchingIndexVar++;
    }
  else // write mode
    WriteVar(var);          
}; // void ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::PushVar(const Symbol& sym)

template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
inline void ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::PushFunc(const Symbol& sym)
{
  CALL("PushFunc(const Symbol& sym)");
  if (_readModeFlag)
    {
      _readModeFlag = ReadFunc(sym);
      if (!_readModeFlag) 
	{
	  InsertFunc(sym);
	  EnterWriteMode(); 
	}; 
    }
  else // write mode
    {
      WriteFunc(sym);
    };   
}; // void ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::PushFunc(const Symbol& sym)

}; // namespace Gematogen

//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_MATCHING_INDEX_TREE
#define DEBUG_NAMESPACE "ForwardMatchingIndexTreeRemoval<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>"
#endif
#include "debugMacros.h"
//=================================================
namespace Gematogen
{
template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
class ForwardMatchingIndexTreeRemoval // objects must be static - too costly constructor
{  
 public: 
  typedef ForwardMatchingIndexTreeInstruction<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId> Instruction;
  typedef ForwardMatchingIndexTreeLeaf<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId> LeafInstr; 
 public:
  ForwardMatchingIndexTreeRemoval(const Flatterm** subst)
    : _constr(),
    _constrPassed(),
    _matchingIndexSubstitution(subst),
    _registeredVariables()   
    {};
  ~ForwardMatchingIndexTreeRemoval() {};
  void Reset(Instruction** n)
    {
      CALL("Reset(Instruction** n)");
      // here *n must be nonnull!
      ASSERT(*n);
      _nodeAddr = n;
      _branchToDestroy = n;
      _constr.Reset();
      _constrPassed.Reset();
      _registeredVariables.Reset();
      _currentMatchingIndexVar = _matchingIndexSubstitution; 
    };
  bool Skip(const Symbol& sym)
    {
      if (sym.isVariable()) { return SkipVar(sym.var()); }
      else return SkipFunc(sym);
    };    
   
  bool SkipFunc(const Symbol& func);
  bool SkipVar(const Symbol& var) { return SkipVar(var.var()); };
  bool SkipComps(); 
  LeafInstr* Leaf() const 
    {
      CALL("Leaf()");
      ASSERT((*_nodeAddr)->isLeaf());
      return (LeafInstr*)(*_nodeAddr); 
    };
  Instruction** BranchToDestroy() const { return _branchToDestroy; }; 
  
 private: 
  typedef EquivalenceRelation<MaxTermSize> EqualityConstraint;

 private:
  void MoveDown() { _nodeAddr = (*_nodeAddr)->nextAddr(); };
  void MoveRight() { _nodeAddr = (*_nodeAddr)->forkAddr(); }; 
  ulong VarNum(const Flatterm** v) const { return v - _matchingIndexSubstitution; };   
  bool SkipVar(ulong var);
  bool SkipVar();

 private:
  Instruction** _nodeAddr;
  Instruction** _branchToDestroy;
  EqualityConstraint _constr; 
  EqualityConstraint _constrPassed;

  const Flatterm** _matchingIndexSubstitution;  
  const Flatterm** _firstVariableOccurence[MaxNumOfVariables];
  MultisetOfVariables<MaxNumOfVariables> _registeredVariables; 
  const Flatterm** _currentMatchingIndexVar;

}; // class ForwardMatchingIndexTreeRemoval<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>
}; // namespace Gematogen


//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_MATCHING_INDEX_TREE
#define DEBUG_NAMESPACE "ForwardMatchingIndexTreeRetrieval<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>"
#endif
#include "debugMacros.h"
//=================================================

namespace Gematogen
{
template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
class ForwardMatchingIndexTreeRetrieval
{   
 public: 
  typedef ForwardMatchingIndexTreeInstruction<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId> Instruction;
  typedef ForwardMatchingIndexTreeLeaf<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId> LeafInstr;  
 public:
  ForwardMatchingIndexTreeRetrieval() 
    : backtrack_nodes("backtrack_nodes"),
    backtrack_cursors("backtrack_cursors")
    {
    };
  ~ForwardMatchingIndexTreeRetrieval() {};
  LeafInstr* Match(const Flatterm* query,Instruction* tree);
  LeafInstr* MatchAgain();
    
 private:
  Stack<Instruction*,MaxTermSize> backtrack_nodes;
  Stack<const Flatterm*,MaxTermSize> backtrack_cursors;
}; // template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> class Retrieval

}; // namespace Gematogen

//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_MATCHING_INDEX_TREE
#define DEBUG_NAMESPACE "Output"
#endif
#include "debugMacros.h"
//=================================================

namespace Gematogen
{
template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
class ForwardMatchingIndexTreeOutput
{  
 public: 
  typedef ForwardMatchingIndexTreeInstruction<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>Instruction;
  typedef ForwardMatchingIndexTreeLeaf<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId> LeafInstr; 
 public:
  ForwardMatchingIndexTreeOutput()
    : vars(0), 
    _leafInfo(&LeafInfoDefault)
    {};
  ~ForwardMatchingIndexTreeOutput() {};
  void Init() { vars = 0; _leafInfo = &LeafInfoDefault; };
 private:
  static void LeafInfoDefault(Instruction*); 
  ostream& Var(ostream& str,const Flatterm** v) const { return str << v - vars; };    
 public:
  void SetSubst(const Flatterm* const * subst) { vars = subst; };
  void SetLeafInfo(void (*f)(Instruction*)) { _leafInfo = f; };
#ifndef NO_DEBUG_VIS             
  ostream& instruction(ostream& str,const Instruction* node) const
    {
      return node->output(str,vars);
    };
  ostream& Tree(ostream& str,const Instruction* tree) const;    
#endif
 private: 
  const Flatterm* const * vars;
  void (*_leafInfo)(Instruction*);
}; // class Output

}; // namespace Gematogen
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_MATCHING_INDEX_TREE
#define DEBUG_NAMESPACE "ForwardMatchingIndexTree<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>"
#endif
#include "debugMacros.h"
//=================================================
namespace Gematogen
{
template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
void ForwardMatchingIndexTree<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::Destroy(Instruction* tree)
{
  CALL("Destroy(Instruction* tree)");
  if (!tree) return;
  if (!tree->isLeaf())
    {
      COP("fork");
      Destroy(tree->fork());
      COP("next");
      Destroy(tree->next());
    }; 
  delete tree;    
}; // void ForwardMatchingIndexTree<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::Destroy(Instruction* tree)

template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
void ForwardMatchingIndexTree<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::DestroyBranch(Instruction** branch)
{
  CALL("DestroyBranch(Instruction** branch)");  
  Instruction* _fork;
  
  if ((*branch)->isLeaf()) { _fork = (Instruction*)0; }
  else 
    {
      _fork = (*branch)->fork();
      (*branch)->setFork((Instruction*)0);
    };
  Destroy(*branch);
  *branch = _fork;
}; // void ForwardMatchingIndexTree<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::DestroyBranch(Instruction** branch)
}; // namespace Gematogen

//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_MATCHING_INDEX_TREE
#define DEBUG_NAMESPACE "ForwardMatchingIndexTreeRemoval<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>"
#endif
#include "debugMacros.h"
//=================================================
namespace Gematogen
{
template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
inline bool ForwardMatchingIndexTreeRemoval<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::SkipVar(ulong var)
{
  if (_registeredVariables.Occurences(var))
    {
      _constr.MakeEquivalent(VarNum(_firstVariableOccurence[var]),VarNum(_currentMatchingIndexVar)); 
    }
  else // first occurence
    {
      _registeredVariables.Register(var);
      _firstVariableOccurence[var] = _currentMatchingIndexVar;
    };

  _currentMatchingIndexVar++;

  return SkipVar();    
}; // bool ForwardMatchingIndexTreeRemoval<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::SkipVar(ulong var) 

template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
inline bool ForwardMatchingIndexTreeRemoval<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::SkipVar()
{
  CALL("SkipVar()");
  ASSERT(*_nodeAddr);
  const Flatterm** var1;
  const Flatterm** var2;
  ulong var_num1;
  ulong var_num2;
  Instruction* curr_node;  
 check_node:
  curr_node = *_nodeAddr;
  switch (curr_node->tag())
    {
    case Instruction::Func: return false; // no functor can preceed a variable
     
    case Instruction::Var:    
      if (curr_node->fork()) _branchToDestroy = _nodeAddr;
      MoveDown();
      return true;     

    case Instruction::Compare:
      curr_node->bothVars(var1,var2);     
      var_num1 = VarNum(var1);
      var_num2 = VarNum(var2);
      if (_constr.Equivalent(var_num1,var_num2))
	{
	  ASSERT(!curr_node->fork());
	  _constrPassed.MakeEquivalent(var_num1,var_num2);
	  MoveDown();
	  goto check_node;
	}       
      else return false;
     
#ifdef DEBUG_NAMESPACE
    default: ERROR("X0"); return false;
#else 
#ifdef _SUPPRESS_WARNINGS_
    default: return false;
#endif
#endif 
    };
}; // bool ForwardMatchingIndexTreeRemoval<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::SkipVar() 

template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
inline bool ForwardMatchingIndexTreeRemoval<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::SkipFunc(const Symbol& func)
{
  CALL("SkipFunc(const Symbol& func)");
  ASSERT(*_nodeAddr);
  const Flatterm** var1;
  const Flatterm** var2;
  ulong var_num1;
  ulong var_num2;
  Instruction* curr_node;  
 check_node:
  curr_node = *_nodeAddr;
  switch (curr_node->tag())
    {
    case Instruction::Func:
      if (curr_node->func() == func) 
	{
	  if (curr_node->fork()) _branchToDestroy = _nodeAddr;
	  MoveDown();
	  return true;
	};
    try_fork:
      if (curr_node->fork())
        {
	  MoveRight();
	  _branchToDestroy = _nodeAddr;
	  goto check_node;
        }        
      else return false; 

    case Instruction::Compare:
      curr_node->bothVars(var1,var2);     
      var_num1 = VarNum(var1);
      var_num2 = VarNum(var2);
      if (_constr.Equivalent(var_num1,var_num2))
	{
	  ASSERT(!curr_node->fork());
	  _constrPassed.MakeEquivalent(var_num1,var_num2);
	  MoveDown();
	  goto check_node;
	}       
      else return false;  

    case Instruction::Var: goto try_fork;

#ifdef DEBUG_NAMESPACE
    default: ERROR("X0"); return false;
#else 
#ifdef _SUPPRESS_WARNINGS_
    default: return false;
#endif
#endif        
    };  
}; // bool ForwardMatchingIndexTreeRemoval<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::SkipFunc(const Symbol& func)

template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
inline bool ForwardMatchingIndexTreeRemoval<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::SkipComps()
{
  CALL("SkipComps()");
  ASSERT(*_nodeAddr);
  const Flatterm** var1;
  const Flatterm** var2;
  ulong var_num1;
  ulong var_num2;
  Instruction* curr_node;
 check_tag:
  curr_node = *_nodeAddr;
 
  if (curr_node->isCompare())
    {
      curr_node->bothVars(var1,var2);
      var_num1 = VarNum(var1);
      var_num2 = VarNum(var2);
      if (_constr.Equivalent(var_num1,var_num2))
	{
	  _constrPassed.MakeEquivalent(var_num1,var_num2);
	  if (curr_node->fork()) _branchToDestroy = _nodeAddr;       
	  MoveDown();       
	  goto check_tag;
	};
      if (curr_node->fork())
	{
	  MoveRight();
	  _branchToDestroy = _nodeAddr;
	  goto check_tag;
	};
      return false;
    };
  // !curr_node->isCompare()
  _constr.Subtract(_constrPassed);  
  return _constr.Identity();  
}; // bool ForwardMatchingIndexTreeRemoval<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::SkipComps()

}; // namespace Gematogen


//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_MATCHING_INDEX_TREE
#define DEBUG_NAMESPACE "ForwardMatchingIndexTreeOutput<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>"
#endif
#include "debugMacros.h"
//=================================================
namespace Gematogen
{

#ifndef NO_DEBUG_VIS 
template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
ostream& ForwardMatchingIndexTreeOutput<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::Tree(ostream& str,const Instruction* tree) const
{
  Stack<const Instruction*,MaxTermSize> forks("forks");    
 next_node:
  instruction(str,tree) << '\n';
  if (tree->fork()) forks.push(tree);
  tree = tree->next();
  if (tree) goto next_node;
  if (forks.empty()) return str;
  tree = forks.pop();
  str << "fork from [" << (ulong)tree << "]\n";
  tree = tree->fork();
  goto next_node;   
}; // ostream& ForwardMatchingIndexTreeOutput<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::Tree(ostream& str,const Instruction* tree) const
#endif
}; // namespace Gematogen


//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_MATCHING_INDEX_TREE
#define DEBUG_NAMESPACE "ForwardMatchingIndexTreeInstruction<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>" 
#endif
#include "debugMacros.h"
//=================================================
namespace Gematogen
{

#ifndef NO_DEBUG_VIS 
template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
ostream& ForwardMatchingIndexTreeInstruction<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::output(ostream& str,const Flatterm* const * subst) const
{
  str << '[' << (ulong)this << "] ";
  switch (tag())
    { 
    case Var: str << "Var " << var() - subst; break;
    case Func: str << "Func " << func(); break; 
    case Compare: str << "Compare " << var1() - subst << ' ' << var2() - subst; break;
    case Leaf: str << "Leaf "; break;
    };
  str << " n[" << (ulong)next() << "] "
      << " f[" << (ulong)fork() << ']'; 
  return str;
}; // ostream& ForwardMatchingIndexTreeInstruction<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::output(const Flatterm* const * subst) const
#endif

}; // namespace Gematogen


//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_MATCHING_INDEX_TREE
#define DEBUG_NAMESPACE "ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>"
#endif
#include "debugMacros.h"
//=================================================
namespace Gematogen
{
template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
inline bool ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::ReadFunc(const Symbol& sym)
{ 
  CALL("ReadFunc(const Symbol& sym)")
    Instruction* curr_node;  
 check_tag:   
  curr_node = *_nodeAddr; 
  ASSERT(curr_node);
  switch (curr_node->tag())
    {
    case Instruction::Func:  
      if (curr_node->func() == sym)
	{
	  DOP(CheckConstrIntegrity());
	  DOP(debugPosInstruction(curr_node));    
	  if (curr_node->fork()) 
	    {
	      if (!_constrToShift.Identity())  
		{       
		  _constrPassedInShift = _constrPassed; 
		  CopyCompsToShift(curr_node->forkAddr());
		};
	    };
	  MoveDown();
	  return true;
	};
      // different functor
      if (curr_node->func() > sym)
	{
	  if (!_constrToShift.Identity())  
	    {
	      _constrPassedInShift = _constrPassed;
	      InsertCompsToShift(_nodeAddr);
	    };
	  return false;
	};
      // curr_node->func() < sym 
      // no break here! go to the next case

    case Instruction::Var:
      // mismatch occured, try fork
      DOP(debugNegInstruction(curr_node));
      DOP(CheckConstrIntegrity());
      if (curr_node->fork())      
	{
	  if (!_constrToShift.Identity())
	    {
	      _constrPassedInShift = _constrPassed;  
	      CopyCompsToShift(curr_node->nextAddr());
	    };
	  MoveRight();
	  goto check_tag;
	};
      // no more forks

      if (!_constrToShift.Identity())
	{
	  _constrPassedInShift = _constrPassed; 
	  InsertCompsToShift(curr_node->nextAddr());
	};
      MoveRight();
      return false; 

    case Instruction::Compare:
      PassCompNF(curr_node);
      goto check_tag;

#ifdef DEBUG_NAMESPACE
    default: ERROR("X0"); return false;
#else 
#ifdef _SUPPRESS_WARNINGS_
    default: return false;
#endif
#endif
    };
}; // bool ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::ReadFunc(const Symbol& sym)

template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
inline bool ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::ReadVar()
{
  CALL("ReadVar()");
  Instruction* curr_node;
 check_tag:
  curr_node = *_nodeAddr;        
  switch (curr_node->tag())
    {
    case Instruction::Func:
      // no functor can preceed variable in a tree

      if (!_constrToShift.Identity())
	{
	  _constrPassedInShift = _constrPassed;
	  InsertCompsToShift(_nodeAddr);
	};   
      return false; 

    case Instruction::Var:
      DOP(CheckConstrIntegrity());
      DOP(debugPosInstruction(curr_node));
      if (curr_node->fork()) 
	{
	  if (!_constrToShift.Identity())
	    {
	      _constrPassedInShift = _constrPassed; 
	      CopyCompsToShift(curr_node->forkAddr()); 
	    };
	};              
      MoveDown();
      return true;

    case Instruction::Compare:
      PassCompNF(curr_node);
      goto check_tag;

#ifdef DEBUG_NAMESPACE
    default: ERROR("X0"); return false;
#else 
#ifdef _SUPPRESS_WARNINGS_
    default: return false;
#endif
#endif
    };  
}; // bool ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::ReadVar() 

template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
inline void ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::ReadCompare(const Flatterm** v1,const Flatterm** v2)
{
  CALL("ReadCompare(const Flatterm** v1,const Flatterm** v2)");
  ASSERT(v1<v2);  
  ASSERT(debugInSubst(v1));
  ASSERT(debugInSubst(v2));
  ulong var_num1 = VarNum(v1);
  ulong var_num2 = VarNum(v2);
  MakeIntegrated(var_num1,var_num2);
  _constrToInsert.MakeEquivalent(var_num1,var_num2);  
  DOP(CheckConstrIntegrity());
}; // void ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::ReadCompare(const Flatterm** v1,const Flatterm** v2) 


template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
inline bool ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::ReadCollectedComps()
{
  CALL("ReadCollectedComps()");
  ASSERT(_readModeFlag);
  Instruction* curr_node;
 check_node:
  curr_node = *_nodeAddr;
  if (curr_node->isCompare())
    {
      if (PassComp(curr_node) || (*_nodeAddr)) goto check_node;
      DOP(CheckConstrIntegrity());
      MoveCompsToInsert(_nodeAddr);
      _constrToShift.Reset();
      ASSERT(!(*_nodeAddr));
      return false;
    };
   
  // !curr_node->isCompare(), must stop here

  if (_constrToShift.Identity())
    {
      Instruction** _nodeAddrBackup = _nodeAddr;
      *_nodeAddr = (Instruction*)0;
      DOP(CheckConstrIntegrity());
      if (MoveCompsToInsert(_nodeAddr))
	{
	  ASSERT(!(*_nodeAddr));
	  ASSERT((*_nodeAddrBackup)->isCompare());
	  (*_nodeAddrBackup)->setFork(curr_node);
	  return false;
	}
      else // !MoveCompsToInsert(_nodeAddr) (did not insert anything)
	{
	  ASSERT(_nodeAddr == _nodeAddrBackup);
	  ASSERT(!(*_nodeAddr));
	  *_nodeAddr = curr_node;          
	  return true; 
	};      
    }
  else // !_constrToShift.Identity(), simple case
    { 
      _constrPassedInShift = _constrPassed;
      InsertCompsToShift(_nodeAddr);
      ASSERT((*_nodeAddr)->isCompare());       
      ASSERT((!_constrIntegrated.Equivalent(VarNum((*_nodeAddr)->var1()),VarNum((*_nodeAddr)->var2()))));
      MoveRight();
      MoveCompsToInsert(_nodeAddr);
      ASSERT(!(*_nodeAddr));
      return false; 
    };
}; // bool ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::ReadCollectedComps()


template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
inline void ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::InsertVar(const Flatterm** v)
{
  CALL("InsertVar(const Flatterm** v)");
  ASSERT(debugInSubst(v));
  Instruction* tmp = *_nodeAddr;
  *_nodeAddr = Instruction::newVar(v);
  DOP(debugPosInstruction(*_nodeAddr));
  (*_nodeAddr)->setFork(tmp);
  MoveDown(); 
}; // void ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::InsertVar(const Flatterm** v)

template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
inline void ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::InsertFunc(const Symbol& func)
{
  Instruction* tmp = *_nodeAddr;
  *_nodeAddr = Instruction::newFunc(func);
  DOP(debugPosInstruction(*_nodeAddr));
  (*_nodeAddr)->setFork(tmp);
  MoveDown();
}; // void ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::InsertFunc(const Symbol& func)  

template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
inline void ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::EnterWriteMode()
{
  CALL("EnterWriteMode()");
  // must be used if ReadVar or ReadFunc failed
  ASSERT(_constrToShift.Identity());

  ASSERT(_constrToInsert.Implies(_constrIntegrated));
  ASSERT(_constrPassed.Implies(_constrIntegrated));
  ASSERT(_constrPassed.Implies(_constrOld));     

  //_constrToInsert.Subtract(_constrPassed);

  MoveCompsToInsert(_nodeAddr);
  ASSERT(!(*_nodeAddr));  
}; // void ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::EnterWriteMode() 

template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
inline void ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::WriteVar(const ulong& v)
{
  CALL("WriteVar(const ulong& v)");
  ASSERT(debugInSubst(_currentMatchingIndexVar));
  *_nodeAddr = Instruction::newVar(_currentMatchingIndexVar);
  DOP(debugPosInstruction(*_nodeAddr));  
  MoveDown();
  if (_registeredVariables.Occurences(v)) { WriteCompare(_firstVariableOccurence[v],_currentMatchingIndexVar); }
  else // first occurence
    {
      _registeredVariables.Register(v);
      _firstVariableOccurence[v] = _currentMatchingIndexVar;
    };   
  _currentMatchingIndexVar++;
}; // void ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::WriteVar(ulong v) 

template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
inline void ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::WriteFunc(const Symbol& func)
{
  *_nodeAddr = Instruction::newFunc(func);
  DOP(debugPosInstruction(*_nodeAddr));
  MoveDown();
}; // void ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::WriteFunc(const Symbol& func) 


template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
inline void ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::WriteCompare(const Flatterm** v1,const Flatterm** v2)
{
  CALL("WriteCompare(const Flatterm** v1,const Flatterm** v2)");
  ASSERT(debugInSubst(v1));
  ASSERT(debugInSubst(v2));
  *_nodeAddr = Instruction::newCompare(v1,v2); 
  DOP(debugPosInstruction(*_nodeAddr));
  MoveDown();
}; // void ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::WriteCompare(const Flatterm** v1,const Flatterm** v2)

template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
inline void ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::CopyCompsToShift(Instruction** addr)
{
  CALL("CopyCompsToShift(Instruction** addr)");
  DOP(bool debugAtLeastOneShifted = _constrToShift.Identity());  
  ASSERT(_constrPassed.Implies(_constrPassedInShift));
  ulong vn1,vn2;

  EqualityConstraint::Iterator iter(_constrToShift);

  Instruction* old_node = *addr;
  Instruction* new_node; 
  while (iter.Next(vn2,vn1))
    {
      ASSERT(vn1<vn2);
      ASSERT(debugInSubst(Var(vn1)));
      ASSERT(debugInSubst(Var(vn2)));
      ASSERT(!_constrIntegrated.Equivalent(vn1,vn2));
      ASSERT(_constrOld.Equivalent(vn1,vn2)); 

      if (!_constrPassedInShift.Equivalent(vn1,vn2))
	{
	  DOP(debugAtLeastOneShifted = true);
	  _constrPassedInShift.MakeEquivalent(vn1,vn2);  
	  new_node = Instruction::newCompare(Var(vn1),Var(vn2));
	  *addr = new_node;
	  addr = new_node->nextAddr(); 
	};
    };
  *addr = old_node;
  ASSERT(debugAtLeastOneShifted);
}; // void ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::CopyCompsToShift(Instruction** addr)

template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
inline void ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::InsertCompsToShift(Instruction** addr)
{
  CALL("InsertCompsToShift(Instruction** addr)");
  DOP(bool debugAtLeastOneShifted = _constrToShift.Identity());  
  DOP(CheckConstrIntegrity());     
  ASSERT(_constrPassed.Implies(_constrPassedInShift));
  ulong vn1,vn2;
  Instruction* old_node = *addr;
  Instruction* new_node; 
  while (_constrToShift.Split(vn2,vn1))
    {    
      ASSERT(vn1<vn2);
      ASSERT(debugInSubst(Var(vn1)));
      ASSERT(debugInSubst(Var(vn2)));

      ASSERT(!_constrIntegrated.Equivalent(vn1,vn2));
      ASSERT(_constrOld.Equivalent(vn1,vn2));

      if (!_constrPassedInShift.Equivalent(vn1,vn2))
	{
	  DOP(debugAtLeastOneShifted = true);  
	  _constrPassedInShift.MakeEquivalent(vn1,vn2);
	  new_node = Instruction::newCompare(Var(vn1),Var(vn2));
	  *addr = new_node;
	  addr = new_node->nextAddr(); 
	};
    };
  ASSERT(_constrToShift.Identity());
  *addr = old_node;
  ASSERT(debugAtLeastOneShifted);
}; // void ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::InsertCompsToShift(Instruction** addr)


template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
inline bool ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::MoveCompsToInsert(Instruction**& addr)
{
  CALL("MoveCompsToInsert(Instruction**& addr)");
  ulong vn1,vn2;
  Instruction* old_node = *addr;
  Instruction* new_node;
  bool res = false;
  while (_constrToInsert.Split(vn2,vn1))
    {
      ASSERT(vn1<vn2);
      ASSERT(debugInSubst(Var(vn1)));
      ASSERT(debugInSubst(Var(vn2)));
      ASSERT(_constrIntegrated.Equivalent(vn1,vn2));

      if (!_constrPassed.Equivalent(vn1,vn2))
	{
	  ASSERT(!_constrOld.Equivalent(vn1,vn2));
	  _constrPassed.MakeEquivalent(vn1,vn2);
	  new_node = Instruction::newCompare(Var(vn1),Var(vn2));
	  DOP(debugPosInstruction(new_node));
	  *addr = new_node;
	  addr = new_node->nextAddr(); 
	  res = true;   
	};
    };
  ASSERT(_constrToInsert.Identity());
  *addr = old_node;

  ASSERT(_constrToInsert.Identity());
  ASSERT(_constrPassed.Implies(_constrIntegrated));
  ASSERT(_constrIntegrated.Implies(_constrPassed));
  return res;
}; // bool ForwardMatchingIndexTreeIntegrator<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::MoveCompsToInsert(Instruction**& addr)

}; // namespace Gematogen

//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_MATCHING_INDEX_TREE
#define DEBUG_NAMESPACE "ForwardMatchingIndexTreeRetrieval<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>"
#endif
#include "debugMacros.h"
//=================================================

namespace Gematogen
{
template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
inline ForwardMatchingIndexTreeLeaf<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>* ForwardMatchingIndexTreeRetrieval<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::Match(const Flatterm* query,Instruction* tree)
{
  CALL("Match(const Flatterm* query,Instruction* tree)");
  if (!tree) return (LeafInstr*)0;

  backtrack_nodes.reset();  
  backtrack_cursors.reset();
  
  const Flatterm* cursor = query;
  Instruction* curr_node = tree;
  Symbol curr_sym;

  COP("before check_node");

 check_node:
  COP("check_node");
  ASSERT(curr_node); 
  switch (curr_node->tag()) 
    {
    case Instruction::Func:
      COP("Func");
      curr_sym = cursor->symbolRef();                 
      if (curr_sym.isVariable()) goto backtrack; 
   
    compare_func:
      COP("compare_func");
      if (curr_sym == curr_node->func())
	{
	  cursor = cursor->next();
	  curr_node = curr_node->next();
	  goto check_node;  
        };      
      // different symbols
      if (curr_sym < curr_node->func()) goto backtrack;
      // here curr_sym > curr_node->func()     
      curr_node = curr_node->fork();
      if (!curr_node) goto backtrack;
    func_check_tag:
      COP("func_check_tag");
      if (curr_node->isFunc()) goto compare_func;
      // here curr_node must point to a Compare without fork
      ASSERT(curr_node->isCompare());
      ASSERT(!curr_node->fork());  
      if ((*(curr_node->var1()))->equal(*(curr_node->var2())))
	{
          curr_node = curr_node->next();
          goto func_check_tag;
	}
      else // Compare failed
	goto backtrack;
                
    case Instruction::Var:
      COP("VAR");
      *(curr_node->var()) = cursor;        
      if (curr_node->fork() && cursor->symbolRef().isNotVariable())
	{
	  backtrack_nodes.push(curr_node->fork());
	  backtrack_cursors.push(cursor);
	};
      cursor = cursor->after();
      curr_node = curr_node->next();
      goto check_node;     

    case Instruction::Compare:
      COP("Compare");
      if ((*(curr_node->var1()))->equal(*(curr_node->var2())))
	{
	  if (curr_node->fork())
	    {
	      backtrack_nodes.push(curr_node->fork());
	      backtrack_cursors.push(cursor);
	    };
	  curr_node = curr_node->next();
	  goto check_node;
	}
      else // Compare failed, try fork
	{
	  curr_node = curr_node->fork();
	  if (curr_node) goto check_node;
	  goto backtrack;
	};

    case Instruction::Leaf:
      COP("Leaf"); 
      return (LeafInstr*)curr_node; // success

#ifdef DEBUG_NAMESPACE
    default: COP("default1"); ERROR("X0"); return (LeafInstr*)0;
#else
#ifdef _SUPPRESS_WARNINGS_
    default: COP("default2"); return (LeafInstr*)0;  
#endif
#endif
    };
 
  //*************** Backtrack section: **********
  COP("before bakctrack");
 backtrack:
  COP("backtrack");
  GlobalStopFlag::check();
  if (backtrack_nodes.empty())
    {   
      // failure
      return (LeafInstr*)0;
    };
  curr_node = backtrack_nodes.pop(); 
  cursor = backtrack_cursors.pop();
  goto check_node;
}; // LeafInstr* ForwardMatchingIndexTreeRetrieval<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::Match(const Flatterm* query,Instruction* tree) 

template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
inline ForwardMatchingIndexTreeLeaf<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>* ForwardMatchingIndexTreeRetrieval<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::MatchAgain()
{
  CALL("MatchAgain()");
  const Flatterm* cursor;
  Instruction* curr_node;
  Symbol curr_sym;

  goto backtrack;

 check_node:
  COP("check_node");
  ASSERT(curr_node); 
  switch (curr_node->tag()) 
    {
    case Instruction::Func:
      COP("FUNC");
      curr_sym = cursor->symbolRef();                 
      if (curr_sym.isVariable()) goto backtrack; 
   
    compare_func:
      COP("compare_func");
      if (curr_sym == curr_node->func())
	{
	  cursor = cursor->next();
	  curr_node = curr_node->next();
	  goto check_node;  
        };      
      // different symbols
      if (curr_sym < curr_node->func()) goto backtrack;
      // here curr_sym > curr_node->func()     
      curr_node = curr_node->fork();
      if (!curr_node) goto backtrack;
    func_check_tag:
      COP("func_check_tag");
      if (curr_node->isFunc()) goto compare_func;
      // here curr_node must point to a Compare without fork
      ASSERT(curr_node->isCompare());
      ASSERT(!curr_node->fork());  
      if ((*(curr_node->var1()))->equal(*(curr_node->var2())))
	{
          curr_node = curr_node->next();
          goto func_check_tag;
	}
      else // Compare failed
	goto backtrack;
                
    case Instruction::Var:
      COP("VAR");
      *(curr_node->var()) = cursor;        
      if (curr_node->fork() && cursor->symbolRef().isNotVariable())
	{
	  backtrack_nodes.push(curr_node->fork());
	  backtrack_cursors.push(cursor);
	};
      cursor = cursor->after();
      curr_node = curr_node->next();
      goto check_node;     

    case Instruction::Compare:
      COP("Compare");
      if ((*(curr_node->var1()))->equal(*(curr_node->var2())))
	{
	  if (curr_node->fork())
	    {
	      backtrack_nodes.push(curr_node->fork());
	      backtrack_cursors.push(cursor);
	    };
	  curr_node = curr_node->next();
	  goto check_node;
	}
      else // Compare failed, try fork
	{
	  curr_node = curr_node->fork();
	  if (curr_node) goto check_node;
	  goto backtrack;
	};

    case Instruction::Leaf: 
      COP("Leaf"); 
      return  (LeafInstr*)curr_node; // success

#ifdef DEBUG_NAMESPACE
    default: COP("default1"); ERROR("X0"); return (LeafInstr*)0;
#else
#ifdef _SUPPRESS_WARNINGS_
    default: COP("default2"); return (LeafInstr*)0;  
#endif
#endif
    };
 
  //*************** Backtrack section: **********
  COP("before bakctrack");
 backtrack:
  COP("backtrack");
  if (backtrack_nodes.empty())
    {
      // failure
      return (LeafInstr*)0;
    };
  curr_node = backtrack_nodes.pop(); 
  cursor = backtrack_cursors.pop();
  goto check_node;
}; // LeafInstr* ForwardMatchingIndexTreeRetrieval<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::MatchAgain()

}; // namespace Gematogen

//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_MATCHING_INDEX_TREE
#define DEBUG_NAMESPACE "ForwardMatchingIndexTreeOutput<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>"
#endif
#include "debugMacros.h"
//=================================================

namespace Gematogen
{
template <ulong MaxTermSize,ulong MaxNumOfVariables,class Alloc,class Symbol,class Flatterm,class LeafInfo,class InstanceId> 
void ForwardMatchingIndexTreeOutput<MaxTermSize,MaxNumOfVariables,Alloc,Symbol,Flatterm,LeafInfo,InstanceId>::LeafInfoDefault(Instruction*) {}; 

}; // namespace Gematogen

//================================================
#undef DEBUG_NAMESPACE
//=============================================================
#endif

