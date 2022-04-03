//
//  file renaming.h 
//  defines class Renaming
//  currently not a very fast implementation
//

#ifndef __renaming__
#define __renaming__


#include "typedefs.h"
#include "memory.h"
#include "tracer.h"
#include "term.h"
#include "pair.h"


class Renaming
{
 public:
  Renaming ();
  ~Renaming ();
  void push ( Var v1, Var v2);
  Var get (Var v1) const; // -1 if not found
  void pop ();

 private:
  typedef Pair<Var, Var, CID_REN_BINDING> Binding;
  typedef List<Binding*, CID_REN_BINDING_LIST> List;

  List* _bindings;
}; // class Renaming


#endif // __renaming__
