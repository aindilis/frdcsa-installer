//
//  file sym_counter.cpp
//  implements class SymCounter counting occurrences of function and predicate symbols
//
//  01/05/2002, Manchester
//


#include "formula.h"
#include "sym_counter.h"
#include "unit.h"
#include "problem.h"
#include "clause.h"
#include "literal.h"
#include "atom.h"
#include "term.h"


SymCounter* SymCounter::norm = 0;


//  01/05/2002, Manchester
SymCounter::SymCounter (Signature& sig)
  :
  _noOfPreds (sig.noOfPreds()),
  _noOfFuns (sig.noOfFuns()),
  _preds ( new Pred[_noOfPreds] ),
  _funs ( new Fun[_noOfFuns] )
{
  Signature::Iterator ps (sig._pbuckets);

  while ( ps.more() ) {
    Signature::Pred* p = static_cast<Signature::Pred*>(ps.next());
    _preds[p->number()].pred(p);
  }

  Signature::Iterator fs (sig._fbuckets);
  while ( fs.more() ) {
    Signature::Fun* f = static_cast<Signature::Fun*>(fs.next());
    _funs[f->number()].fun(f);
  }
} // SymCounter::SymCounter


//  01/05/2002, Manchester
SymCounter::~SymCounter ()
{
  delete [] _preds;
  delete [] _funs;
} // SymCounter::~SymCounter


// this function is for debugging only
//  01/05/2002, Manchester
ostream& operator << ( ostream& str, const SymCounter& c )
{
  str << "Predicates\n";

  for (int i = 0; i < c._noOfPreds; i++ ) {
    SymCounter::Pred& p = c._preds[i];
    str << "  " << p.pred() << "(" 
        << p.nocc() << ',' 
        << p.docc() << ',' 
        << p.pocc() << ")\n";
  }

  str << "Functions\n";

  for (int j = 0; j < c._noOfFuns; j++ ) {
    SymCounter::Fun& p = c._funs[j];
    str << "  " << p.fun() << "(" 
        << p.occ() << ")\n";
  }

  return str;
}  // operator << ( ostream& str, const SymCounter& c )


// c must be 1 or -1, 1 means add number of occurrences for each symbol, -1 means subtract
//  01/05/2002, Manchester
void SymCounter::count (Problem& p,int c)
{
  Unit::Chain::Iterator us ( p.units() );
  while ( us.more() ) {
    count (us.next(),c);
  }
} // SymCounter::count (Problem& p,int c)


//  01/05/2002, Manchester
void SymCounter::count (Unit* u,int c)
{
  switch ( u->unitType () ) {
    case FORMULA:
      count (u->formula(), 1, c);
      return;

    case CLAUSE:
      count (u->clause(), c);
      return;
  }
} // SymCounter::count (Unit* u,int c)


//  01/05/2002, Manchester
void SymCounter::count (Clause* u,int add)
{
  Literal::List::Iterator ls (u->literals());
  while ( ls.more() ) {
    count (ls.next(), add);
  }
} // SymCounter::count (Clause* u,int c)


//  01/05/2002, Manchester
void SymCounter::count (Formula* f,int polarity,int add)
{
  switch ( f->connective() ) {
    case Formula::ATOM:
      count ( f->atom(), polarity, add );
      return;

    case Formula::AND:
    case Formula::OR: {
      Formula::List::Iterator fs (f->args());
      while ( fs.more() ) {
        count ( fs.next(), polarity, add );
      }
      return;
    }

    case Formula::IMP:
      count ( f->left(), -polarity, add );
      count ( f->right(), polarity, add );
      return;

    case Formula::NOT:
      count ( f->arg(), -polarity, add );
      return;

    case Formula::IFF:
    case Formula::XOR:
      count ( f->left(), 0, add );
      count ( f->right(), 0, add );
      return;

    case Formula::FORALL:
    case Formula::EXISTS:
      count ( f->arg(), polarity, add );
      return;

    default:
      ASS( debug_sym_counter, false);
      return;
  }
} // SymCounter::count (Formula* f,...)


//  01/05/2002, Manchester
void SymCounter::count (Literal* l,int add)
{
  count (l->atom(), l->positive() ? 1 : -1, add);
} // SymCounter::count (Literal* l,int c)


//  01/05/2002, Manchester
void SymCounter::count (Atom* a,int polarity,int add)
{
  get(a->functor())->add (polarity,add);

  Term::List::Iterator ts (a->args());
  while ( ts.more() ) {
    count (ts.next(), add);
  }
} // SymCounter::count (Atom* f, ...)


//  01/05/2002, Manchester
void SymCounter::count (Term* t,int add)
{
  if ( t->isVar() ) {
    return;
  }

  get(t->functor())->add (add);

  Term::List::Iterator ts (t->args());
  while ( ts.more() ) {
    count (ts.next(), add);
  }
} // SymCounter::count (Term* f, ...)


void SymCounter::Pred::add (int polarity, int add)
{
  switch (polarity) {
    case -1:
      _nocc += add;
      return;

    case 0:
      _docc += add;
      return;

    case 1:
      _pocc += add;
      return;

    default:
      ASS( debug_sym_counter, false);
      return;
  }
} // SymCounter::Pred::add
