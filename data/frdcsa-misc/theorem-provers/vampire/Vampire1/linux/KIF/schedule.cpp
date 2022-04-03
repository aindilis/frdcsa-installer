//
//  file schedule.cpp (schedules for running problems)
//


#include "VampireKernel.h"
#include "GlobAlloc.h"

#include "property.h"
#include "problem.h"
#include "options.h"
#include "schedule.h"
#include "vampire.h"
#include "int.h"
#include "casc.h"


Schedule::Schedule (Property& p, Options& cmdLineOptions)
  :
  _commandLineOptions (cmdLineOptions)
{
  char features [20];
  p.toString (features);
  int timeLimit = cmdLineOptions.timeLimit ();

  if ( ! strcmp(features,"Hugssfnssm13") ) { // LAT041-1
    Options* general = Options::lrs (Options::P_4);
    general->simplificationOrdering (Options::KBO);
    general->timeLimit (0);

    _schedule = new List (general);
    return;    
  }
  if ( ! strcmp(features,"Hhgsmfgmss22") ) { // LAT006-1
    Options* general = Options::lrs (Options::N_6);
    general->timeLimit (0);

    _schedule = new List (general);
    return;    
  }
  if ( ! strcmp(features,"Pggpffnmmm02") ) { // COL076-1, does not work
    Options* general = Options::lrs (Options::N_2, Options::KINKY);
    general->timeLimit (0);

    _schedule = new List (general);
    return;    
  }
  if ( ! strcmp(features,"nggnmfnfff22") ) { // ANA004-5
    Options* o1 = Options::lrs (Options::P_1003, Options::KINKY);
    o1->timeLimit (150);
    o1->backwardSubsumption (false);

    Options* general = Options::lrs (Options::P_7);
    general->backwardSubsumption (false);
    general->timeLimit (0);

    _schedule = new List (o1, new List (general));
    return;    
  }
  if ( ! strcmp(features,"nggnmfnfff22") ) { // FLD020-1
    Options* o1 = Options::lrs (Options::P_1005, Options::KINKY);
    o1->timeLimit (60);

    Options* o2 = Options::lrs (Options::P_1003);
    o2->timeLimit (60);

    Options* general = Options::lrs (Options::P_4);
    general->backwardSubsumption (false);
    general->timeLimit (0);

    _schedule = new List (o1, new List (o2, new List (general)));
    return;    
  }
  if ( ! strcmp(features,"Pggpffgmmm02") ) { // COL080-1, does not work
    Options* o1 = Options::discount (Options::P_7);
    o1->timeLimit (60);

    Options* o2 = Options::lrs (Options::N_6, Options::KINKY);
    o2->timeLimit (60);

    Options* general = Options::lrs (Options::P_4);
    general->backwardSubsumption (false);
    general->timeLimit (0);

    _schedule = new List (o1, new List (o2, new List (general)));
    return;    
  }

  switch ( p.categoryToChar() ) {
  case 'f': { // FNE
    Options* general = Options::lrs (Options::P_4);
    general->timeLimit (0);

    _schedule = new List (general);
    break;
  }

  case 'n': { // NNE

    Options* o1 = Options::lrs (Options::P_7);
    o1->ageWeightRatio (1);
    o1->timeLimit(620);

    Options* o2 = Options::lrs (Options::P_1003, Options::KINKY);
    o2->ageWeightRatio (16);
    o2->backwardSubsumption (false);
    o2->timeLimit(620);
    
    Options* o3 = Options::lrs (Options::P_1005, Options::KINKY);
    o3->ageWeightRatio (13);
    o3->backwardSubsumption (false);
    o3->timeLimit(420);

    Options* o4 = Options::lrs (Options::P_6, Options::KINKY);
    o4->fullSplitting ();
    o4->backwardSubsumption (false);
    o4->timeLimit(70);

    Options* o5 = Options::lrs (Options::P_1);
    o5->backwardSubsumption (false);
    o5->timeLimit(170);

    Options* o6 = Options::lrs (Options::N_1005);
    o6->literalComparisonMode (Options::KINKY);
    o6->symbolPrecedenceByArity (Options::REVERSE_ARITY);
    o6->ageWeightRatio (11);
    o6->backwardSubsumption (false);
    o6->timeLimit(270);

    Options* o7 = Options::discount (Options::P_7);
    o7->symbolPrecedenceByArity (Options::REVERSE_ARITY);
    o7->ageWeightRatio (172);
    o7->timeLimit(320);

    Options* general = Options::lrs (Options::P_1005);
    general->timeLimit (0);

    _schedule = new List (o1, new List (o2, new List (o3, 
                new List (o4, new List (o5, new List (o6, 
                new List (o7, new List (general))))))));
    break;
  }

  case 'h': { // HNE

    Options* o1 = Options::lrs (Options::P_4);
    o1->ageWeightRatio (3);
    o1->timeLimit(520);
    o1->backwardSubsumption (false);
    o1->elimDef (1);

    Options* o2 = Options::discount (Options::N_1005);
    o2->ageWeightRatio (2);
    o2->timeLimit(70);
    o2->backwardSubsumption (false);

    Options* o3 = Options::discount (Options::N_2);
    o3->timeLimit(70);
    o3->backwardSubsumption (false);

    Options* general = Options::lrs (Options::P_1, Options::KINKY);
    general->ageWeightRatio (54);
    general->backwardSubsumption (false);
    general->timeLimit(0);

    _schedule = new List (o1, new List (o2, new List (o3, 
                new List (general))));
    break;
  }

  case 'H': { // HEQ

    Options* o1 = Options::lrs (Options::P_4);
    o1->ageWeightRatio (1);
    o1->elimDef (1);
    o1->timeLimit(520);

    Options* o2 = Options::discount (Options::P_7);
    o2->ageWeightRatio (9);
    o2->elimDef (1);
    o2->backwardSubsumption (false);    
    o2->timeLimit(570);

    Options* o3 = Options::discount (Options::N_1009);
    o3->ageWeightRatio (8);
    o3->symbolPrecedenceByArity (Options::REVERSE_ARITY);
    o3->timeLimit(70);

    Options* o4 = Options::lrs (Options::N_1005, Options::KINKY);
    o4->ageWeightRatio (2);
    o4->elimDef (1);
    o4->timeLimit(620);

    Options* o5 = Options::lrs (Options::P_1005);
    o5->ageWeightRatio (4);
    o5->timeLimit(520);
    o5->backwardSubsumption (false);    
    o5->forwardDemodulation (Options::PREORDERED);
    o5->maxWeight (16);

    Options* general = Options::lrs (Options::P_1003, Options::KINKY);
    general->timeLimit(0);

    _schedule = new List (o1, new List (o2, new List (o3, 
                new List (o4, new List (o5, new List (general))))));
    break;
  }

  case 'P': { // PEQ
    Options* o1 = Options::lrs (Options::N_2);
    o1->ageWeightRatio (6);
    o1->timeLimit(670);

    Options* o2 = Options::discount (Options::P_1);
    o2->ageWeightRatio (3);
    o2->timeLimit(120);

    Options* o3 = Options::otter (Options::P_1007);
    o3->ageWeightRatio (100);
    o3->backwardSubsumption (false);
    o3->timeLimit(120);

    Options* o4 = Options::discount (Options::N_1005);
    o4->ageWeightRatio (3);
    o4->timeLimit(770);

    Options* general = Options::lrs (Options::P_4);
    general->timeLimit (0);

    _schedule = new List (o1, new List (o2, new List (o3, 
                new List (o4, new List (general)))));
    break;
  }

  case 'F': { // FEQ

    Options* o1 = Options::lrs (Options::P_4);
    o1->ageWeightRatio (1);
    o1->timeLimit(620);
    o1->elimDef (1);

    Options* o2 = Options::lrs (Options::P_6);
    o2->ageWeightRatio (9);
    o2->timeLimit(320);
    o2->backwardSubsumption (false);

    Options* o3 = Options::discount (Options::P_1003);
    o3->ageWeightRatio (9);
    o3->timeLimit(320);

    Options* o4 = Options::lrs (Options::P_1003, Options::KINKY);
    o4->fullSplitting ();
    o4->timeLimit(420);
    o4->ageWeightRatio (7);
    o4->backwardSubsumption (false);
    o4->elimDef (1);

    Options* general = Options::lrs (Options::P_1005);
    general->timeLimit (0);

    _schedule = new List (o1, new List (o2, new List (o3, 
                new List (o4, new List (general)))));
    break;
  }

  case 'E': // EPR
  case 'N': { // NEQ

    Options* o1 = Options::lrs (Options::P_4);
    o1->ageWeightRatio (1);
    o1->timeLimit(470);
    o1->backwardSubsumption (false);

    Options* o2 = Options::lrs (Options::P_1002);
    o2->timeLimit(320);
    o2->backwardSubsumption (false);

    Options* o3 = Options::lrs (Options::N_1005, Options::KINKY);
    o3->timeLimit(520);
    o3->backwardSubsumption (false);
    o3->elimDef (1);

    Options* o4 = Options::discount (Options::N_7, Options::PREDICATE);
    o4->timeLimit(30);

    Options* o5 = Options::lrs (Options::P_7, Options::PREDICATE);
    o5->timeLimit(570);
    o5->ageWeightRatio (4);
    o5->elimDef (1);

    Options* general = Options::lrs (Options::P_1005);
    general->timeLimit (0);

    _schedule = new List (o1, new List (o2, new List (o3, 
                new List (o4, new List (o5, new List (general))))));
    break;
  }

  case 'U': { // UEQ

    Options* general = Options::lrs (Options::P_1);
    general->backwardSubsumption (false);
    general->timeLimit(timeLimit - 600);

    Options* o1 = Options::discount (Options::P_1);
    o1->ageWeightRatio (2);
    o1->backwardSubsumption (false);
    o1->timeLimit(600);

    _schedule = new List ( general, new List (o1) );
    break;
  }

  default:
    ASSERT( debug_schedule, false);
  }
} // Schedule::Schedule


Schedule::~Schedule ()
{
  List::Iterator opts (_schedule);
  while (opts.more()) {
    delete opts.next ();
  }
  _schedule->destroy ();
} // Schedule::~Schedule


char Schedule::run (Problem& problem, BK::GlobalClock::Timer& timer)
{
  // create Vampire kernel
  VampireKernel kernel;

  // create job id
  const char* jobId = generateJobId();

  // create options for the kernel
  VampireKernel::Options kernelOptions;
  // open tab file
  ofstream* tabStream = createTab (_commandLineOptions, jobId);
  kernelOptions.tabulation = tabStream;

  while (_schedule->isNonEmpty()) {
    Options* opts = List::pop (_schedule);
    // recalculate the time limit
    opts->timeLimit (absoluteTime(opts->timeLimit()));

    kernel.closeSession(); // safe even if there is no session
    BK::GlobAlloc::reset();

    // pass the values of options from vampire to the kernel
    passOptions (*opts, kernelOptions, jobId);

    int remainingTime = _commandLineOptions.timeLimit () -
                        timer.elapsedDeciseconds ();
    int timeLimit = _schedule ->isEmpty () ?
                    remainingTime :
                    Int::min (opts->timeLimit(), remainingTime);

    //cout << "OPTS LIMIT " << opts->timeLimit() << "\n"
    //     << "ELAPSED " << timer.elapsedDeciseconds() << "\n"
    //     << "THIS SLICE LIMIT " << timeLimit << "\n"
    //     << "COMMAND LINE LIMIT " << _commandLineOptions.timeLimit () << "\n";
    
    // run the kernel
    switch (::run (kernel, timeLimit, kernelOptions, problem)) {
    case '+':
      cout << "--------------------- PROVED ----------------------\n"
	   << flush;
      kernel.tabulate ();
      kernel.closeSession(); // not really necessary here
      return '+';

    case '-':
      cout << "--------------------- UNPROVABLE ----------------------\n"
	   << flush;
      kernel.tabulate ();
      kernel.closeSession(); // not really necessary here
      return '-';

    case '0':
      delete opts;
      break;
    }
  }

  cout << "--------------------- CANNOT PROVE ----------------------\n"
       << flush;
  kernel.tabulate ();
  kernel.closeSession(); // not really necessary here
  return '0';
} // Schedule::run


int Schedule::absoluteTime (int relativeTime)
{
  if (Casc::slowness <= 1) { // computer is fast enough or slowness is undefined
    return relativeTime;
  }
  float at = ((float)relativeTime) * Casc::slowness;

  return (int) at;
} // absoluteTime

