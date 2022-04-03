//
//  file pure.h 
//  defines class Pure: auxiliary class for finding pure predicate symbols
//
//  27/05/2001 Manchester
//

#ifndef __pure__
#define __pure__


#include "debug.h"


class Clause;
class PSymbol;


// ***************** class Pure, definition ****************************


class ostream;

// A symbol P if pure if and only if either all of its occurrences are
// positive or all of them are negative
class Pure {
 public:

  class Symbol { // made public since used by Iterator
   public:
    bool pos; // has at least one positive occurrence
    bool neg; // has at least one neg occurrence

    Symbol ();
  };  // class Pure::Symbol

  // constructor
  Pure ( int noOfSymbols );
  ~Pure ();
  bool getClause (Clause*); // true if there are still candidate symbols
  int remaining () { return _remaining; }
  Symbol* symbols () { return _symbols; }

  class Iterator {
   public:
    Iterator ( Pure& );
    ~Iterator ();
    PSymbol* next (); // 0 if none
   private:
    PSymbol** _signature;
    Symbol* _symbols;
    int _nextSymbol;
  };  // class Pure::Iterator
 
 private:
  // structure
  int _noOfSymbols;
  int _remaining;   // number of remaining eligible symbols
  Symbol* _symbols;
 
 friend ostream& operator << ( ostream&, const Pure& );
}; // class Resolvable


// for debugging only
ostream& operator << ( ostream&, const Pure& );

// ***************** class Pure, implementation ****************************




#endif // __pure__
