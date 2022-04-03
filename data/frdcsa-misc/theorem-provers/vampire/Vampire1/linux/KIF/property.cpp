//
//  file property.cpp (syntactic properties of problems)
//  06/06/2001 Manchester
//


#include "property.h"
#include "problem.h"



// Initialize Property
// 29/06/2002 Manchester
Property::Property ( Problem& p )
  :
  _goalClauses (0),
  _axiomClauses (0),
  _literals (0),
  _terms (0),
  _unitGoals (0),
  _unitAxioms (0),
  _hornGoals (0),
  _hornAxioms (0),
  _equationalClauses (0),
  _pureEquationalClauses (0),
  _groundUnitAxioms (0),
  _positiveAxioms (0),
  _groundPositiveAxioms (0),
  _groundGoals (0),
  _maxFunArity (0), 
  _maxPredArity (0),
  _hasFormulas (p.hasFormulas())
{
  read (p);

  // determine the category
//    if (p.hasFormulas()) { // FOF, either FEQ or FNE
//     if (_equationalClauses == 0) {
//       _category = FNE;
//     }
//     else {
//       _category = FEQ;
//     }
//   }
//   // no formulas in the input, one of NEQ, HEQ, PEQ, HNE, NNE, EPR, UEQ
//   else 
  if (_maxFunArity == 0) { // all function symbols are constants
    _category = EPR;
  }
  // one of NEQ, HEQ, PEQ, HNE, NNE, UEQ
  else if (_equationalClauses == 0 ) { // HNE, NNE
    if (clauses() == hornClauses()) { // all clauses are horn
      _category = HNE;
    }
    else {
      _category = NNE;
    }
  }
  // one of NEQ, HEQ, PEQ, UEQ
  else if (_pureEquationalClauses == clauses()) { // only equations, UEQ or PEQ
    if (clauses() == unitClauses()) { // all clauses are unit
      _category = UEQ;
    }
    else {
      _category = PEQ;
    }
  }
  // one of NEQ, HEQ
  else if (clauses() == hornClauses()) { // all clauses are horn
    _category = HEQ;
  }
  else {
    _category = NEQ;
  }
} // Property::Property


// read property from a problem
// 29/06/2002 Manchester
void Property::read ( Problem& p )
{
  Unit::Chain& units (p.units());
  Unit::Chain::Iterator us (units);
  while (us.more()) {
    read (us.next());
  }
} // Property::read (Problem&)


// read property from a unit
// 29/06/2002 Manchester
void Property::read ( const Unit* u )
{
  switch ( u->unitType() ) {
  case CLAUSE:
    read (u->clause(), u->inputType() == AXIOM);
    return;

  case FORMULA:
    ASS(debug_property, false);
  }
} // Property::read (const Unit*)


// read property from a clause
// 29/06/2002 Manchester
void Property::read (const Clause* clause, bool isAxiom)
{
  int positiveLiterals = 0;
  int negativeLiterals = 0;
  int equationalLiterals = 0;
  int groundLiterals = 0;
  Literal::List::Iterator lits (clause->literals());
  while ( lits.more() ) {
    Literal* literal = lits.next();
    if (literal->positive()) {
      positiveLiterals ++;
    }
    else {
      negativeLiterals ++;
    }

    Atom* atom = literal->atom();
    if (atom->isEquality()) {
      equationalLiterals++;
    }

    bool isGround = true;
    read (atom, isGround);

    if (isGround) {
      groundLiterals++;
    }
  }
  int literals = positiveLiterals + negativeLiterals;
  _literals += literals;

  if ( equationalLiterals > 0 ) {
    _equationalClauses ++;
  }
  if ( literals == equationalLiterals ) {
    _pureEquationalClauses ++;
  }

  if (isAxiom) {
    _axiomClauses ++;
    if ( literals == 1) {
      _unitAxioms ++;
      if ( groundLiterals == 1) {
	_groundUnitAxioms ++;
      }
    }
    if (positiveLiterals <= 1) {
      _hornAxioms ++;
    }
    if (negativeLiterals == 0) {
      _positiveAxioms ++;
      if (literals == groundLiterals) {
	_groundPositiveAxioms ++;
      }
    }
  }
  else {
    _goalClauses ++;
    if ( literals == 1) {
      _unitGoals ++;
    }
    if (positiveLiterals <= 1) {
      _hornGoals ++;
    }
    if (literals == groundLiterals) {
      _groundGoals ++;
    }
  }
} // Property::read (const Clause* clause, bool isAxiom)


// read property from a clause
// 29/06/2002 Manchester
void Property::read (const Atom* atom, bool& isGround)
{
  if (! atom->isEquality()) {
    int arity = atom->functor()->arity();
    if (arity > _maxPredArity) {
      _maxPredArity = arity;
    }
  }

  Term::List::Iterator args (atom->args());
  while ( args.more() ) {
    read (args.next(), isGround);
  }
} // Property::read (const Atom* term, bool& isGround)


// read property from a clause
// 29/06/2002 Manchester
void Property::read (const Term* term, bool& isGround)
{
  _terms ++;

  if (term->isVar()) {
    isGround = false;
    return;
  }

  int arity = term->functor()->arity();
  if (arity > _maxFunArity) {
    _maxFunArity = arity;
  }

  Term::List::Iterator args (term->args());
  while ( args.more() ) {
    read (args.next(), isGround);
  }
} // Property::read (const Term* term, bool& isGround)


char Property::axiomTypes () const
{
  if (_unitAxioms == _axiomClauses) { // all axioms are unit
    return 'u';
  }
  if (_hornAxioms == _axiomClauses) { // all axioms are horn
    return 'h';
  }
  // general axiom types
  return 'g';
} // Property::axiomTypes


char Property::goalTypes () const
{
  if (_unitAxioms == _goalClauses) { // all goals are unit
    return 'u';
  }
  if (_hornAxioms == _goalClauses) { // all goals are horn
    return 'h';
  }
  // general axiom types
  return 'g';
} // Property::goalTypes


char Property::equalityContent () const
{
  if (_equationalClauses == 0) { // no equality
    return 'n';
  }
  if (_pureEquationalClauses == clauses()) { // all clauses are pure equational
    return 'p';
  }
  // some equational literals but not all
  return 's';
} // Property::equalityContent


// This function has been generated automatically
// on Sun Jun 30 11:44:28 2002
char Property::nonGroundUnitContent () const
{
  float quantity = _unitAxioms ?
    ((float)(_unitAxioms - _groundUnitAxioms)) / (float)_unitAxioms :
    0.5;
 
  switch (_category) {
  case EPR:
    if (quantity <= 0.105263) { return 'f'; } // few
    if (quantity <= 0.5) { return 's'; } // some
    return 'm'; // many
  case HEQ:
    if (quantity <= 0.5) { return 'f'; } // few
    if (quantity <= 0.99999) { return 's'; } // some
    return 'm'; // many
  case UEQ:
    if (quantity <= 0.666667) { return 'f'; } // few
    if (quantity <= 0.99999) { return 's'; } // some
    return 'm'; // many
  case PEQ:
    if (quantity <= 0.933333) { return 'f'; } // few
    if (quantity <= 0.99999) { return 's'; } // some
    return 'm'; // many
  case NNE:
    if (quantity <= 1e-05) { return 'f'; } // few
    if (quantity <= 0.5) { return 's'; } // some
    return 'm'; // many
  case NEQ:
    if (quantity <= 0.7) { return 'f'; } // few
    if (quantity <= 0.807692) { return 's'; } // some
    return 'm'; // many
  case HNE:
    if (quantity <= 0.5) { return 'f'; } // few
    if (quantity <= 0.99999) { return 's'; } // some
    return 'm'; // many
  default:
    ASS(debug_property, false);
  }
} // Property::nonGroundUnitContent ()


// This function has been generated automatically
// on Sun Jun 30 11:36:41 2002
char Property::groundPositiveContent () const
{
  float quantity = _positiveAxioms ?
    ((float)(_groundPositiveAxioms)/ (float)(_positiveAxioms)) :
    0.5;
 
  switch (_category) {
  case EPR:
    if (quantity <= 0.105263) { return 'f'; } // few
    if (quantity <= 0.5) { return 's'; } // some
    return 'm'; // many
  case HEQ:
    if (quantity <= 0.5) { return 'f'; } // few
    if (quantity <= 0.99999) { return 's'; } // some
    return 'm'; // many
  case UEQ:
    if (quantity <= 0.666667) { return 'f'; } // few
    if (quantity <= 0.99999) { return 's'; } // some
    return 'm'; // many
  case PEQ:
    if (quantity <= 0.933333) { return 'f'; } // few
    if (quantity <= 0.99999) { return 's'; } // some
    return 'm'; // many
  case NNE:
    if (quantity <= 1e-05) { return 'f'; } // few
    if (quantity <= 0.5) { return 's'; } // some
    return 'm'; // many
  case NEQ:
    if (quantity <= 0.7) { return 'f'; } // few
    if (quantity <= 0.807692) { return 's'; } // some
    return 'm'; // many
  case HNE:
    if (quantity <= 0.5) { return 'f'; } // few
    if (quantity <= 0.99999) { return 's'; } // some
    return 'm'; // many
  default:
    ASS(debug_property, false);
  }
} // Property::groundPositiveContent ()


char Property::goalsAreGround () const
{
  if (_goalClauses == _groundGoals) { // all goals are ground
    return 'g';
  }
  // general axiom types
  return 'n';
} // Property::axiomTypes


// This function has been generated automatically
// on Sun Jun 30 12:53:21 2002
char Property::setClauseSize () const
{
  int quantity = clauses ();
 
  switch (_category) {
  case EPR:
    if (quantity <= 51) { return 'f'; } // few
    if (quantity <= 340) { return 's'; } // some
    return 'm'; // many
  case HEQ:
    if (quantity <= 11) { return 'f'; } // few
    if (quantity <= 18) { return 's'; } // some
    return 'm'; // many
  case UEQ:
    if (quantity <= 5) { return 'f'; } // few
    if (quantity <= 16) { return 's'; } // some
    return 'm'; // many
  case PEQ:
    if (quantity <= 4) { return 'f'; } // few
    if (quantity <= 8) { return 's'; } // some
    return 'm'; // many
  case NNE:
    if (quantity <= 30) { return 'f'; } // few
    if (quantity <= 38) { return 's'; } // some
    return 'm'; // many
  case NEQ:
    if (quantity <= 95) { return 'f'; } // few
    if (quantity <= 162) { return 's'; } // some
    return 'm'; // many
  case HNE:
    if (quantity <= 5) { return 'f'; } // few
    if (quantity <= 9) { return 's'; } // some
    return 'm'; // many
  default:
    ASS(debug_property, false);
  }
} // Property::setClauseSize ()


// This function has been generated automatically
// on Sun Jun 30 13:21:57 2002
char Property::setLiteralSize () const
{
  int quantity = _literals;
 
  switch (_category) {
  case EPR:
    if (quantity <= 98) { return 'f'; } // few
    if (quantity <= 1060) { return 's'; } // some
    return 'm'; // many
  case HEQ:
    if (quantity <= 14) { return 'f'; } // few
    if (quantity <= 29) { return 's'; } // some
    return 'm'; // many
  case UEQ:
    if (quantity <= 5) { return 'f'; } // few
    if (quantity <= 16) { return 's'; } // some
    return 'm'; // many
  case PEQ:
    if (quantity <= 6) { return 'f'; } // few
    if (quantity <= 12) { return 's'; } // some
    return 'm'; // many
  case NNE:
    if (quantity <= 77) { return 'f'; } // few
    if (quantity <= 93) { return 's'; } // some
    return 'm'; // many
  case NEQ:
    if (quantity <= 219) { return 'f'; } // few
    if (quantity <= 351) { return 's'; } // some
    return 'm'; // many
  case HNE:
    if (quantity <= 7) { return 'f'; } // few
    if (quantity <= 14) { return 's'; } // some
    return 'm'; // many
  default:
    ASS(debug_property, false);
  }
} // Property::setLiteralSize ()


// This function has been generated automatically
// on Sun Jun 30 13:40:24 2002
char Property::setTermSize () const
{
  int quantity = _terms;
 
  switch (_category) {
  case EPR:
    if (quantity <= 142) { return 'f'; } // few
    if (quantity <= 2026) { return 's'; } // some
    return 'm'; // many
  case HEQ:
    if (quantity <= 70) { return 'f'; } // few
    if (quantity <= 120) { return 's'; } // some
    return 'm'; // many
  case UEQ:
    if (quantity <= 47) { return 'f'; } // few
    if (quantity <= 127) { return 's'; } // some
    return 'm'; // many
  case PEQ:
    if (quantity <= 51) { return 'f'; } // few
    if (quantity <= 72) { return 's'; } // some
    return 'm'; // many
  case NNE:
    if (quantity <= 184) { return 'f'; } // few
    if (quantity <= 218) { return 's'; } // some
    return 'm'; // many
  case NEQ:
    if (quantity <= 834) { return 'f'; } // few
    if (quantity <= 1282) { return 's'; } // some
    return 'm'; // many
  case HNE:
    if (quantity <= 38) { return 'f'; } // few
    if (quantity <= 78) { return 's'; } // some
    return 'm'; // many
  default:
    ASS(debug_property, false);
  }
} // Property::setTermSize ()


char Property::maxFunArity () const
{
  switch (_maxFunArity) {
  case 0:
    return '0';
  case 1:
    return '1';
  case 2:
    return '2';
  default:
    return '3';
  }
} // Property::maxFunArity


char Property::maxPredArity () const
{
  switch (_maxPredArity) {
  case 0:
    return '0';
  case 1:
    return '1';
  case 2:
    return '2';
  default:
    return '3';
  }
} // Property::maxPredArity


void Property::toString (char* string) const
{
  *string++ = categoryToChar ();
  *string++ = axiomTypes ();
  *string++ = goalTypes ();
  *string++ = equalityContent ();
  *string++ = nonGroundUnitContent ();
  *string++ = groundPositiveContent ();
  *string++ = goalsAreGround ();
  *string++ = setClauseSize ();
  *string++ = setLiteralSize ();
  *string++ = setTermSize ();
  *string++ = maxPredArity ();
  *string++ = maxFunArity ();

  // mark end of string
  *string = 0;
} // Property::toString


const char* Property::categoryToString () const
{
  switch ( _category ) {
  case NEQ:
    return "NEQ";
  case HEQ:
    return "HEQ";
  case PEQ:
    return "PEQ";
  case HNE:
    return "HNE";
  case NNE:
    return "NNE";
//   case FEQ:       
//     return "FEQ";
//   case FNE:
//     return "FNE";
  case EPR:  
    return "EPR";
  case UEQ:
    return "UEQ";
  default:
    ASS (debug_property, false);
  } 
} // Property::categoryToString


char Property::categoryToChar () const
{
   if ( _hasFormulas ) { // FOF, either FEQ or FNE
     return _equationalClauses == 0 ? 'f' : 'F';
  }

  switch ( _category ) {
  case NEQ:
    return 'N';
  case HEQ:
    return 'H';
  case PEQ:
    return 'P';
  case HNE:
    return 'h';
  case NNE:
    return 'n';
//   case FEQ:       
//     return 'F';
//   case FNE:
//     return 'f';
  case EPR:  
    return 'E';
  case UEQ:
    return 'U';
  default:
    ASS (debug_property, false);
  } 
} // Property::categoryToString
