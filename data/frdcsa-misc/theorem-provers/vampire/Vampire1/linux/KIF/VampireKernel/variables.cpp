
//
// File:         variables.cpp
// Description:  Various classes for dealing with variables and substitutions.
// Created:      Feb 15, 2000, 21:10
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//====================================================
#include "variables.h"
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_VARIABLES
 #define DEBUG_NAMESPACE "Variable"
#endif
#include "debugMacros.h"
//=================================================

using namespace VK;

/********
Unifier::Variable Unifier::Variable::var[VampireKernelConst::MaxNumOfIndices*VampireKernelConst::MaxNumOfVariables];
ulong Unifier::Variable::offset[VampireKernelConst::MaxNumOfIndices];
const void* Unifier::Variable::end_of_vars = (void*)(Unifier::Variable::var + (VampireKernelConst::MaxNumOfIndices*VampireKernelConst::MaxNumOfVariables));
********/

long Unifier::InitStatic::_count; // implicitely initialised to 0L
Unifier* Unifier::_current;
ulong Unifier::_offsetForIndex[VampireKernelConst::MaxNumOfIndices];

#ifndef NO_DEBUG_VIS
ostream& Unifier::Variable::output(ostream& str) const
{
  ulong num;
  ulong index;
  NumAndIndex(num,index);    
  return str << 'X' << num << '{' << index << '}';
}; //  ostream& Unifier::Variable::output(ostream& str) const
    
ostream& Unifier::Variable::outputBinding(ostream& str,ostream& (*outputTerm)(ostream& str,const PREFIX_SYM* t)) const
{ 
  output(str) << " := ";
  if (!Binding()) return str << "NULL"; 
  if (Unifier::current()->isVariable(Binding()))
    return ((Unifier::Variable*)Binding())->output(str);
  return outputTerm(str,(const PREFIX_SYM*)Binding());
}; // ostream& Unifier::Variable::outputBinding(ostream& str,ostream& (*outputTerm)(ostream& str,const PREFIX_SYM* t)) const
#endif

//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_VARIABLES
 #define DEBUG_NAMESPACE "Unifier"
#endif
#include "debugMacros.h"
//=================================================

#ifndef NO_DEBUG_VIS
ostream& Unifier::output(ostream& str,ostream& (*outputTerm)(ostream& str,const PREFIX_SYM* t)) const
{ 
  for(const Unifier::Variable* const * p = bound; p<next; p++)
    (*p)->outputBinding(str,outputTerm) << '\n'; 
  return str;        
}; // ostream& Unifier::output(ostream& str,ostream& (*outputTerm)(ostream& str,const PREFIX_SYM* t)) const
#endif


//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_VARIABLES
 #define DEBUG_NAMESPACE "VAR_TO_INT_RENAMING"
#endif
#include "debugMacros.h"
//=================================================


#ifndef NO_DEBUG_VIS
ostream& VAR_TO_INT_RENAMING::output(ostream& str) const
{
  str << "VAR_TO_INT_RENAMING:\n";
  for (const Unifier::Variable* const * r = renamed; r < next; r++)
    (*r)->output(str) << " -> " << (ulong)(*(renaming.Entry(*r))) << '\n';
  return str << "END OF VAR_TO_INT_RENAMING.\n";
}; // ostream& VAR_TO_INT_RENAMING::output(ostream& str) const
    
#endif


//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_VARIABLES
 #define DEBUG_NAMESPACE "INT_VAR_RENAMING"
#endif
#include "debugMacros.h"
//=================================================



#ifndef NO_DEBUG_VIS
ostream& INT_VAR_RENAMING::output(ostream& str) const
{
  str << "VARIABLE RENAMING:\n";
  for (const ulong* r = renamed; r < next; r++)
    str << 'X' << *r << " -> " << 'X' << renaming[*r] << '\n';
  return str << "END OF VARIABLE RENAMING.\n";
}; // ostream& INT_VAR_RENAMING::output(ostream& str) const
#endif




//====================================================
