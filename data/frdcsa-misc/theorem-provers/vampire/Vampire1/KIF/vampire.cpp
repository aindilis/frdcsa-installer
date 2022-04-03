//
//  file vampire.cpp
//  main file for the big vampire
//

#include <string.h>
#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <strstream.h> 

// parser-related files
#include "pcctscfg.h"
#include "pccts_stdio.h"
#include "tokens.h"
#include "AParser.h"
#include "TPTPParser.h"
#include "DLGLexer.h"

// kernel files
#include "Tabulation.h"
#include "VampireKernel.h"
#include "Exit.h"
#include "GlobAlloc.h"
#include "Input.h"
#include "GlobalClock.h"

// vampire files
#include "options.h"
#include "tracer.h"
#include "assert.h"
#include "renaming.h"
#include "property.h"
#include "schedule.h"

VK::Tabulation* tabulation = 0;
void runtimeErrorHook();
void debugSystemInternalErrorHook();
void setSignalHandlers();
void cancelSignalHandling();


const char* basename(const char* fileName)
{
  int c = strlen(fileName) - 1; 
  while ((c >= 0) && (fileName[c] != '/')) c--;
  c++;
  char* res = new char[strlen(fileName) - c + 1];
  strcpy(res,fileName + c); 
  return res;
};


const char* generateJobId()
{
  time_t sessionTime = time((time_t*)0); 
  tm* gmt = gmtime(&sessionTime); 
  int sessionDateDD = gmt->tm_mday;
  int sessionDateMM = gmt->tm_mon;    
  int sessionDateYY = 1900 + gmt->tm_year;
  int sessionTimeHH = gmt->tm_hour;
  int sessionTimeMM = gmt->tm_min;
  int sessionTimeSS = gmt->tm_sec; 
  pid_t sessionProcessId = getpid();
  ostrstream str;
  const char* hhh = (sessionTimeHH < 10) ? "0" : "";
  const char* mmm = (sessionTimeMM < 10) ? "0" : "";
  const char* sss = (sessionTimeSS < 10) ? "0" : "";
  str << sessionDateDD << sessionDateMM  << sessionDateYY;
  str << hhh << sessionTimeHH << mmm << sessionTimeMM << sss << sessionTimeSS;
  str << sessionProcessId << '\0';
  char* res = new char[strlen(str.str()) + 1];
  strcpy(res,str.str());
  return res; 
  
}; // const char* generateJobId()


ofstream* createTab (Options& opts, const char* jobId)
{
  // read tab file from options
  const char* tabulationFileName = opts.tab ();
  const char* testId = opts.testId ();

  if (! tabulationFileName ) {
    return 0;
  }

  ofstream* tabStream = new ofstream;
  tabStream->open (tabulationFileName, ios::app);
  if ( ! (*tabStream) ) {
    throw MyException ( "Can not open file for tabulation");
  };

  tabulation = new VK::Tabulation();
  tabulation->assignStream (tabStream);
  tabulation->assignJob (jobId);

  const char* problemFileBaseName = basename (opts.file());
  tabulation->header (testId, problemFileBaseName);   
  tabulation->problemName (problemFileBaseName);
#ifndef NO_DEBUG
  BK::Debug::assignJob(problemFileBaseName);
#endif 

  tabulation->testId (testId);

  char hostname[256];
  gethostname(hostname,256);
  tabulation->hostName ((const char*)hostname);

  time_t jobTime = time((time_t*)0);
  tm* gmt = gmtime(&jobTime); 
  int jobDateDD = gmt->tm_mday;
  int jobDateMM = gmt->tm_mon;    
  int jobDateYY = 1900 + gmt->tm_year;
  int jobTimeHH = gmt->tm_hour;
  int jobTimeMM = gmt->tm_min;
  int jobTimeSS = gmt->tm_sec;

  tabulation->jobDate(jobDateDD,jobDateMM,jobDateYY);
  tabulation->jobTime(jobTimeHH,jobTimeMM,jobTimeSS);
  tabulation->version(VampireKernel::versionNumber(),
		      VampireKernel::versionDescription());
  tabulation->versionDate();
  tabulation->versionTime();

  return tabStream;
} // createTab


void passOptions (const Options& vampire, 
		  VampireKernel::Options& kernel, 
		  const char* jobId)
{
  kernel.job_id = jobId;
  kernel.main_alg = (long)vampire.mainAlg ();
  kernel.static_splitting = vampire.staticSplitting ();
  kernel.dynamic_splitting = vampire.dynamicSplitting ();
  kernel.splitting_style = vampire.splittingStyle ();
  kernel.splitting_with_naming = vampire.splittingWithNaming ();
  kernel.paramodulation = vampire.paramodulation ();
  kernel.selection = vampire.selection ();
  kernel.literal_comparison_mode = vampire.literalComparisonMode ();
  kernel.inherited_negative_selection = vampire.inheritedNegativeSelection ();
  kernel.fs_set_mode = vampire.fsSetMode ();
  kernel.fs_optimised_multiset_mode = vampire.fsOptimisedMultisetMode ();
  kernel.simplify_by_fs = vampire.simplifyByFs ();
  kernel.forward_demodulation = vampire.forwardDemodulation ();
  kernel.fd_on_splitting_branches = vampire.fdOnSplittingBranches ();
  kernel.backward_demodulation = vampire.backwardDemodulation ();
  kernel.bd_on_splitting_branches = vampire.bdOnSplittingBranches ();
  kernel.backward_subsumption = vampire.backwardSubsumption ();
  kernel.bs_set_mode = vampire.bsSetMode ();
  kernel.bs_optimised_multiset_mode = vampire.bsOptimisedMultisetMode ();
  kernel.orphan_murder = vampire.orphanMurder ();
  kernel.lrs = vampire.lrs ();
  kernel.elim_def = vampire.elimDef ();
  kernel.symbol_precedence_by_arity = vampire.symbolPrecedenceByArity ();
  kernel.header_precedence_kinky = vampire.headerPrecedenceKinky ();
  //  kernel. = vampire.long timeLimit ();
  kernel.memory_limit = vampire.memoryLimit ();
  kernel.max_skolem = vampire.maxSkolem ();
  kernel.max_passive = vampire.maxPassive ();
  kernel.max_weight = vampire.maxWeight ();
  kernel.age_weight_ratio = vampire.ageWeightRatio ();
  kernel.proof = vampire.proof ();
  kernel.symbol_weight_by_arity = vampire.symbolWeightByArity ();
  kernel.nongoal_penalty_coefficient = vampire.nongoalPenaltyCoefficient ();
  switch (vampire.simplificationOrdering ()) {
  case Options::KBO:
    kernel.simplification_ordering = VampireKernel::Options::StandardKBOrdering;
    break;
  case Options::KBO_NONREC:
    kernel.simplification_ordering = VampireKernel::Options::NonrecursiveKBOrdering;
    break;
  }
  switch (vampire.termWeightingScheme ()) {
  case Options::TWS_UNIFORM:
    kernel.term_weighting_scheme = VampireKernel::Options::UniformTermWeighting;
    break;
  case Options::TWS_NONUNIFORM_CONSTANT:
    kernel.term_weighting_scheme = VampireKernel::Options::NonuniformConstantTermWeighting;
    break;
  }
} // passOptions (Options&, VampireKernel::Options&)


void passTerm (const Term* t, 
	       VampireKernel& kernel,
	       Renaming& renaming,
	       int& lastVar)
{
  CALL("passTerm(Term* term, VampireKernel& kernel)");

  if (t->isVar()) {
    Var v = t->var();
    Var newV = renaming.get(v);
    if (newV == -1) { // v is not not bound
      renaming.push (v, ++lastVar);
      newV = lastVar;
    }
    kernel.receiveVariable(newV);
    return;
  }
  // t is not a variable
  // pass the functor
  kernel.receiveFunction (t->functor()->kernelNumber());
  Term::List::Iterator ts (t->args());
  while (ts.more()) {
    passTerm (ts.next(), kernel, renaming, lastVar);
  }
}; // passTerm(Term* term, VampireKernel& kernel)


void passSignature (VampireKernel& kernel)
{
  kernel.beginInputSignature();
  
  // pass predicate symbols
  Signature::PredIterator preds (sig);
  while (preds.more()) {
    ulong symbolId; // to return symbolId
    Signature::Symbol* pred = preds.next ();
    kernel.registerInputPredicate(pred->name(),
				  pred->arity(),
				  false, // answer literal sign
				  symbolId);
    pred->kernelNumber (symbolId);
  }
  // pass function symbols
  Signature::FunIterator funs (sig);
  while (funs.more()) {
    ulong symbolId; // to return symbolId
    Signature::Symbol* fun = funs.next ();
    kernel.registerInputFunction(fun->name(),
				 fun->arity(),
				 symbolId);
    fun->kernelNumber (symbolId);
  }

  kernel.endInputSignature();  
} // passSignature


int passClauses (Problem& problem, VampireKernel& kernel)
{
  kernel.beginInputClauses();
  int lastClauseNumber = 0;

  Unit::Chain::Iterator units (problem.units());
  while (units.more()) {
    // pick the next clause from the problem
    Unit* u = units.next ();
    // ASSERT(u->unitType() == CLAUSE);
    Clause* c = u->clause ();
    int lastVar = -1;
    Renaming renaming;

    // read its number
    int number = u->number();
    if (number > lastClauseNumber) {
      lastClauseNumber = number;
    }

    // communicate the clause
    kernel.beginInputClause (number, u, u->inputType() != AXIOM);

    // communicate its literals
    Literal::List::Iterator lits (c->literals());
    while (lits.more()) {
      Literal* lit = lits.next ();
      kernel.beginLiteral(lit->positive(),
			  lit->atom()->functor()->kernelNumber());

      Term::List::Iterator args (lit->atom()->args());
      while ( args.more() ) {
	passTerm (args.next(), kernel, renaming, lastVar);
      }

      kernel.endLiteral();		  
    }

    kernel.endInputClause();
  }

  kernel.endInputClauses();

  return lastClauseNumber;
} // passClauses


char run (VampireKernel& kernel, 
	  long timeLimit,
	  VampireKernel::Options& options, 
	  Problem& problem
	  )
{
  try 
    {
      // reset global memory allocator, must be done before every
      // restart of the kernel
      BK::GlobAlloc::reset ();
      // tell kernel to start the new session
      kernel.openSession(options);
      // pass the signature from preprocessor to kernel
      passSignature (kernel);
      // pass the clauses from preprocessor to kernel
      int lastClauseNumber = passClauses (problem, kernel);
  
      kernel.saturate(lastClauseNumber,timeLimit);
    }
  catch (const VampireKernel::Emergency& e)
    {
      BK::DestructionMode::makeThorough();

      switch (e.type())
	{
	case VampireKernel::Emergency::AllocationFailure:
	  // nonfatal
	  return '0';
	case VampireKernel::Emergency::TimeLimit:
	  // nonfatal
	  return '0';
	};
    };
  // the kernel session terminated in a normal way

  char result = '0';
  switch (kernel.terminationReason())
    {
    case VampireKernel::UnknownTerminationReason:
      ICP("ICP0"); // debugging operator, teh control must come here
      VK::RuntimeError::report("Kernel session terminated for unknown reason");
    case VampireKernel::RefutationFound:
      result = '+';
      break;
    case VampireKernel::Satisfiable:
      result = '-';
      break;
    case VampireKernel::EmptyPassive:
      break;
    case VampireKernel::MaxPassive:
      break;
    case VampireKernel::MaxActive:
      break;
    };
  
  return result;
} // run (VampireKernel& kernel, ...)


void runtimeErrorHook()
{
  if (VampireKernel::currentSession())
    {
      cout << "% Trying to shut down current kernel session...\n";
      VampireKernel::currentSession()->reportError();
    };
  if (tabulation)
    {
      tabulation->errorMessage("runtime","some module detected inconsistency in run time");
    };
}; // void runtimeErrorHook()


void debugSystemInternalErrorHook()
{  
  if (VampireKernel::currentSession())
    {
      cout << "% Trying to shut down current kernel session...\n";
      VampireKernel::currentSession()->reportError();
    };
  if (tabulation)
    {
      tabulation->errorMessage("debug","debugging system detected internal inconsistency");
    };
}; // void debugSystemInternalErrorHook()


const char* spellSignal(int sigNum)
{
  switch (sigNum)
    {
    case SIGTERM: return "SIGTERM";
    case SIGQUIT: return "SIGQUIT";
    case SIGHUP: return "SIGHUP";
    case SIGINT: return "SIGINT";
    case SIGXCPU: return "SIGXCPU";
    case SIGILL: return "SIGILL";
    case SIGFPE: return "SIGFPE";
    case SIGSEGV: return "SIGSEGV";
    case SIGBUS: return "SIGBUS";
    case SIGTRAP: return "SIGTRAP";
    case SIGABRT: return "SIGABRT";
    default: return "UNKNOWN SIGNAL";
    };
};

void handleSignal(int sigNum)
{
  switch (sigNum)
    {
    case SIGTERM:
    case SIGQUIT:
    case SIGHUP:
    case SIGXCPU:
      cout << "% Aborted by signal " << spellSignal(sigNum) << "\n";
      if (VampireKernel::currentSession())
	{
	  cout << "% Trying to shut down current kernel session...\n";
	  VampireKernel::currentSession()->reportInterrupt(sigNum);
	  VampireKernel::currentSession()->tabulate();
	};
      if (tabulation) tabulation->result('0');
      cout << "% Bailing out now: aborted by interrupt signal " 
	   << spellSignal(sigNum) << "\n";
      cancelSignalHandling();
      BK::Exit::exit(1);
      
    case SIGINT:
      {
	if (VampireKernel::currentSession())
	  {
	    VampireKernel::currentSession()->suspend();
	  }
	else
	  {
	    cout << "% No kernel session running at the moment.\n";
	  };
	cout << "Continue? (y/n)\n";
	
	char a;
	cin >> a;
	
	if ((a == 'n') || (a == 'N'))
	  {
	    cout << "% Terminated by the user.\n";
	    if (VampireKernel::currentSession()) {
	      VampireKernel::currentSession()->reportInterrupt(sigNum);
              VampireKernel::currentSession()->tabulate();
	    }
	    if (tabulation) tabulation->result('0');
	    BK::Exit::exit(0); 
	  }
	else
	  {
            if (VampireKernel::currentSession())
	      VampireKernel::currentSession()->resume();
	    cout << "\nContinuing...\n";
	  }
      }
      return;
      

    case SIGILL:
    case SIGFPE:
    case SIGSEGV:
    case SIGBUS:
    case SIGTRAP:
    case SIGABRT:
      cout << "% Error signal " << spellSignal(sigNum) << " catched\n";
      if (VampireKernel::currentSession())
	{
	  cout << "% Trying to shut down current kernel session...\n";
	  VampireKernel::currentSession()->reportErrorSignal(sigNum);
          VampireKernel::currentSession()->tabulate();
	};
      if (tabulation)
	{
	  const char* errorQualifier = "Error signal";
	  char errorMessage[256];
	  sprintf(errorMessage,"error signal %d received",sigNum);
	  tabulation->errorMessage(errorQualifier,errorMessage);
	};
      cout << "% Bailing out now: aborted by error signal " << spellSignal(sigNum) << "\n";
      cancelSignalHandling();
      BK::Exit::exit(1); 
    default:
      break;
    };
};

void cancelSignalHandling()
{
  signal(SIGTERM,SIG_DFL);
  signal(SIGQUIT,SIG_DFL);
  signal(SIGHUP,SIG_DFL);
  signal(SIGINT,SIG_DFL);
  signal(SIGXCPU,SIG_DFL);
#ifndef DEBUG_ERROR_SIGNALS
  signal(SIGILL,SIG_DFL);
  signal(SIGFPE,SIG_DFL);
  signal(SIGSEGV,SIG_DFL);
  signal(SIGBUS,SIG_DFL);
  signal(SIGTRAP,SIG_DFL);
  signal(SIGABRT,SIG_DFL);
#endif
}; // void cancelSignalHandling()

void setSignalHandlers()
{
  signal(SIGTERM,handleSignal);
  signal(SIGQUIT,handleSignal);
  signal(SIGHUP,handleSignal);
  signal(SIGINT,handleSignal);
  signal(SIGXCPU,handleSignal);
#ifndef DEBUG_ERROR_SIGNALS
  signal(SIGILL,handleSignal);
  signal(SIGFPE,handleSignal);
  signal(SIGSEGV,handleSignal);
  signal(SIGBUS,handleSignal);
  signal(SIGTRAP,handleSignal);
  signal(SIGABRT,handleSignal);
#endif
}; // void setSignalHandlers()


Unit::List* readInput (const char* inputFileName)
{
  FILE* inputFile = fopen ( inputFileName, "r" );

  if ( ! inputFile ) {
    return 0;
  }

  // define input file
  DLGFileInput in ( inputFile );

  // define an instance of scanner
  DLGLexer scanner ( &in );

  // define token buffer between scanner and parser
  ANTLRTokenBuffer pipe ( &scanner );

  // create a token to use as a model
  ANTLRCommonToken tok;

  // tell the scanner what type the token is
  scanner.setToken ( &tok );

  // create an instance of your parser
  TPTPParser parser ( &pipe );

  // initialise the parser
  parser.init ();

  // parsing
  Unit::List* result = parser.input ();
  fclose ( inputFile );

  return result;
} // readInput


void vampire ( Options& opts, 
	       Problem& problem, 
	       BK::GlobalClock::Timer& timer )
{
  TRACER ("vampire");

  // preprocess the problem
  problem.preprocess (opts);

  // cout << problem;

  // create job id
  const char* jobId = generateJobId();

  // create options for the kernel
  VampireKernel::Options kernelOptions;
  // pass the values of options from vampire to the kernel
  passOptions (opts, kernelOptions, jobId);

  // open tab file
  ofstream* tabStream = createTab (opts, jobId);
  kernelOptions.tabulation = tabStream;

  // Vampire kernel is created
  VampireKernel kernel;

  char result = run (kernel, opts.timeLimit(), kernelOptions, problem);
  // tabulate the result
  kernel.tabulate ();
  if (tabulation) tabulation->result(result);
  kernel.closeSession(); // not really necessary here
}  // vampire


void casc ( Options& opts, 
	    Problem& problem, 
	    BK::GlobalClock::Timer& timer )
{
  TRACER ("casc");

  // preprocess the problem
  problem.preprocess (opts);

  Property property (problem);
  Schedule schedule (property, opts);
  char result = schedule.run (problem, timer);
  if (tabulation) {
    tabulation->result(result);
  }
} // casc


#ifdef DEBUG_MEMORY_LEAKS
//  kernel.destroy(); // will automatically close the session
  
//  DF; BK::GlobAlloc::outputPersistent(cout,10000UL);

//  ASSERT(BK::GlobAlloc::occupiedByObjects() == 0L);  
//  BK::GlobAlloc::reset();
#else
#ifdef NO_DEBUG
//  kernel.closeSession ();
//  BK::DestructionMode::makeFast();
#endif
//  kernel.closeSession(); // not really necessary here
#endif
//}  // casc


int main ( int argc, char* argv [ ] )
{
  TRACER ("main");

  BK::GlobalClock::Timer timer;
  timer.reset();
  timer.start();

  setSignalHandlers();
  BK::RuntimeError::errorHook = runtimeErrorHook;
#ifndef NO_DEBUG
  BK::Debug::setErrorHook(debugSystemInternalErrorHook);
#endif

  // create new global signature (kludge to check memory leaks)
  Signature signature;
  sig = &signature;

  try {
    // read options from the command line
    Options optionss (argc, argv);

    // open input file
    const char* inputFileName = options.file ();
    Unit::List* units = readInput (inputFileName);
    if (! units) {
      cerr << "Cannot open input file\n";
      return EXIT_FAILURE;
    }

    // create problem from the input
    Problem problem (units);

    if ( options.casc() ) {
      casc (options, problem, timer);
    }
    else { // ordinary Vampire
      vampire (options, problem, timer);
    }

    delete IntNameTable::vars;
    BK::DestructionMode::makeFast();

    return EXIT_SUCCESS;
  }
  catch (MyException me) {
    me.cry (cerr);
    delete IntNameTable::vars;

    BK::DestructionMode::makeFast();

    return EXIT_SUCCESS;
  }
  catch (MemoryException me) {
    me.cry (cerr);
    delete IntNameTable::vars;

    BK::DestructionMode::makeFast();

    exit (EXIT_SUCCESS);
  }
} // main
