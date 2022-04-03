//
// File:         InferenceTree.cpp
// Description:  Collecting inference trees.
// Created:      Apr 13, 2001, 17:30
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//============================================================================
#include "InferenceTree.h"
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_INFERENCE_TREE
 #define DEBUG_NAMESPACE "InferenceTree"
#endif
#include "debugMacros.h"
//============================================================================
using namespace VK;

using namespace BK;

#ifdef DEBUG_ALLOC_OBJ_TYPE
 #include "ClassDesc.h"
 ClassDesc InferenceTree::ClBag::LeftRightIter::NodeList::_classDesc("InferenceTree::ClBag::LeftRightIter::NodeList",
                                                                     InferenceTree::ClBag::LeftRightIter::NodeList::minSize(),
                                                                     InferenceTree::ClBag::LeftRightIter::NodeList::maxSize());


 ClassDesc InferenceTree::ClBag::RightLeftIter::NodeList::_classDesc("InferenceTree::ClBag::RightLeftIter::NodeList",
                                                                     InferenceTree::ClBag::RightLeftIter::NodeList::minSize(),
                                                                     InferenceTree::ClBag::RightLeftIter::NodeList::maxSize());

 ClassDesc InferenceTree::ClBag::Node::_classDesc("InferenceTree::ClBag::Node",
                                                  InferenceTree::ClBag::Node::minSize(),
                                                  InferenceTree::ClBag::Node::maxSize());
#endif


void InferenceTree::collect(const Clause* root)
{
  CALL("collect(const Clause* root)");
  collected.destroyThoroughly();
  collectInf(root);
};

void InferenceTree::collectInf(const Clause* root)
{
  CALL("collectInf(const Clause* root)");
  unsigned long oldScore;
  collected.insert(ClWrapper(root),oldScore);
  if (!oldScore)
    {

      if ((!root->isInputClause()) && 
	  (!root->isBuiltInTheoryFact()))
	{
	  for (const ClauseBackground::ClauseList* anc = root->background().ancestors(); 
	       anc; 
	       anc = anc->tl())
	    collectInf(anc->hd());
	};
    };
};




//============================================================================
