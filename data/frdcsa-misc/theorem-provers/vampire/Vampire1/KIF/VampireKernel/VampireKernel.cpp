//
// File:         VampireKernel.cpp
// Description:  Implementation of Vampire kernel. 
// Created:      Jun 09, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//======================================================================
#include <limits.h>
#include <stdio.h>
#include "GlobAlloc.h"
#include "GlobalClock.h"
#include "RuntimeError.h"
#include "GlobalStopFlag.h"
#include "VampireKernel.h"
#include "Signature.h"
#include "Clause.h"
#include "ClauseQueue.h"
#include "ProblemProfile.h"
#include "Tabulation.h"
#include "Master.h"
#include "ClauseNumIndex.h"
#include "TermWeighting.h"
#include "UniformTermWeighting.h"
#include "NonuniformConstantTermWeighting.h"
#include "NonrecursiveKBOrdering.h"
#include "StandardKBOrdering.h"
#include "InferenceTree.h"
#include "tmp_literal.h"
#include "WeightPolynomial.h"
#include "BuiltInTheoryDispatcher.h"
//======================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_VAMPIRE_KERNEL
#define DEBUG_NAMESPACE "VampireKernel::_Implementation"
#endif    
#include "debugMacros.h"
//======================================================================


using namespace VK;

class VampireKernel::_Implementation
{
public:
  _Implementation(const VampireKernel::Options& opt) :
    _options(opt),
    _saturationRunning(false),
    _mainLoopRunning(false),
    _incompleteProcedure(false),
    _tabulation((Tabulation*)0),
    _builtInTheoryDispatcher((BuiltInTheoryDispatcher*)0),
    _uniformTermWeighting((UniformTermWeighting*)0),
    _nonuniformConstantTermWeighting((NonuniformConstantTermWeighting*)0),
    _terminationReason(VampireKernel::UnknownTerminationReason),
    _continuation(Continuation_Init),
    _lookingForMoreAnswers(false)
  {
    CALL("constructor _Implementation(const VampireKernel::Options& opt)");
    Signature::setCurrent(&_signature);
    _signature.setMaxlNumOfSkolemSymbols(_options.max_skolem);
    if (_options.tabulation)
      {
	_tabulation = new Tabulation();
	_tabulation->assignStream(_options.tabulation);
	_tabulation->assignJob(_options.job_id);
	static ulong sessionId = 0UL;
	_tabulation->assignKernelSession(sessionId);
	++sessionId;
      }
    else
      _tabulation = (Tabulation*)0;
    _master.setClauseNumberGenerator(&_clauseNumberGenerator);
    
    WeightPolynomial::setSharing(&_weightPolynomialSharing);

    switch (_options.term_weighting_scheme)
      {
      case VampireKernel::Options::UniformTermWeighting:
	_uniformTermWeighting = new UniformTermWeighting();
	TermWeighting::setCurrent((TermWeighting*)_uniformTermWeighting);
	break;
      case VampireKernel::Options::NonuniformConstantTermWeighting:
	_nonuniformConstantTermWeighting = 
	  new NonuniformConstantTermWeighting();
	TermWeighting::setCurrent((TermWeighting*)_nonuniformConstantTermWeighting);
	break;
      };
    
    switch (_options.simplification_ordering)
      {
      case VampireKernel::Options::NonrecursiveKBOrdering:
	SimplificationOrdering::setCurrent((SimplificationOrdering*)(&_nonrecursiveKBOrdering));
	break;
      case VampireKernel::Options::StandardKBOrdering:
	SimplificationOrdering::setCurrent((SimplificationOrdering*)(&_standardKBOrdering));
	break;
      };
    openSession();
  }; // _Implementation(const VampireKernel::Options& opt)

  ~_Implementation() 
  {
    CALL("destructor ~_Implementation()");
    closeSession();
    if (DestructionMode::isThorough())
      {
	VK::Clause* clause;
	while (_inputClauses.dequeue(clause))
	  clause->DestroyUnshared();
	while (_preprocessingHistory.dequeue(clause))
	  clause->DestroyUnshared();
	if (_tabulation) delete _tabulation;
	if (_builtInTheoryDispatcher) delete _builtInTheoryDispatcher;
	if (_uniformTermWeighting) delete _uniformTermWeighting;
	if (_nonuniformConstantTermWeighting) 
	  delete _nonuniformConstantTermWeighting;
      };
  };

  static void closeSession()
  {
    _currentSession = (_Implementation*)0;
  };
  
  void* operator new(size_t)
  {
    return GlobAlloc::allocate(sizeof(_Implementation));
  };
  
  void operator delete(void* obj)
  {
    GlobAlloc::deallocate(obj,sizeof(_Implementation));
  };

  void beginInputSignature() 
  {
    _symbolWeightConstantPartInputIgnored = false;
    _unaryFunctionWithZeroWeightExists = false;
  };
  void endInputSignature() 
  {
    if (_symbolWeightConstantPartInputIgnored)
      {
	if (_options.std_output)
	  *_options.std_output << "% WARNING: input of some symbol weight constant parts ignored\n"
			       << "%          because of --term_weighting_scheme " 
			       << _options.term_weighting_scheme << "\n";
      };

    if (_options.selection < 0L)
      {
	_signature.adjustHeaderPrecedenceForPositiveSelection();
      };
    if (_options.header_precedence_kinky)
      {
	_signature.makeHeaderPrecedenceKinky();
      };   
    _signature.setDependenceOfPrecedenceOnArity(_options.symbol_precedence_by_arity);
    _signature.setDependenceOfWeightOnArity(_options.symbol_weight_by_arity);
    _signature.endOfInput();
  };
  
  void registerInputFunction(const char* name,
			     ulong arity,
			     ulong& id)
  {
    id = _signature.inputSymbolNumber(false,name,arity);
  };

  void registerInputNumericConstant(const double& value,
				    ulong& id)
  {
    id = _signature.numericConstantNumber(value); 
  };


  void registerInputPredicate(const char* name,
			      ulong arity,
			      bool isAnswer,
			      ulong& id)
  {
    id = _signature.inputSymbolNumber(true,name,arity);
    if (isAnswer) _signature.makeAnswerPredicate(id);
  };
  
  void setSymbolWeightConstPart(ulong symId,
				const VampireKernel::TermWeightType& weight)
  {
    if (_options.term_weighting_scheme == 
	VampireKernel::Options::UniformTermWeighting) 
      {
	_symbolWeightConstantPartInputIgnored = true;
	return;
      };
    
    VampireKernel::TermWeightType w = weight;
    if (w == (VampireKernel::TermWeightType)0)
      {
	ulong arity = _signature.arity(symId);
	if (arity == 0UL)
	  {
	    if (_options.std_output)
	      {
		*_options.std_output << "% WARNING: zero weight of the constant/propositional variable ";
		_signature.outputSymbol(*_options.std_output,symId) << " is reset to 1.\n";
	      };
	    w = (VampireKernel::TermWeightType)1;
	  }
	else
	  if ((arity == 1UL) &&
	      (!_signature.isPredicate(symId)))
	      
	    {
	      if (_unaryFunctionWithZeroWeightExists)
		{
		  if (_options.std_output)
		    {
		      *_options.std_output << "% WARNING: zero weight of the unary function ";
		      _signature.outputSymbol(*_options.std_output,symId) 
			<< " is reset to 1.\n"
			<< "           Zero weight was previously assigned to ";
		      _signature.outputSymbol(*_options.std_output,_unaryFunctionWithZeroWeight) << "\n";
		    };
		  w = (VampireKernel::TermWeightType)1;
		}
	      else
		{
		  _unaryFunctionWithZeroWeightExists = true;
		  _unaryFunctionWithZeroWeight = symId;
		  _signature.assignMaximalPrecedence(symId);
		};	      
	    };
      };
    _signature.setSymbolWeightConstantPart(symId,w);
  };

  void setSymbolPrecedence(ulong symId,long prec)
  {
    if (prec >= Signature::maximalPossibleSymbolPrecedence())
      {
	if (_options.std_output)
	  {
	    *_options.std_output << "% WARNING: precedence of ";
	    _signature.outputSymbol(*_options.std_output,symId) << " ignored: too high.\n";
	  };
	return;
      };
    if ((_unaryFunctionWithZeroWeightExists) && 
	(symId == _unaryFunctionWithZeroWeight))
      {
	if (_options.std_output)
	  {
	    *_options.std_output << "% WARNING: precedence of ";
	_signature.outputSymbol(*_options.std_output,symId) 
	  << " ignored: maximal precedence assigned since the function has zero weight.\n";
	  };
	return;
      };
    
    _signature.setInputSymbolPrecedence(symId,prec);
  };

  void interpret(bool predicate,
		 const char* inputSymbol,
		 ulong arity,
		 const char* theory,
		 const char* nativeSymbol)
  {
    if (!_builtInTheoryDispatcher)
      {
	_builtInTheoryDispatcher = new BuiltInTheoryDispatcher(&_clauseNumberGenerator);
	BuiltInTheoryDispatcher::setCurrent(_builtInTheoryDispatcher);
      };
    ulong symId = _signature.inputSymbolNumber(predicate,inputSymbol,arity);
    if (!_builtInTheoryDispatcher->interpret(symId,theory,nativeSymbol))
      {
	if (_options.std_output)
	  *_options.std_output << "% WARNING: can not interpret " << inputSymbol << "/" << arity 
			       << " as " << theory << "/" << nativeSymbol << "\n";
      };
  };

  
  void beginInputClauses()
  {
    CALL("beginInputClauses()");
    ASSERT(_inputClauses.empty());
    _numberOfRejectedInputClauses = 0UL;
  };

  void endInputClauses()
  {
   
  };
  
  void beginInputClause(ulong clauseNumber,void* origin,bool goal,bool support)
  {
    CALL("beginInputClause(ulong clauseNumber,void* origin,bool goal,bool support)");
    _inputClauseRejected = false;
    _newInputClause.reset();
    _newInputClause.usedRule(ClauseBackground::Input);
    if (goal) _newInputClause.markAsSubgoal();
    if (support) 
      {
	_newInputClause.markAsSupport();
	_incompleteProcedure = true;
      };
    _currentInputClauseNumber = clauseNumber;
    _currentInputClauseOrigin = origin;
  };

  void endInputClause()
  {
    CALL("endInputClause()");
    if (_inputClauseRejected)
      {
	++_numberOfRejectedInputClauses;
      }
    else
      {
	VK::Clause* clause = _newInputClause.result();
	clause->SetNumber(_currentInputClauseNumber);
	clause->setInputClauseOrigin(_currentInputClauseOrigin);
	_inputClauses.enqueue(clause);
	if (clause->Empty())
	  {
	    if (_options.std_output)
	      *_options.std_output << "% WARNING: empty input clause.\n";
	  };
      };
  }; // void endInputClause()
  
  void beginLiteral(bool positive,ulong predicateId)
  {
    CALL("beginLiteral(bool positive,ulong predicateId)");
    if (_inputClauseRejected) return;
    TERM::Polarity polarity = 
      (positive)? TERM::PositivePolarity : TERM::NegativePolarity;
    TERM header((TERM::Arity)_signature.arity(predicateId),
		(TERM::Functor)predicateId,
		polarity);
    if (!_newInputClause.litHeader(header)) _inputClauseRejected = true;
  }; // void beginLiteral(bool positive,ulong predicateId)

  void endLiteral()
  {
  };
  
  void receiveFunction(ulong symId)
  {
    CALL("receiveFunction(ulong symId)");
    if (_inputClauseRejected) return;
    TERM symbol((TERM::Arity)_signature.arity(symId),
		(TERM::Functor)symId,
		TERM::PositivePolarity);
    if (!_newInputClause.functor(symbol)) _inputClauseRejected = true;
  }; // void receiveFunction(ulong symId)

  void receiveNumericConstant(const double& value)
  {
    CALL("receiveNumericConstant(const double& value)");
    ulong id = _signature.numericConstantNumber(value);
    receiveFunction(id);
  }; // void receiveNumericConstant(const double& value


  void receiveVariable(ulong varNum)
  {
    CALL("receiveVariable(ulong varNum)");
    if (_inputClauseRejected) return;
    _newInputClause.variable(TERM(TermVar,varNum));
  };

  void saturate(ulong nextVacantClauseId,long timeLimit)
  {
    CALL("saturate(ulong nextVacantClauseId,long timeLimit)");

    GlobalStopFlag::reset();
    _saturationRunning = true;
    _saturationTimeLimit = timeLimit;
    if (_saturationTimeLimit < 0L) _saturationTimeLimit = 0L;
    _mainLoopRunning = false;
    _timer.reset();
    _timer.start();
    GlobalStopFlag::HandlerActivator 
      globalStopFlagHandlerActivator(globalStopFlagHandler);
    GlobalClock::AlarmHandler alarmHandler(handleAlarm);

    _clauseNumberGenerator.reset(nextVacantClauseId);
    if (_options.std_output)
      *_options.std_output << "% New Vampire Kernel session, time limit: " 
			   << _saturationTimeLimit/10 << "." 
			   << _saturationTimeLimit%10 << " sec.\n";
    if (_numberOfRejectedInputClauses)
      {
	if (_options.std_output)
	  *_options.std_output << "% WARNING: " << _numberOfRejectedInputClauses 
			       << " input clauses rejected.\n";
	_incompleteProcedure = true;
      };
    
    if (_options.show_opt) 
      {	
	if (_options.std_output)
	  {
	    *_options.std_output << "%================== Options: =======================\n";
	    *_options.std_output<< "% time limit = " << ((float)_saturationTimeLimit)/10 << "\n";
	    _options.output(*_options.std_output);
	    *_options.std_output << "%========== End of options: ======================\n";
	  };
      };

    analyseOptions();

    if (_options.show_input && _options.std_output)
      {
	*_options.std_output << "%================== Input clauses: ===================\n";
	outputInputClauses(); 
	*_options.std_output << "%============== End of input clauses: ================\n";
      };
    VK::Clause::setSelectedPosEqPenaltyCoefficient(_options.selected_pos_eq_penalty_coefficient);
    VK::Clause::setNongoalPenaltyCoefficient(_options.nongoal_penalty_coefficient);
    _problemProfile.load(_inputClauses);
    if (_options.show_profile && _options.std_output)
      {
        *_options.std_output << "%========= Profile of input:\n"; 
	_problemProfile.output(*_options.std_output);
	*_options.std_output << "%========= End of profile. ========\n";
	*_options.std_output << _signature;
      };


    GlobalStopFlag::check();
    preprocessAndRunMainLoop();
  }; // void saturate(ulong nextVacantClauseId,long timeLimit)

  void preprocessAndRunMainLoop()
  {
    CALL("preprocessAndRunMainLoop()");    
    long remainingTime;
    float remainingMemory;

    switch (_continuation)
      {
      case Continuation_Init:
	break;
      case Continuation_Prepro_Simplification:
	// same as below
      case Continuation_Prepro_DefElimination_LoadInput:
	// same as below
      case Continuation_Prepro_DefElimination_DefElimination:
	goto preprocess;
      case Continuation_LoadInput:
	if (_options.std_output)
	  *_options.std_output << "% Resuming loading preprocessed clauses...\n";
	goto load_input;
      case Continuation_MainLoop:
	if (_options.std_output)
	  *_options.std_output << "% Resuming main loop...\n";
	goto main_loop;
      default:
	ICP("ICP0");
	RuntimeError::report("Bad _continuation in VampireKernel::_Implementation::preprocessAndRunMainLoop()");
      };


  preprocess:
    preprocessInputClauses();

    if (_master.terminated())
      {
	_solvedByPreprocessing = true;
	reportTermination();
	_timer.stop();
	_saturationRunning = false; 
	return;
      };
    _continuation = Continuation_Init;

    _solvedByPreprocessing = false;

    
    _signature.setCommutativity(_problemProfile.commutativityTable(),
				_problemProfile.symmetryTable());
    _signature.makeCommutative(VampireKernelConst::UnordEqNum);
    _signature.makeCommutative(VampireKernelConst::OrdEqNum);

#ifdef INTERCEPT_FORWARD_MATCHING_INDEXING
    FMILog << _signature;
#endif
#ifdef INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING
    BSILog.unblock();
    BSILog << "MODE_DESCRIPTION\n";
    if (options.bs_set_mode)
      {
	BSILog << "  'set mode'\n";
      }
    else
      if (!options.bs_optimised_multiset_mode)
	{
	  BSILog << "  'multiset mode'\n";
	} 
      else 
	{
	  BSILog << "/* Logging aborted. Incorrect subsumption mode. */\n";
	  BSILog.flush();
	  RuntimeError::report("BSI logging aborted: incorrect subsumption mode."); 
	};
    BSILog << "END_OF_MODE_DESCRIPTION\n";
    BSILog.block();
#endif 
#ifdef INTERCEPT_BACKWARD_MATCHING_INDEXING
    BMILog.unblock();
    // Mode modifiers:
    BMILog << "$\n"; // end of mode description
    BMILog << _signature;
    INTERCEPT_BACKWARD_MATCHING_INDEXING_COMMENT(BMILog << "           // end of signature declaration");
    BMILog.block();
#endif


    if (!_problemProfile.numOfEqLiterals())
      {
	switchBuiltinEqualityOff();
      };

    GlobalStopFlag::check();

    remainingTime = _saturationTimeLimit - _timer.elapsedDeciseconds();
    remainingMemory = 
      (((float)_options.memory_limit)*1024) - GlobAlloc::occupiedByObjects();
    if (remainingMemory < 0.0) remainingMemory = 0.0;
    prepareMasterForSaturation(remainingTime,remainingMemory);

  load_input:
    loadInputClausesIntoMaster();
   
    GlobalStopFlag::check();

    if (_master.terminated())
      {
	reportTermination();
	_timer.stop();
	_saturationRunning = false; 
	_continuation = Continuation_LoadInput;
	return;
      };
    _continuation = Continuation_Init;

  main_loop:
    _mainLoopRunning = true;
    _master.mainLoop();
    _mainLoopRunning = false;
    reportTermination();
    _timer.stop();
    _saturationRunning = false; 
    _continuation = Continuation_MainLoop;
  }; // void preprocessAndRunMainLoop()

  void lookForAnotherAnswer()
  {
    CALL("lookForAnotherAnswer()");
    ASSERT(refutation());
    ASSERT(!_timer.isTicking());
    ASSERT(!_saturationRunning);
    GlobalStopFlag::reset();
    _lookingForMoreAnswers = true;
    _timer.start();
    _saturationRunning = true;
    GlobalStopFlag::HandlerActivator 
      globalStopFlagHandlerActivator(globalStopFlagHandler);
    GlobalClock::AlarmHandler alarmHandler(handleAlarm);

    switch (_continuation)
      {
      case Continuation_Prepro_Simplification:
	// same as below
      case Continuation_Prepro_DefElimination_LoadInput:
	// same as below
      case Continuation_Prepro_DefElimination_DefElimination:
	// same as below
      case Continuation_LoadInput:
	// same as below	
      case Continuation_MainLoop:
	if (_options.std_output)
	  *_options.std_output << "% Resuming saturation in attempt to find another answer...\n";
	preprocessAndRunMainLoop();
	break;
      default:
	ICP("ICP0");
	RuntimeError::report("Bad _continuation in VampireKernel::_Implementation::lookForAnotherAnswer()");
      };
  }; // void lookForAnotherAnswer()


  void suspend()
  {
    if (_saturationRunning)
      {
	_master.suspend();
	_timer.stop();
      };
        
    if (_options.std_output)
      *_options.std_output << "% Kernel session suspended.\n";   
    outputStatistics();
  };

  void resume()
  {
    if (_options.std_output)
      *_options.std_output << "% Kernel session resumed.\n";
    if (_saturationRunning)
      {
	_timer.start();
	_master.resume();
      };
  }; 


  void reportInterrupt(int signalNumber)
  {
    CALL("reportInterrupt(int signalNumber)");
    _terminationReason = VampireKernel::AbortedOnExternalInterrupt;
    
    if (_options.std_output)
      {
	outputStatistics();
	*_options.std_output << "% Proof not found. Aborted on external signal " 
			     << signalNumber << ".\n";
	_options.std_output->flush();
      };
  }; // void reportInterrupt(int signalNumber)

  void reportErrorSignal(int signalNumber)
  {
    CALL("reportErrorSignal(int signalNumber)");
    _terminationReason = VampireKernel::AbortedOnErrorSignal;
    if (_options.std_output)
      *_options.std_output << "% Proof not found. Kernel session aborted on error signal " 
			   << signalNumber << ".\n";
  }; // void reportErrorSignal(int signalNumber)

  
  void reportError()
  {
    CALL("reportError()");
    _terminationReason = VampireKernel::AbortedOnError;
    
    if (_options.std_output)
      *_options.std_output << "% Proof not found. Kernel session aborted due to an error.\n";
  }; // void reportError(const char* errorQualifier,const char* errorMessage)

  VampireKernel::TerminationReason terminationReason()
  {
    return _terminationReason;
  };

  const VampireKernel::Clause* refutation() const
  {
    return (VampireKernel::Clause*)_master.proof();
  };


  bool isNumericConstantId(ulong symbolId) const
    {
      return _signature.isNumericConstant(symbolId);
    };
  bool isInputSymbol(ulong symbolId) const
  {
    return !_signature.isSkolem(symbolId);
  }; 
  bool isAnswerPredicateId(ulong symbolId) const
  {
    return _signature.isAnswerPredicate(symbolId);
  };
  ulong symbolArity(ulong symbolId) const
    {
      return _signature.arity(symbolId);
    };
  const char* symbolPrintName(ulong symbolId) const
    {
      CALL("symbolPrintName(ulong symbolId) const");
      ASSERT(!isNumericConstantId(symbolId));
      return _signature.symbolName(symbolId);
    }; 
  const double& numericConstantValue(ulong symbolId) const
    {
      CALL("numericConstantValue(ulong symbolId) const");
      ASSERT(isNumericConstantId(symbolId));
      return _signature.numericConstantValue(symbolId);
    }; 


private:
  _Implementation() 
  {
  };
  void openSession()
  {
    _currentSession = this;
  };

  static void handleAlarm(int)
  {
    if (_currentSession && _currentSession->_saturationRunning)
      {	
	if (_currentSession->_timer.elapsedDeciseconds() > _currentSession->_saturationTimeLimit)
	  {
	    GlobalStopFlag::raise();
	  }
	else
	  {
	    if (_currentSession->_mainLoopRunning)
	      {
		_currentSession->_master.externalTick();
	      };
	  };
      };
  };

  static void globalStopFlagHandler()
  {
    CALL("globalStopFlagHandler()");
    if (_currentSession) 
      { 
	_currentSession->reportTimeLimitExpired();
	DestructionMode::makeFast();
	throw VampireKernel::Emergency(VampireKernel::Emergency::TimeLimit);
      };
  }; // void globalStopFlagHandler()

  void preprocessInputClauses()
  {
    CALL("preprocessInputClauses()"); 

    switch (_continuation)
      {
      case Continuation_Init:
	break;
      case Continuation_Prepro_Simplification:
	if (_options.std_output)
	  *_options.std_output << "% Resuming preprocessing by saturation "
			       << _preproIterationNumber << "\n";
	goto simplify;
      case Continuation_Prepro_DefElimination_LoadInput:
	// same as below
      case Continuation_Prepro_DefElimination_DefElimination:
	if (_options.std_output)
	  *_options.std_output << "% Resuming preprocessing by def. elimination "
			       << _preproIterationNumber << "\n";
	goto eliminate_definitions;
      default:
	ICP("ICP0");
	RuntimeError::report("Bad _continuation in VampireKernel::_Implementation::preprocessInputClauses()");
      };



    _preproStatSplit = 0UL;
    _preproStatComponents = 0UL;
    _preproStatDifferentComponents = 0UL;
    _preproIterationNumber = 0UL;
    _preproEliminationCycles = 0L;

  simplify:
    preprocessBySimplification(_preproIterationNumber);
    collectPreprocessingIterationStatistics();
    _problemProfile.reset();
    _problemProfile.load(_inputClauses);
    
    if (_options.show_profile && _options.std_output)
      {
	*_options.std_output << "%========= Profile of preprocessed (" 
			     << _preproIterationNumber << "):\n"; 
	_problemProfile.output(*_options.std_output);
	*_options.std_output << "%========= End of profile for preprocessed (" 
			     << _preproIterationNumber << ") ========\n";
      }; 
    if (_master.terminated()) 
      {
	_continuation = Continuation_Prepro_Simplification;
	return;
      };
    _continuation = Continuation_Init;

    if (_problemProfile.numOfEliminated() && 
	(_preproEliminationCycles < _options.elim_def))
      { 
      next_elimination_cycle:
	_preproIterationNumber++;
	_preproEliminationCycles++;

      eliminate_definitions:
	preprocessByDefElimination(_preproIterationNumber);
	collectPreprocessingIterationStatistics();
	_problemProfile.reset();
	_problemProfile.load(_inputClauses);
	if (_options.show_profile && _options.std_output)
	  {
	    *_options.std_output << "%========= Profile of preprocessed (" 
				 << _preproIterationNumber << "):\n"; 
	    _problemProfile.output(*_options.std_output);
	    *_options.std_output << "%========= End of profile for preprocessed (" 
				 << _preproIterationNumber << ") ========\n";
	  }; 
	if (_master.terminated()) 
	  {
	    return;
	  };
      
	_continuation = Continuation_Init;
	if (_problemProfile.numOfEliminated() && 
	    (_preproEliminationCycles < _options.elim_def))
	  goto next_elimination_cycle;
	goto simplify;
      };
    // end of story
    if (_options.std_output)
      *_options.std_output << "%======== Preprocessing has been done in " << _preproIterationNumber + 1 << " stages.\n";
    
  }; // void preprocessInputClauses()

  void preprocessBySimplification(ulong iterationNumber)
  {
    CALL("preprocessBySimplification(ulong iterationNumber)");    
    switch (_continuation)
      {
      case Continuation_Init:
	break;
      case Continuation_Prepro_Simplification:
	goto load_input;
      default:
	ICP("ICP0");
	RuntimeError::report("Bad _continuation in VampireKernel::_Implementation::preprocessInputClauses()");
      };
    
    if (_options.std_output)
      *_options.std_output << "%=============== Preprocessing by simplification " 
			   << iterationNumber << " ============\n";
    prepareMasterForSimplification();
    
  load_input:
    loadInputClausesIntoMaster();
    if (_master.terminated()) return;
    _continuation = Continuation_Init;
    extractPreprocessedClauses();
    if (_options.show_prepro && _options.std_output) 
      {
	
	*_options.std_output << "%=============== Preprocessing  by simplification " 
	     << iterationNumber << " statistics: ============\n";
	outputStatistics();
	*_options.std_output << "%=============== Clauses preprocessed by simplification " 
	     << iterationNumber << " ================\n";
	outputInputClauses();
	*_options.std_output << "%============== End of clauses preprocessed by " 
			     << iterationNumber << "  ==========\n";
      };
  }; // void preprocessBySimplification(ulong iterationNumber)

  void preprocessByDefElimination(ulong iterationNumber)
  {
    CALL("preprocessByDefElimination(ulong iterationNumber)");    

    switch (_continuation)
      {
      case Continuation_Init:
	break;
      case Continuation_Prepro_DefElimination_LoadInput:
	goto load_input;
      case Continuation_Prepro_DefElimination_DefElimination:
	ASSERT_IN(_inputClauses.empty(),"CONT WITH ELIM");
	goto eliminate_definitions;
      default:
	ICP("ICP0");
	RuntimeError::report("Bad _continuation in VampireKernel::_Implementation::preprocessInputClauses()");
      };


    if (_options.std_output) 
      *_options.std_output << "%=============== Preprocessing by def. elimination " 
			   << iterationNumber << " ============\n";
    prepareMasterForDefElimination();

  load_input:

    loadInputClausesIntoMaster();
    if (_master.terminated()) 
      {
	_continuation = Continuation_Prepro_DefElimination_LoadInput;
	return;
      };  
    _continuation = Continuation_Init;

  eliminate_definitions:
    ASSERT_IN(_inputClauses.empty(),"ELIMINATE");
  
    _master.eliminateDefinitions();
    if (_master.terminated())
      {
	ASSERT_IN(_inputClauses.empty(),"PROVED BY DEF ELIM");
	_continuation = Continuation_Prepro_DefElimination_DefElimination;

	return;
      };
    _continuation = Continuation_Init;
    extractPreprocessedClauses(); 

    if (_options.show_prepro && _options.std_output) 
      {
	*_options.std_output << "%=============== Preprocessing  by def. elimination " 
			     << iterationNumber << " statistics: ============\n";
	outputStatistics();
        *_options.std_output << "%=============== Clauses preprocessed by def. elimination " 
			     << iterationNumber << " ================\n";
	outputInputClauses();
	*_options.std_output << "%============== End of clauses preprocessed by " 
			     << iterationNumber << " ==========\n";
      };

  }; // void preprocessByDefElimination(ulong iterationNumber)

  void prepareMasterForSimplification()
  {
    CALL("prepareMasterForSimplification()");
    _master.reset();
    _master.options.main_alg = 0;
    _master.options.binary_resolution = false; 
    _master.options.no_inferences_between_rules = false;
    _master.options.splitting = _options.static_splitting;
    _master.options.splitting_style = _options.splitting_style;
    _master.options.splitting_with_naming = _options.splitting_with_naming;  
    _master.options.neg_eq_splitting = _options.static_neg_eq_splitting;  
    _master.options.paramodulation = false;
    _master.options.sim_back_sup = 0;
    _master.options.selection = _options.selection;
    _master.options.literal_comparison_mode = _options.literal_comparison_mode;
    _master.options.inherited_negative_selection = _options.inherited_negative_selection;

    // strategy switches        
    _master.options.handle_equality = true;
    _master.options.forward_subsumption = true;
    _master.options.fs_set_mode = true; // not to worry about completeness here  
    _master.options.fs_optimised_multiset_mode = false;
    _master.options.fs_use_sig_filters = false;
    _master.options.fs_old_style = false;
    _master.options.simplify_by_fs = _options.simplify_by_fs;
    _master.options.forward_demodulation = 0; 
    _master.options.fd_on_splitting_branches = false;
    if (_options.normalize_by_commutativity)
      {
	_master.options.normalize_by_commutativity = _problemProfile.commutativityTable();
      }
    else 
      _master.options.normalize_by_commutativity = (const VK::SymbolInfo*)0;
    _master.options.backward_demodulation = 0;
    _master.options.bd_on_splitting_branches = false;
    _master.options.simplify_by_eq_res = true;
    _master.options.backward_subsumption = true;
    _master.options.bs_set_mode = true; // not to worry about completeness here  
    _master.options.bs_optimised_multiset_mode = false;
    _master.options.orphan_murder = 0;
    _master.options.lrs = 0;
    _master.options.pred_elim_table = (const VK::SymbolInfo*)0;
    // numeric options
    _master.options.age_weight_ratio = 0;
    _master.options.timeLimit = LONG_MAX/10;
    _master.options.memoryLimit = (float)LONG_MAX;
    _master.options.allocation_buffer_price = (float)_options.allocation_buffer_price;
    _master.options.maxActive = LONG_MAX;
    _master.options.maxPassive = LONG_MAX;
    _master.options.maxWeight = LONG_MAX;
    _master.options.first_time_check = 1.0; 
    _master.options.first_mem_check = 1.0;
    // interface options
    _master.options.outputStreamForGenerated = (_options.show_gen) ? _options.std_output : ((ostream*)0);
    _master.options.showPassive = (_options.show_passive) ? outputClause : 0;
    _master.options.showActive = (_options.show_active) ? outputClause : 0;
    _master.options.showWeightChange = (_options.show_weight_limit_changes) ? outputWeightChange : 0;
    _master.setStrategy();
  }; // void prepareMasterForSimplification()

  void prepareMasterForDefElimination()
  {
    CALL("prepareMasterForDefElimination()");
    _master.reset();

    _master.options.main_alg = 0;
    _master.options.binary_resolution = true; // for elimination of definitions
    _master.options.no_inferences_between_rules = false;
    _master.options.splitting = false;   
    _master.options.splitting_style = 1L;
    _master.options.splitting_with_naming = false; 
    _master.options.neg_eq_splitting = 0L; // off  
    _master.options.paramodulation = false;
    _master.options.sim_back_sup = 0;
    _master.options.selection = _options.selection;
    _master.options.literal_comparison_mode = _options.literal_comparison_mode;
    _master.options.inherited_negative_selection = _options.inherited_negative_selection;

    // strategy switches        
    _master.options.handle_equality = true;
    _master.options.forward_subsumption = true;
    _master.options.fs_set_mode = true; // not to worry about completeness here  
    _master.options.fs_optimised_multiset_mode = false;
    _master.options.fs_use_sig_filters = false;
    _master.options.fs_old_style = false;
    _master.options.simplify_by_fs = _options.simplify_by_fs;
    _master.options.forward_demodulation = 0;
    _master.options.fd_on_splitting_branches = false;
    if (_options.normalize_by_commutativity)
      {
	_master.options.normalize_by_commutativity = _problemProfile.commutativityTable();
      }
    else 
      _master.options.normalize_by_commutativity = (const VK::SymbolInfo*)0;
    _master.options.backward_demodulation = 0;
    _master.options.bd_on_splitting_branches = false;
    _master.options.simplify_by_eq_res = true;
    _master.options.backward_subsumption = true;
    _master.options.bs_set_mode = true; // not to worry about completeness here  
    _master.options.bs_optimised_multiset_mode = false;
    _master.options.orphan_murder = 0;
    _master.options.lrs = 0;
    _master.options.pred_elim_table = _problemProfile.eliminationTable();
    // numeric options
    _master.options.age_weight_ratio = 0;
    _master.options.timeLimit = LONG_MAX/10;
    _master.options.memoryLimit = (float)LONG_MAX;
    _master.options.allocation_buffer_price = (float)_options.allocation_buffer_price;
    _master.options.maxActive = LONG_MAX;
    _master.options.maxPassive = LONG_MAX;
    _master.options.maxWeight = LONG_MAX;
    _master.options.first_time_check = 1.0;
    _master.options.first_mem_check = 1.0;
    // interface options
    _master.options.outputStreamForGenerated = (_options.show_gen) ? _options.std_output : ((ostream*)0);
    _master.options.showPassive = (_options.show_passive) ? outputClause : 0;
    _master.options.showActive = (_options.show_active) ? outputClause : 0;
    _master.options.showWeightChange = (_options.show_weight_limit_changes) ? outputWeightChange : 0;
    _master.setStrategy();
  }; // void prepareMasterForDefElimination()

  void loadInputClausesIntoMaster()
  {
    CALL("loadInputClausesIntoMaster()");

    VK::Clause* cl;
    switch (_continuation)
      {
      case Continuation_Init:
	break;
      case Continuation_Prepro_Simplification:
	// same as below
      case Continuation_Prepro_DefElimination_LoadInput:
	// same as below
      case Continuation_LoadInput:
	goto load_clause;
      default:
	ICP("ICP0");
	RuntimeError::report("Bad _continuation in VampireKernel::_Implementation::loadInputClausesIntoMaster()");
      };

  next_clause:
    if (_inputClauses.dequeue(cl))
      {
	_preprocessingHistory.enqueue(cl);
      load_clause:
	_master.inputClause(cl);
	if (_master.terminated()) 
	  {
	    return;
	  };
	_continuation = Continuation_Init;
	goto next_clause;
      };
    _master.endOfInput();
    ASSERT(_inputClauses.empty());
  }; // void loadInputClausesIntoMaster()


  void extractPreprocessedClauses()
  {
    CALL("extractPreprocessedClauses()");


    ASSERT(_inputClauses.empty());
    _master.passiveResetIter();
    _master.historyResetIter();
    VK::Clause* cl;
    for (cl = _master.nextPassive(); cl; cl = _master.nextPassive())
      {
	ASSERT(!cl->isBuiltInTheoryFact());
	VK::Clause* copy = cl->unsharedCopy();
	_numberToClauseMapping.insert(copy);
	_inputClauses.enqueue(copy);
      };
    for (cl = _master.nextHistory(); cl; cl = _master.nextHistory())
      {
	VK::Clause* copy = cl->unsharedCopy();
	_numberToClauseMapping.insert(copy);
	_preprocessingHistory.enqueue(copy); 
      };
    
    for (const ClauseQueue::Entry* qe = _inputClauses.begin(); 
	 qe; 
	 qe = qe->next())
      {
	cl = qe->value();
	if ((!cl->isInputClause()) &&
	    (!cl->isBuiltInTheoryFact()))
	  {
	    for (ClauseBackground::ClauseList* ancestors = 
		   cl->background().ancestors(); 
		 ancestors; 
		 ancestors = ancestors->tl())
	      {
		VK::Clause* ancClause = ancestors->hd();
		VK::Clause* ancCopy = (VK::Clause*)_numberToClauseMapping.find(ancClause->Number());
		if (ancCopy)  { ancestors->hd() = ancCopy; }
		else 
		  {
		    ASSERT(ancClause->isInputClause() || ancClause->isBuiltInTheoryFact());
		  }; 
	      };  
	  };

      };

    for (const ClauseQueue::Entry* qe = _preprocessingHistory.begin(); 
	 qe; 
	 qe = qe->next())
      {
	cl = qe->value(); 
	if ((!cl->isInputClause()) &&
	    (!cl->isBuiltInTheoryFact()))
	  {
	    for (ClauseBackground::ClauseList* ancestors = 
		   cl->background().ancestors(); 
		 ancestors; 
		 ancestors = ancestors->tl())
	      {
		VK::Clause* ancClause = ancestors->hd();
		VK::Clause* ancCopy = (VK::Clause*)_numberToClauseMapping.find(ancClause->Number());
		if (ancCopy)  { ancestors->hd() = ancCopy; }
		else 
		  {
		    ASSERT(ancClause->isInputClause() || ancClause->isBuiltInTheoryFact());
		  }; 
	      };
	  };
      };
  }; // void extractPreprocessedClauses()

  void prepareMasterForSaturation(long timeLimit, // deciseconds
				  float memoryLimit) // bytes
  {
    _master.reset();
    _master.options.incompleteAPriori = _incompleteProcedure;
    _master.options.main_alg = _options.main_alg;
    _master.options.binary_resolution = true;
    _master.options.no_inferences_between_rules = _options.no_inferences_between_rules;
    _master.options.splitting = _options.dynamic_splitting;  
    _master.options.splitting_style = _options.splitting_style;
    _master.options.splitting_with_naming = _options.splitting_with_naming;
    _master.options.neg_eq_splitting = 0L; // off  
    _master.options.paramodulation = _options.paramodulation;
    _master.options.sim_back_sup = _options.sim_back_sup;
    _master.options.selection = _options.selection;
    _master.options.literal_comparison_mode = _options.literal_comparison_mode;
    _master.options.inherited_negative_selection = _options.inherited_negative_selection;

    // strategy switches
    _master.options.handle_equality = _options.paramodulation;
    _master.options.forward_subsumption = _options.forward_subsumption;
    _master.options.fs_set_mode = _options.fs_set_mode;
    _master.options.fs_optimised_multiset_mode = _options.fs_optimised_multiset_mode;
    _master.options.fs_use_sig_filters = _options.fs_use_sig_filters;
    _master.options.fs_old_style = _options.fs_old_style;
    _master.options.simplify_by_fs = _options.simplify_by_fs;
    _master.options.forward_demodulation = _options.forward_demodulation;
    _master.options.fd_on_splitting_branches = _options.fd_on_splitting_branches;
    if (_options.normalize_by_commutativity)
      {
	_master.options.normalize_by_commutativity = _problemProfile.commutativityTable();
      }
    else
      {
	_master.options.normalize_by_commutativity = (const VK::SymbolInfo*)0;
      };
    _master.options.backward_demodulation = _options.backward_demodulation;
    _master.options.bd_on_splitting_branches = _options.bd_on_splitting_branches;
    _master.options.simplify_by_eq_res = _options.simplify_by_eq_res;
    _master.options.backward_subsumption = _options.backward_subsumption;
    _master.options.bs_set_mode = _options.bs_set_mode;
    _master.options.bs_optimised_multiset_mode = _options.bs_optimised_multiset_mode;
    _master.options.orphan_murder = _options.orphan_murder;
    _master.options.lrs = _options.lrs;

    _master.options.pred_elim_table = (const VK::SymbolInfo*)0;

    // numeric options
    _master.options.age_weight_ratio = _options.age_weight_ratio;
    _master.options.timeLimit = timeLimit;
    _master.options.memoryLimit = memoryLimit;
    _master.options.allocation_buffer_price = (float)_options.allocation_buffer_price;
    _master.options.maxActive = _options.max_active;
    _master.options.maxPassive = _options.max_passive;
    _master.options.maxWeight = _options.max_weight;
    _master.options.maxInferenceDepth = _options.max_inference_depth;
    _master.options.first_time_check = ((float)_options.first_time_check)/100;
    _master.options.first_mem_check = ((float)_options.first_mem_check)/100;
    // interface options 
    _master.options.outputStreamForGenerated = (_options.show_gen) ? _options.std_output : ((ostream*)0);
    _master.options.showPassive = (_options.show_passive) ? outputClause : 0;
    _master.options.showActive = (_options.show_active) ? outputClause : 0;
    _master.options.showWeightChange = (_options.show_weight_limit_changes) ? outputWeightChange : 0;
    _master.setStrategy();
#ifdef INTERCEPT_BACKWARD_MATCHING_INDEXING
    BMILog.unblock();
#endif

#ifdef INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING
    BSILog.unblock();
#endif
  }; // void prepareMasterForSaturation(long timeLimit,float memoryLimit)


  void reportTermination()
  {
    CALL("reportTermination()");
    
    outputStatistics();
    switch (_master.terminationReason())
      {
      case Master::UnknownTR:
	ICP("ICP0");
	if (_options.std_output)
	  *_options.std_output << "% Proof not found. Internal error: unknown termination type.\n"; 
	_terminationReason = VampireKernel::UnknownTerminationReason;
	break;  
      case Master::Proof: 
	if (_options.std_output)
	  *_options.std_output << "% Proof found. Thanks to Tanya!\n"; 
	_terminationReason = VampireKernel::RefutationFound;
	if (_options.proof && _options.std_output)
	  {
	    _proofTree.collect(_master.proof());    
	    *_options.std_output << "%================== Proof: ======================\n"; 
	    _proofTree.resetIter();   
	    for (const VK::Clause* cl = _proofTree.getNextClause(); 
		 cl; 
		 cl = _proofTree.getNextClause())
	      *_options.std_output << "% " << cl << '\n';
	    
	    *_options.std_output << "%==============  End of proof. ==================\n"; 
	  }; 
	break; 
      case Master::EmptyPassive: 
	if (_master.completeStrategyUsed() && 
	    (!_lookingForMoreAnswers))
	  {
	    if (_options.std_output)
	      *_options.std_output << "% Unprovable.\n";  
	    _terminationReason = VampireKernel::Satisfiable;
	  }
	else 
	  { 
	    if (_options.std_output)
	      *_options.std_output << "% Proof not found. Empty passive.\n"; 
	    _terminationReason = VampireKernel::EmptyPassive;
	  };
	break;

      case Master::MaxPassive: 
	if (_options.std_output)
	  *_options.std_output << "% Proof not found. Max. num. of passive reached. \n";
	_terminationReason = VampireKernel::MaxPassive;
	break; 
      case Master::MaxActive: 
	if (_options.std_output)
	  *_options.std_output << "% Proof not found. Max. num. of active reached. \n"; 
	_terminationReason = VampireKernel::MaxActive;
	break; 
      }; 
    if (_options.std_output) _options.std_output->flush();
  }; // void reportTermination()  

  void reportTimeLimitExpired()
  {
    CALL("reportTimeLimitExpired()");  
    _terminationReason = VampireKernel::TimeLimitExpired;
    if (_saturationRunning)
      {
	outputStatistics();
      };
    if (_options.std_output)
      {
	*_options.std_output << "% Proof not found. Time limit expired.\n";
	_options.std_output->flush();
      };
  }; // void reportTimeLimitExpired()


  static void setHardMemoryLimit(float lim) // bytes
  {
    CALL("setHardMemoryLimit(float lim)");
    if (lim < 0.0) lim = 0.0;
    long truncated;
    if (lim > (LONG_MAX/2))
      {
	truncated = LONG_MAX/2;
      }
    else
      truncated = (long)lim;
    GlobAlloc::setHardMemoryLimit(truncated);
    GlobAlloc::basicAllocationFailureHandling().setMemoryReleaseRequest(releaseMemory);
    GlobAlloc::basicAllocationFailureHandling().setUnrecoverableAllocationFailureHandler(handleAllocationFailure);
  }; // setHardMemoryLimit(float lim

  static void setAllocationBufferSize(long percentage)
  {
    GlobAlloc::setAllocationBufferSize(percentage);
  }; 

  static bool releaseMemory(ulong size)
  {
    CALL("releaseMemory(ulong size)");
    if (_currentSession && (_currentSession->_mainLoopRunning))
      {
	return _currentSession->_master.releaseMemory(size);
      }
    else
      return false;
  }; // bool releaseMemory(ulong size)
  
  static void handleAllocationFailure(ulong size)
  {
    CALL("handleAllocationFailure(ulong size)");
    if (_currentSession) 
      {
	_currentSession->reportAllocationFailure(size);
	DestructionMode::makeFast();
	throw VampireKernel::Emergency(VampireKernel::Emergency::AllocationFailure);
      }
    else
      {
	cout << "% Allocation failure occured when no kernel session is running.\n";
	DestructionMode::makeFast();
	throw VampireKernel::Emergency(VampireKernel::Emergency::AllocationFailure);
      };
  }; // void handleAllocationFailure(ulong size)

  void reportAllocationFailure(ulong size)
  {
    CALL("reportAllocationFailure(ulong size)");
    _terminationReason = VampireKernel::AbortedOnAllocationFailure;
    if (_options.std_output)
      {
	*_options.std_output << "% ------------- ALLOCATION FAILURE: " 
			     << size << " bytes -----------------\n";
	outputStatistics();
	*_options.std_output << "% Proof not found. Allocation failure.\n";
	_options.std_output->flush();
      };
  }; // void reportAllocationFailure(ulong size)

  static void outputClause(const VK::Clause* cl)
  {
    if (_currentSession && (_currentSession->_options.std_output))
      *(_currentSession->_options.std_output) << "% " << cl << "\n";
  };
  
  static void outputWeightChange(long oldWL,long newWL)
  {
    if (_currentSession && (_currentSession->_options.std_output))
     *(_currentSession->_options.std_output) << "% WEIGHT LIMIT CHANGED: " << oldWL << " ------> " << newWL << "\n";
  };


private:
  enum Continuation
  {
    Continuation_Init,
    Continuation_Prepro_Simplification,
    Continuation_Prepro_DefElimination_LoadInput,
    Continuation_Prepro_DefElimination_DefElimination,
    Continuation_LoadInput,
    Continuation_MainLoop
  };


private:
  void outputInputClauses()
  {
    if (!_options.std_output) return;
    for (const ClauseQueue::Entry* cl = _inputClauses.begin();
	 cl;
	 cl = cl->next())
      {
	*_options.std_output << "% " << cl->value() << "\n";
      };
  };
  
  void outputStatistics()
  {
    if (!_options.std_output) return;
    *_options.std_output << "%================= Statistics: =======================\n";
    *_options.std_output << "% time " << _timer.elapsedSeconds() 
	 << " (" << _master.timer().elapsedSeconds() << " current loop)\n";
    *_options.std_output << "% memory " << (GlobAlloc::occupiedByObjects()/1024) 
	 << "Kb (" << (GlobAlloc::occupiedByChunks()/1024) << "Kb in chunks), " 
	 << "total buffer deficit " << (GlobAlloc::totalBufferDeficit()/1024) << "Kb\n"; 
    *_options.std_output << "%=== Generated clauses:\n";
    *_options.std_output << "%      total " << _master.statGenerated() << '\n';
    *_options.std_output << "%      forward subsumed " << _master.statFSubsumed() << '\n';
    *_options.std_output << "%      eq.tautologies   " << _master.statEqTautologies() << '\n';
    *_options.std_output << "%      prop.tautologies " << _master.statPropTautologies() << '\n';
    *_options.std_output << "%      simplified " << _master.statFSimplified() << '\n';
    *_options.std_output << "%         by FSR          " << _master.statFSimplifiedByFSR() << '\n';
    *_options.std_output << "%         by demod.       " << _master.statFSimplifiedByDemod() << '\n';
    *_options.std_output << "%               on splitting branches " << _master.statFDOnSplittingBranches() << '\n';
    *_options.std_output << "%         by built-in th. " << _master.statSimplifiedByBuiltInTheories() << '\n'; 
    *_options.std_output << "%         by eq. res.     " << _master.statFSimplifiedByEqRes() << '\n';
    *_options.std_output << "%      split " << _master.statSplit() 
	 << "  avg.components " << _master.statAvgComponents() 
	 << "  diff.components " << _master.statDiffComponents() << '\n';
    *_options.std_output << "%      useless:\n";
    *_options.std_output << "%        too deep inferences " << _master.statTooDeepInferences() << '\n';
    *_options.std_output << "%        too heavy " << _master.statTooHeavy() << '\n'; 
    *_options.std_output << "%        too heavy literals " << _master.statTooHeavyLiterals() << '\n'; 
    *_options.std_output << "%        too long " << _master.statTooLong() << '\n';  
    *_options.std_output << "%        too deep " << _master.statTooDeep() << '\n';  
    *_options.std_output << "%        too many variables " << _master.statTooManyVariables() << '\n';  
    *_options.std_output << "%        refused allocation " << _master.statRefusedAllocation() << '\n';

    *_options.std_output << "%=== Retained clauses:\n";
    *_options.std_output << "%      total " << _master.statRetained() << '\n';
    *_options.std_output << "%      used " << _master.statUsed() << '\n';
    *_options.std_output << "%      back subsumed " << _master.statBSubsumed() << '\n';
    *_options.std_output << "%      back simplified " << _master.statBSimplified() << '\n';
    *_options.std_output << "%        on splitting branches " << _master.statBDOnSplittingBranches() << '\n';
    *_options.std_output << "%      rewritten by def. unfolding " << _master.statRewrittenByDefUnfolding() << '\n';
    *_options.std_output << "%      orphans murdered " << _master.statOrphansMurdered() << '\n';
    *_options.std_output << "%               passive " << _master.statPassiveOrphansMurdered() << '\n';
    *_options.std_output << "%               active  " << _master.statActiveOrphansMurdered() << '\n';
    *_options.std_output << "%      recycled " << _master.statRecycled() << '\n';
    *_options.std_output << "%      currently passive " << _master.statCurrentlyPassive()        
	 << " (" << _master.statPassiveReachable() << " reachable)\n";
    *_options.std_output << "%      reserved passive " << _master.statReservedPassive() << '\n';
    *_options.std_output << "%      currently active " << _master.statCurrentlyActive() << '\n'; 

    *_options.std_output << "%========== End of statistics. ======================\n";
    _options.std_output->flush();
  }; // void outputStatistics()

  void tabulateStatistics()
  {
    _tabulation->stat("time",_timer.elapsedSeconds());
    _tabulation->stat("memory",GlobAlloc::occupiedByObjects()/1024);
    _tabulation->stat("generated",_master.statGenerated());
    _tabulation->stat("eq_tautologies",_master.statEqTautologies());
    _tabulation->stat("prop_tautologies",_master.statPropTautologies());
    _tabulation->stat("forward_subsumed",_master.statFSubsumed());
    _tabulation->stat("forward_simplified",_master.statFSimplified());
    _tabulation->stat("forward_demodulated_on_splitting_branches",_master.statFDOnSplittingBranches());
    _tabulation->stat("too_heavy_literals",_master.statTooHeavyLiterals());
    _tabulation->stat("too_deep",_master.statTooDeep());
    _tabulation->stat("too_heavy_clauses",_master.statTooHeavy());
    _tabulation->stat("too_long_clauses",_master.statTooLong());
    _tabulation->stat("too_many_variables",_master.statTooManyVariables());
    _tabulation->stat("backward_subsumed",_master.statBSubsumed());
    _tabulation->stat("backward_simplified",_master.statBSimplified());
    _tabulation->stat("backward_demodulated_on_splitting_branches",_master.statBDOnSplittingBranches());
    _tabulation->stat("rewritten_by_def_unfolding",_master.statRewrittenByDefUnfolding());
    _tabulation->stat("passive_orphans_murdered",_master.statPassiveOrphansMurdered()); 
    _tabulation->stat("active_orphans_murdered",_master.statActiveOrphansMurdered()); 
    _tabulation->stat("used",_master.statUsed());
    _tabulation->stat("kept_total",_master.statRetained());
    _tabulation->stat("solved_by_preprocessing",_solvedByPreprocessing);
    _tabulation->stat("split",_master.statSplit()); 
    _tabulation->stat("different_components",_master.statDiffComponents());
    _tabulation->stat("average_components",_master.statAvgComponents());
  }; // void tabulateStatistics()


  void tabulatePreprocessingStatistics()
  {
    _tabulation->stat("prepro_split",_preproStatSplit); 
    _tabulation->stat("prepro_different_components",
		      _preproStatDifferentComponents);
    _tabulation->stat("prepro_average_components",
		      (_preproStatSplit) ? 
		      (((float)_preproStatComponents)/_preproStatSplit) :
		      0.0);
  }; // void tabulatePreprocessingStatistics()

  const char* flagValue(bool flag)
  {
    if (flag) return "on";
    return "off";
  };

  void tabulateOptions()
  {
    CALL("tabulateOptions()");
    if (!_tabulation) return;
    // main algorithm
   
    _tabulation->option("main_alg",_options.main_alg);

    // inference rules
    _tabulation->option("no_inferences_between_rules",flagValue(_options.no_inferences_between_rules));
    _tabulation->option("static_splitting",flagValue(_options.static_splitting));
    _tabulation->option("dynamic_splitting",flagValue(_options.dynamic_splitting));
    _tabulation->option("splitting_style",_options.splitting_style);
    _tabulation->option("splitting_with_naming",flagValue(_options.splitting_with_naming));
    _tabulation->option("static_neg_eq_splitting",flagValue(_options.static_neg_eq_splitting));
    _tabulation->option("paramodulation",flagValue(_options.paramodulation));
    _tabulation->option("sim_back_sup",_options.sim_back_sup);
    _tabulation->option("selection",_options.selection);
    _tabulation->option("literal_comparison_mode",_options.literal_comparison_mode);
    _tabulation->option("inherited_negative_selection",flagValue(_options.inherited_negative_selection));
     
    // strategy 
    _tabulation->option("forward_subsumption",flagValue(_options.forward_subsumption));
    _tabulation->option("fs_set_mode",flagValue(_options.fs_set_mode));
    _tabulation->option("fs_optimised_multiset_mode",flagValue(_options.fs_optimised_multiset_mode));
    _tabulation->option("fs_use_sig_filters",flagValue(_options.fs_use_sig_filters));
    _tabulation->option("fs_old_style",flagValue(_options.fs_old_style));
    _tabulation->option("simplify_by_fs",_options.simplify_by_fs);
    _tabulation->option("forward_demodulation",_options.forward_demodulation);
    _tabulation->option("fd_on_splitting_branches",flagValue(_options.fd_on_splitting_branches));
    _tabulation->option("normalize_by_commutativity",flagValue(_options.normalize_by_commutativity));
    _tabulation->option("backward_demodulation",_options.backward_demodulation);
    _tabulation->option("bd_on_splitting_branches",flagValue(_options.bd_on_splitting_branches));
    _tabulation->option("simplify_by_eq_res",flagValue(_options.simplify_by_eq_res));
    _tabulation->option("backward_subsumption",_options.backward_subsumption);
    _tabulation->option("bs_set_mode",flagValue(_options.bs_set_mode));
    _tabulation->option("bs_optimised_multiset_mode",flagValue(_options.bs_optimised_multiset_mode));
    _tabulation->option("orphan_murder",flagValue(_options.orphan_murder));
    _tabulation->option("lrs",_options.lrs);
    _tabulation->option("elim_def",_options.elim_def);
    
    switch (_options.simplification_ordering)
      {
      case VampireKernel::Options::NonrecursiveKBOrdering:
	_tabulation->option("simplification_ordering","nonrecursive_KB");
	break;
      case VampireKernel::Options::StandardKBOrdering:
	_tabulation->option("simplification_ordering","standard_KB");
	break;
      default: 
	RuntimeError::report("Wrong value of --simplification_ordering.");
      };

    _tabulation->option("symbol_precedence_by_arity",_options.symbol_precedence_by_arity);
    _tabulation->option("header_precedence_kinky",flagValue(_options.header_precedence_kinky));
    switch (_options.term_weighting_scheme)
      {
      case VampireKernel::Options::UniformTermWeighting:
	_tabulation->option("term_weighting_scheme","uniform");
	break;
      case VampireKernel::Options::NonuniformConstantTermWeighting: 
	_tabulation->option("term_weighting_scheme","nonuniform_constant");
	break;
      default:
	RuntimeError::report("Wrong value of --term_weighting_scheme.");
      };
    _tabulation->option("symbol_weight_by_arity",_options.symbol_weight_by_arity);


    // numeric options 
    _tabulation->option("time_limit",((float)_saturationTimeLimit)/10);
    _tabulation->option("memory_limit",_options.memory_limit);
    _tabulation->option("allocation_buffer_size",_options.allocation_buffer_size); 
    _tabulation->option("allocation_buffer_price",_options.allocation_buffer_price); 
    _tabulation->option("max_skolem",_options.max_skolem);
    _tabulation->option("max_active",_options.max_active);
    _tabulation->option("max_passive",_options.max_passive);
    _tabulation->option("max_weight",_options.max_weight);
    _tabulation->option("max_inference_depth",_options.max_inference_depth);
    _tabulation->option("age_weight_ratio",_options.age_weight_ratio);
    _tabulation->option("selected_pos_eq_penalty_coefficient",_options.selected_pos_eq_penalty_coefficient);    
    _tabulation->option("nongoal_penalty_coefficient",_options.nongoal_penalty_coefficient);
    _tabulation->option("first_time_check",_options.first_time_check);   
    _tabulation->option("first_mem_check",_options.first_mem_check);   

    // interface options
    _tabulation->option("proof",flagValue(_options.proof));  
  }; // void tabulateOptions()


  void tabulate()
  {
    CALL("tabulate()");
    if (!_tabulation) return;
    tabulateOptions();
    _signature.tabulate(_tabulation);
    tabulatePreprocessingStatistics();
    tabulateStatistics();
    
    if ((_terminationReason == VampireKernel::RefutationFound) && 
	_options.proof)
      {
	// _proofTree is supposed to be collected here,
        // see reportTermination()
	_proofTree.resetIter();   
	for (const VK::Clause* cl = _proofTree.getNextClause(); 
	     cl; 
	     cl = _proofTree.getNextClause())
	  _tabulation->proofClause(cl);
      }; 
  }; // void tabulate()


  void switchBuiltinEqualityOff()
  {
    CALL("switchBuiltinEqualityOff()");
    if  (_options.paramodulation 
	 || _options.inherited_negative_selection
	 || _options.forward_demodulation
	 || _options.fd_on_splitting_branches
	 || _options.backward_demodulation
	 || _options.bd_on_splitting_branches
	 || _options.simplify_by_eq_res)
      {
	if (_options.std_output)
	  *_options.std_output << "% WARNING: No equality. All equality related features are switched off!\n";   
	_options.paramodulation = false;
	_options.inherited_negative_selection = false;
	_options.forward_demodulation = false;
	_options.fd_on_splitting_branches = false;
	_options.backward_demodulation = false;
	_options.bd_on_splitting_branches = false;
	_options.simplify_by_eq_res = false;
      };
  }; // void switchBuiltinEqualityOff()

  void analyseOptions()
  {
    CALL("analyseOptions()");
    switch (_options.selection)
      { 
      case 1006L:
      case 1007L:
      case 1008L:
      case 1009L:
      case -1009L:
	_options.inherited_negative_selection = false;
	if (_options.std_output)
	  *_options.std_output << "% WARNING: bidirectional literal selection "
			       << _options.selection 
			       << " switches inherited_negative_selection off!\n";
      };
    
    if (_options.header_precedence_kinky)
      {
	_incompleteProcedure = true;
	// may be refined in the future 
      };
    
    if ((_options.term_weighting_scheme != VampireKernel::Options::UniformTermWeighting) &&
	(_options.term_weighting_scheme != VampireKernel::Options::NonuniformConstantTermWeighting))
      {
	_incompleteProcedure = true;
	// will be refined in the future 
      };


    if (_options.fs_set_mode || _options.bs_set_mode)
      {	
	_incompleteProcedure = true;
      };
    

  }; // void analyseOptions()


  void collectPreprocessingIterationStatistics()
  {   
    _preproStatSplit += _master.statSplit();
    _preproStatComponents += _master.statComponents();
    _preproStatDifferentComponents += _master.statDiffComponents();

    if (_master.statTooHeavy() || 
	_master.statTooHeavyLiterals() || 
	_master.statTooManyVariables() || 
	_master.statTooLong() || 
	_master.statTooDeep() || 
	_master.statRecycled() || 
	_master.statRefusedAllocation())
      {
	_incompleteProcedure = true;
      }; 

  }; // void collectPreprocessingIterationStatistics()


private:  
  static VampireKernel::_Implementation* _currentSession;
  VampireKernel* _wrapper;
  VampireKernel::Options _options;
  GlobalClock::Timer _timer;
  long _saturationTimeLimit;
  bool _saturationRunning;
  bool _mainLoopRunning;
  Signature _signature;
  VK::Clause::OpenUnshared _newInputClause;
  ulong _currentInputClauseNumber;
  void* _currentInputClauseOrigin;
  ClauseQueue _inputClauses;
  bool _inputClauseRejected;
  ulong _numberOfRejectedInputClauses;
  bool _incompleteProcedure;
  ProblemProfile _problemProfile;
  Tabulation* _tabulation;
  VK::Clause::NewNumber _clauseNumberGenerator;
  WeightPolynomial::Sharing _weightPolynomialSharing;
  NonrecursiveKBOrdering _nonrecursiveKBOrdering;
  StandardKBOrdering _standardKBOrdering;
  BuiltInTheoryDispatcher* _builtInTheoryDispatcher;
  Master _master;
  ClauseQueue _preprocessingHistory;
  ClauseNumIndex _numberToClauseMapping;
  UniformTermWeighting* _uniformTermWeighting;
  NonuniformConstantTermWeighting* _nonuniformConstantTermWeighting;
  bool _solvedByPreprocessing;
  VampireKernel::TerminationReason _terminationReason;
  InferenceTree _proofTree;  
  ulong _preproStatSplit;
  ulong _preproStatComponents;
  ulong _preproStatDifferentComponents;
  bool _symbolWeightConstantPartInputIgnored;
  bool _unaryFunctionWithZeroWeightExists;
  ulong _unaryFunctionWithZeroWeight;
  Continuation _continuation;
  ulong _preproIterationNumber;
  long _preproEliminationCycles;
  bool _lookingForMoreAnswers;
  friend class VampireKernel;
}; // class VampireKernel::_Implementation

VampireKernel::_Implementation* 
VampireKernel::_Implementation::_currentSession;

//======================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_VAMPIRE_KERNEL
#define DEBUG_NAMESPACE "VampireKernel"
#endif    
#include "debugMacros.h"
//======================================================================

VampireKernel::VampireKernel() 
{
  CALL("constructor VampireKernel()");
  _implementation = (_Implementation*)0; 
};
VampireKernel::~VampireKernel() 
{
  CALL("destructor ~VampireKernel()");
  if (DestructionMode::isThorough())
    {
      destroy();
    };
};

void VampireKernel::openSession(const Options& opt) 
{
  CALL("openSession(const Options& opt)");
  ASSERT(!GlobAlloc::occupiedByObjects());
  GlobalStopFlag::reset();
  _Implementation::setHardMemoryLimit(((float)opt.memory_limit)*1024);
  _Implementation::setAllocationBufferSize(opt.allocation_buffer_size);
  _implementation = new _Implementation(opt);
  _implementation->_wrapper = this;
};

void VampireKernel::closeSession()
{
  _Implementation::closeSession();
  _implementation = (_Implementation*)0;
  TMP_LITERAL::resetStatic(); // ugly
}; // void VampireKernel::closeSession()

void VampireKernel::destroy() 
{
  CALL("destroy()");
  if (_implementation)
    {
      delete _implementation;
      _implementation = (_Implementation*)0;
    };
  TMP_LITERAL::destroyStatic(); // ugly
}; 

float VampireKernel::versionNumber()
{
  return 5.08;
};

const char* VampireKernel::versionDescription() 
{
  return "Abacus (5.07 + optional standard output + recognising built-in facts added to the kernel interface)";
};

void VampireKernel::beginInputSignature() 
{
  _implementation->beginInputSignature();
};  
void VampireKernel::endInputSignature() 
{
  _implementation->endInputSignature();
};


void VampireKernel::registerInputFunction(const char* name,
					  ulong arity,
					  ulong& id)
{
  _implementation->registerInputFunction(name,arity,id);
}; 
  
void VampireKernel::registerInputNumericConstant(const double& value,
						 ulong& id)
{
  _implementation->registerInputNumericConstant(value,id);
}; 
  

void VampireKernel::registerInputPredicate(const char* name,
					   ulong arity,
					   bool isAnswer,
					   ulong& id)
{
  _implementation->registerInputPredicate(name,arity,isAnswer,id);
};  

void VampireKernel::setSymbolWeightConstPart(ulong symId,const VampireKernel::TermWeightType& weight) 
{
  _implementation->setSymbolWeightConstPart(symId,weight);
};


void VampireKernel::setSymbolPrecedence(ulong symId,long prec) 
{
  _implementation->setSymbolPrecedence(symId,prec);
}; 


void VampireKernel::interpretFunction(const char* inputSymbol,
				      ulong arity,
				      const char* theory,
				      const char* nativeSymbol)
{
  _implementation->interpret(false,inputSymbol,arity,theory,nativeSymbol);
};

void VampireKernel::interpretPredicate(const char* inputSymbol,
				       ulong arity,
				       const char* theory,
				       const char* nativeSymbol)
{
  _implementation->interpret(true,inputSymbol,arity,theory,nativeSymbol);
};

  
void VampireKernel::beginInputClauses() 
{
  _implementation->beginInputClauses();
};

void VampireKernel::endInputClauses() 
{
  _implementation->endInputClauses();
};
  
void VampireKernel::beginInputClause(ulong clauseNumber,
				     void* origin,
				     bool goal,
				     bool support) 
{
  _implementation->beginInputClause(clauseNumber,origin,goal,support); 
};

void VampireKernel::endInputClause() 
{
  _implementation->endInputClause();
};
  
void VampireKernel::beginLiteral(bool positive,ulong predicateId) 
{
  _implementation->beginLiteral(positive,predicateId);
};
void VampireKernel::endLiteral() 
{
  _implementation->endLiteral();
};
  
void VampireKernel::receiveFunction(ulong symId) 
{
  _implementation->receiveFunction(symId);
};

void VampireKernel::receiveNumericConstant(const double& value)
{
  _implementation->receiveNumericConstant(value);
};

void VampireKernel::receiveVariable(ulong varNum) 
{
  _implementation->receiveVariable(varNum);
};
    
void VampireKernel::saturate(ulong nextVacantClauseId,long timeLimit) 
{
  _implementation->saturate(nextVacantClauseId,timeLimit);
};


void VampireKernel::lookForAnotherAnswer()
{
  _implementation->lookForAnotherAnswer();
};


VampireKernel* VampireKernel::currentSession()
{
  if (_Implementation::_currentSession)
    {
      return _Implementation::_currentSession->_wrapper;
    }
  else
    return (VampireKernel*)0;
};


void VampireKernel::suspend()
{
  _implementation->suspend();
};

void VampireKernel::resume()
{
  _implementation->resume();
}; 

void VampireKernel::tabulate()
{
  _implementation->tabulate();
};

void VampireKernel::reportInterrupt(int signalNumber)
{
  if (_implementation)
    {
      _implementation->reportInterrupt(signalNumber);
    };
};
  
void VampireKernel::reportErrorSignal(int signalNumber)
{
  if (_implementation)
    {
      _implementation->reportErrorSignal(signalNumber);
    };
};

void VampireKernel::reportError()
{
  if (_implementation)
    {
      _implementation->reportError();
    };
};

// Communicating search results

VampireKernel::TerminationReason VampireKernel::terminationReason()
{
  return _implementation->terminationReason();
};
const VampireKernel::Clause* VampireKernel::refutation() const 
{
  return _implementation->refutation();
};

bool VampireKernel::isNumericConstantId(ulong symbolId) const
{
  return _implementation->isNumericConstantId(symbolId);
}; 
bool VampireKernel::isInputSymbol(ulong symbolId) const
{
  return _implementation->isInputSymbol(symbolId);
}; 

bool VampireKernel::isAnswerPredicateId(ulong symbolId) const
{
  return _implementation->isAnswerPredicateId(symbolId);
};

ulong VampireKernel::symbolArity(ulong symbolId) const
{
  return _implementation->symbolArity(symbolId);
};
const char* VampireKernel::symbolPrintName(ulong symbolId) const
{
  return _implementation->symbolPrintName(symbolId);
}; 
const double& VampireKernel::numericConstantValue(ulong symbolId) const
{
  return _implementation->numericConstantValue(symbolId);
}; 


//======================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_VAMPIRE_KERNEL
#define DEBUG_NAMESPACE "VampireKernel::Options"
#endif    
#include "debugMacros.h"
//======================================================================

void VampireKernel::Options::setDefaultValues()
{
  // session attributes
  job_id = "default_job_id";
  
  // main algorithm
    
  main_alg = 0L;

  // inference rules
  no_inferences_between_rules = false;
  static_splitting = true;
  dynamic_splitting = false;
  splitting_style = 1L;
  splitting_with_naming = true;
  static_neg_eq_splitting = 3L;
  paramodulation = true;
  sim_back_sup = 0;
  selection = 4L;
  literal_comparison_mode = 0;
  inherited_negative_selection = false;
     
  // strategy 
  forward_subsumption = true;
  fs_set_mode = false;
  fs_optimised_multiset_mode = false;
  fs_use_sig_filters = false;
  fs_old_style = false;
  simplify_by_fs = 1L;
  forward_demodulation = 4L;
  fd_on_splitting_branches = true;
  normalize_by_commutativity = true;
  backward_demodulation = 4L;
  bd_on_splitting_branches = true;
  simplify_by_eq_res = true;
  backward_subsumption = true;
  bs_set_mode = false;
  bs_optimised_multiset_mode = false;
  orphan_murder = false;
  lrs = 1L;
  elim_def = 5L;
  simplification_ordering = NonrecursiveKBOrdering;
  symbol_precedence_by_arity = 1L;
  header_precedence_kinky = false;
  term_weighting_scheme = UniformTermWeighting;
  symbol_weight_by_arity = 0L;

  // numeric options 
  memory_limit = LONG_MAX; // kilobytes
  allocation_buffer_size = 10L;
  allocation_buffer_price = 2.0;
  max_skolem = 1024; 
  max_active = LONG_MAX;
  max_passive = LONG_MAX;
  max_weight = LONG_MAX;
  max_inference_depth = LONG_MAX;
  age_weight_ratio = 5L;
  selected_pos_eq_penalty_coefficient = 1.0;  
  nongoal_penalty_coefficient = 1.0;
  first_time_check = 5L;   
  first_mem_check = 200L;   
    
  // interface options    
  show_opt = false;
  show_input = false;
  show_prepro = false;
  show_gen = false;
  show_passive = false;
  show_active = false; 
  proof = true;
  show_profile = false;
  show_weight_limit_changes = false;  
  tabulation = (ofstream*)0;
  std_output = &cout;
}; // void VampireKernel::Options::setDefaultValues()


ostream& VampireKernel::Options::output(ostream& str)
{
  str << "% job_id = " << job_id << '\n';

  str << "% main_alg = " << main_alg << '\n';
  
  str << "% no_inferences_between_rules = " << no_inferences_between_rules << '\n';
  str << "% static_splitting = " << static_splitting << '\n';
  str << "% dynamic_splitting = " << dynamic_splitting << '\n';
  str << "% splitting_style = " << splitting_style << '\n'; 
  str << "% splitting_with_naming = " << splitting_with_naming << '\n'; 
  str << "% static_neg_eq_splitting = " << static_neg_eq_splitting << '\n';
  str << "% paramodulation = " << paramodulation << '\n'; 
  str << "% sim_back_sup = " << sim_back_sup << '\n';   
  str << "% selection = " << selection << '\n';
  str << "% literal_comparison_mode = " << literal_comparison_mode << '\n';
  str << "% inherited_negative_selection = " << inherited_negative_selection << '\n'; 
     
  str << "% forward_subsumption = " << forward_subsumption << '\n'; 
  str << "% fs_set_mode = " << fs_set_mode << '\n'; 
  str << "% fs_optimised_multiset_mode = " << fs_optimised_multiset_mode << '\n'; 
  str << "% fs_use_sig_filters = " << fs_use_sig_filters << '\n';
  str << "% fs_old_style = " << fs_old_style << '\n';  
  str << "% simplify_by_fs = " << simplify_by_fs << '\n';
  str << "% forward_demodulation = " << forward_demodulation << '\n';
  str << "% fd_on_splitting_branches = " << fd_on_splitting_branches << '\n';
  str << "% normalize_by_commutativity = " << normalize_by_commutativity << '\n';
  str << "% backward_demodulation = " << backward_demodulation << '\n';
  str << "% bd_on_splitting_branches = " << bd_on_splitting_branches << '\n';
  str << "% simplify_by_eq_res = " << simplify_by_eq_res << '\n'; 
  str << "% backward_subsumption = " << backward_subsumption << '\n'; 
  str << "% bs_set_mode = " << bs_set_mode << "\n";
  str << "% bs_optimised_multiset_mode = " << bs_optimised_multiset_mode << "\n";
  str << "% orphan_murder = " << orphan_murder << '\n'; 
  str << "% lrs = " << lrs << '\n';
  str << "% elim_def = " << elim_def << '\n';
  str << "% simplification_ordering = " << simplification_ordering << '\n';
  str << "% symbol_precedence_by_arity = " << symbol_precedence_by_arity << '\n';
  str << "% header_precedence_kinky = " << header_precedence_kinky << '\n';
  str << "% term_weighting_scheme = " << term_weighting_scheme << '\n';
  str << "% symbol_weight_by_arity = " << symbol_weight_by_arity <<  '\n';

  str << "% memory_limit = " << memory_limit << '\n';
  str << "% allocation_buffer_size = " << allocation_buffer_size << '\n';
  str << "% allocation_buffer_price = " << allocation_buffer_price << '\n';
  str << "% max_skolem = " << max_skolem << '\n';
  str << "% max_active = " << max_active << '\n'; 
  str << "% max_passive = " << max_passive << '\n';  
  str << "% max_weight = " << max_weight << '\n';    
  str << "% max_inference_depth = " << max_inference_depth << '\n';  
  str << "% age_weight_ratio = " << age_weight_ratio << '\n';
  str << "% selected_pos_eq_penalty_coefficient = " 
      << selected_pos_eq_penalty_coefficient << '\n';  
  str << "% nongoal_penalty_coefficient = "
      << nongoal_penalty_coefficient << '\n';
  str << "% first_time_check = " << first_time_check << '\n';   
  str << "% first_mem_check = " << first_mem_check << '\n';  
    
  str << "% tabulation = " << (bool)tabulation << '\n';
  str.flush();
  return str;
}; // ostream& VampireKernel::Options::output(ostream& str)

//======================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_VAMPIRE_KERNEL
#define DEBUG_NAMESPACE "VampireKernel::Clause"
#endif    
#include "debugMacros.h"
//======================================================================

ulong VampireKernel::Clause::number() const
{
  CALL("number() const");
  return ((const VK::Clause*)this)->Number();
};

const VampireKernel::Literal* VampireKernel::Clause::literals() const
{
  CALL("literals() const");
  ASSERT(!isBuiltInTheoryFact());
  return 
    (const VampireKernel::Literal*)(((const VK::Clause*)this)->LitList()->firstElem());
};

bool VampireKernel::Clause::isInputClause() const
{
  CALL("isInputClause() const");  
  ASSERT(!isBuiltInTheoryFact());
  return ((const VK::Clause*)this)->isInputClause();
};

bool VampireKernel::Clause::isBuiltInTheoryFact() const
{
  CALL("isBuiltInTheoryFact() const"); 
  return ((const VK::Clause*)this)->isBuiltInTheoryFact();
};


void* VampireKernel::Clause::inputClauseOrigin() const
{
  CALL("inputClauseOrigin() const");
  ASSERT(!isBuiltInTheoryFact());
  ASSERT(isInputClause());
  return ((const VK::Clause*)this)->inputClauseOrigin();
};

const VampireKernel::Clause::Ancestor* VampireKernel::Clause::firstAncestor() const
{
  CALL("firstAncestor() const");  
  ASSERT(!isBuiltInTheoryFact());
  if (isInputClause()) return (const VampireKernel::Clause::Ancestor*)0;
  return 
    (const VampireKernel::Clause::Ancestor*)
    (((const VK::Clause*)this)->background().ancestors());
}; 


//======================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_VAMPIRE_KERNEL
#define DEBUG_NAMESPACE "VampireKernel::Clause::Ancestor"
#endif    
#include "debugMacros.h"
//======================================================================

const VampireKernel::Clause* VampireKernel::Clause::Ancestor::clause() const
{
  CALL("clause() const");
  return 
    (const VampireKernel::Clause*)
    (((const VK::ClauseBackground::ClauseList*)this)->hd());
}; 

const VampireKernel::Clause::Ancestor* VampireKernel::Clause::Ancestor::next() const
{
  CALL("next() const");  
  return 
    (const VampireKernel::Clause::Ancestor*)
    (((const VK::ClauseBackground::ClauseList*)this)->tl());
};

//======================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_VAMPIRE_KERNEL
#define DEBUG_NAMESPACE "VampireKernel::Literal"
#endif    
#include "debugMacros.h"
//======================================================================


const VampireKernel::Literal* 
VampireKernel::Literal::next() const
{
  CALL("next() const");
  return
    (const VampireKernel::Literal*)((const VK::LiteralList::Element*)this)->next();
}; 

bool VampireKernel::Literal::isPositive() const 
{
  CALL("isPositive() const");
  return 
    ((const VK::LiteralList::Element*)this)->positive();
};


ulong VampireKernel::Literal::predicate() const
{
  CALL("predicate() const");
  ulong predId = ((const VK::LiteralList::Element*)this)->literal()->functor();
  if (predId == VampireKernelConst::OrdEqNum) predId = VampireKernelConst::UnordEqNum;
  return predId;
};

const VampireKernel::Subterm* VampireKernel::Literal::firstArgument() const
{
  CALL("firstArgument() const");
  const TERM* args = (((const VK::LiteralList::Element*)this)->literal()->Args());
  if (args->IsNull()) return (const VampireKernel::Subterm*)0;
  return (const VampireKernel::Subterm*)args;
};


//======================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_VAMPIRE_KERNEL
#define DEBUG_NAMESPACE "VampireKernel::Subterm"
#endif    
#include "debugMacros.h"
//======================================================================

const VampireKernel::Subterm* VampireKernel::Subterm::next() const
{
  CALL("next() const");
  const TERM* nxt = ((const TERM*)this)->Next();
  if (nxt->IsNull()) return (const VampireKernel::Subterm*)0;
  return (const VampireKernel::Subterm*)nxt;
};

bool VampireKernel::Subterm::isVariable() const
{
  CALL("isVariable() const");
  return ((const TERM*)this)->isVariable();
};

ulong VampireKernel::Subterm::variableNumber() const
{
  CALL("variableNumber() const");
  ASSERT(isVariable());
  return ((const TERM*)this)->var();
};

ulong VampireKernel::Subterm::topFunction() const
{
  CALL("topFunction() const");
  ASSERT(!isVariable());
  return ((const TERM*)this)->First()->functor();
};

const VampireKernel::Subterm* VampireKernel::Subterm::firstArgument() const
{
  CALL("firstArgument() const");
  ASSERT(!isVariable());
  const TERM* args = ((const TERM*)this)->First()->Args();  
  if (args->IsNull()) return (const VampireKernel::Subterm*)0;
  return (const VampireKernel::Subterm*)args;
};

//======================================================================







