//
// File:         InstanceRetrievalWithPathIndex.h
// Description:  Indexing for instance retrieval
//               based on path-indexing + database joins. 
// Created:      Jul 31, 2001, 21:00
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
// Note:         This file is a part of the Gematogen library.
//============================================================================
#ifndef INSTANCE_RETRIEVAL_WITH_PATH_INDEX_H
//=============================================================================
      // Uncomment any of the definitions to switch off
      // the corresponding optimisations

//#define INSTANCE_RETRIEVAL_INDEX_NO_SKIP_LISTS
//#define INSTANCE_RETRIEVAL_INDEX_BIG_HEADER_SKIP_LISTS
//#define INSTANCE_RETRIEVAL_INDEX_NO_SECONDARY_POSITIONS 
//#define INSTANCE_RETRIEVAL_INDEX_NO_PATH_RELATION_SORTING

//============================================================================= 
#define INSTANCE_RETRIEVAL_INDEX_H 
#ifndef NO_DEBUG_VIS
 #include <iostream.h>
#endif 
#include "jargon.h"
#include "Gematogen.h"
#include "GematogenDebugFlags.h" 
#include "RuntimeError.h"
#include "Stack.h"
#include "PathIndex.h"
#ifdef INSTANCE_RETRIEVAL_INDEX_NO_SKIP_LISTS
 #include "OrdList.h"
#else 
 #include "SkipList.h"
 #include "SmallHeaderSkipList.h"
#endif
#include "GIncCountingSort.h"
#include "DestructionMode.h"
//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_INSTANCE_RETRIEVAL_WITH_PATH_INDEX
 #define DEBUG_NAMESPACE "InstanceRetrievalIndex<Alloc,Symbol,TermId,MaxTermDepth,MaxTermSize,MaxNumOfVariables>"
#endif
#include "debugMacros.h"
//============================================================================ 

namespace Gematogen
{
template <class Alloc,class Symbol,class TermId,ulong MaxTermDepth,ulong MaxTermSize,ulong MaxNumOfVariables>
class InstanceRetrievalIndex
{ 
 public:
  InstanceRetrievalIndex() 
   : _trie(),
    _integrator(),
    _removal(),
    _destruction(),
    _pathRelationRetrieval()
    #ifndef INSTANCE_RETRIEVAL_INDEX_NO_PATH_RELATION_SORTING
    , _pathSorting()
    #endif
  {
  };
  ~InstanceRetrievalIndex() 
    {
      CALL("destructor ~InstanceRetrievalIndex()");
      if (DestructionMode::isThorough()) destroy();
    };
  // Integration of a new term
  void integrationReset(const TermId& integratedTermId,const Symbol& topFunc);
  void integrationNonconstFunc(const TermId& subtermId,const Symbol& f);
  void integrationConstant(const TermId& subtermId,const Symbol& c); 
  void integrationVariable(const TermId& subtermId);  
  bool integrationDone(); 
   // returns true <=> at least one new occurence was really added to the index
  
  // Removal of a term
  
  bool removalReset(const TermId& removedTermId,const Symbol& topFunc);
  bool removalNonconstFunc(const Symbol& f);
  bool removalConstant(const Symbol& c); 
  void removalVariable();  
  void removalAfter();
  bool removalDone();

  // Destroying the whole index 
    
  void destroy();  

  // Compiling the query
  
  bool queryReset(const Symbol& topFunc);
  bool queryNonconstFunc(const Symbol& f);  
  bool queryConstant(const Symbol& c);
  bool queryVariable(ulong var);
  bool queryEnd();

  // Retrieval 
 
  bool nextMatch(TermId& matchId); 
  const TermId& subst(ulong var) const { return _substitution[var]; };

  // Implementation info
  
  static const char* about() 
  { 
   return GEMATOGEN_VERSION 
          ", simple inst. ret. based on path-ind. and DB joins" 
         #ifdef INSTANCE_RETRIEVAL_INDEX_NO_SKIP_LISTS
          " - skip-lists (ordinary sorted lists are used instead)" 
         #endif
         #ifdef INSTANCE_RETRIEVAL_INDEX_BIG_HEADER_SKIP_LISTS
          " + constant-size headers in skip-lists" 
         #endif
         #ifdef INSTANCE_RETRIEVAL_INDEX_NO_SECONDARY_POSITIONS 
          " - spec. treatment of secondary positions" 
         #endif
         #ifdef INSTANCE_RETRIEVAL_INDEX_NO_PATH_RELATION_SORTING
          " - sorting of path-relations" 
         #endif
          ;
  };   

 private:
  #ifndef INSTANCE_RETRIEVAL_INDEX_NO_SKIP_LISTS
   #ifdef INSTANCE_RETRIEVAL_INDEX_BIG_HEADER_SKIP_LISTS
    typedef VoidSkipList<Alloc,TermId,2,28,InstanceRetrievalIndex> ClosedPathRelation; 
    typedef SkipList<Alloc,TermId,TermId,2,28,InstanceRetrievalIndex> OpenPathRelation; 
   #else
    typedef SmallHeaderVoidSkipList<Alloc,TermId,2,InstanceRetrievalIndex> ClosedPathRelation;  
    typedef SmallHeaderSkipList<Alloc,TermId,TermId,2,InstanceRetrievalIndex> OpenPathRelation;   
   #endif 
  #else
   // Using ordered lists instead of skip lists
   typedef VoidOrdList<Alloc,TermId,InstanceRetrievalIndex> ClosedPathRelation; 
   typedef OrdList<Alloc,TermId,TermId,InstanceRetrievalIndex> OpenPathRelation;
  #endif
  
   
  typedef PathIndex<Alloc,Symbol,OpenPathRelation,ClosedPathRelation,MaxTermDepth,MaxTermSize> PathIndexForInstanceRetrieval;
 
  class AbstractMachineInstruction
  {
   public: 
    enum Tag
    {
       // Atomary actions
     OpenEqualiseTermId,
     ClosedEqualiseTermId,
     OpenFindTermId,
     OpenLoadSubtermId,
     OpenCompare,
     Success,
       // Combined actions
     OpenFindTermIdAndLoadSubtermId,
     OpenFindTermIdAndCompare
    };
   public:
    AbstractMachineInstruction() {}; 
    ~AbstractMachineInstruction() {}; 

    #ifndef NO_DEBUG_VIS
     static ostream& output(ostream& str,const Tag& tag);
     ostream& output(ostream& str,const TermId* substitution) const;
    #endif
   public:
    Tag tag;
    union
     {
      OpenPathRelation::Surfer* openPathRelationSurfer;
      ClosedPathRelation::Surfer* closedPathRelationSurfer;
      const OpenPathRelation::Node* const * openPathCurrNode;
     };
    TermId* subst; 
    #ifdef DEBUG_NAMESPACE
     ulong debugInputPathNum;     
    #endif
  }; // class AbstractMachineInstruction

 private: 
  #ifndef NO_DEBUG_VIS
   ostream& outputMachineCode(ostream& str);
  #endif
 private:
  PathIndexForInstanceRetrieval::Trie _trie; 
 
  // Integration
  PathIndexForInstanceRetrieval::Integrator _integrator;
  TermId _integratedTermId;
  bool _integrationSuccessful; // <=> at least one new occurence was really added to the index

  // Removal
  PathIndexForInstanceRetrieval::Removal _removal;
  TermId _removedTermId;
  bool _removalSuccessful; // <=> at least one occurence was really found in the index

  // Destruction
  PathIndexForInstanceRetrieval::Destruction _destruction;

   // Compiling the query
  PathIndexForInstanceRetrieval::Retrieval _pathRelationRetrieval;
  ulong _nextInputPath;  
  bool _inputPathIsOpen[MaxTermSize];
  OpenPathRelation* _openPathRelation[MaxTermSize];
  bool _openPathIsPrimary[MaxTermSize];
  ulong _openPathVariable[MaxTermSize];
  ClosedPathRelation* _closedPathRelation[MaxTermSize];
 
  #ifndef INSTANCE_RETRIEVAL_INDEX_NO_SECONDARY_POSITIONS
   bool _primaryPositionPossible;
   ulong _numOfVarsBeforePrimary;  
  #endif
  
  #ifndef INSTANCE_RETRIEVAL_INDEX_NO_PATH_RELATION_SORTING
   GSimpleIncCountingSort<ulong,MaxTermSize> _pathSorting;
  #else
   ulong _numOfPrimaryPositions;
  #endif
  bool _substLoaded[MaxNumOfVariables];
  OpenPathRelation::Surfer _openPathRelationSurfer[MaxTermSize];
  ClosedPathRelation::Surfer _closedPathRelationSurfer[MaxTermSize]; 

  // Substitution

  TermId _substitution[MaxNumOfVariables];

  // Abstract machine memory
  AbstractMachineInstruction _machineCode[MaxTermSize*2 + 1];
  bool _firstRetrievalCall;
}; // class InstanceRetrievalIndex<..>

}; // namespace Gematogen

//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_INSTANCE_RETRIEVAL_WITH_PATH_INDEX
 #define DEBUG_NAMESPACE "InstanceRetrievalIndex<Alloc,Symbol,TermId,MaxTermDepth,MaxTermSize,MaxNumOfVariables>"
#endif
#include "debugMacros.h"
//============================================================================

namespace Gematogen
{
          // Integration of a new term

template <class Alloc,class Symbol,class TermId,ulong MaxTermDepth,ulong MaxTermSize,ulong MaxNumOfVariables>
inline void InstanceRetrievalIndex<Alloc,Symbol,TermId,MaxTermDepth,MaxTermSize,MaxNumOfVariables>::integrationReset(const TermId& integratedTermId,const Symbol& topFunc)
{
 CALL("integrationReset(const TermId& integratedTermId,const Symbol& topFunc)");
 _integratedTermId = integratedTermId;
 _integrator.reset(_trie);
 _integrator.nonconstFunc(topFunc);
 _integrationSuccessful = false;
}; // void InstanceRetrievalIndex<..>::integrationReset(const TermId& integratedTermId,const Symbol& topFunc)


template <class Alloc,class Symbol,class TermId,ulong MaxTermDepth,ulong MaxTermSize,ulong MaxNumOfVariables>
inline void InstanceRetrievalIndex<Alloc,Symbol,TermId,MaxTermDepth,MaxTermSize,MaxNumOfVariables>::integrationNonconstFunc(const TermId& subtermId,const Symbol& f)
{
 CALL("integrationNonconstFunc(const TermId& subtermId,const Symbol& f)");
 bool newNode;
 OpenPathRelation::Node* node = _integrator.indexedObj().insert(_integratedTermId,newNode);
 if (newNode) _integrationSuccessful = true;
 node->value() = subtermId;
 _integrator.nonconstFunc(f); 
}; //  void InstanceRetrievalIndex<..>::integrationNonconstFunc(const TermId& subtermId,const Symbol& f)

template <class Alloc,class Symbol,class TermId,ulong MaxTermDepth,ulong MaxTermSize,ulong MaxNumOfVariables>
inline void InstanceRetrievalIndex<Alloc,Symbol,TermId,MaxTermDepth,MaxTermSize,MaxNumOfVariables>::integrationConstant(const TermId& subtermId,const Symbol& c)
{
 CALL("integrationConstant(const TermId& subtermId,const Symbol& c)");
 bool newNode;
 OpenPathRelation::Node* node = _integrator.indexedObj().insert(_integratedTermId,newNode);
 if (newNode) _integrationSuccessful = true;
 node->value() = subtermId;
 _integrator.constant(c);
 if (_integrator.constIndObj().insert(_integratedTermId))
  _integrationSuccessful = true; 
}; // void InstanceRetrievalIndex<..>::integrationConstant(const TermId& subtermId,const Symbol& c)

template <class Alloc,class Symbol,class TermId,ulong MaxTermDepth,ulong MaxTermSize,ulong MaxNumOfVariables>
inline void InstanceRetrievalIndex<Alloc,Symbol,TermId,MaxTermDepth,MaxTermSize,MaxNumOfVariables>::integrationVariable(const TermId& subtermId)  
{
 CALL("integrationVariable()"); 
 bool newNode;
 OpenPathRelation::Node* node = _integrator.indexedObj().insert(_integratedTermId,newNode);
 if (newNode) _integrationSuccessful = true;
 node->value() = subtermId; 
 _integrator.variable();
}; // void InstanceRetrievalIndex<..>::integrationVariable()  

template <class Alloc,class Symbol,class TermId,ulong MaxTermDepth,ulong MaxTermSize,ulong MaxNumOfVariables>
inline bool InstanceRetrievalIndex<Alloc,Symbol,TermId,MaxTermDepth,MaxTermSize,MaxNumOfVariables>::integrationDone()
{
 CALL("integrationDone()");
 return _integrationSuccessful;
}; // bool InstanceRetrievalIndex<..>::integrationDone()



         // Removal of a term


template <class Alloc,class Symbol,class TermId,ulong MaxTermDepth,ulong MaxTermSize,ulong MaxNumOfVariables>
inline bool InstanceRetrievalIndex<Alloc,Symbol,TermId,MaxTermDepth,MaxTermSize,MaxNumOfVariables>::removalReset(const TermId& removedTermId,const Symbol& topFunc)
{
 CALL("removalReset(const TermId& removedTermId,const Symbol& topFunc)");
 _removedTermId = removedTermId;
 _removal.reset(_trie);
 _removalSuccessful = false;
 return _removal.nonconstFunc(topFunc);
}; // bool InstanceRetrievalIndex<..>::removalReset(const TermId& removedTermId,const Symbol& topFunc)

template <class Alloc,class Symbol,class TermId,ulong MaxTermDepth,ulong MaxTermSize,ulong MaxNumOfVariables>
inline bool InstanceRetrievalIndex<Alloc,Symbol,TermId,MaxTermDepth,MaxTermSize,MaxNumOfVariables>::removalNonconstFunc(const Symbol& f)
{
 CALL("removalNonconstFunc(const Symbol& f)");
 if (_removal.occurences()) 
  {
   TermId dummySubtermId; 
   if (_removal.indexedObj().remove(_removedTermId,dummySubtermId))
    _removalSuccessful = true; 
  };  
 return _removal.nonconstFunc(f);
}; // bool InstanceRetrievalIndex<..>::removalNonconstFunc(const Symbol& f)

template <class Alloc,class Symbol,class TermId,ulong MaxTermDepth,ulong MaxTermSize,ulong MaxNumOfVariables>
inline bool InstanceRetrievalIndex<Alloc,Symbol,TermId,MaxTermDepth,MaxTermSize,MaxNumOfVariables>::removalConstant(const Symbol& c)
{
 CALL("removalConstant(const Symbol& c)");
 if (_removal.occurences()) 
  {
   TermId dummySubtermId;   
   if (_removal.indexedObj().remove(_removedTermId,dummySubtermId))
    _removalSuccessful = true;
  };  
 if (_removal.constant(c))
  {
   if (_removal.constIndObj().remove(_removedTermId))
    _removalSuccessful = true;
   return true;
  }; 
 return false; 
}; // bool InstanceRetrievalIndex<..>::removalConstant(const Symbol& c)

template <class Alloc,class Symbol,class TermId,ulong MaxTermDepth,ulong MaxTermSize,ulong MaxNumOfVariables>
inline void InstanceRetrievalIndex<Alloc,Symbol,TermId,MaxTermDepth,MaxTermSize,MaxNumOfVariables>::removalVariable()
{
 CALL("removalVariable()");
 if (_removal.occurences()) 
  {
   TermId dummySubtermId;   
   if (_removal.indexedObj().remove(_removedTermId,dummySubtermId))
    _removalSuccessful = true;
  };  
 _removal.after();
}; // void InstanceRetrievalIndex<..>::removalVariable()

template <class Alloc,class Symbol,class TermId,ulong MaxTermDepth,ulong MaxTermSize,ulong MaxNumOfVariables>
inline void InstanceRetrievalIndex<Alloc,Symbol,TermId,MaxTermDepth,MaxTermSize,MaxNumOfVariables>::removalAfter()
{
 CALL("removalAfter()");
 _removal.after();
}; // void InstanceRetrievalIndex<..>::removalAfter()
  
template <class Alloc,class Symbol,class TermId,ulong MaxTermDepth,ulong MaxTermSize,ulong MaxNumOfVariables>
inline bool InstanceRetrievalIndex<Alloc,Symbol,TermId,MaxTermDepth,MaxTermSize,MaxNumOfVariables>::removalDone()
{
 CALL("removalDone()");
 _removal.recycleObsoleteNodes();
 return _removalSuccessful;
}; // bool InstanceRetrievalIndex<..>::removalDone()


               // Total destruction

template <class Alloc,class Symbol,class TermId,ulong MaxTermDepth,ulong MaxTermSize,ulong MaxNumOfVariables>
inline void InstanceRetrievalIndex<Alloc,Symbol,TermId,MaxTermDepth,MaxTermSize,MaxNumOfVariables>::destroy()
{
 CALL("destroy()");
 bool wasConstant;
 if (_destruction.reset(_trie,wasConstant))
  {
   do
    { 
     if (wasConstant)
      {
       _destruction.constIndObj().destroy(); 
      }
     else // !wasConstant
      {
       _destruction.indexedObj().destroy();
      }; 
    }
   while (_destruction.next(wasConstant));
  _destruction.recycle(); 
  };
}; // bool InstanceRetrievalIndex<..>::destroy()
 
             // Compiling the query
  
template <class Alloc,class Symbol,class TermId,ulong MaxTermDepth,ulong MaxTermSize,ulong MaxNumOfVariables>
inline bool InstanceRetrievalIndex<Alloc,Symbol,TermId,MaxTermDepth,MaxTermSize,MaxNumOfVariables>::queryReset(const Symbol& topFunc)
{
 CALL("queryReset(const Symbol& topFunc)");
 ASSERT(topFunc.arity() > 0);
 _nextInputPath = 0UL;

 #ifndef INSTANCE_RETRIEVAL_INDEX_NO_PATH_RELATION_SORTING
  _pathSorting.reset();
 #else
  _numOfPrimaryPositions = 0UL;
 #endif 

 #ifndef INSTANCE_RETRIEVAL_INDEX_NO_SECONDARY_POSITIONS
  _primaryPositionPossible = true;
  _numOfVarsBeforePrimary = topFunc.arity() - 1;  
 #endif

 return _pathRelationRetrieval.reset(_trie) 
        && _pathRelationRetrieval.nonconstFunc(topFunc);


} // bool InstanceRetrievalIndex<..>::queryReset(const Symbol& topFunc)
  
template <class Alloc,class Symbol,class TermId,ulong MaxTermDepth,ulong MaxTermSize,ulong MaxNumOfVariables>
inline bool InstanceRetrievalIndex<Alloc,Symbol,TermId,MaxTermDepth,MaxTermSize,MaxNumOfVariables>::queryNonconstFunc(const Symbol& f)
{
 CALL("queryNonconstFunc(const Symbol& f)");
 if (!_pathRelationRetrieval.nonconstFunc(f)) return false;
 
 #ifndef INSTANCE_RETRIEVAL_INDEX_NO_SECONDARY_POSITIONS
  _primaryPositionPossible = true;
  _numOfVarsBeforePrimary = f.arity() - 1;  
 #endif

 return true; 
} // bool InstanceRetrievalIndex<..>::queryNonconstFunc(const Symbol& f)

template <class Alloc,class Symbol,class TermId,ulong MaxTermDepth,ulong MaxTermSize,ulong MaxNumOfVariables>
inline bool InstanceRetrievalIndex<Alloc,Symbol,TermId,MaxTermDepth,MaxTermSize,MaxNumOfVariables>::queryConstant(const Symbol& c)
{
 CALL("queryConstant(const Symbol& c)");
 if (!_pathRelationRetrieval.constant(c)) return false;
 _closedPathRelation[_nextInputPath] = _pathRelationRetrieval.constIndObjAddr();
 if (!(*(_closedPathRelation[_nextInputPath]))) return false;   

 _inputPathIsOpen[_nextInputPath] = false;

 #ifndef INSTANCE_RETRIEVAL_INDEX_NO_PATH_RELATION_SORTING
  _pathSorting.push(_closedPathRelation[_nextInputPath]->size());
 #else
  _numOfPrimaryPositions++;
 #endif 

 _nextInputPath++;
 #ifndef INSTANCE_RETRIEVAL_INDEX_NO_SECONDARY_POSITIONS
  _primaryPositionPossible = false; // the constant covers everything
 #endif

 return true; 
} // bool InstanceRetrievalIndex<..>::queryConstant(const Symbol& c)

template <class Alloc,class Symbol,class TermId,ulong MaxTermDepth,ulong MaxTermSize,ulong MaxNumOfVariables>
inline bool InstanceRetrievalIndex<Alloc,Symbol,TermId,MaxTermDepth,MaxTermSize,MaxNumOfVariables>::queryVariable(ulong var)
{
 CALL("queryVariable(ulong var)");
 _openPathRelation[_nextInputPath] = _pathRelationRetrieval.indexedObjAddr();
 if (!(*(_openPathRelation[_nextInputPath]))) return false;

 #ifdef INSTANCE_RETRIEVAL_INDEX_NO_SECONDARY_POSITIONS 
  _openPathIsPrimary[_nextInputPath] = true; 
  
  #ifndef INSTANCE_RETRIEVAL_INDEX_NO_PATH_RELATION_SORTING
   _pathSorting.push(_openPathRelation[_nextInputPath]->size()); // only for primary paths
  #else
   _numOfPrimaryPositions++;
  #endif

 #else
  if (_primaryPositionPossible) 
   {
    if (_numOfVarsBeforePrimary)
     {
      _numOfVarsBeforePrimary--;
      _openPathIsPrimary[_nextInputPath] = false; // will be covered later
     }
    else // must be primary
     {
      _primaryPositionPossible = false;
      _openPathIsPrimary[_nextInputPath] = true;

      #ifndef INSTANCE_RETRIEVAL_INDEX_NO_PATH_RELATION_SORTING
       _pathSorting.push(_openPathRelation[_nextInputPath]->size()); // only for primary paths
      #else
       _numOfPrimaryPositions++;
      #endif
     };
   }
  else // already covered by something
   {
   _openPathIsPrimary[_nextInputPath] = false; 
   };
 #endif

 _inputPathIsOpen[_nextInputPath] = true;
 _openPathVariable[_nextInputPath] = var;
 _nextInputPath++;
 _pathRelationRetrieval.after();
 _substLoaded[var] = false;
 return true;
} // bool InstanceRetrievalIndex<..>::queryVariable(ulong var)

template <class Alloc,class Symbol,class TermId,ulong MaxTermDepth,ulong MaxTermSize,ulong MaxNumOfVariables>
inline bool InstanceRetrievalIndex<Alloc,Symbol,TermId,MaxTermDepth,MaxTermSize,MaxNumOfVariables>::queryEnd()
{
 CALL("queryEnd()"); 

 //DF; _pathSorting.output(cout << "\n") << "\n";

 // Writing the code
 
 #ifndef INSTANCE_RETRIEVAL_INDEX_NO_PATH_RELATION_SORTING
  AbstractMachineInstruction* nextSubstInstr = _machineCode + _pathSorting.size();
 #else
  AbstractMachineInstruction* nextSubstInstr = _machineCode + _numOfPrimaryPositions;
 #endif


 AbstractMachineInstruction* currentTermIdEqualisingInstr;

 #ifndef INSTANCE_RETRIEVAL_INDEX_NO_PATH_RELATION_SORTING
  const ulong* posInSorting = _pathSorting.num();
 #else
  ulong currentPrimaryPosNum = 0;
 #endif

 for (ulong i = 0; i < _nextInputPath; i++)
  {
   if (_inputPathIsOpen[i])
    {
     ulong var = _openPathVariable[i];
     if (_openPathIsPrimary[i])
      {

       #ifndef INSTANCE_RETRIEVAL_INDEX_NO_PATH_RELATION_SORTING
        currentTermIdEqualisingInstr = _machineCode + (*posInSorting); 
        posInSorting++;
       #else
        currentTermIdEqualisingInstr = _machineCode + currentPrimaryPosNum;
        currentPrimaryPosNum++;
       #endif

       currentTermIdEqualisingInstr->tag = AbstractMachineInstruction::OpenEqualiseTermId;
       currentTermIdEqualisingInstr->openPathRelationSurfer = _openPathRelationSurfer + i;     
       currentTermIdEqualisingInstr->openPathRelationSurfer->reset(*(_openPathRelation[i]));

       DOP(currentTermIdEqualisingInstr->debugInputPathNum = i);
       if (_substLoaded[var])
	{
	 nextSubstInstr->tag = AbstractMachineInstruction::OpenCompare;
        }
       else
	{
	 _substLoaded[var] = true;
	 nextSubstInstr->tag = AbstractMachineInstruction::OpenLoadSubtermId;

        };
       nextSubstInstr->openPathCurrNode = currentTermIdEqualisingInstr->openPathRelationSurfer->currNodeAddr();
       nextSubstInstr->subst = _substitution + var;
       DOP(nextSubstInstr->debugInputPathNum = i);
       nextSubstInstr++;
      }
     else // secondary path
      { 
       if (_substLoaded[var])
	{
	 nextSubstInstr->tag = AbstractMachineInstruction::OpenFindTermIdAndCompare;
         
        }
       else 
	{ 
	 _substLoaded[var] = true;
	 nextSubstInstr->tag = AbstractMachineInstruction::OpenFindTermIdAndLoadSubtermId;

        };
       nextSubstInstr->openPathRelationSurfer = _openPathRelationSurfer + i;
       nextSubstInstr->openPathRelationSurfer->reset(*(_openPathRelation[i]));
       nextSubstInstr->subst = _substitution + var;
       DOP(nextSubstInstr->debugInputPathNum = i);
       nextSubstInstr++; 
      };
    }
   else // closed path
    {
     #ifndef INSTANCE_RETRIEVAL_INDEX_NO_PATH_RELATION_SORTING
      currentTermIdEqualisingInstr = _machineCode + (*posInSorting); 
      posInSorting++;
     #else
      currentTermIdEqualisingInstr = _machineCode + currentPrimaryPosNum;
      currentPrimaryPosNum++;
     #endif

     currentTermIdEqualisingInstr->tag = AbstractMachineInstruction::ClosedEqualiseTermId;
     currentTermIdEqualisingInstr->closedPathRelationSurfer = _closedPathRelationSurfer + i;
     currentTermIdEqualisingInstr->closedPathRelationSurfer->reset(*(_closedPathRelation[i]));
     DOP(currentTermIdEqualisingInstr->debugInputPathNum = i);
    };
  };
 nextSubstInstr->tag = AbstractMachineInstruction::Success;
 nextSubstInstr++;

 //DF; outputMachineCode(cout << "\n\n") << "\n\n";

 _firstRetrievalCall = true;
 return true; 
} // bool InstanceRetrievalIndex<..>::queryEnd()
 
 
      
              // Retrieval

template <class Alloc,class Symbol,class TermId,ulong MaxTermDepth,ulong MaxTermSize,ulong MaxNumOfVariables>
bool InstanceRetrievalIndex<Alloc,Symbol,TermId,MaxTermDepth,MaxTermSize,MaxNumOfVariables>::nextMatch(TermId& matchId)
{
 CALL("nextMatch(TermId& matchId)");
 AbstractMachineInstruction* instr = _machineCode;
 TermId termId; 
 
 // First initialise termId
 switch (instr->tag)
  {
   case AbstractMachineInstruction::OpenEqualiseTermId:

     //DF; cout << "InitId PrimVar " << _firstRetrievalCall << "\n";

    if (_firstRetrievalCall)
     {
      _firstRetrievalCall = false;
     }
    else
     {
      if (!instr->openPathRelationSurfer->next()) return false;
     };
    ASSERT(instr->openPathRelationSurfer->currNode());
    termId = instr->openPathRelationSurfer->currNode()->key();
    break;

   case AbstractMachineInstruction::ClosedEqualiseTermId:

     //DF; cout << "InitId PrimConst " << _firstRetrievalCall << "\n";

    if (_firstRetrievalCall)
     {
      _firstRetrievalCall = false;
     }
    else
     {
      if (!instr->closedPathRelationSurfer->next()) return false;
     };
    ASSERT(instr->closedPathRelationSurfer->currNode());
    termId = instr->closedPathRelationSurfer->currNode()->key();
    break;
   #ifdef DEBUG_NAMESPACE
    default: ICP("ICP0"); return false;
   #else
    #ifdef _SUPPRESS_WARNINGS_
     default:
      RuntimeError::report("Wrong instruction type in the beginning of the code in InstanceRetrievalIndex<..>::nextMatch(..)");
      return false;
    #endif
   #endif
  }; 
 
 next_instr:

  instr++;

  //DF; instr->output(cout << "INS ",_substitution) << "\n"; 

  perform_instr:

  //DF; instr->output(cout << "        ",_substitution) << "\n";    

   switch (instr->tag)
    {
     case AbstractMachineInstruction::OpenEqualiseTermId:

       //DF; cout << "Forward PrimVar\n";

      if (instr->openPathRelationSurfer->find(termId)) goto next_instr; 
      if (instr->openPathRelationSurfer->currNode())
       {
	termId = instr->openPathRelationSurfer->currNode()->key();
        instr = _machineCode;
	
	//DF; cout << "NotFound\n";

        goto perform_instr;
       }
      else 
       {

	 //DF; cout << "Failure\n";     

        return false;
       };

     case AbstractMachineInstruction::ClosedEqualiseTermId:

       //DF; cout << "Forward PrimConst\n";

      if (instr->closedPathRelationSurfer->find(termId)) goto next_instr; 
      if (instr->closedPathRelationSurfer->currNode())
       {
	termId = instr->closedPathRelationSurfer->currNode()->key();
        instr = _machineCode;

	//DF; cout << "NotFound\n";

        goto perform_instr;
       }
      else 
       {

	 //DF; cout << "Failure\n";     

        return false;
       }; 

     case AbstractMachineInstruction::OpenFindTermId:
      instr->openPathRelationSurfer->getTo(termId);
      goto next_instr;

     case AbstractMachineInstruction::OpenLoadSubtermId:
      ASSERT(*(instr->openPathCurrNode));
      *(instr->subst) = (*(instr->openPathCurrNode))->value();    
      goto next_instr;

     case AbstractMachineInstruction::OpenCompare:
      ASSERT(*(instr->openPathCurrNode));
      if ((*(instr->subst)) != (*(instr->openPathCurrNode))->value()) goto backtrack;
      goto next_instr;

     case AbstractMachineInstruction::Success: 
      matchId = termId;

      //DF; cout << "Success\n";

      return true; 
          
     case AbstractMachineInstruction::OpenFindTermIdAndLoadSubtermId: 
      instr->openPathRelationSurfer->getTo(termId);
      *(instr->subst) = instr->openPathRelationSurfer->currNode()->value();
      goto next_instr;

     case AbstractMachineInstruction::OpenFindTermIdAndCompare:
      instr->openPathRelationSurfer->getTo(termId);
      if ((*(instr->subst)) != instr->openPathRelationSurfer->currNode()->value()) goto backtrack; 
      goto next_instr;
 
     #ifdef DEBUG_NAMESPACE
      default: ICP("ICP1"); return false;
     #else
      #ifdef _SUPPRESS_WARNINGS_
       RuntimeError::report("Wrong instruction type in InstanceRetrievalIndex<..>::nextMatch(..)");
       return false;
      #endif
     #endif 
    };  
  
 ICP("ICP3");

 backtrack:
  instr = _machineCode;
  switch (instr->tag)
   {
    case AbstractMachineInstruction::OpenEqualiseTermId:


      //DF; cout << "Backtrack PrimVar\n";

     if (instr->openPathRelationSurfer->next()) 
      {
       ASSERT(instr->openPathRelationSurfer->currNode());
       termId = instr->openPathRelationSurfer->currNode()->key();
       goto next_instr;
      }
     else 
      { 

	//DF; cout << "Failure\n";     

       return false;
      };

    case AbstractMachineInstruction::ClosedEqualiseTermId:


      //DF; cout << "Backtrack PrimConst\n";


     if (instr->closedPathRelationSurfer->next())
      {
       ASSERT(instr->closedPathRelationSurfer->currNode());
       termId = instr->closedPathRelationSurfer->currNode()->key();
       goto next_instr;
      }
     else 
      {

	//DF; cout << "Failure\n";     

       return false;
      };

    #ifdef DEBUG_NAMESPACE
     default: ICP("ICP2"); return false;
    #else
     #ifdef _SUPPRESS_WARNINGS_
      default:
       RuntimeError::report("Wrong instruction type in InstanceRetrievalIndex<..>::nextMatch(..)::backtrack");
       return false;
     #endif
    #endif 
   };
 ICP("END");
}; //  bool InstanceRetrievalIndex<..>::nextMatch(TermId& matchId)





            // Debugging output

#ifndef NO_DEBUG_VIS
 template <class Alloc,class Symbol,class TermId,ulong MaxTermDepth,ulong MaxTermSize,ulong MaxNumOfVariables>
 inline ostream& InstanceRetrievalIndex<Alloc,Symbol,TermId,MaxTermDepth,MaxTermSize,MaxNumOfVariables>::outputMachineCode(ostream& str)
 {
  for (AbstractMachineInstruction* instr = _machineCode;
       instr->tag != AbstractMachineInstruction::Success;
       instr++)
   instr->output(str,_substitution) << "\n";          
  return str;
 } // ostream& InstanceRetrievalIndex<..>::outputMachineCode(ostream& str)

#endif


}; // namespace Gematogen

//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_INSTANCE_RETRIEVAL_WITH_PATH_INDEX
 #define DEBUG_NAMESPACE "InstanceRetrievalIndex<..>::AbstractMachineInstruction"
#endif
#include "debugMacros.h"
//============================================================================

namespace Gematogen
{
#ifndef NO_DEBUG_VIS
 template <class Alloc,class Symbol,class TermId,ulong MaxTermDepth,ulong MaxTermSize,ulong MaxNumOfVariables> 
 inline ostream& InstanceRetrievalIndex<Alloc,Symbol,TermId,MaxTermDepth,MaxTermSize,MaxNumOfVariables>::AbstractMachineInstruction::output(ostream& str,const Tag& tag)
 {
  switch (tag)
   {
    case OpenEqualiseTermId: return str << "OpenEqualiseTermId";
    case ClosedEqualiseTermId: return str << "ClosedEqualiseTermId";
    case OpenFindTermId: return str << "OpenFindTermId";
    case OpenLoadSubtermId: return str << "OpenLoadSubtermId";
    case OpenCompare: return str << "OpenCompare";
    case Success: return str << "Success";
    case OpenFindTermIdAndLoadSubtermId: return str << "OpenFindTermIdAndLoadSubtermId";
    case OpenFindTermIdAndCompare: return str << "OpenFindTermIdAndCompare";
    default: return str << "UnknownInstructionType";
   };
 } // ostream& InstanceRetrievalIndex<..>::AbstractMachineInstruction::output(ostream& str,const Tag& tag)

 template <class Alloc,class Symbol,class TermId,ulong MaxTermDepth,ulong MaxTermSize,ulong MaxNumOfVariables>    
 inline ostream& InstanceRetrievalIndex<Alloc,Symbol,TermId,MaxTermDepth,MaxTermSize,MaxNumOfVariables>::AbstractMachineInstruction::output(ostream& str,const TermId* substitution) const
 {
  output(str,tag) << " "; 
  switch (tag) 
   {
    case OpenEqualiseTermId:  
     DOP(str << "path#=" << debugInputPathNum << " ");
     break;
    case ClosedEqualiseTermId:  
     DOP(str << "path#=" << debugInputPathNum << " ");
     break; 
    case OpenFindTermId:   
     DOP(str << "path#=" << debugInputPathNum << " ");
     break;
    case OpenLoadSubtermId:
     DOP(str << "path#=" << debugInputPathNum << " ");
     str << "var#=" << subst - substitution;
     break;
    case OpenCompare:   
     DOP(str << "path#=" << debugInputPathNum << " ");
     str << "var#=" << subst - substitution;
     break;
    case Success:   
     DOP(str << "path#=" << debugInputPathNum << " ");
     break;
    case OpenFindTermIdAndLoadSubtermId:  
     DOP(str << "path#=" << debugInputPathNum << " ");
     str << "var#=" << subst - substitution;
     break;
    case OpenFindTermIdAndCompare:   
     DOP(str << "path#=" << debugInputPathNum << " ");
     str << "var#=" << subst - substitution;
     break;
    default: break;
   };
  return str;
 } // ostream& InstanceRetrievalIndex<..>::AbstractMachineInstruction::output(ostream& str,const TermId* substitution) const

#endif

}; // namespace Gematogen

//======================================================================
#undef DEBUG_NAMESPACE
//======================================================================
#endif
