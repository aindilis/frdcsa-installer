//
//  file pure.cpp
//  implements class Pure: auxiliary class for defining pure predicates
//
//  27/05/2001 Manchester
//


#include "pure.h"
#include "clause.h"


Pure::Pure ( int noOfSymbols )
  :
  _noOfSymbols (noOfSymbols),
  _remaining (noOfSymbols),
  _symbols ( new Symbol [noOfSymbols] )
{
  // number of symbols must be non-zero
  ASSERT( debug_pure, noOfSymbols );  
} // Pure::Pure


Pure::Symbol::Symbol ()
  :
  pos (false),
  neg (false)
{
} // Pure::Symbol::Symbol


Pure::~Pure ()
{
  delete [] _symbols;
} // Pure::Pure


bool Pure::getClause (Clause* c)
{
  Literal::List::Iterator ls ( c->literals() );
  while ( ls.more() ) {
    Literal* l = ls.next();
    Atom* atom = l->atom();
    if ( atom->isEquality() )
      continue;
    Symbol& s = _symbols[atom->functor()->number()];

    if ( s.pos && s.neg ) { // s is not a candidate anyhow
      continue;
    }

    if ( l->positive() ) {
      s.pos = true;
    }
    else {
      s.neg = true;
    }

    if ( s.pos && s.neg ) { // s is not a candidate any more
      _remaining --;
      if ( _remaining == 0 ) { // no remaining candidate symbols
        return false;
      }
    }
  }

  return true;
} // Pure::getClause


ostream& operator << ( ostream& str, const Pure& res )
{
  PSymbol** signature = PSymbol::signature ();
  for ( int i = res._noOfSymbols - 1; i >= 0; i-- ) {
    if ( ! res._symbols[i].pos || ! res._symbols[i].neg ) {
      const char* diagnosis;
      if ( res._symbols[i].pos ) {
        diagnosis = "pure positive";
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
} // operator << (ostream&, const Pure&)


Pure::Iterator::Iterator ( Pure& pure )
  :
  _signature ( PSymbol::signature() ),
  _symbols ( pure.symbols() ),
  _nextSymbol ( PSymbol::noOfSymbols() - 1 )
{
} // Pure::Iterator::Iterator
 

Pure::Iterator::~Iterator ()
{
  delete [] _signature;
} // Pure::Iterator::~Iterator
 

PSymbol* Pure::Iterator::next ()
{
  while ( _nextSymbol >= 0 ) {
    Symbol& s = _symbols [_nextSymbol];
    if ( s.pos == ! s.neg ) { // 
      return _signature [_nextSymbol--];
    }
    // no occurrences
    _nextSymbol--;
  }

  return 0;
} // Pure::Iterator::Iterator

