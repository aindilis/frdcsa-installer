//
//  file substitution.h 
//  defines class Substitution
//  currently not a very fast implementation
//

#ifndef __substitution__
#define __substitution__


#include "typedefs.h"
#include "memory.h"
#include "tracer.h"
#include "term.h"
#include "pair.h"


class Substitution
{
 public:
  Substitution ();
  ~Substitution ();
  void push ( Var v, Term* t);
  Term* get (Var v) const;
  void pop ();
  VarList* originalVars () const { return originalVars (_bindings); }
  Term::List* terms () const { return terms (_bindings); }

 private:
  typedef Pair<Var, Term*, CID_BINDING> Binding;
  typedef List<Binding*, CID_BINDING_LIST> List;

  // structure
  List* _bindings;

  // auxiliary
  static VarList* originalVars (const List*);
  static Term::List* terms (const List*);

 friend ostream& operator << (ostream& str, Substitution&);
}; // class Substitution


ostream& operator << (ostream& str, Substitution&);


#endif // __substitution__
