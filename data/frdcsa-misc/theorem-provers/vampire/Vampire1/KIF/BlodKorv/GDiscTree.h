//
// File:         GDiscTree.h
// Description:  Simple generic discrimination trees. 
// Created:      May 4, 2000, 17:20
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//============================================================================
#ifndef G_DISC_TREE_H
//=============================================================================
#define  G_DISC_TREE_H
#ifndef NO_DEBUG_VIS
 #include <iostream.h>                                 // for debugging output
#endif
#include "jargon.h"
#include "BlodKorvDebugFlags.h"
#ifdef DEBUG_ALLOC_OBJ_TYPE
 #include "ClassDesc.h"       
#endif
#ifndef NO_DEBUG
 #include "ObjDesc.h" 
#endif
#include "DestructionMode.h"
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_G_DISC_TREE
 #define DEBUG_NAMESPACE "GDiscTree<class Alloc,class DiscValType,class IndObjType>"
#endif
#include "debugMacros.h"
//============================================================================
#ifdef DEBUG_ALLOC_OBJ_TYPE
 #include "ClassDesc.h"       
#endif
//============================================================================

namespace BK 
{

template <class Alloc,class DiscValType,class IndObjType>
class GDiscTree
{
 public: 
  class Node
  {
   public:
    bool isLeaf() const 
    {
     CALL("Node::isLeaf() const");
     ASSERT(checkObj()); 
     return (!_next); 
    };   
    const DiscValType& val() const 
    {
     CALL("Node::val() const"); 
     ASSERT(checkObj()); 
     return _val; 
    };
    const Node* const & next() const 
    {
     CALL("next() const"); 
     ASSERT(checkObj()); 
     return _next; 
    };
    const Node* const & alt() const 
    {
     CALL("alt() const"); 
     ASSERT(checkObj()); 
     return _alt; 
    };
    IndObjType** indexedObjAddr() 
    {
     CALL("Node::indexedObjAddr()");
     ASSERT(checkObj()); 
     ASSERT(isLeaf());
     return (IndObjType**)(&_alt);
    }; 
    const IndObjType* const * indexedObjAddr() const
    {
     CALL("Node::indexedObjAddr() const");
     ASSERT(checkObj()); 
     ASSERT(isLeaf());
     return (const IndObjType* const *)(&_alt);
    }; 
  
    #ifndef NO_DEBUG 
     bool checkObj() const
     {
      #ifndef DEBUG_ALLOC
       return true;
      #else
       ObjDesc* objDesc = Alloc::hostObj((void*)this);
       if (!objDesc) 
        { 
         DMSG("GDiscTree<..>::Node::checkObj()> Null pointer to object descriptor.\n");  
         return false; 
        };  
       bool res = true;
       #ifdef DEBUG_ALLOC_OBJ_SIZE
        if (objDesc->size !=  sizeof(Node)) 
         {
          DMSG("GDiscTree<..>::Node::checkObj()> objDesc->size !=  sizeof(GDiscTree<..>::Node)\n");
          res = false;
         };
       #endif
       #ifdef DEBUG_ALLOC_OBJ_STATUS
        if (objDesc->status != ObjDesc::Persistent) 
         {
          DMSG("GDiscTree<..>::Node::checkObj()> objDesc->status != ObjDesc::Persistent\n");
          res = false;
         };
       #endif
       #ifdef DEBUG_ALLOC_OBJ_TYPE
        if (objDesc->classDesc != &_classDesc) 
         {
          DMSG("GDiscTree<..>::Node::checkObj()> objDesc->classDesc != &_classDesc\n");
          res = false;
         };
       #endif
       if (!res) DOP(DMSG("GDiscTree<..>::Node::checkObj()> host object ") << *objDesc << "\n"); 
       return res;
      #endif
     };
    #endif

   //private:
   public: // made public to please VC++
    Node() : _next((Node*)0), _alt((Node*)0) {};
    Node(const DiscValType& v) : _val(v), _next((Node*)0), _alt((Node*)0) {};
    Node(const DiscValType& v,Node* alt) : _val(v), _next((Node*)0), _alt(alt) {};   
    ~Node() {};
    static ulong minSize() { return sizeof(Node); };
    static ulong maxSize() { return sizeof(Node); };
    void* operator new(size_t) 
    {
     #ifdef DEBUG_ALLOC_OBJ_TYPE 
      ALWAYS(_classDesc.registerAllocated(sizeof(Node)));
      return Alloc::allocate(sizeof(Node),&_classDesc); 
     #else
      return Alloc::allocate(sizeof(Node)); 
     #endif
    };
    void  operator delete(void* obj) 
    { 
     #ifdef DEBUG_ALLOC_OBJ_TYPE 
      ALWAYS(_classDesc.registerDeallocated(sizeof(Node)));
      Alloc::deallocate(obj,sizeof(Node),&_classDesc); 
     #else
      Alloc::deallocate(obj,sizeof(Node)); 
     #endif
    };  
    Node*& next() 
    {
     CALL("next()"); 
     ASSERT(checkObj()); 
     return _next; 
    };
    Node*& alt() 
    {
     CALL("alt()"); 
     ASSERT(checkObj()); 
     return _alt; 
    };

    Node** nextAddr() { return &_next; };
    Node** altAddr() { return &_alt; };
   private:
    DiscValType _val;
    Node* _next;
    Node* _alt; 
    #ifdef DEBUG_ALLOC_OBJ_TYPE
     static ClassDesc _classDesc;
    #endif

   friend class Integrator;
   friend class Remover;
   friend class GDiscTree;
  }; // class Node

  class Integrator
  {
   public:
    Integrator() : _tree((GDiscTree*)0) {};
    void init() { _tree = (GDiscTree*)0; };
    Integrator(GDiscTree& dt) : _tree(&dt) {};
    void init(GDiscTree& dt) { _tree = &dt; };
    ~Integrator() {};
    void destroyThoroughly() { init(); }; 
    void setTree(GDiscTree& dt) { _tree = &dt; };
    void reset()
    {
     _currNode = _tree->_root;
     _currNodeAddr = &(_tree->_root);
    };
    void skipOrInsert(const DiscValType& val)
    {
     CALL("Integrator::skipOrInsert(const DiscValType& val)");
     check_node:
      if (_currNode) 
       {
        if (_currNode->isLeaf())
	 {
          _currNode = new Node(val,_currNode);  
	  *_currNodeAddr = _currNode;  
          moveDown();
          return;            
         };
        // normal node
        if (val == _currNode->val()) { moveDown(); return; };
        if (val < _currNode->val())
	 {
          _currNode = new Node(val,_currNode);  
	  *_currNodeAddr = _currNode;  
          moveDown();
          return; 
         };
        // val > _currNode->val()
        moveRight();
        goto check_node;
       }     
      else // write mode
       {
        _currNode = new Node(val);
        *_currNodeAddr = _currNode;
        moveDown();
       };
    }; // void skipOrInsert(const DiscValType& val)
 
    IndObjType** indexedObjAddr(bool& newLeaf) 
    {
     CALL("Integrator::indexedObjAddr(bool& newLeaf)");
     check_node:
      if (_currNode) 
       {     
	if (_currNode->isLeaf()) { newLeaf = false; return _currNode->indexedObjAddr(); };      
        // not a leaf, check the alternatives       
        moveRight();
        goto check_node;
       }
      else // write a new leaf
       {
        _currNode = new Node();
        *_currNodeAddr = _currNode;
        newLeaf = true;
        return _currNode->indexedObjAddr(); 
       };
    }; // IndObjType** indexedObjAddr(bool& newLeaf) 

    IndObjType** indexedObjAddr() 
    {
     // To be used when it is known that the object
     // is in the index
     CALL("Integrator::indexedObjAddr()");
     check_node:
      if (_currNode) 
       {     
	if (_currNode->isLeaf()) return _currNode->indexedObjAddr();  
        // not a leaf, check the alternatives       
        moveRight();
        goto check_node;
       }
      else // write a new leaf
       {
        _currNode = new Node();
        *_currNodeAddr = _currNode;
        return _currNode->indexedObjAddr();  
       };
    }; // IndObjType** indexedObjAddr()     

   private: 
    void moveDown() 
    {
     CALL("Integrator::moveDown()");
     ASSERT(!_currNode->isLeaf()); 
     _currNodeAddr = _currNode->nextAddr();
     _currNode = *_currNodeAddr; 
    };
   
    void moveRight()
    {
     CALL("Integrator::moveRight()");
     ASSERT(!_currNode->isLeaf());     
     _currNodeAddr = _currNode->altAddr();
     _currNode = *_currNodeAddr; 
    };   
 
   private:
    GDiscTree* _tree;
    Node* _currNode;
    Node** _currNodeAddr;
  }; // class Integrator
  
  class Remover
  {
   public:
    Remover() : _tree((GDiscTree*)0) {};
    void init() { _tree = (GDiscTree*)0; };
    Remover(GDiscTree& dt) : _tree(&dt) {};
    void init(GDiscTree& dt) { _tree = &dt; };
    ~Remover(){};
    void destroyThoroughly() { init(); };
    void setTree(GDiscTree& dt) { _tree = &dt; };
    void reset()
    {
     _currNode = _tree->_root;
     _currNodeAddr = &(_tree->_root);
     _branchToDestroy = _currNodeAddr;
    };

    bool skip(const DiscValType& val)
    { 
     CALL("Remover::skip(const DiscValType& val)");
     check_node:
      if ((!_currNode) || (_currNode->isLeaf()) || (val < _currNode->val())) return false;
      if (val == _currNode->val()) { moveDown(); return true; };       
      // val >= _currNode->val(), try the alternatives
      moveRight();
      goto check_node;
    }; // bool skip(const DiscValType& val)

    IndObjType** indexedObjAddr()
    {
     CALL("Remover::indexedObjAddr()");      
     check_node:
      if (!_currNode) return (IndObjType**)0;
      if (_currNode->isLeaf()) return (IndObjType**)(_currNode->altAddr());
      moveRight();
      goto check_node;
    }; // IndObjType** indexedObjAddr() 

    void destroyBranch()
    {
     CALL("Remover::destroyBranch()");
     Node* node = *_branchToDestroy;
     if (node->isLeaf()) { *_branchToDestroy = (Node*)0; delete node; return; };
     *_branchToDestroy = node->alt();
     Node* tmp;
     do
      {
       tmp = node;
       node = node->next(); 
       delete tmp;
       ASSERT((!node) || node->isLeaf() || (!node->alt()));
      }  
     while (node);
    }; // void destroyBranch() 

   private:
    void moveDown() 
    {
     CALL("Remover::moveDown()");
     ASSERT(!_currNode->isLeaf()); 
     if (_currNode->alt()) _branchToDestroy = _currNodeAddr;
     _currNodeAddr = _currNode->nextAddr();
     _currNode = *_currNodeAddr; 
    };
   
    void moveRight()
    {
     CALL("Remover::moveRight()");
     ASSERT(!_currNode->isLeaf());     
     _currNodeAddr = _currNode->altAddr();
     _currNode = *_currNodeAddr; 
     _branchToDestroy = _currNodeAddr;
    };       
   private:
    GDiscTree* _tree;
    Node* _currNode;
    Node** _currNodeAddr;
    Node** _branchToDestroy;
  }; // class Remover

 public:
  GDiscTree() : _root((Node*)0), _integrator(*this), _remover(*this) {};
  ~GDiscTree() { if (DestructionMode::isThorough()) destroyThoroughly(); };

  void destroyThoroughly() 
    {
      if (_root) 
	{ 
	  destroyTree(_root); 
	  _root = (Node*)0; 
	};
      _integrator.destroyThoroughly();
      _remover.destroyThoroughly();
      _integrator.init(*this);
      _remover.init(*this);
    };
  void destroyWithObjects() 
  { 
   if (_root) 
     { 
       destroyTreeWithObjects(_root); 
       _root = (Node*)0; 
     }; 
   _integrator.destroyThoroughly();
   _remover.destroyThoroughly();
   _integrator.init(*this);
   _remover.init(*this);
  }; 

#ifndef NO_DEBUG
  bool clean() const { return !_root; };
#endif 
  Integrator& integrator() { return _integrator; };
  Remover& remover() { return _remover; };
  const Node* const & root() const { return _root; };
 private:
  void destroyTree(Node* tree) 
  {
   CALL("destroyTree(Node* tree)");
   if (tree->isLeaf()) { delete tree; return; };
   if (tree->alt()) destroyTree(tree->alt());
   destroyTree(tree->next());
   delete tree;
  };
  void destroyTreeWithObjects(Node* tree) 
  {
   CALL("destroyTreeWithObjects(Node* tree");
   if (tree->isLeaf()) 
    {
     ((IndObjType*)(tree->alt()))->destroy();
     delete tree; 
     return; 
    };
   if (tree->alt()) destroyTreeWithObjects(tree->alt());
   destroyTreeWithObjects(tree->next());
   delete tree;
  };
 private:  
  Node* _root;
  Integrator _integrator;
  Remover _remover;
 friend class Integrator;
 friend class Remover;
 friend class Node;
}; // class GDiscTree<class Alloc,class DiscValType,class IndObjType>


}; // namespace BK

//=============================================================================
#endif
