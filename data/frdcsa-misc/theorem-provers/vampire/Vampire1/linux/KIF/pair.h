// *************************************************************
//
//  Class Pair<C,D> for implementing pairs
//
//  14/04/2000 Breckenridge
//  02/06/2000 Manchester, == added
//
// *************************************************************

#ifndef __pair__
#  define __pair__


#include "assert.h"
#include "memory.h"


// *******************Class Pair*******************************


template <class C, class D, ClassID cid>
class Pair
#   if debug_memory
    : public Memory <cid>
#   endif
{
 public:

  // constructors
  inline
  Pair ( C left, D right )
    :
    _left ( left ),
    _right ( right ) {}

  inline
  Pair () {}

  // structure quering
  inline C left () const { return _left; }
  inline D right () const { return _right; }

  // change left/right elements
  inline void left ( C lft ) { _left = lft; }
  inline void right ( D rht ) { _right = rht; }

  // equality
  inline 
  bool operator == ( Pair& rhs ) const 
    { return left() == rhs.left() &&
             right() == rhs.right(); }

 protected:  // structure
  C _left;
  D _right;
};  // class Pair


// ******************* Class Pair, definitions ****************************


#endif


