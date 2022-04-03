#include <iostream.h>

#include "memory.h"
#include "assert.h"

#define mem_static(CID) Memory<CID>* Memory<CID>::_freeList = 0; Memory<CID>* Memory<CID>::_allocated = 0; int Memory<CID>::_allocs = 0; int Memory<CID>::_deallocs = 0; Memory<CID>::LeakChecker dummy_##CID;

mem_static (CID_TERM);
mem_static (CID_ATOM);
mem_static (CID_FORMULA);
mem_static (CID_VARLIST);
mem_static (CID_INTLIST);
mem_static (CID_VARLIST_LIST);
mem_static (CID_ASSOC_PAIR_LIST);
mem_static (CID_SYMBOL);
mem_static (CID_FSYM_LIST);
mem_static (CID_PSYM_LIST);
mem_static (CID_TERM_LIST);
mem_static (CID_ATOM_LIST);
mem_static (CID_LITERAL_LIST);
mem_static (CID_CLAUSE_LIST);
mem_static (CID_FORMULA_LIST);
mem_static (CID_ENTRY);
mem_static (CID_ENTRY_LIST);
mem_static (CID_UNIT);
mem_static (CID_UNIT_LIST);
mem_static (CID_UNIT_LINK);
mem_static (CID_BINDING);
mem_static (CID_BINDING_LIST);
mem_static (CID_REN_BINDING);
mem_static (CID_REN_BINDING_LIST);
mem_static (CID_OPTION_LIST);
mem_static (CID_ULONG_LIST);

const char* id2name (ClassID cid)
{
  switch (cid) {
    case CID_TERM:
      return "Term";
    case CID_ATOM:
      return "Atom";
    case CID_FORMULA:
      return "Formula";
    case CID_VARLIST:
      return "Variable list";
    case CID_INTLIST:
      return "Integer list";
    case CID_VARLIST_LIST:
      return "Variable list list";
    case CID_ASSOC_PAIR_LIST:
      return "Assoc pair list";
    case CID_SYMBOL:
      return "Symbol";
    case CID_FSYM_LIST:
      return "Signature::Fun list";
    case CID_PSYM_LIST:
      return "Signature::Pred list";
    case CID_TERM_LIST:
      return "Term list";
    case CID_ATOM_LIST:
      return "Atom list";
    case CID_LITERAL_LIST:
      return "Literal list";
    case CID_CLAUSE_LIST:
      return "Clause list";
    case CID_FORMULA_LIST:
      return "Formula CList";
    case CID_ENTRY:
      return "Entry";
    case CID_ENTRY_LIST:
      return "Entry list";
    case CID_UNIT:
      return "Unit";
    case CID_UNIT_LIST:
      return "Unit list";
    case CID_UNIT_LINK:
      return "Unit link";
    case CID_BINDING:
      return "Substitution::Binding";
    case CID_BINDING_LIST:
      return "Substitution::Binding list";
    case CID_REN_BINDING:
      return "Renaming::Binding";
    case CID_REN_BINDING_LIST:
      return "Renaming::Binding list";
    case CID_OPTION_LIST:
      return "Option list";
    case CID_ULONG_LIST:
      return "ulong list";
    default:
      return "Unidentified class";
  }
} // id2name

// "private: static class Memory<3> *  Memory<3>::_freeList" (?_freeList@?$Memory@$02@@0PAV1@A)
// "private: static class Memory<3> *  Memory<3>::_freeList" (?_freeList@?$Memory@$02@@0PAV1@A)
