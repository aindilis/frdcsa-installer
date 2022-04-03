//
// File:         GIncCountingSort.h
// Description:  Incremental version of sorting by counting comparisons.
// Created:      Feb 13, 2000, 16:30
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//============================================================================
#ifndef G_INC_COUNTING_SORT_H
//=============================================================================
#define G_INC_COUNTING_SORT_H
#ifndef NO_DEBUG_VIS
 #include <iostream.h>
#endif
#include "jargon.h"
#include "BlodKorvDebugFlags.h"
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_G_INC_COUNTING_SORT
 #define DEBUG_NAMESPACE "GIncCountingSort<KeyType,ElType,MaxNumOfElements>"
#endif
#include "debugMacros.h"
//============================================================================
namespace BK 
{

template <class KeyType,class ElType,unsigned long MaxNumOfElements>
class GIncCountingSort
{
 public:
  GIncCountingSort() { init(); };
  void init() { _next = 0UL; };
  ~GIncCountingSort() {};
  void reset() 
  {
   _next = 0UL;
  };
  ElType* next() { return _elem + _next; };
  void push()
  {
   CALL("push()");
   ASSERT(_next < MaxNumOfElements);
   KeyType key = _elem[_next].key();
   unsigned long num = 0;
   for (unsigned long i = 0; i < _next; i++)
    if (_elem[i].key() < key) { num++; }
    else _num[i]++;
   _num[_next] = num;
   _next++;     
  }; 
  void push(const ElType& el)
  {
   CALL("push(const KeyType& key)");
   ASSERT(_next < MaxNumOfElements);
   _elem[_next] = el;
   KeyType key = el.key();
   unsigned long num = 0;
   for (unsigned long i = 0; i < _next; i++)
    if (_elem[i].key() < key) { num++; }
    else _num[i]++;
   _num[_next] = num;
   _next++; 
  };
  const ElType* elem() const { return _elem; };
  const unsigned long* num() const { return _num; };
  unsigned long size() const { return _next; };
  #ifndef NO_DEBUG_VIS
   ostream& output(ostream& str) const 
   {
    for (unsigned long i = 0; i < _next; i++) str << _elem[i].key() << " [" << _num[i] << "] "; 
    return str << '\n';
   };
  #endif
 private:
  ElType _elem[MaxNumOfElements];
  unsigned long _num[MaxNumOfElements];
  unsigned long _next;
}; // template <class KeyType,class ElType,unsigned long MaxNumOfElements> class GIncCountingSort

}; // namespace BK

//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_G_INC_COUNTING_SORT
 #define DEBUG_NAMESPACE "GSimpleIncCountingSort<KeyType,MaxNumOfElements>"
#endif
#include "debugMacros.h"
//============================================================================
namespace BK 
{

template <class KeyType,unsigned long MaxNumOfElements>
class GSimpleIncCountingSort
{
 public:
  GSimpleIncCountingSort() { init(); };
  void init() { _next = 0UL; };
  void reset() 
  {
   _next = 0UL;
  };
  KeyType* next() { return _key + _next; };
  void push()
  {   
   CALL("push()");
   ASSERT(_next < MaxNumOfElements);
   KeyType key = _key[_next];
   unsigned long num = 0;
   for (unsigned long i = 0; i < _next; i++)
    if (_key[i] < key) { num++; }
    else _num[i]++;
   _num[_next] = num;
   _next++;    
  }; 
  void push(const KeyType& key)
  {
   CALL("push(const KeyType& key)");
   ASSERT(_next < MaxNumOfElements);
   _key[_next] = key;
   unsigned long num = 0;
   for (unsigned long i = 0; i < _next; i++)
    if (_key[i] < key) { num++; }
    else _num[i]++;
   _num[_next] = num;
   _next++;   
  };
  const unsigned long* num() const { return _num; };
  unsigned long size() const { return _next; }; 
  #ifndef NO_DEBUG_VIS
   ostream& output(ostream& str) const 
   {
    for (unsigned long i = 0; i < _next; i++) str << _key[i] << " [" << _num[i] << "] "; 
    return str << '\n';
   };
  #endif
 private:
  KeyType _key[MaxNumOfElements];
  unsigned long _num[MaxNumOfElements];
  unsigned long _next;
}; // template <class KeyType,unsigned long MaxNumOfElements> class GSimpleIncCountingSort

}; // namespace BK
//============================================================================
#undef DEBUG_NAMESPACE
//============================================================================
#endif
