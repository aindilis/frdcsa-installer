//
// File:         InstanceRetrievalForDemodulation.cpp
// Description:  Indexing for matching in backward demodulation.
// Created:      May 09, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//====================================================
#include "InstanceRetrievalForDemodulation.h"
//====================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_INSTANCE_RETRIEVAL_FOR_DEMODULATION
 #define DEBUG_NAMESPACE "InstanceRetrievalForDemodulation"
#endif
#include "debugMacros.h"
//====================================================


using namespace VK;
using namespace BK;
using namespace Gematogen;

#ifdef DEBUG_ALLOC_OBJ_TYPE

ClassDesc InstanceRetrievalForDemodulation::Index::NonmaxPathRelation::_classDesc("InstanceRetrievalForDemodulation::Index::NonmaxPathRelation",
										  InstanceRetrievalForDemodulation::Index::NonmaxPathRelation::minSize(),
										  InstanceRetrievalForDemodulation::Index::NonmaxPathRelation::maxSize());

ClassDesc InstanceRetrievalForDemodulation::Index::NonmaxPathRelation::_headerClassDesc("InstanceRetrievalForDemodulation::Index::NonmaxPathRelation(header)",
											InstanceRetrievalForDemodulation::Index::NonmaxPathRelation::headerMinSize(),
											InstanceRetrievalForDemodulation::Index::NonmaxPathRelation::headerMaxSize());



ClassDesc InstanceRetrievalForDemodulation::Index::NonmaxPathRelation::Node::_classDesc("InstanceRetrievalForDemodulation::Index::NonmaxPathRelation::Node",
											InstanceRetrievalForDemodulation::Index::NonmaxPathRelation::Node::minSize(),
											InstanceRetrievalForDemodulation::Index::NonmaxPathRelation::Node::maxSize());





ClassDesc InstanceRetrievalForDemodulation::Index::MaxPathRelation::_classDesc("InstanceRetrievalForDemodulation::Index::MaxPathRelation",
									       InstanceRetrievalForDemodulation::Index::MaxPathRelation::minSize(),
									       InstanceRetrievalForDemodulation::Index::MaxPathRelation::maxSize());

ClassDesc InstanceRetrievalForDemodulation::Index::MaxPathRelation::_headerClassDesc("InstanceRetrievalForDemodulation::Index::MaxPathRelation(header)",
										     InstanceRetrievalForDemodulation::Index::MaxPathRelation::headerMinSize(),
										     InstanceRetrievalForDemodulation::Index::MaxPathRelation::headerMaxSize());



ClassDesc InstanceRetrievalForDemodulation::Index::MaxPathRelation::Node::_classDesc("InstanceRetrievalForDemodulation::Index::MaxPathRelation::Node",
										     InstanceRetrievalForDemodulation::Index::MaxPathRelation::Node::minSize(),
										     InstanceRetrievalForDemodulation::Index::MaxPathRelation::Node::maxSize());





ClassDesc InstanceRetrievalForDemodulation::Index::PathIndexForInstanceRetrieval::Node::_funcClassDesc(
												       "InstanceRetrievalForDemodulation::Index::PathIndexForInstanceRetrieval::Node(func)",
												       InstanceRetrievalForDemodulation::Index::PathIndexForInstanceRetrieval::Node::funcMinSize(),
												       InstanceRetrievalForDemodulation::Index::PathIndexForInstanceRetrieval::Node::funcMaxSize());

ClassDesc InstanceRetrievalForDemodulation::Index::PathIndexForInstanceRetrieval::Node::_constClassDesc(
													"InstanceRetrievalForDemodulation::Index::PathIndexForInstanceRetrieval::Node(const)",
													InstanceRetrievalForDemodulation::Index::PathIndexForInstanceRetrieval::Node::constMinSize(),
													InstanceRetrievalForDemodulation::Index::PathIndexForInstanceRetrieval::Node::constMaxSize());


#endif

InstanceRetrievalForDemodulation::InstanceRetrievalForDemodulation() 
{
};
InstanceRetrievalForDemodulation::~InstanceRetrievalForDemodulation() 
{
};

void InstanceRetrievalForDemodulation::destroy()
{
  CALL("destroy()");
  _index.destroyThoroughly();
};

void InstanceRetrievalForDemodulation::integrate(const TERM* complexTerm)
{
  CALL("integrate(const TERM* complexTerm)");
  _index.integrate(TERM((TERM*)complexTerm));
}; 
void InstanceRetrievalForDemodulation::remove(const TERM* complexTerm)
{
  CALL("remove(const TERM* complexTerm)");
  _index.remove(TERM((TERM*)complexTerm));
}; 

bool InstanceRetrievalForDemodulation::newQuery(const TERM* query)
{
  CALL("newQuery(const TERM* query)");
  _noMoreMatches = !_index.compileQuery(TERM((TERM*)query));
  DOP(_debugQuery = query);
  return !_noMoreMatches;
}; 
bool InstanceRetrievalForDemodulation::nextMatch(const TERM*& match)
{
  CALL("nextMatch(const TERM*& match)");
  if (_noMoreMatches) return false;
  TERM matchId;
  bool result = _index.nextMatch(matchId);
  if (result)
    {
      ASSERT(matchId.IsReference());
      match = matchId.First();
      ASSERT(isValidMatch(match));
      return true;
    };
  _noMoreMatches = true;
  return false;
}; 


#ifdef DEBUG_NAMESPACE
bool InstanceRetrievalForDemodulation::isValidMatch(const TERM* term) const
{
  static Substitution::Instance queryInst;
  static TERM::Iterator matchIter; 
  queryInst.reset(_debugQuery,substitution());
  matchIter.Reset(term);
  do
    {
      if (queryInst.symbol() != matchIter.symbolRef()) return false;
      matchIter.Next();
    }
  while (queryInst.next());
  return true;
}; // bool InstanceRetrievalForDemodulation::isValidMatch(const TERM* term) const

#endif

//====================================================
