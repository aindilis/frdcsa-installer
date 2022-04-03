//
// File:         FIFO.h
// Description:  Queues of unrestricted capacity.
// Created:      Feb 16, 2000, 14:35
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
// Revised:      Apr 17, 2001, a bug fixed in removeFirst, AR
//============================================================================
#ifndef FIFO_H
//=============================================================================
#define FIFO_H 
#include "jargon.h"
#include "BlodKorvDebugFlags.h"
#include "GList.h"
#include "DestructionMode.h"
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FIFO
 #define DEBUG_NAMESPACE "FIFO<class Alloc,class ElType,class InstanceId>"
#endif
#include "debugMacros.h"
//============================================================================
namespace BK 
{

template <class Alloc,class ElType,class InstanceId>
class FIFO
{
 public:
  class Entry
  {
   public:    
    const ElType& value() const { return ((const List*)this)->hd(); };
    ElType& value() { return ((List*)this)->hd(); }; 
    const Entry* next() const { return (const Entry*)(((const List*)this)->tl()); };
    Entry* next() { return (Entry*)(((List*)this)->tl()); };
  };
 public:
  FIFO() : _first((List*)0), _last((List*)0) 
  {
   CALL("constructor FIFO()"); 
  }; 
  ~FIFO() 
    { 
      CALL("destructor ~FIFO()"); 
      if (DestructionMode::isThorough()) destroyThoroughly(); 
    };
  void destroyThoroughly() 
    { 
      List::destroyList(_first);
      _first = (List*)0; 
      _last = (List*)0;      
    }; 
  void reset() 
  { 
    destroyThoroughly();
  };
  operator bool() const { return _last; };
  bool empty() const { return !_last; };
  bool nonempty() const { return _last; };
  ulong count() const
  {
   ulong res = 0;
   for (const Entry* e = begin(); e; e = e->next()) res++;
   return res;
  };

  bool contains(const ElType& x) const
    {
      CALL("contains(const ElType& x) const");
      for (const Entry* e = begin(); e; e = e->next())
	if (e->value() == x) return true;      
      return false;
    };

  void enqueue(const ElType& x)
  {
   CALL("enqueue(const ElType& x)");
   if (!_last) // empty queue
    {
     ASSERT(!_first);
     _first = (_last = new List(x,(List*)0));
     ASSERT(_first);
    }
   else // nonempty queue
    {
     ASSERT(_first);
     _last->tl() = new List(x,(List*)0);
     _last = _last->tl();
     ASSERT(_last);
    };
  }; // void enqueue(const ElType& x)

  bool dequeue(ElType& x)
  {
   CALL("dequeue(ElType& x)");
   if (_first)
    { 
     ASSERT(_last);
     x = _first->hd();
     List* tmp = _first->tl();    
     delete _first;
     _first = tmp;
     if (!_first) _last = (List*)0; // was only one element
     return true;
    };
   // empty queue
   ASSERT(!_last);
   return false;
  }; // bool dequeue(ElType& x)

  const Entry* begin() const { return (const Entry*)_first; };
  Entry* begin() { return (Entry*)_first; };

  void removeFirst(const ElType& x)
  {
   CALL("removeFirst(const ElType& x)");
   if (_first)
    {
     if (_first->hd() == x) 
      {      
       List* tmp = _first->tl(); 
       delete _first;
       _first = tmp;
       if (!_first) _last = (List*)0;
      }
     else
      {
       List* prev = _first;
       List* curr = _first->tl();
       while (curr) 
	{
	 if (curr->hd() == x)
	  {
	   prev->tl() = curr->tl();
           // if (_last == curr) _last = (List*)0; // bug! 
           if (_last == curr) _last = prev;
           delete curr;         
           return; 
          };
         prev = curr;
         curr = curr->tl(); 
        };
      };
    };   
  }; // void removeFirst(const ElType& x)

  #ifndef NO_DEBUG
   bool checkIntegrity() const
   { 
     return (_first && _last) || ((!_first) && (!_last));  
   };
  #endif
 private:
  typedef GList<Alloc,ElType,InstanceId> List;
 private:
  FIFO(const FIFO&) {}; 
 private:
  List* _first;
  List* _last;
}; // template <class Alloc,class ElType,class InstanceId> class FIFO

}; // namespace BK


//======================================================================
#undef DEBUG_NAMESPACE
//======================================================================
#endif
