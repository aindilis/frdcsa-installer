//
//  file typedefs.h 
//  
//  15/04/2001, Manchester
//


#ifndef __typedefs__
#  define __typedefs__

// variables
typedef int Var;
#define firstVar 0


// lists
#include "list.h"
#include "debug.h"

class Term;
typedef List <Var,CID_VARLIST> VarList;
typedef List <ulong,CID_ULONG_LIST> UlongList;
typedef List <VarList*,CID_VARLIST_LIST> VarListList;


#endif // __typedefs__
