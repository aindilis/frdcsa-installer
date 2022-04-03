//
// File:         ObjectPool.h
// Description:  Pools of reusable objects. 
// Created:      May 22, 2002, Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//==========================================================================
#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H
//==========================================================================
#include "jargon.h"
#include "BlodKorvDebugFlags.h"
#include "DestructionMode.h"
//==========================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_OBJECT_POOL
 #define DEBUG_NAMESPACE "ObjectPool<class Alloc,class C>"
#endif
#include "debugMacros.h"
//==========================================================================

namespace BK 
{

template <class Alloc,class C>
class ObjectPool
{
 public:
  class AutoObject
    {
    public:
      AutoObject(ObjectPool& pool) 
	{
	  CALL("AutoObject::constructor AutoObject(ObjectPool& pool)");
	  _pool = &pool;
	  _object = pool.reserveObject();
	};
      ~AutoObject() 
	{
	  CALL("AutoObject::destructor ~AutoObject()");
	  if (DestructionMode::isThorough()) 
	    _pool->releaseObject(_object);
	}; 
      C& object() { return *_object; };
      const C& object() const { return *_object; };
    private:
      AutoObject() 
	{ 
	  CALL("AutoObject::constructor AutoObject()"); 
	  ICP("ICP0");
	};
    private:
      C* _object;
      ObjectPool* _pool;
    }; // class AutoObject
 public:
  ObjectPool() { init(); };
  ~ObjectPool() 
    {
      if (DestructionMode::isThorough()) destroyThoroughly();
    };
  void init() 
    { 
      _allObjects = (Wrapper*)0;
      _lastReserved = (Wrapper*)0;
    };
  
  void destroyThoroughly();
  void reset();
  C* reserveObject();
  void releaseObject(C* obj);

 private:
  class Wrapper : public C
    {
    public:
      Wrapper() : 
#ifdef DEBUG_NAMESPACE
	debugIsReserved(false),
#endif
	_previous((Wrapper*)0), 
	_next((Wrapper*)0)
	{
	};
      ~Wrapper() {};
      Wrapper*& next() { return _next; };
      Wrapper*& previous() { return _previous; };
      void* operator new(size_t)
	{
	  return Alloc::allocate(sizeof(Wrapper));
	};
      void operator delete(void* obj)
	{
	  Alloc::deallocate(obj,sizeof(Wrapper));
	};
#ifdef DEBUG_NAMESPACE
    public:
      bool debugIsReserved;
#endif
    private:
      Wrapper* _previous;
      Wrapper* _next;
    };
  
 private:
  Wrapper* _allObjects;
  Wrapper* _lastReserved;
}; // class ObjectPool<class Alloc,class C>

}; // namespace BK

//==========================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_OBJECT_POOL
#define DEBUG_NAMESPACE "ObjectPool<class Alloc,class C>"
#endif
#include "debugMacros.h"
//==========================================================================

namespace BK 
{
template <class Alloc,class C>  
inline  
void ObjectPool<Alloc,C>::destroyThoroughly()
{
  CALL("destroyThoroughly()");
  while (_allObjects)
    {
      Wrapper* tmp = _allObjects;
      _allObjects = _allObjects->next();
      delete tmp;
    };
  _lastReserved = (Wrapper*)0;
};// void destroyThoroughly()

template <class Alloc,class C>
inline
void ObjectPool<Alloc,C>::reset()
{
  CALL("reset()");
     
#ifdef DEBUG_NAMESPACE
  if (_lastReserved)
    {
      Wrapper* obj = _allObjects;
      while (obj != _lastReserved)
	{
	  ASSERT(obj->debugIsReserved);
	  obj->debugIsReserved = false;
	  obj = obj->next();
	};
      ASSERT(obj->debugIsReserved);
      obj->debugIsReserved = false;
    };
#endif
  _lastReserved = (Wrapper*)0;
}; // void reset()
 
template <class Alloc,class C>
inline
C* ObjectPool<Alloc,C>::reserveObject()
{
  CALL("reserveObject()");
  Wrapper* res;
  if (_lastReserved)
    {
      res = _lastReserved->next();
      if (!res)
	{
	  res = new Wrapper();
	  res->previous() = _lastReserved;
	  _lastReserved->next() = res;
	};
    }
  else
    {
      if (!_allObjects) _allObjects = new Wrapper();
      res = _allObjects;
      ASSERT(!res->previous());
    };
  _lastReserved = res;
  DOP(res->debugIsReserved = true);
  return (C*)res;
}; // C* reserveObject()
  

template <class Alloc,class C>
inline
void ObjectPool<Alloc,C>::releaseObject(C* obj)
{
  CALL("releaseObject(C* obj)");
  ASSERT(((Wrapper*)obj)->debugIsReserved);
  DOP(((Wrapper*)obj)->debugIsReserved = false);
  if (((Wrapper*)obj) == _lastReserved)
    {
      _lastReserved = _lastReserved->previous();
      return;
    };
   
  Wrapper* prev = ((Wrapper*)obj)->previous();
  Wrapper* nxt = ((Wrapper*)obj)->next();
  ASSERT(nxt);
      
  ((Wrapper*)obj)->previous() = _lastReserved;
  ((Wrapper*)obj)->next() = _lastReserved->next();
  _lastReserved->next() = ((Wrapper*)obj);
  ((Wrapper*)obj)->next()->previous() = ((Wrapper*)obj);    
      
  if (prev)
    {
      prev->next() = nxt;
    }
  else
    {
      ASSERT(_allObjects == ((Wrapper*)obj));
      _allObjects = nxt;
    };
  nxt->previous() = prev;
}; // void releaseObject(C* obj)

}; // namespace BK


//==========================================================================
#endif
