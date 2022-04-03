//
// File:         BSIndex.cpp
// Description:  Path index for backward subsumption.
// Created:      Feb 26, 2000.
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
// Revised:      Dec 23, 2001. 
//               1) The array BSIndex::_propLitInd replaced by
//               extendable array BSIndex::_propLitOccurences
//               2) Freed from HellConst::MaxNumOfDifferentHeaders.
//============================================================================
#include "BSIndex.h"
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_BS_INDEX
 #define DEBUG_NAMESPACE "BSIndex"
#endif
#include "debugMacros.h"
//============================================================================

using namespace VK;
using namespace BK;

#ifdef DEBUG_ALLOC_OBJ_TYPE
 ClassDesc BSIndex::LList::_classDesc("BSIndex::LList",
                                      BSIndex::LList::minSize(),
                                      BSIndex::LList::maxSize());
 ClassDesc BSIndex::LTList::_classDesc("BSIndex::LTList",
                                       BSIndex::LTList::minSize(),
                                       BSIndex::LTList::maxSize());
 ClassDesc BSIndex::LAList::_classDesc("BSIndex::LAList",
                                       BSIndex::LAList::minSize(),
                                       BSIndex::LAList::maxSize());
 ClassDesc BSIndex::LATList::_classDesc("BSIndex::LATList",
                                        BSIndex::LATList::minSize(),
                                        BSIndex::LATList::maxSize());

 ClassDesc BSIndex::OrdLitPITree::FuncNode::_funcClassDesc("BSIndex::OrdLitPITree::FuncNode(nonconst)",
                                                           BSIndex::OrdLitPITree::FuncNode::funcMinSize(),
                                                           BSIndex::OrdLitPITree::FuncNode::funcMaxSize());
 ClassDesc BSIndex::OrdLitPITree::FuncNode::_constClassDesc("BSIndex::OrdLitPITree::FuncNode(const)",
                                                          BSIndex::OrdLitPITree::FuncNode::constMinSize(),
                                                          BSIndex::OrdLitPITree::FuncNode::constMaxSize());

 ClassDesc BSIndex::SymLitPITree::FuncNode::_funcClassDesc("BSIndex::SymLitPITree::FuncNode(nonconst)",
                                                          BSIndex::SymLitPITree::FuncNode::funcMinSize(),
                                                          BSIndex::SymLitPITree::FuncNode::funcMaxSize());
 ClassDesc BSIndex::SymLitPITree::FuncNode::_constClassDesc("BSIndex::SymLitPITree::FuncNode(const)",
                                                          BSIndex::SymLitPITree::FuncNode::constMinSize(),
                                                          BSIndex::SymLitPITree::FuncNode::constMaxSize());


 ClassDesc BSIndex::OrdLitPITree::OccNode::_classDesc("BSIndex::OrdLitPITree::OccNode",
                                                      BSIndex::OrdLitPITree::OccNode::minSize(),
                                                      BSIndex::OrdLitPITree::OccNode::maxSize());
 ClassDesc BSIndex::SymLitPITree::OccNode::_classDesc("BSIndex::SymLitPITree::OccNode",
                                                      BSIndex::SymLitPITree::OccNode::minSize(),
                                                      BSIndex::SymLitPITree::OccNode::maxSize());
#endif

#ifdef DEBUG_ALLOC_OBJ_TYPE
 #ifndef BS_USE_ORD_LISTS
  ClassDesc BSIndex::CSkipList::_classDesc("BSIndex::CSkipList",
                                            BSIndex::CSkipList::minSize(),
                                            BSIndex::CSkipList::maxSize());
  ClassDesc BSIndex::CSkipList::Node::_classDesc("BSIndex::CSkipList::Node",
                                                 BSIndex::CSkipList::Node::minSize(),
                                                 BSIndex::CSkipList::Node::maxSize());
  ClassDesc BSIndex::CLSkipList::_classDesc("BSIndex::CLSkipList",
                                             BSIndex::CLSkipList::minSize(),
                                             BSIndex::CLSkipList::maxSize());
  ClassDesc BSIndex::CLSkipList::Node::_classDesc("BSIndex::CLSkipList::Node",
                                                  BSIndex::CLSkipList::Node::minSize(),
                                                  BSIndex::CLSkipList::Node::maxSize());
  ClassDesc BSIndex::CLTSkipList::_classDesc("BSIndex::CLTSkipList",
                                              BSIndex::CLTSkipList::minSize(),
                                              BSIndex::CLTSkipList::maxSize());
  ClassDesc BSIndex::CLTSkipList::Node::_classDesc("BSIndex::CLTSkipList::Node",
                                                   BSIndex::CLTSkipList::Node::minSize(),
                                                   BSIndex::CLTSkipList::Node::maxSize());
  ClassDesc BSIndex::CLASkipList::_classDesc("BSIndex::CLASkipList",
                                              BSIndex::CLASkipList::minSize(),
                                              BSIndex::CLASkipList::maxSize());
  ClassDesc BSIndex::CLASkipList::Node::_classDesc("BSIndex::CLASkipList::Node",
                                                   BSIndex::CLASkipList::Node::minSize(),
                                                   BSIndex::CLASkipList::Node::maxSize());
  ClassDesc BSIndex::CLATSkipList::_classDesc("BSIndex::CLATSkipList",
                                               BSIndex::CLATSkipList::minSize(),
                                               BSIndex::CLATSkipList::maxSize());
  ClassDesc BSIndex::CLATSkipList::Node::_classDesc("BSIndex::CLATSkipList::Node",
                                                    BSIndex::CLATSkipList::Node::minSize(),
                                                    BSIndex::CLATSkipList::Node::maxSize());
 #endif
#endif

void BSIndex::destroy() 
{
 CALL("destroy()");
 for (ulong i = 0; i < _propLitOccurences.size(); i++) 
   if (_propLitOccurences[i])
     delete (_propLitOccurences[i]);
 _propLitOccurences.destroyThoroughly();
#ifndef NO_DEBUG
 _propLitOccurences.init((CSkipList*)0,"BSIndex::_propLitOccurences");
#endif

 bool constant;
 OrdLitPITree::Destruction _ordLitIndDestruction;
 if (_ordLitIndDestruction.reset(_ordLitInd[0],constant))
  { 
   do 
    {
     if (constant) { recycle(_ordLitIndDestruction.constIndObj()); }
     else recycle(_ordLitIndDestruction.indexedObj());
    }
   while (_ordLitIndDestruction.next(constant));
   _ordLitIndDestruction.recycle(); 
  };
  
 SymLitPITree::Destruction _symLitIndDestruction;
 if (_symLitIndDestruction.reset(_symLitInd[0],constant))
  { 
   do 
    {
     if (constant) 
      { 
       recycle(_symLitIndDestruction.constIndObj()); 
       ASSERT(!_symLitIndDestruction.constIndObj());
      }
     else 
      {
       recycle(_symLitIndDestruction.indexedObj());
       ASSERT(!_symLitIndDestruction.indexedObj());
      };
    }
   while (_symLitIndDestruction.next(constant));
   _symLitIndDestruction.recycle(); 
  };
}; //  void BSIndex::destroy()  


//============================================================================
