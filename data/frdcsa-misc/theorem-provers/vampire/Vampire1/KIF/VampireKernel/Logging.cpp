//
// File:         Logging.cpp
// Description:  Various utilities for logging data.
// Created:      Mar 26, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//======================================================================
#include "Logging.h"
//======================================================================
using namespace VK;

using namespace BK;
long* Logging::Integer32::_basePointer = (long)0;
bool Logging::Integer32::_basePointerSet = false;

//======================================================================
