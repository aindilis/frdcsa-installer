//
// File:         Master.h
// Description:  Main loop.
// Created:      Mar 5, 2000, 15:00
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//====================================================
#ifndef MASTER_H
//====================================================
#define MASTER_H
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "GlobAlloc.h"
#include "GlobalStopFlag.h"
#include "GlobalClock.h"
#include "clause_set.h"
#include "kept.h"
#include "sharing.h"
#include "ClauseAssembler.h"
#include "NewClause.h"
#include "ForwardSubsumptionFrontEnd.h"
#include "ComponentName.h"
#include "InferenceDispatcher.h"
#include "BackwardSubsumption.h"
#include "ForwardDemodulation.h"
#include "BackDemodulation.h"
#include "ClauseNumIndex.h"
#include "ClauseQueue.h"
#include "PagedPassiveClauseStorage.h"
#include "LiteralSelection.h"
#include "DefinedHeaderTable.h"
#include "DestructionMode.h"
namespace VK
{
  using namespace BK;
  using namespace Gematogen;
class Clause;
}; // namespace VK

//====================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_MASTER
#define DEBUG_NAMESPACE "Master"
#endif
#include "debugMacros.h"
//====================================================
namespace VK
{
  using namespace BK;
  using namespace Gematogen;

class SymbolInfo;

class Master
{
 public:
  enum TerminationReason 
  { 
    UnknownTR,
    Proof, 
    EmptyPassive, 
    MaxPassive, 
    MaxActive 
  }; 
  class Options
    {
    public:
      Options() { reset(); };
      void reset(); 
      // main algorithm
      bool incompleteAPriori;
      long main_alg;
      // inference rules
      bool binary_resolution;
      bool no_inferences_between_rules;
      bool splitting;
      long splitting_style;
      bool splitting_with_naming;
      long neg_eq_splitting;   
      bool paramodulation;
      long sim_back_sup;
      long selection;
      long literal_comparison_mode;
      bool inherited_negative_selection;

      // strategy switches
      bool handle_equality;
      bool forward_subsumption;
      bool fs_set_mode;
      bool fs_optimised_multiset_mode;
      bool fs_use_sig_filters;
      bool fs_old_style;
      long simplify_by_fs;
      long forward_demodulation;
      bool fd_on_splitting_branches;
      const SymbolInfo* normalize_by_commutativity;
      long backward_demodulation;
      bool bd_on_splitting_branches;
      bool simplify_by_eq_res;
      bool backward_subsumption;
      bool bs_set_mode;
      bool bs_optimised_multiset_mode;
      long orphan_murder;
      long lrs;
      const SymbolInfo* pred_elim_table;
      const bool* answer_predicate_table;
      ulong answer_predicate_table_size;
      // numeric options
      long age_weight_ratio;
      long timeLimit; // deciseconds
      float memoryLimit;
      float allocation_buffer_price; 
      long maxActive;
      long maxPassive;
      long maxWeight;
      long maxInferenceDepth;
      float first_time_check;
      float first_mem_check;
      // interface options
      ostream* outputStreamForGenerated;
      void (*showPassive) (const Clause*);        
      void (*showActive) (const Clause*); 
      void (*showWeightChange) (long,long);
      void (*showUsagePace) (float);
    }; // class Options

 public:
  Master() :
    _continuation(Continuation_Init),
    _terminated(false),
    _proof((Clause*)0),
    _reservedPassive("Master::_reservedPassive"),
    _active("Master::_active"),
    _history("Master::_history"),
    _answers("Master::_answers"),
    _infDispatcher(&_freshClause),
    _backSubsumption(&_clauseNumIndex),
    _backDemodulation(&_freshClause,&_sharingIndex,&_clauseNumIndex),
    _clauseAssembler(&_sharingIndex,&_litSelection),
    _inferenceDispatcherSemaphore(false),
    _backwardSubsumptionSemaphore(false),
    _backwardDemodulationSemaphore(false)
    {
      CALL("constructor Master(ulong (*nextClauseNumFunc)())");
      _freshClause.SetClauseIndex(&_clauseAssembler);
      _freshClause.SetComponentName(&_componentName);
      _freshClause.SetForwardSubsumption(&_forwardSubsumption);  
      _freshClause.SetSimplificationByUEq(&_forwardDemodulation);  
    };
  ~Master() 
    {
      CALL("destructor ~Master()");
      if (DestructionMode::isThorough()) destroy();
    };

  void setStrategy()
    { 
      initParameters();
      initObjects();
    };
  void setClauseNumberGenerator(Clause::NewNumber* ng) 
    {
      _clauseNumberGenerator = ng;
    };
  void destroy();
  void reset();

#ifndef NO_DEBUG
  bool clean();
#endif 


  void endOfInput();
  const GlobalClock::Timer& timer() const { return _timer; }; 
  bool terminated() const { return _terminated; };
  TerminationReason terminationReason() const { return _terminationReason; };  
  const Clause* proof() const { return _proof; };
  void mainLoop() 
    { 
      switch (_parameters.main_alg)
	{
	case 0: mainLoop0(); break;
	case 3: mainLoop3(); break; 
	default:
	  RuntimeError::report("Wrong --main_alg value.");
	};     
    };
  void eliminateDefinitions();
  void inputClause(Clause* cl) 
    { 
      switch (_parameters.main_alg)
	{
	case 0: inputClause0(cl); break;
	case 3: inputClause3(cl); break;
	default: 
	  RuntimeError::report("Wrong --main_alg value.");
	};
    };

  void externalTick()
    {
      _externalTickFlag = true;
    };

  void suspend()
    {
      if (_timerIsOn) _timer.stop();
    };

  void resume()
    {
      if (_timerIsOn) _timer.start();
    };
    
  bool releaseMemory(ulong size)
    {
      CALL("releaseMemory(ulong size)");
      switch (_parameters.main_alg)
	{
	case 0: return false;
	case 3: return releaseMemory3(size);
	default: ICP("ICP0"); return false;  
	};        
    };

  bool releaseMemory3(ulong size);

 
  void passiveResetIter() { _passive.ResetIterator(); };
  Clause* nextPassive() { return _passive.IterNext(); };
  void historyResetIter() { _history.ResetIterator(); };
  Clause* nextHistory() { return _history.Next(); };
  void terminate(TerminationReason tr) 
    {
      _terminated = true; 
      _terminationReason = tr; 
    };
  void cancelTermination()
    {
      CALL("cancelTermination()");
      ASSERT(_terminated);
      ASSERT(_proof);
      _terminated = false;
      _proof = (Clause*)0;
      ASSERT(!_timerIsOn);
      _timer.start();
      _timerIsOn = true;
    };
  ulong statGenerated() const { return _freshClause.statGenerated(); };
  ulong statFSubsumed() const { return _freshClause.statForwardSubsumed(); };   
  ulong statFSimplified() const 
    {
      return statFSimplifiedByFSR() + 
	statFSimplifiedByDemod() + 
	statFSimplifiedByEqRes() +
	statSimplifiedByBuiltInTheories();
    };
  ulong statFSimplifiedByFSR() const 
    { 
      return _freshClause.statSimplifiedByFSR(); 
    };
  ulong statFSimplifiedByDemod() const 
    { 
      return _freshClause.statSimplifiedByDemod(); 
    };
  ulong statSimplifiedByBuiltInTheories() const
    {
      return _freshClause.statSimplifiedByBuiltInTheories();
    };
  ulong statFDOnSplittingBranches() const
    {
      return _freshClause.statFDOnSplittingBranches();
    };
  ulong statFSimplifiedByEqRes() const 
    { 
      return _freshClause.statSimplifiedByEqRes(); 
    }; 
  ulong statSplit() const { return _freshClause.statSplit(); };
  ulong statComponents() const { return _freshClause.statComponents(); };
  float statAvgComponents() const 
    { 
      if (_freshClause.statSplit()) 
	{
	  return ((float)_freshClause.statComponents())/_freshClause.statSplit();
	}
      else
	return 1.0;
    };
  ulong statDiffComponents() const { return _freshClause.statDifferentComponents(); };
  ulong statEqTautologies() const { return _freshClause.statEqTautologies(); };
  ulong statPropTautologies() const { return _freshClause.statPropTautologies(); }; 
  ulong statTooDeepInferences() const 
    {
      return _freshClause.statTooDeepInferences();
    };
  ulong statTooHeavy() const { return _freshClause.statTooHeavy(); };
  ulong statTooHeavyLiterals() const { return _freshClause.statTooHeavyLiterals(); };
  ulong statTooManyVariables() const { return _freshClause.statTooManyVariables(); };
  ulong statTooLong() const { return _freshClause.statTooLong(); };
  ulong statTooDeep() const { return _freshClause.statTooDeep(); };
  ulong statRetained() const 
    {
      if (_parameters.main_alg == 3)
	{
	  return _pagedPassive.totalNumberOfClauses();
	};
      return _passive.Total(); 
    };
  ulong statRefusedAllocation() const { return _statClausesRefusedAllocation; };
  ulong statUsed() const { return _active.Total(); };
  ulong statBSubsumed() const { return _statBackSubsumed; };   
  ulong statBSimplified() const { return _statBackSimplified; };
  ulong statBDOnSplittingBranches() const
    {
      return _backDemodulation.statBDOnSplittingBranches();
    };
  ulong statRewrittenByDefUnfolding() const { return _statRewrittenByDefUnfolding; }; 

  ulong statOrphansMurdered() const 
    { 
      return _statPassiveOrphansMurdered + _statActiveOrphansMurdered; 
    };
  ulong statPassiveOrphansMurdered() const { return _statPassiveOrphansMurdered; };
  ulong statActiveOrphansMurdered() const { return _statActiveOrphansMurdered; };
  ulong statRecycled() const { return _statRecycled + _pagedPassive.numberOfCancelledClauses(); }; 
  ulong statCurrentlyPassive() const { return _passive.CurrentlyKept() + _pagedPassive.numberOfCurrentlyKeptClauses(); };
  ulong statReservedPassive() const { return _reservedPassive.Size(); };
  ulong statPassiveReachable() const { return _parameters.reachable; };
  ulong statCurrentlyActive() const { return _active.Size(); };

  bool completeStrategyUsed();

  /***************/
#ifndef NO_DEBUG
  void scanSharingIndex(ostream& str) // for debugging
    {
      cout << "Scanning the sharing index... ";
      cout.flush();
      _sharingIndex.ScanIndex(str);
      cout << "done\n";
    };
#endif

 
 public:
  Options options;
 private:
  class Parameters
    {
    public:
      Parameters() {};
      // main algorithm
      bool incompleteAPriori;
      long main_alg;
      // inference rules
      bool binary_resolution;
      bool no_inferences_between_rules;
      bool splitting;  
      long splitting_style;
      bool splitting_with_naming;
      long neg_eq_splitting;
      bool paramodulation;
      long sim_back_sup;
      long selection;
      long literal_comparison_mode;
      bool inherited_negative_selection;

      // strategy switches
      bool handle_equality;
      bool forward_subsumption;
      bool fs_set_mode;
      bool fs_optimised_multiset_mode;
      bool fs_use_sig_filters;
      bool fs_old_style;
      long simplify_by_fs;
      long forward_demodulation;
      bool fd_on_splitting_branches;
      const SymbolInfo* normalize_by_commutativity;
      long backward_demodulation;
      bool bd_on_splitting_branches;
      bool simplify_by_eq_res;
      bool backward_subsumption;
      bool bs_set_mode;
      bool bs_optimised_multiset_mode;
      long orphan_murder;
      long lrs; 
      const SymbolInfo* pred_elim_table; 
      const bool* answer_predicate_table; 
      ulong answer_predicate_table_size;
      // numeric parameters
      long age_weight_ratio;
      long timeLimit;
      float memoryLimit;
      float allocation_buffer_price; 
      long maxActive;
      long maxPassive; 
      long maxWeight;
      long maxMaxWeight;
      long maxInferenceDepth;
      float first_time_check;
      float first_mem_check; 
      long reachable;
      long unreachable;
      // interface
      ostream* outputStreamForGenerated;
      void (*showPassive) (const Clause*);     
      void (*showActive) (const Clause*);
      void (*showWeightChange) (long,long);
      void (*showUsagePace) (float); 

      // auxilliary
      bool useClauseNumIndex;  
    }; // class Parameters

  enum Continuation
  {
    Continuation_Init,
    Continuation_InputClause0_ProcessCollectedInput,
    Continuation_InputClause0_BackSimplify,
    Continuation_EliminateDefinitions_ProcessCollectedInput,
    Continuation_EliminateDefinitions_BackSimplify,
    Continuation_MainLoop0_ProcessNewInference,
    Continuation_MainLoop0_BackSimplify,
    Continuation_InputClause3_ProcessCollectedInput,
    Continuation_MainLoop3_SelectCurrent,
    Continuation_MainLoop3_BackSimplify, 
    Continuation_MainLoop3_ProcessNewInference
  };

 private:
  void resetStatistics() 
    {
      _statClausesRefusedAllocation = 0;
      _statBackSubsumed = 0;
      _statBackSimplified = 0;
      _statRewrittenByDefUnfolding = 0;
      _statPassiveOrphansMurdered = 0;
      _statActiveOrphansMurdered = 0; 
      _statRecycled = 0;
    };
  void initParameters();            
  void initObjects();
  void inputClause0(Clause* cl);
  void inputClause3(Clause* cl);
  void mainLoop0();
  void mainLoop3();
  Clause* reanimate3(PagedPassiveClauseStorage::StoredClause& cl); 
  void processCollected0();
  void processCollected3();  
  void processCollectedInput0();
  void processCollectedInput3(); 
  Clause* select0();
  Clause* select1() 
    { 
      CALL("select1()");
      Clause* res = _passive.Next();
      if (res) 
	{
	  ASSERT(res->mainSet() == Clause::MainSetPassive);
	  res->setMainSet(Clause::MainSetFree); 
	};
      return res;  
    };
  Clause* select2() 
    {
      CALL("select2()");
      Clause* res = _passive.Next();
      if (res) 
	{
	  ASSERT(res->mainSet() == Clause::MainSetPassive);
	  res->setMainSet(Clause::MainSetFree); 
	};   
      return res;  
    };
  Clause* select3() 
    {
      CALL("select3()");
      Clause* res = _passive.Next();
      if (res) 
	{
	  ASSERT(res->mainSet() == Clause::MainSetPassive);
	  res->setMainSet(Clause::MainSetFree); 
	};   
      return res;  
    };
  
  void supportClausesIntoActive0();

  void makeActive0(Clause* cl);
  void makeActive3(Clause* cl);
  void makePassive0(Clause* cl);
  void rememberAnswer0(Clause* cl);
  void rememberAnswer3(Clause* cl);
  void supportIntoActive3(Clause* cl);
  bool insertIntoPassive3(Clause* cl);
  void integratePassive(Clause* cl);
  void integrateIntoBackDemIndexFromQueue();
  void makeReserved(Clause* cl);
  void enqueueForDisposal(Clause* cl);
  

  void disposeObsolete();
  void eliminateAllocationBufferDeficit0();

  void recycleUnreachablePassive0();

  void killActiveOrphans0();
  void killActiveOrphans3();

  void removeFromIndices(Clause* cl);
  void disposeClause(Clause* cl);
  void destroyPassive0();
  void destroyPassive3();
  void loadReservedClauses();
  bool collectBackSimplified(); // _backDemodulation -->> _freshClause 
  bool backSubsumeBy(Clause* cl);
  void backSimplify();  
  void backSimplifyBy3(Clause* simplifier);
  float usedMemory();
  void estimateResources()
    {
      switch (_parameters.lrs)
	{
	case 0L: 
	  if (_parameters.showUsagePace)
	    {
	      float used_time = _timer.elapsedSeconds();
	      if (used_time < 1.0) used_time = 1.0;      
	      long moved_from_passive = _passive.UsedAndRemoved();    
	      _parameters.showUsagePace(moved_from_passive/used_time);
	    };
	  return;

	case 1L: estimateResources1(); break;
      
	case 2L: estimateResources2(); break;

	case 3L: estimateResources3(); break;

	case 4L: estimateResources4(); break;

	case 5L: estimateResources5(); break;

	default: estimateResources1(); 
	};
    };
  
  void estimateResources1(); 
  void estimateResources2();
  void estimateResources3();
  void estimateResources4();
  void estimateResources5();
  void internalTick0();
 
  void integrateIntoForwardSubsumptionIndex(Clause* cl) 
    { 
      CALL("integrateIntoForwardSubsumptionIndex(Clause* cl)");
      ASSERT(!cl->flag(Clause::FlagInForwardSubsumptionIndex));
      _forwardSubsumption.Integrate(cl);
      cl->setFlag(Clause::FlagInForwardSubsumptionIndex);
    };
  
  void removeFromForwardSubsumptionIndex(Clause* cl) 
    { 
      CALL("removeFromForwardSubsumptionIndex(Clause* cl)");
      ASSERT(cl->flag(Clause::FlagInForwardSubsumptionIndex));  
      _forwardSubsumption.Remove(cl);
      cl->clearFlag(Clause::FlagInForwardSubsumptionIndex);
    };

  void integrateIntoBackwardSubsumptionIndex(Clause* cl)
    {
      CALL("integrateIntoBackwardSubsumptionIndex(Clause* cl)");
      ASSERT(!cl->flag(Clause::FlagInBackwardSubsumptionIndex));
      _backSubsumption.integrate(cl);
      cl->setFlag(Clause::FlagInBackwardSubsumptionIndex);
    };

  void removeFromBackwardSubsumptionIndex(Clause* cl) 
    { 
      CALL("removeFromBackwardSubsumptionIndex(Clause* cl)");
      ASSERT(cl->flag(Clause::FlagInBackwardSubsumptionIndex));  
      _backSubsumption.remove(cl);
      cl->clearFlag(Clause::FlagInBackwardSubsumptionIndex);
    };

  void integrateIntoForwardDemodulationIndex(Clause* cl) 
    { 
      CALL("integrateIntoForwardDemodulationIndex(Clause* cl)");
      ASSERT(!cl->flag(Clause::FlagInForwardDemodulationIndex));
      _forwardDemodulation.Integrate(cl);
      cl->setFlag(Clause::FlagInForwardDemodulationIndex);
    };
  
  void removeFromForwardDemodulationIndex(Clause* cl) 
    { 
      CALL("removeFromForwardDemodulationIndex(Clause* cl)");
      ASSERT(cl->flag(Clause::FlagInForwardDemodulationIndex));  
      _forwardDemodulation.Remove(cl);
      cl->clearFlag(Clause::FlagInForwardDemodulationIndex);
    };

  void integrateIntoBackDemodulationIndex(Clause* cl)
    {
      CALL("integrateIntoBackDemodulationIndex(Clause* cl)");
      ASSERT(!cl->flag(Clause::FlagInBackDemodulationIndex));
      _backDemodulation.integrate(cl);
      cl->setFlag(Clause::FlagInBackDemodulationIndex);
    };

  void removeFromBackDemodulationIndex(Clause* cl) 
    { 
      CALL("removeFromBackDemodulationIndex(Clause* cl)");
      ASSERT(cl->flag(Clause::FlagInBackDemodulationIndex));  
      _backDemodulation.remove(cl);
      cl->clearFlag(Clause::FlagInBackDemodulationIndex);
    };
   
  bool backDemodulationQuery(Clause* cl)
    { 
      CALL("backDemodulationQuery(Clause* cl)");
      _backDemodulationQuery = cl;
      if (_backDemodulation.query(cl)) { cl->Block(); return true; };
      return false;
    };

  bool nextBackDemodulated()
    {
      CALL("nextBackDemodulated()");
      if (!_backDemodulation.nextSimplified()) { _backDemodulationQuery->Unblock(); return false; };
      return true;  
    };

 private:
  GlobalClock::Timer _timer;
  Continuation _continuation;
  bool _terminated;
  TerminationReason _terminationReason; 
  Clause* _proof;
  bool _timerIsOn;
  bool _externalTickFlag;
  Parameters _parameters;
  Unifier _unifier;
  KEPT _passive;
  PagedPassiveClauseStorage _pagedPassive;
  CLAUSE_SET _reservedPassive;
  CLAUSE_SET _active;
  CLAUSE_SET _history;
  CLAUSE_SET _answers;
  ClauseQueue _retainedNewClauses;
  ClauseQueue _queueForDisposal;

  ClauseQueue _queueForBackSimplification;
  ClauseQueue _queueForIntegrationIntoBackwardDemodIndex;

  InferenceDispatcher _infDispatcher;
  ForwardSubsumptionFrontEnd _forwardSubsumption;
  BackwardSubsumption _backSubsumption;
  ForwardDemodulation _forwardDemodulation;
  BackDemodulation _backDemodulation;
  Clause* _backDemodulationQuery;
  ComponentName _componentName;  // temporary, must be part of a higher level module
  SHARING_INDEX _sharingIndex;
  LiteralSelection _litSelection;
  ClauseAssembler _clauseAssembler; 
  bool _inferenceDispatcherSemaphore;
  bool _backwardSubsumptionSemaphore;
  bool _backwardDemodulationSemaphore;
  NewClause _freshClause;
  float _startMemory;
  ClauseNumIndex _clauseNumIndex; // temporary, must be part of a higher level module
  Clause::NewNumber* _clauseNumberGenerator; 
  ulong _statClausesRefusedAllocation;
  ulong _statBackSubsumed;
  ulong _statBackSimplified;
  ulong _statRewrittenByDefUnfolding;   
  ulong _statPassiveOrphansMurdered;
  ulong _statActiveOrphansMurdered; 
  ulong _statRecycled;
  Clause* _reanimatedClause;
  enum { _halfLongMax = (LONG_MAX/2) };
}; // class Master

#ifndef NO_DEBUG
inline bool Master::clean()
{
  CALL("clean()");
  if (!_passive.clean())
    {
#ifdef DEBUG_NAMESPACE
      DMSG("Master::clean()> ") << "!_passive.clean()\n";
#endif
      return false;
    }; 
  if (!_reservedPassive.clean())
    {
#ifdef DEBUG_NAMESPACE
      DMSG("Master::clean()> ") << "!_reservedPassive.clean()\n";
#endif
      return false;
    };  
  if (!_active.clean())
    {
#ifdef DEBUG_NAMESPACE
      DMSG("Master::clean()> ") << "!_active.clean()\n";
#endif
      return false;
    }; 
  if (!_history.clean())
    {
#ifdef DEBUG_NAMESPACE
      DMSG("Master::clean()> ") << "!_history.clean()\n";
#endif
      return false;
    };  
         
  if (!_answers.clean())
    {
#ifdef DEBUG_NAMESPACE
      DMSG("Master::clean()> ") << "!_answers.clean()\n";
#endif
      return false;
    };  
         

  if (_retainedNewClauses)
    {
#ifdef DEBUG_NAMESPACE
      DMSG("Master::clean()> ") << "_retainedNewClauses nonempty\n";
#endif
      return false;
    };  
  if (_queueForDisposal)
    {
#ifdef DEBUG_NAMESPACE
      DMSG("Master::clean()> ") << "_queueForDisposal nonempty\n";
#endif
      return false;
    };  
  if (_queueForBackSimplification)
    {
#ifdef DEBUG_NAMESPACE
      DMSG("Master::clean()> ") << "_queueForBackSimplification nonempty\n";
#endif
      return false;
    }; 
  if (_queueForIntegrationIntoBackwardDemodIndex)
    {
#ifdef DEBUG_NAMESPACE
      DMSG("Master::clean()> ") << "_queueForIntegrationIntoBackwardDemodIndex nonempty\n";
#endif
      return false;
    }; 
  if (!_infDispatcher.clean())        
    {
#ifdef DEBUG_NAMESPACE
      DMSG("Master::clean()> ") << "!_infDispatcher.clean()\n";
#endif
      return false;
    };  
  if (!_forwardSubsumption.clean())
    {
#ifdef DEBUG_NAMESPACE
      DMSG("Master::clean()> ") << "!_forwardSubsumption.clean()\n";
#endif
      return false;
    }; 
  if (!_backSubsumption.clean())
    {
#ifdef DEBUG_NAMESPACE
      DMSG("Master::clean()> ") << "!_backSubsumption.clean()\n";
#endif
      return false;
    }; 
  if (!_forwardDemodulation.clean())
    {
#ifdef DEBUG_NAMESPACE
      DMSG("Master::clean()> ") << "!_forwardDemodulation.clean()\n";
#endif
      return false;
    };  
  if (!_sharingIndex.clean())
    {
#ifdef DEBUG_NAMESPACE
      DMSG("Master::clean()> ") << "!_sharingIndex.clean()\n";
#endif
      return false;
    };  
  if (!_clauseAssembler.clean())
    {
#ifdef DEBUG_NAMESPACE
      DMSG("Master::clean()> ") << "!_clauseAssembler.clean()\n";
#endif
      return false;
    };  
  if (!_clauseNumIndex.clean())
    {
#ifdef DEBUG_NAMESPACE
      DMSG("Master::clean()> ") << "!_clauseNumIndex.clean()\n";
#endif
      return false;
    }; 
  return true; 
}; // bool Master::clean()
#endif


inline void Master::Options::reset()
{
  incompleteAPriori = false;
  // main algorithm
  main_alg = 0;
 
  // inference rules
  binary_resolution = true;
  no_inferences_between_rules = false;
  splitting = false;  
  splitting_style = 1L;
  splitting_with_naming = false;
  neg_eq_splitting = 0L; 
  paramodulation = false;
  sim_back_sup = 0;
  selection = 1;
  literal_comparison_mode = 1;
  inherited_negative_selection = false;

  // strategy switches
  handle_equality = false;
  forward_subsumption = true;
  fs_set_mode = false;
  fs_optimised_multiset_mode = false;
  fs_use_sig_filters = false;
  fs_old_style = false;
  simplify_by_fs = 0;
  forward_demodulation = 0;
  fd_on_splitting_branches = false;
  normalize_by_commutativity = (const SymbolInfo*)0;
  backward_demodulation = 0;
  bd_on_splitting_branches = false;
  simplify_by_eq_res = false;
  backward_subsumption = false;
  bs_set_mode = false;
  bs_optimised_multiset_mode = false;
  orphan_murder = 0;
  lrs = 0;
  pred_elim_table = (const SymbolInfo*)0;
  answer_predicate_table = (const bool*)0;
  answer_predicate_table_size = 0UL;
  // numeric options
  age_weight_ratio = 0;
  timeLimit = LONG_MAX/10;
  memoryLimit = (float)LONG_MAX;
  allocation_buffer_price = 1.0;
  maxActive = LONG_MAX;
  maxPassive = LONG_MAX;
  maxWeight = VampireKernelConst::MaxClauseSize;
  maxInferenceDepth = LONG_MAX;
  first_time_check = 1.0;
  first_mem_check = 1.0;
  // interface options
  outputStreamForGenerated = (ostream*)0;
  showPassive = 0;
  showActive = 0;
  showWeightChange = 0;
  showUsagePace = 0;
}; // void Master::Options::reset()

inline void Master::initParameters()
{
  CALL("Master::initParameters()");

  _parameters.incompleteAPriori = options.incompleteAPriori;
 
  // main algorithm

  _parameters.main_alg = options.main_alg;

  // inference rules
  _parameters.binary_resolution = options.binary_resolution;
  _parameters.no_inferences_between_rules = options.no_inferences_between_rules;
  _parameters.splitting = options.splitting; 
  _parameters.splitting_style = options.splitting_style;
  _parameters.splitting_with_naming = options.splitting_with_naming;
  _parameters.neg_eq_splitting = options.neg_eq_splitting;  
  _parameters.paramodulation = options.paramodulation;
  _parameters.sim_back_sup = options.sim_back_sup;
  _parameters.selection = options.selection;
  _parameters.literal_comparison_mode = options.literal_comparison_mode;
  _parameters.inherited_negative_selection = options.inherited_negative_selection;

  // strategy switches
  _parameters.handle_equality = options.handle_equality;
  _parameters.forward_subsumption = options.forward_subsumption;
  _parameters.fs_set_mode = options.fs_set_mode; 
  _parameters.fs_optimised_multiset_mode = options.fs_optimised_multiset_mode;
  _parameters.fs_use_sig_filters = options.fs_use_sig_filters; 
  _parameters.fs_old_style = options.fs_old_style;
  _parameters.simplify_by_fs = options.simplify_by_fs;
  _parameters.forward_demodulation = options.forward_demodulation;
  _parameters.fd_on_splitting_branches = options.fd_on_splitting_branches;
  _parameters.normalize_by_commutativity = options.normalize_by_commutativity;
  _parameters.backward_demodulation = options.backward_demodulation;
  _parameters.bd_on_splitting_branches = options.bd_on_splitting_branches;
  _parameters.simplify_by_eq_res = options.simplify_by_eq_res;
  _parameters.backward_subsumption = options.backward_subsumption;
  _parameters.bs_set_mode = options.bs_set_mode;
  _parameters.bs_optimised_multiset_mode = options.bs_optimised_multiset_mode;
  _parameters.orphan_murder = options.orphan_murder;
  _parameters.lrs = options.lrs;
  _parameters.pred_elim_table = options.pred_elim_table;
  _parameters.answer_predicate_table = options.answer_predicate_table;
  _parameters.answer_predicate_table_size = options.answer_predicate_table_size;
  // numeric parameters  
  _parameters.age_weight_ratio = options.age_weight_ratio;
  _parameters.timeLimit = options.timeLimit;
  _parameters.memoryLimit = options.memoryLimit;
  _parameters.allocation_buffer_price = options.allocation_buffer_price;
  _parameters.maxActive = options.maxActive;
  _parameters.maxPassive = options.maxPassive;
  _parameters.maxWeight = (options.maxWeight <= ((long)VampireKernelConst::MaxClauseSize)) ? options.maxWeight : VampireKernelConst::MaxClauseSize;
  _parameters.maxMaxWeight = _parameters.maxWeight;
  _parameters.maxInferenceDepth = options.maxInferenceDepth;
  _parameters.first_time_check = options.first_time_check; 
  _parameters.first_mem_check = options.first_mem_check;   
  _parameters.reachable = LONG_MAX;
  _parameters.unreachable = (-1)*_halfLongMax;     
  // interface parameters
  _parameters.outputStreamForGenerated = options.outputStreamForGenerated;
  _parameters.showPassive = options.showPassive;
  _parameters.showActive = options.showActive;
  _parameters.showWeightChange = options.showWeightChange;
  _parameters.showUsagePace = options.showUsagePace;
  // auxilliary
  _parameters.useClauseNumIndex = options.backward_demodulation || options.backward_subsumption;
}; // void Master::initParameters()

inline void Master::initObjects()
{
  // clause sets
  _passive.SetPickGivenRatio(_parameters.age_weight_ratio);
  _pagedPassive.setAgeWeightRatio(_parameters.age_weight_ratio);


  // literal selection
  
  _litSelection.setSplittingFlag(_parameters.splitting);
  _litSelection.setSplittingStyle(_parameters.splitting_style);
  _litSelection.setFunNum(_parameters.selection);
  _litSelection.setLiteralComparisonMode(_parameters.literal_comparison_mode);

  // clause sharing 
  _clauseAssembler.setInheritNegativeSelection(_parameters.inherited_negative_selection);
  _clauseAssembler.setEqualityIsPossibleFlag(_parameters.handle_equality);
  _clauseAssembler.setEliminatedPredicateTable(_parameters.pred_elim_table);

  // _freshClause
  _freshClause.SetForwardSubsumptionFlag(_parameters.forward_subsumption);
  _freshClause.SetSimplificationByFS(_parameters.simplify_by_fs);
  _freshClause.SetSimplificationByUEq(_parameters.forward_demodulation);
  _freshClause.SetFDOnSplittingBranchesFlag(_parameters.fd_on_splitting_branches);
  _freshClause.SetNormalizationByCommutativity(_parameters.normalize_by_commutativity);
  _freshClause.SetSimplificationByEqRes(_parameters.simplify_by_eq_res);  
  _freshClause.SetSplitting(_parameters.splitting);
  _freshClause.SetSplittingStyle(_parameters.splitting_style);
  _freshClause.SetSplittingWithNaming(_parameters.splitting_with_naming);
  _freshClause.SetNegEqSplitting(_parameters.neg_eq_splitting); 
  _freshClause.SetEqualityPossible(_parameters.handle_equality);
  _freshClause.SetMaxWeight(_parameters.maxWeight);
  _freshClause.setMaxInferenceDepth(_parameters.maxInferenceDepth);
  // _forwardSubsumption      
  _forwardSubsumption.SetSetMode(_parameters.fs_set_mode);
  _forwardSubsumption.SetOptimisedMultisetMode(_parameters.fs_optimised_multiset_mode);
  _forwardSubsumption.SetUseSigFilters(_parameters.fs_use_sig_filters);
  _forwardSubsumption.SetOldStyle(_parameters.fs_old_style);
  _forwardSubsumption.setSubsumptionResolutionMinLength(_parameters.simplify_by_fs);
  // _backSubsumption

  _backSubsumption.setSetMode(_parameters.bs_set_mode);
  _backSubsumption.setOptimisedMultisetMode(_parameters.bs_optimised_multiset_mode);

  // forward demodulation
  
  _forwardDemodulation.SetDegree(_parameters.forward_demodulation);
  _forwardDemodulation.SetFDOnSplittingBranchesFlag(_parameters.fd_on_splitting_branches);

  // _backDemodulation
  _backDemodulation.setDegree(_parameters.backward_demodulation);
  _backDemodulation.setBDOnSplittingBranchesFlag(_parameters.bd_on_splitting_branches);

  // _infDispatcher    
  _infDispatcher.binResFlag = _parameters.binary_resolution;
  _infDispatcher.setNoInfBetweenRulesFlag(_parameters.no_inferences_between_rules);
  _infDispatcher.setParamodulationFlag(_parameters.paramodulation);
  _infDispatcher.setSimBackSup(_parameters.sim_back_sup);
}; // void Master::initObjects()

inline float Master::usedMemory()
{ 
  float res = ((float)GlobAlloc::occupiedByObjects()) - _startMemory;
  if (res <= 0) return 0.0001;
  return res;
}; // float Master::usedMemory() 

inline void Master::internalTick0()
{
  CALL("internalTick0()");
  GlobalStopFlag::check();
  if (_externalTickFlag)
    {
      estimateResources();
      _externalTickFlag = false;      
    };
}; // void Master::internalTick0()


inline void Master::eliminateAllocationBufferDeficit0()
{
  CALL("eliminateAllocationBufferDeficit0()");
  if (GlobAlloc::activeBufferDeficit())
    {
      long disposalBound = (long)(GlobAlloc::occupiedByObjects() - (_parameters.allocation_buffer_price * GlobAlloc::activeBufferDeficit()));
      do
	{
	  Clause* cl = _passive.TheWorstUnblockedClause();    
	  if (!cl) return;  
	  ASSERT(cl->mainSet() == Clause::MainSetPassive);
	  _passive.Cancel(cl);
	  cl->setMainSet(Clause::MainSetFree);
	  enqueueForDisposal(cl);
	  _statRecycled++;
	}
      while (GlobAlloc::activeBufferDeficit() && (GlobAlloc::occupiedByObjects() > disposalBound));
      if (GlobAlloc::activeBufferDeficit()) GlobAlloc::annulBufferDeficit();
    };
}; // void Master::eliminateAllocationBufferDeficit0() 

}; // namespace VK


//====================================================
#endif
