//
// File:         OptimisedPathIndex.h
// Description:  Tries for optimised path indexing.
// Created:      Sep 12, 2001
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
// Note:         This file is a part of the Gematogen library.
//============================================================================
#ifndef OPTIMISED_PATH_INDEX_H
//=============================================================================
#define OPTIMISED_PATH_INDEX_H 
#include "jargon.h"
#include "GematogenDebugFlags.h" 
#include "Stack.h"
#ifdef DEBUG_ALLOC_OBJ_TYPE 
 #include "ClassDesc.h"
#endif
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_OPTIMISED_PATH_INDEX
 #define DEBUG_NAMESPACE "OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>"
#endif
#include "debugMacros.h"
//============================================================================

namespace Gematogen
{
  using namespace BK;
template <class Alloc,class Symbol,class FuncIndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
// FuncIndObj and ConstIndObj must have operator bool() const
class OptimisedPathIndex
{
 public: 
  class Integrator;
  class Retrieval;
  class Removal;
  class Destruction;
 private:
  class Node
  {
   public:
    Node(const Symbol& f) : _func(f), _altern((Node*)0) // for nonconst. functors only
    {
     CALL("Node::(constructor Node(const Symbol& f))");
     ASSERT(_func.isComplex());
     ulong arity = f.arity();
     ASSERT(arity);
     Node** deeperPos = deeper();
     for (ulong argn = 0; argn < arity; argn++) deeperPos[argn] = (Node*)0; 
     indObj().init();
    }; 
    Node(const Symbol& c,bool) 
     : _func(c), _altern((Node*)0) // for constants only
    {
     CALL("Node::(constructor Node(const Symbol& c))");
     ASSERT(_func.isComplex());
     ASSERT(!(_func.arity()));
     constIndObj().init();
    };

    ~Node() 
      {
	CALL("Node::destructor ~Node()");
	if (DestructionMode::isThorough())
	  {
	    if (isConstant())
	      {
		constIndObj().destroy();
	      }
	    else
	      indObj().destroy();
	  };
	  
      };
    void* operator new(size_t,ulong arity); // for nonconst. functors only
    void* operator new(size_t); // for constants only
    void operator delete(void* obj);

    static ulong funcMinSize() { return sizeof(FuncIndObj) + sizeof(Node) + sizeof(Node*); };
    static ulong funcMaxSize() { return sizeof(FuncIndObj) + sizeof(Node) + Symbol::maxArity()*sizeof(Node*); };
    static ulong constMinSize() { return sizeof(ConstIndObj) + sizeof(Node); };
    static ulong constMaxSize() { return sizeof(ConstIndObj) + sizeof(Node); };

    const Symbol& func() const { return _func; }; 
    Node*& altern() { return _altern; };
    const Node* const & altern() const { return _altern; };
    Node** alternAddr() { return &_altern; };
    const Node* const * alternAddr() const { return &_altern; };

    FuncIndObj& indObj() // for nonconstants only
    {
     CALL("Node::indObj()");
     ASSERT(!isConstant());
     return *(((FuncIndObj*)this) - 1);
    };
    ConstIndObj& constIndObj() // for constants only
    {
     CALL("Node::constIndObj()");
     ASSERT(isConstant());
     return *(((ConstIndObj*)this) - 1);
    };    
    FuncIndObj& indObj() const // for nonconstants only
    {
     CALL("Node::indObj() const");
     ASSERT(!isConstant());
     return *(((FuncIndObj*)this) - 1);
    };
    ConstIndObj& constIndObj() const// for constants only
    {
     CALL("Node::constIndObj() const");
     ASSERT(isConstant());
     return *(((ConstIndObj*)this) - 1);
    };
    const FuncIndObj* indObjAddr() const // for nonconstants only
    {
     CALL("Node::indObjAddr() const");
     ASSERT(!isConstant());
     return ((FuncIndObj*)this) - 1;
    };
    const ConstIndObj* constIndObjAddr() const // for constants only
    {
     CALL("Node::constIndObjAddr() const");
     ASSERT(isConstant());
     return ((ConstIndObj*)this) - 1;
    };

    Node** deeper() // for nonconst. functors only
    {
     CALL("Node::deeper()");
     ASSERT(isNonconst());
     return (Node**)(this + 1);
    };   
    const Node* const * deeper() const // for nonconst. functors only
    {
     CALL("Node::deeper() const");
     ASSERT(isNonconst());
     return (const Node* const *)(this + 1);
    };
    ulong arity() const { return func().arity(); };
    bool isConstant() const { return !(func().arity()); };
    bool isNonconst() const { return func().arity(); };
   private:
    Symbol _func;
    Node* _altern;
    #ifdef DEBUG_ALLOC_OBJ_TYPE
     static ClassDesc _funcClassDesc;
     static ClassDesc _constClassDesc;
    #endif 
   }; // class Node

 public:
  class Trie
  {
   public:
    Trie() : _content((Node*)0) {};
    ~Trie() {};
    void init() { _content = (Node*)0; }; 
    operator bool() const { return (bool)_content; };
   private:
    Node** contentAddr() { return &_content; };   
   private:
    Node* _content;
   friend class Integrator;
   friend class Retrieval;
   friend class Removal;
   friend class Destruction;
  }; // class Trie

  class Integrator
  {
   public:
    Integrator()
     : _backtrackNodeAddr()
    {
    };
    ~Integrator() {};
    void destroyThoroughly() {};
    void reset(Trie& tree);
    void nonconstFunc(const Symbol& f);
    void constant(const Symbol& c); 
    void variable();
    FuncIndObj& indexedObj() const { return _lastNode->indObj(); };
    ConstIndObj& constIndObj() const { return _lastNode->constIndObj(); };

   private:
    void backtrack();

   private:
    Node** _nodeAddr;
    Node* _lastNode;
    Stack<Node**,MaxTermSize> _backtrackNodeAddr;
  }; // class Integrator


 class Retrieval
 {
  public:
   Retrieval() : _backtrackNodeAddr() {};
   ~Retrieval() {};
   void destroyThoroughly() {};
   
   bool reset(Trie& tree);
   
   bool nonconstFunc(const Symbol& f);
   bool constant(const Symbol& c);
   void after() { CALL("Retrieval::after()"); ASSERT(!debugEndOfTerm); backtrack(); };

   const FuncIndObj& indObj() const { return _lastNode->indObj(); };
   const ConstIndObj& constIndObj() const { return _lastNode->constIndObj(); };  
   const FuncIndObj* indObjAddr() const { return _lastNode->indObjAddr(); };
   const ConstIndObj* constIndObjAddr() const { return _lastNode->constIndObjAddr(); };  

  private:
   void backtrack();

  private:
   Node** _nodeAddr;
   Node* _lastNode;
   Stack<Node**,MaxTermSize> _backtrackNodeAddr;
   #ifdef DEBUG_NAMESPACE
    bool debugEndOfTerm;  
   #endif
 }; // class Retrieval

 class Removal
 {
  public:
   Removal()
    : _backtrackNodeAddr(),
      _candidatesForRecycling()
   {
   };
 
   ~Removal() {};
   void destroyThoroughly() {};
   void reset(Trie& tree);

   bool nonconstFunc(const Symbol& f);
   bool constant(const Symbol& c);
   void after() { CALL("Removal::after()"); ASSERT(!debugEndOfTerm); backtrack(); }; 

   FuncIndObj& indexedObj() const 
   { 
    CALL("Removal::indexedObj() const");   
    return _lastNode->indObj(); 
   };
   ConstIndObj& constIndObj() const 
   { 
    CALL("Removal::constIndObj() const"); 
    return _lastNode->constIndObj(); 
   };  

   void recycleObsoleteNodes()
   {
    CALL("Removal::recycleObsoleteNodes()"); 
    while (_candidatesForRecycling.nonempty()) recycleObsolete(*(_candidatesForRecycling.pop()));
   };

  private:
   void backtrack();

  private:
   Node** _nodeAddr;
   Node* _lastNode;
   Stack<Node**,MaxTermSize> _backtrackNodeAddr;
   Stack<Node**,MaxTermSize> _candidatesForRecycling;
   #ifdef DEBUG_NAMESPACE
    bool debugEndOfTerm;  
   #endif
 }; // class Removal

 class Destruction
 {
  public:
   Destruction() : _backtrackNodeAddr() {};
   ~Destruction() {};
   void destroyThoroughly() {};
   bool reset(Trie& tree);

   void recycle() { destroyTree(*_tree); };

   bool next(bool& constant);

   FuncIndObj& indexedObj() const { return _lastNode->indObj(); };
   ConstIndObj& constIndObj() const { return _lastNode->constIndObj(); };
  private:
   Node** _tree;
   Node** _nodeAddr;
   Node* _lastNode;
   Stack<Node**,MaxTermSize + MaxTermDepth> _backtrackNodeAddr;
 }; // class Destruction


 private: // for class OptimisedPathIndex
  // Releasing memory  
  static void destroyTree(Node*& tree);
  static void recycleObsolete(Node*& node);
 
 friend class Removal;
 friend class Destruction;
}; // template <class Alloc,class Symbol,class FuncIndObj,class ConstIndObj,ulong MaxTermDepth> class OptimisedPathIndex

}; // namespace Gematogen


//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_OPTIMISED_PATH_INDEX
 #define DEBUG_NAMESPACE "OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>"
#endif
#include "debugMacros.h"
//============================================================================

namespace Gematogen
{

template <class Alloc,class Symbol,class FuncIndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::destroyTree(Node*& tree)
{   
 CALL("destroyTree(Node*& tree)");
 if (tree)
  {
   destroyTree(tree->altern());   
   if (tree->isNonconst())
    {             
     for (ulong a = 0; a < tree->arity(); a++) 
      {
       destroyTree(tree->deeper()[a]);
      };
    };
   delete tree;
   tree = (Node*)0;
  };
}; // void OptimisedPathIndex<..>::destroyTree(Node*& tree)


template <class Alloc,class Symbol,class FuncIndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::recycleObsolete(Node*& node)
{
 CALL("recycleObsolete(Node*& node)");
 Node* tmp;
 if (node) 
  {
   if (node->isConstant()) 
    {
     if (!node->constIndObj()) // obsolete node
      {
       tmp = node;       
       node = tmp->altern();
       delete tmp;
      };
    }
   else // !node->isConstant()
    { 
     if (!node->indObj())
      {
       ulong arity = node->arity();
       for (ulong argn = 0; argn < arity; argn++) 
	if (node->deeper()[argn]) return;  // not obsolete
       // obsolete node
       tmp = node;       
       node = tmp->altern();
       delete tmp; 
      };
    };
  };
}; // void OptimisedPathIndex<..>::recycleObsolete(Node*& node) 

}; // namespace Gematogen


//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_OPTIMISED_PATH_INDEX
 #define DEBUG_NAMESPACE "OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Destruction"
#endif
#include "debugMacros.h"
//============================================================================

namespace Gematogen
{

template <class Alloc,class Symbol,class FuncIndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline bool OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Destruction::reset(Trie& tree) 
{ 
 CALL("reset(Trie& tree,bool& constant)");
 _nodeAddr = tree.contentAddr();
 _tree = _nodeAddr;
 return (bool)(*_nodeAddr);
}; // bool OptimisedPathIndex<..>::Destruction::reset(Trie& tree) 

template <class Alloc,class Symbol,class FuncIndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline bool OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Destruction::next(bool& constant)
{
 CALL("next(bool& constant)"); 
 Node** deeper;
 ulong arity;
 _lastNode = *_nodeAddr;
 if (_lastNode)
  {
   process_node:
    constant = _lastNode->isConstant();
    _nodeAddr = _lastNode->alternAddr();
    if (!constant)
     {
      if (*_nodeAddr) _backtrackNodeAddr.push(_nodeAddr);
      arity = _lastNode->arity();
      deeper = _lastNode->deeper();
      for (ulong argn = arity - 1; argn; argn--) 
       {
        _nodeAddr = deeper + argn;
        if (*_nodeAddr) _backtrackNodeAddr.push(_nodeAddr); 
       };
      _nodeAddr = deeper;
     };
    return true;
  }
 else // !_lastNode
  {
   if (_backtrackNodeAddr)
    {
     _nodeAddr = _backtrackNodeAddr.pop();
     _lastNode = *_nodeAddr;
     ASSERT(_lastNode);
     goto process_node;
    }
   else return false;
  };
}; // bool OptimisedPathIndex<..>::Destruction::next(bool& constant) 


}; // namespace Gematogen

//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_OPTIMISED_PATH_INDEX
 #define DEBUG_NAMESPACE "OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Removal"
#endif
#include "debugMacros.h"
//============================================================================

namespace Gematogen
{
template <class Alloc,class Symbol,class FuncIndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Removal::backtrack()
{
 CALL("Removal::backtrack()");

 if (_backtrackNodeAddr) { _nodeAddr = _backtrackNodeAddr.pop(); }
 else 
  {
   DOP(debugEndOfTerm = true);
  };
}; // void backtrack()



template <class Alloc,class Symbol,class FuncIndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Removal::reset(Trie& tree) 
{ 
 _nodeAddr = tree.contentAddr();
 _backtrackNodeAddr.reset();
 _candidatesForRecycling.reset(); 
 DOP(debugEndOfTerm = false);
}; // void OptimisedPathIndex<..>::Removal::reset(Trie& tree)


template <class Alloc,class Symbol,class FuncIndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline bool OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Removal::nonconstFunc(const Symbol& f)
{
 CALL("nonconstFunc(const Symbol& f)");
 ASSERT(!debugEndOfTerm);
 ASSERT(f.arity());
 check_node: 
  if (!(*_nodeAddr)) return false;
  _lastNode = *_nodeAddr;  
  if (_lastNode->func() == f)
   {
    _candidatesForRecycling.push(_nodeAddr);
    _nodeAddr = _lastNode->deeper();
    ulong arity = f.arity();
    for (ulong argn = arity - 1; argn; argn--) _backtrackNodeAddr.push(_lastNode->deeper() + argn); 
    return true; 
   };
  _nodeAddr = _lastNode->alternAddr();
  goto check_node;
}; // bool OptimisedPathIndex<..>::Removal::nonconstFunc(const Symbol& f)


template <class Alloc,class Symbol,class FuncIndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline bool OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Removal::constant(const Symbol& c)
{
 CALL("constant(const Symbol& c)"); 
 ASSERT(!debugEndOfTerm);   
 ASSERT(!c.arity());
 check_node: 
  if (!(*_nodeAddr)) return false;
  _lastNode = *_nodeAddr;  
  if (_lastNode->func() == c)
   {
    _candidatesForRecycling.push(_nodeAddr);
    backtrack();
    return true; 
   };
  _nodeAddr = _lastNode->alternAddr();
  goto check_node;
}; // bool OptimisedPathIndex<..>::Removal::constant(const Symbol& c)  





}; // namespace Gematogen

//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_OPTIMISED_PATH_INDEX
 #define DEBUG_NAMESPACE "OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Retrieval"
#endif
#include "debugMacros.h"
//============================================================================

namespace Gematogen
{

template <class Alloc,class Symbol,class FuncIndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>  
inline bool OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Retrieval::reset(Trie& tree) 
{ 
 _nodeAddr = tree.contentAddr();
 _backtrackNodeAddr.reset();
 DOP(debugEndOfTerm = false);
 return *_nodeAddr;
}; // bool OptimisedPathIndex<..>::Retrieval::reset(Trie& tree)


template <class Alloc,class Symbol,class FuncIndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline bool OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Retrieval::nonconstFunc(const Symbol& f)
{
 CALL("nonconstFunc(const Symbol& f)");
 ASSERT(!debugEndOfTerm);
 ASSERT(f.arity());
 check_node: 
  if (!(*_nodeAddr)) return false;
  _lastNode = *_nodeAddr;  
  if (_lastNode->func() == f)
   {
    _nodeAddr = _lastNode->deeper();
    ulong arity = f.arity();
    for (ulong argn = arity - 1; argn; argn--) _backtrackNodeAddr.push(_lastNode->deeper() + argn); 
    return true; 
   };
  _nodeAddr = _lastNode->alternAddr();
  goto check_node;
}; // bool OptimisedPathIndex<..>::Retrieval::nonconstFunc(const Symbol& f)


template <class Alloc,class Symbol,class FuncIndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline bool OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Retrieval::constant(const Symbol& c)
{
 CALL("constant(const Symbol& c)"); 
 ASSERT(!debugEndOfTerm);   
 ASSERT(!c.arity());
 check_node: 
  if (!(*_nodeAddr)) return false;
  _lastNode = *_nodeAddr;  
  if (_lastNode->func() == c)
   {
    backtrack();
    return true; 
   };
  _nodeAddr = _lastNode->alternAddr();
  goto check_node;

}; // bool OptimisedPathIndex<..>::Retrieval::constant(const Symbol& c)  


template <class Alloc,class Symbol,class FuncIndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Retrieval::backtrack()
{
 CALL("backtrack()");
 if (_backtrackNodeAddr) { _nodeAddr = _backtrackNodeAddr.pop(); }
 else 
  {
   DOP(debugEndOfTerm = true);
  };
}; // void OptimisedPathIndex<..>::Retrieval::backtrack()


}; // namespace Gematogen

//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_OPTIMISED_PATH_INDEX
 #define DEBUG_NAMESPACE "OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Integrator"
#endif
#include "debugMacros.h"
//============================================================================

namespace Gematogen
{


template <class Alloc,class Symbol,class FuncIndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Integrator::reset(Trie& tree) 
{ 
 _nodeAddr = tree.contentAddr();
 _backtrackNodeAddr.reset();
}; // void OptimisedPathIndex<..>::Integrator::reset(Trie& tree) 

template <class Alloc,class Symbol,class FuncIndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize> 
inline void OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Integrator::nonconstFunc(const Symbol& f)
{
 CALL("nonconstFunc(const Symbol& f)");
 ulong arity = f.arity();
 ASSERT(arity);

 check_node:
  if (!(*_nodeAddr)) 
   {
    _lastNode = new (arity) Node(f);
    *_nodeAddr = _lastNode;
    node_found:   
     _nodeAddr = _lastNode->deeper();
     for (ulong argn = arity - 1; argn; argn--) _backtrackNodeAddr.push(_lastNode->deeper() + argn);
     return;
   };   
  _lastNode = *_nodeAddr;
  if (_lastNode->func() == f) goto node_found;
  _nodeAddr = _lastNode->alternAddr();  
  goto check_node;
}; // void OptimisedPathIndex<..>::Integrator::nonconstFunc(const Symbol& f) 

template <class Alloc,class Symbol,class FuncIndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Integrator::constant(const Symbol& c)
{
 CALL("constant(const Symbol& c)");    
 ASSERT(!c.arity());
 check_node:
  if (!(*_nodeAddr)) 
   {
    _lastNode = new Node(c,true);    
    *_nodeAddr = _lastNode;
    node_found: 
     backtrack();
     return;
   };
  _lastNode = *_nodeAddr;
  if (_lastNode->func() == c) goto node_found;  
  _nodeAddr = _lastNode->alternAddr();  
  goto check_node;
}; // void OptimisedPathIndex<..>::Integrator::constant(const Symbol& c)


template <class Alloc,class Symbol,class FuncIndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Integrator::variable()
{
 CALL("variable()");
 backtrack();  
};


template <class Alloc,class Symbol,class FuncIndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Integrator::backtrack()
{
 CALL("backtrack()");
 if (_backtrackNodeAddr) _nodeAddr = _backtrackNodeAddr.pop();
}; // void OptimisedPathIndex<..>::Integrator::backtrack()


}; // namespace Gematogen

//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_OPTIMISED_PATH_INDEX
 #define DEBUG_NAMESPACE "OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Node"
#endif
#include "debugMacros.h"
//============================================================================

namespace Gematogen
{
template <class Alloc,class Symbol,class FuncIndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void* OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Node::operator new(size_t,ulong arity) 
// for nonconst. functors only
{ 
 CALL("operator new(size_t,ulong arity)");
 ASSERT(arity);
 #ifdef DEBUG_ALLOC_OBJ_TYPE
  ALWAYS(_funcClassDesc.registerAllocated(sizeof(FuncIndObj) + sizeof(Node) + arity*sizeof(Node*)));
  return (void*)(((FuncIndObj*)(Alloc::allocate(sizeof(FuncIndObj) + sizeof(Node) + arity*sizeof(Node*),&_funcClassDesc))) + 1); 
 #else
  return (void*)(((FuncIndObj*)(Alloc::allocate(sizeof(FuncIndObj) + sizeof(Node) + arity*sizeof(Node*)))) + 1);
 #endif
}; // void* OptimisedPathIndex<..>::Node::operator new(size_t,ulong arity)


template <class Alloc,class Symbol,class FuncIndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void* OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Node::operator new(size_t) // for constants only 
{ 
 CALL("operator new(size_t)");
 #ifdef DEBUG_ALLOC_OBJ_TYPE
  ALWAYS(_constClassDesc.registerAllocated(sizeof(ConstIndObj) + sizeof(Node)));
  return (void*)(((ConstIndObj*)(Alloc::allocate(sizeof(ConstIndObj) + sizeof(Node),&_constClassDesc))) + 1); 
 #else 
  return (void*)(((ConstIndObj*)(Alloc::allocate(sizeof(ConstIndObj) + sizeof(Node)))) + 1); 
 #endif
}; // void* OptimisedPathIndex<..>::Node::operator new(size_t)


template <class Alloc,class Symbol,class FuncIndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void OptimisedPathIndex<Alloc,Symbol,FuncIndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Node::operator delete(void* obj) 
{
 CALL("operator delete(void* obj)");
 ulong arity = ((Node*)obj)->func().arity();
 if (arity)
  {
   #ifdef DEBUG_ALLOC_OBJ_TYPE
    ALWAYS(_funcClassDesc.registerDeallocated(sizeof(FuncIndObj) + sizeof(Node) + arity*sizeof(Node*)));
    Alloc::deallocate((void*)(((FuncIndObj*)obj) - 1),sizeof(FuncIndObj) + sizeof(Node) + arity*sizeof(Node*),&_funcClassDesc);
   #else
    Alloc::deallocate((void*)(((FuncIndObj*)obj) - 1),sizeof(FuncIndObj) + sizeof(Node) + arity*sizeof(Node*));
   #endif
  }
 else
  {
   #ifdef DEBUG_ALLOC_OBJ_TYPE
    ALWAYS(_constClassDesc.registerDeallocated(sizeof(ConstIndObj) + sizeof(Node)));
    Alloc::deallocate((void*)(((ConstIndObj*)obj) - 1),sizeof(ConstIndObj) + sizeof(Node),&_constClassDesc);
   #else
    Alloc::deallocate((void*)(((ConstIndObj*)obj) - 1),sizeof(ConstIndObj) + sizeof(Node));
   #endif
  };
}; // void OptimisedPathIndex<..>::Node::operator delete(void* obj)   

}; // namespace Gematogen
//======================================================================
#undef DEBUG_NAMESPACE
//======================================================================
#endif
