//
// File:         GList.h
// Description:  Generic lists.
// Created:      Jan 30, 2000, 20:00 
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//============================================================================
#ifndef G_LIST_H
//=============================================================================
#define G_LIST_H 
#ifndef NO_DEBUG_VIS
 #include <iostream.h>
#endif
#include <stddef.h>
#include "jargon.h"
#include "BlodKorvDebugFlags.h"
#ifdef DEBUG_ALLOC_OBJ_TYPE
#include "ClassDesc.h"
#endif
#ifdef DEBUG_ALLOC
#include "ObjDesc.h"
#endif
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_G_LIST
 #define DEBUG_NAMESPACE "GList<class Alloc,class ElType,class InstanceId>"
#endif
#include "debugMacros.h"
//============================================================================
namespace BK 
{
template <class Alloc,class ElType,class InstanceId>
class GList
{
 public:
  GList() {};
  GList(GList<Alloc,ElType,InstanceId>* t) : tail(t) {};
  GList(const ElType& h,GList<Alloc,ElType,InstanceId>* t) : head(h), tail(t) {};
  GList(const ElType& h) : head(h), tail((GList*)0) {};
  ~GList() { CALL("destructor ~GList()"); };
  void* operator new(size_t) 
  {
   CALL("operator new(size_t)"); 
   #ifdef DEBUG_ALLOC_OBJ_TYPE
    ALWAYS(_classDesc.registerAllocated(sizeof(GList<Alloc,ElType,InstanceId>)));
    return Alloc::allocate(sizeof(GList<Alloc,ElType,InstanceId>),&_classDesc); 
   #else
    return Alloc::allocate(sizeof(GList<Alloc,ElType,InstanceId>));  
   #endif
  };
  void  operator delete(void* obj) 
  { 
   CALL("operator delete(void* obj)");
   
   #ifdef DEBUG_ALLOC_OBJ_TYPE
    ALWAYS(_classDesc.registerDeallocated(sizeof(GList<Alloc,ElType,InstanceId>)));
    Alloc::deallocate(obj,sizeof(GList<Alloc,ElType,InstanceId>),&_classDesc);  
   #else 
    Alloc::deallocate(obj,sizeof(GList<Alloc,ElType,InstanceId>));
   #endif
   //   *(((ulong*)obj) + 1) = 24894; // debug 
  };
  static ulong minSize() { return sizeof(GList<Alloc,ElType,InstanceId>); };
  static ulong maxSize() { return sizeof(GList<Alloc,ElType,InstanceId>); };
  ElType& hd() 
    {
      CALL("hd()");
      ASSERT(checkObj());
      return head; 
    };
  const ElType& hd() const { return head; };
  GList<Alloc,ElType,InstanceId>*& tl() { return tail; };  
  GList<Alloc,ElType,InstanceId>* const & tl() const { return tail; };   
  static void destroyList(GList<Alloc,ElType,InstanceId>* lst) 
  {
   CALL("destroyList(GList<Alloc,ElType,InstanceId>* lst)");
   GList<Alloc,ElType,InstanceId>* rest;
   while (lst) { rest = lst->tl(); delete lst; lst = rest; };
  };
  static GList* inv(const GList* lst)
  {
   GList* res = (GList*)0;
   while (lst)
    {
     res = new GList(lst->hd(),res);
     lst = lst->tl();
    };
   return res;  
  };  
  static void inverse(GList*& lst)
  {
   GList* next;
   GList* prev = (GList*)0;
   while (lst) 
    {
     next = lst->tl();
     lst->tl() = prev;
     prev = lst;
     lst = next; 
    }; 
   lst = prev;
  }; 
  static bool member(const ElType& x,const GList* lst)
  {
   while (lst)
    {
     if (lst->hd() == x) return true;
     lst = lst->tl();
    };
   return false;
  };
 
  static ulong length(const GList* lst)
  {
   ulong res = 0;
   while (lst)
    {
     res++;
     lst = lst->tl();          
    };
   return res;
  };

  #ifndef NO_DEBUG_VIS   
   ostream& output(ostream& str) const { return str << hd(); };
  #endif

#ifdef DEBUG_ALLOC
   bool checkObj() const
     {
       ObjDesc* objDesc = Alloc::hostObj((void*)this);
#ifdef DEBUG_ALLOC_OBJ_STATUS
       if (objDesc->status != ObjDesc::Persistent)
	 {
	   cout << "Bad list node: " << *(objDesc) << "\n";
	   return false;
	 };
#endif
       return true;
     };
#endif

 public:
  class Iter
  {
   public:
    Iter() : lst(0) {};
    Iter(GList<Alloc,ElType,InstanceId>* l) : lst(l) {};
    operator bool() const { return (bool)lst; };
    void reset(GList<Alloc,ElType,InstanceId>* l) { lst = l; };
    void next() { lst = lst->tl(); };
    GList<Alloc,ElType,InstanceId>* rest() const { return lst; };
    ElType& currEl() const { return lst->hd(); };
   private:
    GList<Alloc,ElType,InstanceId>* lst; 
  };

 private:
  ElType head;
  GList* tail;
  #ifdef DEBUG_ALLOC_OBJ_TYPE
   static ClassDesc _classDesc;
  #endif
}; // template <class Alloc,class ElType,class InstanceId> class GList

}; // namespace BK

#ifndef NO_DEBUG_VIS 
 template <class Alloc,class ElType,class InstanceId> 
 ostream& operator<<(ostream& str,const BK::GList<Alloc,ElType,InstanceId>& node) 
 {
  return node.output(str);
 };

 template <class Alloc,class ElType,class InstanceId> 
 ostream& operator<<(ostream& str,const BK::GList<Alloc,ElType,InstanceId>* list) 
 {
  while (list) 
   {
    str << *list;
    list = list->tl();
    if (list) str << " , ";
   };
  return str; 
 };
#endif

//======================================================================
#undef DEBUG_NAMESPACE
//======================================================================
#endif
