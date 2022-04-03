//
// File:         ClauseQueue.h
// Description:  Queues of clauses. 
// Created:      Apr 13, 2001, 16:00
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//============================================================================
#ifndef CLAUSE_QUEUE_H
//=============================================================================
#define CLAUSE_QUEUE_H 
#include "jargon.h"
#include "FIFO.h"
#include "GlobAlloc.h"
#include "Clause.h"
//======================================================================

namespace VK
{
  using namespace BK;
typedef FIFO<GlobAlloc,Clause*,Clause> ClauseQueue;

}; // namespace VK

//======================================================================
#endif
