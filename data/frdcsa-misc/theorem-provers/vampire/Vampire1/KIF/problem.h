//
//  file problem.h 
//  defines class Problem
//


#ifndef __problem__
#define __problem__


#include "typedefs.h"
#include "unit.h"
#include "formula.h"


class FSymbol;
class Options;


// ****************** class Problem, definition *******************


class Problem {
 public:
  // constructors & destructors
  explicit Problem ( Unit::List* );
  explicit Problem (Formula::List* kifDB);
  ~Problem ();
  void preprocess (Options&);

  // query the structure
  Unit::Chain& units () { return _units; }

  // properties
  bool hasFormulas () const { return _hasFormulas; }

  // miscellaneous
  int removeUnusedDefinitions (); // the result is the number of removed definitions
  int removeEqualityAxioms ();
  int split ();
  void ennf ();
  int removeIff ();
  int miniscope ();
  void flatten ();
  void rectify ();
  void skolemize (); 
  void clausify ();
  void addUnit (Unit* u); // for incremental building
  void removeAllConjectures (); // for work with KIF
  VarList* originalAnswerVariables () const
    { return _originalAnswerVariables; }
  Literal* answerLiteral () const { return _answerLiteral; }

 private:

  // structure
  Unit::Chain _units;
  bool _hasFormulas;
  VarList* _originalAnswerVariables;
  Literal* _answerLiteral;

  // auxiliary functions
  Formula* hasFreeVars () const;
  int findDefinitions ();
  void clausify (Formula::List* formulas, 
		 Literal::List* literals,
		 Unit* parent);
};


ostream& operator << ( ostream&, Problem& );

 
#endif // __problem_
