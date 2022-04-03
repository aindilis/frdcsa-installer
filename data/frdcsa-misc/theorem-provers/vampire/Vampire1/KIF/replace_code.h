//
//  File replace_code.h
//
//  defines code for replacement f(x1,...,xn) -> t
//
//  04/06/2000, Manchester
//  29/03/2001, Macnchester, Functor changed to Functor*
//  21/05/2001 Functor replaced by FSymbol
//  22/05/2001 apply () returns boolean
//


#ifndef __replace_code__
#define __replace_code__


#include "typedefs.h"
#include "term.h"

class Clause;
class FSymbol;
class Atom;


class ReplaceCode {
 public:
  
  // constructors & destructors
  ReplaceCode ( const Term* lhs, const Term* rhs );
  ~ReplaceCode ();
  int newVars();

  int maxLitWeight (const Clause*) const;   // checks maximal literal weight after application without actually applying the code, 0 if not applicable
  bool apply ( Clause* ) const;

 private:

   private:

  Term* _code;
  FSymbol* _functor;
  int _newVars;   // number of new variables

  // miscellaneous
  Term* compile ( const Term*, VarSubst& vars, VarSubst& newvars, int& lastNewVar ); 
  Term::List* compile ( Term::List*, VarSubst& vars, VarSubst& newvars, int& lastNewVar ); 
  Term* replace ( const Term* termToReplace, Term::List* args, Var lastVar ) const;
  Term::List* replace ( Term::List* termsToReplace, Term::List* args, Var lastVar ) const;
  bool apply ( Clause*, Var& lastVar ) const;
  bool apply ( Term*, Var& lastVar ) const;
  bool apply ( Term::List*, Var& lastVar ) const;
  bool apply ( Atom*, Var& lastVar ) const;
  int weightAfter (const Atom*,bool& applicable) const; 
  int weightAfter (const Term::List*,bool& applicable) const; 
  static int weightAfter (const Term* code, const Term::List* subst); 
  static int weightAfter (const Term::List* code, const Term::List* subst); 
  
 friend ostream& operator << ( ostream&, ReplaceCode& );
}; // class ReplaceCode


ostream& operator << ( ostream&, ReplaceCode& );


inline
int ReplaceCode::newVars()
{
  return _newVars;
}


#endif // __replace_code__
