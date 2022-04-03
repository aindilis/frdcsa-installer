//
// File:         InstanceRetrievalForDemodulation.h
// Description:  Indexing for matching in backward demodulation.
// Created:      May 09, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//====================================================
#ifndef INSTANCE_RETRIEVAL_FOR_DEMODULATION_H
#define INSTANCE_RETRIEVAL_FOR_DEMODULATION_H
//====================================================
#ifndef NO_DEBUG_VIS
#include <iostream.h>
#endif   
#include "jargon.h"
#include "VampireKernelConst.h"
#include "VampireKernelDebugFlags.h"
#include "Term.h"
#include "GlobAlloc.h"
#include "InstanceRetrievalWithOptimisedPathIndexAndEarlySubstFactoring.h"
//====================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_INSTANCE_RETRIEVAL_FOR_DEMODULATION
 #define DEBUG_NAMESPACE "InstanceRetrievalForDemodulation"
#endif
#include "debugMacros.h"
//====================================================

namespace VK
{
  using namespace BK;
  using namespace Gematogen;
class InstanceRetrievalForDemodulation
{
 public:
  class Substitution
    {
    public:
      class Instance;
    public:
      const TERM& map(ulong var) const 
	{ 
	  return ((const Index*)this)->subst(var);
	};
      const TERM* nativeVar(ulong varNum) const
	{ 
	  return ((const Index*)this)->nativeVar(varNum);
	};

#ifndef NO_DEBUG_VIS
      ostream& output(ostream& str) const
	{
	  return ((const Index*)this)->outputSubst(str);
	};
#endif      

    private:
      Substitution() {};
      ~Substitution() {};
      friend class InstanceRetrievalForDemodulation;
    };
 public:
  InstanceRetrievalForDemodulation();
  ~InstanceRetrievalForDemodulation();
  void destroy();
  void integrate(const TERM* complexTerm);
  void remove(const TERM* complexTerm);
  bool newQuery(const TERM* query);
  bool nextMatch(const TERM*& match);
  const Substitution* substitution() const
    {
      CALL("substitution() const");
      ASSERT(!_noMoreMatches);
      return (const Substitution*)(&_index);
    };
 private:
  class Term
    {
    public:
      typedef TERM Symbol;
      typedef TERM Id;
      typedef TERM::Traversal Traversal;
      typedef TERM::Arguments Arguments;
    };
  typedef OptimisedInstanceRetrievalWithEarlySubstFactoringIndex<GlobAlloc,Term,VampireKernelConst::MaxTermDepth,VampireKernelConst::MaxTermSize,VampireKernelConst::MaxNumOfVariables> Index;

 private:
#ifdef DEBUG_NAMESPACE
  bool isValidMatch(const TERM* term) const;
#endif

 private:
  Index _index;
  bool _noMoreMatches;
  DOP(const TERM* _debugQuery);
}; // class InstanceRetrievalForDemodulation
}; // namespace VK

#ifndef NO_DEBUG_VIS
inline ostream& operator<<(ostream& str,const VK::InstanceRetrievalForDemodulation::Substitution* subst)
{
  return subst->output(str);
};
#endif 


//====================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_INSTANCE_RETRIEVAL_FOR_DEMODULATION
 #define DEBUG_NAMESPACE "InstanceRetrievalForDemodulation::Substitution::Instance"
#endif
#include "debugMacros.h"
//====================================================
namespace VK
{
  using namespace BK;
  using namespace Gematogen;

class InstanceRetrievalForDemodulation::Substitution::Instance
{
 public:
  Instance() : 
    _dummyTerm0((TERM::Arity)0,(TERM::Functor)0,TERM::PositivePolarity)  
    {
      _dummyTerm1[0].MakeComplex((TERM::Arity)1,(TERM::Functor)0,TERM::PositivePolarity);
    };
  ~Instance() {};
  void reset(const TERM& t,const InstanceRetrievalForDemodulation::Substitution* subst);
  void reset(const TERM* t,const InstanceRetrievalForDemodulation::Substitution* subst);
  const TERM& symbol() const;
  bool next();
 private:
  const InstanceRetrievalForDemodulation::Substitution* _subst;
  TERM::Iterator _topLevel;
  TERM::Iterator _substIter;
  bool _onTop;
  TERM _dummyTerm0;
  TERM _dummyTerm1[2];
}; // class InstanceRetrievalForDemodulation::Substitution::Instance

}; // namespace VK


//====================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_INSTANCE_RETRIEVAL_FOR_DEMODULATION
 #define DEBUG_NAMESPACE "InstanceRetrievalForDemodulation::Substitution::Instance"
#endif
#include "debugMacros.h"
//====================================================
 
namespace VK
{
  using namespace BK;
  using namespace Gematogen;


inline
void 
InstanceRetrievalForDemodulation::Substitution::Instance::reset(const TERM& t,
								const InstanceRetrievalForDemodulation::Substitution* subst)
{
  CALL("reset(const TERM& t,const InstanceRetrievalForDemodulation::Substitution* subst)");
  if (t.isVariable())
    {
      _subst = subst;
      if (subst->map(t.var()).isVariable())
	{
	  _dummyTerm1[1] = subst->map(t.var());
	  _substIter.Reset(_dummyTerm1);
	  ALWAYS(_substIter.Next());
	}
      else
	{
	  ASSERT(subst->map(t.var()).IsReference());
	  _substIter.Reset(subst->map(t.var()).First());
	};
      _topLevel.Reset(&_dummyTerm0);
      NEVER(_topLevel.Next());
      _onTop = false;
    }
  else
    {
      ASSERT(t.IsReference());
      reset(t.First(),subst);
    };
}; // void InstanceRetrievalForDemodulation::Substitution::Instance::reset(const TERM& t,const InstanceRetrievalForDemodulation::Substitution* subst)

inline
void 
InstanceRetrievalForDemodulation::Substitution::Instance::reset(const TERM* t,
						  const InstanceRetrievalForDemodulation::Substitution* subst)
{
  CALL("reset(const TERM* t,const InstanceRetrievalForDemodulation::Substitution* subst)");
  ASSERT(t->isComplex());
  _subst = subst;
  _topLevel.Reset(t);
  _onTop = true;
}; // void InstanceRetrievalForDemodulation::Substitution::Instance::reset(const TERM* t,const InstanceRetrievalForDemodulation::Substitution* subst)


inline const TERM& InstanceRetrievalForDemodulation::Substitution::Instance::symbol() const
{
  CALL("symbol() const");
  if (_onTop) return _topLevel.symbolRef();
  return _substIter.symbolRef();
}; // const TERM& InstanceRetrievalForDemodulation::Substitution::Instance::symbol() const

inline bool InstanceRetrievalForDemodulation::Substitution::Instance::next()
{
  CALL("next()");
  if (_onTop)
    {
    next_on_top:
      if (!_topLevel.Next()) 
	{
	  return false;
	};
      if (_topLevel.CurrentSymIsVar())
	{
	  _onTop = false;
	  if (_subst->map(_topLevel.symbolRef().var()).isVariable())
	    {
	      _dummyTerm1[1] = _subst->map(_topLevel.symbolRef().var());
	      _substIter.Reset(_dummyTerm1);
	      ALWAYS(_substIter.Next());
	    }
	  else
	    {
	      ASSERT(_subst->map(_topLevel.symbolRef().var()).IsReference());
	      _substIter.Reset(_subst->map(_topLevel.symbolRef().var()).First());
	    };
	};
      return true;
    }
  else // inside a substitution term
    {
      if (_substIter.Next())
	{
	  return true;
	};
      _onTop = true;
      goto next_on_top;
    };
}; // bool InstanceRetrievalForDemodulation::Substitution::Instance::next()

}; // namespace VK








//====================================================
#endif
