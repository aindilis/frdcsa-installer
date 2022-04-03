//
// File:         Input.h
// Description:  Input signature, terms and clauses.
// Created:      Jun 04, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//======================================================================
#ifndef INPUT_H
#define INPUT_H
//======================================================================
#ifndef NO_DEBUG_VIS
#include <iostream.h>
#endif
#include <string.h>
#include <stdio.h>
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "DestructionMode.h"
#include "GList.h"
#include "Malloc.h"
#include "VampireKernel.h"
//======================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_INPUT
 #define DEBUG_NAMESPACE "Input"
#endif
#include "debugMacros.h"
//======================================================================

namespace VK
{
  using namespace BK;

class Input
{
 public:
  class Symbol;
  class Signature;
  class Term;
  class Literal;
  class Clause;
  class Interpretation;
  typedef GList<Malloc,Term*,Input> TermList;
  typedef GList<Malloc,Literal*,Input> LiteralList;
  typedef GList<Malloc,Clause*,Input> ClauseList;
  typedef GList<Malloc,Interpretation,Input> InterpretationList;
 public:
  static void destroyTermList(TermList* lst);
  static void destroyLiteralList(LiteralList* lst);
  static void destroyClauseList(ClauseList* lst);
};
}; // namespace VK


#ifndef NO_DEBUG_VIS
ostream& operator<<(ostream& str,const VK::Input::ClauseList* lst);
#endif

//======================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_INPUT
 #define DEBUG_NAMESPACE "Input::Interpretation"
#endif
#include "debugMacros.h"
//======================================================================

namespace VK
{
  class Input::Interpretation
    {
    public:
      Interpretation() :
	_theoryName((char*)0),
	_nativeSymbolName((char*)0) 
	{
	};
      void init(const char* theory,const char* nativeSymbol) 
	{
	  CALL("init(const char* theory,const char* nativeSymbol)");
	  _theoryName = (char*)Malloc::allocate(strlen(theory) + 1);
	  strcpy(_theoryName,theory);
	  _nativeSymbolName = (char*)Malloc::allocate(strlen(nativeSymbol) + 1);
	  strcpy(_nativeSymbolName,nativeSymbol);
	};
      ~Interpretation()
	{
	  CALL("destructor ~Interpretation()");
	  if (_theoryName)
	    Malloc::deallocate((void*)_theoryName,strlen(_theoryName)+1);
	  if (_nativeSymbolName)
	    Malloc::deallocate((void*)_nativeSymbolName,strlen(_nativeSymbolName)+1);
	  
	};
      const char* theoryName() const { return _theoryName; };
      const char* nativeSymbolName() const { return _nativeSymbolName; };
    private:
      char* _theoryName;
      char* _nativeSymbolName;
    };

}; // namespace VK

//======================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_INPUT
 #define DEBUG_NAMESPACE "Input::Symbol"
#endif
#include "debugMacros.h"
//======================================================================

namespace VK
{
  using namespace BK;
class Input::Symbol
{
 public:
  enum Type { Variable, Function, Predicate };
  const Type& type() const { return _type; };
  const char* printName() const { return _printName; };
  ulong number() const { return _number; };
  void assignNumber(ulong n) { _number = n; };
  ulong arity() const 
    {
      CALL("arity() const");
      return _arity; 
    };
  ulong variableNumber() const { return _normalisedVariableNumber; };
  bool precedenceAssigned() const { return _precedenceAssigned; };
  long precedence() const 
    {
      CALL("precedence() const");
      ASSERT(precedenceAssigned());
      return _precedence;
    };

  void addInterpretation(const char* theory,const char* nativeSymbol)
    {
      CALL("addInterpretation(const char* theory,const char* nativeSymbol)");
      _interpretations = 
	new InterpretationList(_interpretations);
      _interpretations->hd().init(theory,nativeSymbol);
    };

  const InterpretationList* interpretations() const { return _interpretations; };

  bool isAnswer() const { return _isAnswer; };

  bool weightAssigned() const { return _weightAssigned; };
  VampireKernel::TermWeightType weightConstPart() const { return _weightConstPart; };


  void setWeightConstPart(VampireKernel::TermWeightType c)
    {
      _weightConstPart = c;
      _weightAssigned = true;
    };

  Symbol* next() { return _next; };

 private:
  Symbol(const Type& t,const char* nm,ulong a) :
    _type(t),
    _arity(a),
    _precedenceAssigned(false),
    _isAnswer(false),
    _weightConstPart(1L),
    _weightAssigned(false),
    _interpretations((InterpretationList*)0),
    _variableTimeStamp(0UL),
    _normalisedVariable((Symbol*)0),
    _smallerNames((Symbol*)0),
    _greaterNames((Symbol*)0),
    _smallerPrecedences((Symbol*)0),
    _greaterPrecedences((Symbol*)0),
    _next((Symbol*)0)
    {
      CALL("constructor Symbol(const Type& t,const char* nm,ulong a)");
      _printName = (char*)Malloc::allocate(strlen(nm) + 1);
      strcpy(_printName,nm);
    };
  ~Symbol()
    {
      CALL("destructor ~Symbol()");
      if (DestructionMode::isThorough())
	{
	  Malloc::deallocate((void*)_printName,strlen(_printName) + 1);
	  InterpretationList::destroyList(_interpretations);
	};
    };
  
  void* operator new(size_t)
    {
      CALL("operator new(size_t)");
      return Malloc::allocate(sizeof(Symbol));
    };
  
  void operator delete(void* obj)
    {
      CALL("operator delete(void* obj)");
      Malloc::deallocate(obj,sizeof(Symbol));
    };

  void setType(const Type& t) { _type = t; };

  ulong& normalisedVariableNumber() 
    {
      CALL("normalisedVariableNumber()");
      ASSERT(_type == Variable);
      return _normalisedVariableNumber; 
    };
  ulong& variableTimeStamp() 
    {
      CALL("variableTimeStamp()");
      ASSERT(_type == Variable);
      return _variableTimeStamp; 
    };
  Symbol*& normalisedVariable() 
    {
      CALL("normalisedVariable()");
      ASSERT(_type == Variable);
      return _normalisedVariable; 
    };
  Symbol*& smallerNames() { return _smallerNames; };
  Symbol*& greaterNames() { return _greaterNames; };
  Symbol*& smallerPrecedences() { return _smallerPrecedences; };
  Symbol*& greaterPrecedences() { return _greaterPrecedences; };
  
  void assignPrecedence(long prec) 
    {
      CALL("assignPrecedence(long prec)");
      ASSERT(!precedenceAssigned());
      _precedenceAssigned = true;
      _precedence = prec;
    };
  
  void markAsAnswer() { _isAnswer = true; };
  void setNext(Symbol* s) { _next = s; };

 private:
  Type _type;
  char* _printName;
  ulong _number;
  ulong _arity;
  bool _precedenceAssigned;
  long _precedence;
  bool _isAnswer;
  VampireKernel::TermWeightType _weightConstPart;
  bool _weightAssigned;
  InterpretationList* _interpretations;
  ulong _normalisedVariableNumber;
  ulong _variableTimeStamp;
  Symbol* _normalisedVariable;
  Symbol* _smallerNames;
  Symbol* _greaterNames;
  Symbol* _smallerPrecedences;
  Symbol* _greaterPrecedences;
  Symbol* _next;
  friend class Input::Signature;  
}; // class Input::Symbol

}; // namespace VK

#ifndef NO_DEBUG_VIS
inline
ostream& operator<<(ostream& str,const VK::Input::Symbol* s)
{
  return str << s->printName();
};

#endif

//======================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_INPUT
 #define DEBUG_NAMESPACE "Input::Signature"
#endif
#include "debugMacros.h"
//======================================================================

namespace VK
{
  using namespace BK;

class Input::Signature
{
 public:
  Signature() : 
    _symbols((Symbol*)0),
    _normalisedVariables((Symbol*)0),
    _nextNormalisedVariable(&_normalisedVariables),
    _nextNormalisedVariableNumber(0UL),
    _currentVariableTimeStamp(0UL),
    _precedences((Symbol*)0),
    _symbolList((Symbol*)0)
    {
    };
  ~Signature()
    {
      CALL("destructor ~Signature()");
      if (DestructionMode::isThorough())
	{
	  destroyTree(_symbols);
	  destroyTree(_normalisedVariables);
	};
    };
  
  void resetVariables() 
    {
      _nextNormalisedVariable = &_normalisedVariables;
      ++_currentVariableTimeStamp;
    };

  const Symbol* registerVariable(const char* name);
  const Symbol* registerFunction(const char* name,ulong arity);
  const Symbol* registerPredicate(const char* name,ulong arity);
  Symbol* registerSymbol(bool predicate,const char* name,ulong arity);
  bool registerPrecedence(bool predicate,const char* name,ulong arity,long prec);
  bool registerAnswerPredicate(const char* name,ulong arity);
  Symbol* symbolList() { return _symbolList; };

 private:
  Symbol* nextNormalisedVariable();
  static const char* normalisedVariableName(ulong varNum);
  static void destroyTree(Symbol* tree);
  static Symbol** lookUp(bool predicate,const char* name,ulong arity,Symbol** tree);
  static Symbol** lookUpPrecedence(long precedence,Symbol** tree);
 private:
  Symbol* _symbols;
  Symbol* _normalisedVariables;
  Symbol** _nextNormalisedVariable;
  ulong _nextNormalisedVariableNumber;
  ulong _currentVariableTimeStamp;
  Symbol* _precedences;
  Symbol* _symbolList;
}; // class Input::Signature

}; // namespace VK

//======================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_INPUT
 #define DEBUG_NAMESPACE "Input::Term"
#endif
#include "debugMacros.h"
//======================================================================

namespace VK
{
  using namespace BK;
class Input::Term
{
 public:
  Term(const Symbol* t) : _top(t), _arguments((TermList*)0) {};
  Term(const Symbol* t,TermList* args) : _top(t), _arguments(args) {};
  ~Term()
    {
      CALL("destructor ~Term()");
      if (DestructionMode::isThorough())
	{
	  destroyTermList(_arguments);
	};
    };
  void* operator new(size_t)
    {
      CALL("operator new(size_t)");
      return Malloc::allocate(sizeof(Term));
    };
  void operator delete(void* obj)
    {
      CALL("operator delete(void* obj)");
      Malloc::deallocate(obj,sizeof(Term));
    };
  const Symbol* top() const { return _top; };
  const TermList* arguments() const { return _arguments; };

 private:
  Term() {};
 private:
  const Symbol* _top;
  TermList* _arguments;
}; // class Input::Term
}; // namespace VK

#ifndef NO_DEBUG_VIS
ostream& operator<<(ostream& str,const VK::Input::Term* t);
#endif

//======================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_INPUT
 #define DEBUG_NAMESPACE "Input::Literal"
#endif
#include "debugMacros.h"
//======================================================================

namespace VK
{
  using namespace BK;
class Input::Literal
{
 public:
  Literal(bool positive,Term* at) : 
    _isPositive(positive),
    _atom(at)
    {
    };
  ~Literal() 
    {
      CALL("destructor ~Literal()");
      if (DestructionMode::isThorough())
	{
	  delete _atom;
	};
    };   
  void* operator new(size_t)
    {
      CALL("operator new(size_t)");
      return Malloc::allocate(sizeof(Literal));
    };
  void operator delete(void* obj)
    {
      CALL("operator delete(void* obj)");
      Malloc::deallocate(obj,sizeof(Literal));
    };
  bool isPositive() const { return _isPositive; };
  const Term* atom() const { return _atom; };
 private:
  Literal() {}; 
 private:
  bool _isPositive;
  Term* _atom;
}; // class Input::Literal
}; // namespace VK

#ifndef NO_DEBUG_VIS

inline
ostream& operator<<(ostream& str,const VK::Input::Literal* lit)
{
  if (!lit->isPositive()) str << "~";
  return str << lit->atom();
};

#endif

//======================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_INPUT
 #define DEBUG_NAMESPACE "Input::Clause"
#endif
#include "debugMacros.h"
//======================================================================


namespace VK
{
  using namespace BK;

class Input::Clause
{
 public:
  enum Category { Unclassified, Axiom, Conjecture, Hypothesis };
 public:
  Clause(const char* nm,LiteralList* lits) : 
    _literals(lits), 
    _category(Unclassified) 
    {
      _name = (char*)Malloc::allocate(strlen(nm) + 1);
      strcpy(_name,nm);
    };
  ~Clause()
    {
      CALL("destructor ~Clause()");
      if (DestructionMode::isThorough())
	{
	  destroyLiteralList(_literals);
	  Malloc::deallocate((void*)_name,strlen(_name) + 1);
	};
    };
  void* operator new(size_t)
    {
      CALL("operator new(size_t)");
      return Malloc::allocate(sizeof(Clause));
    };
  void operator delete(void* obj)
    {
      CALL("operator delete(void* obj)");
      Malloc::deallocate(obj,sizeof(Clause));
    }; 
  const LiteralList* literals() const { return _literals; };
  const Category& getCategory() const { return _category; };
  void setCategory(const Category& cat) { _category = cat; };
  const char* name() const { return _name; };
 private:
  Clause() {};
 private:
  LiteralList* _literals;
  Category _category;
  char* _name;
}; // class Input::Clause

}; // namespace VK



#ifndef NO_DEBUG_VIS
ostream& operator<<(ostream& str,const VK::Input::Clause* cl);
#endif


//======================================================================
#endif
