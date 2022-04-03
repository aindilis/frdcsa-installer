//
// File:         VampireKernelInterceptMacros.cpp
// Description:  Some objects used in VampireKernelInterceptMacros.h .
// Created:      Dec 05, 2000, 14:00
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//====================================================  

#ifdef INTERCEPT_FORWARD_MATCHING_INDEXING
#include "Logging.h"
Logging FMILog(Logging::ForwardMatching);
const Flatterm* interceptedForwardMatchingQuery = (const Flatterm*)0;
long maxSizeOfFMILogFile = LONG_MAX; 
#endif

#ifdef INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING
#include "Logging.h"
Logging BSILog(Logging::BackwardSubsumption);
const Clause* interceptedBackwardSubsumptionQuery = (const Clause*)0;
ulong interceptedBackwardSubsumptionResultTotal = 0UL;
long maxSizeOfBSILogFile = LONG_MAX;
ulong interceptedBackwardSubsumptionNextSymbolToDeclare = 0UL;
#endif

#ifdef INTERCEPT_BACKWARD_MATCHING_INDEXING
#include "Logging.h"
Logging BMILog(Logging::BackwardMatching);
const TERM* interceptedBackwardMatchingQuery = (const TERM*)0;
ulong interceptedBackwardMatchingResultTotal = 0UL;
long maxSizeOfBMILogFile = LONG_MAX;
#endif

//=====================================================
