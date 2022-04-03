//
// File:         BSIndex.h
// Description:  Path index for backward subsumption.
// Created:      Feb 26, 2000.
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
// Revised:      Dec 23, 2001. 
//               1) The array BSIndex::_propLitInd replaced by
//               extendable array BSIndex::_propLitOccurences
//               2) Freed from HellConst::MaxNumOfDifferentHeaders.
//============================================================================
#ifndef BS_INDEX_H
//=============================================================================

//#define BS_USE_ORD_LISTS

#define BS_INDEX_H 
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "VampireKernelConst.h"
#include "DestructionMode.h"
#include "Term.h"
#include "GlobAlloc.h"
#include "Array.h"
#ifdef BS_USE_ORD_LISTS
 #include "OrdList.h"
#else
 #include "SkipList.h"
#endif
#include "GList.h"
#include "Tuple.h"
#include "PathIndex.h"
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_BS_INDEX
 #define DEBUG_NAMESPACE "BSIndex"
#endif
#include "debugMacros.h"
//============================================================================
namespace VK
{
  using namespace BK;
  using namespace Gematogen;
class BSIndex
{
 public:
  class Integrator;
  class Retrieval;
  class Removal;
  typedef Tuple2<ulong,TERM> LTPair;
  typedef Tuple2<ulong,ulong> LAPair;
  typedef Tuple3<ulong,ulong,TERM> LATTriple;
  typedef GList<GlobAlloc,ulong,BSIndex> LList; // lit. num.
  typedef GList<GlobAlloc,LTPair,BSIndex> LTList; // lit. num. + term
  typedef GList<GlobAlloc,LAPair,BSIndex> LAList; // lit.num. + arg.num.
  typedef GList<GlobAlloc,LATTriple,BSIndex> LATList; // lit.num. + arg.num. + term

  #ifndef BS_USE_ORD_LISTS
   typedef VoidSkipList<GlobAlloc,ulong,2,28,BSIndex> CSkipList; // for prop. literals
   typedef SkipList<GlobAlloc,LList*,ulong,2,28,BSIndex> CLSkipList; // for constants in ordinary literals
   typedef SkipList<GlobAlloc,LTList*,ulong,2,28,BSIndex> CLTSkipList; // for all in ordinary literals
   typedef SkipList<GlobAlloc,LAList*,ulong,2,28,BSIndex> CLASkipList; // for constants in symmetric literals
   typedef SkipList<GlobAlloc,LATList*,ulong,2,28,BSIndex> CLATSkipList; // for all in symmetric literals 
  #else
   typedef VoidOrdList<GlobAlloc,ulong,BSIndex> CSkipList; // for prop. literals
   typedef OrdList<GlobAlloc,LList*,ulong,BSIndex> CLSkipList; // for constants in ordinary literals
   typedef OrdList<GlobAlloc,LTList*,ulong,BSIndex> CLTSkipList; // for all in ordinary literals
   typedef OrdList<GlobAlloc,LAList*,ulong,BSIndex> CLASkipList; // for constants in symmetric literals
   typedef OrdList<GlobAlloc,LATList*,ulong,BSIndex> CLATSkipList; // for all in symmetric literals  
  #endif
   
  class Sizes 
  {
   public:
    Sizes() 
     : _maxlNumOfNonproplLits(0UL)
    {
    };
    ~Sizes() {}; 
    ulong maxlNumOfNonproplLits() const { return _maxlNumOfNonproplLits; };
   private:
    ulong _maxlNumOfNonproplLits;
   friend class BSIndex;
   friend class Integrator; 
  }; // class Sizes

 public:
  BSIndex() 
   : 
    #ifdef NO_DEBUG
     _propLitOccurences((CSkipList*)0,(int)0),
    #else
     _propLitOccurences((CSkipList*)0,"BSIndex::_propLitOccurences"),    
    #endif 

    _sizes()
  {
   CALL("constructor BSIndex()");
    
   DOP(_propLitOccurences.freeze());
   _ordLitInd[0].init(); 
   _symLitInd[0].init(); 
  }; 
  ~BSIndex() 
   { 
    CALL("destructor ~BSIndex()"); 
    if (DestructionMode::isThorough()) destroy();
   }; 
 
  void destroy();
  const Sizes* sizes() const { return &_sizes; };

  #ifndef NO_DEBUG
   bool clean() const 
   {
    for (ulong i = 0; i < _propLitOccurences.size(); i++) 
      {
	if (_propLitOccurences[i] && 
	    _propLitOccurences[i]->isNonempty())
	  return false;
      };
    return (!_ordLitInd[0]) && (!_symLitInd[0]);
   }; 

   static bool greater(const LAPair& p1,const LAPair& p2) 
   {
    if (p1.el1 == p2.el1) return p1.el2 > p2.el2;
    return p1.el1 > p2.el1;
   };

   static bool greater(const LATTriple& p1,const LATTriple& p2) 
   {
    if (p1.el1 == p2.el1) return p1.el2 > p2.el2;
    return p1.el1 > p2.el1;
   };
  #endif 

 private: // types
  typedef PathIndex<GlobAlloc,TERM,CLTSkipList,CLSkipList,VampireKernelConst::MaxTermDepth,VampireKernelConst::MaxTermSize> OrdLitPITree; 
  typedef PathIndex<GlobAlloc,TERM,CLATSkipList,CLASkipList,VampireKernelConst::MaxTermDepth,VampireKernelConst::MaxTermSize> SymLitPITree;  

 private: // methods
  static void remove(const ulong& clNum,CLSkipList& lst)
  {
   LList* llist;
   if (lst.remove(clNum,llist)) LList::destroyList(llist); 
  };
  static void remove(const ulong& clNum,CLTSkipList& lst)
  {
   LTList* ltlist;
   if (lst.remove(clNum,ltlist)) LTList::destroyList(ltlist); 
  }; 
  static void remove(const ulong& clNum,CLASkipList& lst)
  {
   LAList* lalist;
   if (lst.remove(clNum,lalist)) LAList::destroyList(lalist); 
  };  
  static void remove(const ulong& clNum,CLATSkipList& lst)
  {
   LATList* latlist;
   if (lst.remove(clNum,latlist)) LATList::destroyList(latlist); 
  };
 
  static void recycle(CLSkipList& lst)
  { 
   LList* llist;
   CLSkipList::Destruction destr(lst);
   while (destr.next(llist)) LList::destroyList(llist);      
  };

  static void recycle(CLTSkipList& lst)
  { 
   LTList* ltlist;
   CLTSkipList::Destruction destr(lst);
   while (destr.next(ltlist)) LTList::destroyList(ltlist);      
  }; 
  
  static void recycle(CLASkipList& lst)
  { 
   LAList* lalist;
   CLASkipList::Destruction destr(lst);
   while (destr.next(lalist)) LAList::destroyList(lalist);      
  };
 
  static void recycle(CLATSkipList& lst)
  { 
   LATList* latlist;
   CLATSkipList::Destruction destr(lst);
   while (destr.next(latlist)) LATList::destroyList(latlist);      
  };
 
  void insertPropLit(const TERM& lit,ulong clauseNum)
  {
   CALL("insertPropLit(const TERM& lit,ulong clauseNum)");
   DOP(_propLitOccurences.unfreeze());
   CSkipList* occList = _propLitOccurences.sub(lit.HeaderNum());
   DOP(_propLitOccurences.freeze());
   if (!occList) 
    {
     occList = new CSkipList();
     _propLitOccurences[lit.HeaderNum()] = occList;
    };
   occList->insert(clauseNum);
   //_propLitInd[lit.HeaderNum()].insert(clauseNum);
  };

  void removePropLit(const TERM& lit,ulong clauseNum)
  {
   CALL("removePropLit(const TERM& lit,ulong clauseNum)");
   if (_propLitOccurences[lit.HeaderNum()])
     _propLitOccurences[lit.HeaderNum()]->remove(clauseNum);
  };

  const CSkipList* propLitOccurences(const TERM& lit)
  {
   CALL("propLitOccurences(const TERM& lit)");
   DOP(_propLitOccurences.unfreeze());
   CSkipList* occList = _propLitOccurences.sub(lit.HeaderNum());
   DOP(_propLitOccurences.freeze());
   if (!occList) 
    {
     // This is done to simulate the old version functionality
     occList = new CSkipList();
     _propLitOccurences[lit.HeaderNum()] = occList;
    };
   return occList;
  };

 private: // data
   
  Array<GlobAlloc,CSkipList*,128UL,BSIndex> _propLitOccurences;
  
  OrdLitPITree::Trie _ordLitInd[1];
  SymLitPITree::Trie _symLitInd[1];
  Sizes _sizes;
 friend class Integrator;
 friend class Retrieval;
 friend class Removal; 
}; // class BSIndex; 

}; // namespace VK


//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_BS_INDEX
 #define DEBUG_NAMESPACE "BSIndex::Integrator"
#endif
#include "debugMacros.h"
//============================================================================
namespace VK
{
  using namespace BK;
  using namespace Gematogen;

class BSIndex::Integrator
{ 
 public:
  Integrator() 
   : _ordLitIntegrator(),
     _symLitIntegrator()
  {
   CALL("constructor Integrator()");
  }; 
  Integrator(BSIndex* index)
   : _ordLitIntegrator(),
     _symLitIntegrator()
  {
   CALL("constructor Integrator(BSIndex* index)");  
   reset(index); 
  };
  void reset(BSIndex* index) { _index = index; }; 
  void clause(const ulong& clNum) { _clauseNum = clNum; _litNum = 0; };
  void endOfClause()
  {
   if (_litNum > _index->_sizes._maxlNumOfNonproplLits)
    _index->_sizes._maxlNumOfNonproplLits = _litNum;
  };
  void propLit(const TERM& lit)
  {
   CALL("propLit(const TERM& lit)");
   _index->insertPropLit(lit,_clauseNum);
  };
  void litHeader(const TERM& hd)
  {
   _ordLitIntegrator.reset(_index->_ordLitInd[0]);
   _ordLitIntegrator.nonconstFunc(hd);   
   _litNum++;
  };
  void subterm(const TERM& t)
  {
   CALL("subterm(const TERM& t)");
   bool newNode;
   CLTSkipList::Node* node = _ordLitIntegrator.indexedObj().insert(_clauseNum,newNode);  
   if (newNode)
    {
     node->value() = new LTList(LTPair(_litNum,t)); 
    }
   else
    {
     ASSERT(node->value());
     ASSERT(_litNum > node->value()->hd().el1);
     node->value() = new LTList(LTPair(_litNum,t),node->value()); 
    };
   if (t.isVariable())
    {
     _ordLitIntegrator.variable();
    } 
   else // t is reference to a complex term 
    {
     ASSERT(t.IsReference());
     TERM sym = *(t.First());
     if (sym.arity())
      {
       _ordLitIntegrator.nonconstFunc(sym);
      }
     else // constant
      {
       _ordLitIntegrator.constant(sym);
       CLSkipList::Node* cnode = _ordLitIntegrator.constIndObj().insert(_clauseNum,newNode); 
       if (newNode) 
	{ 
         cnode->value() = new LList(_litNum);
        }    
       else 
        {
	 ASSERT(cnode->value());
         ASSERT(_litNum > cnode->value()->hd());
         cnode->value() = new LList(_litNum,cnode->value());
        };
      };
    };
  }; // void subterm(const TERM& t)
  void symLitHeader(const TERM& hd)
  {
   _symLitIntegrator.reset(_index->_symLitInd[0]);
   _symLitIntegrator.nonconstFunc(hd);   
   _litNum++;
   _symLitArgNum = 0;
   _symLitHeader = hd;
  };
  void symLitSecondArg() 
  { 
   _symLitIntegrator.reset(_index->_symLitInd[0]);
   _symLitIntegrator.nonconstFunc(_symLitHeader);   
   _symLitArgNum = 1; 
  };
  void symLitSubterm(const TERM& t)
  {
   CALL("symLitSubterm(const TERM& t)");
   bool newNode;


   CLATSkipList::Node* node = _symLitIntegrator.indexedObj().insert(_clauseNum,newNode);
   ASSERT(newNode || greater(LATTriple(_litNum,_symLitArgNum,t),node->value()->hd()));
   if (newNode) node->value() = (LATList*)0;  
   node->value() = new LATList(LATTriple(_litNum,_symLitArgNum,t),node->value());
   if (t.isVariable())
    {   
     _symLitIntegrator.variable();  
    } 
   else // t is reference to a complex term 
    {    
     ASSERT(t.IsReference()); 
     TERM sym = *(t.First());
     if (sym.arity())
      {  
       _symLitIntegrator.nonconstFunc(sym);
      }
     else // constant
      {  
       _symLitIntegrator.constant(sym); 
       CLASkipList::Node* cnode = _symLitIntegrator.constIndObj().insert(_clauseNum,newNode);
       ASSERT(newNode || greater(LAPair(_litNum,_symLitArgNum),cnode->value()->hd()));
       if (newNode) cnode->value() = (LAList*)0; 
       cnode->value() = new LAList(LAPair(_litNum,_symLitArgNum),cnode->value());
      };
    };
  
  }; // void symLitSubterm(const TERM& t)
 private:
  BSIndex* _index;
  
  OrdLitPITree::Integrator _ordLitIntegrator;
  SymLitPITree::Integrator _symLitIntegrator;

  ulong _clauseNum;
  ulong _litNum;
  ulong _symLitArgNum;
  TERM _symLitHeader;
}; // class BSIndex::Integrator


}; // namespace VK


//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_BS_INDEX
 #define DEBUG_NAMESPACE "BSIndex::Retrieval"
#endif
#include "debugMacros.h"
//============================================================================

namespace VK
{
  using namespace BK;
  using namespace Gematogen;
class BSIndex::Retrieval
{
 public:
  Retrieval() 
   : _ordLitRetrieval(),
     _symLitRetrieval()
  {
   CALL("constructor Retrieval()");
  }; 
  Retrieval(BSIndex* index)
   : _ordLitRetrieval(),
     _symLitRetrieval()
  { 
   CALL("constructor Retrieval(BSIndex* index)");
   reset(index); 
  };
  void reset(BSIndex* index) { _index = index; }; 
  bool propLit(const TERM& lit,const CSkipList*& occList)
  {
   CALL("propLit(const TERM& lit,const CSkipList*& occList)");
   occList = _index->propLitOccurences(lit);
   //occList = _index->_propLitInd + lit.HeaderNum();
   return (bool)(*occList);
  };
  bool litHeader(const TERM& hd)
  {
   return ((_ordLitRetrieval.reset(_index->_ordLitInd[0])) && (_ordLitRetrieval.nonconstFunc(hd)));
  };
  bool nonconstFunc(const TERM& f)
  {
   return _ordLitRetrieval.nonconstFunc(f); 
  };
  bool constant(const TERM& c,const CLSkipList*& occList)
  {
   if (_ordLitRetrieval.constant(c))
    {
     occList = &(_ordLitRetrieval.constIndObj());
     return (bool)(*occList);
    }; 
   return false;
  };
  bool variable(const CLTSkipList*& occList)
  {
   occList = &(_ordLitRetrieval.indexedObj());  
   _ordLitRetrieval.after();
   return (bool)(*occList);
  };

  bool symLitHeader(const TERM& hd)
  {
   _symLitHeader = hd;
   return ((_symLitRetrieval.reset(_index->_symLitInd[0])) && (_symLitRetrieval.nonconstFunc(hd)));
  };
 
  bool symLitSecondArg()
  {
   return ((_symLitRetrieval.reset(_index->_symLitInd[0])) && (_symLitRetrieval.nonconstFunc(_symLitHeader)));   
  };

  bool symLitNonconstFunc(const TERM& f)
  {
   return _symLitRetrieval.nonconstFunc(f); 
  };
  bool symLitConstant(const TERM& c,const CLASkipList*& occList)
  {
   if (_symLitRetrieval.constant(c))
    {
     occList = &(_symLitRetrieval.constIndObj());
     return (bool)(*occList);
    }; 
   return false;
  };
  bool symLitVariable(const CLATSkipList*& occList)
  {
   occList = &(_symLitRetrieval.indexedObj());  
   _symLitRetrieval.after();
   return (bool)(*occList);
  };

 private:
  BSIndex* _index;
  OrdLitPITree::Retrieval _ordLitRetrieval;
  SymLitPITree::Retrieval _symLitRetrieval;
  TERM _symLitHeader;
}; // class BSIndex::Retrieval

}; // namespace VK
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_BS_INDEX
 #define DEBUG_NAMESPACE "BSIndex::Removal"
#endif
#include "debugMacros.h"
//============================================================================
namespace VK
{
  using namespace BK;
  using namespace Gematogen;

class BSIndex::Removal
{
 public:
  Removal() 
   : _ordLitRemoval(),
     _symLitRemoval()
  {
   CALL("constructor Removal()");
  }; 
  Removal(BSIndex* index)
   : _ordLitRemoval(),
     _symLitRemoval()
  {
   CALL("constructor Removal(BSIndex* index)");
   reset(index); 
  };
  void reset(BSIndex* index) { _index = index; }; 
  void clause(const ulong& clNum) { _clauseNum = clNum; };
  void propLit(const TERM& lit)
  {
   CALL("propLit(const TERM& lit)");
   _index->removePropLit(lit,_clauseNum); 
  };

  bool litHeader(const TERM& hd)
  {
   CALL("litHeader(const TERM& hd)");
   //_ordLitRemoval.reset();
   _ordLitRemoval.reset(_index->_ordLitInd[0]);
   return _ordLitRemoval.nonconstFunc(hd);
  };

  bool nonconstFunc(const TERM& f)
  {
   CALL("nonconstFunc(const TERM& f)");
   if (_ordLitRemoval.occurences()) BSIndex::remove(_clauseNum,_ordLitRemoval.indexedObj());
   return _ordLitRemoval.nonconstFunc(f);
  };
 
  bool constant(const TERM& c)
  {
   CALL("constant(const TERM& c)");

   if (_ordLitRemoval.occurences()) BSIndex::remove(_clauseNum,_ordLitRemoval.indexedObj());
   if (_ordLitRemoval.constant(c))
    {
     BSIndex::remove(_clauseNum,_ordLitRemoval.constIndObj());
     return true;
    }; 
   return false;
  };
 
  void variable()
  {
   CALL("variable()");
   if (_ordLitRemoval.occurences()) BSIndex::remove(_clauseNum,_ordLitRemoval.indexedObj());
   _ordLitRemoval.after();
  };
  
  void after() { _ordLitRemoval.after(); };

  bool symbol(const TERM& s) 
  {
   CALL("symbol(const TERM& s)");
   if (s.isVariable()) { variable(); return true; };
   if (s.arity()) return nonconstFunc(s); 
   return constant(s);
  };
 
  void endOfLit() { CALL("endOfLit()"); _ordLitRemoval.recycleObsoleteNodes(); };

  bool symLitHeader(const TERM& hd)
  {
   CALL("symLitHeader(const TERM& hd)");
   //_symLitRemoval.reset();
   _symLitHeader = hd;
   _symLitRemoval.reset(_index->_symLitInd[0]);
   return _symLitRemoval.nonconstFunc(hd);
  };
  
  bool symLitSecondArg()
  {
   CALL("symLitSecondArg()");
   _symLitRemoval.recycleObsoleteNodes();
   _symLitRemoval.reset(_index->_symLitInd[0]);
   return _symLitRemoval.nonconstFunc(_symLitHeader);  
  };
 
  bool symLitNonconstFunc(const TERM& f)
  {
   CALL("symLitNonconstFunc(const TERM& f)");
   if (_symLitRemoval.occurences()) BSIndex::remove(_clauseNum,_symLitRemoval.indexedObj());
   return _symLitRemoval.nonconstFunc(f);
  };
 
  bool symLitConstant(const TERM& c)
  {
   CALL("symLitConstant(const TERM& c)");
   if (_symLitRemoval.occurences()) BSIndex::remove(_clauseNum,_symLitRemoval.indexedObj());
   if (_symLitRemoval.constant(c))
    {
     BSIndex::remove(_clauseNum,_symLitRemoval.constIndObj());
     return true;
    }; 
   return false;
  };
 
  void symLitVariable()
  {
   CALL("symLitVariable()");
   if (_symLitRemoval.occurences()) BSIndex::remove(_clauseNum,_symLitRemoval.indexedObj());
   _symLitRemoval.after();
  };

  void symLitAfter() { _symLitRemoval.after(); };
  bool symLitSymbol(const TERM& s) 
  {
   CALL("symLitSymbol(const TERM& s)");
   if (s.isVariable()) { symLitVariable(); return true; };
   if (s.arity()) return symLitNonconstFunc(s); 
   return symLitConstant(s);
  };

  void endOfSymLit() { CALL("endOfSymLit()"); _symLitRemoval.recycleObsoleteNodes(); };

 private:
  BSIndex* _index;
  OrdLitPITree::Removal _ordLitRemoval;
  SymLitPITree::Removal _symLitRemoval;
  TERM _symLitHeader;
  ulong _clauseNum;
}; // class BSIndex::Removal

}; // namespace VK

//======================================================================
#undef DEBUG_NAMESPACE
//======================================================================
#endif
