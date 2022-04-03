// *************************************************************
//
//  Class Triple<C,D,E> for implementing triples
//
//  20/04/2000 Train London-Manchester
//
// *************************************************************

#ifndef __triple__
#  define __triple__


#include "assert.h"
#include "debug.h"


// *******************Class Triple*******************************


template <class C, class D, class E>
class Triple
{
 public:

  // constructors
  Triple ( C fst, D snd, E thrd );
  Triple ();

  // structure quering
  C first ();
  D second ();
  E third ();

  // change elements
  void first ( C );
  void second ( D );
  void third ( E );

  // memory management
//  void* operator new ( size_t );
//  void operator delete ( void* obj );

 protected:  // structure

  C _first;
  D _second;
  E _third;
  // for debugging 
  static const char* name ();
};  // class Triple


// ******************* Class Pair, definitions ****************************


template <class C,class D,class E>
inline
const char* Triple<C,D,E>::name ()
{
  return "Triple";
}


//template <class C,class D,class E>
//inline
//void* Triple<C,D,E>::operator new (size_t)
//{
//  return Memory::alloc ( sizeof(Triple<C,D,E>), name () );
//}
//
//
//template <class C,class D,class E>
//inline
//void Triple<C,D,E>::operator delete (void* obj)
//{
//  Memory::dealloc ( obj, sizeof (Triple<C,D,E>), name () );
//}


template <class C,class D,class E>
inline
Triple<C,D,E>::Triple ()
{
}


template <class C,class D,class E>
inline
Triple<C,D,E>::Triple (C fst, D snd, E thrd)
  :
  _first ( fst ),
  _second ( snd ),
  _third ( thrd )
{
}


template <class C,class D,class E>
inline
C Triple<C,D,E>::first ()
{
  return _first;
}


template <class C,class D,class E>
inline
D Triple<C,D,E>::second ()
{
  return _second;
}


template <class C,class D,class E>
inline
E Triple<C,D,E>::third ()
{
  return _third;
}


template <class C,class D,class E>
inline
void Triple<C,D,E>::first ( C fst )
{
  _first = fst;
}


template <class C,class D,class E>
inline
void Triple<C,D,E>::second ( D snd )
{
  _second = snd;
}


template <class C,class D,class E>
inline
void Triple<C,D,E>::third ( E third )
{
  _third = thrd;
}


#endif

