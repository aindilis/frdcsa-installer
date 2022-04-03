//
//  file renaming.cpp
//  implements Renaming
//  currently not a very fast implementation
//

#include "renaming.h"

// 27/06/2002 Manchester
Renaming::Renaming ()
  : _bindings (List::empty())
{
} // Renaming::Renaming


// 27/06/2002 Manchester
Renaming::~Renaming ()
{
  List::Iterator bs (_bindings);
  while (bs.more()) {
    delete bs.next ();
  }
  _bindings->destroy ();
} // Renaming::~Renaming


// 27/06/2002 Manchester
void Renaming::push ( Var v, Var t)
{
  List::push ( new Binding(v,t), _bindings );
} // Renaming::push


// 27/06/2002 Manchester
Var Renaming::get (Var v) const
{
  List::Iterator bs (_bindings);
  while (bs.more()) {
    Binding* b = bs.next ();
    if (b->left() == v) {
      return b->right();
    }
  }
  // not found
  return -1;
} // Renaming::get


// 27/06/2002 Manchester
void Renaming::pop ()
{
  ASS( debug_renaming, ! _bindings->isEmpty() );

  delete List::pop (_bindings);
} // Renaming::pop


