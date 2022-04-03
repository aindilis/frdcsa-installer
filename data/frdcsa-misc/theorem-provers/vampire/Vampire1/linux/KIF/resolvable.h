//
//  file resolvable.h 
//  defines class Resolvable: auxiliary class for defining resolvable predicates
//
//  26/05/2001 Manchester
//

#ifndef __resolvable__
#define __resolvable__


#include "debug.h"


class Clause;


// ***************** class Resolvable, definition ****************************


class ostream;
class PSymbol;


// A symbol P if (positively) resolvable if and only if
//  (i) all positive occurrences of P are in unit clauses
//  (ii) it has exactly one negative occurrence, and it is flat and linear
// or, symmetrically, with positive swapped with negative
class Resolvable {
 public:
  // constructor
  Resolvable ( int noOfSymbols );
  ~Resolvable ();
  bool getClause (Clause*); // true if there are still candidate symbols
  int remaining () { return _remaining; }
  PSymbol* first ( bool& sign ) const; // return first resolvable symbol or 0 if none
 
 private:
  class Symbol {
   public:
    bool pres; // positively resolvable
    bool nres; // negatively resolvable
    bool occurs;  // occurs in this clause
    bool pos; // has at least one positive occurrence 
    bool neg; // has at least one negative occurrence

    Symbol ();
  };
  
  // structure
  int _noOfSymbols;
  int _remaining;   // number of remaining eligible symbols
  Symbol* _symbols;
 
 friend ostream& operator << ( ostream&, const Resolvable& );
}; // class Resolvable


// for debugging only
ostream& operator << ( ostream&, const Resolvable& );

// ***************** class Resolvable, implementation ****************************




#endif // __resolvable__
