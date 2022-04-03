//
// File:         BackParaCandidates.h
// Description:  Indexed objects for back paramodulation.
// Created:      May 25, 2000, 14:20
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//============================================================================
#ifndef BACK_PARA_CANDIDATES_H
//=============================================================================
#define BACK_PARA_CANDIDATES_H
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "VampireKernelConst.h"
#include "GlobAlloc.h"
#include "SkipList.h"
#include "GList.h"
#include "Tuple.h"
#include "SimplificationOrdering.h"
#ifdef DEBUG_ALLOC_OBJ_TYPE
 #include "ClassDesc.h"
#endif
#include "DestructionMode.h"
namespace VK
{
  using namespace BK;
  using namespace Gematogen;
class TERM;
class Clause;
}; // namespace VK
//=============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_BACK_PARA_CANDIDATES
 #define DEBUG_NAMESPACE "BackParaCandidates"
#endif
#include "debugMacros.h"
//============================================================================

namespace VK
{
  using namespace BK;
  using namespace Gematogen;

class BackParaCandidates
{
 public: 
  class LitRedexPair
  {
   public:
    LitRedexPair(ulong ln,TERM* red,SimplificationOrdering::StoredConstraint* cns,LitRedexPair* nxt)
     : _tripple((List*)nxt)
    {
     _tripple.hd().el1 = ln;
     _tripple.hd().el2 = red;
     if (cns)
       {
	 _tripple.hd().el3 = SimplificationOrdering::current()->copy(cns); 
       }
     else
       _tripple.hd().el3 = (SimplificationOrdering::StoredConstraint*)0;
    };  
    LitRedexPair(ulong ln,TERM* red,SimplificationOrdering::StoredConstraint* cns)
     : _tripple((List*)0)
    {
     _tripple.hd().el1 = ln;
     _tripple.hd().el2 = red;
     if (cns)
       {
	 _tripple.hd().el3 = SimplificationOrdering::current()->copy(cns); 
       }
     else
       _tripple.hd().el3 = (SimplificationOrdering::StoredConstraint*)0;
    };
    ~LitRedexPair() 
    {
      CALL("destructor ~LitRedexPair()");
      if (DestructionMode::isThorough())
	{
	  if (constraint())
	    {
	      SimplificationOrdering::current()->releaseConstraint(constraint());
	    };
	};
    };
    void* operator new(size_t) 
    { 
     CALL("LitRedexPair::operator new(size_t)");
     #ifdef DEBUG_ALLOC_OBJ_TYPE
      ALWAYS(_classDesc.registerAllocated(sizeof(LitRedexPair)));
      return GlobAlloc::allocate(sizeof(LitRedexPair),&_classDesc); 
     #else
      return GlobAlloc::allocate(sizeof(LitRedexPair)); 
     #endif 
    };
    void  operator delete(void* obj) 
    {
     CALL("LitRedexPair::operator delete(void* obj)"); 
     #ifdef DEBUG_ALLOC_OBJ_TYPE
      ALWAYS(_classDesc.registerDeallocated(sizeof(LitRedexPair)));
      GlobAlloc::deallocate(obj,sizeof(LitRedexPair),&_classDesc);
     #else
      GlobAlloc::deallocate(obj,sizeof(LitRedexPair));  
     #endif 
    };
        
    static ulong minSize() { return sizeof(LitRedexPair); };
    static ulong maxSize() { return sizeof(LitRedexPair); };

    ulong litNum() const { return _tripple.hd().el1; };
    TERM* redex() const { return _tripple.hd().el2; };
    SimplificationOrdering::StoredConstraint* constraint() const 
      {
	CALL("constraint() const");
	return _tripple.hd().el3; 
      };
    LitRedexPair* next() const { return (LitRedexPair*)_tripple.tl(); }; 
    static void destroy(LitRedexPair* lst) 
    {
     CALL("destroy(LitRedexPair* lst)");
     LitRedexPair* rest; 
     while (lst) { rest = lst->next(); delete lst; lst = rest; };
    };
   private:
    typedef Tuple3<ulong,TERM*,SimplificationOrdering::StoredConstraint*> Tripple;
    typedef GList<GlobAlloc,Tripple,BackParaCandidates::LitRedexPair> List; 
   private:
    List _tripple;
    #ifdef DEBUG_ALLOC_OBJ_TYPE 
     static ClassDesc _classDesc;
    #endif
  }; // class LitRedexPair

 private:
  typedef SkipList<GlobAlloc,LitRedexPair*,Clause*,2,28,BackParaCandidates> SkList;

 public:      

  class Iterator     
  {
   public:
    Iterator() {};
    Iterator(const BackParaCandidates& lst) { reset(lst); };
    ~Iterator() {};
    void reset(const BackParaCandidates& lst) 
    { 
     _currNode = lst._skList.first(); 
     if (_currNode) { _currLRPair = _currNode->value(); }
     else _currLRPair = (LitRedexPair*)0;
    };

    bool next(Clause*& cl,LitRedexPair*& lrlist)
    {
     CALL("next(Clause*& cl,LitRedexPair*& lrlist)");
     if (_currLRPair) 
      { 
       cl = _currNode->key();       
       lrlist = _currLRPair;
       _currLRPair = _currLRPair->next();
       return true;
      };

     // !_currLRPair, try next node

     if (_currNode) 
      {
       next_node:
        _currNode = _currNode->next();
        if (_currNode)
 	 {
          lrlist = _currNode->value();
          if (lrlist) 
           {
            cl = _currNode->key();  
            _currLRPair = lrlist->next();
            return true;
           }
          else goto next_node;
         };
      };
     return false;
    }; // bool next(Clause*& cl,LitRedexPair*& lrlist)    

   private:
    const SkList::Node* _currNode;
    LitRedexPair* _currLRPair; 
  }; // class Iterator

 public:
  BackParaCandidates() : _skList() {};
  ~BackParaCandidates() 
  {
    CALL("destructor ~BackParaCandidates()");
    if (DestructionMode::isThorough()) destroy();
  };
  void destroy(); 
  void* operator new(size_t) 
  { 
   return GlobAlloc::allocate(sizeof(BackParaCandidates)); 
  };
  void operator delete(void* obj) 
  { 
   GlobAlloc::deallocate(obj,sizeof(BackParaCandidates)); 
  };
  void add(Clause* cl,ulong ln,TERM* red,SimplificationOrdering::StoredConstraint* cns);
  void remove(Clause* cl);
  bool empty() const { return !_skList; };
  #ifndef NO_DEBUG
   bool contains(Clause* cl) const { return _skList.contains(cl); }; 
   ulong size() const { return _skList.size(); };
   Clause* first() const { return _skList.first()->key(); };
   Clause* second() const { return _skList.first()->next()->key(); };
  #endif
 private:
  SkList _skList;
 friend class Iterator;
}; // class BackParaCandidates

inline void BackParaCandidates::destroy()
{
 CALL("destroy()");
 SkList::Destruction destr(_skList); 
 LitRedexPair* lrlist;
 while (destr.next(lrlist)) LitRedexPair::destroy(lrlist); 
}; // void BackParaCandidates::destroy()

inline void BackParaCandidates::add(Clause* cl,ulong ln,TERM* red,SimplificationOrdering::StoredConstraint* cns)
{
 CALL("add(Clause* cl,ulong ln,TERM* red,SimplificationOrdering::StoredConstraint* cns)");

 bool newNode;
 SkList::Node* node = _skList.insert(cl,newNode);
 if (newNode) 
  { 
   node->value() = new LitRedexPair(ln,red,cns);
  }
 else
  { 
   LitRedexPair* lrlist = node->value();
   ASSERT(lrlist);
   if ((lrlist->litNum() != ln) || (lrlist->redex() != red))
    {
     node->value() = new LitRedexPair(ln,red,cns,lrlist);     
    };
  };

 ASSERT(_skList.contains(cl));
}; // void BackParaCandidates::add(Clause* cl,ulong ln,TERM* red,SimplificationOrdering::StoredConstraint* cns)

inline void BackParaCandidates::remove(Clause* cl)
{
 CALL("remove(Clause* cl)");
 LitRedexPair* lrlist; 
 if (_skList.remove(cl,lrlist)) LitRedexPair::destroy(lrlist); 
}; // void BackParaCandidates::remove(Clause* cl)


}; // namespace VK

//=============================================================================
#endif
