//
// File int.h
//
// Implements several functions on integers
//
//  990806 Uppsala
//  990928 Uppsala, min added
//  19/02/2000 Manchester, slightly reimplemented
//  09/06/2000 Manchester, toString added and int.cpp created
//


#ifndef __int__
#define __int__


#include <limits.h>
#include "list.h"


enum Compare 
{
  LESS = -1,
  EQUAL = 0,
  GREATER = 1
};


class Int 
{
 public:

  static int max ( int i1, int i2 );
  static int min ( int i1, int i2 );
  static Compare compare ( int i1, int i2 );
  static const int max ();
  static const int min ();
  static void toString ( int i, char* str );

  typedef List<int,CID_INTLIST> List;
};


inline 
int Int::max ( int i1, int i2 ) 
{
  return  i1 > i2 ? i1 : i2 ;
}


inline 
int Int::min ( int i1, int i2 ) 
{
  return  i1 < i2 ? i1 : i2 ;
}


inline 
Compare Int::compare ( int i1, int i2 ) 
{
  if ( i1 > i2 )
    return GREATER;

  if ( i1 == i2 )
    return EQUAL;

  return LESS;
} // Int::compare


inline
const int Int::max ()
{
  return INT_MAX;
}


inline
const int Int::min ()
{
  return INT_MIN;
}


#endif  // __int__

