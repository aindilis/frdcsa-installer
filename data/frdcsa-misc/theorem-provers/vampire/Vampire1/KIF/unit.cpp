//
//  file unit.cpp
//

#include <string.h>

#include "debug.h"
#include "tracer.h"
#include "unit.h"
#include "formula.h"
#include "clause.h"

long Unit::_lastNumber = 0;

ostream& operator << ( ostream& str, InputType t )
{
  switch ( t ) {
    case AXIOM:
      str << "axiom";
      break;

    case CONJECTURE:
      str << "conjecture";
      break;

    case HYPOTHESIS:
      str << "hypothesis";
      break;
  }

  return str;
} // operator << ( ostream& str, InputType t )


ostream& operator << ( ostream& str, UnitType t )
{
  switch ( t ) {
    case CLAUSE:
      str << "input_clause";
      break;

    case FORMULA:
      str << "input_formula";
      break;
  }

  return str;
} // operator << ( ostream& str, UnitType t )


ostream& operator << ( ostream& str, Unit::Chain& units )
{
  Unit::Chain::Iterator us (units);

  while ( us.more() ) {
    Unit* unit = us.next ();

    str << unit->unitType () 
        << '(';

    if (unit->rule() == IR_INPUT && unit->name()) {
      str << unit->name ();
    }
    else {
      str << "unit_" << unit->number();
    }

    str << ','
        << unit->inputType()
        << ",\n";

    switch ( unit->unitType() )  {
      case FORMULA:
        unit->formula()->prettyPrint ( 4, str );
        break;

      case CLAUSE:
        unit->clause()->prettyPrint ( 4, str );
        break;
    }
    str << " ).\n\n";
  }

  return str;
} // operator << ( ostream& str, Unit::chain& c )


Unit::Unit ( char* name, InputType intype, Clause* c )
  :
  _untype (CLAUSE),
  _intype (intype),
  _rule (IR_INPUT),
  _number (++ _lastNumber),
  _name (name),
  _clause (c),
  _defType (DT_NONE),
  _count (0)
{
} // Unit::Unit


Unit::Unit ( char* name, InputType intype, Formula* f )
  :
  _untype (FORMULA),
  _intype (intype),
  _rule (IR_INPUT),
  _number (++ _lastNumber),
  _name (name),
  _formula (f),
  _defType (DT_NONE),
  _count (0)
{
  f->inc ();
} // Unit::Unit


// unit obtained by an inference with one parent
// 07/05/2002 Manchester
Unit::Unit ( InferenceRule ir, Formula* f, Unit* parent )
  :
  _untype (FORMULA),
  _intype (parent->_intype),
  _rule ( ir ),
  _number (++ _lastNumber),
  _parents (new List (parent)),
  _formula (f),
  _defType (DT_NONE),
  _count (0)
{
  f->inc ();
  parent->inc ();
} // Unit::Unit


// unit obtained by an inference with one parent
// 07/05/2002 Manchester
Unit::Unit ( InferenceRule ir, Clause* c, Unit* parent )
  :
  _untype (CLAUSE),
  _intype (parent->_intype),
  _rule ( ir ),
  _number (++ _lastNumber),
  _parents (new List (parent)),
  _clause (c),
  _defType (DT_NONE),
  _count (0)
{
  parent->inc ();
} // Unit::Unit


Unit::~Unit ()
{
  TRACER ( "Unit::~Unit" );

  if ( rule() == IR_INPUT ) {
    delete [] _name;
  }
  else {
    Unit::List::Iterator ps (_parents);
    while ( ps.more() ) {
      Unit* p = ps.next ();
      p->dec ();
    }

    _parents->destroy ();
  }

  switch ( unitType () ) {
    case FORMULA:
      _formula->dec ();
      return;

    case CLAUSE:
      delete _clause;
      return;
  }
} // Unit::~Unit


void Unit::dec ()
{
  TRACER( "Unit::dec" );
  ASS( debug_unit, _count > 0 );

  _count --;
  if (_count == 0) { // the unit has no more references, destroy it
    delete this;
  }
} // Unit::~Unit


// Warning: only resets an already existing formula
// 11/05/2002 Manchester
void Unit::formula ( Formula* f ) 
{
  ASS ( debug_problem, _formula && 
                          unitType() == FORMULA );

  f->inc();
  _formula->dec(); 
  _formula = f;
} // Unit::formula


/*
int Unit::countOccurrences ( const PSymbol* p ) const
{
  switch ( unitType() ) {
    case CLAUSE:
      return clause()->countOccurrences (p);

    case FORMULA:
      return formula()->countOccurrences (p);

    default:
      ASS( debug_unit, false );
      return false;
  }
} // Unit::countOccurrences


int Unit::countOccurrences ( const FSymbol* p ) const
{
  switch ( unitType() ) {
    case CLAUSE:
      return clause()->countOccurrences (p);

    case FORMULA:
      return formula()->countOccurrences (p);

    default:
      ASS( debug_unit, false );
      return false;
  }
} // Unit::countOccurrences


bool Unit::occurs ( const FSymbol* p ) const
{
  ASS( debug_unit, unitType() == CLAUSE );
  
  return clause()->occurs (p);
} // Unit::occurs


// 03/06/2001
bool Unit::occurs ( const PSymbol* p ) const
{
  switch ( unitType() ) {
    case CLAUSE:
      return clause()->occurs (p);

    case FORMULA:
      return formula()->occurs (p);

    default:
      ASS( debug_unit, false );
      return false;
  }
} // Unit::occurs


Var* Unit::isFunctionalityAxiom ( const PSymbol* pred, FSymbol*& fun, int argNo, Atom*& atom ) const
{
  ASS( debug_unit, unitType() == CLAUSE );

  return clause()->isFunctionalityAxiom (pred,fun,argNo, atom);
} // Unit::isFunctionalityAxiom


// the unit is P(x,y) & P(x,z) -> y = z
bool Unit::isUniquenessAxiom ( PSymbol*& pred, int& argNo ) const
{
  ASS( debug_unit, unitType() == CLAUSE );

  return clause()->isUniquenessAxiom (pred, argNo);
} // Unit::isUniquenessAxiom


// the unit is f(X,y,Z) = f(X,u,Z) -> y = u
bool Unit::isCancellationAxiom ( FSymbol*& fun, int& argNo ) const
{
  ASS( debug_unit, unitType() == CLAUSE );

  return clause()->isCancellationAxiom (fun, argNo);
} // Unit::isCancellationAxiom


// replace the cancellation axiom 
//   f(X,y,Z) = f(X,u,Z) -> y = u
// by 
//   i(f(X,y,Z),X,Z) = y
//void Unit::replaceCancellation ( const FSymbol* fun,int argNo,FSymbol* inv, ostream& report )
//{
//  ASS( debug_unit, unitType() == CLAUSE );
//
//  clause()->replaceCancellation (argNo, inv);
//
//  report << "inference(" << clause() << "," << name() 
//         << "_rcbi,[" << name () << "],[rcbi]]).\n";
//  _name += "_rcbi"; // replace cancellation by inverse
//} // Unit::replaceCancellation


//void Unit::apply ( const ReplaceCode& code, const Unit* def, ostream& report ) 
//{
//  ASS( debug_unit, unitType() == CLAUSE );
//
//  if ( code.apply (clause()) ) {
//    report << "inference(" << clause() << "," << name() 
//           << "_dapp,[" << name () << "," << def->name () << "],[dapp]]).\n";
//    _name += "_dapp"; // replace cancellation by inverse
//  }
//} // Unit::apply


int Unit::maxLitWeight ( const ReplaceCode& code ) const
{
  ASS( debug_unit, unitType() == CLAUSE );

  return code.maxLitWeight (clause());
} // Unit::maxLitWeight


int Unit::weight () const 
{
  switch ( unitType() ) {
    case CLAUSE:
      return clause()->weight ();

    case FORMULA:
      return formula()->weight ();

    default:
      ASS( debug_unit, false );
      return false;
  }
} // Unit::weight


Term* Unit::isDefinition ( Term* & rhs ) const
{
  ASS( debug_unit, unitType() == CLAUSE );

  return clause()->isDefinition (rhs);
} // Unit::isDefinition
*/


// 03/06/2001
Atom* Unit::isPredicateDefinition ( Formula* & rhs ) const
{
  TRACER ( "Unit::isPredicateDefinition" );

  switch ( unitType() ) {
    case CLAUSE:
      return 0;

    case FORMULA:
      return formula()->isPredicateDefinition (rhs);

    default:
      ASS ( debug_unit, false );
      return 0;
  }
} // Unit::isPredicateDefinition


/*
Term* Unit::isLinearDefinition ( Term* & rhs ) const
{
  ASS( debug_unit, unitType() == CLAUSE );

  return clause()->isLinearDefinition (rhs);
} // Unit::isLinearDefinition


bool Unit::hasFunctionSymbols () const
{
  ASS( debug_unit, unitType() == CLAUSE );

  return clause()->hasFunctionSymbols ();
} // Unit::hasFunctionSymbols


bool Unit::hasEquality () const
{
  ASS( debug_unit, unitType() == CLAUSE );

  return clause()->hasEquality ();
} // Unit::hasEquality


bool Unit::isHorn () const
{
  ASS( debug_unit, unitType() == CLAUSE );

  return clause()->isHorn ();
} // Unit::isHorn


bool Unit::isUnit () const
{
  ASS( debug_unit, unitType() == CLAUSE );

  return clause()->isUnit ();
} // Unit::isUnit


bool Unit::isPropositional () const
{
  ASS( debug_unit, unitType() == CLAUSE );

  return clause()->isPropositional ();
} // Unit::isPropositional


// 06/06/2001 made from pureEquality
bool Unit::hasNonEquality () const
{
  ASS( debug_unit, unitType() == CLAUSE );

  return clause()->hasNonEquality ();
} // Unit::hasNonEquality


int Unit::numberOfLiterals () const
{
  ASS( debug_unit, unitType() == CLAUSE );

  return clause()->length ();
} // Unit::numberOfLiterals


//void Unit::replaceRelationByFunction ( PSymbol* pred, FSymbol* fun, int argNo, int* vs, ostream& report )
//{
//  if ( clause()->replaceRelationByFunction (pred, fun, argNo, vs) ) {
//    report << "inference(" << clause() << "," << name() 
//           << "_rrbf,[" << name () << ",replace_" << pred << "_by_" << fun  << "],[rrbf]]).\n";
//    _name += "_rrbf";
//
//    removeVarEquality (report);
//  }
//} // Unit::replaceRelationByFunction


Unit* Unit::resolveOn ( const PSymbol* f, Unit* unit, ostream& report )
{
  static int res_number = 0;
  static const char prefix [] = "res_";
  static const int prefixLength = strlen (prefix);

  // making name for the new unit
  char* newName = new char [prefixLength + 10];
  strcpy ( newName, prefix );
  char* cursor = newName + prefixLength;
  Int::toString ( res_number, cursor );
  res_number++;

  Clause* c = clause()->resolveOn (f, unit->clause());
  Unit* result = new Unit ( newName, inputType(), c );
  report << "inference(" << c << "," << newName << ",[" 
         << name () <<"," << unit->name () << "],[br]).\n";
  
  return result;
} // Unit::resolveOn


Compare Unit::compare ( const Unit* u ) const
{
  // only units of the same type can be compared at this moment
  ASS ( debug_unit, _untype == u->_untype );

  switch ( _untype ) {
    case CLAUSE:
      return clause()->compare(u->clause());

    case FORMULA:
      return formula()->compare(u->formula());

    default:
      ASS ( debug_unit, false );
  }
} // Unit::compare


//bool Unit::removeVarEquality ( ostream& report )
//{
//  // only clauses can be treated at this moment
//  ASS ( debug_unit, _untype == CLAUSE );
//
//  if ( clause()->removeVarEquality() ) {
//    report << "inference(" << clause() << "," << name() << "_rve,[" << name () << "],[rve]).\n";
//    _name += "_rve";
//    return true;
//  }
//
//  return false;
//} // Unit::removeVarEquality


bool Unit::isTautology ( ostream& report ) 
{
  // only clauses can be treated at this moment
  ASS ( debug_unit, _untype == CLAUSE );

  if ( clause()->isTautology() ) {
    report << "prepro(removed_as_tautology," << name() << "," << clause() << ").\n";
    return true;
  }

  return false;
} // Unit::isTautology


int Unit::maxLitWeight ()
{
  // only clauses can be treated at this moment
  ASS ( debug_unit, _untype == CLAUSE );

  return clause()->maxLitWeight();
} // Unit::maxLitWeight


void Unit::splitLargeArity ( const PSymbol* p, FSymbol::List* newFs )
{
  // only clauses can be treated at this moment
  ASS ( debug_unit, _untype == CLAUSE );

  clause()->splitLargeArity (p, newFs);
} // Unit::splitLargeArity


void Unit::splitLargeArity ( const FSymbol* f, FSymbol::List* newFs )
{
  // only clauses can be treated at this moment
  ASS ( debug_unit, _untype == CLAUSE );

  clause()->splitLargeArity (f, newFs);
} // Unit::splitLargeArity


// no time to tab the split now
Unit::List* Unit::negativeEqualitySplit ( int thresholdWeight, ostream& tab )
{
  // only clauses can be treated at this moment
  ASS ( debug_unit, _untype == CLAUSE );

  Clause::List* cs = clause()->negativeEqualitySplit( thresholdWeight );

  List* newUnits = Unit::List::empty ();

  Clause::List::Iterator cls ( cs );
  while ( cls.more() ) {
    static int number = 0;
    static char name [] = "term_def_          ";
    Int::toString ( ++number, name+9 );
    char* nm = new char [strlen(name)+1];
    strcpy (nm, name);
    Unit::List::push ( new Unit (nm, AXIOM, cls.next()), newUnits );
  }

  cs->destroy ();

  return newUnits;
} // Unit::negativeEqualitySplit
*/

// the unit is one of the equality axioms
// 03/06/2001
// 29/04/2002 added check for clauses
bool Unit::isEqualityAxiom () const
{
  TRACER("Unit::isEqualityAxiom");

  switch ( unitType() ) {
    case CLAUSE:  
      return clause()->isEqualityAxiom ();

    case FORMULA:
      return formula()->isEqualityAxiom ();

    default:
      ASS ( debug_unit, false );
      return 0;
  }
} // Unit::isEqualityAxiom


// 04/06/2001
// 11/05/2002 Manchester, changed completely
Clause::List* Unit::skolemize ()
{
  switch ( unitType() ) {
    case CLAUSE:  
      return 0;

    case FORMULA: {
//      bool polarity = true;
//
//      switch ( inputType() ) {
//
//        case AXIOM:
//        case HYPOTHESIS:
//          break;
//
//        case CONJECTURE:
//          polarity = false;
//          break;
//      }
//      
//      // skolemize
//      Clause::List* newClauses = formula ()->skolemize (polarity);
//      // to generate names for the new units
//      int appendix = 0; // to append to the unit name
//      char app [10];  
//      app[0] = '_';
//      // create new units and append them to appendTo
//      Clause::List::Iterator cls (newClauses);
//      while ( cls.more() ) {
//        // make a new name by appending the current name with appendix
//        Int::toString (appendix++,app+1);
//        // create a new unit and append it
//        Unit::List::push ( new Unit (name(), app, inputType(), cls.next()),
//                           appendTo );
//      }
//      int result = newClauses->length ();
//      newClauses->destroy ();
//      return result;
    }  

    default:
      ASS ( debug_unit, false );
      return 0;
  }  
} // Unit::skolemize


/*
// 04/06/2001
void Unit::polarities ( const PSymbol* p, int& pol ) const
{
  switch ( unitType() ) {
    case CLAUSE:  
      clause()->polarities (p,pol);
      return;

    case FORMULA:
      switch ( inputType() ) {
        case AXIOM:
        case HYPOTHESIS:
          formula()->polarities (p,true,pol);
          return;
        case CONJECTURE:
          formula()->polarities (p,false,pol);
          return;
      }

    default:
      ASS ( debug_unit, false );
      return;
  }
} // Unit::polarities


// 04/06/2001
// the unit is a definition (P(X) <=> rhs)
// is pos, then it should be changed into (P(X)->rhs), otherwise into
// (rhs->P(X)), i.e. pos is the polarity of rsh in the new formula
//void Unit::makeHalfDefinition ( bool positive, const Formula* rhs )
//{
//  ASS ( debug_unit, _untype == FORMULA );
//  ASS ( debug_unit, _intype != HYPOTHESIS );
//  
//  _name += positive ? "_neg" : "_pos";
//  formula()->makeHalfDefinition ( positive, rhs );
//} // Unit::makeHalfDefinition


// 06/06/2001
bool Unit::isGoal () const
{
  return inputType () == CONJECTURE;
} // Unit::isGoal


*/

// true if the unit is a formula with free variables
// 06/05/2002 Manchester
bool Unit::formulaWithFreeVars () const
{
  return unitType() == FORMULA &&
         formula()->hasFreeVars ();
} // Unit::formulaWithFreeVars


// If the unit is not a definition
//    return 0
// If the unit is a definition in the right form
//    change the _defType tag of the unit and return the unit itself
// If the unit is a definition but e.g. with lhs and rhs swapped then return
//    a new unit obtained by transforming the original one
// 07/05/2002 Manchester
Unit* Unit::makeDefinition ()
{
  TRACER ( "Unit::makeDefinition" );

  switch ( unitType() ) {
    case CLAUSE: {
      Clause* c = clause();
      Term* lt;
      Term* rt;
      lt = c->isFunctionDefinition (rt);
      if (! lt) {
        return 0;
      }

      // now either lhs or lt is non-zero
      Unit* current = this;

      // now we have a definition
      ASS( debug_unit, ! c->isEmpty() );

      // check that the lt and rt are in the right order
      Atom* a = c->literals()->head ()->atom ();
      ASS( debug_unit, a->isEquality() );
      Term::List* ts = a->args ();
      if (ts->head() == lt) {
        // the order is correct
        current->_defType = DT_FUN;
        return current;
      }
      // the order is wrong, build a new unit
      Clause* c1 = new Clause (
                     new Literal::List ( 
                       new Literal ( true,
                         new Atom (ts->second(), ts->head()) ) ) );

      // increase counter since the unit will be replaced by a new one
      current = new Unit ( IR_REORDER_EQ, c1, current );
      current->_defType = DT_FUN;
      return current;
    }

    case FORMULA: {
      Formula* f = formula();
      Formula* rhs;
      Atom* lhs = f->isPredicateDefinition (rhs);
      Term* lt = 0;
      Term* rt;
      if (! lhs) {
        lt = f->isFunctionDefinition (rt);
        if (! lt) {
          return 0;
        }
      }
      // now either lhs or lt is non-zero
      Unit* current = this;

      // now we have a definition
      ASS( debug_unit, 
              f->connective() == Formula::IFF || 
              f->connective() == Formula::ATOM || 
              f->connective() == Formula::FORALL ) 
      // check if the unit needs flattening
      if ( f->connective() == Formula::FORALL &&
           f->arg()->connective() == Formula::FORALL ) {
        // the formula needs flattening, first collect outermost variables in vs
        VarList* vs = VarList::empty();
        Formula* g = f;
        while (g->connective() == Formula::FORALL) {
          vs = VarList::concat (vs, g->vars()->copy());
          g = g->arg ();
        }
        // build a new flattened formula
        f = new Formula (Formula::FORALL, vs, g);

        // increase counter since the unit will be replaced by a new one
        current = new Unit ( IR_FLATTEN, f, current );
      }
      if (lhs) { // it was a predicate definition
        // now current is the (possibly new) unit with the flattened formula f
        // check that the lhs and rhs of f are in the right order
        ASS( debug_unit, f->connective() == Formula::IFF || f->connective() == Formula::FORALL );
        Formula* g = f->connective() == Formula::IFF ? f : f->arg();
        // g is the iff-part of f
        ASS( debug_unit, g->connective() == Formula::IFF);
        if (g->right() == rhs) {
          // the order is correct
          current->_defType = DT_PRED;
          return current;
        }
        // the order is wrong, build a new unit
        Formula* g1 = new Formula (Formula::IFF, g->right(), g->left());
        if (f ->connective() == Formula::FORALL) {
          g1 = new Formula (Formula::FORALL, f->vars()->copy(), g1);
        }

        // increase counter since the unit will be replaced by a new one
        current = new Unit ( IR_PERMUT, g1, current );
        current->_defType = DT_PRED;
        return current;
      }
      // it is a function definition
      // now current is the (possibly new) unit with the flattened formula f
      // check that the lt and rt are in the right order
      Formula* g = f->connective() == Formula::FORALL ? f->arg() : f;
      ASS( debug_unit, g->connective() == Formula::ATOM );
      Atom* a = g->atom ();
      ASS( debug_unit, a->isEquality() );
      Term::List* ts = a->args ();
      if (ts->head() == lt) {
        // the order is correct
        current->_defType = DT_FUN;
        return current;
      }
      // the order is wrong, build a new unit
      Formula* g1 = new Formula ( new Atom (ts->second(), ts->head()) );
      if (f ->connective() == Formula::FORALL) {
        g1 = new Formula (Formula::FORALL, f->vars()->copy(), g1);
      }

      // increase counter since the unit will be replaced by a new one
      current = new Unit ( IR_REORDER_EQ, g1, current );
      current->_defType = DT_FUN;
      return current;
    }

    default:
      ASS ( debug_unit, false );
      return 0;
  }
} // Unit::makeDefinition


// flatten the unit's formula
// 27/06/2002 Manchester
void Unit::flatten ()
{
  TRACER ( "Unit::flatten" );

  switch ( unitType () ) {
    case FORMULA:
      formula ( formula()->flatten() );
      return;

    case CLAUSE:
      return;
  }
} // Unit::flatten

// return the parents of a unit
// 22/07/2002 Manchester
Unit::List* Unit::parents () const
{
  if (rule() == IR_INPUT) {
    return Unit::List::empty ();
  }

  return _parents;
} // Unit::parents

