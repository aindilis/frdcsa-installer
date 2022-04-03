//
// File:         SkipList.h
// Description:  Generic skip lists.
// Created:      Feb 16, 2000, 20:40
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
// Note:         A faster version of SkipList::remove(..) is needed
//               that does not return the value in the removed node.          
//============================================================================
#ifndef SKIP_LIST_H
//=============================================================================
#define SKIP_LIST_H 
#include <stddef.h>
//#include <limits.h>                                   // for ULONG_MAX
#ifndef NO_DEBUG_VIS 
 #include <iostream.h>                                 // for debugging output
#endif
#include "jargon.h"
#include "BlodKorvDebugFlags.h"
#include "DestructionMode.h"
#ifdef DEBUG_ALLOC_OBJ_TYPE
 #include "ClassDesc.h"       
#endif
#ifndef NO_DEBUG_VIS 
 #include "Label.h"                                    // for debugging output
 #include "Tab.h"                                      // for debugging output
#endif
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_SKIP_LIST
 #define DEBUG_NAMESPACE "SkipList<class Alloc,class ValType,class KeyType,ulong Base,ulong MaxNumOfLevels, class InstanceId>"
#endif
#include "debugMacros.h"
//============================================================================

namespace BK 
{

template < class Alloc, class ValType, class KeyType, ulong Base,ulong MaxNumOfLevels, class InstanceId >
class SkipList
{
 public:
  class Surfer;
  class Destruction;
  class Node
    {
    public:
      ~Node() {};
      KeyType key() const { return _key; };
      const ValType& value() const { return _value; };
      ValType& value() { return _value; };
      Node* next() { return *(jump()); };
      const Node* next() const { return *(jump()); };    
      static ulong minSize() { return sizeof(Node) + sizeof(Node*); }; 
      static ulong maxSize() { return sizeof(Node) + (MaxNumOfLevels*sizeof(Node*)); }; 


#ifndef NO_DEBUG_VIS
      ostream& output(ostream& ostr,int maxLevel) const
	{
	  ostr << '[' << Label((void*)this) << "] " << Tab<Alloc>::ULong(key()) << " ";
	  for (int n = 0; n <= level(); n++)
	    {
	      ostr << "<" << Label((void*)(jump()[n])) << "> ";
	    };
	  for (int n = level() + 1; n <= maxLevel; n++)
	    {
	      ostr << (Tab<Alloc>::String(Label::Length + 3,"|   "));
	    };
	  return ostr;
	};
#endif

    private:
      int level() const { return _level; };

    private: 
      // the following functions are public because Surfer and SkipList use them,
      // "friend" can't be used due to VC++
      Node** jump() { return (Node**)(this + 1); }; 
      const Node* const * jump() const { return (const Node* const *)(this + 1); };
      Node** jump(int lev) { return ((Node**)(this + 1)) + lev; }; 
      const Node* const * jump(int lev) const { return ((const Node* const *)(this + 1)) + lev; };
   
      Node(int l,const KeyType& k) : _level(l), _key(k) {};
      Node* next(int lev) { return ((Node**)(this + 1))[lev]; }; 
      const Node* next(int lev) const { return ((const Node* const *)(this + 1))[lev]; };
      
      static const Node* node(const Node * const * jmp)  // This is a hack! Must be removed.
	{
	  return ((Node*)jmp) - 1;
	};

      static Node* node(Node ** jmp)           // This is a hack! Must be removed.
	{
	  return ((Node*)jmp) - 1;
	};
  
      void* operator new(size_t size,int lv) 
	{    
	  CALL("operator new(size_t size,int lv)");  
#ifdef DEBUG_ALLOC_OBJ_TYPE
	  ALWAYS(_classDesc.registerAllocated(size + (lv+1)*sizeof(Node*)));
	  return Alloc::allocate(size + (lv+1)*sizeof(Node*),&_classDesc);
#else
	  return Alloc::allocate(size + (lv+1)*sizeof(Node*));
#endif
	};
      void operator delete(void* obj)
	{
	  CALL("operator delete(void* obj)");
#ifdef DEBUG_ALLOC_OBJ_TYPE
	  ALWAYS(_classDesc.registerDeallocated(sizeof(Node) + (((Node*)obj)->level() + 1)*sizeof(Node*)));
	  Alloc::deallocate(obj,sizeof(Node) + (((Node*)obj)->level() + 1)*sizeof(Node*),&_classDesc);
#else
	  Alloc::deallocate(obj,sizeof(Node) + (((Node*)obj)->level() + 1)*sizeof(Node*));
#endif
	};   
    private: 
      int _level;
      ValType _value;
      KeyType _key;    
#ifdef DEBUG_ALLOC_OBJ_TYPE
      static ClassDesc _classDesc;
#endif   
      friend class SkipList<Alloc,ValType,KeyType,Base,MaxNumOfLevels,InstanceId>;
      friend class Surfer;
      friend class Destruction;
    }; // class Node

  class Surfer
    {
    public:
      Surfer() {};
      Surfer(const SkipList& sl) { reset(sl); };
      ~Surfer() {};
      void reset(const SkipList& sl) 
	{
	  CALL("Surfer::reset(const SkipList& sl)");
	  ASSERT(sl.checkIntegrity());
	  _currMaxLevel = sl._currMaxLevel;
	  _jump = sl._topIndex;
	  _currNode = *_jump;
	};
      const Node* currNode() const { return _currNode; };
      const Node* const * currNodeAddr() const { return &_currNode; }; 

      bool next()
	{
	  CALL("next()");
	  _currNode = _currNode->next();
	  return (bool)_currNode;
	}; // bool next()

      bool find(const KeyType& searchKey) { return find(searchKey,_currNode); };
      bool find(const KeyType& searchKey,const Node*& node)
	{
	  CALL("Surfer::find(const KeyType& searchKey,const Node*& node)");
	ml_next_node:
	  node = _jump[_currMaxLevel]; 
	  if (node) 
	    {	  
	      ASSERT(node->level() == _currMaxLevel); 
	      if (node->key() < searchKey)
		{
		  if (!_currMaxLevel) return false;
		  int currLevel = _currMaxLevel - 1;
		  const Node* const * jump_ = _jump;        
		next_node:
		  node = jump_[currLevel];
		  ASSERT(node->level() >= currLevel);
		  if (node->key() == searchKey) return true;
		  if (node->key() < searchKey)
		    { // try smaller level  
		      if (!currLevel) return false;   
		      currLevel--;  
		    }
		  else // node->key() > searchKey, proceed with the same level    
		    { 
		      jump_ = node->jump();
		    };
		  goto next_node;
		};
	      // node->key() >= searchKey 
	      _jump = node->jump();
	      if (node->key() == searchKey) return true;
	      goto ml_next_node;
	    }
	  else // !node         
	    {
	      if (!_currMaxLevel) return false;
	      _currMaxLevel--;
	      goto ml_next_node;
	    };      
	}; // bool find(const KeyType& searchKey,const Node*& node) 
    private:
      int _currMaxLevel;
      const Node* const * _jump;
      const Node* _currNode;
    }; // class Surfer  

  class Destruction
    {
    public:
      Destruction() {};
      Destruction(SkipList& sl) { reset(sl); }; 
      ~Destruction() {};
      void reset(SkipList& sl) 
	{ 
	  _node = sl.first();
	  sl._currMaxLevel = 0;
	  sl._insertCount = 0;
	  sl._size = 0;
	  *sl._topIndex = (Node*)0;
	};
      bool next(ValType& val) 
	{
	  if (!_node) return false;
	  val = _node->value();
	  Node* tmp = _node;
	  _node = _node->next();
	  delete tmp;
	  return true;
	};
    private: 
      Node* _node;
    };

 public:
  SkipList() 
    {
      CALL("constructor SkipList()"); 
      init();
    };
  void init() 
    {
      CALL("init()");
      *_topIndex = (Node*)0; 
      _currMaxLevel = 0;
      _insertCount = 0;
      _size = 0;
    };
  ~SkipList() 
    {
      CALL("destructor ~SkipList()");
      if (DestructionMode::isThorough()) destroy(); 
    };
  void* operator new(size_t) 
    {
      CALL("operator new(size_t)"); 
#ifdef DEBUG_ALLOC_OBJ_TYPE
      ALWAYS(_classDesc.registerAllocated(sizeof(SkipList)));
      return Alloc::allocate(sizeof(SkipList),&_classDesc);
#else
      return Alloc::allocate(sizeof(SkipList));
#endif 
    };
  void operator delete(void* obj) 
    {
      CALL("operator delete(void* obj)"); 
#ifdef DEBUG_ALLOC_OBJ_TYPE
      ALWAYS(_classDesc.registerDeallocated(sizeof(SkipList)));
      Alloc::deallocate(obj,sizeof(SkipList),&_classDesc);
#else
      Alloc::deallocate(obj,sizeof(SkipList));
#endif 
    };
  static ulong minSize() { return sizeof(SkipList); }; // has nothing to do with size()
  static ulong maxSize() { return sizeof(SkipList); }; // has nothing to do with size()
  void destroy();
  void wipeOut() 
    {
      CALL("wipeOut()");
      init();
    };
  void reset() 
    {
      CALL("reset()");
      destroy();
      init();
    };
  operator bool() const { return *_topIndex; }; 
  const Node* first() const { return *_topIndex; };
  Node* first() { return *_topIndex; };
  ulong size() const { return _size; }; 

  ulong count() const 
    {
      ulong res = 0; 
      for (const Node* n = first(); n; n = n->next()) res++;
      return res;
    };

  bool contains(const KeyType& searchKey) const
    {
      Surfer surfer(*this);
      const Node* dummy;
      return surfer.find(searchKey,dummy);
    }; // bool contains (const KeyType& searchKey) const

  Node* insert(const KeyType& searchKey,bool& newNode)  
    {
      CALL("insert(const KeyType& searchKey,bool& newNode)");
      ASSERT(checkIntegrity());
      Node* update[MaxNumOfLevels];

      Node* currNode = Node::node(_topIndex);      // tricky!   
      int currLevel = _currMaxLevel;
      Node* nextNode = currNode->next(currLevel);  // tricky!

      KeyType nodeKey;
    check_next_node:
      if (nextNode)
	{
	  ASSERT(nextNode->level() <= currLevel);
	  nodeKey = nextNode->key();
	  if (nodeKey > searchKey) 
	    {
	      currNode = nextNode;
	      nextNode = nextNode->next(currLevel);        
	      goto check_next_node;
	    };
	  // nodeKey <= searchKey
	  if (nodeKey == searchKey) { newNode = false; ASSERT(checkIntegrity()); return nextNode; };
	  // nodeKey < searchKey, "if (nextNode)" is not needed anymore
	  update[currLevel] = currNode;
	  while (currLevel) 
	    { 
	      currLevel--;
	      nextNode = currNode->next(currLevel);
	      nodeKey = nextNode->key();
	      while (nodeKey > searchKey)
		{
		  currNode = nextNode;
		  nextNode = nextNode->next(currLevel); 
		  nodeKey = nextNode->key();
		};
	      // nodeKey <= searchKey
	      if (nodeKey == searchKey) { newNode = false; ASSERT(checkIntegrity()); return nextNode; };
	      // nodeKey < searchKey, "if (nextNode)" is not needed anymore
	      update[currLevel] = currNode;
	    };
	  ASSERT(nextNode->key() < searchKey);
	}
      else // !next_node
	{
	  update[currLevel] = currNode;
	  if (currLevel) 
	    {
	      currLevel--;
	      nextNode = currNode->next(currLevel);
	      goto check_next_node;
	    };
	};
 
      ASSERT(!currLevel);    

      // insert new node
      newNode = true;
      ASSERT(_insertCount < INT_MAX);
      _insertCount++;  
      _size++;
      currLevel = randomLevel();

      currNode = new (currLevel) Node(currLevel,searchKey); 
      Node** nodeJump = currNode->jump();  
      Node** jump; 

      if (currLevel > _currMaxLevel)
	{
	  for (int lev = _currMaxLevel; lev>=0; lev--)
	    { 
	      jump = update[lev]->jump() + lev; 
	      ASSERT((*jump) ? ((*jump)->key() < searchKey) : true);
	      nodeJump[lev] = *jump;       
	      *jump = currNode;
	    };
	  do
	    {
	      _currMaxLevel++;
	      _topIndex[_currMaxLevel] = currNode;
	      nodeJump[_currMaxLevel] = (Node*)0;
	    }
	  while (_currMaxLevel < currLevel);
	}
      else // currLevel <= _currMaxLevel
	{
	  for (int lev = currLevel; lev>=0; lev--)
	    { 
	      jump = update[lev]->jump() + lev; 
	      ASSERT((*jump) ? ((*jump)->key() < searchKey) : true); 
	      nodeJump[lev] = *jump;       
	      *jump = currNode;
	    };
	}; 
      ASSERT(checkIntegrity());
      return currNode;
    }; // Node* insert(const KeyType& searchKey,bool& newNode) 

  bool remove(const KeyType& searchKey,ValType& val) 
    {
      CALL("remove(const KeyType& searchKey,ValType& val)");
      ASSERT(checkIntegrity());
      Node** jump = _topIndex;  
      int currLevel = _currMaxLevel;
      Node* node;
    next_node:   
      node = jump[currLevel]; 
      if (node)
	{
	  ASSERT(node->level() >= currLevel);  
	  if (node->key() == searchKey)
	    { 
	      jump[currLevel] = node->jump()[currLevel]; 

	      if (currLevel) { currLevel--; goto next_node; };

	      val = node->value();
	      while ((!(_topIndex[_currMaxLevel])) && _currMaxLevel) _currMaxLevel--;
	      delete node;
	      _size--;
	      ASSERT(checkIntegrity());
	      ASSERT(!contains(searchKey));
	      return true;
	    };
	  if (node->key() > searchKey) 
	    {
	      jump = node->jump();        
	      goto next_node;
	    };
	  // node->key() < searchKey, try smaller level
	  if (currLevel) { currLevel--; goto next_node; }; 
	  ASSERT(checkIntegrity());    
	  ASSERT(!contains(searchKey)); 
	  return false;
	};
    
      // !node
      if (currLevel) { currLevel--; goto next_node; };
      ASSERT(checkIntegrity());
      ASSERT(!contains(searchKey));
      return false;
    }; // bool remove(const KeyType& searchKey,ValType& val)  

#ifndef NO_DEBUG
  bool checkIntegrity() const
    {
      if (_currMaxLevel)
	for (int lev = 0; lev <= _currMaxLevel; lev++)
	  {
	    if (!(_topIndex[lev])) return false;     
	  };
      return (count() == size()); 
    }; // bool checkIntegrity() const
#endif
 
#ifndef NO_DEBUG_VIS
  ostream& output(ostream& ostr) const
    {
      for (const Node* node = first(); node; node = node->next())
	{
	  node->output(ostr,_currMaxLevel) << "\n";
	};
      return ostr;
    };

  ostream& outputKeys(ostream& ostr) const
    {
      for (const Node* node = first(); node; node = node->next())
	{
	  ostr << node->key() << "\n";
	};
      return ostr;
    };
#endif

 private:
  SkipList(const SkipList&)
    {
      CALL("constructor SkipList(const SkipList&)"); 
      ICP("ICP0");
    };
  SkipList& operator=(const SkipList&)
  {
    CALL("operator=(const SkipList&)");
    ICP("ICP0");
    return *this;
  };
  int randomLevel() const
    { 
      int res = 0;
      ulong tmp = _insertCount;
      while (!(tmp % Base))
	{ 
	  res++;
	  tmp = tmp/Base;
	};
      return res;
    }; // randomLevel() 
 private:
  Node* _topIndex[MaxNumOfLevels];
  int _currMaxLevel;
  ulong _insertCount;
  ulong _size;
#ifdef DEBUG_ALLOC_OBJ_TYPE
  static ClassDesc _classDesc;
#endif
  friend class Surfer; 
  friend class Destruction;   
}; // template <class Alloc,class ValType,class KeyType,ulong Base,ulong MaxNumOfLevels, class InstanceId> class SkipList

template <class Alloc,class ValType,class KeyType,ulong Base,ulong MaxNumOfLevels, class InstanceId>
void SkipList<Alloc,ValType,KeyType,Base,MaxNumOfLevels,InstanceId>::destroy()
{
  CALL("destroy()");
  Node* node = *_topIndex;
  Node* tmp;
  while (node) 
    { 
      tmp = node;   
      node = node->next(); 
      delete tmp;
    };  
}; // void SkipList<Alloc,ValType,KeyType,Base,MaxNumOfLevels,InstanceId>::destroy()

}; // namespace BK

#ifndef NO_DEBUG_VIS
template <class Alloc,class ValType,class KeyType,ulong Base,ulong MaxNumOfLevels, class InstanceId>
ostream& operator << (ostream& ostr,const BK::SkipList<Alloc,ValType,KeyType,Base,MaxNumOfLevels,InstanceId>& lst)
{
  return lst.output(ostr);
};
#endif

//============ Specialised version for ValType = void
//=================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_SKIP_LIST
#define DEBUG_NAMESPACE "VoidSkipList<class Alloc,class KeyType,ulong Base,ulong MaxNumOfLevels, class InstanceId>"
#endif
#include "debugMacros.h"
//================================================================
namespace BK 
{

template <class Alloc,class KeyType,ulong Base,ulong MaxNumOfLevels, class InstanceId>
class VoidSkipList
{
 public:
  class Surfer;                 
  class Node
    {
    public:
      ~Node() {};
      KeyType key() const { return _key; };
      Node* next() { return *(jump()); };
      const Node* next() const { return *(jump()); };    
      static ulong minSize() { return sizeof(Node) + sizeof(Node*); }; 
      static ulong maxSize() { return sizeof(Node) + (MaxNumOfLevels*sizeof(Node*)); }; 

    
#ifndef NO_DEBUG_VIS
      ostream& output(ostream& ostr,int maxLevel) const
	{
	  ostr << '[' << Label((void*)this) << "] " << Tab<Alloc>::ULong(key()) << " ";
	  for (int n = 0; n <= level(); n++)
	    {
	      ostr << "<" << Label((void*)(jump()[n])) << "> ";
	    };
	  for (int n = level() + 1; n <= maxLevel; n++)
	    {
	      ostr << Tab<Alloc>::String(Label::Length + 3,"|   ");
	    };
	  return ostr;
	};
#endif
    private:
      Node(int l,const KeyType& k) : _level(l), _key(k) {};
      int level() const { return _level; };

      Node** jump() { return (Node**)(this + 1); }; 
      const Node* const * jump() const { return (const Node* const *)(this + 1); };
      Node** jump(int lev) { return ((Node**)(this + 1)) + lev; }; 
      const Node* const * jump(int lev) const { return ((const Node* const *)(this + 1)) + lev; };

      Node* next(int lev) { return ((Node**)(this + 1))[lev]; }; 
      const Node* next(int lev) const { return ((const Node* const *)(this + 1))[lev]; };
      
      static const Node* node(const Node * const * jmp)
	{
	  return ((Node*)jmp) - 1;
	};
      static Node* node(Node ** jmp)
	{
	  return ((Node*)jmp) - 1;
	};
  
      void* operator new(size_t size,int lv) 
	{ 
	  CALL("operator new(size_t size,int lv)");
#ifdef DEBUG_ALLOC_OBJ_TYPE
	  ALWAYS(_classDesc.registerAllocated(size + (lv+1)*sizeof(Node*)));
	  return Alloc::allocate(size + (lv+1)*sizeof(Node*),&_classDesc);
#else
	  return Alloc::allocate(size + (lv+1)*sizeof(Node*));
#endif
	};
      void operator delete(void* obj)
	{
	  CALL("operator delete(void* obj)");
#ifdef DEBUG_ALLOC_OBJ_TYPE 
	  ALWAYS(_classDesc.registerDeallocated((ulong)(sizeof(Node) + (((Node*)obj)->level() + 1)*sizeof(Node*))));
	  Alloc::deallocate(obj,(ulong)(sizeof(Node) + (((Node*)obj)->level() + 1)*sizeof(Node*)),&_classDesc); 
#else
	  Alloc::deallocate(obj,sizeof(Node) + (((Node*)obj)->level() + 1)*sizeof(Node*));  
#endif
	};   
    private:
      int _level;
      KeyType _key;
#ifdef DEBUG_ALLOC_OBJ_TYPE
      static ClassDesc _classDesc;
#endif
      friend class VoidSkipList;
      friend class Surfer;
    }; // class Node

  class Surfer
    {
    public:
      Surfer() {};
      Surfer(const VoidSkipList& sl) { reset(sl); };
      ~Surfer() {};
      void reset(const VoidSkipList& sl) 
	{
	  CALL("Surfer::reset(const VoidSkipList& sl)");
	  ASSERT(sl.checkIntegrity());
	  _currMaxLevel = sl._currMaxLevel;
	  _jump = sl._topIndex;
	  _currNode = *_jump;
	};
      const Node* currNode() const { return _currNode; };
      const Node* const * currNodeAddr() const { return &_currNode; }; 
      bool next()
	{
	  CALL("next()");
	  _currNode = _currNode->next();
	  return (bool)_currNode;
	}; // bool next()
      bool find(const KeyType& searchKey) { return find(searchKey,_currNode); };
      bool find(const KeyType& searchKey,const Node*& node)
	{
	  CALL("Surfer::find(const KeyType& searchKey,const Node*& node)");
	ml_next_node:
	  node = _jump[_currMaxLevel];   
	  if (node) 
	    {
	      ASSERT(node->level() == _currMaxLevel); 
	      if (node->key() < searchKey)
		{
		  if (!_currMaxLevel) return false;
		  int currLevel = _currMaxLevel - 1;
		  const Node* const * jump_ = _jump;        
		next_node:
		  node = jump_[currLevel];
		  ASSERT(node->level() >= currLevel);
		  if (node->key() == searchKey) return true;
		  if (node->key() < searchKey)
		    { // try smaller level  
		      if (!currLevel) return false;   
		      currLevel--;  
		    }
		  else // node->key() > searchKey, proceed with the same level            
		    jump_ = node->jump();
		  goto next_node;
		};
	      // node->key() >= searchKey 
	      _jump = node->jump();
	      if (node->key() == searchKey) return true;
	      goto ml_next_node;
	    }
	  else // !node         
	    {
	      if (!_currMaxLevel) return false;
	      _currMaxLevel--;
	      goto ml_next_node;
	    };      
	}; // bool find(const KeyType& searchKey,const Node*& node) 
    private:
      int _currMaxLevel;
      const Node* const * _jump;
      const Node* _currNode;
    }; // class Surfer  

 public:
  VoidSkipList()
    {
      CALL("constructor VoidSkipList()");
      init();
    };
  void init() 
    {
      CALL("init()");
      *_topIndex = (Node*)0; 
      _currMaxLevel = 0;
      _insertCount = 0;
      _size = 0;
    };
  ~VoidSkipList() 
    {
      CALL("destructor ~VoidSkipList()");
      if (DestructionMode::isThorough()) destroy(); 
    };
  void* operator new(size_t) 
    {
      CALL("operator new(size_t)");
#ifdef DEBUG_ALLOC_OBJ_TYPE
      ALWAYS(_classDesc.registerAllocated(sizeof(VoidSkipList))); 
      return Alloc::allocate(sizeof(VoidSkipList),&_classDesc); 
#else
      return Alloc::allocate(sizeof(VoidSkipList)); 
#endif
    };
  void operator delete(void* obj) 
    {
      CALL("operator delete(void* obj)");
#ifdef DEBUG_ALLOC_OBJ_TYPE 
      ALWAYS(_classDesc.registerDeallocated(sizeof(VoidSkipList))); 
      Alloc::deallocate(obj,sizeof(VoidSkipList),&_classDesc); 
#else 
      Alloc::deallocate(obj,sizeof(VoidSkipList));
#endif
    };
  static ulong minSize() { return sizeof(VoidSkipList); }; // has nothing to do with size()
  static ulong maxSize() { return sizeof(VoidSkipList); }; // has nothing to do with size()
  void destroy();
  void wipeOut() 
    {
      CALL("fastReset()");
      init();
    };
  void reset() 
    {
      CALL("reset()");
      destroy();
      init();
    };
  operator bool() const 
    {
      return *_topIndex; 
    };
  bool isNonempty() const { return *_topIndex; };
  ulong size() const { return _size; };
  ulong count() const 
    {
      ulong res = 0; 
      for (const Node* n = first(); n; n = n->next()) res++;
      return res;
    }; 
  const Node* first() const { return *_topIndex; };
  Node* first() { return *_topIndex; };
  bool contains(const KeyType& searchKey) const
    {
      Surfer surfer(*this);
      const Node* dummy;
      return surfer.find(searchKey,dummy);
    }; // bool contains (const KeyType& searchKey) const

  bool insert(const KeyType& searchKey)  
    {
      CALL("insert(const KeyType& searchKey)");
      ASSERT(checkIntegrity());
      Node* update[MaxNumOfLevels];
      Node* currNode = Node::node(_topIndex);
      int currLevel = _currMaxLevel;
      Node* nextNode = currNode->next(currLevel);
      KeyType nodeKey;
    check_next_node:
      if (nextNode)
	{
	  ASSERT(nextNode->level() <= currLevel);
	  nodeKey = nextNode->key();
	  if (nodeKey > searchKey) 
	    {
	      currNode = nextNode;
	      nextNode = nextNode->next(currLevel);        
	      goto check_next_node;
	    };
	  // nodeKey <= searchKey
	  if (nodeKey == searchKey) { ASSERT(checkIntegrity()); return false; };
	  // nodeKey < searchKey, "if (nextNode)" is not needed anymore
	  update[currLevel] = currNode;
	  while (currLevel) 
	    { 
	      currLevel--;
	      nextNode = currNode->next(currLevel);
	      nodeKey = nextNode->key();
	      while (nodeKey > searchKey)
		{
		  currNode = nextNode;
		  nextNode = nextNode->next(currLevel); 
		  nodeKey = nextNode->key();
		};
	      // nodeKey <= searchKey
	      if (nodeKey == searchKey) { ASSERT(checkIntegrity()); return false; };
	      // nodeKey < searchKey, "if (nextNode)" is not needed anymore
	      update[currLevel] = currNode;
	    };
	  ASSERT(nextNode->key() < searchKey);
	}
      else // !next_node
	{
	  update[currLevel] = currNode;
	  if (currLevel) 
	    {
	      currLevel--;
	      nextNode = currNode->next(currLevel);
	      goto check_next_node;
	    };
	};
 
      ASSERT(!currLevel);    

      // insert new node
      ASSERT(_insertCount < INT_MAX);
      _insertCount++;  
      _size++;
      currLevel = randomLevel();

      currNode = new (currLevel) Node(currLevel,searchKey); 
      Node** nodeJump = currNode->jump();  
      Node** jump; 

      if (currLevel > _currMaxLevel)
	{
	  for (int lev = _currMaxLevel; lev>=0; lev--)
	    { 
	      jump = update[lev]->jump() + lev; 
	      ASSERT((*jump) ? ((*jump)->key() < searchKey) : true);
	      nodeJump[lev] = *jump;       
	      *jump = currNode;
	    };
	  do
	    {
	      _currMaxLevel++;
	      _topIndex[_currMaxLevel] = currNode;
	      nodeJump[_currMaxLevel] = (Node*)0;
	    }
	  while (_currMaxLevel < currLevel);
	}
      else // currLevel <= _currMaxLevel
	{
	  for (int lev = currLevel; lev>=0; lev--)
	    { 
	      jump = update[lev]->jump() + lev; 
	      ASSERT((*jump) ? ((*jump)->key() < searchKey) : true); 
	      nodeJump[lev] = *jump;       
	      *jump = currNode;
	    };
	}; 
      ASSERT(checkIntegrity());
      return true;
    }; // bool insert(const KeyType& searchKey) 

  bool remove(const KeyType& searchKey) 
    {
      CALL("remove(const KeyType& searchKey)");
      ASSERT(checkIntegrity());
      Node** jump = _topIndex;  
      int currLevel = _currMaxLevel;
      Node* node;
    next_node:   
      node = jump[currLevel]; 
      if (node)
	{
	  ASSERT(node->level() >= currLevel);  
	  if (node->key() == searchKey)
	    { 
	      jump[currLevel] = node->jump()[currLevel]; 
	      if (currLevel) { currLevel--; goto next_node; };
	      while ((!(_topIndex[_currMaxLevel])) && _currMaxLevel) _currMaxLevel--;
	      delete node;
	      _size--;
	      ASSERT(checkIntegrity());
	      ASSERT(!contains(searchKey));
	      return true;
	    };
	  if (node->key() > searchKey) 
	    {
	      jump = node->jump();        
	      goto next_node;
	    };
	  // node->key() < searchKey, try smaller level
	  if (currLevel) { currLevel--; goto next_node; }; 
	  ASSERT(checkIntegrity());
	  ASSERT(!contains(searchKey));     
	  return false;
	};
    
      // !node
      if (currLevel) { currLevel--; goto next_node; };
      ASSERT(checkIntegrity());
      ASSERT(!contains(searchKey));
      return false;
    }; // bool remove(const KeyType& searchKey)  

#ifndef NO_DEBUG
  bool checkIntegrity() const
    {
      if (_currMaxLevel)
	for (int lev = 0; lev <= _currMaxLevel; lev++)
	  {
	    if (!(_topIndex[lev])) return false;
	  };
      return  (count() == size()); 
    }; // bool checkIntegrity() const
#endif

#ifndef NO_DEBUG_VIS
  ostream& output(ostream& ostr) const
    {
      for (const Node* node = first(); node; node = node->next())
	{
	  node->output(ostr,_currMaxLevel) << "\n";
	};
      return ostr;
    };
  ostream& outputKeys(ostream& ostr) const
    {
      for (const Node* node = first(); node; node = node->next())
	{
	  ostr << node->key() << "\n";
	};
      return ostr;
    };
#endif

 private:  
  VoidSkipList(const VoidSkipList&)
    {
      CALL("constructor VoidSkipList(const VoidSkipList&)"); 
      ICP("ICP0");
    };  
  VoidSkipList& operator=(const VoidSkipList&)
  {
    CALL("operator=(const VoidSkipList&)");
    ICP("ICP0");
    return *this;
  };
  int randomLevel() const
    { 
      int res = 0;
      ulong tmp = _insertCount;
      while (!(tmp % Base))
	{ 
	  res++;
	  tmp = tmp/Base;
	};
      return res;
    }; // randomLevel() 
 private:
  Node* _topIndex[MaxNumOfLevels];
  int _currMaxLevel;
  ulong _insertCount;
  ulong _size;
#ifdef DEBUG_ALLOC_OBJ_TYPE
  static ClassDesc _classDesc;
#endif
  friend class Surfer;    
}; // template <class Alloc,class ValType,class KeyType,ulong Base,ulong MaxNumOfLevels, class InstanceId> class VoidSkipList


template <class Alloc,class KeyType,ulong Base,ulong MaxNumOfLevels, class InstanceId>
void VoidSkipList<Alloc,KeyType,Base,MaxNumOfLevels,InstanceId>::destroy()
{
  CALL("destroy()");

  Node* node = *_topIndex;

  Node* tmp;
  while (node) 
    { 
      tmp = node;   
      node = node->next(); 
      delete tmp;
    };  
}; // void VoidSkipList<Alloc,KeyType,Base,MaxNumOfLevels,InstanceId>::destroy()

}; // namespace BK


#ifndef NO_DEBUG_VIS
template <class Alloc,class KeyType,ulong Base,ulong MaxNumOfLevels, class InstanceId>
ostream& operator << (ostream& ostr,const BK::VoidSkipList<Alloc,KeyType,Base,MaxNumOfLevels,InstanceId>& lst)
{
  return lst.output(ostr);
};
#endif
//======================================================================
#endif
