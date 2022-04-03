#ifndef IU_COMMAND_H
//===========================================================
#define IU_COMMAND_H
#ifndef NO_DEBUG_VIS
 #include <iostream.h>
#endif
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "prefix.h"
//#include "variables.h"
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_IU_COMMAND
 #define DEBUG_NAMESPACE "IU_COMMAND"
#endif
#include "debugMacros.h"
//=================================================
namespace VK
{
class IU_COMMAND
 {
  public: 
   enum TAG
    {
     START,
     VAR,
     VAR_FIRST_OCC,
     FUNC,
     FUNC_PLAIN,
     FUNC_GROUND,
     CONST,
     END
    };
  private: // structure of a command
   TAG tag;
   PREF_SYM sym;
   PREFIX_SYM* term;
   IU_COMMAND* nextCommand;
  public:
   IU_COMMAND() {};
   ~IU_COMMAND() {};
   TAG& Tag() { return tag; };
   const TAG& Tag() const { return tag; };
   const PREF_SYM& Symbol() const { return sym; };
   PREF_SYM& Symbol() { return sym; };
   PREFIX_SYM*& Term() { return term; };
   const PREFIX_SYM* const & Term() const { return term; };
   IU_COMMAND*& NextCommand() { return nextCommand; };
   const IU_COMMAND* const & NextCommand() const { return nextCommand; };
   IU_COMMAND*& AllVars() 
    {
     CALL("AllVars()");
     ASSERT(Tag() == START);
     return nextCommand; 
    };
   const IU_COMMAND* const & AllVars() const 
    {
     CALL("AllVars()");
     ASSERT(Tag() == START);
     return nextCommand; 
    };
   IU_COMMAND*& NextVar() 
    {
     CALL("NextVar()");
     ASSERT(Tag() == VAR_FIRST_OCC);
     return nextCommand; 
    };
   const IU_COMMAND* const & NextVar() const
    {
     CALL("NextVar()");
     ASSERT(Tag() == VAR_FIRST_OCC);
     return nextCommand; 
    }; 
   void Inversepolarity() // applicable to a START command only
    {
     CALL("Inversepolarity()");
     ASSERT(Tag() == START);
     sym.Inversepolarity();
    };
  public: // for debugging 
   #ifndef NO_DEBUG_VIS
    ostream& output(ostream& str,const IU_COMMAND* start) const;
    ostream& outputCode(ostream& str) const; 
   #endif
 }; // class IU_COMMAND

}; // namespace VK

//================================================
#undef DEBUG_NAMESPACE
//===========================================================
#endif
