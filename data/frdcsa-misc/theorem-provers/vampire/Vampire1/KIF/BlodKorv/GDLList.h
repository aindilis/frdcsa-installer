//
// File:         GDLList.h
// Description:  Generic double-linked lists.
// Created:      Oct 07, 2000, 19:30 
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//============================================================================
#ifndef G_D_L_LIST_H
//=============================================================================
#define G_D_L_LIST_H 
#ifndef NO_DEBUG_VIS
 #include <iostream.h>
#endif
#include "jargon.h"
#include "BlodKorvDebugFlags.h"
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_G_D_L_LIST
 #define DEBUG_NAMESPACE "GDLList<class StatAlloc,class ElType>"
#endif
#include "debugMacros.h"
//============================================================================
namespace BK 
{

template <class Alloc,class ElType>
class GDLList
{
 public:
  GDLList() {};
  GDLList(GDLList* aft) : _after(aft), _before((GDLList*)0) {};
  GDLList(GDLList* bef,GDLList* aft) : _after(aft), _before(bef) {};
  GDLList(const ElType& v) : _after((GDLList*)0), _before((GDLList*)0) { val() = v; };
  GDLList(const ElType& v,GDLList* aft) : _after(aft), _before((GDLList*)0) { val() = v; };
  GDLList(const ElType& v,GDLList* bef,GDLList* aft) : _after(aft), _before(bef) { val() = v; };  

  ~GDLList() { CALL("destructor ~GDLList()"); }; 
  void* operator new(size_t) { return Alloc::allocate(sizeof(GDLList<Alloc,ElType>)); };
  void  operator delete(void* obj) 
  { 
   CALL("operator delete(void* obj)");
   Alloc::deallocate(obj,sizeof(GDLList<Alloc,ElType>)); 
  };

  ElType& val() { return *((ElType*)_memory); };
  const ElType& val() const { return *((const ElType*)_memory); };
  GDLList*& after() { return _after; };
  GDLList* const & after() const { return _after; };
  GDLList*& before() { return _before; };
  GDLList* const & before() const { return _before; };

  #ifndef NO_DEBUG_VIS   
   ostream& output(ostream& str) const { return str << val(); };
  #endif

 private: 
  char _memory[sizeof(ElType)];
  GDLList* _after;
  GDLList* _before;  
}; // template <class Alloc,class ElType> class GDLList


}; // namespace BK

#ifndef NO_DEBUG_VIS 
 template <class Alloc,class ElType> 
 ostream& operator<<(ostream& str,const BK::GDLList<Alloc,ElType>& node) 
 {
  return node.output(str);
 };

 template <class Alloc,class ElType> 
 ostream& operator<<(ostream& str,const BK::GDLList<Alloc,ElType>* list) 
 {
  CALL("operator<<(ostream& str,const BK::GDLList<Alloc,ElType>* list)");
  if (!list) return str; 
  const GDLList<Alloc,ElType>* lst = list->before();
  if (lst) 
   {
    while (lst->before()) lst = lst->before();
    ASSERT(lst && lst->after());
    do
     {
      str << *lst << " <- ";
      lst = lst->after(); 
      ASSERT(lst && lst->after()); 
     }
    while (lst != list);
   };
  str << "[ " << *list << " ]";
  lst = list->after();
  while (lst) 
   {
    str << " -> " << *lst;
    lst = lst->after(); 
   };
  return str; 
 };
#endif

//======================================================================
#undef DEBUG_NAMESPACE
//======================================================================
#endif
