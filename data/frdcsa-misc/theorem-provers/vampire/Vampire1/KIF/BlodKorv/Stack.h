//
// File:         Stack.h
// Description:  Simple fixed-size stacks.
// Created:      Sep 24, 1999, 16:30
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk 
//=======================================================================
#ifndef STACK_H
//=======================================================================
#define STACK_H
#include "jargon.h"
#include "BlodKorvDebugFlags.h"
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_STACK
 #define DEBUG_NAMESPACE "Stack<class ElementT,ulong maxSize>"
#endif
#include "debugMacros.h"
//======================================================================
namespace BK 
{


template <class ElementT,ulong maxSize>
class Stack
{
 public:
  Stack() : cursor(((ElementT*)memory)) 
  {
   #ifdef DEBUG_STACK
    name = "?";
   #endif
  };
  Stack(const char* nm) 
   : cursor(((ElementT*)memory)) 
  {
   #ifdef DEBUG_STACK
    name = nm;
   #endif
  };
  Stack(const Stack& s) 
   : cursor(((ElementT*)memory))
  {
   for (ElementT* p = (ElementT*)(s.memory); p < s.cursor; p++) push(*p); 
   #ifdef DEBUG_STACK
    name = "?";
   #endif        
  }; 
  Stack(const Stack& s,const char* nm) 
   : cursor(((ElementT*)memory))
  {
   for (ElementT* p = (ElementT*)(s.memory); p < s.cursor; p++) push(*p); 
   #ifdef DEBUG_STACK
    name = nm;
   #endif        
  };
  ~Stack() { CALLM(name,"destructor ~Stack()"); };
  Stack& operator=(const Stack& s) { copy(s); return *this; };     
  
  void init() 
  {
   #ifdef DEBUG_STACK
    name = "?";
   #endif 
   cursor = ((ElementT*)memory); 
  };
  void init(const char* nm) 
  { 
   #ifdef DEBUG_STACK
    name = nm;
   #endif   
   cursor = ((ElementT*)memory); 
  };
  void reset() { cursor = ((ElementT*)memory); };
  void copy(const Stack& s) 
  { 
   reset();
   for (ElementT* p = (ElementT*)(s.memory); p < s.cursor; p++) push(*p); 
  }; 

  operator bool() const { return cursor != ((ElementT*)memory); };
  bool empty() const { return cursor ==  ((ElementT*)memory); };
  bool nonempty() const { return cursor != ((ElementT*)memory); };
  bool full() const { return size() == maxSize; };
  bool nonfull() const { return size() < maxSize; };

  
  bool roomFor(unsigned  long n) const 
  {
   return size() + n <= maxSize;   
  };
 

  ulong size() const { return (cursor - ((ElementT*)memory)); }; 
  
  ElementT* begin() { return ((ElementT*)memory); };
  ElementT* end() { return cursor; };
  ElementT* last() { return cursor - 1; };
  const ElementT* begin() const { return ((ElementT*)memory); };
  const ElementT* end() const { return cursor; };
  const ElementT* last() const { return cursor - 1; };
  ElementT& top() 
  {
   CALLM(name,"(ElementT& top())"); 
   ASSERT(cursor > (ElementT*)memory);
   return *(cursor-1); 
  };
  const ElementT& top() const 
  {
   CALLM(name,"(const ElementT& top())"); 
   ASSERT(cursor > (ElementT*)memory);
   return *(cursor-1); 
  };
  ElementT& next() 
  {
   CALLM(name,"(ElementT& next())");
   ASSERT(nonfull());
   return *cursor; 
  };
  const ElementT& next() const 
  {
   CALLM(name,"(const ElementT& next())");
   ASSERT(nonfull()); 
   return *cursor; 
  };
  void push(const ElementT& el) 
  {
   CALLM(name,"push(const ElementT& el)");
   ASSERT(nonfull());
   *cursor = el; 
   cursor++; 
  };
  void push() 
  { 
   CALLM(name,"push()");
   ASSERT(nonfull());
   cursor++; 
  };
  const ElementT& pop() 
  {
   CALLM(name,"const ElementT& pop()");
   ASSERT(nonempty());
   cursor--; 
   return *cursor; 
  };
  void skipTop() 
  {
   CALLM(name,"skipTop()");
   ASSERT(nonempty());
   cursor--; 
  };
  const ElementT& nth(ulong n) const
  {
   return ((const ElementT*)memory)[n]; 
  };
  ElementT& nth(ulong n)
  {
   return ((ElementT*)memory)[n]; 
  };   
 private:
  char memory[sizeof(ElementT)*maxSize];
  ElementT* cursor;  
  #ifdef DEBUG_NAMESPACE
   const char* name;
  #endif   
}; // template <class ElementT,ulong maxSize> class Stack
}; // namespace BK

//=========================================================================
#undef DEBUG_NAMESPACE
//=========================================================================
#endif
