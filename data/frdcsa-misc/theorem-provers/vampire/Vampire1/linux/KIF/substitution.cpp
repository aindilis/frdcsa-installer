//
//  file substitution.cpp
//  implements Substitution
//  currently not a very fast implementation
//

#include "substitution.h"

// 27/06/2002 Manchester
Substitution::Substitution ()
  : _bindings (List::empty())
{
} // Substitution::Substitution


// 27/06/2002 Manchester
Substitution::~Substitution ()
{
  List::Iterator bs (_bindings);
  while (bs.more()) {
    Binding* b = bs.next ();
    b->right()->dec();
    delete b;
  }
  _bindings->destroy ();
} // Substitution::~Substitution


// 27/06/2002 Manchester
void Substitution::push ( Var v, Term* t)
{
  t->inc ();
  List::push ( new Binding(v,t), _bindings );
} // Substitution::push


// 27/06/2002 Manchester
Term* Substitution::get (Var v) const
{
  List::Iterator bs (_bindings);
  while (bs.more()) {
    Binding* b = bs.next ();
    if (b->left() == v) {
      return b->right();
    }
  }
  // not found
  return 0;
} // Substitution::get


// 27/06/2002 Manchester
void Substitution::pop ()
{
  ASS( debug_substitution, ! _bindings->isEmpty() );

  Binding* b = List::pop (_bindings);
  b->right()->dec ();
  delete b;
} // Substitution::pop


ostream& operator << (ostream& str, Substitution& sbst)
{
  str << '[';

  Substitution::List::Iterator bindings (sbst._bindings);
  while (bindings.more()) {
    Substitution::Binding* bind = bindings.next();
    str << 'X' << bind->left() << '=' 
	<< bind->right();
    if (bindings.more()) {
      str << ',';
    }
  }

  str << ']';
  return str;
} // operator << (ostream& str, Substitution& sbst)


// collect into VarList lhs of all bindings
// 03/08/2002 Torrevieja
VarList* Substitution::originalVars (const List* bindings)
{
  if (bindings->isEmpty()) {
    return VarList::empty ();
  }

  return new VarList (bindings->head()->left(),
	              originalVars (bindings->tail()));
} // Substitution::originalVars


// collect into VarList rhs of all bindings
// 03/08/2002 Torrevieja
Term::List* Substitution::terms (const List* bindings)
{
  if (bindings->isEmpty()) {
    return Term::List::empty ();
  }

  return new Term::List (bindings->head()->right(),
	                 terms (bindings->tail()));
} // Substitution::terms

