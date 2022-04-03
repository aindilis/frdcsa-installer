//
//  file wizard.cpp (class Wizard)
//    NOTE: Wizard is an abstract Wizard, has nothing to do with Gandalf
//  07/06/2001 Manchester
//


#include "wizard.h"
#include "property.h"
#include "problem.h"
#include "prepro.h"
#include "strategy.h"

#include <fstream.h>


void Wizard::execute (Problem& problem, int timeLimit, int memLimit, char* tabFile, char* outFile, ostream& tab)
{ 
  Prepro prepro (problem);

  // preprocess
  prepro.simplifyBasic (tab);
  // these were introduced just recently for experiments
  prepro.simplifyA (tab);
  prepro.simplifyB (tab);
  prepro.simplifyC (tab);
  prepro.simplifyD (tab);
  prepro.simplifyE (tab);
  prepro.simplifyF (tab);
  
  Property property (problem);
  Strategy strategy (property);

  // open file for writing a preprocessed TPTP problem
  ofstream out (outFile);
  if ( ! out ) {
    cerr << "Cannot open output file " << outFile << "\n";
    return;
  }

  strategy.execute (prepro, cout, out, timeLimit, memLimit, tabFile, tab);
  out.close();
} // Wizard::execute


