//
//  file literal.cpp 
//  implements class Literal
//


#include "int.h"
#include "literal.h"
#include "atom.h"


ostream& operator << ( ostream& str , const Literal* l )
{
  // print sign
  if ( l->positive() ) // positive literal
    str << "++ ";
  else
    str << "-- ";

  // print atom
  str << l->atom ();

  return str;
} // operator << ( ostream& str , Literal* l )


// comparison of two literals, needed to normalize
// 26/06/2002 Manchester
Compare Literal::compare ( const Literal* l ) const  
{
  // negative literals are less than positive
  if ( negative() ) {
    if ( l->positive() )
      return LESS;
  }
  else { // positive
    if ( l->negative() )
      return GREATER;
  }

  // polarities are equal
  return atom()->compare(l->atom());
} // Literal::compare


// comparison of two literals, needed to normalize
// 26/06/2002 Manchester
bool Literal::isless ( const Literal* l ) const  
{
  switch ( compare(l) ) {
  case LESS:
    return true;

  case EQUAL:
  case GREATER:
    return false;
  }

  ASS( debug_literal, false);
} // Literal::isless


/*
int Literal::weight () 
{
  if ( _weight >= 0 )
    return _weight;

  _weight = atom()->weight ();

  return _weight;
} // Literal::weight


Literal::List* Literal::copy ( const List* ls )
{
  if ( ls->isEmpty () ) 
    return Literal::List::empty ();

  return new List ( ls->head()->copy(), copy (ls->tail()) );
} // Literal::copy (List*)


Literal::List* Literal::sort ( List* toSort, List* appendTo )
{
  if ( toSort->isEmpty() ) 
    return appendTo;

  if ( toSort->tail()->isEmpty() ) {
    toSort->tail ( appendTo );
    return toSort;
  }

  List* less;
  List* equal;
  List* greater;

  split ( toSort->head(), toSort->tail(), less, equal, greater );
  // sort greater and append to appendTo
  greater = sort ( greater, appendTo );
  // concatenate equal with it
  equal = Literal::List::concat ( equal, greater );
  // push head of list
  toSort->tail ( equal );
  // sort less and append to it
  return sort ( less, toSort );
} // Literal::sort


Literal::List* Literal::sort ( List* toSort )
{
  return sort ( toSort, Literal::List::empty() );
} // Literal::sort


void Literal::split ( Literal* l, List* spl, 
                     List*& less, List*& equal, List*& greater )
{
  ASS ( debug_clause, spl->isNonEmpty() );

  less = Literal::List::empty();
  equal = Literal::List::empty();
  greater = Literal::List::empty();

  for (;;) {
    List* tail = spl->tail ();

    switch ( spl->head()->compare(l) ) {
      case GREATER:
        spl->tail ( greater );
        greater = spl;
        break;
      case EQUAL:
        spl->tail ( equal );
        equal = spl;
        break;
      case LESS:
        spl->tail ( less );
        less = spl;
        break;
    }

    if ( tail->isEmpty() )
      return;
    spl = tail;
  }
} // Literal::split
*/

void Literal::destroy ()
{
  atom()->dec ();
  delete this;
} // Literal::destroy

/*
// replace the literal
//   f(X,y,Z) != f(X,u,Z)
// by 
//   i(f(X,y,Z),X,Z) = y
void Literal::introduceInverse ( int argNo, FSymbol* inv )
{
  // make literal into positive
  _sign = true;

  atom()->introduceInverse (argNo, inv);
} // Literal::introduceInverse


void Literal::splitLargeArity (const PSymbol* p, FSymbol::List* newFs )
{
  atom()->splitLargeArity (p, newFs);
} // Literal::splitLargeArity


void Literal::splitLargeArity (const FSymbol* f, FSymbol::List* newFs )
{
  atom()->splitLargeArity (f, newFs);
} // Literal::splitLargeArity


Literal* Literal::negativeEqualitySplit ( int thresholdWeight )
{
  if ( positive() ) // only negative equalities can be split
    return 0;

  Atom* a = atom()->negativeEqualitySplit (thresholdWeight);

  return a ? new Literal ( true, a ) : 0 ; 
} // Literal::negativeEqualitySplit
*/
