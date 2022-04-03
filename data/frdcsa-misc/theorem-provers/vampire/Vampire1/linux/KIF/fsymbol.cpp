//
//  file fsymbol.cpp
//  implements class FSymbol
//
//  21/05/2001, Manchester, made from name_table.cpp and functor.cpp
//  30/05/2001 Manchester, signature()
//


#include <string.h>


#include "signature.h"
#include "int.h"


/*
const int FSymbol::_noOfBuckets = 127;
FSymbol FSymbol::_dummy;
FSymbol::List* FSymbol::_buckets [_noOfBuckets];
int FSymbol::_noOfSymbols = 0;


FSymbol* FSymbol::create ( const char* str, int arity )
{
  int bucket = hash ( str );

  for ( List* l = _buckets [ bucket ]; ! l->isEmpty (); l = l->tail() ) {
    if ( ! strcmp (str, l->head()->name() ) ) { // found
      return l->head ();
    }
  }

  char* copy = new char [strlen (str) + 1];

  strcpy ( copy, str );

  FSymbol* newf = new FSymbol (copy,arity);
  FSymbol::List::push ( newf, _buckets [bucket] );

  return newf;
} // FSymbol::create


// make a new function symbol starting with prefix
FSymbol* FSymbol::createNew ( const char* prefix, int arity ) 
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

  FSymbol* newf = new FSymbol (copy,arity);
  FSymbol::List::push ( newf, _buckets [bucket] );

  return newf;
} // FSymbol::createNew


// this constructor is just to initialize the name table 
FSymbol::FSymbol ()
{
  // initialize all buckets to empty
  for ( int i = _noOfBuckets - 1; i >= 0; i-- ) 
    _buckets [i] = FSymbol::List::empty ();
} // FSymbol::FSymbol


// very simple 
int FSymbol::hash ( const char* str )
{
  int result = 0;

  for ( const char* s = str ; *s != 0; s++ ) 
    result = ( result * 10 + *s ) % _noOfBuckets;
  
  return result;
} // FSymbol::hash


ostream& operator << ( ostream& str, const FSymbol* f )
{
  str << f->name ();

  return str;
} // operator << ( ostream& str, const FSymbol* f )


// return a new array sig of all symbols such that sig[i]
// is the symbol with the number i
FSymbol** FSymbol::signature () 
{
  FSymbol** sig = new FSymbol* [_noOfSymbols];

  for ( int i = _noOfBuckets - 1; i >= 0; i-- ) {
    FSymbol::List::Iterator ps ( _buckets [i] );
    while ( ps.more() ) {
      FSymbol* p = ps.next ();
      sig [p->_number] = p;
    }
  }

  return sig;
} // FSymbol::signature

// 08/06/2001
void FSymbol::arities ( int max, int* ars )
{
  FSymbol** sig = signature ();

  for ( int i = _noOfSymbols - 1; i >= 0; i-- ) {
    FSymbol* f = sig[i];
    ars [Int::min(max,f->arity ())] += 1;
  }

  delete [] sig;
} // FSymbol::arities
*/