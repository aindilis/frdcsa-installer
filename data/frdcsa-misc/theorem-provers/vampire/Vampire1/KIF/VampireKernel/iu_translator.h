// Revised:    Jan 16, 2002
//             Bug fix in IU_TRANSLATOR::CompilePseudoLiteral(..).
//             The bug was introduced in v2.64.
//===========================================================
#ifndef IU_TRANSLATOR_H
//===========================================================
#define IU_TRANSLATOR_H
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "iu_command.h"
#include "GlobAlloc.h"
#include "ExpandingStack.h"
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_SUPERPOSITION
 #define DEBUG_NAMESPACE "IU_TRANSLATOR"
#endif
#include "debugMacros.h"
//=================================================
namespace VK
{

class IU_TRANSLATOR
 {
  public: 
   IU_TRANSLATOR() : var_counter() 
   {
    CALL("constructor IU_TRANSLATOR()");
   };
   void init() 
   {
    CALL("init()");
    var_counter.init();
   };
   ~IU_TRANSLATOR() { CALL("destructor ~IU_TRANSLATOR()"); };
   inline bool CompileLiteral(PREFIX_SYM* word,
                              IU_COMMAND*& code_mem,
                              PREFIX_SYM*& data_mem,
                              ulong max_code_size);
   inline bool CompilePseudoLiteral(TERM header,
                                    PREFIX_SYM* word,
                                    IU_COMMAND*& code_mem,
                                    PREFIX_SYM*& data_mem,
                                    ulong max_code_size);   
  private:
   LARGE_VAR_COUNTER var_counter;
   ExpandingStack<GlobAlloc,IU_COMMAND*,128UL,IU_TRANSLATOR> _openBrackets;
   ExpandingBracketCounter _bracketCounter;
 }; // class IU_TRANSLATOR

//******************* Definitions for IU_TRANSLATOR: *********************

inline bool IU_TRANSLATOR::CompileLiteral(PREFIX_SYM* word,
                                          IU_COMMAND*& code_mem,
                                          PREFIX_SYM*& data_mem,
                                          ulong max_code_size)
 {
  CALL("CompileLiteral(PREFIX_SYM*,IU_COMMAND*&,PREFIX_SYM*&,ulong)");
  
 
  if (!max_code_size) return false;
  _openBrackets.reset(); 

  PREFIX::ITER iter;

  _bracketCounter.reset();

  iter.Reset(word);
     
  IU_COMMAND* var_list = (IU_COMMAND*)0; 
  IU_COMMAND* start = code_mem;
  PREFIX_SYM* data = data_mem;
  if (iter.CurrSubterm()->Head().Func().arity())
   {
    _bracketCounter.openSafe(iter.CurrSubterm()->Head().Func().arity());
   };

  _openBrackets.pushSafe(code_mem);

  code_mem->Tag() = IU_COMMAND::START;
  code_mem->Symbol() = word->Head();
  data_mem->Head() = word->Head();
  //code_mem->Symbol().Func().Inversepolarity();
  code_mem->Term() = data_mem;
  code_mem++;
  data_mem++;
  max_code_size--;  
  iter.SkipSym(); // skipping header
     
  var_counter.Reset();
     
  Unifier::Variable* var;
  IU_COMMAND* last_var = code_mem-1;
  IU_COMMAND* last_var_not_fo = code_mem-1;
  ulong closed = 0UL;
     
  check_iter:
   if (!max_code_size) return false; 
   // ^ this check is enough since at most one instruction is written 
   // per iteration cycle
   
   if (iter.Empty()) 
    {
     code_mem->Tag() = IU_COMMAND::END;
     code_mem++;
     max_code_size--;
     data->SetBracket(data_mem); // needed only when the literal is propositional
     start->AllVars() = var_list;
     return true;
    };

   var = iter.CurrVar();
   if (var)
    { 
     if (var_counter.Register(var))
      {
       code_mem->Tag() = IU_COMMAND::VAR;
       last_var_not_fo = code_mem;
      }
     else // first occurence of the variable
      {
       code_mem->Tag() = IU_COMMAND::VAR_FIRST_OCC;
       code_mem->NextVar() = var_list;
       var_list = code_mem;
      };
     code_mem->Symbol().MkVar(var);
        
     last_var = code_mem;
        
     data_mem->Head().MkVar(var);
     data_mem->SetBracket(data_mem + 1);
     closed = _bracketCounter.close();
     //closed = bracket_counter.Close();
    }
   else // functor
    {
        
     ulong arity = iter.CurrSubterm()->Head().Func().arity();
     if (arity)
      {
       code_mem->Tag() = IU_COMMAND::FUNC;
       
       _openBrackets.pushSafe(code_mem);
       
       _bracketCounter.openSafe(arity);

      }
     else // constant
      {
       code_mem->Tag() = IU_COMMAND::CONST;
       closed = _bracketCounter.close();

       data_mem->SetBracket(data_mem + 1);
      };
     code_mem->Symbol() = iter.CurrSubterm()->Head();
     code_mem->Term() = data_mem;
     data_mem->Head() = iter.CurrSubterm()->Head();
    };
       
   iter.SkipSym();
   code_mem++; 
   data_mem++;
   max_code_size--;  

   while (closed)
    {
     IU_COMMAND* currentOpenBracket = _openBrackets.pop(); 
     currentOpenBracket->NextCommand() = code_mem;    
     unsigned length = code_mem - currentOpenBracket;
     currentOpenBracket->Term()->SetBracket(currentOpenBracket->Term() + length);

     if (last_var < currentOpenBracket) // the term is ground
      {
       currentOpenBracket->Tag() = IU_COMMAND::FUNC_GROUND;
      }
     else
      {
       if (last_var_not_fo < currentOpenBracket)
        {
         currentOpenBracket->Tag() = IU_COMMAND::FUNC_PLAIN;
        };
      };

     closed--;
    };
      
   goto check_iter;
 }; // bool IU_TRANSLATOR::CompileLiteral(PREFIX_SYM* word,IU_COMMAND*& code_mem,PREFIX_SYM*& data_mem,ulong max_code_size)


inline bool IU_TRANSLATOR::CompilePseudoLiteral(TERM header,
                                                PREFIX_SYM* word,
                                                IU_COMMAND*& code_mem,
                                                PREFIX_SYM*& data_mem,
                                                ulong max_code_size)
 {
   CALL("CompilePseudoLiteral(TERM,PREFIX_SYM*,IU_COMMAND*&,PREFIX_SYM*&)");
  // word must represent a complex term

  if (!max_code_size) return false;
  _openBrackets.reset(); 


  PREFIX::ITER iter;

  _bracketCounter.reset();

  iter.Reset(word);
     
  IU_COMMAND* var_list = (IU_COMMAND*)0; 
  IU_COMMAND* start = code_mem;
  PREFIX_SYM* data = data_mem;
  if (iter.CurrSubterm()->Head().Func().arity())
    {
     _bracketCounter.openSafe(iter.CurrSubterm()->Head().Func().arity());
    };

  _openBrackets.pushSafe(code_mem);

  code_mem->Tag() = IU_COMMAND::START;
  
  
  PREF_SYM pr_header(header);     
  code_mem->Symbol() = pr_header; 
  data_mem->Head() = pr_header;        
  code_mem->Symbol().Func().Inversepolarity(); 
 
  code_mem->Term() = data_mem;
  code_mem++;
  data_mem++;
  max_code_size--;
     
  var_counter.Reset();
     
  Unifier::Variable* var;
  IU_COMMAND* last_var = code_mem-1;
  IU_COMMAND* last_var_not_fo = code_mem-1;
  ulong closed = 0UL;
     
  check_iter:

   if (!max_code_size) return false; 
   // ^ this check is enough since at most one instruction is written 
   // per iteration cycle

   if (iter.Empty()) 
    {

     code_mem->Tag() = IU_COMMAND::END;
     code_mem++;

     data->SetBracket(data_mem); // needed only when the literal is propositional

     start->AllVars() = var_list;
     return true;
    };
   var = iter.CurrVar();
   if (var)
    {
     if (var_counter.Register(var))
      {
       code_mem->Tag() = IU_COMMAND::VAR;
       last_var_not_fo = code_mem;
      }
     else // first occurence of the variable
      {
       code_mem->Tag() = IU_COMMAND::VAR_FIRST_OCC;
       code_mem->NextVar() = var_list;
       var_list = code_mem;
      };
     code_mem->Symbol().MkVar(var);
        
     last_var = code_mem;
        
     data_mem->Head().MkVar(var);
     data_mem->SetBracket(data_mem + 1);
     closed = _bracketCounter.close();
     //closed = bracket_counter.Close();
    }
   else // functor
    {
        
     ulong arity = iter.CurrSubterm()->Head().Func().arity();
     if (arity)
      {
       code_mem->Tag() = IU_COMMAND::FUNC;

       _openBrackets.pushSafe(code_mem);

       _bracketCounter.openSafe(arity);
      }
     else // constant
      {
       code_mem->Tag() = IU_COMMAND::CONST;

       closed = _bracketCounter.close();
       data_mem->SetBracket(data_mem + 1);
      };
     code_mem->Symbol() = iter.CurrSubterm()->Head();
     code_mem->Term() = data_mem;
     data_mem->Head() = iter.CurrSubterm()->Head();
    };
       
   iter.SkipSym();
   code_mem++; 
   data_mem++;
   max_code_size--;

   while (closed)
    {
     IU_COMMAND* currentOpenBracket = _openBrackets.pop(); 
     currentOpenBracket->NextCommand() = code_mem;    
     unsigned length = code_mem - currentOpenBracket;
     currentOpenBracket->Term()->SetBracket(currentOpenBracket->Term() + length);

     if (last_var < currentOpenBracket) // the term is ground
      {
       currentOpenBracket->Tag() = IU_COMMAND::FUNC_GROUND;
      }
     else
      {
       if (last_var_not_fo < currentOpenBracket)
        {
         currentOpenBracket->Tag() = IU_COMMAND::FUNC_PLAIN;
        };
      };

     closed--;
    };
      
   goto check_iter;
 }; // void IU_TRANSLATOR::CompilePseudoLiteral(TERM header,PREFIX_SYM* word,IU_COMMAND*& code_mem,PREFIX_SYM*& data_mem)
}; // namespace VK
//================================================
#undef DEBUG_NAMESPACE 
//===========================================================
#endif
