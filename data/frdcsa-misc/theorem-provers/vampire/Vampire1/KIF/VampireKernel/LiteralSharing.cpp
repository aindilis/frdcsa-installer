//
// File:         LiteralSharing.cpp
// Description:  Implements literal sharing.
// Created:      Dec 13, 2001 
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
// Note:         Was LITERAL_INDEX in clause_index.{h,cpp}.
//============================================================================
#include "LiteralSharing.h"
#include "Flatterm.h"
#include "dtree.h"
#include "sharing.h"
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_LITERAL_SHARING
 #define DEBUG_NAMESPACE "LiteralSharing"
#endif
#include "debugMacros.h"
//============================================================================

using namespace VK;

using namespace BK;

DTREE_NODE* LiteralSharing::Integrate(TERM* lit) 
  // returns the leaf corresponding to the literal
{
  CALL("Integrate(TERM* lit)");
  TERM header = *lit;
  DTREE_NODE** tree = Tree(header);
  
  if (header.arity())
    {
      TERM::Iterator iter;
      iter.Reset(lit);
      DTREE::SURFER surf;
      surf.Reset(tree,1);
   
      if (*tree)
	{
	read_mode_next_sym:
	  if (iter.Next())
	    {
	      if (surf.Read(iter.Symbol()))
		{
		  goto read_mode_next_sym;
		}
	      else
		{
		  surf.Write(iter.Symbol());
		  goto write_mode_next_sym;
		}; 
	    }
	  else // the literal is in the index already
	    {
	      return surf.Leaf();
	    };
	}
      else
	{
	write_mode_next_sym:
	  if (iter.Next())
	    {
	      surf.Write(iter.Symbol());
	      goto write_mode_next_sym;
	    }
	  else // end of the literal, leaf must be created
	    {
	      surf.CreateLeaf(_sharingIndex->CopyLiteral(lit));
         
	      *(surf.Leaf()->Term()->InfoField()) = (void*)0;
         
	      return surf.Leaf();
	    };
	}; 
    }
  else // propositional
    {
      if (!(*tree)) 
	{
	  *tree = new DTREE_NODE(ShareProp(header));
	};
    
      ASSERT((*tree)->IsLeaf());
    
      return *tree;
    };
}; // DTREE_NODE* LiteralSharing::Integrate(TERM* lit)

DTREE_NODE* LiteralSharing::Integrate(const Flatterm* lit) 
  // returns the leaf corresponding to the literal
{
  CALL("Integrate(const Flatterm* lit)");
  TERM header = lit->Symbol();
  DTREE_NODE** tree = Tree(header);
  
  if (header.arity())
    {
      Flatterm::Iterator iter;
      iter.Reset(lit);
      DTREE::SURFER surf;
      surf.Reset(tree,1);
      if (*tree)
	{
	read_mode_next_sym:
	  iter.NextSym();
	  if (iter.EndOfTerm()) return surf.Leaf(); // the literal is in the index already
	  if (surf.Read(iter.CurrPos()->Symbol())) { goto read_mode_next_sym; };
	  surf.Write(iter.CurrPos()->Symbol()); 
	  goto write_mode_next_sym;
	}
      else
	{
	write_mode_next_sym:
	  iter.NextSym();
	  if (iter.EndOfTerm()) 
	    {
	      surf.CreateLeaf(_sharingIndex->CopyFlatLiteral(lit));
         
	      *(surf.Leaf()->Term()->InfoField()) = (void*)0;
         
	      return surf.Leaf();
	    };       
	  surf.Write(iter.CurrPos()->Symbol());
	  goto write_mode_next_sym;
	}; 
    }
  else // propositional
    {
      if (!(*tree)) 
	{
	  *tree = new DTREE_NODE(ShareProp(header));
	};
    

      ASSERT((*tree)->IsLeaf());
    
      return *tree;
    };
}; // DTREE_NODE* LiteralSharing::Integrate(const Flatterm* lit) 

DTREE_NODE* LiteralSharing::IntegrateEq(const TERM& header,const Flatterm* arg1,const Flatterm* arg2) 
  // returns the leaf corresponding to the literal
{
  CALL("IntegrateEq(const TERM&,const Flatterm*,const Flatterm*)");
  DTREE_NODE** tree = Tree(header);
  Flatterm::Iterator iter;
  iter.Reset(arg1);
  bool first_arg = true;
  DTREE::SURFER surf;
  surf.Reset(tree,1);
  if (*tree)
    {
    read_mode_next_sym:
      if (iter.EndOfTerm()) 
	{
	  if (first_arg)
	    {// the second argument must be processed
	      iter.Reset(arg2);
	      first_arg = false;
	      goto read_mode_next_sym;
	    };
	  return surf.Leaf(); // the literal is in the index already
	};
      if (surf.Read(iter.CurrPos()->Symbol())) 
	{
	  iter.NextSym();
	  goto read_mode_next_sym;
	};
      surf.Write(iter.CurrPos()->Symbol()); 
      iter.NextSym();
      goto write_mode_next_sym;
    }
  else
    {
    write_mode_next_sym:
      if (iter.EndOfTerm()) 
	{
	  if (first_arg)
	    {// the second argument must be processed
	      iter.Reset(arg2);
	      first_arg = false;
	      goto write_mode_next_sym;
	    };
	  surf.CreateLeaf(_sharingIndex->CopyFlatEq(header,arg1,arg2));
	  *(surf.Leaf()->Term()->InfoField()) = (void*)0;
	  return surf.Leaf();
	};
       
      surf.Write(iter.CurrPos()->Symbol());
      iter.NextSym();
      goto write_mode_next_sym;
    }; 
}; // DTREE_NODE* LiteralSharing::IntegrateEq(const TERM& header,const Flatterm* arg1,const Flatterm* arg2) 

void LiteralSharing::InfoForDeletion(TERM* lit,
                                     DTREE_NODE**& branch_to_destroy,
                                     DTREE_NODE*& leaf,
                                     DTREE_NODE*& last_fork)
{
  CALL("InfoForDeletion(TERM*,DTREE_NODE**&,DTREE_NODE*&,DTREE_NODE*&)");
  // lit can't be propositional here
  ASSERT(lit->arity());
  
  DTREE_NODE** tree = Tree(*lit); // *tree must be nonnull and nonleaf here
    
  ASSERT(*tree);
  ASSERT(!((*tree)->IsLeaf()));
  
  TERM::Iterator iter;
  iter.Reset(lit);
  iter.Next(); // skipping the header
  
  DTREE::InfoForDeletion(iter,tree,branch_to_destroy,leaf,last_fork);
}; // void LiteralSharing::InfoForDeletion(TERM* lit,...              






void LiteralSharing::Destroy()
{
  CALL("Destroy()");
  for (ulong n = 0; n < _propositional.size(); n++)
    if (_propositional[n])
      *(_propositional[n]->InfoField()) = (void*)0; 

  for (ulong n = 0; n < _discTree.size(); n++)
    {
      DTREE::Destroy(_discTree[n],DestroyLeafContent);
      _discTree[n] = (DTREE_NODE*)0; 
    };
  
}; //void LiteralSharing::Destroy()  

void LiteralSharing::DestroyLeafContent(DTREE_NODE* leaf)
{  
  if (leaf->Term()->arity()) 
    leaf->Term()->DeleteLit();
}; // void LiteralSharing::DestroyLeafContent(DTREE_NODE* leaf)


#ifndef NO_DEBUG   
bool LiteralSharing::clean() const 
{
  CALL("clean() const");
  for (ulong n = 0; n < _discTree.size(); n++)
    if (_discTree[n]) 
      { 
	cout << "LiteralSharing::clean()> _discTree[" << n << "] != (DTREE_NODE*)0\n";
	return false;
      };

  for (ulong n = 0; n < _propositional.size(); n++)
    if (_propositional[n] && (*(_propositional[n]->InfoField())))
      { 
	return false;
      };
  return true; 
};
#endif


inline TERM* LiteralSharing::ShareSplittingBranchName(const TERM& prop)
{
  CALL("ShareSplittingBranchName(const TERM& prop)");
  ASSERT(!(prop.arity()));
  ASSERT(prop.IsSplitting());
 
  ulong hdNum = prop.HeaderNum();
  DOP(_propositional.unfreeze());
  TERM* res = _propositional.sub(hdNum);
  DOP(_propositional.freeze());
  if (!res) 
    {
      res = TERM::AllocLit(prop);
      *(res->InfoField()) = (void*)0;
      _propositional[hdNum] = res;
    };
  return res;
};


inline TERM* LiteralSharing::ShareProp(const TERM& prop)
{
  CALL("ShareProp(const TERM& prop)");
  ASSERT(!(prop.arity()));
  ulong hdNum = prop.HeaderNum();
  DOP(_propositional.unfreeze());
  TERM* res = _propositional.sub(hdNum);
  DOP(_propositional.freeze());
  if (!res) 
    {
      res = TERM::AllocLit(prop);
      *(res->InfoField()) = (void*)0;
      _propositional[hdNum] = res;
    };
  return res;
};




//============================================================================

