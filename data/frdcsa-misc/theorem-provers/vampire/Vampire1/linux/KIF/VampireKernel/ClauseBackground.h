//
// File:         ClauseBackground.h
// Description:  Ancestors of a clause.
// Created:      Feb 12, 2000, 20:10
// Revised:      Feb 1, 2001, AR, 
//               representation changed from arrays to lists
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk 
//==================================================================
#ifndef  ClauseBackground_H
//==================================================================
#define ClauseBackground_H
#include <iostream.h>
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#ifdef DEBUG_ALLOC_OBJ_TYPE
#include "ClassDesc.h"
#endif
#ifndef NO_DEBUG
#include "ObjDesc.h"
#endif
#include "GlobAlloc.h"
#include "ExpandingStack.h"
#include "BitWord.h"
#include "GList.h"
#include "DestructionMode.h"
namespace VK
{
  class Clause;
}; // namespace VK
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_CLAUSE_BACKGROUND
#define DEBUG_NAMESPACE "ClauseBackground"
#endif
#include "debugMacros.h"
//===================================================================
namespace VK
{
  using namespace BK;
  class ClauseBackground
    {
    public:
      enum Rule 
      { 
	Input = 0, 
	Prepro = 1, 
	BinRes = 2, 
	ForwSup = 3,
	BackSup = 4,
	EqRes = 5,
	EqFact = 6, 
	ForwDemod = 7,
	BackDemod = 8,
	EqResSimp = 9,
	ForwSubsRes = 10,
	BackSubsRes = 11,     
	Split = 12,
	Reanimation = 13,
	Name = 14,
	NegSel = 15,
	SimplificationByBuiltInTheories = 16,
	Support = 28,
	BuiltInTheoryFact = 29,
	Subgoal = 30,
	VIP = 31
      };
      typedef BitWord Rules; 
      typedef GList<GlobAlloc,Clause*,ClauseBackground> ClauseList;
    public:
      ClauseBackground() : 
	_ancestors((ClauseList*)0),
	_inferenceDepth(0L)
	{
	};
      ~ClauseBackground()
	{
	  CALL("desctructor ~ClauseBackground()");
	  if (DestructionMode::isThorough()) 
	    {
	      if (!isInput()) ClauseList::destroyList(_ancestors);
	    };
	}; 

      const Rules& properties() const { CALL("properties() const");  return _rules; };
      void SetRules(const Rules& r) { CALL("SetRules(Rules r");  _rules = r; };
      void MakeVIP() { CALL("MakeVIP()");  _rules.set(VIP); };
      bool IsVIP() const { CALL("MakeVIP()");  return _rules.bit(VIP); };
      bool isInput() const { return _rules.bit(Input); };
      void MarkAsName() { CALL("MarkAsName()");  _rules.set(Name); };
      bool isSubgoal() const { return _rules.bit(Subgoal); };
      void markAsSubgoal() { _rules.set(Subgoal); };
      bool isBuiltInTheoryFact() const { return _rules.bit(BuiltInTheoryFact); };
      void markAsBuiltInTheoryFact() { _rules.set(BuiltInTheoryFact); };
      bool isSupport() const { return _rules.bit(Support); };
      void markAsSupport() { _rules.set(Support); };

      bool NegativeSelection() const
	{ 
	  CALL("NegativeSelection() const"); 
	   
	  return _rules.bit(NegSel); 
	};
      void SetNegativeSelection(bool fl) 
	{
	  CALL("SetNegativeSelection(bool fl)");
	  
	  if (fl) { _rules.set(NegSel); } else _rules.clear(NegSel);
	}; 
      ClauseList* ancestors() 
	{
	  CALL("ancestors()");
	  ASSERT(!isInput());
	  return _ancestors; 
	};
      const ClauseList* ancestors() const 
	{
	  CALL("ancestors() const");
	  ASSERT(!isInput());
	  return _ancestors; 
	};
      void* inputClauseOrigin() const
	{
	  CALL("inputClauseOrigin() const");
	  ASSERT(isInput());
	  return _inputClauseOrigin; 
	};
      
      void setInputClauseOrigin(void* orig)
	{
	  CALL("setInputClauseOrigin()");
	  ASSERT(isInput());
	  _inputClauseOrigin = orig; 
	};
	
      long inferenceDepth() const { return _inferenceDepth; };
      void setInferenceDepth(long d) { _inferenceDepth = d; };

      void copyTo(ClauseBackground& copy) const 
	{
	  CALL("ClauseBackground& copy");
	  if (isInput())
	    {
	      copy = *this;
	    }
	  else
	    {
	      ClauseList* orgClauses = _ancestors;
	      ClauseList* targClauses = (ClauseList*)0;
	      ClauseList** targClausesAddr = &targClauses;      
	      while (orgClauses)
		{
		  *targClausesAddr = new ClauseList(orgClauses->hd(),(ClauseList*)0);
		  targClausesAddr = &((*targClausesAddr)->tl());
		  orgClauses = orgClauses->tl();
		};
	      copy.init(_rules,targClauses,_inferenceDepth);
	    };
	};
   
      static ostream& outputRules(ostream& str,const BitWord& _rules) 
	{
	  if (_rules.bit(ClauseBackground::Input)) str << "in ";
	  if (_rules.bit(ClauseBackground::Prepro)) str << "pp ";
	  if (_rules.bit(ClauseBackground::Subgoal)) str << "sg ";
	  if (_rules.bit(ClauseBackground::BuiltInTheoryFact)) str << "bitf ";	
	  if (_rules.bit(ClauseBackground::Support)) str << "sprt ";  
	  if (_rules.bit(ClauseBackground::BinRes)) str << "br ";
	  if (_rules.bit(ClauseBackground::ForwSup)) str << "fs ";
	  if (_rules.bit(ClauseBackground::BackSup)) str << "bs ";
	  if (_rules.bit(ClauseBackground::EqRes)) str << "er ";
	  if (_rules.bit(ClauseBackground::EqFact)) str << "ef ";
	  if (_rules.bit(ClauseBackground::ForwDemod)) str << "fd ";
	  if (_rules.bit(ClauseBackground::BackDemod)) str << "bd ";
	  if (_rules.bit(ClauseBackground::EqResSimp)) str << "ers ";
	  if (_rules.bit(ClauseBackground::ForwSubsRes)) str << "fsr ";
	  if (_rules.bit(ClauseBackground::BackSubsRes)) str << "bsr ";
	  if (_rules.bit(ClauseBackground::Split)) str << "sp ";
	  if (_rules.bit(ClauseBackground::Reanimation)) str << "rea ";
	  if (_rules.bit(ClauseBackground::Name)) str << "nm ";
	  if (_rules.bit(ClauseBackground::NegSel)) str << "ns ";
	  if (_rules.bit(ClauseBackground::SimplificationByBuiltInTheories)) str << "sbit ";
	  return str;
	};    
   
    private:     
      void init(const Rules& r,ClauseList* clauses,long inferenceDepth)
	{
	  _rules = r;
	  _ancestors = clauses;
	  _inferenceDepth = inferenceDepth;
	};
      void* operator new(size_t)
	{
	  CALL("operator new(size_t)");
	  ICP("ICP0");
	  return (void*)0;
	};
      void operator delete(void* obj)
	{
	  CALL("operator delete(void* obj)");
	  ICP("ICP0");
	};

    private:
      BitWord _rules;
      union
      {
	ClauseList* _ancestors;
	void* _inputClauseOrigin;
      };
      long _inferenceDepth;
#ifdef DEBUG_ALLOC_OBJ_TYPE
      static ClassDesc _classDesc;
#endif
      friend class OpenClauseBackground;
    }; // class ClauseBackground
}; // namespace VK

//========================================
#undef DEBUG_NAMESPACE
//========================================
#ifdef DEBUG_CLAUSE_BACKGROUND
#define DEBUG_NAMESPACE "OpenClauseBackground"
#else 
#undef DEBUG_NAMESPACE
#endif
#include "debugMacros.h"
//========================================
namespace VK
{
  using namespace BK;
  class OpenClauseBackground
    {
    public:
      OpenClauseBackground() 
	: _rules(),
	_ancestors(DOP("OpenClauseBackground::_ancestors"))
	{
	  DOP(_ancestors.freeze());
	};
      ~OpenClauseBackground() {};
      void Reset() 
	{ 
	  _rules.clear();
	  _ancestors.reset(); 
	  _inferenceDepth = 0L;
	};
      void MakeVIP() { _rules.set(ClauseBackground::VIP); };
      void UsedRule(ClauseBackground::Rule r) { _rules.set(r); };
      void markAsSubgoal() { _rules.set(ClauseBackground::Subgoal); };
      bool isSupport() const { return _rules.bit(ClauseBackground::Support); };
      void markAsSupport() { _rules.set(ClauseBackground::Support); };
      void CancelRule(ClauseBackground::Rule r) { _rules.clear(r); };
      void UsedRules(const ClauseBackground::Rules& rs) { _rules |= rs; };
      void pushAncestor(Clause* cl,long depth) 
	{
	  CALL("pushAncestor(Clause* cl)");
	  DOP(_ancestors.unfreeze());
	  _ancestors.pushSafe(cl);
	  DOP(_ancestors.freeze()); 
	  if (depth + 1 > _inferenceDepth) _inferenceDepth = depth + 1;
	};
      void InverseOrder() 
	{
	  Clause** b = _ancestors.begin();
	  Clause** e = _ancestors.last();    
	  while (b < e) 
	    {
	      Clause* tmp = *b;
	      *b = *e;
	      *e = tmp;
	      b++;
	      e--;
	    };
	};
      void writeNewBackgroundTo(ClauseBackground& b) 
	{
	  CALL("writeNewBackgroundTo(ClauseBackground& b)");
	  ClauseBackground::ClauseList* clauses = (ClauseBackground::ClauseList*)0;

	  for (Clause** anc = _ancestors.begin(); anc < _ancestors.end(); anc++)
	    {
	      clauses = new ClauseBackground::ClauseList(*anc,clauses);
	      
	    };
     
	  b.init(_rules,clauses,_inferenceDepth);
	};  

      const BitWord& Rules() const { return _rules; };
      ulong numberOfAncestors() const { return _ancestors.size(); }; 

      Clause* const * begin() const { return _ancestors.begin(); };
      Clause* const * end() const { return _ancestors.end(); };
      Clause* const * last() const { return _ancestors.last(); };

      bool singletone() const { return _ancestors.size() == 1; }; 

      long inferenceDepth() const { return _inferenceDepth; };

    private:
      ClauseBackground::Rules _rules;
      ExpandingStack<GlobAlloc,Clause*,128UL,OpenClauseBackground> _ancestors;
      long _inferenceDepth;
    }; // OpenClauseBackground
}; // namespace VK
//=================================================================
#undef DEBUG_NAMESPACE   
//==========================================================================
#endif
