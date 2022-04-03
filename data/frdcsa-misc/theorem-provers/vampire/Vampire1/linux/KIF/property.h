//
//  file property.h (syntactic properties of problems)
//  06/06/2001 Manchester
//


#ifndef __property__
#define __property__


class Problem;
class Unit;
class Clause;
class Literal;
class Atom;
class Term;
class ostream;            // for debugging

class Property {
 public:

  enum Category {
    NEQ,  
    HEQ,
    PEQ,
    HNE,
    NNE,
//     FEQ,       // fof division 
//     FNE, 
    EPR,       // epr division
    UEQ        // ueq division
  };

  // constructor
  explicit Property (Problem&);

  // output
  void toString (char* string) const;
  const char* categoryToString () const;
  char Property::categoryToChar () const;

 private:
  // reading in properties of problems
  void read (Problem&);
  void read (const Unit*);
  void read (const Clause*, bool isAxiom);
  void read (const Atom*, bool& isGround);
  void read (const Term*, bool& isGround);

  char axiomTypes () const;
  char goalTypes () const;
  char equalityContent () const;
  char nonGroundUnitContent () const;
  char groundPositiveContent () const;
  char goalsAreGround () const;
  char setClauseSize () const;
  char setLiteralSize () const;
  char setTermSize () const;
  char maxFunArity () const;
  char maxPredArity () const;

  // TEMPORARY
 public:
  // derived numbers
  inline int clauses () const { return _goalClauses + _axiomClauses; }
  inline int unitClauses () const { return _unitGoals + _unitAxioms; }
  inline int hornClauses () const { return _hornGoals + _hornAxioms; }

  // structure
  int _goalClauses;
  int _axiomClauses;
  int _literals;
  int _terms;
  int _unitGoals;
  int _unitAxioms;
  int _hornGoals;
  int _hornAxioms;
  int _equationalClauses;
  int _pureEquationalClauses;
  int _groundUnitAxioms;
  int _positiveAxioms;
  int _groundPositiveAxioms;
  int _groundGoals;
  int _maxFunArity;
  int _maxPredArity;
  bool _hasFormulas;
  // derived
  Category _category;
}; // class Property


// for debugging
ostream& operator << ( ostream&, Property& );


#endif // __property__
