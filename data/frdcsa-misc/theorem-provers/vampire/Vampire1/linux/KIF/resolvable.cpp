//
//  file resolvable.cpp
//  implements class Resolvable: auxiliary class for defining resolvable predicates
//
//  26/05/2001 Manchester
//


#include "resolvable.h"
#include "clause.h"


Resolvable::Resolvable ( int noOfSymbols )
  :
  _noOfSymbols (noOfSymbols),
  _remaining (noOfSymbols),
  _symbols ( new Symbol [noOfSymbols] )
{
  // number of symbols must be non-zero
  ASSERT( debug_resolvable, noOfSymbols );  
} // Resolvable::Resolvable


Resolvable::Symbol::Symbol ()
  :
  pres (true),
  nres (true),
  pos (false),
  neg (false)
{
} // Resolvable::Symbol::Symbol


Resolvable::~Resolvable ()
{
  delete [] _symbols;
} // Resolvable::Resolvable


bool Resolvable::getClause (Clause* c)
{
  for ( int i = _noOfSymbols - 1; i >= 0; i-- ) {
    _symbols[i].occurs = false;
  }
  bool unit = (c->isUnit());

  Literal::List::Iterator ls ( c->literals() );
  while ( ls.more() ) {
    Literal* l = ls.next ();
    Atom* atom = l->atom();
    if ( atom->isEquality() )
      continue;
    int num = atom->functor()->number();
    Symbol& s = _symbols[num];

    if ( ! (s.pres || s.nres) ) { // s is not a candidate anyhow
      continue;
    }

    if ( s.occurs ) { // second occurrence
      goto not_a_candidate;
    }

    // first occurrence of s
    // mark that there was an occurrence
    s.occurs = true;
    if ( l->positive() ) {
      if ( s.pos ) { // second positive occurrence, can't be negatively resolvable
        if ( ! s.pres ) {
          goto not_a_candidate;
        }
        s.nres = false;
      }
      else { 
        s.pos = true;
      }
      if ( s.pres &&   // was positively resolvable
           ! unit) {   // but now is not unit
        s.pres = false;
      }
      if ( s.nres &&                      // was negatively resolvable
           ! atom->isLinearAndFlat() ) {  // but now is not linear and flat
        s.nres = false;
      }
    }
    else { // l is negative
      if ( s.neg ) { // second negative occurrence, can't be negatively resolvable
        if ( ! s.nres ) {
          goto not_a_candidate;
        }
        s.pres = false;
      }
      else { 
        s.neg = true;
      }
      if ( s.nres &&   // was negatively resolvable
           ! unit) {   // but now is not unit
        s.nres = false;
      }
      if ( s.pres &&                      // was negatively resolvable
           ! atom->isLinearAndFlat() ) {  // but now is not linear and flat
        s.pres = false;
      }
    }

    // check that it is still a candidate
    if ( s.nres || s.pres ) 
      continue;

    // not a candidate
    not_a_candidate:
    _remaining --;
    if ( _remaining == 0 ) { // no remaining candidate symbols
      return false;
    }
    s.pres = false;
    s.nres = false;
  }

  return true;
} // Resolvable::getClause


//  return the first resolvable symbol
//  sign is true if the flat and linear one is positive
PSymbol* Resolvable::first ( bool& sign ) const
{
  for ( int i = _noOfSymbols - 1; i >= 0; i-- ) {
    Symbol& s = _symbols [i];
    if ( s.pos && s.neg && (s.pres || s.nres) ) {
      sign = s.nres;
      return PSymbol::nth (i);
    }
  }

  return 0;
} // Resolvable::first


ostream& operator << ( ostream& str, const Resolvable& res )
{
  PSymbol** signature = PSymbol::signature ();
  for ( int i = res._noOfSymbols - 1; i >= 0; i-- ) {
    if ( res._symbols[i].pres || res._symbols[i].nres ) {
      const char* diagnosis;
      if ( res._symbols[i].pos ) {  // there is a positive occurrence
        if ( res._symbols[i].neg ) { // both positive and negative occurrences
          diagnosis = "resolvable";
        }
        else {
          diagnosis = "pure positive";
        }
      }
      else if ( res._symbols[i].neg ) { // negative occurrence only
        diagnosis = "pure negative";
      }
      else {
        diagnosis = "no occurrences";
      }
      str << "  " << signature[i] << " (" << diagnosis << ")\n";
    }
  }

  delete [] signature;

  return str;
} // operator << (ostream&, const Resolvable&)


