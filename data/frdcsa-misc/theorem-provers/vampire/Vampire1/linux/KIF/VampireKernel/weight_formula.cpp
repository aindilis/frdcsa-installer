#include "weight_formula.h"

//=================================================
#undef DEBUG_NAMESPACE  
#ifdef DEBUG_WEIGHT_FORMULA
 #define DEBUG_NAMESPACE "LARGE_WEIGHT_FORMULA"
#endif
#include "debugMacros.h"
//=================================================

namespace VK
{
LARGE_WEIGHT_FORMULA::LARGE_WEIGHT_FORMULA() 
 : var_counter(), 
   const_part(0L) 
{
  CALL("constructor LARGE_WEIGHT_FORMULA()");
};


#ifndef NO_DEBUG_VIS
ostream& LARGE_WEIGHT_FORMULA::output(ostream& str)
{
  var_counter.ResetIteration();
  str << const_part;
  for (const Unifier::Variable* var = var_counter.NextRegistered();var;var = var_counter.NextRegistered())
    {
      long score = var_counter.Score(var);
      if (score < 0) { str << " - " << score*(-1); }
      else str << " + " << score;
      var->output(str);
    };
  return str; 
}; // ostream& LARGE_WEIGHT_FORMULA::output(ostream& str)

ostream& LARGE_WEIGHT_FORMULA::outputWithSubst(ostream& str)
{
  output(str) << "  WHERE\n";
  return Unifier::current()->output(str,PREFIX::outputWordWithSubst);
}; // ostream& LARGE_WEIGHT_FORMULA::outputWithSubst(ostream& str)

#endif 

}; // namespace VK

//=================================================
#undef DEBUG_NAMESPACE  
#ifdef DEBUG_WEIGHT_FORMULA
 #define DEBUG_NAMESPACE "ORD_FORMULA"
#endif
#include "debugMacros.h"
//=================================================

namespace VK
{
#ifndef NO_DEBUG_VIS
ostream& ORD_FORMULA::output(ostream& str)
{
  PREFIX::outputWordWithSubst(str,term1) << " > ";
  PREFIX::outputWordWithSubst(str,term2) << '\n';
  str << "DIFF.FORMULA:   ";
  difference.output(str) << '\n';
  str << "COMPUTED DIFF.: "; 
  return subst_diff.output(str) << '\n';
}; // ostream& ORD_FORMULA::output(ostream& str) 
  
ostream& ORD_FORMULA::outputWithSubst(ostream& str)
{
  PREFIX::outputWordWithSubst(str,term1) << " > ";
  PREFIX::outputWordWithSubst(str,term2) << '\n';
  str << "DIFF.FORMULA:   ";
  difference.outputWithSubst(str) << '\n';
  str << "COMPUTED DIFF.: "; 
  return subst_diff.output(str) << '\n';
}; // ostream& ORD_FORMULA::outputWithSubst(ostream& str)

#endif


}; // namespace VK


//=================================================
#undef DEBUG_NAMESPACE  
#ifdef DEBUG_WEIGHT_FORMULA
 #define DEBUG_NAMESPACE "WEIGHT_FORMULA"
#endif
#include "debugMacros.h"
//=================================================

namespace VK
{
#ifndef NO_DEBUG_VIS
ostream& WEIGHT_FORMULA::output(ostream& str) 
{
  var_counter.ResetIteration();
  str << const_part;
  for (const Unifier::Variable* var = var_counter.NextRegistered();var;var = var_counter.NextRegistered())
    { 
      str << " + " << var_counter.Score(var);
      var->output(str);
    };  
  return str;
}; // ostream& WEIGHT_FORMULA::output(ostream& str)     
#endif

}; // namespace VK


//=================================================
