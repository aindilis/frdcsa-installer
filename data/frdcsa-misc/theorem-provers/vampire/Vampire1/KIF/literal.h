//
//  file literal.h 
//  defines class Literal
//


#ifndef __literal__
#define __literal__


#include "typedefs.h"
#include "atom.h"


// ******************* class Literal, definition *********************


class Literal {
 public:

  typedef CList <Literal*,CID_LITERAL_LIST> List;

  // constructor
  Literal ( bool sign, Atom* atom ) : 
    _sign ( sign ), 
    _atom ( atom ),
    _count (0)
    { atom->inc (); }
  void inc () { _count++; }
  void dec () 
    { ASS (debug_literal, _count > 0);
      _count--; 
      if (_count == 0) {
        destroy ();
      }
    }

  // query/change the structure
  bool positive () const { return _sign; } 
  bool negative () const  { return ! _sign; } 
  bool sign () const { return _sign; } 
  Atom* atom () const { return _atom; } 
  void atom ( Atom* );
  void destroy ();
/*
  int countOccurrences ( const FSymbol* ) const;
  int countOccurrences ( Var ) const;
  bool occurs ( const FSymbol* ) const;
  bool complementary ( Literal* ); // not for equality literals
  Var maxVar ();
  Literal* apply ( Subst&, VarSubst&, int& lastVar ); // copies the argument
  Literal* copy () const; 
  static List* copy ( const List* );
  static List* sort ( List* );
  int weight (); // weight in the number of symbols
  void introduceInverse ( int argNo,FSymbol* inv );
  void splitLargeArity (const PSymbol* p, FSymbol::List* newFs );
  void splitLargeArity (const FSymbol* p, FSymbol::List* newFs );
  Literal* negativeEqualitySplit ( int thresholdWeight );
*/
  //miscellaneous
  bool isless (const Literal* l) const;       // comparison, used for normalisation
  Compare compare (const Literal* l) const; // comparison, used for normalisation
  inline void normalize () 
    { atom()->normalize(); }

 private:
  
  bool _sign;
  Atom* _atom;
  long _count;
/*
  int _weight;  // -1 is not computed before

  static List* sort ( List* toSort, List* appendTo );
  static void split ( Literal*, List*, // for sorting
                      List*& less, List*& equal, List*& greater );
*/
}; // class Literal


ostream& operator << ( ostream&, const Literal* );
/*
ostream& operator << ( ostream&, Literal* );


// ******************* class Literal, implementation *********************


inline
void Literal::atom ( Atom* t )
{
  _atom = t;
  _weight = -1;
} // Literal::atom
 

inline
int Literal::countOccurrences ( const FSymbol* f ) const
{
  return atom()->countOccurrences ( f );
} // Literal::countOccurrences


inline
bool Literal::occurs ( const FSymbol* f ) const
{
  return atom()->occurs ( f );
} // Literal::occurs


inline
int Literal::countOccurrences ( Var v ) const
{
  return atom()->countOccurrences ( v );
} // Literal::countOccurrences


inline 
Var Literal::maxVar ()
{
  return atom()->maxVar ();
} // Literal::maxVar


inline 
bool Literal::complementary ( Literal* l )
{
  return _sign != l->_sign &&
         ( ! atom()->isEquality() ) &&
         atom()->equal( l->atom() );
} // Literal::complementary


inline
Literal* Literal::apply ( Subst& s, VarSubst& vs, int& lastVar ) // copies the argument
{
  return new Literal ( sign(), atom()->apply(s,vs,lastVar) );
} // Literal::apply


inline
Literal* Literal::copy () const
{
  return new Literal ( _sign, _atom->copy(), _weight );
} // Literal::copy

*/
#endif
