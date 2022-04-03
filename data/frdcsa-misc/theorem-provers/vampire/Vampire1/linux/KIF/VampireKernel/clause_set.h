#ifndef CLAUSE_SET_H
//===================================================================
#define CLAUSE_SET_H
#ifndef NO_DEBUG_VIS
#include <iostream.h>
#endif
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "Clause.h"
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_CLAUSE_SET
#define DEBUG_NAMESPACE "CLAUSE_SET"
#endif
#include "debugMacros.h"
//============================================================================
namespace VK
{

class CLAUSE_SET 
{
 private: 
  Clause clauses;
  Clause* current;
  long numOfClauses;
  long removed;
#ifdef DEBUG_NAMESPACE
  const char* name;
#endif
 public:
  bool Empty() const { return (clauses.Next() == (&clauses)); };
  bool Nonempty() const { return (clauses.Next() != (&clauses)); }; 
  CLAUSE_SET() :  
    numOfClauses(0L), 
    removed(0L)
#ifdef DEBUG_NAMESPACE
    , name("?")
#endif
    {
    };

  CLAUSE_SET(const char* nm) : 
    numOfClauses(0L), 
    removed(0L)
#ifdef DEBUG_NAMESPACE
    , name(nm)
#endif
    {
    };
  ~CLAUSE_SET() { CALL("destructor ~CLAUSE_SET()"); };
#ifndef NO_DEBUG
  bool clean() const
    {
      if (clauses.Next() != &clauses) 
	{

	  DF; cout << "NEXT\n";

	  return false;
	};  
      if (clauses.Previous() != &clauses) 
	{
	  DF; cout << "PREVIOUS\n";
	  return false;
	}; 
      if (numOfClauses)
	{
	  DF; cout << "NUM\n";
	  return false;
	};
      if (removed) 
	{
	  DF; cout << "REMOVED\n";
	  return false;
	};
      return true; 
    };
#endif

  void Insert(Clause* c) { numOfClauses++; Clause::InsertAfter(c,&clauses); };
  void Remove(Clause* c) 
    {
      CALLM(name,"Remove(Clause* c)"); 
      numOfClauses--; 
      ASSERT(numOfClauses >= 0L);
      removed++;
      COP("A10"); 
      Clause::Remove(c); 
    };
  long Size() const { return numOfClauses; };
  long Removed() const { return removed; };
  long Total() const { return (Size() + Removed()); };
  void ResetIterator() 
    {
      current = Empty() ? ((Clause*)0) : clauses.Next(); 
    };
  Clause* Next() 
    {
      CALLM(name,"Next()");
      Clause* res = current;
      if (current)
	{
	  current = current->Next();
	  if (current == (&clauses)) { current = (Clause*)0; };
	};
      return res;
    };
  Clause* Top() 
    {
      CALLM(name,"Top()");
      if (Empty()) return (Clause*)0;     
      return clauses.Next();
    };

  Clause* Pop()
    {
      CALLM(name,"Pop()");
      ASSERT(Nonempty());
      Clause* c = clauses.Next();
      //Clause::Remove(c);
      Remove(c);
      return c;
    };
   
  void Purge()
    {
      CALL("Purge()");
      while (!Empty())
	{
	  Pop();
	};
      ASSERT(numOfClauses == 0UL);
      removed = 0L;
    };
    
  void DestroyAllClauses()
    {
      CALLM(name,"DestroyAllClauses()");
      while (!Empty())
	{
	  Pop()->Destroy();
	};
      removed = 0L;
    };

#ifndef NO_DEBUG
  bool contains(const Clause* cl) const
    {
      for (const Clause* c = clauses.Next();
	   c != &clauses;
	   c = c->Next())
	if (c == cl) return true;
      return false;
    };
#endif

   
#ifndef NO_DEBUG_VIS
  ostream& outputClauses(ostream& str)
    {
      ResetIterator();
      for (Clause* c = Next(); c; c = Next()) str << c << '\n';
      return str;
    };
#endif
}; // class CLAUSE_SET
}; // namespace VK

//============================================================================
#undef DEBUG_NAMESPACE
//===================================================================
#endif








 
