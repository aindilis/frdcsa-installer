//
//  file term.cpp
//  implements class Term
//

#include <string.h>

#include "tracer.h"
#include "debug.h"
#include "term.h"
#include "sort.h"
#include "substitution.h"


const char* Term::varPrefix = "X";

//
//  ostream& operator << ( ostream& str, const Term::List* ts ) -
//    output to str nonempty list of terms ts in the form t1,...,tn
//
ostream& operator << ( ostream& str, const Term::List* ts )
{
  ASS( debug_term,
          ! ts->isEmpty () );

  Term::List* tail = ts ->tail ();
  
  while ( ! tail->isEmpty () ) {
    str << ts->head () << ',';
    ts = tail;
    tail = tail->tail ();
  }

  str << ts->head ();

  return str;
} // operator << ( ostream& str, List* vs )


//
// ostream& operator << ( ostream& str, Term* t ) -
//    output to str the term t in the usual logical form
//      (1) variables are printed in the form Xi, where X is Term::varPrefix 
//           and i the variable number
//      (2) constants are written as c
//      (3) compund non-constant terms are written as f(t1,...,tn)
//
ostream& operator << ( ostream& str, const Term* t )
{
  if ( t->isVar () ) {  // t is a variable
    str << Term::varPrefix << t->var ();
  }
  else { // t is not a variable
    str << t->functor();
    if ( ! t->args()->isEmpty() ) // non-constant term
      str << '(' << t->args() << ')';
  }

  return str;
} // operator << ( ostream&, Term* f )

/*
//
//  bool Term::isLinearAndFlat () const
//    the term has the form f(x1,...,xn), 
//      where xi are pairwise different variables
//
bool Term::isLinearAndFlat () const
{
  if ( isVar () )
    return false;

  // non-variable term
  return sequenceOfUniqueVariables (args());
} // Term::isLinearAndFlat


//
//  static bool Term::sequenceOfUniqueVariables ( const List* ts )
//    ts is a sequence of pairwise different variables (x1,...,xn) 
//
//  17/07/2001 Manchester, changed from a quadratic algorithm to a linear one
//
bool Term::sequenceOfUniqueVariables ( const List* terms )
{
  // initial size = 32, size increment by 8, initial value false
  typedef ExtendableArray <bool, 32, 8, false> Array; 

  // array used to denote occurrences of variables in the previously parsed arguments
  static Array occur;

  bool result = true;
  const List* ts; 

  // check that all terms are variables
  for ( ts = terms; ! ts->isEmpty (); ts = ts->tail () ) {
    // check that this argument is a variables
    Term* t = ts->head ();
    if ( ! t->isVar() ) { // t is not a variable
      result = false;
      goto reset_array;
    }

    // t is a variable
    Var v = t->var ();

    if ( occur[v] ) { // the variable has already occurred
      result = false;
      goto reset_array;      
    }
    // v is a new variable, mark occurs
    occur[v] = true;
  }

  reset_array:
  // reset values set to true before by false
  for ( const List* ns = terms; ns != ts; ns = ns->tail () ) {
    ASS ( debug_term, occur[ns->head()->var()] );
    occur[ns->head()->var()] = false;
  }

  return result;
} // Term::sequenceOfUniqueVariables


//
//  void Term::destroy () -
//    destroy the whole term, going recursively to the arguments
//
//  17/07/2001 Manchester, changed to use Iterator
//
void Term::destroy ()
{
  if ( ! isVar() ) { // non-variable
    // destroy every argument
    Term::List::Iterator ts ( args() );

    while ( ts.more() ) {
      ts.next()->destroy ();
    }

    // release the memory occupied by the list
    args()->destroy();
  }

  // release the memory occupied by the term header
  delete this;
} // Term::destroy


//
//  int Term::weight () const -
//    return the number of function symbols and variables in the term
//
int Term::weight () const
{
  if ( isVar() )
    return 1;

return 0;
//  return 1 + weight ( args() );
} // Term::weight


//
//  Term::weight ( const List* ts ) -
//    return the number of function symbols and variables in the list of terms ts
//
int Term::weight ( const List* ts )
{
  int sz = 0;

  while ( ts->isNonEmpty() ) {
    sz += ts->head()->weight();
    ts = ts->tail ();
  }

  return sz;
} // Term::weight ( const List* )
*/

//  bool Term::occurs ( const Signature::Fun* f ) const -
//    true if function symbol f occurs in the term
bool Term::occurs ( const Signature::Fun* f ) const
{
  if ( isVar() )
    return false;

  if ( functor () == f )
    return true;

  return occurs( args(), f );
} // Term::occurs


//  bool Term::occurs ( Var v ) const -
//    true if variable v occurs in the term
bool Term::occurs ( Var v ) const
{
  if ( isVar() )
    return var() == v;

  return occurs( args(), v );
} // Term::occurs


//  bool Term::occurs ( const List* ts, const Signature::Fun* f ) -
//    true if function symbol f occurs in the term list ts
bool Term::occurs ( const List* ts, const Signature::Fun* f )
{
  while ( ts->isNonEmpty() ) {
    if ( ts->head()->occurs(f) )
      return true;

    ts = ts->tail();
  }

  return false;
} // Term::occurs ( List* ts, Signature::Fun* f )


// true if variable v occurs in the term list ts
// 25/05/2002 Manchester, changed to use iterators
bool Term::occurs ( const List* ts, Var v )
{
  Term::List::Iterator ss (ts);
  while ( ss.more() ) {
    if ( ss.next()->occurs(v) ) {
      return true;
    }

    ts = ts->tail();
  }

  return false;
} // Term::occurs ( List* ts, Var v )


/*
//
//  int Term::countOccurrences ( const FSymbol* f ) const -
//    the number of occurrences of function symbol f in the term
//
int Term::countOccurrences ( const FSymbol* f ) const
{
  if ( isVar () )
    return 0;

  int occ = functor() == f ? 1 : 0;

  Term::List::Iterator ts ( args() );
  while ( ts.more() ) {
    occ += ts.next()->countOccurrences (f);
  }

  return occ;
} // Term::countOccurrences


//
//  int Term::countOccurrences ( const Var v ) const -
//    the number of occurrences of variable v in the term
//
int Term::countOccurrences ( const Var v ) const
{
  if ( isVar () )
    return var() == v ? 1 : 0;

  int occ = 0;

  Term::List::Iterator ts ( args() );
  while ( ts.more() ) {
    occ += ts.next()->countOccurrences (v);
  }

  return occ;
} // Term::countOccurrences


//
//  void Term::maxVar ( Var& max ) -
//    sets max to the maximum of its old value and the maximal 
//    variable number in the term
//
void Term::maxVar ( Var& max )
{
  if ( isVar () ) {
    if ( var() > max )
      max = var ();
    return;
  }

  // not a variable
  Term::List::Iterator ts ( args() );

  while ( ts.more() ) {
    ts.next()->maxVar (max);
  }
} // Term::maxVar


//
//  void Term::replace ( Var v, Term* t ) -
//    replace in the term all occurrences of v by t
//
void Term::replace ( Var v, Term* t )
{
  if ( isVar() ) { // the term is a variable
    if ( var() == v ) { // the term coincides with v
      // replace the term by t
      if ( t->isVar() ) { // t is a variable
        // simply replace the variable numbet by that of v
        var ( t->var() );
        return;
      }
      // t is not variable, make a deep copy of t to this
      tag ( COMPOUND );
      functor ( t->functor() );
      args ( copy(t->args()) );
      return;
    }
    // t is a variable different from v, do nothing
    return;
  }

  // t is not a variable, replace v by t in all arguments
  Term::List::Iterator ts ( args() );

  while ( ts.more() ) {
    ts.next()->replace ( v, t );
  }
} // Term::replace
*/

//
//  bool Term::equal ( const Term* t ) const -
//    true if the term is equal to t
//
bool Term::equal ( const Term* t ) const
{
  TRACER("Term::equal-1");

  if ( isVar() ) {
    return t->isVar() && 
           var() == t->var();
  }
  if ( t->isVar() ) {
    return false;
  }

  return functor() == t->functor() &&
         equal ( args(), t->args() );
} // Term::equal



//
//  static bool Term::equal ( const List* ts1, const List* ts2 ) -
//    true if ts1 is equal to ts2
//
//  20/07/2001 Manchester, iterators used
//  29/04/2002 Manchester, bug in ASS fixed
bool Term::equal ( const List* ts1, const List* ts2 )
{
  TRACER("Term::equal-2");

  Term::List::Iterator ss1 ( const_cast<List*>(ts1) );
  Term::List::Iterator ss2 ( const_cast<List*>(ts2) );
  while ( ss1.more() ) {
    ASS ( debug_term, ss2.more() );
    
    if ( ss1.next() != ss2.next() )
      return false;
  }
  ASS ( debug_term, ! ss2.more() );

  return true;
} // Term::equal( List* , List* )


//
//  Compare Term::compare ( const Term* t ) const -
//    compare terms this and t using an order described below
//    Used to normalize terms
//
//    t1 is less than t2 if
//      (1) t1 is variable and t2 is not, or else
//      (2) t1->functor < t2.functor, or else
//      (3) t1.args << t2.args,  where << is the lexicographic comparison
//
Compare Term::compare ( const Term* t ) const
{
  if ( isVar () ) {
    if ( t->isVar() ) 
      return EQUAL;
    else 
      return LESS;
  }

  // this is nonvariable
  if ( t->isVar() ) 
    return GREATER;

  // both are nonvariable
  switch ( functor()->compare (t->functor()) ) {
    case LESS:
      return LESS;

    case GREATER:
      return GREATER;

    case EQUAL:
      // both are nonvariable and functors are equal
      return compare ( args(),t->args() );

    #if debug_term
    default:
      ASS(debug_term, false);
    #endif
  }
} // Term::compare


//  Compare Term::compare ( const List* ts1, const List* ts2 ) -
//    compare terms lists ts1 and ts2 using the order above
//    Used to normalize terms
Compare Term::compare ( const List* t1, const List* t2 ) 
{
  for (;;) {
    if ( t1->isEmpty() ) {
      if (t2->isEmpty()) {
        return EQUAL;
      }
      return LESS;
    }
    // t1 is non-empty
    if (t2->isEmpty()) {
      return GREATER;
    }
    // both non-empty
    Compare cmp = t1->head()->compare (t2->head());
    if (cmp != EQUAL) {
      return cmp;
    }

    t1 = t1->tail();
    t2 = t2->tail();
  }
} // Term::compare


/*
//
//  void Term::split ( FSymbol* f, int m, int i, int r, FSymbol* g, FSymbol* h )
//    replace all occurrences of f(t_1,...,t_mi+r) in the term by
//    f(g(t_1,...,t_m),...,g(t_m(i-1)+1,...,t_mi),h(t_mi+1,...,t_mi+r))
//
void Term::split ( FSymbol* tosplit, int maxArity, int iterations, int restArity, 
                   FSymbol* firstFun, FSymbol* restFun )
{
  if ( isVar() )
    return;

  // the term is a non-variable
  // first, make the replacement in all arguments
  Term::List::Iterator ts ( args() );
  
  while ( ts.more() ) {
    ts.next()->split ( tosplit, maxArity, iterations, restArity, firstFun, restFun );
  }

  // now make the replacement on the top level
  if ( functor() != tosplit ) // the functor is not f, terminate
    return;
    
  // the functor is f, replace
  args ( split(args(), maxArity, iterations, restArity, firstFun, restFun) );
} // Term::split


//
//  void Term::split ( List* ts, int m, int i, int r, FSymbol* g, FSymbol* h )
//    given the list ts = (t_1,...,t_mi+r), return the list
//    g(t_1,...,t_m),...,g(t_m(i-1)+1,...,t_mi),h(t_mi+1,...,t_mi+r)
//
Term::List* Term::split ( List* ts, int maxArity, int iterations, int restArity, 
                          FSymbol* firstFun, FSymbol* restFun )
{
  // note that the arguments are already splitted
  if ( iterations == 0 ) {  // no more iterations
    // return h(t_mi+1,...,t_mi+r), if r != 0 or 
    // the empty list if r = 0
    if ( restArity == 0 ) { // r = 0
      ASS( debug_term, ts->isEmpty() );

      return Term::List::empty ();
    }

    // restArity != 0;
    return new List ( new Term (restFun, ts) );
  }

  // iterations != 0
  // chop from the head of ts a list of length maxArity
  List* last = ts;

  for ( int i = maxArity - 1; i > 0; i-- ) {
    last = last->tail ();
  } 
  
  // now last points to the last chain before the rest
  // rest will be used as the rest of the input list, without the first m members
  List* rest = last->tail();
  
  // make ts the list of the form (t_1,...,t_m)
  last->tail ( Term::List::empty() );

  return new List ( new Term (firstFun,ts),
                        split ( rest, maxArity, iterations - 1, restArity, firstFun, restFun ) );
    
} // Term::split


Term::List* Term::splitLargeArity ( Term::List* ts, FSymbol::List* newFs )
{
  if ( newFs->isEmpty() ) {
    ASS ( debug_term, ts->isEmpty() );
    return Term::List::empty ();
  }

  // 
  List* rest;
  FSymbol* f = newFs->head();
  List* first = ts->split (f->arity(), rest);
  return new Term::List ( new Term (f, first),
                          splitLargeArity (rest, newFs->tail()) );

} // Term::splitLargeArity


void Term::splitLargeArity (const FSymbol* f, FSymbol::List* newFs )
{
  if ( isVar() )
    return;

  Term::List::Iterator ts ( args() );
  while ( ts.more() ) {
    ts.next()->splitLargeArity (f,newFs);
  }

  if ( functor() == f ) {
    args ( Term::splitLargeArity (args(),newFs) );
  }
} // Term::splitLargeArity


bool Term::isGround () const
{
  if ( isVar() )
    return false;

  Term::List::Iterator ts ( args() );
  while ( ts.more() ) {
    if ( ! ts.next()->isGround() ) {
      return false;
    }
  }

  return true;
} // Term::isGround
*/


// 03/06/2001
// ls can be obtained from rs by swapping x and y
// 30/04/2002 Manchester, changed
bool Term::equalUpTo ( const List* ls, const List* rs, Var x, Var y )
{
  Term::List::Iterator ts1 (const_cast<List*>(ls));
  Term::List::Iterator ts2 (const_cast<List*>(rs));

  while ( ts1.more() ) {
    // ls and rs must be of the same length
    ASS ( debug_term, ts2.more() );
    if (! ts1.next()->equalUpTo (ts2.next(), x, y) ) {
      return false;
    }
  }
  ASS ( debug_term, ! ts2.more() );

  return true;
} // Term::equalUpTo


// 30/04/2002 Manchester
// rcan be obtained from this by swapping x and y
bool Term::equalUpTo ( const Term* r, Var x, Var y ) const
{
  if ( isVar() ) { // this is a variable
    if (! r->isVar() ) { // but r is not
      return false;
    }
    // both this and r are variables
    if (var() == r->var()) {
      return true;
    }
    // this and r are different vars
    return (var() == x && r->var() == y) ||
           (var() == y && r->var() == x);
  }

  // this is not a variable
  if ( r->isVar() ) {
    return false;
  }
  // this,r are non-variables
  return functor() == r->functor() &&
         equalUpTo(args(),r->args(),x,y);
} // Term::equalUpTo


// given a sequence of variables v1,...,vn
// return the sequence of terms v1,...,vn
// 03/06/2001
Term::List* Term::varsToTerms (const VarList* vs)

{
  if ( vs->isEmpty() )
    return List::empty();

  return new List ( new Term (vs->head()), 
		    varsToTerms(vs->tail()) );
} // Term::varsToTerms


// true if any of the terms has free variables not occurring in vs
// 06/05/2002 Manchester
bool Term::hasVarsNotIn (const List* ts, const VarListList* vs)
{
  Term::List::Iterator terms (ts);
  while ( terms.more() ) {
    Term* t = terms.next ();
    if ( t->isVar() ) {
      // check that the variable does not occur in any of the lists in vs
      Var v = t->var ();

      VarListList::Iterator vss (vs);
      bool found = false;
      while ( vss.more() ) {
        if ( vss.next()->member(v) ) { // found
          found = true;
          break;
        }
      }
      if (! found ) {
        return true;
      }
    }
    else { // t is a compound term
      if ( hasVarsNotIn (t->args(),vs) ) {
        return true;
      }
    }
  }

  return false;
} // hasVarsNotIn


// ts consists of variables only
// 07/05/2002 Manchester
bool Term::varsOnly (const List* ts) 
{
  Term::List::Iterator t (ts);
  while ( t.more() ) {
    if (! t.next()->isVar() )
      return false;
  }

  return true;
} // Term::varsOnly


bool allIn (const Term* t, Sort<Var>& s)
{
  if ( t->isVar() ) {
    return s.member (t->var());
  }

  // non-variable
  Term::List::Iterator ts (t->args());
  while (ts.more()) {
    if ( ! allIn (ts.next(),s) ) {
      return false;
    }
  }

  return true;
} // allIn


// checks that this has the form t[x] and lhs has the form f(x)
// where f is a function symbol and t[x] is a term of x not containing f
// assumes both arguments are non-variables
// 09/05/2002, Manchester
bool Term::defines ( const Term* l ) const  // auxiliary function for atoms
{
  TRACER( "Term::defines" );

  List* ls = l->args();
  int ll = ls->length ();

  if ( ! varsOnly(ls) ||
       occurs( l->functor()) ) {
    return false;
  }

  // sorting variables in ls
  Sort<Var> lvar (ll);
  Term::List::Iterator largs (ls);
  while (largs.more()) {
    lvar.add (largs.next()->var());
  }
  lvar.sort ();
  lvar.removeDuplicates ();
  if (ll != lvar.length()) { // there was a duplicate removed, so lhs is not linear
    return false;
  }

  // now we have to check that every variable occuring in l 
  // also occurs in the sorted list
  return allIn (this, lvar);
} // Term::defines


// 27/06/2002 Manchester
Term* Term::apply ( const Substitution& subst ) 
{
  if (isVar()) {
    Var v = var();
    Term* t = subst.get (v);
    if (t) {
      return t;
    }
    // variable is not bound
    return this;
  }

  return new Term ( functor(), 
		    Term::apply (args(), subst) );
} // Term::apply


// 27/06/2002 Manchester
Term::List* Term::apply ( const List* ts, const Substitution& subst  )
{
  if (ts->isEmpty()) {
    return List::empty ();
  }

  return new List ( ts->head()->apply(subst), 
		    apply (ts->tail(), subst) );
} // Term::apply


// 14/07/2002 Manchester
Term* Term::rectify ( Substitution& subst, Var& last, VarList*& freeVars ) 
{
  if (isVar()) {
    Var v = var();
    Term* t = subst.get (v);
    if (t) {
      return t;
    }
    // variable is not bound
    Var newV = ++last;
    Term* s = new Term (newV);
    subst.push (v, s);
    VarList::push (newV, freeVars);
    return s;
  }

  return new Term ( functor(), 
		    Term::rectify (args(), subst, last, freeVars) );
} // Term::rectify


// 14/07/2002 Manchester
Term::List* Term::rectify ( const List* ts, 
			    Substitution& subst, 
			    Var& last, 
			    VarList*& freeVars)
{
  if (ts->isEmpty()) {
    return List::empty ();
  }

  return new List ( ts->head()->rectify(subst, last, freeVars), 
		    rectify (ts->tail(), subst, last, freeVars) );
} // Term::rectify


// output term list in the KIF format
// 24/07/2002 Torrevieja
void Term::outputKIF (Term::List* terms, ostream& str)
{
  Term::List::Iterator ts (terms);
  while (ts.more()) {
    Term* term = ts.next();
    str << ' ';
    if (term->isVar()) {
      str << "?X" << term->var();
    }
    else { // non-variable
      Term::List* args = term->args ();
      if (args) {
	str << '(';
      }
      str << term->functor();
      if (args) {
	outputKIF (args, str);
      }
    }
  }
} // outputKIF


