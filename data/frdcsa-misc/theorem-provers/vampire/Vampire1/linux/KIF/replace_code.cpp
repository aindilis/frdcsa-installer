//
//  File replace_code.cpp
//
//  implements code for replacement f(x1,...,xn) -> t
//
//  04/06/2000, Manchester
//  12/04/2001, Manchester, maxLitWeight ()
//  22/05/2001 apply () returns boolean
//


#include "replace_code.h"
#include "term.h"
#include "literal.h"
#include "clause.h"


ReplaceCode::ReplaceCode ( const Term* lhs, const Term* rhs )
{
  // initializing varsubst
  VarSubst args;
  Var v (0);
  for ( Term::List* ts = lhs->args (); ts->isNonEmpty(); ts = ts->tail() ) {
    args.bind ( ts->head()->var(), v );
    v++;
  }

  VarSubst newVars;
  Var lastNewVar = 0;

  _code = compile( rhs, args, newVars, lastNewVar );
  _functor = lhs->functor ();

  _newVars = - lastNewVar;
} // ReplaceCode::ReplaceCode


Term* ReplaceCode::compile ( const Term* t, VarSubst& vars, VarSubst& newvars, int& lastNewVar )
{
  Var w;

  if ( t->isVar () ) {
    if ( vars.bound(t->var(),w) ) // t is a variable that is argument to lhs
      return new Term ( w );
   
    // t is a variable that is not an argument to lhs
    if ( newvars.bound(t->var(),w) ) // t occurred before
      return new Term ( w );
    
    // t is a new variable
    lastNewVar --;
    newvars.bind ( t->var(), lastNewVar );
    return new Term ( lastNewVar );
  }

  // t is not a variable
  return new Term ( t->functor(), compile (t->args(), vars, newvars, lastNewVar) );
} // ReplaceCode::compile


Term::List* ReplaceCode::compile ( Term::List* ts, VarSubst& vars, VarSubst& newvars, int& lastNewVar )
{
  if ( ts->isEmpty() )
    return Term::List::empty ();

  return new Term::List ( compile (ts->head(), vars, newvars, lastNewVar),
                        compile (ts->tail(), vars, newvars, lastNewVar) );
} // ReplaceCode::compile


ReplaceCode::~ReplaceCode ()
{
  _code->destroy ();
}


bool ReplaceCode::apply ( Clause* c ) const
{
  Var last = c->maxVar();

  return apply ( c, last );
} // ReplaceCode::apply ( Clause* )


ostream& operator << ( ostream& str, ReplaceCode& rc )
{
  str << rc._code;

  return str;
} // operator << ( ostream& str, ReplaceCode& rc )


bool ReplaceCode::apply ( Clause* c, Var& lastVar ) const
{
  bool applied = false;
  Literal::List::Iterator ls ( c->literals () );
  while ( ls.more() ) {
    if ( apply (ls.next()->atom(), lastVar) ) {
      applied = true;
    }
  }

  return applied;
} // ReplaceCode::apply ( Clause*, Var& )


bool ReplaceCode::apply ( Term* t, Var& lastVar ) const
{
  if ( t->isVar() )
    return false;

  return apply( t->args(), lastVar );
} // ReplaceCode::apply ( Term*, Var& )


bool ReplaceCode::apply ( Atom* a, Var& lastVar ) const
{
  return apply( a->args(), lastVar );
} // ReplaceCode::apply ( Atom*, Var& )


bool ReplaceCode::apply ( Term::List* ts, Var& lastVar ) const
{
  bool applied = false;

  while ( ts->isNonEmpty() ) {
    Term* h = ts->head ();
    // first, apply to the arguments of h
    if ( apply ( h, lastVar ) ) {
      applied = true;
    }

    // then, apply to the top position of h
    if ( (! h->isVar() ) && h->functor() == _functor ) { // replacement should be made
      ts->head ( replace (_code, h->args(), lastVar) );
      applied = true;

      lastVar += _newVars;
    }

    ts = ts->tail ();
  }

  return applied;
} // ReplaceCode::apply ( Term::List*, Var& )


Term* ReplaceCode::replace ( const Term* t, Term::List* args, Var lastVar ) const
{
  if ( t->isVar() ) {
    Var v = t->var ();
    if ( v >= 0 )
      return args->nth(v)->copy();
    // v < 0, new variable should be used
    return new Term ( lastVar - v );
  }
  // t is not a variable
  return new Term ( t->functor(), replace( t->args(), args, lastVar) );
} // ReplaceCode::replace


Term::List* ReplaceCode::replace ( Term::List* ts, Term::List* args, Var lastVar ) const
{
  if ( ts->isEmpty() )
    return Term::List::empty ();

  return new Term::List ( replace( ts->head(), args, lastVar),
                        replace( ts->tail(), args, lastVar) );
} // ReplaceCode::replace


int ReplaceCode::maxLitWeight ( const Clause* c ) const
{
  int mlw = 0;
  bool applicable = false;

  for ( Literal::List* ls = c->literals (); ls->isNonEmpty(); ls = ls->tail() ) {
    int lw = weightAfter (ls->head()->atom(), applicable);
    if ( lw > mlw )
      mlw = lw;
  }

  return applicable ?
         mlw :
         0;
} // ReplaceCode::maxLitWeight ( Clause* )


int ReplaceCode::weightAfter ( const Atom* a, bool& applicable ) const
{
  return weightAfter ( a->args(), applicable ) + 1;
} // ReplaceCode::weightAfter ( Atom*, bool )


int ReplaceCode::weightAfter ( const Term::List* ts, bool& applicable ) const
{
  int w = 0;
  while ( ts->isNonEmpty() ) {
    Term* t = ts->head ();
    if ( t->isVar() ) {
      w++;
    }
    // t is not a variable
    else if ( t->functor() != _functor ) { // functors are different
      w += 1 + weightAfter ( t->args(), applicable );
    }
    else {  // functors coincide, applicable
      applicable = true;
      w += weightAfter ( _code, t->args() );
    }
    ts = ts->tail ();
  }

  return w;
} // ReplaceCode::weightAfter ( Term::List*, bool& )



int ReplaceCode::weightAfter ( const Term* code, const Term::List* subst ) 
{
  if ( code->isVar() ) {
    if ( code->var() < 0 ) { // new variable
      return 1;
    }
    // variable x_n, return the size of subst_n
    return subst->nth(code->var())->weight();
  }

  return 1 + weightAfter (code->args(), subst);
} // ReplaceCode::weightAfter ( Term*, Term::List* )


int ReplaceCode::weightAfter ( const Term::List* code, const Term::List* subst ) 
{
  int w = 0;

  while ( code->isNonEmpty() ) {
    w += weightAfter ( code->head(), subst );
    code = code->tail ();
  }

  return w;
} // ReplaceCode::weightAfter ( Term*, Term::List* )


