//
// File:         Label.cpp
// Description:  
// Created:      Oct 29, 2000, 20:50
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//========================================================================
#ifndef NO_DEBUG
//========================================================================
#include "Label.h"
//==========================================================================
const ulong BK::Label::Length = ((sizeof(unsigned long)*CHAR_BIT)/4) + (((sizeof(unsigned long)*CHAR_BIT)%4) ? 1 : 0);
//==========================================================================
#endif

