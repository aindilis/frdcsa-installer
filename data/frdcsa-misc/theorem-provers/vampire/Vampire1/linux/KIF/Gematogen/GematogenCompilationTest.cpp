//
// File:         GematogenCompilationTest.cpp
// Description:  File for testing compilation of the Gematogen library.
// Created:      Jan 28, 2001, 17:40
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//====================================================
#include <iostream.h>
#include "jargon.h"
#include "ClassDesc.h"
#include "ObjDesc.h"
#include "ForwardMatchingIndex.h"
#include "InstanceRetrievalIndex.h"
//====================================================

// Dummy classes and values to be used as parameters of the templates 

namespace GematogenCompilationTest
{
  using namespace Gematogen;
    
  // Common stuff
  
 const ulong MaxTermSize = 1024;
 const ulong MaxTermDepth = 1024;
 const ulong MaxNumOfVariables = 1024;
 const ulong MaxNumOfFunctors = 1024;
 const ulong MaxNumOfTermArguments = 31;

 class Allocator
 {
  public:
   static void* allocate(size_t size); 
   static void deallocate(void* obj,size_t size); 
   #ifndef NO_DEBUG
    static void* allocate(size_t size,ClassDesc* classDesc); 
    static void deallocate(void* obj,size_t size,ClassDesc* classDesc); 
    static ObjDesc* hostObj(void* ptr); 
   #endif
  private:
   ulong _dummy;
 };

 class Symbol
 {
  public:
   typedef ulong Arity;
   typedef ulong Functor;
   typedef ulong Polarity;
  public:   
   Symbol() {};
   Symbol(Arity arity,Functor functor,Polarity polarity) {}; // creates a functor  
   ulong functor() const { return 0UL; };   // returns functor number
   ulong var() const { return 0UL; }; // returns variable number
   Arity arity() const { return (Arity)1; };
   bool isComplex() const { return true; }; // symbol.isComplex() <=> symbol is a function 
   bool isVariable() const { return true; }; // symbol.isVariable() <=> symbol is a variable
   bool isNotVariable() const { return true; }; // symbol.isNotVariable() <=> symbol is not a variable, 
                                                // symbol.isNotVariable() is not necessarily the same as symbol.isComplex()
                                                // although symbol.isComplex() => symbol.isNotVariable()
   bool operator==(const Symbol& t) const { return true; };
   bool operator>(const Symbol& t) const { return true; }; 
   bool operator<(const Symbol& t) const { return true; };
   static Arity maxArity() { return MaxNumOfTermArguments; };  
    
  public:
   static const Polarity PositivePolarity = 0UL;
   static const Polarity NegativePolarity = 1UL;
  private:
   ulong _dummy;  
 };
 class Flatterm
 {
  public:  
   const Symbol& symbolRef() const { return _sym; };
   bool equal(const Flatterm* t) const { return true; };
   const Flatterm* after() const { return this; };
   const Flatterm* next() const { return (this + 1); };
  private:
   Symbol _sym;
 };

 typedef Symbol* TermId;

  // Testing forward matching indexing module(s)

 class ForwardMatchingIndexTreeLeafInfo
 {
  private: 
   ulong _dummy;
 };
 class ForwardMatchingIndexInstanceId
 {
  private: 
   ulong _dummy;
 };
 
 ForwardMatchingIndex<MaxTermSize,MaxNumOfVariables,MaxNumOfFunctors,Allocator,Symbol,Flatterm,ForwardMatchingIndexTreeLeafInfo,ForwardMatchingIndexInstanceId> forwardMatchingIndex;
 
 void forwardMatchingIndexMethodsTest()
 {
  Symbol fun;
  Symbol var;
  
  // Integration 
  forwardMatchingIndex.integrationReset(fun);
  forwardMatchingIndex.integrationFunc(fun);
  forwardMatchingIndex.integrationVar(var);
  ForwardMatchingIndexTreeLeaf<MaxTermSize,MaxNumOfVariables,Allocator,Symbol,Flatterm,ForwardMatchingIndexTreeLeafInfo,ForwardMatchingIndexInstanceId>* leaf;
  bool newLeaf;
  leaf = forwardMatchingIndex.integrationGetLeaf(newLeaf);
  ulong num1 = forwardMatchingIndex.integrationGetVariables().Size();
  num1 = num1 + 1; // to suppress warnings
  const Flatterm** ptr1 = forwardMatchingIndex.integrationMapVariable(var.var());
  ptr1 = ptr1 + 1; // to suppress warnings

  // Retrieval
  Flatterm query[3];
  leaf = forwardMatchingIndex.Match(query);
  leaf = forwardMatchingIndex.MatchAgain();
  
  // Removal 
  forwardMatchingIndex.removalReset(fun);
  forwardMatchingIndex.removalFunc(fun);  
  forwardMatchingIndex.removalVar(var);
  ForwardMatchingIndexTreeInstruction<MaxTermSize,MaxNumOfVariables,Allocator,Symbol,Flatterm,ForwardMatchingIndexTreeLeafInfo,ForwardMatchingIndexInstanceId>** branchToDestroy;
  forwardMatchingIndex.removalFinish(leaf,branchToDestroy);
 
  // Destruction
    
  forwardMatchingIndex.Destroy();
 }; // forwardMatchingIndexMethodsTest()



   // Testing instance retrieval indexing module(s)
  
 InstanceRetrievalIndex<Allocator,Symbol,TermId,MaxTermDepth,MaxTermSize> instanceRetrievalIndex; 


 void instanceRetrievalIndexMethodsTest()
 { 
  Symbol f;
  Symbol g;
  Symbol c;
  Symbol v;
  TermId t1 = &f;
  TermId t2 = &g;
  TermId t3 = &c;
  TermId t4 = &v;


  // Integration 

  instanceRetrievalIndex.integrationReset(t1,f);
  instanceRetrievalIndex.integrationNonconstFunc(t2,g);
  instanceRetrievalIndex.integrationConstant(t3,c);
  instanceRetrievalIndex.integrationVariable(t4);
  instanceRetrievalIndex.integrationDone();

  // Retrieval

  // Removal 
  instanceRetrievalIndex.removalReset(t1,f);
  instanceRetrievalIndex.removalNonconstFunc(g);
  instanceRetrievalIndex.removalConstant(c); 
  instanceRetrievalIndex.removalVariable();  
  instanceRetrievalIndex.removalAfter();
  instanceRetrievalIndex.removalDone();

  // Destruction
    
 }; // void forwardMatchingIndexMethodsTest() 
  
 
}; // namespace GematogenCompilationTest
 

//====================================================
