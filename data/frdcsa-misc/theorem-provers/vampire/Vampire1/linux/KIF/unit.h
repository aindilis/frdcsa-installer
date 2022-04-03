//
//  file unit.h 
//


#ifndef __unit__
#define __unit__


#include "debug.h"
#include "typedefs.h"
#include "string.h"
#include "int.h"
#include "signature.h"
#include "chain.h"
#include "clause.h"


class Formula;
class Clause;
class Atom;
class Problem;


enum InputType {
  AXIOM,
  CONJECTURE,
  HYPOTHESIS
};


enum UnitType {
  CLAUSE,
  FORMULA
};


enum DefType {
  DT_NONE,
  DT_PRED,
  DT_FUN
};


enum InferenceRule {
  IR_INPUT,         // input formula or clause
  IR_PROP,          // obtained by propositional manipulations, e.g., a -> b replaced by ~a \/ b
  IR_PERMUT,        // obtained by permutations, e.g. f <=> g replaced by g <=> f
  IR_FLATTEN,       // obtained by some form of flattening (quantifiers, conjunctions etc.)
  IR_REORDER_EQ,    // obtained by reordering equality
  IR_HALF_EQUIV,    // obtained by rewriting a positive equivalence into an implication
  IR_AND_ELIM,      // obtain Ai from A1 & ... & An
  IR_ENNF,          // clear
  IR_RM_EQUIV,      // remove equivalences
  IR_MINISCOPE,
  IR_SKOLEMIZE,
  IR_RECTIFY,
  IR_SWAP,          // obtained by using commutativity of connectives, eg &
  IR_CLAUSIFY       // obtain clause from a formula
}; 


// ****************** class Unit, definition *******************


class Unit 
#   if debug_memory
    : public Memory <CID_UNIT>
#   endif
{ 
 public:

  typedef List<Unit*,CID_UNIT_LIST> List;
  typedef Chain<Unit*,CID_UNIT_LINK> Chain;

  Unit ( char* name, InputType untype, Formula* );
  Unit ( char* name, InputType untype, Clause* );
  Unit ( InferenceRule, Formula*, Unit* parent );
  Unit ( InferenceRule, Clause*, Unit* parent );

  ~Unit (); // also destroys the unit

  // query/change structure
  char* name () const { return _name; }
  InferenceRule rule () const { return _rule; }
  InputType inputType () const { return _intype; }
  UnitType unitType () const { return _untype; }
  DefType defType () const { return _defType; }
  long number () const { return _number; }
  void inc () { _count++; }
  void dec ();
  List* parents () const;

  Formula* formula () const 
    { ASS ( debug_problem, unitType() == FORMULA );
      return _formula; }
  void formula ( Formula* f ); // note: only resets an already existing formula
  Clause* clause () const
    { ASS ( debug_problem, unitType() == CLAUSE );
      return _clause; }

  // properties
  bool occurs ( const Signature::Pred* p ) const;
  bool isEqualityAxiom () const;      // is one of the equality axioms
  bool formulaWithFreeVars () const;  // is a formula with free variables
  Atom* isPredicateDefinition (Formula* & rhs) const;

  // miscellaneous
  Clause::List* skolemize ();
  Unit* makeDefinition (); 
  void flatten ();

 private:

  // structure
  UnitType _untype;
  InputType _intype;
  InferenceRule _rule;  
  long _number;
  union {
    List* _parents;
    char* _name;
  };
  union {
    Clause* _clause;
    Formula* _formula;
  };
  DefType _defType;
  int _count; // count the number of references for deleted units

  static long _lastNumber;
}; // class Unit


ostream& operator << ( ostream&, InputType );
ostream& operator << ( ostream&, UnitType );
ostream& operator << ( ostream&, Unit::Chain& );



#endif // __unit__
