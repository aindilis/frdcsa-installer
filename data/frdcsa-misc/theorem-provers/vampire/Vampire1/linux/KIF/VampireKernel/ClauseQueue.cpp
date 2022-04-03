//
// File:         ClauseQueue.cpp
// Description:  Queues of clauses. 
// Created:      Apr 13, 2001, 16:00
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//============================================================================
#include "ClauseQueue.h"
//======================================================================
using namespace VK;
using namespace BK;
#ifdef DEBUG_ALLOC_OBJ_TYPE
 #include "ClassDesc.h"
 ClassDesc ClauseQueue::List::_classDesc("ClauseQueue::List",
					 ClauseQueue::List::minSize(),
                                         ClauseQueue::List::maxSize());
#endif
//======================================================================
