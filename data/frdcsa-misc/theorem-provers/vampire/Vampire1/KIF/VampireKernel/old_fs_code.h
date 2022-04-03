//
// File:         old_fs_code.h
// Description:  Old-style fs code for flatterm query representation.
// Created:      Dec 22, 1999, 15:10
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
// Revised:      Feb 13, 2000, 21:10
//==================================================================
#ifndef OLD_FS_CODE_H
//==================================================================
#define OLD_FS_CODE_H
#ifndef NO_DEBUG_VIS
#include <iostream.h>
#endif
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "VampireKernelConst.h"
#include "MultisetOfVariables.h"
#include "fs_command.h"
#include "Stack.h"
#include "Queue.h"
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_OLD_FS_CODE
#define DEBUG_NAMESPACE "OLD_FS_CODE"
#endif
#include "debugMacros.h"
//=================================================
namespace VK
{
class TERM;
class Clause;
class Flatterm;
class TmpLitList;
class TMP_LITERAL;
class OLD_FS_CODE
{
 public:
  OLD_FS_CODE(const Flatterm** sbst) 
    : tree((FS_COMMAND*)0),
    subst(sbst),
    integrator(subst), 
    remover(subst)  
    {
    };
  ~OLD_FS_CODE() { CALL("destructor ~OLD_FS_CODE()"); };
#ifndef NO_DEBUG
  bool clean() const { return !tree; };
#endif

  void integrate(Clause* cl);
  void integrate(TmpLitList& component,void* name);
  bool remove(Clause* cl);
  void destroy();
  void destroy(void (*destroyIndexedObj)(void*)); 
  FS_COMMAND* theTree() const { return tree; };

#ifndef NO_DEBUG_VIS
  ostream& outputCode(ostream& str) const;
#endif
  
 private:
  class INTEGRATOR
    {
    public:
      INTEGRATOR(const Flatterm** subst)
	: fs_subst(subst),
	registeredVars(),
	compares("compares")
	{
	};  
      ~INTEGRATOR() {};
      void reset(FS_COMMAND** n)
	{
	  writeMode = !(*n);  
	  node_addr = n;
	  registeredVars.Reset();
	  nextTechnicalVar = fs_subst;
	};
      void beginLit() { compares.reset(); };
      void litHeader(const TERM& header);
      void propLit(const TERM& header);
      void ordEq(const TERM& header);
      void unordEq(const TERM& header);
      void secondEqArg();  
      void endOfLit();    
      void func(const TERM& sym);
      void var(unsigned long v);    
      void endOfClause(Clause* cl);
      void endOfComponent(void* name,unsigned long numOfLits);
    private: 
      void moveDown();
      void moveRight();
      bool findCompare(const Flatterm** v1,const Flatterm** v2);
    private:
      const Flatterm** fs_subst;
      MultisetOfVariables<VampireKernelConst::MaxNumOfVariables> registeredVars;
      bool writeMode;
      FS_COMMAND** node_addr;
      const Flatterm** varFirstOcc[VampireKernelConst::MaxClauseSize];
      const Flatterm** nextTechnicalVar;
      Stack<const Flatterm**,2*VampireKernelConst::MaxClauseSize> compares;
    }; // class INTEGRATOR

  class REMOVER
    { 
    public:
      REMOVER(const Flatterm** subst)
	: fs_subst(subst),
	registeredVars(),
	compares()
	{
	};
      ~REMOVER() {};
      bool reset(FS_COMMAND** n)
	{
	  node_addr = n;
	  linBranch = n;
	  registeredVars.Reset();
	  nextTechnicalVar = fs_subst;
	  compares.reset();
	  return (bool)(*n);
	};
      bool beginLit() 
	{
	  CALL("REMOVER::beginLit()");
	  ASSERT(compares.empty());
	  return true; 
	};
      bool litHeader(const TERM& header);
      bool propLit(const TERM& header);
      bool ordEq(const TERM& header);
      bool unordEq(const TERM& header);
      bool secondEqArg() 
	{ 
	  CALL("REMOVER::secondEqArg()");
	  ASSERT((*node_addr)->Tag() == FS_COMMAND::SECOND_EQ_ARG);
	  ASSERT(!((*node_addr)->Fork()));
	  moveDown(); 
	  return true; 
	};  
      bool endOfLit();    
      bool func(const TERM& sym);
      bool var(unsigned long v);    
      bool endOfClause(Clause* cl);
    private:
      void moveDown() 
	{ 
	  if ((*node_addr)->Fork()) linBranch = node_addr; 
	  node_addr = (*node_addr)->NextAddr(); 
	};
      void moveRight() 
	{
	  CALL("REMOVER::moveRight()");
	  ASSERT((*node_addr)->Fork());
	  node_addr = (*node_addr)->ForkAddr(); 
	  linBranch = node_addr; 
	};
      bool linearBranch(FS_COMMAND* t) 
	{
	  if (!t) return true;
	  if (t->Fork()) return false;
	  return linearBranch(t->Next());
	};
    private:
      const Flatterm** fs_subst;
      MultisetOfVariables<VampireKernelConst::MaxNumOfVariables> registeredVars; 
      FS_COMMAND** node_addr;
      FS_COMMAND** linBranch;
      const Flatterm** nextTechnicalVar;
      const Flatterm** firstOccTechVar[VampireKernelConst::MaxClauseSize]; 
      Queue<const Flatterm**,2*VampireKernelConst::MaxClauseSize> compares; 
    }; // class REMOVER

 private:
  void integrateLit(const TERM* lit);
  void integrateLit(TMP_LITERAL* lit); 
  bool removeLit(const TERM* lit);
 private:
  FS_COMMAND* tree;
  const Flatterm** subst;
  INTEGRATOR integrator;
  REMOVER remover;
}; // class OLD_FS_CODE

}; // namespace VK
//================================================
#undef DEBUG_NAMESPACE
//=================================================================
#endif
