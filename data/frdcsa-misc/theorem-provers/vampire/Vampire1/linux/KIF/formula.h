//
//  file formula.h 
//  defines class Formula
//

#ifndef __formula__
#define __formula__


#include "typedefs.h"
#include "term.h"
#include "clause.h"
#include "clist.h"
#include "memory.h"
#include "tracer.h"


class Atom;
class Substitution;


class Formula 
#   if debug_memory
    : public Memory <CID_FORMULA>
#   endif
{
 public:

  typedef CList <Formula*,CID_FORMULA_LIST> List;
  
  enum Connective {
    ATOM = 0,     // WARNING: the order is important for
    AND = 1,      // normalize() to work
    OR = 2,
    IMP = 3,
    IFF = 4,
    XOR = 5,
    NOT = 6,
    FORALL = 7,
    EXISTS = 8
  }; // enum Formula::Connective

  // constructors
  Formula ( Connective c, Formula* l, Formula* r) : // for binary formulas
    _con (c),
    _left (l),
    _right (r),
    _count (0)
    { ASS(debug_formula, c == IFF || c == XOR || c == IMP);
      l->inc ();
      r->inc (); }
  Formula ( Connective c, Formula* a ) : // for unary formulas
    _con ( c ),
    _arg ( a ),
    _count (0)
    { ASS ( debug_formula, c == NOT );
      a->inc (); }
  Formula ( Connective c, List* fs ) : // for and/or formulas
    _con ( c ),
    _args ( fs ),
    _count (0)
    { ASS ( debug_formula, c == AND || c == OR );
      if (fs->isNonEmpty()) 
        fs->inc (); } 
  Formula (Connective c, VarList* vs, Formula* a) : // for quantified formulas
    _con (c),
    _vars (vs),
    _arg (a),
    _count (0)
    { ASS ( debug_formula, c == FORALL || c == EXISTS );
      a->inc (); } 
  explicit Formula ( Atom* a ) : // for atomic formulas
    _con (ATOM),
    _atom (a),
    _count (0)
    { a->inc(); } 

  // similar to a binary constructor
  static Formula* junction (Connective, Formula* l, Formula* r); 
  // the same but only used by the parser
  static Formula* parseJunction (Connective, Formula* l, Formula* r); 

  void inc () { _count++; }
  void dec () 
    { ASS (debug_formula, _count > 0);
      TRACER( "Formula::dec" );

      _count--; 
      if (_count == 0) {
        delete this;
      }
    }

  // query the structure
  Connective connective () const { return _con; } 

  Atom* atom () const
    { ASS ( debug_formula, _con == ATOM );
      return _atom; } 
  Formula* left () const
    { ASS ( debug_formula, _con == IFF || _con == XOR || _con == IMP );
      return _left; } 
  Formula* right () const
    { ASS ( debug_formula, _con == IFF || _con == XOR || _con == IMP );
      return _right; } 
  Formula* arg () const
    { ASS ( debug_formula, _con == FORALL || _con == EXISTS || _con == NOT );
      return _arg; } 
  List* args () const
    { ASS ( debug_formula, _con == AND || _con == OR );
      return _args; }
  VarList* vars () const
    { ASS ( debug_formula, _con == FORALL || _con == EXISTS );
      return _vars; } 

  // statistics
  bool isEqualityAxiom () const;  // is one of the equality axioms
  bool occurs ( const Signature::Pred* p ) const; // predicate symbol f occurs in the formula

  // miscellaneous
  void prettyPrint ( int indent, ostream& );
  Atom* isPredicateDefinition ( Formula* & rhs );   // return lhs of the definition or 0
  Term* isFunctionDefinition ( Term* & rhs ) const; // return lhs of the definition or 0
  bool hasFreeVars () const; 
  List* conjunctionsOut (bool polarity);
  Formula* ennf (bool polarity);
  Formula* removeIff (bool polarity);
  Formula* miniscope ();
  Formula* flatten ();  // flatten the formula
  Formula* rectify (); // renames variables in the order of their occurrence
  Formula* rectify (VarList*& originalVars, Term::List*& answerAtom); // same
  Formula* skolemizeNNF () const;
  void outputKIF (ostream& to, int indent) const; // output in the KIF format

 private: 
  // structure
  Connective _con;

  union {
    Formula* _left;
    Atom* _atom;
    VarList* _vars;
  };
  union {
    Formula* _right;
    Formula* _arg;
    List* _args;
  };
  long _count;

  // destructor
  ~Formula ();

  // structure change
  void connective ( Connective c ) { _con = c; }

  void args ( List* as )
    { ASS ( debug_formula, _con == AND || _con == OR );
      as->inc ();
      _args->dec ();
      _args = as; }

  // auxiliary functions
  static char* printName ( Connective );
  void print ( ostream& );
  void print ( Connective outer, ostream& );
  void prettyPrint ( Connective outer, int indent, ostream& );
  bool printOneLine ( int& limit ) const;  // 
  bool printOneLine () const;
  bool hasFreeVars (VarListList*) const; 
  static List* conjunctionsOut (List*, bool polarity);
  static Formula* ennf (Connective andOr, List*, bool polarity);
  static List* removeIff (List*, bool polarity);
  bool occurs (Var v) const; // variable v occurs free in the formula
  static List* flatten ( List*, Connective );  
  Formula* rectify ( Substitution&, Var& last, VarList*& freeVars ); 
  static List* rectify ( List*, Substitution&, Var& last, VarList*& freeVars ); 
  static VarList* rectifyVars ( VarList*, Substitution&, Var& last ); 
  Formula* skolemizeNNF (Substitution& subst, const VarList*) const; 
  static List* skolemizeNNF (const List*, Substitution& subst, const VarList*);
  static const char* KIFPrintName (Connective c);

  // miniscoping
  class Miniscope;
  static List* miniscope (Connective, List*);
  Formula* topMiniscope (Connective, VarList*);
  static List* topMiniscope (Connective, VarList*, List*);
  VarList* occurring (VarList*);
  static List* pushVarsDown ( List*, Connective quant, int j, VarList*);

/*
  bool isLinearAndFlat (); 
  static List* copy( List* );
  void copyFrom ( const Formula* );
  void removeIff ();  // remove IFF and XOR
  void removeIff ( Connective c );  // remove IFF and XOR, c is the external connective (AND or OR)
  Formula* copy ();
  static List* sort ( List* );
  static List* sort ( List* toSort, List* appendTo );
  static void split ( Formula*, List*, // for sorting
                      List*& less, List*& equal, List*& greater );
*/
  // equality axiom check
  bool isReflexivityAxiom () const;
  bool isFunctionReflexivityAxiom () const;
  bool isPredicateReflexivityAxiom () const;
  bool isSymmetryAxiom () const;
  bool isTransitivityAxiom () const;
/*
  void miniscope (bool sign);
  void miniscope (bool sign, Connective quantifier, VarList*);
  // void moveDown (bool sign,VarList*); // auxiliary for miniscope
  VarList* occur ( const VarList* vs ) const; // sublist of those variables in vs occuring in the formula
  void nnf (bool sign); 

  typedef Alist <Var,Term*> Subst;
                          Subst& sbst, Clause::List*& result );
*/
 friend ostream& operator << ( ostream&, Formula* );
}; // class Formula


ostream& operator << ( ostream&, Formula* );

#endif // __formula__
