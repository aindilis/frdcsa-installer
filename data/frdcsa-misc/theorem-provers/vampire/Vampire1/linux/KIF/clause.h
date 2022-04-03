//
//  file clause.h 
//  defines class Clause
//
//  28/05/2000, Manchester
//  01/06/2000, Manchester, isDefinition added
//  03/06/2000, Manchester, class Literal moved to a separate file
//  29/03/2001, Machester, Functor changed to Functor* everywhere
//  09/05/2001 Manchester: occurs ()
//  15/05/2001 Manchester: replaceCancellation()
//  22/05/2001 Manchester: isLinearDefinition()
//  27/05/2001 Manchester: occurs (PSymbol*)
//  30/05/2001 Manchester, splitLargeArity/2, negativeEqualitySplit/1
//


#ifndef __clause__
#define __clause__


#include "typedefs.h"
#include "literal.h"
class Term;
class Atom;


class Clause {
 public:

  typedef List<Clause*,CID_CLAUSE_LIST> List;

  // constructor
  explicit Clause ( Literal::List* ll ) :
    _literals ( ll )
    { if (ll->isNonEmpty()) 
        ll->inc (); }
  ~Clause () { _literals->dec(); }

  // structure
  Literal::List* literals () const { return _literals; }
  void literals (Literal::List* l) 
    { l->inc ();
      _literals->dec ();
      _literals = l;
    }

  // output
  void prettyPrint ( int indent, ostream& str ) const;

  // properties
  bool isEqualityAxiom () const;  // is one of the equality axioms
  bool isEmpty () const { return literals()->isEmpty(); }
  Term* isFunctionDefinition ( Term* & rhs ) const; // return lhs of the definition or 0

  // miscellaneous
  void normalize ();
  bool isless (const Clause*) const; // used to compare clauses, needed for normalisation
/*
  // statistics
  int countOccurrences ( const FSymbol* ) const;
  int countOccurrences ( const PSymbol* ) const;
  int countOccurrences ( const Var ) const;
  Var maxVar () const;
  bool isHorn () const;
  bool isUnit () const;
  bool hasEquality () const;
  bool hasNonEquality () const;
  bool hasFunctionSymbols () const;
  bool isPropositional () const;
  bool isTautology () const;
  int length () const;
  int weight () const;
  Term* isLinearDefinition ( Term*& rhs ) const; 
  bool isUniquenessAxiom ( PSymbol*& pred, int& argNo ) const; // the clause is P(x,y) & P(x,z) -> y = z
  bool isCancellationAxiom ( FSymbol*& fun, int& argNo ) const; // the clause is f(U,x,Z) = f(U,y,Z) -> x = y
  Var* isFunctionalityAxiom ( const PSymbol* pred, FSymbol*& fun, int argNo, Atom*& ) const; // the clause if P(x,f(x))
  bool becomesUnitEq ( PSymbol*& pred, bool& sign );
  bool occurs ( const PSymbol* pred, bool& sign ) const; // true if predicate pred occurs, sign = polarity of pred's first occurrence
  bool occurs ( const FSymbol* ) const; 
  bool occurs ( const PSymbol* ) const; 

  // miscellaneous
  void replaceCancellation ( int argNo,FSymbol* inv );
  bool replaceRelationByFunction ( PSymbol* pred, FSymbol* fun, int argNo, int* vs );
  int removeVarEquality (); // remove x != t, result is the number of remove equalities
  void split ( FSymbol* tosplit, int maxArity, int iterations, int restArity, 
               FSymbol* firstFun, FSymbol* restFun );
  int maxLitWeight ();
  bool canRemovePredicateDefinition ( PSymbol* pred, bool sign );
  Clause* resolveOn ( const PSymbol* pred, Clause* );
  Compare compare ( const Clause* ) const; // comparison of normalized clauses
  void splitLargeArity (const PSymbol* p, FSymbol::List* newfs );
  void splitLargeArity (const FSymbol* p, FSymbol::List* newfs );
  List* negativeEqualitySplit ( int thresholdWeight );
  void polarities ( const PSymbol* p, int& pol ) const;
*/

 private:
  // structure
  Literal::List* _literals;
/*
  int _weight;

  // auxiliary functions
  bool isEquationalTautology () const;
  bool isNonEquationalTautology () const;
*/
  // equality axiom check
  bool isReflexivityAxiom () const;
  bool isFunctionReflexivityAxiom () const;
  bool isPredicateReflexivityAxiom () const;
  bool isSymmetryAxiom () const;
  bool isTransitivityAxiom () const;
}; // class Clause


ostream& operator << ( ostream&, const Clause* );

#endif
