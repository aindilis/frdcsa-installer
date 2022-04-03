#ifndef PARA_MACHINE_H
//=================================================================
#define PARA_MACHINE_H
#ifndef NO_DEBUG_VIS
 #include <iostream.h>
#endif
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "iu_command.h"
#include "iu_machine.h"
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_FORWARD_SUPERPOSITION
 #define DEBUG_NAMESPACE "PARA_MACH"
#endif
#include "debugMacros.h"
//=================================================
namespace VK
{
class PREFIX_SYM;
class BackParaCandidates;
class PARA_MACH : private IU_MACH
 {
  public: 
   PARA_MACH() {};
   ~PARA_MACH() { CALL("destructor ~PARA_MACH()"); };
   void SetIndex(ulong ind) { IU_MACH::SetIndex(ind); };
   ulong GetIndex() const { return IU_MACH::GetIndex(); };
   void CompileComplexTerm(PREFIX_SYM* t)
    {
     CALL("CompileComplexTerm(PREFIX_SYM* t)");      
     IU_MACH::CompileLiteral(t);
    };
   void CompilePseudoLiteral(TERM header,PREFIX_SYM* t)
    {
     IU_MACH::CompilePseudoLiteral(header,t);
    }; 
   bool FindFirstLeaf(OR_INDEX_TREE_NODE* tree) 
    {
     return IU_MACH::FindFirstLeaf(tree); 
    }; 
   bool FindNextLeaf() { return IU_MACH::FindNextLeaf(); };
   void ResetCandidatesFromLeaf() { IU_MACH::ResetCandidatesFromLeaf(); };
   RES_PAIR_LIST* NextCandidateFromTheLeaf() { return IU_MACH::NextCandidate(); };
   

   BackParaCandidates* BPCandidatesFromLeaf() 
   {
    return IU_MACH::CurrentLeaf()->BPCandidates();
   };   

   bool VariablesFromIndexAffected() { return IU_MACH::VariablesFromIndexAffected(); };
  public: // for debugging
   #ifndef NO_DEBUG_VIS
    ostream& output(ostream& str) const
    {  
     return IU_MACH::outputCode(str);
    };
   #endif
 }; // class PARA_MACH : private IU_MACH
}; // namespace VK
//================================================
#undef DEBUG_NAMESPACE 
//=================================================================
#endif




























