//
//  file formula.cpp
//  implements class Formula
//

#include <string.h>


#include "term.h"
#include "atom.h"
#include "tracer.h"
#include "formula.h"
#include "sort.h"
#include "substitution.h"


// 14/06/2001
// this function must only be used by the parser
// it builds a new formula of the form (l c r)
// use of the function outside of the parser may cause 
// segment violation
Formula* Formula::parseJunction ( Connective c, Formula* l, Formula* r ) 
{
  ASS (debug_formula, c == AND || c == OR);

  if ( r->connective() == c ) {
    if ( l->connective() == c ) { // both r and l are c-junctions
      l->_args = Formula::List::concat( l->args(), r->args () );
      delete r;
      return l;
    }
    // only r is a c-junction
    List* as = new List ( l, r->args() );
    r->args (as);
    return r;
  }
  // r is not a c-junction
  if ( l->connective() == c ) { // l is a c-junction
    List* last = new List(r);
    l->_args = Formula::List::concat ( l->args(), last );
    return l;
  }
  
  // neither l nor r is a c-junction
  return new Formula (c, new List (l, new List(r)));
} // Formula::junction


/*
// size is the total number of occurrences of connectives, quantifiers, and atoms
// in the formula
int Formula::size () const
{
  switch ( connective () ) {
    case ATOM: 
      return 1;

    case AND:
    case OR: {
      int sz = -1;
      
      for ( List* fs = args(); fs->isNonEmpty (); fs = fs->tail() )
        sz += fs->head()->size() + 1;
         
      return sz;
    }
        
    case IMP:
    case IFF:
    case XOR:
      return left()->size() + right()->size () + 1;

    case NOT:
      return arg()->size () + 1;

    case FORALL:
    case EXISTS:
      return vars()->length() + arg()->size () + 1;

    default:
      ASS ( debug_formula, false );
      return 0;
  }
} // Formula::size


// the total number of symbols in the formula
// a bug fixed 30/06/2001, flight Kopenhagen-Manchester
int Formula::weight () const
{
  switch ( connective () ) {
    case ATOM: 
      return atom()->weight();

    case AND:
    case OR: {
      int sz = -1;
      
      for ( List* fs = args(); fs->isNonEmpty (); fs = fs->tail() )
        sz += fs->head()->weight() + 1;
         
      return sz;
    }
        
    case IMP:
    case IFF:
    case XOR:
      return left()->weight() + 
             right()->weight () + 1;

    case NOT:
      return arg()->weight () + 1;

    case FORALL:
    case EXISTS:
      return vars()->length() + arg()->weight () + 1;

    default:
      ASS ( debug_formula, false );
      return 0;
  }
} // Formula::weight
*/

// the print name of connectives and quantifiers, spaces not included
//  30/06/2001, flight Copenhagen-Manchester, ASS added
char* Formula::printName ( Connective c )
{
  switch ( c ) {
    case AND:
      return "&";
    case OR:
      return "|";
    case IMP:
      return "=>";
    case NOT:
      return "~";
    case IFF:
      return "<=>";
    case XOR:
      return "<~>";
    case FORALL:
      return "!";
    case EXISTS:
      return "?";
    default:
      ASS(debug_formula, false);
      return "";
  }
} // Formula::printName


ostream& operator << ( ostream& str, VarList* vs )
{
  ASS (debug_formula, ! vs->isEmpty ());

  VarList* tail = vs ->tail ();
  
  while ( ! tail->isEmpty () ) {
    str << 'X' << vs->head () << ',';
    vs = tail;
    tail = tail->tail ();
  }

  str << Term::varPrefix << vs->head ();

  return str;
} // operator << ( ostream& str, VarList* vs )


ostream& operator << ( ostream& str, Formula* f )
{
  f->print ( str );

  return str;
} // operator << ( ostream& str, Formula* f )


void Formula::print ( ostream& str )
{
  print ( ATOM, str );
}


void Formula::prettyPrint ( int indent, ostream& str )
{
  for ( int i = indent; i > 0; i-- )
    str << ' ';

  prettyPrint ( ATOM, indent, str );
} // Formula::prettyPrint


void Formula::print ( Connective out, ostream& str )
{
  bool par = false; // parentheses required

  switch ( connective () ) {
    case ATOM: 
      
      str << atom ();
      return;

    case AND:
      switch ( out ) {
        case OR:
        case NOT:
          par = true;
        default:
          break;
      }
      goto and_or;

    case OR: {
      switch ( out ) {
        case AND:
        case NOT:
          par = true;
        default:
          break;
      }

      and_or:
      if ( par ) str << '(';

      List* fs = args ();
      List* next = fs->tail ();

      while ( next->isNonEmpty () ) {
        fs->head()->print ( connective(), str );
        str << ' ' << printName ( connective () ) << ' ';
        fs = next;
        next = next->tail ();
      }

      fs->head()->print ( connective(), str );

      if ( par ) str << ')';
      return;
    }

    case IMP:
      switch ( out ) {
        case AND:
        case OR:
        case IMP:
        case NOT:
          par = true;
        default:
          break;
      }

      binary:
      if ( par ) str << '(';
      left()->print ( connective(), str );
      str << ' ' << printName ( connective () ) << ' ';
      right()->print ( connective(), str );
      if ( par ) str << ')';
      return;

    case NOT:

      str << printName ( NOT );
      arg()->print ( NOT, str );
      return;

    case IFF:
    case XOR:

      if ( out != ATOM ) 
        par = true;
      goto binary;

    case FORALL:
    case EXISTS:

      str << printName ( connective () ) << " [" << vars() << "] : ";
      arg()->print ( NOT, str );
      return;

    default:
      ASS ( debug_formula, false );
  }
} // Formula::print ( Connective out, ostream& str )


// 01/05/2002 Manchester, changed using printOneLine instead of size
void Formula::prettyPrint ( Connective out, int indent, ostream& str )
{
  // short formulas are printed on one line
  // for VERY long formulas the call of length () may be inefficient
  if ( printOneLine() ) {
    print ( out, str );
    return;
  }

  bool par = false; // parentheses required

  int i; // indentation counter

  switch ( connective () ) {
    case AND:
      switch ( out ) {
        case OR:
        case NOT:
          par = true;
        default:
          break;
      }

      andor: {
        if ( par ) {
          str << "( ";
          List* fs = args();
          fs->head()->prettyPrint (connective(), indent + 2, str );
          fs = fs->tail();
        
          while ( fs->isNonEmpty() ) {
            str << '\n';
            for ( int i = indent - strlen (printName(connective())); i > 0; i-- )
              str << ' ';

            str << printName ( connective() ) << ' ';
            fs->head()->prettyPrint ( connective(), indent + 2, str );
            fs = fs->tail();
          }
          str << " )";
        }
        else {
          List* fs = args();
          fs->head()->prettyPrint ( connective(), indent, str );
          fs = fs->tail();
        
          while ( fs->isNonEmpty() ) {
            str << '\n';
            for ( int i = indent - 1; i > 0; i-- )
              str << ' ';

            str << printName ( connective() ) << ' ';
            fs->head()->prettyPrint ( connective(), indent + strlen (printName(connective())) + 1, str );
            fs = fs->tail();
          }
        }
      }
      return;

    case OR:
      switch ( out ) {
        case AND:
        case NOT:
          par = true;
        default:
          goto andor;
      }

    case IMP:
      switch ( out ) {
        case AND:
        case OR:
        case IMP:
        case NOT:
          par = true;
        default:
          break;
      }

      binary:
      // first, print left subformula
      if ( par ) {
        str << " (";
        left()->prettyPrint ( connective(), indent+2, str );
      }
      else {
        str << " ";
        left()->prettyPrint ( connective(), indent+1, str );
      }
      str << '\n';

      // then print the indentation and the connective
      for ( i = indent; i > 0; i-- )
        str << ' ';
      str << printName ( connective () ) << ' ';
      if ( out == connective () &&
           (out == AND || out == OR) )
        right()->prettyPrint ( connective(), indent, str );
      else 
        right()->prettyPrint ( connective(), indent + 2 + strlen (printName (connective())), str );
      if ( par ) str << ')';
      return;

    case NOT:

      str << printName ( NOT ) << ' ';
      arg()->prettyPrint ( NOT, indent + strlen (printName(NOT)) + 1, str );
      return;

    case IFF:
    case XOR:

      if ( out != ATOM ) 
        par = true;
      goto binary;

    case FORALL:
    case EXISTS:

      str << printName ( connective () ) << " [" << vars() << "] :\n";
      for ( i = indent; i >= 0; i-- )
        str << ' ';
      arg()->prettyPrint ( NOT, indent + 1, str );
      return;

    default:
      ASS ( debug_formula, false );
  }
} // Formula::prettyPrint ( Connective out, ostream& str )


// 01/05/2002, Manchester
bool Formula::printOneLine () const
{
  int limit = 1; // formulas of size <= 2 are printed in one line
                 // but a formula is expected to have at least size 1
  return printOneLine (limit);
} // Formula::printOneLine


// 01/05/2002, Manchester
bool Formula::printOneLine ( int& limit ) const
{
  TRACER("Formula::printOneLine");

  switch ( connective() ) {
    case ATOM:
      return true;

    case OR:
    case AND: {
      limit -= args()->length() - 2;
      if (limit < 0) {
        return false;
      }
      Formula::List::Iterator as (args());
      while ( as.more() ) {
        if (! as.next()->printOneLine(limit) ) {
          return false;
        }
      }
      return true;
    }

    case IMP: 
    case IFF:
    case XOR:
      limit -= 2;
      if (limit < 0) {
        return false;
      }
      return left()->printOneLine (limit) &&
             right()->printOneLine (limit);

    case NOT:
      if (--limit < 0) {
        return false;
      } 
      return arg()->printOneLine (limit);

    case FORALL:
    case EXISTS:
      limit -= vars()->length() + 1;
      if (--limit < 0) {
        return false;
      } 
      return arg()->printOneLine (limit);

    default:
      ASS ( debug_formula, false );
      return false;
  }
} // Formula::printOneLine


// rectify the formula 
// and universally quantify its free variables (added 14/07/2002)
// 27/06/2002 Manchester
Formula* Formula::rectify ()
{
  TRACER ("Formula::rectify");

  Substitution sbst;
  Var last = firstVar - 1;
  VarList* free = VarList::empty ();
  
  Formula* rectified = rectify (sbst, last, free);
  if (free->isEmpty()) {
    return rectified;
  }

  return new Formula (FORALL, free, rectified);
} // Formula::rectify


// rectify the formula 
// return in vs the list of free variables of vs 
// and in args the corresponding renamed list of terms
// 03/08/2002 Torrevieja
Formula* Formula::rectify (VarList*& vs, Term::List*& args)
{
  TRACER ("Formula::rectify");

  Substitution sbst;
  Var last = firstVar - 1;
  VarList* free = VarList::empty ();
  
  Formula* rectified = rectify (sbst, last, free);
  if (free->isEmpty()) {
    return rectified;
  }

  vs = sbst.originalVars ();
  args = sbst.terms ();

  return new Formula (FORALL, free, rectified);
} // Formula::rectify


// rectify the formula using previously collected substitution sbst
// and universally quantify its free variables (added 14/07/2002)
// 27/06/2002 Manchester
Formula* Formula::rectify ( Substitution& sbst, Var& last, VarList*& freeVars )
{
  TRACER ("Formula::rectify+");

  switch ( connective () ) {
  case ATOM:
    return new Formula (atom()->rectify (sbst, last, freeVars));

  case AND: 
  case OR: 
    return new Formula ( connective(), 
			 rectify (args(), sbst, last, freeVars) );

  case IMP: 
  case IFF: 
  case XOR: {
    Formula* l = left()->rectify (sbst, last, freeVars);
    Formula* r = right()->rectify (sbst, last, freeVars);
    return new Formula (connective(), l, r);
  }

  case NOT:
    return new Formula ( NOT, 
			 arg()->rectify (sbst, last, freeVars) );

  case FORALL: 
  case EXISTS: {
    VarList* vs = rectifyVars (vars(), sbst, last);
    Formula* result = new Formula ( connective(),
				    vs,
				    arg()->rectify (sbst, last, freeVars) );
    for (int i = vars()->length()-1; i >= 0; i--) {
      sbst.pop ();
    }
    return result;
  }

  default:
    ASS( debug_formula, false );
  }
} // Formula::rectify


Formula::List* Formula::rectify ( List* fs, 
				  Substitution& subst, 
				  Var& last, 
				  VarList*& freeVars )
{
  TRACER ("Formula::rectify++");

  if (fs->isEmpty()) {
    return List::empty ();
  }
  Formula* h = fs->head()->rectify (subst, last, freeVars);

  return new List (h, rectify (fs->tail(), subst, last, freeVars));
} // Formula::rectify


VarList* Formula::rectifyVars ( VarList* vs, Substitution& subst, Var& last )
{
  TRACER ("Formula::rectifyVars");

  if (vs->isEmpty()) {
    return VarList::empty ();
  }

  Var v = vs->head();
  Var newV = ++last;
  subst.push (v, new Term (newV));
  return new VarList ( newV,
		       rectifyVars (vs->tail(), subst, last) );
} // Formula::rectifyVars


// true if p occurs in this
bool Formula::occurs ( const Signature::Pred* p ) const
{
  switch ( connective () ) {
    case ATOM: 
      return p == atom()->functor ();

    case AND:
    case OR: {
      for ( List* fs = args(); fs->isNonEmpty(); fs = fs->tail() ) {
        if ( fs->head()->occurs(p) )
          return true;
      }
      return false;
    }

    case IMP:
    case IFF:
    case XOR:
      return left()->occurs ( p )
          || right()->occurs ( p );

    case NOT:
    case FORALL:
    case EXISTS:
      return arg()->occurs ( p );

    default:
      ASS ( debug_formula, false ) ;
      return false;
  }
} // Formula::occurs


// checks that the formula has the form forall x (P(x) <=> A[x]),
// where P is a predicate symbol and A[x] is a formula of x not containing P
// 04/05/2002, Manchester, bug fixed: previously it was not checked that x are all variables of A
Atom* Formula::isPredicateDefinition ( Formula*& rhs )
{
  // first, collect the variables occurring in the outermost quantifiers in a list,
  // sort it and remove duplicates
  int noOfVars = 0;   // number of vars in the outermost quantifier prefix

  Formula* f = this;
  // skip all universal quantifiers in front of the formula
  while ( f->connective() == FORALL ) {
    noOfVars += f->vars()->length ();
    f = f->arg ();
  }

  // check that f has the form lft <=> rht
  if ( f->connective() != IFF ) 
    return 0;

  Formula* lft = f->left ();
  Formula* rht = f->right ();
  Atom* la = lft->connective() == ATOM ? lft->atom() : 0;
  Atom* ra = rht->connective() == ATOM ? rht->atom() : 0;

  // we are only interested in lhs (or rhs) which are non-flat non-equality atoms
  // whose predicate symbols do not occur in the other side
  if ( la && 
        ( la->isEquality() ||
          ! la->isFlat() || 
          rht->occurs (la->functor()) ) ) {
    la = 0;
  }
  if ( ra && 
        ( ra->isEquality() ||
          ! ra->isFlat() || 
          lft->occurs (ra->functor()) ) ) {
    ra = 0;
  }

  if (! (la || ra) ) {
    return 0;
  }

  // now at least one of la, ra is flat
  // build an ordered list of the universally quantified variables of the formula prefix
  // with duplicates removed
  Sort<Var> univ (noOfVars);

  // add all variables to univ
  f = this;
  // skip all universal quantifiers in front of the formula
  while ( f->connective() == FORALL ) {
    VarList::Iterator vs (f->vars());
    while ( vs.more() ) {
      univ.add ( vs.next() );
    }
    f = f->arg ();
  }

  univ.sort ();
  univ.removeDuplicates ();
  int length = univ.length();

  // check that la (resp. ra) qualifies for the defined atom. This is done as follows:
  // the arguments of la are sorted. la qualifies iff the list of arguments coincides 
  // with the list univ 
  if (la) {
    if (la->functor()->arity() != length) {
      la = 0;
    }
    else {
      Sort<Var> lvar (length);
      Term::List::Iterator args (la->args());
      while (args.more()) {
        lvar.add (args.next()->var());
      }
      lvar.sort ();
      for (int i = 0; i < length;i++) {
        if ( lvar[i] != univ[i] ) {
          la = 0;
          break;
        }
      }
    }
  }
  if (la) {
    rhs = rht;
    return la;
  }

  if (ra) {
    if (ra->functor()->arity() != length) {
      ra = 0;
    }
    else {
      Sort<Var> rvar (length);
      Term::List::Iterator args (ra->args());
      while (args.more()) {
        rvar.add (args.next()->var());
      }
      rvar.sort ();
      for (int i = 0; i < length;i++) {
        if ( rvar[i] != univ[i] ) {
          ra = 0;
          break;
        }
      }
    }
  }
  if (ra) {
    rhs = lft;
    return ra;
  }

  return 0;
} // Formula::isPredicateDefinition


// checks that the formula has the form forall x (f(x) = t[x]),
// where f is a function symbol and t[x] is a term of x
// WARNING: it is assumed that the formula contains no free variables!
// 09/05/2002, Manchester
Term* Formula::isFunctionDefinition ( Term* & rhs ) const
{
  TRACER ( "Formula::isFunctionDefinition" );

  // first, collect the variables occurring in the outermost quantifiers in a list,
  // sort it and remove duplicates
  const Formula* f = this;
  // skip all universal quantifiers in front of the formula
  while ( f->connective() == FORALL ) {
    f = f->arg ();
  }

  // check that f is an atom
  if ( f->connective() != ATOM ) 
    return 0;

  return f->atom()->isDefinition (rhs);
} // Formula::isFunctionDefinition


/*
bool Formula::isLinearAndFlat ()
{
  return ( connective () == ATOM ) 
      && atom()->isLinearAndFlat ();      
} // Formula::isLinearAndFlat
*/


Formula::~Formula ()
{
  TRACER ( "Formula::~Formula" );
  ASS (debug_formula, _count == 0);

  switch ( connective () ) {
    case ATOM:
      atom()->dec ();
      break;

    case AND:
    case OR: 
      args()->dec ();
      break;

    case IMP:
    case IFF:
    case XOR:
      left()->dec();
      right()->dec();
      break;

    case NOT:
      arg()->dec ();
      break;

    case FORALL:
    case EXISTS:
      vars()->destroy ();
      arg()->dec ();
      break;
  }; 
} // Formula::~Formula


/*
int Formula::countOccurrences ( const FSymbol* f ) const
{
  switch ( connective() ) {
    case ATOM:
      return atom()->countOccurrences(f);

    case AND:
    case OR: {
      int occ = 0;

      for ( List* fs = args(); fs->isNonEmpty(); fs = fs->tail() )
        occ += fs->head()->countOccurrences ( f );

      return occ;
    }

    case IMP:
    case IFF:
    case XOR:
      return left()->countOccurrences(f) + 
             right()->countOccurrences(f);

    case NOT:
    case FORALL:
    case EXISTS:
      return arg()->countOccurrences(f);

    default:
      ASS ( debug_formula, false );
      return 0;
  }
}


int Formula::countOccurrences ( const PSymbol* p ) const
{
  switch ( connective() ) {
    case ATOM:
      return atom()->functor() == p ? 1 : 0;

    case AND:
    case OR: {
      int occ = 0;

      for ( List* fs = args(); fs->isNonEmpty(); fs = fs->tail() )
        occ += fs->head()->countOccurrences ( p );

      return occ;
    }

    case IMP:
    case IFF:
    case XOR:
      return left()->countOccurrences(p) + 
             right()->countOccurrences(p);

    case NOT:
    case FORALL:
    case EXISTS:
      return arg()->countOccurrences(p);

    default:
      ASS ( debug_formula, false );
      return 0;
  }
}


// 02/06/2001, copied from Clause
void Formula::split ( Formula* l, List* spl, 
                      List*& less, List*& equal, List*& greater )
{
  ASS ( debug_clause, spl->isNonEmpty() );

  less = Formula::List::empty();
  equal = Formula::List::empty();
  greater = Formula::List::empty();

  for (;;) {
    List* tail = spl->tail ();

    switch ( spl->head()->compare(l) ) {
      case GREATER:
        spl->tail ( greater );
        greater = spl;
        break;
      case EQUAL:
        spl->tail ( equal );
        equal = spl;
        break;
      case LESS:
        spl->tail ( less );
        less = spl;
        break;
    }

    if ( tail->isEmpty() )
      return;
    spl = tail;
  }
} // Formula::split
*/


// 03/06/2001
// 29/04/2002, changed to use arbitrary universal quantifiers and call function on atoms
bool Formula::isReflexivityAxiom () const
{
  TRACER("Formula::isReflexivityAxiom");
  const Formula* f = this;
  while ( f->connective() == FORALL ) {
    f = f->arg ();
  }

  // f must be t = t
  return f->connective() == ATOM &&
         f->atom()->isTautology();
} // Formula::isReflexivityAxiom


// 03/06/2001
// 29/04/2002, changed to use arbitrary universal quantifiers and call function on atoms
bool Formula::isSymmetryAxiom () const
{
  TRACER("isSymmetryAxiom");
  const Formula* f = this;
  while ( f->connective() == FORALL ) {
    f = f->arg ();
  }

  // f must be x = y -> y = x
  if ( f->connective() != IMP ) {
    return false;
  }

  // f1 must be x = y 
  Formula* f1 = f->left ();
  if ( f1->connective() != ATOM ) {
    return false;
  }
  // f2 must be y = x
  Formula* f2 = f->right ();
  if ( f2->connective() != ATOM ) {
    return false;
  }

  return f1->atom()->swap (f2->atom());
} // Formula::isSymmetryAxiom


// 03/06/2001
// 29/04/2002, changed to use arbitrary universal quantifiers and call function on atoms
bool Formula::isTransitivityAxiom () const
{
  TRACER("isTransitivityAxiom");
  const Formula* f = this;
  while ( f->connective() == FORALL ) {
    f = f->arg ();
  }

  // f must be x = y & y = z -> x = z
  if ( f->connective() != IMP ) {
    return false;
  }

  // c must be x = y & y = z
  Formula* c = f->left ();
  if ( c->connective() != AND || c->args()->length() != 2 ) {
    return false;
  }

  // c1,c2 must be x = y and y = z
  Formula* c1 = c->args()->head ();
  if ( c1->connective() != ATOM ) {
    return false;
  }
  Formula* c2 = c->args()->second ();
  if ( c2->connective() != ATOM ) {
    return false;
  }
  // c3 must be x = z
  Formula* c3 = f->right();
  if ( c3->connective() != ATOM ) {
    return false;
  }

  return Atom::transitivity (c1->atom(), c2->atom(), c3->atom());
} // Formula::isTransitivityAxiom


// 03/06/2001
// 29/04/2002, changed to use arbitrary universal quantifiers and call function on atoms
bool Formula::isFunctionReflexivityAxiom () const
{
  TRACER("isFunctionReflexivityAxiom");
  const Formula* f = this;
  while ( f->connective() == FORALL ) {
    f = f->arg ();
  }

  // f must be x = y -> F(x)=F(y)
  if ( f->connective() != IMP ) {
    return false;
  }

  // both subformulas must be atoms
  if ( f->left()->connective() != ATOM || 
       f->right()->connective() != ATOM ) {
    return false;
  }
  
  return Atom::functionMonotonicity (f->left()->atom(), f->right()->atom());
} // Formula::isFunctionReflexivityAxiom


// 03/06/2001
// 29/04/2002, changed to use arbitrary universal quantifiers and call function on atoms
bool Formula::isPredicateReflexivityAxiom () const
{
  TRACER("isPredicateReflexivityAxiom");
  const Formula* f = this;
  while ( f->connective() == FORALL ) {
    f = f->arg ();
  }

  // f must be x = y & P(x) -> P(y)
  if ( f->connective() != IMP ) {
    return false;
  }

  // c must be x = y & P(x) (but in unclear order)
  Formula* c = f->left ();
  if ( c->connective() != AND || c->args()->length() != 2 ) {
    return false;
  }
  Formula* c1 = c->args()->head();
  if ( c1->connective() != ATOM ) {
    return false;
  }
  Formula* c2 = c->args()->second();
  if ( c2->connective() != ATOM ) {
    return false;
  }
  Formula* c3 = f->right (); // P(y)
  if ( c3->connective() != ATOM ) {
    return false;
  }
  Atom* a1 = c1->atom();
  Atom* a2 = c2->atom();
  Atom* a3 = c3->atom();

  return Atom::predicateMonotonicity (a1,a2,a3) ||
         Atom::predicateMonotonicity (a2,a1,a3);
} // Formula::isPredicateReflexivityAxiom


// 03/06/2001
bool Formula::isEqualityAxiom () const
{
  TRACER("Formula::isEqualityAxiom");

  return isReflexivityAxiom () || 
         isSymmetryAxiom () ||
         isTransitivityAxiom () ||
         isFunctionReflexivityAxiom () ||
         isPredicateReflexivityAxiom ();
} // Formula::isEqualityAxiom


// true if v has a free occurrence in the formula
// 13/06/2001 Manchester
bool Formula::occurs (Var v) const
{
  switch ( connective() ) {
    case ATOM:
      return atom()->occurs (v);

    case AND:
    case OR: {
      Formula::List::Iterator fs (args());
      while (fs.more()) {
        if ( fs.next()->occurs (v) ) 
          return true;
      }
      return false;
    }

    case IFF:
    case XOR:
    case IMP:
      return left()->occurs (v) ||
             right()->occurs (v);

    case FORALL:
    case EXISTS:
      if ( vars()->member(v) ) 
        return false;
      return arg()->occurs(v);

    case NOT:
      return arg()->occurs(v);

    default:
      ASS (debug_formula,false);
      return 0;
  }
} // Formula::occurs (Var v)


// true if the formula has free variables
// 06/05/2002 Manchester
bool Formula::hasFreeVars () const
{
  return hasFreeVars (VarListList::empty());
} // Formula::hasFreeVars


// true if the formula has free variables not occurring in vs
// 06/05/2002 Manchester
bool Formula::hasFreeVars (VarListList* vs) const
{
  TRACER( "Formula::hasFreeVars" );

  switch ( connective() ) {
    case ATOM:
      return atom()->hasVarsNotIn (vs);

    case AND: {
    case OR:
      Formula::List::Iterator arg (args());
      while ( arg.more() ) {
        if ( arg.next()->hasFreeVars(vs) ) {
          return true;
        }
      }
      return false;
    }

    case IMP:
    case IFF:
    case XOR:
      return left()->hasFreeVars (vs) ||
             right()->hasFreeVars (vs);

    case NOT:
      return arg()->hasFreeVars (vs);

    case FORALL:
    case EXISTS: {
      VarListList::push (vars(),vs);
      bool result = arg()->hasFreeVars (vs);
      VarListList::pop (vs);
      return result;
    }

    default:
      ASS (debug_formula,false);
      return false;
  }
} // Formula::hasFreeVars


// if the formula can be CHANGED to formula A1 & ... & An return the 
// list [A1,..,An]. Otherwise, return 0
// 12/05/2002 Manchester
Formula::List* Formula::conjunctionsOut (bool polarity) 
{
  switch ( connective() ) {
    case ATOM:
    case IFF:
    case XOR:
    case FORALL:
    case EXISTS: 
      return 0;

    case AND: {
      if ( ! polarity ) { // essentially, a disjunction
        return 0;
      }
      List* fs = conjunctionsOut (args(), true);
      if ( fs == args() ) {
        return 0;
      }
      return fs;
    }

    case OR: 
      if ( polarity ) { // a real disjunction
        return 0;
      }
      // now, essentially a conjunction
      return conjunctionsOut (args(), false);

    case IMP: {
      if ( polarity ) { // essentially, a disjunction
        return 0;
      }
      // polarity = false
      List* ls = left()->conjunctionsOut (true);
      List* rs = right()->conjunctionsOut (false);
      if (ls) { // left has changed
        if (! rs) {
          rs = new List (new Formula (NOT,right()));
        }
        return Formula::List::concat (ls,rs);
      }
      // left has not changed
      if (! rs) { // rs 
        return 0;
      }
      // only right has changed
      if (left()->connective() == AND) {
        return left()->args()->append (rs);
      }
      return new List (left(), rs);
    }

    case NOT: 
      return arg()->conjunctionsOut (!polarity);

    default:
      ASS (debug_formula,false);
      return 0;
  }
} // Formula::conjunctionsOut 


// auxiliary function for conjunctionsOut (bool) 
// 12/05/2002 Manchester
Formula::List* Formula::conjunctionsOut (List* fs, bool polarity) 
{
  if ( fs->isEmpty() ) {
    return fs;
  }

  Formula* h = fs->head();
  List* h1 = h->conjunctionsOut (polarity);
  List* ts = fs->tail ();
  List* ts1 = conjunctionsOut (ts, polarity);

  if (polarity) { // real conjunction
    if (h1) { // head has been changed
      return Formula::List::concat (h1, ts1);
    }
    // head has not been changed
    if (ts == ts1) { // nothing has changed
      return fs;
    }
    // only tail has changed
    if ( h->connective() == AND ) {
      return h1->append (ts1);
    }
    // h->connective() != AND
    return new List (h, ts1);
  }

  // polarity is false. Now the tail either is empty or must have changed, i.e. 
  // ts = [] or ts != ts1
  if (! h1) { // head has not changed
    ASS( debug_formula, h->connective() != AND );

    return new List (new Formula(NOT, h), ts1);
  }

  // now h has changed. Remember that the tail either is empty or must have changed
  return Formula::List::concat (h1, ts1);
} // Formula::conjunctionsOut 


// transform each formula into ennf
// 12/05/2002 Manchester
Formula* Formula::ennf (bool polarity)
{
  switch ( connective() ) {
    case ATOM:
      if (polarity) {
        return this;
      }
      return new Formula (NOT, this);

    case AND: 
    case OR: {
      Formula* f = ennf (polarity == (connective() == AND) ? AND : OR, args(), polarity);
      if (f) { 
        return f;
      }
      return this;
    }
      
    case IMP: {
      Formula* l = left()->ennf (!polarity);
      Formula* r = right()->ennf (polarity);

      bool lj = (polarity && l->connective() == OR) || // left is the right kind of junction
                (!polarity && l->connective() == AND);
      bool rj = (polarity && r->connective() == OR) || // right is the right kind of junction
                (!polarity && r->connective() == AND);

      if (lj) { // l1 & ... & ln
        if (rj) { // l1 & ... & ln, r1 & ... & rm
          List* ars = l->args()->append(r->args());

          if (l->_count != 0) { // l is reused 
            if (r->_count != 0) { // r is reused
              return new Formula (polarity ? OR : AND, ars);
            }
            // r is new, l is reused
            r->args ( ars );
            return r;
          }
          // l is new
          if (r->_count != 0) { // r is reused
            l->args (ars);
            return l;
          }
          // both are new
          l->args (ars);
          delete r;
          return l;
        }
        // l1 & ... & ln, r0
        List* ars = l->args()->append(new List(r));
        if (l->_count == 0) { // l is new
          l->args(ars);
          return l;
        }
        // l is reused
        return new Formula ( polarity ? OR : AND, ars);
      }
      // l0
      if (rj) { // r1 & ... & rm
        List* ars = new List (l, r->args());
        if (r->_count != 0) { // r is reused
          return new Formula (polarity ? OR : AND, ars);
        }
        // r is new 
        r->args ( ars );
        return r;
      }
      // neither l0, r0
      return new Formula ( polarity ? OR : AND,
                           new List (l, new List (r)) );
    }

    case IFF: 
    case XOR: {
      Formula* l = left()->ennf (true);
      Formula* r = right()->ennf (true);
      if (polarity && l == left() && r == right()) { // nothing has changed
        return this;
      }
      return new Formula (polarity == (connective() == IFF) ? IFF : XOR, l, r);
    }

    case NOT:
      if (polarity && arg()->connective() == ATOM) {
        return this;
      }
      return arg()->ennf(!polarity);

    case FORALL:
    case EXISTS: {
      Connective c = polarity == (connective() == FORALL) ? FORALL : EXISTS;
      Formula* n = arg()->ennf(polarity);

      if (n->connective() == c) { // Ex Ey A
        // change into Exy A
        Formula* res = new Formula (c, vars()->append(n->vars()->copy()), n->arg());
        if (n->_count == 0) {
          delete n;
        }
        return res;
      }

      // n->connective != c
      if (n == arg()) { // the formula has not changed
        ASS(debug_formula, polarity);
        return this;
      }
      // n->connective != c, n != arg
      return new Formula (c, vars()->copy(), n);
    }

    default:
      ASS( debug_formula, false );
      return false;
  }
} // Formula::ennf


// transform a list of formulas into ennf, returns 0 if the list does not change
// 12/05/2002 Manchester
Formula* Formula::ennf (Connective c, List* fs, bool polarity)
{
  ASS( debug_formula, c == AND || c == OR );
  ASS( debug_formula, fs->isNonEmpty() );

  Formula* hd = fs->head();
  List* tl = fs->tail();

  Formula* h = hd->ennf (polarity);
  if (!tl) { // not really a list
    return h == hd ? 0 : h;
  }

  // tl is non-empty
  Formula* t = ennf (c, tl, polarity);
  if (!t && h == hd) { // nothing changed
    return 0;
  }

  // computing new arguments
  if (h->connective() == c) { // h1 & ... & hn
    if (t) {  // tl has changed
      if (t->connective() == c) { // t1 & ... & tm
        List* ars = h->args()->append(t->args()); // h1 & ... & hn & t1 & ... & tm
        if (h->_count == 0) { // h new
          h->args (ars);
          if (! t->_count) {
            delete t;
          }
          return h;
        }
        // h is not new
        if (t->_count == 0) { // t new
          t->args (ars);
          return t;
        }
        // h,t reused
        return new Formula (c, ars);
      }
      
      // h1 & ... & hn, t0
      List* ars = h->args()->append(new List(t)); // h1 & ... & hn & t0
      if (h->_count == 0) { // h new
        h->args (ars);
        if (t->_count == 0) {
          delete t;
        }
        return h;
      }
      // h1 & ... & hn, t0; h is reused
      Formula* res = new Formula (c, ars);
      if (t->_count == 0) {
        delete t;
      }
      return res;
    }
    // h1 & ... & hn; tl has not changed
    List* ars = h->args()->append(tl);
    if (h->_count == 0) { // h new
      h->args(ars);
      return h;
    }
    // h1 & ... & hn; tl has not changed, h reused
    return new Formula (c, ars);
  }

  // h0
  if (t) {  // tl has changed
    if (t->connective() == c) { // t1 & ... & tm
      List* ars = new List (h, t->args()); // h0 & t1 & ... & tm
      if (t->_count == 0) { // t new
        t->args (ars);
        return t;
      }
      // t is not new
      return new Formula (c, ars);
    }
    
    // h0, t0
    return new Formula (c, new List(h, new List(t)) );
  }

  // h0; tl has not changed
  return new Formula (c, new List(h,tl));
} // Formula::ennf


// remove equivalence from ennf
// 14/05/2002 Manchester
Formula* Formula::removeIff (bool polarity)
{
  switch ( connective() ) {
    case ATOM:
      return polarity ? this : new Formula(NOT, this);

    case AND:
    case OR: {
      List* fs = removeIff (args(), polarity);
      if (polarity && fs == args()) { // nothing changed
        return this;
      }
      return new Formula (polarity == (connective() == AND) ? AND : OR, fs);
    }

    case IFF: 
    case XOR: {
      Formula* l = left()->removeIff(true);
      Formula* r = right()->removeIff(true);
      if (polarity == (connective() == IFF)) {  // essentially iff
        // return (l => r) && (r => l)
        return new Formula (AND, 
                     new List (new Formula (IMP, l, r),
                     new List (new Formula (IMP, r, l) ) ) );
      }
      // essentially, XOR, 
      //  return (l => ~r) & (~l => r)
      return new Formula (AND, 
                   new List (new Formula (IMP, l, new Formula(NOT, r)),
                   new List (new Formula (IMP, new Formula(NOT, l), r) ) ) );
    }

    case NOT:
      ASS( debug_formula, arg()->connective() == ATOM );
      return polarity ? this : arg ();

    case FORALL:
    case EXISTS: {
      Formula* n = arg()->removeIff(polarity);
      if (polarity && n == arg()) {
        return this;
      }
      return new Formula (polarity == (connective() == FORALL) ? FORALL : EXISTS, 
                          vars()->copy (), n);
    }

    case IMP:
    default:
      ASS( debug_formula, false );
      return false;
  }
} // Formula::removeIff


// remove equivalence from ennf
// 14/05/2002 Manchester
Formula::List* Formula::removeIff (List* fs, bool polarity)
{
  if (fs->isEmpty()) {
    return fs;
  }

  Formula* nf = fs->head()->removeIff(polarity);
  List* nfs = removeIff (fs->tail(), polarity);
  if (nf == fs->head() && nfs == fs->tail()) {
    return fs;
  }
  return new List (nf, nfs);
} // Formula::removeIff


// miniscope a flattened nnf
// 25/05/2002 Manchester
Formula* Formula::miniscope ()
{
  switch ( connective() ) {
    case ATOM:
      return this;

    case NOT:
      ASS(debug_formula, arg()->connective() == ATOM);
      return this;

    case AND:
    case OR: {
      List* nas = miniscope(connective(), args());
      if (nas == args()) {
        return this;
      }
      return new Formula (connective(), nas);
    }

    case EXISTS: 
    case FORALL: {
      Formula* down = arg()->miniscope();
      VarList* vs = down->occurring (vars());
      Formula* f = down->topMiniscope(connective(), vs);

      if (f) {  // top-miniscoping worked
        if (down != arg()) {
          ASS (debug_formula, down->_count == 0);
          delete down;
        }

        // f must be returned but memory cleaned
        if (vs != vars()) {
          vs->destroy();
        }
        return f;
      }

      // top-miniscoping down did not work
      if (down == arg()) {  // miniscoping did not change arg
        if (vs == vars()) { // it did not change vars as well
          return this;
        }
        // variables have changed
        if (vs->isEmpty()) { // none of the variables occurs in the formula
          return down;
        }
        // variables have changed but are non-empty
        return new Formula (connective(), vs, down);
      }
      
      // arg changed by miniscoping
      if (vs == vars()) { // but vars did not change
        return new Formula (connective(), vs->copy(), down);
      }

      // both arg and vars changed
      if (vs->isEmpty()) { // none of the variables occurs in the formula
        return down;
      }
      
      // both arg and vars changed, vars are non-empty
      return new Formula (connective(), vs, down);
    }

    case IMP:
    case IFF:
    case XOR:
    default:
      ASS( debug_formula, false );
      return false;
  }
} // Formula::miniscope


// return the list of variables in vs which have free occurrences in the formula
// if all variables occur in the formula, return vs itself, otherwise a new list
// 25/05/2002 Manchester
// WARNING: quadratic, can be improved considerably
VarList* Formula::occurring (VarList* vs)
{
  if (vs->isEmpty()) {
    return vs;
  }
  // vs is non-empty
  VarList* ws = occurring (vs->tail());
  if ( occurs(vs->head()) ) { // the first variable occurs in the formula
    return ws == vs->tail() ? // all variables in the tail occur in the formula
           vs :
           new VarList (vs->head(), ws);
  }

  // the first variable does not occur in the head
  return ws == vs->tail() ? // all variables in the tail occur in the formula
         ws->copy () :
         ws;
} // Formula::occurring


// miniscope each nnf in fs and return the resulting list
// (either fs itself or a new list). If any formula has the
// connective c, then flatten the list
// 26/05/2002 Manchester
// WARNING: fs must be flattened, i.e. no formula in fs has c as the main connective
Formula::List* Formula::miniscope (Connective c, List* fs)
{
  if (fs->isEmpty ()) {
    return fs;
  }
  ASS(debug_formula, fs->head()->connective() != c);
  
  Formula* g = fs->head()->miniscope();
  List* gs = miniscope(c, fs->tail());

  if (g != fs->head()) { // head was really miniscoped
    if (g->connective() == c) { // g is a newly introduced formula
      List* result = g->args()->append(gs);
      delete g;
      return result;
    }

    // the connective is not c
    return new List (g,gs);
  }

  // g == fs->head()
  if (fs->tail() == gs) { // tail did not change either
    return fs;
  }
  
  return new List (g,gs);
} // Formula::miniscope (Connective c, List* fs)


class Formula::Miniscope 
{
 public:
  // constructor, destructor
  Miniscope ( Formula::List*, VarList* );
  ~Miniscope ();
  VarList* nonZeros ();               // return the list of variables with at least one occurrence
  VarList* all ();                    // return the list of all variables
  VarList* ones (int& j);             // return a list of variables with exactly one occurrence
  inline int num (int i) const // the number of formulas among Fj in which vi occurs
    { return _N[i]; }; 
  bool noMiniscoping () const; // no miniscoping is possible
  Formula* topple (Formula::List*, Connective c);

 private:
  int _nfs;       // number of formulas
  int _nvs;       // number of variables
  bool** _occ;    // array or arrays to allocate occurrences
  bool* _mem;     // memory for storing _occ[i]
  bool** _index;   // for every variable, give its allocation in the array, needed only in the constructor
  Var* _vars;     // array of variables
  int* _N;        // N[i] is the number of formulas among Fj in which vi occurs
  Var _maxV;      // maximal variable in vs

  void count (const Formula*, int formNumber);
  void count (const Term*, int formNumber);
  bool same (int i1, int i2); // vi1 and vi2 occur in the same formulas
}; // class Formula::Miniscope, private class for miniscoping


// the formula F is a subformula of (Q vs)F and is miniscoped
// this function tries to push the quantifier (Q vs) down the formula
// the result if the new formula, if miniscoped and 0 otherwise
// 26/05/2002 Manchester
Formula* Formula::topMiniscope (Connective Q, VarList* vs)
{
  switch ( connective() ) {
    case ATOM: // nothing to miniscope
    case NOT:
      return 0;

    case AND:
    case OR: {
      if ( (connective() == AND) == (Q == FORALL) ) { 
        // forall X (F1 & ... & Fn) or
        // exists X (F1 \/ ... \/ Fn)
        return new Formula ( connective(), topMiniscope(Q, vs, args()) );
      }
      // forall x1 ... xm (F1 \/ ... \/ Fn) or
      // exists x1 ... xm (F1 & ... & Fn)

      // create a miniscope class. It counts the number of occurrences of xi in Fj
      Miniscope cnt (args(), vs);
      if ( cnt.noMiniscoping() ) {
        return 0;
      }

      // miniscoping is possible
      VarList* ws = cnt.nonZeros ();
      if ( cnt.noMiniscoping() ) { // no more miniscoping is possible
        // at least one variable had to be removed from vs
        ASS( debug_formula, vs->length() != ws->length()); 
        if ( ws->isEmpty() ) {
          return this;
        }
        // ws is non-empty
        return new Formula (Q, ws, this);
      }
      ws->destroy ();

      // further miniscoping is possible
      int jone;
      VarList* os;
      Formula* f = this;
      while ( (os = cnt.ones (jone)) != 0 ) {
        Formula* g = new Formula (connective(), pushVarsDown (f->args(), Q, jone, os));
        if (f != this) {
          ASS( debug_formula, f->_count == 0 );
          delete f;
        }
        f = g;
      }

      // now f contains the possibly miniscoped formula in which there is no variable
      // occurring in exactly one subformula
      Formula* g;
      while ( (g = cnt.topple (f->args(), Q)) != 0 ) {
        if (f != this) {
          ASS( debug_formula, f->_count == 0 );
          delete f;
        }
        f = g;
      }

      ASS( debug_formula, f != this );
      ws = cnt.all (); // all remaining variables
      if ( ws->isEmpty() ) {
        return f;
      }
      return new Formula (Q, ws, f);
    }

    case FORALL:
    case EXISTS: 
      if (connective() == Q) { 
        // this is a rare case, it can only happen when in 
        // (forall X)(exists Y)(forall Z)F Y did not occur in (forall Z)F
        // or in (forall X)(forall Z)F the quantifier (forall X) was miniscoped
        Formula* f = arg()->topMiniscope (Q,vs);
        if (! f) {
          return new Formula ( Q, vs->append (vars()->copy()), arg() );
        }
        // miniscoping arg worked
        if (f->connective() == Q) {
          Formula* result = new Formula ( Q, f->vars()->append(vars()->copy()), f->arg() );
          delete f;
          return result;
        }
        // miniscoping arg worked, f->connective != Q
        else {
          return new Formula ( Q, vars()->copy(), f );
        }
        return 0;
      }
      // (forall X)(exists Y)F 
      return 0;

    case IMP:
    case IFF:
    case XOR:
    default:
      ASS( debug_formula, false );
      return false;
  }

  return 0;
} // Formula::topMiniscope


// fs is a junction of miniscoped formulas, e.g. F1 & ... & Fn
// the result is the miniscoped version of (Q X)F1 & ... (Q vs)Fn
// 26/05/2002 Manchester
Formula::List* Formula::topMiniscope (Connective Q, VarList* X, List* fs)
{
  if (fs->isEmpty()) {
    return fs;
  }
  List* gs = topMiniscope (Q, X, fs->tail());

  // fs is non-empty
  Formula* down = fs->head();
  VarList* vs = down->occurring (X);
  if (vs->isEmpty()) { // X does not occur in F1, simply make the first formula F1
    return new List (down, gs);
  }

  Formula* f = down->topMiniscope(Q, vs);

  if (f) {  // top-miniscoping worked
    if (vs != X) { // vs was a new list
      vs->destroy();
    }
    return new List (f,gs);
  }

  // top-miniscoping down did not work, simply make the first formula (Q X)F1
  if (vs == X) {  // vs is not a new list
    // make it a new list
    vs = X->copy();
  }

  // at least one variable of X occurs in F1, so vs is non-empty
  return new List (new Formula(Q, vs, down), gs);
} // Formula::topMiniscope


// initialise:
// _occ such that _occ[i][j] is the numbeer of occurrences of vi in Fj
// _N such that _N[i] = number of j such that _occ[i,j] > 0
// _index such that _index[v] = occ[i], if v = v_i
//                  _index[v] = 0, otherwise
// 29/05/2002 Manchester
Formula::Miniscope::Miniscope (Formula::List* F, VarList* V)
  :
  _nfs (F->length()),
  _nvs (V->length()),
  _occ (new bool* [_nvs]),
  _mem (new bool [_nfs * _nvs]),
  _index (0),
  _vars (new Var[_nvs]),
  _N (new int [_nvs])
{
  ASS(debug_formula, _nfs > 1 && _nvs > 0);

  if (! (_occ && _mem && _vars && _N) ) {
    NO_MEMORY;
  }
  // initialise elements of _mem
  for (int k = _nfs * _nvs - 1; k >= 0; k--) {
    _mem [k] = 0;
  }

  // find a maximal variable in V
  _maxV = V->head ();
  VarList::Iterator vs (V->tail());
  while (vs.more()) {
    Var v = vs.next();
    if (v > _maxV) {
      _maxV = v;
    }
  }

  // allocate the index
  _index = new bool* [_maxV+1];
  if (! _index) {
    NO_MEMORY;
  }
  // initialise elements of the index to 0 first
  for (int v = _maxV; v >= 0; v--) {
    _index [v] = 0;
  }
  // then initialise the elements of _index, _occ _vars corresponding to variables in V
  // by their values
  bool* nxt = _mem;
  Var* nv = _vars;
  bool** nocc = _occ;
  vs.reset (V);
  while (vs.more()) {
    Var v = vs.next();
    _index [v] = nxt;
    *nocc = nxt;
    nocc++;
    nxt += _nfs;
    *nv = v;
    nv++;
  }

  int j = 0; // formula number
  Formula::List::Iterator fs (F);
  while (fs.more()) {
    count (fs.next (), j);
    j++;
  }
  // now _index is not needed any more
  delete [] _index;
  _index = 0;

  // initialise elements of _N 
  // place in _occ corresponding to occurrences of vn in Fm
  bool* onm = _mem;
  int* Nn = _N; // same, but for variable vn in _N
  for (int n = 0; n < _nvs; n++) {
    Nn[0] = 0;
    for (int m = 0; m < _nfs; m++) {
      if (*onm) { // at least one occurrence
        Nn[0] ++;
      }
      onm ++;
    }
    Nn++;
  }
} // Formula::Miniscope::Miniscope


// 29/05/2002 Manchester
Formula::Miniscope::~Miniscope ()
{
  delete [] _mem;
  delete [] _occ;
  delete [] _vars;
  delete [] _N;
} // Formula::Miniscope::~Miniscope


// count the number of occurrences of variables in nnf Fj
// WARNING: assumes that the formula has no occurrences of quantifiers (Q x) in the 
// range of other quantifiers (Q x) bounding the same variable
// 29/05/2002 Manchester
void Formula::Miniscope::count (const Formula* f, int j)
{
  switch ( f->connective() ) {
    case ATOM: {
      Term::List::Iterator ts (f->atom()->args());
      while (ts.more()) {
        count(ts.next(), j);
      }
      return;
    }

    case AND:
    case OR: {
      Formula::List::Iterator fs (f->args());
      while (fs.more()) {
        count(fs.next(), j);
      }
      return;
    }

    case NOT:
    case FORALL:
    case EXISTS:
      count (f->arg(), j);
      return;

    case IMP:
    case IFF:
    case XOR:
    default:
      ASS( debug_formula, false );
      return;
  }
} // Formula::Miniscope::count


// count the number of occurrences of variables in the term
// 29/05/2002 Manchester
void Formula::Miniscope::count (const Term* t, int j)
{
  if (t->isVar()) {
    Var v = t->var ();
    if (v > _maxV) {
      return;
    }

    bool* vi = _index[v];
    if (! vi) {
      return;
    }

    vi[j] = true;
    return;
  }

  // the term is compound
  Term::List::Iterator ts (t->args());
  while (ts.more()) {
    count(ts.next(), j);
  }
} // Formula::Miniscope::count


// true if no miniscoping is possible
// 31/05/2002 Manchester
bool Formula::Miniscope::noMiniscoping () const
{
  for (int i = _nvs-1; i >= 0; i--) {
    if ( _N[i] < _nfs ) {
      return false;
    }
  }

  return true;
} // Formula::Miniscope::noMiniscoping ()


// return the list of variables with at least one occurrence
// and remove information about variables with 0 occurrences
// 31/05/2002 Manchester
VarList* Formula::Miniscope::nonZeros () 
{
  int shift = 0;

  // remove information about 0-occurrences variables
  for (int i = 0; i < _nvs; i++) {
    if (_N[i] == 0) { // found
      shift ++;
    }
    else if (shift) { // shift all arrays
      int k = i - shift;
      _N[k] = _N[i];
      _occ[k] = _occ[i];
      _vars[k] = _vars[i];
    }
    // no 0-occurrence variables so far
  }
  _nvs -= shift;

  // now collect the list
  return all ();
} // Formula::Miniscope::nonZeros


// return the list of all variables 
// 07/06/2002 Manchester
VarList* Formula::Miniscope::all () 
{
  // now collect the list
  VarList* result = VarList::empty ();
  for (int m = _nvs-1; m >= 0; m-- ) {
    VarList::push (_vars[m], result);
  }

  return result;
} // Formula::Miniscope::all


// return a list of variables with exactly one occurrence in the same Fj
// and instantiate jone to this j
// 31/05/2002 Manchester
VarList* Formula::Miniscope::ones (int& jone) 
{
  // remove information about 0-occurrences variables
  for (int i = 0; i < _nvs; i++) {
    if (_N[i] == 1) { // found
      // collect the rest occurring in the same formulas as vi
      VarList* is = VarList::empty ();
      for (int i1 = _nvs-1; i1 > i; i1--) {
        if ( same(i,i1) ) {
          VarList::push (_vars[i1],is);
          // mark _N[i1] to be removed
          _N[i1] = -1;
        }
      }
      VarList::push (_vars[i],is);
      // find j
      for (int j = _nfs - 1; j >= 0; j--) {
        if (_occ[i][j]) {
          jone = j;
          // shift all arrays before returning
          int shift = 1;
          for (i = i+1; i < _nvs; i++) {
            if (_N[i] == -1) { // found
              shift ++;
            }
            else { // shift all arrays
              int k = i - shift;
              _N[k] = _N[i];
              _occ[k] = _occ[i];
              _vars[k] = _vars[i];
            }
            // no 0-occurrence variables so far
          }
          _nvs -= shift;

          return is;
        }
      }
      ASS (debug_formula, false);
    }
  }

  return VarList::empty ();
} // Formula::Miniscope::ones


// return a list of variables with exactly one occurrence in the same Fj
// and instantiate jone to this j
// 31/05/2002 Manchester
bool Formula::Miniscope::same (int i1, int i2) 
{
  for (int j = _nfs-1; j >= 0; j--) {
    if (_occ[i1] != _occ[i2]) {
      return false;
    }
  }

  return true;
} // Formula::Miniscope::same


// Fs is a list (F0,...,Fn) 
// return the list (F0,...,Fj-1,(Q vs)F,Fj+1,...,Fn), where F is top-Miniscoped
// 04/06/2002 Manchester
Formula::List* Formula::pushVarsDown ( List* Fs, Connective Q, int j, VarList* vs)
{
  ASS( debug_formula, j >= 0);

  if (j != 0) {
    return new List (Fs->head(), pushVarsDown (Fs->tail(), Q, j-1, vs));
  }
  // j == 0
  Formula* f = Fs->head();
  Formula* F = f->topMiniscope (Q, vs);
  if (! F) { // top-miniscoping f did not work
    F = new Formula (Q, vs, f);
  }
  else {
    vs->destroy ();
  }

  return new List (F, Fs->tail());
} // Formula::List* Formula::pushVarsDown

// assuming c is & (the case of \/ is dual)
// Fs is a list F1 ... Fn
// we find a proper subset J of {1...n} such that variables V occur in {Fj | j in J} only
// if such a subset does not exist, then 0 is returned
// if J exists, then we return (& Fj    & (exists V)&Fj)
//                           {1...n}-J              J
// among all possible J we select the one of the smallest size, and if sizes are equal
// having the longest V
// 07/06/2002 Manchester
Formula* Formula::Miniscope::topple (List* Fs, Connective c)
{
  return 0;
//   ASS( debug_formula, c == AND || c == OR);
//   ASS( debug_formula, Fs->length() == _nfs);

//   // finding the minimal size of J
//   int min = _nfs;   // the minimal size
//   int imin;         // index of the first variable having the minimal size
//   for (int i1 = 0; i1 < _nvs; i1++) {
//     if ( _N[i1] < min ) {
//       min = _N[i1];
//       imin = i1;
//     }
//   }
//   if ( min == _nfs ) {
//     return 0;
//   }
//   // now such a J exists. Call the minimal variable any variable having occurrences
//   // in min formulas
//   // build an array of lists of minimal variables. Each list contains variables having
//   // exactly the same occurrences
//   Int::List** mvs = new Int::List* [_nvs+1];
//   if (! mvs) {
//     NO_MEMORY;
//   }
//   for (int i2 = _nvs; i2 >= 0; i2--) {
//     mvs[i2] = Int::List::empty();
//   }
//   for (int i3 = _nvs; i3 >= 0; i3--) {
//     if (_N[i3] != min) {
//       continue;
//     }
//     bool found = false;
//     // now i3 is a minimal variable
//     Int::List** m;
//     for (m = mvs; m[0]->isNonEmpty(); m++) {
//       if ( same (m[0]->head(), i3) ) {
//         Int::List::push (i3, m[0]);
//         found = true;
//         break;
//       }
//     }
//     if (! found) {
//       m[0] = new Int::List (i3);
//     }
//   }
//   // mvs is such a list
//   // select the longest list among mvs[i]
//   Int::List* longest = mvs[0];
//   int maxlen = longest->length();
//   // we should have considered maxlen == 1 when ones had been called
//   ASS( debug_formula, maxlen != 1);
//   for (Int::List** m = mvs+1; m[0]->isNonEmpty(); m++) {
//     int l = m[0]->length();
//     // we should have considered l == 1 when ones had been called
//     ASS( debug_formula, l != 1);

//     if (l > maxlen) {
//       maxlen = l;
//       longest = m[0];
//     }
//   }
//   // now we only deal with the list longest
//   for (Int::List** m1 = mvs; m1[0]->isNonEmpty(); m1++) {
//     if ( m1[0] != longest ) {
//       m1[0]->destroy ();
//     }
//   }
//   delete [] mvs;
//   // now longest contains the longest list
//   // collect the list of variables V
//   VarList* V = VarList::empty ();
//   Int::List::Iterator ll (longest);
//   int i0 = 0; // the smallest index of a variable in V, initialisation is not required
//   while (ll.more()) {
//     i0 = ll.next();
//     VarList::push (_vars[i0], V);
//   }

//   // note that J = {j | v occurs in Fj}
//   // now select j0 in J and create an array F of formulas such that 
//   //   F[j] = the new index of Fj if j is not in J
//   //   F[j] = j0 if j in J
//   int* F = new int [_nfs];
//   int shift = 0;
//   if (! F) {
//     NO_MEMORY;
//   }
//   int j0 = -1;
//   bool* occi0 = _occ[i0];
//   for (int j1 = 0; j1 < _nfs; j1++) {
//     if ( occi0[j1] ) {
//       if (j0 == -1) {
//         j0 = j1;
//       }
//       else { // not the first element of J
//         shift++;
//       }
//       F[j1] = j0;
//     }
//     else {
//       F[j1] = j1 - shift;
//     }
//   }

//   // now build the list FF of formulas {Fj | j in J}
//   // and the array of formulas R to be used in the result
//   List* FF = Fs;

//   // remove variables in longest from the structure
//   longest = longest->reverse ();
//   int sft = 0;
//   Int::List::Iterator vs (longest);
//   _nvs -= maxlen;
//   longest->destroy ();

//   // re-count occurrences
//   for (int i4 = _nvs -1; i4 >= 0; i4--) {
//     bool* oc = _occ[i4];
//     for (int j = 0; j < _nfs; j++) {
//       oc[F[j]] = oc[j];
//     }
//   }
//   delete [] F;

//   // change _nfs;
//   _nfs -= shift;
} // Formula::Miniscope::topple


Formula* Formula::flatten ()
{
  switch ( connective() ) {
  case ATOM:
    return this;

  case AND:
  case OR: {
    List* flattenedArgs = flatten (args(), connective());
    if ( flattenedArgs == args() ) {
      return this;
    }
    return new Formula (connective(), flattenedArgs);
    }

  case IMP:
  case IFF:
  case XOR: {
    Formula* flattenedLeft = left()->flatten();
    Formula* flattenedRight = right()->flatten();
    if (flattenedLeft == left() && flattenedRight == right()) {
      return this;
    }
    return new Formula ( connective(),
			 flattenedLeft,
			 flattenedRight );
    }
    
  case NOT: {
    Formula* flattenedArg = arg()->flatten();
    if (flattenedArg == arg()) {
      return this;
    }
    return new Formula (NOT, flattenedArg);
  }
    
  case FORALL:
  case EXISTS: {
    Formula* flattenedArg = arg()->flatten();
    if (flattenedArg->connective() == connective()) {
      // needs more flattening, concatenate variables
      Formula* result = new Formula (connective(),
			             vars()->append(flattenedArg->vars()->copy()),
				     flattenedArg->arg() );
      flattenedArg->inc ();
      flattenedArg->dec ();
      return result;
      }
    // no more flattening
    if (flattenedArg == arg()) {
      return this;
      }
    return new Formula (connective(), 
			vars()->copy(), 
			flattenedArg);
  }

  default:
    ASS( debug_formula, false );
    return false;
  }
} // Formula::flatten ()


// flatten the list of formulas (connected by c)
// 27/06/2002 Manchester
Formula::List* Formula::flatten (List* fs, Connective c)
{
  if (fs->isEmpty()) {
    return List::empty();
  }
  Formula* flattenedHead = fs->head()->flatten();
  List* flattenedTail = flatten(fs->tail(), c);
  if (flattenedHead->connective() == c) { // needs further flattening
    List* result = flattenedHead->args()->append(flattenedTail);
    flattenedHead->inc();
    flattenedHead->dec();
    return result;
  }
  // needs no further flattening
  if (flattenedHead == fs->head() && flattenedTail == fs->tail()) {
    return fs;
  }
  return new List (flattenedHead, flattenedTail);
} // Formula::List* Formula::flatten


// 28/06/2002 Manchester
Formula* Formula::skolemizeNNF () const 
{
  Substitution subst;

  return skolemizeNNF (subst, VarList::empty());
} // Formula::skolemizeNNF


// 28/06/2002 Manchester
Formula* Formula::skolemizeNNF (Substitution& subst, 
				const VarList* vs) const
{
  switch ( connective() ) {
  case ATOM:
    return new Formula (atom()->apply(subst));

  case AND:
  case OR:
    return new Formula (connective(), skolemizeNNF(args(), subst, vs));

  case NOT:
    ASS( debug_formula, arg()->connective() == ATOM);
    return new Formula ( NOT, 
			 new Formula (arg()->atom()->apply(subst)) );

  case FORALL: {
    VarList* newVars = vars()->append(vs->copy());
    Formula* result = arg()->skolemizeNNF (subst, newVars);
    newVars->destroy ();
    return result;
    }

  case EXISTS: {
    // add skolem functions to the substitution
    Term::List* skolemArgs = Term::varsToTerms (vs);
    int noOfArgs = vs->length ();
    VarList::Iterator newSubstVars (vars());
    while (newSubstVars.more()) {
      subst.push ( newSubstVars.next (),
		   new Term ( sig->newSkolemFunction (noOfArgs),
			      skolemArgs ) );
    }
    // skolemize the argument
    Formula* result = arg()->skolemizeNNF (subst, vs);
    // restore the substitution
    for (int i = vars()->length() - 1; i >= 0; i--) {
      subst.pop ();
    }
    return result;
    }

  case IMP:
  case IFF:
  case XOR:
  default:
    ASS( debug_formula, false );
    return false;
  }
} // Formula::skolemizeNNF


// 28/06/2002 Manchester
Formula::List* Formula::skolemizeNNF (const List* fs,
				      Substitution& subst,
				      const VarList* skolemArgs) 
{
  if (fs->isEmpty()) {
    return List::empty ();
  }

  Formula* h =  fs->head()->skolemizeNNF(subst, skolemArgs);
  return new List (h, skolemizeNNF (fs->tail(), subst, skolemArgs));
} // Formula::skolemizeNNF


// output formula in the KIF format
// 24/07/2002 Torrevieja
void Formula::outputKIF (ostream& str, int indent) const
{
  // indent
  for (int i = indent; i >= 0; i--) {
    str << ' ';
  }
  // then output
  switch ( connective() ) {
  case ATOM:
    atom()->outputKIF(str);
    return;

  case AND:
  case OR: {
    str << '(' << KIFPrintName (connective()) << '\n';
    Formula::List::Iterator fs (args());
    while (fs.more()) {
      fs.next()->outputKIF(str, indent+2);
      if (fs.more()) {
	str << '\n';
      }
    }
    str << ')';
    return;
  }

  case IMP:
  case IFF:
  case XOR:
    str << '(' << KIFPrintName (connective()) << '\n';
    left()->outputKIF(str, indent+2);
    str << '\n';
    right()->outputKIF(str, indent+2);
    str << ')';
    return;

  case NOT:
    if (arg()->connective() == ATOM) {
      str << '(' << KIFPrintName (NOT) << ' ';
      arg()->atom()->outputKIF (str);
      str << ')';
    }
    else {
      str << '(' << KIFPrintName (NOT) << '\n';
      arg()->outputKIF(str, indent+2);
      str << ')';
    }
    return;

  case FORALL:
  case EXISTS: {
    str << '(' << KIFPrintName (connective()) << " (";
    VarList::Iterator vs (vars());
    while (vs.more()) {
      str << "?X" << vs.next();
      if (vs.more()) {
	str << ' ';
      }
    }

    if (arg()->connective() == ATOM) {
      str << ") ";
      arg()->atom()->outputKIF (str);
      str << ')';
    }
    else {
      str << ")\n";
      arg()->outputKIF(str, indent+2);
      str << ')';
    }
    return;
  }

  default:
    ASS( debug_formula, false );
    return;
  }
} // Formula::outputKIF


// print name of the connective in the KIF format
// 24/07/2002 Torrevieja
const char* Formula::KIFPrintName (Connective c)
{
  switch (c) {
  case AND:
    return "and";

  case OR:
    return "or";

  case IMP:
    return "=>";

  case IFF:
    return "<=>";

  case XOR:
    return "<~>";

  case NOT:
    return "not";

  case FORALL:
    return "forall";

  case EXISTS:
    return "exists";

  case ATOM:
  default:
    ASS( debug_formula, false );
    return false;
  }
} // Formula::KIFPrintName

/*
  THIS IS USEFUL
  switch ( connective() ) {
  case ATOM:
  case AND:
  case OR:
  case IMP:
  case IFF:
  case XOR:
  case NOT:
  case FORALL:
  case EXISTS:
  default:
    ASS( debug_formula, false );
    return false;
  }

  switch ( ->connective() ) {
  case Formula::ATOM:
  case Formula::AND:
  case Formula::OR:
  case Formula::IMP:
  case Formula::IFF:
  case Formula::XOR:
  case Formula::NOT:
  case Formula::FORALL:
  case Formula::EXISTS:
  default:
    ASS( debug_???, false );
    return false;
  }
*/

