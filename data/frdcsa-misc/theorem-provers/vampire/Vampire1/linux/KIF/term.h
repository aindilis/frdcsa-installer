//
//  file term.h 
//  defines class Term
//
//  15/05/2000, Dallas
//  28/05/2000, Manchester, Functor made into a class
//  01/06/2000, Manchester, occurs(Functor), size and unify added
//  03/06/2000, Manchester, class Functor moved to separate file
//  09/06/2000, Manchester, isEquality()
//  29/03/2001, Machester, Functor changed to Functor* everywhere
//  31/03/2001, Machester, size changed to weight
//  01/04/2001, Dagstuhl, several functions moved to atom
//  30/05/2001 Manchester, splitLargeArity/2
//  31/05/2001 Manchester isGround/0
//  03/06/2001 equalUpTo/3, varArgs/1, createSkolemTerm/1
//

#ifndef __term__
#define __term__


class Substitution;


#include "typedefs.h"
#include "int.h"
#include "signature.h"
#include "memory.h"
#include "clist.h"



// ****************** class Term, definition *************************


class Term 
#   if debug_memory
    : public Memory <CID_TERM>
#   endif
{
 public:

  typedef CList<Term*,CID_TERM_LIST> List;
  
  enum Tag {
    VAR,
    COMPOUND,
    REAL
  }; // enum Term::Tag

  // constructors
  Term ( Var v ) : // for variables
    _tag ( VAR ),
    _var ( v ),
    _count (0)
    {} 
  Term ( Signature::Fun* f, List* args ) : // for compound terms
    _tag ( COMPOUND ),
    _args ( args ),
    _functor( f ),
    _count (0)
    { if (args->isNonEmpty()) 
        args->inc(); } 

  // reference counting
  void inc () { _count++; }
  void dec () 
    { ASS (debug_term, _count > 0);
      _count--; 
      if (_count == 0) {
        if (! isVar() ) {
          args()->dec ();
        }
        delete this;
      }
    }

/*
  bool isLinearAndFlat () const; // term has the form f (x1,...,xn) where xi are pairwise different
  static bool sequenceOfUniqueVariables ( const List* ); // sequence has the form (x1,...,xn) where xi are different variables
*/

  // change/query the structure
  bool isVar () const { return _tag == VAR; } 
  void tag ( Tag t ) { _tag = t; } 
  void functor ( Signature::Fun* f ) { _functor = f; } 
  void args ( List* ts ) { _args = ts; } 

  Signature::Fun* functor () const
    { ASS ( debug_term, _tag == COMPOUND );
      return _functor; } 
  List* args () const 
    { ASS ( debug_term, _tag == COMPOUND );
      return _args; } 
  Var var () const
    { ASS ( debug_term, _tag == VAR );
      return _var; }
  // query the structure
  void var ( Var v )
    { ASS ( debug_term, _tag == VAR );
      _var = v; } 
  int arity () const
    { ASS ( debug_term, _tag == COMPOUND );
      return _functor->arity(); } 

  // properties
  static bool varsOnly (const List* ts); // ts consists of variables only
  static bool hasVarsNotIn (const List*, const VarListList*); 
  bool occurs ( const Signature::Fun* ) const;
  static bool occurs ( const List*, const Signature::Fun* );
  bool occurs (Var v) const;
  static bool occurs ( const List*, Var v );

  // miscellaneous
  bool equalUpTo (const Term* r, Var x, Var y) const; // this obtained from rs by swapping x and y
  static bool equalUpTo ( const List* ls, const List* rs, Var x, Var y); // ls obtained from rs by swapping x and y
  bool equal ( const Term* ) const;
  static bool equal ( const List*, const List* );
  bool defines ( const Term* lhs ) const;  // auxiliary function for atoms
  Compare compare (const Term* l) const; // comparison, used for normalisation
  static Compare compare (const List* l1, const List* l2); 
  Term* apply ( const Substitution& );  
  static List* apply (const List*, const Substitution&);  
  Term* rectify (Substitution&, Var& last, VarList*& freeVars);  
  static List* rectify (const List*, Substitution&, Var& last, VarList*& freeVars);  
  static List* varsToTerms (const VarList* vs);

  // for printing
  static const char* varPrefix;
  static void outputKIF (List* terms, ostream& str);

 private: 

  // structure
  Tag _tag;
  union {
    Var _var;
    List* _args;
    float _number;
  };
  Signature::Fun* _functor;
  long _count;

 friend ostream& operator << ( ostream&, const Term::List* );
}; // class Term


ostream& operator << ( ostream&, const Term* );
// un-commenting this causes INTERNAL COMPILER ERROR for VC++
// ostream& operator << ( ostream&, const Term::List* );


#endif // __term__

