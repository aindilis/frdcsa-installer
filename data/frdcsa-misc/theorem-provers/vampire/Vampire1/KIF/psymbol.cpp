//
//  file psymbol.cpp
//  implements class PSymbol
//
//  21/05/2001 Manchester, made from name_table.cpp and functor.cpp
//  27/05/2001 Manchester, nth
//  30/05/2001 Manchester, createNew
//


#include <string.h>


#include "signature.h"
#include "int.h"


const int PSymbol::_noOfBuckets = 127;
PSymbol PSymbol::_equality ( "equal", 2 );
PSymbol PSymbol::_dummy;
PSymbol::List* PSymbol::_buckets [_noOfBuckets];
int PSymbol::_noOfSymbols = -1; // equality will have number -1


bool PSymbol::isEquality () const
{
  return this == &_equality;
}


PSymbol* PSymbol::create ( const char* str, int arity )
{
  // equality is treated in a special way
  if ( arity == 2 &&
       ! strcmp (str,"equal") ) {
    return equality ();
  }

  int bucket = hash ( str );

  for ( List* l = _buckets [ bucket ]; ! l->isEmpty (); l = l->tail() ) {
    if ( ! strcmp (str, l->head()->name() ) ) { // found
      return l->head ();
    }
  }

  char* copy = new char [strlen (str) + 1];

  strcpy ( copy, str );

  PSymbol* newf = new PSymbol (copy,arity);
  PSymbol::List::push ( newf, _buckets [bucket] );

  return newf;
} // PSymbol::create


// this constructor is just to initialize the name table 
PSymbol::PSymbol ()
{
  // initialize all buckets to empty
  for ( int i = _noOfBuckets - 1; i >= 0; i-- ) 
    _buckets [i] = PSymbol::List::empty ();
} // PSymbol::PSymbol


// very simple 
int PSymbol::hash ( const char* str )
{
  int result = 0;

  for ( const char* s = str ; *s != 0; s++ ) 
    result = ( result * 10 + *s ) % _noOfBuckets;
  
  return result;
} // PSymbol::hash



ostream& operator << ( ostream& str, const PSymbol* p )
{
  str << p->name ();

  return str;
} // operator << ( ostream& str, const PSymbol* p )


// return a new array sig of all symbols such that sig[i]
// is the symbol with the number i
PSymbol** PSymbol::signature () 
{
  PSymbol** sig = new PSymbol* [_noOfSymbols];

  for ( int i = _noOfBuckets - 1; i >= 0; i-- ) {
    PSymbol::List::Iterator ps ( _buckets [i] );
    while ( ps.more() ) {
      PSymbol* p = ps.next ();
      sig [p->number()] = p;
    }
  }

  return sig;
} // PSymbol::signature


// symbol with the number n
PSymbol* PSymbol::nth (int n) 
{
  for ( int i = _noOfBuckets - 1; i >= 0; i-- ) {
    PSymbol::List::Iterator ps ( _buckets [i] );
    while ( ps.more() ) {
      PSymbol* p = ps.next ();
      if ( p->number() == n )
        return p;
    }
  }
  ASSERT( debug_psymbol, false );

  return 0;
} // PSymbol::nth


// make a new function symbol starting with prefix
PSymbol* PSymbol::createNew ( const char* prefix, int arity ) 
{
  int len = strlen (prefix);
  char* name = new char [len + 10]; // assume 10^9 is enough
  strcpy (name, prefix);
  char* postfix = name + len; // postfix, part of name coming after prefix

  int post = 0; // number used for postfix

  loop:
  // write post as the postfix
  Int::toString (post++, postfix);
  // searching for the name
  int bucket = hash ( name );

  for ( List* l = _buckets [ bucket ]; ! l->isEmpty (); l = l->tail() ) {
    if ( ! strcmp (name, l->head()->name() ) ) { // found
      goto loop;
    }
  }
  // not found, insert it
  char* copy = new char [strlen (name) + 1];

  strcpy ( copy, name );
  delete [] name;

  PSymbol* newf = new PSymbol (copy,arity);
  PSymbol::List::push ( newf, _buckets [bucket] );

  return newf;
} // FSymbol::createNew



/*
// 08/06/2001
void PSymbol::arities ( int max, int* ars )
{
  PSymbol** sig = signature ();

  for ( int i = _noOfSymbols - 1; i >= 0; i-- ) {
    PSymbol* f = sig[i];
    ars [Int::min(max,f->arity ())] += 1;
  }

  delete [] sig;
} // FSymbol::arities
*/