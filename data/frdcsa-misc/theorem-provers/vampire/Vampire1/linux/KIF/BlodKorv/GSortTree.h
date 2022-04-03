//
// File:         GSortTree.h
// Description:  Simple generic binary sorting trees.
// Created:      Feb 13, 2000, 16:30
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//============================================================================
#ifndef G_SORT_TREE_H
//=============================================================================
#define G_SORT_TREE_H 
#include "jargon.h"
#include "BlodKorvDebugFlags.h"
#include "GList.h"
#include "DestructionMode.h"
#ifdef DEBUG_ALLOC_OBJ_TYPE
 #include "ClassDesc.h"
#endif
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_G_SORT_TREE
 #define DEBUG_NAMESPACE "GSortTree<class Alloc,class ValType,class InstanceId>"
#endif
#include "debugMacros.h"
//============================================================================
namespace BK 
{
template <class Alloc,class ValType,class InstanceId>
class GSortTree
{
 public:
  class Node
  {
   public: 
    Node(const ValType& v) : val(v), count(1), left((Node*)0), right((Node*)0) 
    {
     CALL("constructor Node(const ValType& v)");
    };
    Node(const ValType& v,Node* l,Node* r) : val(v), count(1), left(l), right(r) 
    {
     CALL("constructor Node(const ValType& v,Node* l,Node* r)");
    };
    ~Node() {};
    void* operator new(size_t) 
    { 
     CALL("operator new(size_t)");
     #ifdef DEBUG_ALLOC_OBJ_TYPE
      ALWAYS(_classDesc.registerAllocated(sizeof(Node)));
      return Alloc::allocate(sizeof(Node),&_classDesc);  
     #else
      return Alloc::allocate(sizeof(Node));   
     #endif
    }; 
    void  operator delete(void* obj) 
    { 
     CALL("operator delete(void* obj)");
     #ifdef DEBUG_ALLOC_OBJ_TYPE
      ALWAYS(_classDesc.registerDeallocated(sizeof(Node)));
      Alloc::deallocate(obj,sizeof(Node),&_classDesc); 
     #else
      Alloc::deallocate(obj,sizeof(Node)); 
     #endif
    };
    static ulong minSize() { return sizeof(Node); };
    static ulong maxSize() { return sizeof(Node); };
    const ValType& value() const { return val; };
    ValType& value() { return val; };
    const unsigned long& score() const { return count; };
    unsigned long& score() { return count; };
    Node* const & less() const { return left; }; 
    Node*& less() { return left; }; 
    Node* const & greater() const { return right; }; 
    Node*& greater() { return right; };
   private:
    ValType val;
    unsigned long count;
    Node* left;
    Node* right;    
    #ifdef DEBUG_ALLOC_OBJ_TYPE
     static ClassDesc _classDesc;
    #endif
  }; // class Node

  class LeftRightIter
  {
   public:
    LeftRightIter() : currNode((const Node*)0), stack((NodeList*)0) {};
    LeftRightIter(const GSortTree& tree) : stack((NodeList*)0)
    {
     currNode = tree.root;
     if (!currNode) return;
     while (currNode->less())
      {
       stack = new NodeList(currNode,stack);
       currNode = currNode->less();
      };
    }; 
    ~LeftRightIter() 
      { 
	if (DestructionMode::isThorough()) destroyThoroughly();
      };
    void destroyThoroughly()
      {
	NodeList::destroyList(stack);
	currNode = (const Node*)0;
	stack = (NodeList*)0;
      };
    void reset(const GSortTree& tree)
    {
     NodeList::destroyList(stack);
     stack = (NodeList*)0; 
     currNode = tree.root;
     if (!currNode) return;
     while (currNode->less())
      {
       stack = new NodeList(currNode,stack);
       currNode = currNode->less();
      };
    };
    operator bool() const { return (bool)currNode; };
    void next()
    {
     if (currNode->greater())     
      {
       currNode = currNode->greater();       
       while (currNode->less())
        {
         stack = new NodeList(currNode,stack);
         currNode = currNode->less();
        };
      }
     else // try to pop the stack
      if (stack)
       {
	currNode = stack->hd();
        NodeList* tmp = stack;
        stack = stack->tl();
        delete tmp;        
       }	
      else currNode = (const Node*)0;             
    };
    const ValType& value() const { return currNode->value(); };
    const unsigned long& score() const { return currNode->score(); };
   private:
    typedef GList<Alloc,const Node*,GSortTree<Alloc,ValType,InstanceId>::LeftRightIter> NodeList;
   private:
    const Node* currNode;   
    NodeList* stack;
  }; // class LeftRightIter

  class RightLeftIter
  {
   public:
    RightLeftIter() : currNode((const Node*)0), stack((NodeList*)0) {};
    RightLeftIter(const GSortTree& tree) : stack((NodeList*)0)
    {
     reset(tree);
    }; 
    ~RightLeftIter() 
      { 
	if (DestructionMode::isThorough()) 
	  {
	    NodeList::destroyList(stack); 
	    stack = (NodeList*)0;
	  };
      };
    void reset(const GSortTree& tree)
    {
     NodeList::destroyList(stack);
     stack = (NodeList*)0; 
     currNode = tree.root;
     if (!currNode) return;
     while (currNode->greater())
      {
       stack = new NodeList(currNode,stack);
       currNode = currNode->greater();
      };
    };
    operator bool() const { return (bool)currNode; };
    void next()
    {
     if (currNode->less())     
      {
       currNode = currNode->less();       
       while (currNode->greater())
        {
         stack = new NodeList(currNode,stack);
         currNode = currNode->greater();
        };
      }
     else // try to pop the stack
      if (stack)
       {
	currNode = stack->hd();
        NodeList* tmp = stack;
        stack = stack->tl();
        delete tmp;        
       }	
      else currNode = (const Node*)0;             
    };
    const ValType& value() const { return currNode->value(); };
    const unsigned long& score() const { return currNode->score(); };
   private:
    typedef GList<Alloc,const Node*,GSortTree::RightLeftIter> NodeList;
   private:
    const Node* currNode;   
    NodeList* stack;
  }; // class RightLeftIter

 public:
  GSortTree() : root((Node*)0) {};
  ~GSortTree() 
    { 
      CALL("destructor ~GSortTree()"); 
      if (DestructionMode::isThorough()) destroyThoroughly(); 
    };
  void insert(const ValType& v)
  {
   Node** n = find(v,&root);
   if (*n) { ((*n)->score())++; }
   else *n = new Node(v);        
  };
  void insert(const ValType& v,unsigned long& oldScore)
  {
   Node** n = find(v,&root);
   if (*n) { oldScore = (*n)->score(); (*n)->score()++; }
   else { *n = new Node(v); oldScore = 0; };        
  };
  unsigned long score(const ValType& v) const
  {
   Node** n = find(v,&root);
   if (*n) return (*n)->score();
   return 0;
  };
  void destroyThoroughly() { destroyTree(root); root = (Node*)0; };
 private:
  static Node** find(const ValType& v,Node** rt)
  {
   Node* r = *rt;
   if ((!r) || (r->value() == v)) return rt;
   if (r->value() > v) return find(v,&(r->less())); 
   return find(v,&(r->greater()));  
  };
  static void destroyTree(Node* rt) 
   {
    if (rt)
     {
      destroyTree(rt->less());
      destroyTree(rt->greater());
      delete rt; 
     }; 
   }; 
 private: 
  Node* root;
 friend class LeftRightIter;
 friend class RightLeftIter;
}; // template <class Alloc,class ValType,class InstanceId> class GSortTree

}; // namespace BK

//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_G_SORT_TREE
 #define DEBUG_NAMESPACE "GMSortTree<class Alloc,class ValType,class InfoType,class InstanceId>"
#endif
#include "debugMacros.h"
//============================================================================

namespace BK 
{


// [G]eneric [M]arked SortTree 

template <class Alloc,class ValType,class InfoType,class InstanceId>
class GMSortTree
{
 public:
  class Node
  {
   public: 
    Node(const ValType& v) : val(v), left((Node*)0), right((Node*)0) 
    {
     CALL("constructor Node(const ValType& v)");
    };
    Node(const ValType& v,const InfoType& i) : val(v), _info(i), left((Node*)0), right((Node*)0) 
    {
     CALL("constructor Node(const ValType& v,const InfoType& i)"); 
    };
    Node(const ValType& v,const InfoType& i,Node* l,Node* r) : val(v), _info(i), left(l), right(r) 
    {
     CALL("constructor Node(const ValType& v,const InfoType& i,Node* l,Node* r)");
    };
    Node(const ValType& v,Node* l,Node* r) : val(v), left(l), right(r) 
    {
     CALL("constructor Node(const ValType& v,Node* l,Node* r)");
    };

    ~Node() {};
    void* operator new(size_t) 
    { 
     CALL("operator new(size_t)");
     #ifdef DEBUG_ALLOC_OBJ_TYPE
      ALWAYS(_classDesc.registerAllocated(sizeof(Node)));
      return Alloc::allocate(sizeof(Node),&_classDesc);  
     #else
      return Alloc::allocate(sizeof(Node));   
     #endif
    }; 
    void  operator delete(void* obj) 
    { 
     CALL("operator delete(void* obj)");
     #ifdef DEBUG_ALLOC_OBJ_TYPE
      ALWAYS(_classDesc.registerDeallocated(sizeof(Node)));
      Alloc::deallocate(obj,sizeof(Node),&_classDesc); 
     #else
      Alloc::deallocate(obj,sizeof(Node)); 
     #endif
    };

    static ulong minSize() { return sizeof(Node); };
    static ulong maxSize() { return sizeof(Node); };

    const ValType& value() const { return val; };
    ValType& value() { return val; };
    const InfoType& info() const { return _info; };
    InfoType& info() { return _info; }; 
    Node* const & less() const { return left; }; 
    Node*& less() { return left; }; 
    Node* const & greater() const { return right; }; 
    Node*& greater() { return right; };
   private:
    ValType val;
    InfoType _info;
    Node* left;
    Node* right; 
    #ifdef DEBUG_ALLOC_OBJ_TYPE
     static ClassDesc _classDesc;
    #endif   
  }; // class Node

  class LeftRightIter
  {
   public:
    LeftRightIter() : currNode((const Node*)0), stack((NodeList*)0) {};
    LeftRightIter(const GMSortTree& tree) : stack((NodeList*)0)
    {
     reset(tree);
    }; 
    ~LeftRightIter() 
      {
	if (DestructionMode::isThorough())
	  {
	    NodeList::destroy(stack);
	    stack = (NodeList*)0;
	  };
      };
    void reset(const GMSortTree& tree)
    {
     NodeList::destroy(stack);
     stack = (NodeList*)0; 
     currNode = tree.root;
     if (!currNode) return;
     while (currNode->less())
      {
       stack = new NodeList(currNode,stack);
       currNode = currNode->less();
      };
    };
    operator bool() const { return (bool)currNode; };
    void next()
    {
     if (currNode->greater())     
      {
       currNode = currNode->greater();       
       while (currNode->less())
        {
         stack = new NodeList(currNode,stack);
         currNode = currNode->less();
        };
      }
     else // try to pop the stack
      if (stack)
       {
	currNode = stack->hd();
        NodeList* tmp = stack;
        stack = stack->tl();
        delete tmp;        
       }	
      else currNode = (const Node*)0;             
    };
    const ValType& value() const { return currNode->value(); };
    const InfoType& info() const { return currNode->info(); };
   private:
    typedef GList<Alloc,const Node*,GSortTree<Alloc,ValType,InstanceId>::LeftRightIter> NodeList;
   private:
    const Node* currNode;   
    NodeList* stack;
  }; // class LeftRightIter

  class RightLeftIter
  {
   public:
    RightLeftIter() : currNode((const Node*)0), stack((NodeList*)0) {};
    RightLeftIter(const GMSortTree& tree) : stack((NodeList*)0)
    {
     reset(tree);
    }; 
    ~RightLeftIter() 
      {
	if (DestructionMode::isThorough())
	  {
	    NodeList::destroy(stack);
	    stack = (NodeList*)0;
	  };
      };
    void reset(const GMSortTree& tree)
    {
     NodeList::destroy(stack);
     stack = (NodeList*)0;       
     currNode = tree.root;
     if (!currNode) return;
     while (currNode->greater())
      {
       stack = new NodeList(currNode,stack);
       currNode = currNode->greater();
      };
    };
    operator bool() const { return (bool)currNode; };
    void next()
    {
     if (currNode->less())     
      {
       currNode = currNode->less();       
       while (currNode->greater())
        {
         stack = new NodeList(currNode,stack);
         currNode = currNode->greater();
        };
      }
     else // try to pop the stack
      if (stack)
       {
	currNode = stack->hd();
        NodeList* tmp = stack;
        stack = stack->tl();
        delete tmp;        
       }	
      else currNode = (const Node*)0;             
    };
    const ValType& value() const { return currNode->value(); };
    const InfoType& info() const { return currNode->info(); };
   private:
    typedef GList<Alloc,const Node*,GSortTree<Alloc,ValType,InstanceId>::RightLeftIter> NodeList;
   private:
    const Node* currNode;   
    NodeList* stack;
  }; // class RightLeftIter

 public:
  GMSortTree() : root((Node*)0) {};
  GMSortTree(const InfoType& def) : root((Node*)0) { defInfo() = def; };  
  ~GMSortTree() 
    { 
      CALL("destructor ~GMSortTree()"); 
      if (DestructionMode::isThorough()) destroy(); 
    };
  void setDefInfo(const InfoType& def) { defInfo() = def; };  
  void insert(const ValType& v,const InfoType& i)
  {
   Node** n = find(v,&root);
   if (*n) { (*n)->info() = i; }
   else *n = new Node(v,i);        
  };
  InfoType& info(const ValType& v)
  {
   Node** n = find(v,&root);
   if (!(*n)) *n = new Node(v,defInfo());
   return (*n)->info();  
  };  
  void destroy() { destroyTree(root); root = (Node*)0; };
 
  Node* node(const ValType& v) 
  {
   CALL("node(const ValType& v)");
   return *(find(v,&root));
  };   

 private:
  InfoType& defInfo() { return *((InfoType*)_defInfo); };
  const InfoType& defInfo() const { return *((const InfoType*)_defInfo); };
  static Node** find(const ValType& v,Node** rt)
  {
   Node* r = *rt;
   if ((!r) || (r->value() == v)) return rt;
   if (r->value() > v) return find(v,&(r->less())); 
   return find(v,&(r->greater()));  
  };
  static void destroyTree(Node* rt) 
   {
    if (rt)
     {
      destroyTree(rt->less());
      destroyTree(rt->greater());
      delete rt; 
     }; 
   }; 
 private: 
  Node* root;
  char _defInfo[sizeof(InfoType)];
 friend class LeftRightIter;
 friend class RightLeftIter;
}; // template <class Alloc,class ValType,class InfoType,class InstanceId> class GMSortTree


}; // namespace BK

//======================================================================
#undef DEBUG_NAMESPACE
//======================================================================
#endif
