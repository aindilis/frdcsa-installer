//
//  file atom.h 
//  defines class Atom
//

#ifndef __atom__
#define __atom__


class Substitution;


#include "typedefs.h"
#include "int.h"
#include "term.h"
#include "signature.h"
#include "memory.h"
#include "clist.h"


// ****************** class Atom, definition *************************


class Atom 
#   if debug_memory
    : public Memory <CID_ATOM>
#   endif
{
 public:
  typedef CList <Atom*,CID_ATOM_LIST> List;

  // constructors
  Atom ( Signature::Pred* p, Term::List* args ) :
    _functor(p),
    _args (args),
    _count (0)
    { if (args->isNonEmpty()) 
        args->inc (); } 

  Atom ( Term* l, Term* r ); // for equality

  // reference counting
  void inc () { _count++; }
  void dec () 
    { ASS(debug_atom,_count > 0); 
      _count--; 
      if (_count == 0) 
        destroy ();
    }

  // query the structure
  Signature::Pred* functor () const { return _functor; } 
  Term::List* args () const { return _args; } 
  int arity () const { return _functor->arity (); } 

  bool isEquality () const 
    { return functor()->isEquality (); }
  // deallocation and copying
  void destroy ();

  // statistics
  bool occurs (Var v) const 
    { return Term::occurs (args(), v); }

  // miscellaneous
  bool equal ( Atom* ) const;
  bool hasVarsNotIn (const VarListList*) const; 
  bool isFlat () const // atom has the form P (x1,...,xn) where xi are variables
    { return Term::varsOnly (args()); }
  Term* isDefinition ( Term*& rhs ) const; // the result is lhs of the definition
  Compare compare (const Atom* l) const; // comparison, used for normalisationy
  void normalize ();
  Atom* apply (const Substitution&) const;  
  Atom* rectify (Substitution&, Var& last, VarList*& freeVars) const;  
  void outputKIF (ostream& to) const; // output in the KIF format

  // functions for checking equality axioms
  bool isTautology () const;
  bool swap (const Atom* a) const; // true if this is t1 = t2 and a is t2 = t1
  static bool transitivity (const Atom* a1, const Atom* a2, const Atom* a3);
  static bool functionMonotonicity (const Atom* a1, const Atom* a2);
  static bool predicateMonotonicity (const Atom* a1, const Atom* a2, const Atom* a3);

 private: 
  // structure
  Signature::Pred* _functor;
  Term::List* _args;
  long _count;

  inline void functor ( Signature::Pred* f ) { _functor = f; } 
  inline void args ( Term::List* ts ) 
    { 
      ts->inc ();
      _args->dec ();
      _args = ts; 
    } 
}; // class Atom


ostream& operator << ( ostream& str, const Atom* t );


#endif // __atom__
