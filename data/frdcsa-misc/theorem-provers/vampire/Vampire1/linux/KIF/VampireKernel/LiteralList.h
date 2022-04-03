//
// File:         LiteralList.h
// Description:  Representation of lists of stored marked literals.
// Created:      Dec 09, 2000, 16:20
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
// Revised:      Dec 11, 2001.
//               LiteralList::Iterator::{litNum(),_litNum} added.
//====================================================
#ifndef LITERAL_LIST_H
//====================================================
#define LITERAL_LIST_H
#include <iostream.h>
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#ifdef DEBUG_ALLOC_OBJ_TYPE
#include "ClassDesc.h"       
#endif
#include "VampireKernelConst.h"
#include "GlobAlloc.h"
#include "DestructionMode.h"
#include "Term.h"

//===================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_LITERAL_LIST
#define DEBUG_NAMESPACE "LiteralList"
#endif 
#include "debugMacros.h"
//=================================================== 
namespace VK
{
  using namespace BK;

class LiteralList
{
 public:
  enum Mark
  {
    Clean              = 0x00000000, 
    InherentlySelected = 0x00000001, 
    Reserved10         = 0x00000002, 
    Reserved11         = 0x00000003
  };
  class Element
    {
    public: 
      Mark mark() const 
	{
	  CALL("mark() const");
	  return Mark(_content.Flag()); 
	};

      bool isInherentlySelected() const 
	{
	  CALL("isInherentlySelected() const"); 
	  return mark() == InherentlySelected; 
	};

      void setMark(Mark m) 
	{
	  CALL("Element::setMark(Mark m)");

	  //DF; if (m == InherentlySelected) { REPCPH; };
  
	  _content.SetFlag(TermFlag(m));
	  ASSERT(mark() == m); 
	};
      TERM* literal() const
	{  
	  CALL("Element::literal()");
	  return (TERM*)((_content.Content() >> 2) << 2);
	};
         
      TERM* cleanLiteral() const 
	{  
	  // for those cases when we know that the literal is not marked
	  CALL("Element::cleanLiteral() const");
	  ASSERT(mark() == Clean);
	  return _content.First();
	};
      TERM literalRef() const 
	{
	  CALL("literalRef() const");
	  return TERM(literal());
	}; 
      TERM cleanLiteralRef() const 
	{
	  // for those cases when we know that the literal is not marked
	  CALL("cleanLiteralRef() const");
	  return _content;
	}; 
      void setLiteral(TERM* lit) 
	{
	  CALL("setLiteral(TERM* lit)");
	  _content.SetFirst(lit);      
	};
      void setLiteral(TERM* lit,Mark m) 
	{ 
	  CALL("setLiteral(TERM* lit,Mark m)");
	  _content.SetFirst(lit); 
	  setMark(m); 
	};
      Element& operator=(const Element& el) 
      {
	_content = el._content; 
	return *this; 
      };
    
      Element* next() { return _next; };  
      const Element* next() const { return _next; };   


      // some useful shorthands 
      TERM::Arity arity() const { return literal()->arity(); };
      TERM::Polarity polarity() const { return literal()->polarity(); }; 
      bool positive() const { return literal()->Positive(); };
      bool negative() const { return literal()->Negative(); };
      TERM::Functor functor() const { return literal()->functor(); };
      ulong headerNum() const { return literal()->HeaderNum(); };
      bool isSymLit() const { return literal()->IsSymLitHeader(); };    
      bool isEquality() const { return literal()->IsEquality(); };     
      bool isOrderedEq() const { return literal()->IsOrderedEq(); };
      bool isUnorderedEq() const { return literal()->IsUnorderedEq(); };
      TERM* args() { return literal()->Args(); };
      const TERM* args() const { return literal()->Args(); }; 
      TERM* arg1() { return literal()->Arg1(); };
      const TERM* arg1() const { return literal()->Arg1(); };
      TERM* arg2() { return literal()->Arg2(); };
      const TERM* arg2() const { return literal()->Arg2(); };

    private:
      Element() {};
      Element(const Element&) {};
      Element(Element* nxt) { _next = nxt; };
      ~Element() {};
      void* operator new(size_t) 
	{ 
#ifdef DEBUG_ALLOC_OBJ_TYPE
	  ALWAYS(_classDesc.registerAllocated(sizeof(Element)));
	  return GlobAlloc::allocate(sizeof(Element),&_classDesc); 
#else
	  return GlobAlloc::allocate(sizeof(Element)); 
#endif 
	};
      void  operator delete(void* obj) 
	{ 
	  CALL("Element::operator delete(void* obj)");
#ifdef DEBUG_ALLOC_OBJ_TYPE
	  ALWAYS(_classDesc.registerDeallocated(sizeof(Element)));
	  GlobAlloc::deallocate(obj,sizeof(Element),&_classDesc); 
#else
	  GlobAlloc::deallocate(obj,sizeof(Element)); 
#endif
	};
    private:
      TERM _content;
      Element* _next;
#ifdef DEBUG_ALLOC_OBJ_TYPE 
      static ClassDesc _classDesc;
#endif
      friend class LiteralList; 
    }; // class Element
  
  class Iterator
    {
    public:
      Iterator() : _current((Element*)0), _litNum(0UL)
	//, _end((Element*)0) 
	{
	};
      Iterator(LiteralList& lst) { reset(lst); };
      Iterator(LiteralList* lst) { reset(lst); };
      Iterator(LiteralList& lst,LiteralList::Element* start) { reset(lst,start); };
      Iterator(LiteralList* lst,LiteralList::Element* start) { reset(lst,start); };
      ~Iterator() {};
      void reset(LiteralList& lst) 
	{
	  _current = lst.firstElem();
	  //_end = lst.endOfElements();
	  _litNum = 0UL;
	};
      void reset(LiteralList* lst) 
	{
	  _current = lst->firstElem();
	  //_end = lst->endOfElements();
	  _litNum = 0UL;
	};
      void reset(LiteralList& lst,LiteralList::Element* start) 
	{
	  CALL("reset(LiteralList& lst,LiteralList::Element* start)");
	  _current = start;
	  //_end = lst.endOfElements();
	  _litNum = 0UL;
	};
      void reset(LiteralList* lst,LiteralList::Element* start) 
	{
	  CALL("reset(LiteralList* lst,LiteralList::Element* start)");
	  _current = start;
	  //_end = lst->endOfElements();
	  _litNum = 0UL;
	};
      operator bool() const { return (bool)_current; };
      bool end() const { return !_current; };
      bool last() const 
	{ 
	  return (_current) && (!_current->next()); 
	}; 
      bool notEnd() const { return (bool)_current; };
      void next() 
	{
	  CALL("Iterator::next()");
	  ASSERT(_current);
	  _current = _current->next();
	  _litNum++;
	};

      Element* currentElement() const
	{
	  CALL("currentElement() const");
	  return _current;
	};
      TERM* currentLiteral() const
	{
	  CALL("currentLiteral() const");
	  ASSERT(_current);
	  return _current->literal();
	};
      ulong litNum() const { return _litNum; };
    private:
      Element* _current;
      //Element* _end;
      ulong _litNum; 
    }; // class LiteralList::Iterator

  
 public:
  LiteralList(ulong numOfLits,ulong numOfSelectedLits)
    {
      CALL("constructor LiteralList(ulong numOfLits,ulong numOfSelectedLits)");
      ASSERT(numOfLits <= VampireKernelConst::MaxlNumOfLits);
      ASSERT(numOfSelectedLits <= numOfLits);
      ((_sizes = numOfLits) <<= 16) |= numOfSelectedLits;
      _elements = (Element*)0;
      for (ulong n = 0; n < numOfLits; n++)
	_elements = new Element(_elements);
   
    };
  ~LiteralList() 
    { 
      CALL("destructor ~LiteralList()");
      if (DestructionMode::isThorough())
	{
	  while (_elements) 
	    { 
	      Element* tmp = _elements->next();
	      delete _elements;
	      _elements = tmp;  
	    };
	};
    };

  void* operator new(size_t,ulong numOfLits)
    {
#ifdef DEBUG_ALLOC_OBJ_TYPE
      ALWAYS(_classDesc.registerAllocated(sizeof(LiteralList)));
      return GlobAlloc::allocate(sizeof(LiteralList),&_classDesc);
#else 
      return GlobAlloc::allocate(sizeof(LiteralList)); 
#endif
    };

  void operator delete(void* obj)
    {
#ifdef DEBUG_ALLOC_OBJ_TYPE
      ALWAYS(_classDesc.registerDeallocated(sizeof(LiteralList)));
      GlobAlloc::deallocate(obj,sizeof(LiteralList),&_classDesc);
#else
      GlobAlloc::deallocate(obj,sizeof(LiteralList));
#endif
    };
    
  ulong length() const { return (_sizes >> 16); };
  ulong numOfSelectedLits() const { return ((_sizes << 16) >> 16); };
  void setNumOfSelected(ulong n) 
    { 
      CALL("setNumOfSelected(ulong n)");
      ASSERT(n < VampireKernelConst::MaxlNumOfLits);
      ((_sizes >>= 16) <<= 16) |= n;
    };
  Element* firstElem() 
    {
      return _elements; 
    };
  const Element* firstElem() const { return _elements; };
  TERM* firstLit() const 
    {
      CALL("firstLit() const");
      ASSERT((length()) && (length() <= VampireKernelConst::MaxlNumOfLits));
      return _elements->literal(); 
    };
  
  Element* nth(ulong n)
    {
      CALL("nth(ulong n)");
      ASSERT(n <= length());
      Element* res = _elements;
      while (n) 
	{
	  res = res->next();
	  n--;
	};
      return res;
    };

  bool containsOnlyAnswerLiterals() const
    {
      for (const Element* el = firstElem(); el; el = el->next())
	if (!el->literal()->isAnswerLiteral()) return false;
      return true;
    };

  ostream& output(ostream& str) const;
   
 private:
#ifdef DEBUG_ALLOC_OBJ_TYPE 
  static ClassDesc _classDesc;
#endif
 private:
  LiteralList() { CALL("constructor LiteralList()"); ICP("0"); };
  void* operator new(size_t)
    {
      CALL("operator new(size_t)");
      ICP("0");
      return (void*)0;
    };

 private:
  ulong _sizes;  
  Element* _elements;
  friend class Iterator;
  friend class Element;
}; // class LiteralList

}; // namespace VK

//===================================================
#endif
