//
// File:         Malloc.cpp
// Description:  
// Created:      Oct 29, 2000, 21:00
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//========================================================================
#include "Malloc.h"
#include "BlodKorvDebugFlags.h"
//==========================================================================
#ifdef DEBUG_ALLOC
 const size_t BK::Malloc::_prefSize = sizeof(ObjDesc);
 const size_t BK::Malloc::_nprefSize = sizeof(ObjDesc)/sizeof(void*);
#endif
//==========================================================================


