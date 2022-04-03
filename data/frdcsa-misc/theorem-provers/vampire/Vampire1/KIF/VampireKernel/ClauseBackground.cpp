//
// File:         ClauseBackground.cpp
// Description:  Ancestors of a clause.
// Created:      Mar 3, 2000, 13:25
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk 
//==================================================================
#include "ClauseBackground.h"
//==================================================================
using namespace VK;
using namespace BK;

#ifdef DEBUG_ALLOC_OBJ_TYPE
 ClassDesc ClauseBackground::_classDesc("ClauseBackground",sizeof(ClauseBackground),sizeof(ClauseBackground));
 ClassDesc ClauseBackground::ClauseList::_classDesc("ClauseBackground::ClauseList",
                                                    ClauseBackground::ClauseList::minSize(),
                                                    ClauseBackground::ClauseList::maxSize());
#endif
//==================================================================
