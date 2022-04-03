//
//  file sym_counter.h 
//  defines class SymCounter counting occurrences of function and predicate symbols
//
//  01/05/2002, Manchester
//

#ifndef __sym_counter__
#define __sym_counter__


class Problem;
class Unit;
class Clause;
class Formula;
class Literal;
class Atom;
class Term;


#include "signature.h"


// ***************** class SymCounter, definition ****************************


class SymCounter { 
 public:

  class Fun {
    Signature::Fun* _fun;
    int _occ;
   public:
    Fun () : _occ(0) {}
    void fun (Signature::Fun* f) { _fun = f; }
    int occ () const { return _occ; }
    Signature::Fun* fun () { return _fun; }
    void add (int add) { _occ += add; }
  };

  class Pred {
    Signature::Pred* _pred;
    int _pocc;  // positive occurrences
    int _nocc;  // negative occurrences
    int _docc;  // double occurrences (under equivalence)
   public:
    Pred () 
      : _pocc (0), 
        _nocc (0),
        _docc (0) 
        {}
    void pred (Signature::Pred* p) { _pred = p; }
    int pocc () const { return _pocc; }
    int nocc () const { return _nocc; }
    int docc () const { return _docc; }
    Signature::Pred* pred () { return _pred; }
    void add (int polarity, int add);
  };

  explicit SymCounter (Signature&);
  ~SymCounter ();

  // counting functions
  void count (Problem&, int);
  void count (Unit*, int);
  void count (Clause*, int);
  void count (Formula*, int polarity, int add);
  void count (Literal*, int);
  void count (Atom*, int polarity, int add);
  void count (Term*, int add);

  // get 
  Pred* get (const Signature::Pred* p) { return _preds + p->number(); }
  Fun* get (const Signature::Fun* f) { return _funs + f->number(); }

  static SymCounter* norm;    // used for normalisation

 private:

  // structure
  int _noOfPreds;
  int _noOfFuns;
  Pred* _preds;
  Fun* _funs;

 friend ostream& operator << ( ostream& str, const SymCounter& c );
}; // class SymCounter


#endif // __sym_counter__
