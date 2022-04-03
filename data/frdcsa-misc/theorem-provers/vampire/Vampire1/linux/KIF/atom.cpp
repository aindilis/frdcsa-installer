//
//  file atom.cpp
//  implements class Atom
//

#include "tracer.h"
#include "term.h"
#include "atom.h"
#include "options.h"


// equality constructor
// 09/05/2002 Manchester
Atom::Atom (Term* l, Term* r) 
  :
  _functor (sig->equality()),
  _args (new Term::List (l, new Term::List(r))),
  _count (0)
{
  _args->inc (); 
} // Atom::Atom (Term* l, Term* r) 


/*
Atom* Atom::replaceRelationByFunction ( PSymbol* pred, FSymbol* fun, int argNo, int* vs )
{
  if ( functor () != pred )
    return 0;

  Term* lhs = args()->nth ( argNo );

  // create the argument list for the rhs
  Term::List* ts = Term::List::empty ();
  for ( int last = args()->length() - 2; last >= 0; last-- ) 
    Term::List::push ( args()->nth(vs[last]), ts );

  Term* rhs = new Term ( fun, ts );

  Term::List* newargs = new Term::List ( rhs );
  Term::List::push ( lhs, newargs );

  args()->destroy();
  delete this;

  return new Atom ( PSymbol::equality(), newargs );
} // Atom::replaceRelationByFunction
*/


void Atom::destroy ()
{
  args()->dec();
  delete this;
} // Atom::destroy


/*
Var Atom::maxVar ()
{
  Var v = firstVar - 1;

  Term::List::Iterator ts ( args() ); 
  while ( ts.more() ) {
    ts.next()->maxVar (v);
  }

  return v;
} // Atom::maxVar


Atom* Atom::copy () const
{
  return new Atom ( functor(), Term::copy(args()) );
} // Atom::copy 


void Atom::replace ( Var v, Term* t )
{
  for ( Term::List* ts = args(); ts->isNonEmpty(); ts = ts->tail() )
    ts->head()->replace ( v, t );
} // Atom::replace
*/


bool Atom::equal (Atom* t) const
{
  return functor() == t->functor() &&
         Term::equal ( args(), t->args() );
} // Atom::equal


// 27/06/2002 Manchester
Atom* Atom::apply ( const Substitution& subst ) const
{
  TRACER ("Atom::apply");

  return new Atom ( functor(), 
		    Term::apply (args(), subst) );
} // Atom::apply


// 14/07/2002 Manchester
Atom* Atom::rectify (Substitution& subst, Var& last, VarList*& freeVars) const
{
  TRACER ("Atom::apply");

  return new Atom ( functor(), 
		    Term::rectify (args(), subst, last, freeVars) );
} // Atom::rectify


// output the atom to str
// 26/05/2002 Manchester, infix equality printing added
ostream& operator << ( ostream& str, const Atom* a )
{
  if ( options->outputEqualityInfix() && a->isEquality() ) {
    Term::List* ts = a->args();
    str << ts->head() << "=" << ts->second();
    return str;
  }

  str << a->functor();
  if ( ! a->args()->isEmpty() ) 
    str << '(' << a->args() << ')';

  return str;
} // operator << ( ostream&, const Atom* f )


// comparison of two atoms, needed to normalize
// 26/04/2002, Manchester
Compare Atom::compare ( const Atom* a ) const 
{ 
  // equality is less than nonequality
  if ( isEquality() ) {
    if ( ! a->isEquality() )
      return LESS;
  }
  else if ( a->isEquality() ) {
      return GREATER;
  }
  // now either both are equality or both are not equality

  switch ( functor()->compare(a->functor()) ) {
    case LESS:
      return LESS;

    case EQUAL:
      return Term::compare ( args(),a->args() );

    case GREATER:
      return GREATER;

    #ifdef debug_atom
    default:
      ASS( debug_atom, false);
    #endif
  }
}  // Atom::compare


void Atom::normalize () 
{
  if ( ! isEquality() ) {
    return;
  }

  // equality
  Term::List* as = args ();
  Term* l = as->head ();
  Term* r = as->second ();

  if (l->compare(r) == LESS) {
    Term::List* newAs = new Term::List (r, new Term::List (l));
    args ( newAs );
  }
} // Atom::normalize


/*
void Atom::rename ( VarSubst& subst )
{
  for ( Term::List* ts = args (); ! ts->isEmpty (); ts = ts->tail () )
    ts->head()->rename( subst );
} // Atom::rename


int Atom::weight () const
{
  return 1 + Term::weight ( args() );
} // Atom::weight


int Atom::countOccurrences ( const FSymbol* f ) const
{
  int occ = 0;

  for ( Term::List* ts = args(); ts->isNonEmpty(); ts = ts->tail() ) 
    occ += ts->head()->countOccurrences (f);

  return occ;
} // Atom::countOccurrences


int Atom::countOccurrences ( const Var v ) const
{
  int occ = 0;

  for ( Term::List* ts = args(); ts->isNonEmpty(); ts = ts->tail() ) 
    occ += ts->head()->countOccurrences (v);

  return occ;
} // Atom::countOccurrences


bool Atom::occurs ( const FSymbol* f ) const
{
  return Term::occurs( args(), f );
} // Atom::occurs


// replace the atom
//   f(X,y,Z) = f(X,u,Z)
// by 
//   i(f(X,y,Z),X,Z) = y
void Atom::introduceInverse ( int argNo, FSymbol* inv )
{
  Term* l = args()->head();
  Term* r = args()->second();
  Term* u = Term::List::deleteNth ( r->_args, argNo );
  u->destroy();
  // the atom l=r takes the form f(X,y,Z) = f(X,Z)
  args()->head(new Term (inv, new Term::List (l, r->args())));    // set lhs to i(f(X,y,Z),X,Z)
  delete r;
  args()->tail()->head(new Term(l->args()->nth(argNo)->var())); // set rhs to y
} // Atom::introduceInverse


void Atom::splitLargeArity (const PSymbol* p, FSymbol::List* newFs )
{
  if ( functor() == p ) {
    args ( Term::splitLargeArity (args(),newFs) );
  }
} // Atom::splitLargeArity


void Atom::splitLargeArity (const FSymbol* f, FSymbol::List* newFs )
{
  Term::List::Iterator ts ( args() );
  while ( ts.more() ) {
    ts.next()->splitLargeArity (f,newFs);
  }
} // Atom::splitLargeArity


Atom* Atom::negativeEqualitySplit ( int thresholdWeight )
{
  if ( ! isEquality() )
    return 0;

  Term* l = args()->head();
  Term::List* rs = args()->tail();
  Term* r = rs->head();

  int lw; // weight of l;
  int rw; // weight of l;
  PSymbol* P; // will be used as a new name
  if ( l->isGround() && (lw = l->weight()) >= thresholdWeight ) {
    if ( r->isGround() && (rw = r->weight()) >= thresholdWeight && rw > lw ) { // both splittable but right is greater
      goto split_right;
    }
    // l is used for splitting
    P = PSymbol::createNew ("spl_", 1);
    // changing =(l,r) into P(r)
    _functor = P;
    args()->tail ( Term::List::empty() );
    args()->head (r);
    // create P(l) and return it as a result
    rs->head (l);
    return new Atom (P, rs);
  }
  // l is non-splittable
  else if ( r->isGround() && (rw = r->weight()) >= thresholdWeight )  { // r is splittable
   split_right:
    P = PSymbol::createNew ("spl", 1);
    // changing =(l,r) into P(l)
    _functor = P;
    args()->tail ( Term::List::empty() );
    // create P(r) and return it as a result
    return new Atom (P, rs);
  }
  else {  // none is splittable
    return 0;
  }
} // Atom::negativeEqualitySplit
*/


// A focus-pocus for Visual C++, if you want to see VC++ disappear by itself
// uncomment the following declaration of Lst and of lst. VC++ will disappear 
// while you wait. Gnu C++ solves this problem by restricting the depth of
// embedded templates by 17

// template <int i> 
// class Lst
// {
//   int head;
//   Lst <i+1> tail;
// };

// Lst <0> lst;


// return true if the atom is t = t
// 29/04/2002 Manchester
bool Atom::isTautology () const
{
  TRACER("Atom::isTautology");

  if ( ! isEquality() )
    return false;

  return args()->head()->equal (args()->second());
} // Atom::isTautology


// true if a1 is t1 = t2 and a2 is t2 = t1
// 29/04/2002, Manchester
bool Atom::swap (const Atom* a) const
{
  TRACER("Atom::swap");

  if ( ! isEquality() )
    return false;
  if ( ! a->isEquality() )
    return false;

  return args()->head()->equal (a->args()->second()) &&
         a->args()->head()->equal (args()->second());
} // Atom::swap


// return true if the atoms are r = s, s = t, and r = t (up to commutativity)
// 29/04/2002 Manchester
bool Atom::transitivity (const Atom* a1, const Atom* a2, const Atom* a3) 
{
  TRACER("Atom::transitivity");

  if ( ! (a1->isEquality() && 
          a2->isEquality() && 
          a3->isEquality()) ) {
    return false;
  }
  // a1, a2, a3 are equalities
  Term* l1 = a1->args()->head();
  Term* r1 = a1->args()->second();
  if ( l1->equal(r1) ) {
    return false;
  }
  Term* l2 = a2->args()->head();
  Term* r2 = a2->args()->second();
  if ( l2->equal(r2) ) {
    return false;
  }
  Term* l3 = a3->args()->head();
  Term* r3 = a3->args()->second();
  if ( l3->equal(r3) ) {
    return false;
  }

  if ( l1->equal(l2) ) { // l1 = l2 = s
    return (r1->equal(l3) && r2->equal(r3)) ||
           (r2->equal(l3) && r1->equal(r3));
  }
  // l1 != l2
  if ( l1->equal(r2) ) { // l1 = r2 = s
    return (r1->equal(l3) && l2->equal(r3)) ||
           (l2->equal(l3) && r1->equal(r3));
  }
  // 
  if ( r1->equal(l2) ) { // r1 = l2 = s
    return (l1->equal(l3) && r2->equal(r3)) ||
           (r2->equal(l3) && l1->equal(r3));
  }
  // l1 not in {l2,r2}, r1 != l2
  if ( r1->equal(r2) ) { // r1 = r2 = s
    return (l1->equal(l3) && l2->equal(r3)) ||
           (l2->equal(l3) && l1->equal(r3));
  }

  return false;
} // Atom::transitivity


// return true if the atoms are x = y, t[x]=t[y]
// 29/04/2002 Manchester
bool Atom::functionMonotonicity (const Atom* a1, const Atom* a2) 
{
  TRACER("Atom::functionMonotonicity");

  if ( ! a1->isEquality() )
    return false;
  if ( ! a2->isEquality() )
    return false;

  Term* x = a1->args()->head();
  Term* y = a1->args()->second();
  if ( ! x->isVar() || ! y->isVar() )
    return false;

  Var vx = x->var();
  Var vy = y->var();
  // variables must be different
  if ( vx == vy ) 
    return false;

  return a2->args()->head()->equalUpTo (a2->args()->second(),vx,vy);
} // Atom::functionMonotonicity


// return true if the atoms are x = y, P[x] and P[y]
// 29/04/2002 Manchester
bool Atom::predicateMonotonicity (const Atom* a1, const Atom* a2, const Atom* a3) 
{
  TRACER("Atom::predicateMonotonicity");

  if ( ! a1->isEquality() )
    return false;
  if ( a2->isEquality() )
    return false;
  if ( a3->isEquality() )
    return false;

  Term* x = a1->args()->head();
  Term* y = a1->args()->second();
  if ( ! x->isVar() || ! y->isVar() )
    return false;

  Var vx = x->var();
  Var vy = y->var();
  // variables must be different
  if ( vx == vy ) 
    return false;

  if ( a2->functor() != a3->functor() )
    return false;
  
  return Term::equalUpTo (a2->args(),a3->args(),vx,vy);
} // Atom::predicateMonotonicity


// true if the atom has free variables not occurring in vs
// 06/05/2002 Manchester
bool Atom::hasVarsNotIn (const VarListList* vs) const
{
  return Term::hasVarsNotIn (args(),vs);
} // Atom::hasVarsNotIn


// checks that the atom has the form f(x) = t[x],
// where f is a function symbol and t[x] is a term of x not containing f
// 09/05/2002, Manchester
Term* Atom::isDefinition ( Term*& rhs ) const
{
  TRACER ( "Atom::isDefinition" );

  if ( ! isEquality() ) {
    return 0;
  }

  // the atom is an equality
  Term* l = args()->head();
  Term* r = args()->second();
  if ( l->isVar() || r->isVar() ) {
    return 0;
  }
  // l=r, both l and r are non-variables
  if ( r->defines(l) ) {
    rhs = r;
    return l;
  }

  if ( l->defines(r) ) {
    rhs = l;
    return r;
  }

  return 0;
} // Term* Atom::isDefinition


// output atom in the KIF format
// 24/07/2002 Torrevieja
void Atom::outputKIF (ostream& str) const
{
  if (args()) {
    str << '(';
  }
  if (isEquality()) {
    str << '=';
  }
  else {
    str << functor();
  }
  if (args()) {
    Term::outputKIF(args(), str);
    str << ')';
  }
} // Atom::outputKIF
