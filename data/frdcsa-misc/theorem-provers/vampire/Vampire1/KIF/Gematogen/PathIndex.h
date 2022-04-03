//
// File:         PathIndex.h
// Description:  Tries for path indexing.
// Created:      Feb 25, 2000, 15:00
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
// Revised:      Jul 31, 2000
//               New template parameter Symbol is introduced 
//               to avoid using the class TERM directly.
// Note:         This file is a part of the Gematogen library.
// Note:         The code inside the PathIndex class declaration is clumsy. 
//               One of the reasons is an error in gcc 2.91.66.
//============================================================================
#ifndef PATH_INDEX_H
//=============================================================================
#define PATH_INDEX_H 
#include "jargon.h"
#include "GematogenDebugFlags.h" 
#include "Stack.h"
#ifdef DEBUG_ALLOC_OBJ_TYPE 
 #include "ClassDesc.h"
#endif
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_PATH_INDEX
 #define DEBUG_NAMESPACE "PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>"
#endif
#include "debugMacros.h"
//============================================================================

namespace Gematogen
{
  using namespace BK;
template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
// IndObj and ConstIndObj must have operator bool() const
class PathIndex
{
 public:
  class Integrator;
  class Retrieval;
  class Removal;
  class Destruction;
 private:
  class OccNode;
  class FuncNode
  {
   public:
    FuncNode(const Symbol& f) : _func(f), _altern((FuncNode*)0) // for nonconst. functors only
    {
     CALL("FuncNode::(constructor FuncNode(const Symbol& f))");
     ASSERT(_func.isComplex());
     ulong arity = f.arity();
     ASSERT(arity);
     OccNode** deeperPos = deeper();
     for (ulong argn = 0; argn < arity; argn++) deeperPos[argn] = (OccNode*)0;  
    }; 
    FuncNode(const Symbol& c,bool) 
     : _func(c), _altern((FuncNode*)0) // for constants only
    {
     CALL("FuncNode::(constructor FuncNode(const Symbol& c))");
     ASSERT(_func.isComplex());
     ASSERT(!(_func.arity()));
     indObj().init();
    };
    ~FuncNode() {};
    void* operator new(size_t,ulong arity); // for nonconst. functors only
    void* operator new(size_t); // for constants only
    void operator delete(void* obj);

    static ulong funcMinSize() { return sizeof(FuncNode) + sizeof(OccNode*); };
    static ulong funcMaxSize() { return sizeof(FuncNode) + Symbol::maxArity()*sizeof(OccNode*); };
    static ulong constMinSize() { return sizeof(FuncNode) + sizeof(ConstIndObj); };
    static ulong constMaxSize() { return sizeof(FuncNode) + sizeof(ConstIndObj); };

    const Symbol& func() const { return _func; }; 
    FuncNode*& altern() { return _altern; };
    const FuncNode* const & altern() const { return _altern; };
    ConstIndObj& indObj() // for constants only
    {
     CALL("FuncNode::indObj()");
     ASSERT(isConstant());
     return *((ConstIndObj*)(this + 1));
    };
    const ConstIndObj& indObj() const // for constants only
    {
     CALL("FuncNode::indObj() const");
     ASSERT(isConstant());
     return *((const ConstIndObj*)(this + 1));
    };

    ConstIndObj* indObjAddr() // for constants only
    {
     CALL("FuncNode::indObjAddr()");
     ASSERT(isConstant());
     return (ConstIndObj*)(this + 1);
    };
    const ConstIndObj* indObjAddr() const // for constants only
    {
     CALL("FuncNode::indObjAddr() const");
     ASSERT(isConstant());
     return (const ConstIndObj*)(this + 1);
    };

    OccNode** deeper() // for nonconst. functors only
    {
     CALL("FuncNode::deeper()");
     ASSERT(isNonconst());
     return (OccNode**)(this + 1);
    };   
    const OccNode* const * deeper() const // for nonconst. functors only
    {
     CALL("FuncNode::deeper() const");
     ASSERT(isNonconst());
     return (const OccNode* const *)(this + 1);
    };
    ulong arity() const { return func().arity(); };
    bool isConstant() const { return !(func().arity()); };
    bool isNonconst() const { return func().arity(); };
    bool obsolete() const 
    {
     if (isConstant()) return !(indObj());
     for (ulong a = 0; a < arity(); a++) if (deeper()[a]) return false; 
     return true;  
    };   
   private:
    Symbol _func;
    FuncNode* _altern;
    #ifdef DEBUG_ALLOC_OBJ_TYPE
     static ClassDesc _funcClassDesc;
     static ClassDesc _constClassDesc;
    #endif 
   }; // class FuncNode

  class OccNode
  { 
   public:
    OccNode() : _complex((FuncNode*)0) 
    {
     CALL("OccNode::constructor OccNode()");
     indObj().init();
    };
    ~OccNode() {};
    void* operator new(size_t);
    void operator delete(void* obj);
    static ulong minSize() { return sizeof(OccNode); };
    static ulong maxSize() { return sizeof(OccNode); };   
    IndObj& indObj() { return *((IndObj*)_indObj); };    
    const IndObj& indObj() const { return *((IndObj*)_indObj); };
    IndObj* indObjAddr() { return (IndObj*)_indObj; };    
    const IndObj* indObjAddr() const { return (const IndObj*)_indObj; };
    FuncNode*& complex() { return _complex; }; 
    const FuncNode* const & complex() const { return _complex; }; 
   private:
    //IndObj _indObj would be bad because then the destructor would be called
    char _indObj[sizeof(IndObj)]; 
    FuncNode* _complex;
    #ifdef DEBUG_ALLOC_OBJ_TYPE
     static ClassDesc _classDesc;
    #endif  
  }; // class OccNode

 public:
  class Trie
  {
   public:
    Trie() : _content((OccNode*)0) {};
    ~Trie() {};
    void init() { _content = (OccNode*)0; }; 
    operator bool() const { return (bool)_content; };
   private:
    OccNode** contentAddr() { return &_content; };   
   private:
    OccNode* _content;
   friend class Integrator;
   friend class Retrieval;
   friend class Removal;
   friend class Destruction;

  }; // class Trie

  class Integrator
  {
   public:
    Integrator()
     : _backtrackFuncNodes(),
       _backtrackArgNums()
    {
    };

    ~Integrator() {};
    void reset(Trie& tree);
    void nonconstFunc(const Symbol& f);
    void constant(const Symbol& c); 
    void variable();
    IndObj& indexedObj() const { return (*_occAddr)->indObj(); };
    ConstIndObj& constIndObj() const { return _lastConstNode->indObj(); };  

   private:
    void createOccNodeIfNeeded() const;
    void backtrack();

   private:
    OccNode** _occAddr;
    FuncNode* _lastConstNode;
    Stack<FuncNode*,MaxTermDepth> _backtrackFuncNodes;
    Stack<ulong,MaxTermDepth> _backtrackArgNums;
  }; // class Integrator


 class Retrieval
 {
  public:
   Retrieval() : _backtrackFuncNodes(), _backtrackArgNums() {};
   ~Retrieval() {};
   bool reset(Trie& tree);
   
   bool nonconstFunc(const Symbol& f);
   bool constant(const Symbol& c);
   void after() { CALL("Retrieval::after()"); ASSERT(!debugEndOfTerm); backtrack(); };

   IndObj& indexedObj() const { return (*_occAddr)->indObj(); };
   ConstIndObj& constIndObj() const { return _lastConstNode->indObj(); };  
   IndObj* indexedObjAddr() const { return (*_occAddr)->indObjAddr(); };
   ConstIndObj* constIndObjAddr() const { return _lastConstNode->indObjAddr(); };  

  private:
   void backtrack();

  private:
   OccNode** _occAddr;
   FuncNode* _lastConstNode;
   Stack<FuncNode*,MaxTermDepth> _backtrackFuncNodes;
   Stack<ulong,MaxTermDepth> _backtrackArgNums;
   #ifdef DEBUG_NAMESPACE
    bool debugEndOfTerm;  
   #endif
 }; // class Retrieval

 class Removal
 {
  public:
   Removal()
    : _backtrackFuncNodes(),
      _backtrackArgNums(),
      _candidatesForRecycling()
   {
   };
 
   ~Removal() {};
   //void reset() { _candidatesForRecycling.reset(); };

   void reset(Trie& tree);

   bool nonconstFunc(const Symbol& f);
   bool constant(const Symbol& c);
   bool occurences() { return (bool)(*_occAddr); }; 

   void after() { CALL("Removal::after()"); ASSERT(!debugEndOfTerm); backtrack(); }; 

   IndObj& indexedObj() const 
   { 
    CALL("Removal::indexedObj() const"); 
    ASSERT(*_occAddr);  
    return (*_occAddr)->indObj(); 
   };
   ConstIndObj& constIndObj() const 
   { 
    CALL("Removal::constIndObj() const"); 
    ASSERT(_lastConstNode); 
    return _lastConstNode->indObj(); 
   };  
   void recycleObsoleteNodes()
   {
    CALL("Removal::recycleObsoleteNodes()"); 
    while (_candidatesForRecycling.nonempty()) recycleObsolete(*(_candidatesForRecycling.pop()));
   };

  private:
   void backtrack();

  private:
   OccNode** _occAddr;
   FuncNode* _lastConstNode;
   FuncNode** _candForRecycling;
   Stack<FuncNode*,MaxTermDepth> _backtrackFuncNodes;
   Stack<ulong,MaxTermDepth> _backtrackArgNums;
   Stack<FuncNode**,MaxTermSize> _candidatesForRecycling;
   #ifdef DEBUG_NAMESPACE
    bool debugEndOfTerm;  
   #endif
 }; // class Removal

 class Destruction
 {
  public:
   Destruction() : _backtrackFuncNodes(), _backtrackArgNums() {};
   ~Destruction() {};
   bool reset(Trie& tree,bool& constant);

   void recycle() { destroyTree(*_tree); };
   bool next(bool& constant);
   IndObj& indexedObj() const 
   { 
    CALL("Removal::indexedObj() const"); 
    ASSERT(*_occAddr);  
    return (*_occAddr)->indObj(); 
   };
   ConstIndObj& constIndObj() const 
   { 
    CALL("Removal::constIndObj() const"); 
    ASSERT(_lastConstNode); 
    return _lastConstNode->indObj(); 
   }; 
  private:
   OccNode** _occAddr;
   OccNode** _tree;
   FuncNode* _lastConstNode;
   Stack<FuncNode*,MaxTermDepth> _backtrackFuncNodes;
   Stack<ulong,MaxTermDepth> _backtrackArgNums;
 }; // class Destruction

 private: // for class PathIndex
  // Releasing memory  
  static void destroyTree(FuncNode*& tree);
  static void destroyTree(OccNode*& tree);
  static void recycleObsolete(FuncNode*& tree);
  static void recycleObsolete(OccNode*& tree);
 
 friend class Removal;
 friend class Destruction;
}; // template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth> class PathIndex

}; // namespace Gematogen


//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_PATH_INDEX
 #define DEBUG_NAMESPACE "PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>"
#endif
#include "debugMacros.h"
//============================================================================

namespace Gematogen
{
template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::destroyTree(FuncNode*& tree)
{   
 CALL("destroyTree(FuncNode*& tree)");
 if (tree)
  {
   destroyTree(tree->altern());
   if (tree->isNonconst())
    {             
     for (ulong a = 0; a < tree->arity(); a++) 
      {
       if (tree->deeper()[a])
	{
         destroyTree(tree->deeper()[a]);
        }; 
      };
    }
   else
    {       
    };
   delete tree;
   tree = (FuncNode*)0;
  };
}; // void PathIndex<..>::destroyTree(FuncNode*& tree)



template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::destroyTree(OccNode*& tree)
{
 CALL("destroyTree(OccNode*& tree)");
 if (tree)   
  {
   destroyTree(tree->complex());     
   delete tree;
   tree = (OccNode*)0;
  };
}; // void PathIndex<..>::destroyTree(OccNode*& tree)
  

template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::recycleObsolete(FuncNode*& tree)
{
 CALL("recycleObsolete(FuncNode*& tree)");
 if (tree) 
  {
   bool obsolete = true;
   if (tree->isConstant()) { obsolete = !tree->indObj(); }
   else
    {
     for (ulong a = 0; a < tree->arity(); a++) 
      { 
       recycleObsolete(tree->deeper()[a]);
       if (tree->deeper()[a]) obsolete = false; 
      };
    };
   if (obsolete) 
    { 
     FuncNode* tmp = tree;
     tree = tmp->altern();
     delete tmp;
    }; 
  };   
}; // void PathIndex<..>::recycleObsolete(FuncNode*& tree) 

template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::recycleObsolete(OccNode*& tree)
{
 CALL("recycleObsolete(OccNode*& tree)");
 if (tree && (!tree->indObj()) && (!tree->complex()))
  {
   delete tree; 
   tree = (OccNode*)0;
  };                  
}; // void PathIndex<..>::recycleObsolete(OccNode*& tree)


}; // namespace Gematogen



//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_PATH_INDEX
 #define DEBUG_NAMESPACE "PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Destruction"
#endif
#include "debugMacros.h"
//============================================================================

namespace Gematogen
{

template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline bool PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Destruction::reset(Trie& tree,bool& constant) 
{ 
 CALL("reset(Trie& tree,bool& constant)");
 _occAddr = tree.contentAddr();
 _tree = _occAddr;
 constant = false;  // "constant" is here for generality
 _lastConstNode = (FuncNode*)0;
 return *_occAddr;
}; // bool PathIndex<..>::Destruction::reset(Trie& tree,bool& constant) 

template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline bool PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Destruction::next(bool& constant)
{
 CALL("next(bool& constant)");  
 FuncNode* nextFunc;
 if (_lastConstNode)  // was constant
  {
   ASSERT(_lastConstNode->isConstant());
   nextFunc = _lastConstNode->altern();
   check_func_node:
    if (nextFunc)
     { 
      if (nextFunc->isConstant()) 
       {
        _lastConstNode = nextFunc;
        constant = true;
        return true;
       }
      else
       {
        for (ulong a = 0; a < nextFunc->arity(); a++)
  	 if (nextFunc->deeper()[a])
	  {
	   if (a + 1 < nextFunc->arity())  
	    { 
	     _backtrackFuncNodes.push(nextFunc);
             _backtrackArgNums.push(a + 1);                
            }
           else
	    if (nextFunc->altern()) 
             {
	      _backtrackFuncNodes.push(nextFunc->altern());
              _backtrackArgNums.push(0);
             };
           _occAddr = nextFunc->deeper() + a;
           _lastConstNode = (FuncNode*)0;
           constant = false;
           return true; 
          };
        nextFunc = nextFunc->altern();
        goto check_func_node;   
       };
    }      
   else // try to backtrack
    {
     if (_backtrackFuncNodes.empty()) return false;     
     nextFunc = _backtrackFuncNodes.pop();  
     if (nextFunc->isConstant())
      {
       _backtrackArgNums.pop();
       _lastConstNode = nextFunc;
       constant = true;
       return true;
      };
     for (ulong a = _backtrackArgNums.pop(); a < nextFunc->arity(); a++) 
      if (nextFunc->deeper()[a])
       {
        if (a + 1 < nextFunc->arity())  
         { 
	  _backtrackFuncNodes.push(nextFunc);
          _backtrackArgNums.push(a + 1);                
         }
        else
	 if (nextFunc->altern()) 
          {
	   _backtrackFuncNodes.push(nextFunc->altern());
           _backtrackArgNums.push(0);
          };
        _occAddr = nextFunc->deeper() + a;
	_lastConstNode = (FuncNode*)0;
        constant = false;
        return true;
       };
     nextFunc = nextFunc->altern();
     goto check_func_node;
    };       
  }
 else // was not a constant 
  {
   nextFunc = (*_occAddr)->complex();
   goto check_func_node;
  };
}; // bool PathIndex<..>::Destruction::next(bool& constant) 

}; // namespace Gematogen


//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_PATH_INDEX
 #define DEBUG_NAMESPACE "PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Removal"
#endif
#include "debugMacros.h"
//============================================================================

namespace Gematogen
{

template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Removal::backtrack()
{
 CALL("Removal::backtrack()");
 if (_backtrackFuncNodes.nonempty()) 
  { 
   FuncNode* node = _backtrackFuncNodes.pop();    
   ulong argn = _backtrackArgNums.pop();
   _occAddr = node->deeper() + argn;
   argn++;
   ASSERT(argn <= node->arity());
   if (argn < node->arity())
    {
     _backtrackFuncNodes.push(node);    
     _backtrackArgNums.push(argn);
    };  
  }
 else 
  {
   DOP(debugEndOfTerm = true);
  };
}; // void backtrack()



template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Removal::reset(Trie& tree) 
{ 
 _occAddr = tree.contentAddr();
 _backtrackFuncNodes.reset();
 _backtrackArgNums.reset();
 _candidatesForRecycling.reset();
 DOP(debugEndOfTerm = false);
}; // void PathIndex<..>::Removal::reset(Trie& tree)


template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline bool PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Removal::nonconstFunc(const Symbol& f)
{
 CALL("nonconstFunc(const Symbol& f)");
 ASSERT(!debugEndOfTerm);
 if (!(*_occAddr)) return false;
 ulong arity = f.arity();
 ASSERT(arity);
   
 _candForRecycling = &((*_occAddr)->complex());
 FuncNode* node = (*_occAddr)->complex();

 while (node)
  {
   if (node->func() == f) 
    { 
     _occAddr = node->deeper();     
     if (arity > 1)
      { 
       _backtrackFuncNodes.push(node);
       _backtrackArgNums.push(1);
      };
     ASSERT(*_candForRecycling);
     _candidatesForRecycling.push(_candForRecycling);
     return true;
    };  
   //node->func() != f
   _candForRecycling = &(node->altern());
   node = node->altern();
  };
  // !node
  return false;
}; // bool PathIndex<..>::Removal::nonconstFunc(const Symbol& f)


template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline bool PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Removal::constant(const Symbol& c)
{
 CALL("constant(const Symbol& c)"); 
 ASSERT(!debugEndOfTerm);   
 ASSERT(!c.arity());
 if (!(*_occAddr)) return false;
 _candForRecycling = &((*_occAddr)->complex());
 _lastConstNode = (*_occAddr)->complex();
 while (_lastConstNode)
  {
   if (_lastConstNode->func() == c) 
    { 
     backtrack();
     ASSERT(*_candForRecycling);
     _candidatesForRecycling.push(_candForRecycling);
     return true;
    }  
   else 
    {
     _candForRecycling = &(_lastConstNode->altern());
     _lastConstNode = _lastConstNode->altern();
    };
  };
 // !_lastConstNode, there is no node with this constant
 return false;
}; // bool PathIndex<..>::Removal::constant(const Symbol& c)  


}; // namespace Gematogen

//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_PATH_INDEX
 #define DEBUG_NAMESPACE "PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Retrieval"
#endif
#include "debugMacros.h"
//============================================================================

namespace Gematogen
{
template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>  
inline bool PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Retrieval::reset(Trie& tree) 
{ 
 _occAddr = tree.contentAddr();
 _backtrackFuncNodes.reset();
 _backtrackArgNums.reset();
 DOP(debugEndOfTerm = false);
 return *_occAddr;
}; // bool PathIndex<..>::Retrieval::reset(Trie& tree)


template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline bool PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Retrieval::nonconstFunc(const Symbol& f)
{
 CALL("nonconstFunc(const Symbol& f)");
 ASSERT(!debugEndOfTerm);
 if (!(*_occAddr)) return false;
 ulong arity = f.arity();
 ASSERT(arity);
    
 FuncNode* node = (*_occAddr)->complex();
 while (node)
  {
   if (node->func() == f) 
    { 
     _occAddr = node->deeper();     
     if (arity > 1)
      { 
       _backtrackFuncNodes.push(node);
       _backtrackArgNums.push(1);
      };
     return true;
    };  
   //node->func() != f
   node = node->altern();
  };
 // !node
 return false;
}; // bool PathIndex<..>::Retrieval::nonconstFunc(const Symbol& f)


template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline bool PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Retrieval::constant(const Symbol& c)
{
 CALL("constant(const Symbol& c)"); 
 ASSERT(!debugEndOfTerm);   
 ASSERT(!c.arity());
 if (!(*_occAddr)) return false;
 _lastConstNode = (*_occAddr)->complex();
 while (_lastConstNode)
  {
   if (_lastConstNode->func() == c) 
    { 
     backtrack();
     return true;
    }  
   else _lastConstNode = _lastConstNode->altern();
  };
 // !_lastConstNode, there is no node with this constant
 return false;
}; // bool PathIndex<..>::Retrieval::constant(const Symbol& c)  


template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Retrieval::backtrack()
{
 CALL("backtrack()");
 if (_backtrackFuncNodes.nonempty()) 
  { 
   FuncNode* node = _backtrackFuncNodes.pop();    
   ulong argn = _backtrackArgNums.pop();
   _occAddr = node->deeper() + argn;
   argn++;
   ASSERT(argn <= node->arity());
   if (argn < node->arity())
    {
     _backtrackFuncNodes.push(node);    
     _backtrackArgNums.push(argn);
    };  
  }
 else 
  {
   DOP(debugEndOfTerm = true);
  };
}; // void PathIndex<..>::Retrieval::backtrack()


}; // namespace Gematogen


//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_PATH_INDEX
 #define DEBUG_NAMESPACE "PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Integrator"
#endif
#include "debugMacros.h"
//============================================================================

namespace Gematogen
{

template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Integrator::reset(Trie& tree) 
{ 
 _occAddr = tree.contentAddr();
 createOccNodeIfNeeded();
 _backtrackFuncNodes.reset();
 _backtrackArgNums.reset();
}; // void PathIndex<..>::Integrator::reset(Trie& tree) 

template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize> 
inline void PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Integrator::nonconstFunc(const Symbol& f)
{
 CALL("nonconstFunc(const Symbol& f)");
 ulong arity = f.arity();
 ASSERT(arity);
 ASSERT(*_occAddr);
 FuncNode** nodeAddr = &((*_occAddr)->complex());
 FuncNode* node = *nodeAddr;
 while (node)
  {
   if (node->func() == f) 
    { 
     _occAddr = node->deeper(); 
     createOccNodeIfNeeded();      
     if (arity > 1)
      { 
       _backtrackFuncNodes.push(node);
       _backtrackArgNums.push(1);
      };
     return;
    };  
   //node->func() != f
   nodeAddr = &(node->altern());
   node = *nodeAddr;
  };
 // !node, there is no node with this functor, create one
 node = new (arity) FuncNode(f);
 *nodeAddr = node;
 _occAddr = node->deeper();
 *_occAddr = new OccNode();        
 if (arity > 1)
  { 
   _backtrackFuncNodes.push(node);
   _backtrackArgNums.push(1);
  };
}; // void PathIndex<..>::Integrator::nonconstFunc(const Symbol& f) 

template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Integrator::constant(const Symbol& c)
{
 CALL("constant(const Symbol& c)");    
 ASSERT(!c.arity());
 ASSERT(*_occAddr);
 FuncNode** nodeAddr = &((*_occAddr)->complex());
 _lastConstNode = *nodeAddr;
 while (_lastConstNode)
  {
   if (_lastConstNode->func() == c) 
    { 
     backtrack();
     return;
    };  
   //_lastConstNode->func() != c
   nodeAddr = &(_lastConstNode->altern());
   _lastConstNode = *nodeAddr;
  };
 // !node, there is no node with this constant, create one
 _lastConstNode = new FuncNode(c,true);
 *nodeAddr = _lastConstNode;
 backtrack();
}; // void PathIndex<..>::Integrator::constant(const Symbol& c)


template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Integrator::variable()
{
 CALL("variable()");
 ASSERT(*_occAddr);
 backtrack();  
};

template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Integrator::createOccNodeIfNeeded() const
{
 if (!(*_occAddr)) 
  {
   *_occAddr = new OccNode();
  };   
}; // void PathIndex<..>::Integrator::createOccNodeIfNeeded() const

template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::Integrator::backtrack()
{
 CALL("backtrack()");
 if (_backtrackFuncNodes.nonempty()) 
  { 
   FuncNode* node = _backtrackFuncNodes.pop();    
   ulong argn = _backtrackArgNums.pop();
   _occAddr = node->deeper() + argn;
   createOccNodeIfNeeded();
   argn++;
   ASSERT(argn <= node->arity());
   if (argn < node->arity())
    {
     _backtrackFuncNodes.push(node);    
     _backtrackArgNums.push(argn);
    };  
  };
}; // void PathIndex<..>::Integrator::backtrack()



}; // namespace Gematogen


//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_PATH_INDEX
 #define DEBUG_NAMESPACE "PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::OccNode"
#endif
#include "debugMacros.h"
//============================================================================

namespace Gematogen
{
template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void* PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::OccNode::operator new(size_t) 
{
 CALL("operator new(size_t)");
 #ifdef DEBUG_ALLOC_OBJ_TYPE
  ALWAYS(_classDesc.registerAllocated(sizeof(OccNode)));
  return Alloc::allocate(sizeof(OccNode),&_classDesc); 
 #else 
  return Alloc::allocate(sizeof(OccNode)); 
 #endif
}; // void* PathIndex<..>::OccNode::operator new(size_t) 


template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::OccNode::operator delete(void* obj) 
{ 
 CALL("operator delete(void* obj)");
 #ifdef DEBUG_ALLOC_OBJ_TYPE
  ALWAYS(_classDesc.registerDeallocated(sizeof(OccNode)));
  Alloc::deallocate(obj,sizeof(OccNode),&_classDesc); 
 #else 
  Alloc::deallocate(obj,sizeof(OccNode)); 
 #endif
}; // void PathIndex<..>::OccNode::operator delete(void* obj)

}; // namespace Gematogen


//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_PATH_INDEX
 #define DEBUG_NAMESPACE "PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::FuncNode"
#endif
#include "debugMacros.h"
//============================================================================

namespace Gematogen
{

template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void* PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::FuncNode::operator new(size_t,ulong arity) 
// for nonconst. functors only
{ 
 CALL("operator new(size_t,ulong arity)");
 ASSERT(arity);
 #ifdef DEBUG_ALLOC_OBJ_TYPE
  ALWAYS(_funcClassDesc.registerAllocated(sizeof(FuncNode) + arity*sizeof(OccNode*)));
  return Alloc::allocate(sizeof(FuncNode) + arity*sizeof(OccNode*),&_funcClassDesc); 
 #else
  return Alloc::allocate(sizeof(FuncNode) + arity*sizeof(OccNode*)); 
 #endif
}; // void* PathIndex<..>::FuncNode::operator new(size_t,ulong arity)


template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void* PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::FuncNode::operator new(size_t) // for constants only 
{ 
 CALL("operator new(size_t)");
 #ifdef DEBUG_ALLOC_OBJ_TYPE
  ALWAYS(_constClassDesc.registerAllocated(sizeof(FuncNode) + sizeof(ConstIndObj)));
  return Alloc::allocate(sizeof(FuncNode) + sizeof(ConstIndObj),&_constClassDesc);  
 #else 
  return Alloc::allocate(sizeof(FuncNode) + sizeof(ConstIndObj));  
 #endif
}; // void* PathIndex<..>::FuncNode::operator new(size_t)


template <class Alloc,class Symbol,class IndObj,class ConstIndObj,ulong MaxTermDepth,ulong MaxTermSize>
inline void PathIndex<Alloc,Symbol,IndObj,ConstIndObj,MaxTermDepth,MaxTermSize>::FuncNode::operator delete(void* obj) 
{
 CALL("operator delete(void* obj)");
 ulong arity = ((FuncNode*)obj)->func().arity();
 if (arity)
  {
   #ifdef DEBUG_ALLOC_OBJ_TYPE
    ALWAYS(_funcClassDesc.registerDeallocated(sizeof(FuncNode) + arity*sizeof(OccNode*)));
    Alloc::deallocate(obj,sizeof(FuncNode) + arity*sizeof(OccNode*),&_funcClassDesc);
   #else
    Alloc::deallocate(obj,sizeof(FuncNode) + arity*sizeof(OccNode*));
   #endif
  }
 else
  {
   #ifdef DEBUG_ALLOC_OBJ_TYPE
    ALWAYS(_constClassDesc.registerDeallocated(sizeof(FuncNode) + sizeof(ConstIndObj)));
    Alloc::deallocate(obj,sizeof(FuncNode) + sizeof(ConstIndObj),&_constClassDesc);
   #else
    Alloc::deallocate(obj,sizeof(FuncNode) + sizeof(ConstIndObj));
   #endif
  };
}; // void PathIndex<..>::FuncNode::operator delete(void* obj)   

}; // namespace Gematogen

//======================================================================
#undef DEBUG_NAMESPACE
//======================================================================
#endif
