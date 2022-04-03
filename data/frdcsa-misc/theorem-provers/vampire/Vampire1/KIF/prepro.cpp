//
//  file prepro.cpp
//  implements preprocessor
//
//  09/06/2000 Manchester
//  09/05/2001 Manchester, several functions zipped out
//  10/05/2001 Manchester, simplifyA
//  15/05/2001 Manchester, simplifyB
//  22/05/2001 Manchester, simplifyC
//  23/05/2001 Manchester, simplifyBasic/1
//  27/05/2001 Manchester, eliminatePure added to simplifyBasic
//  29/05/2001 Manchester, simplifyD
//  23/05/2001 Manchester, simplifyBasic/0
//  30/05/2001 Manchester, simplifyE
//  31/05/2001 Manchester, normalize/0
//

//
//  More to do: 
//    (1) negative equality splitting
//    (2) more removal of f(x) = t
//    (3) removal of linear flat atoms

/*
#include <fstream.h>
*/


#include "prepro.h"
#include "problem.h"
#include "options.h"
#include "int.h"


// 07/06/2001 Manchester, new constructor (since now Prepro is only in charge
//            of preprocessing, but not calling Vampire
Prepro::Prepro ( Problem& problem ) 
  :
  _problem (problem)
{
//  problem.normalize ();
} // Prepro::Prepro


/*
// used to classify the problem using the TPTP classification,
// for example PEQ or EPR
// no simplifications are used
char* Prepro::classify () 
{
  // there must be a tab file for outputting statistics about the preprocessed problem
  const char* tabFileName = "prepro.tab";
  ofstream tab ( tabFileName, ios::app );

  if ( ! tab ) {
    cerr << "Cannot open output file for tabulation\n";
    exit ( EXIT_FAILURE );
  }

  if ( _problem.hasEquality() ) {
    if ( _problem.isUnit () ) 
      return "UEQ";
    if ( _problem.isHorn () ) 
      return "HEQ";
    if ( ! _problem.hasNonEquality () ) 
      return "PEQ";
    return "NEQ";
  }
  
  // no equality
  if ( ! _problem.hasFunctionSymbols () ) 
    return "EPR";
  if ( _problem.isHorn () ) 
    return "HNE";
  return "NNE";
} // Prepro::classify
*/


// 07/06/2001 changed (can only be called with a tab file)
// only the following simplifications are used:
//  (1) removal of x != t
//  (2) removal of tautologies
//  (3) removal of pure literals
//  (4) removal of relational function definitions
// the result is true if (4) has been applied 
// Basic simplifications plus
//  (5) removal of dummy function definitions f(x) = t with f not occurring anywhere else
// the result is the number of applicable (5) 
int Prepro::simplifyBasic () 
{
  int changes = 0;
  changes += _problem.removeEqualityAxioms ();
// I DON't KNOW WHY THESE TWO WERE COMMENTED OUT
//  changes += _problem.removeDummyPredicateDefinitions (tab);
//  changes += _problem.removePredicateSemiDefinitions (tab);

//  changes += _problem.clausify ();

  // now the problem is in clausal form

  // simplify first
//  changes += _problem.removeVarEquality ( tab );
//  changes += _problem.removeTautologies ( tab );
//  changes += _problem.eliminatePure ( tab );

//  changes += _problem.removeAllRelationalFunctionDefinitions (tab);

  // remove cancellation axioms
//  simplifyB (tab);

//  _problem.splitLargeArity ( 31 ); // must be tabulated!
  return changes;
} // Prepro::simplifyBasic


/*
// Basic simplifications plus
//  (5) removal of dummy function definitions f(x) = t with f not occurring anywhere else
// the result is the number of applicable (5) 
int Prepro::simplifyA ( ostream& tab ) 
{
  return _problem.Problem::removeDummyFunctionDefinitions(tab);
} // Prepro::simplifyA


// replace cancellation axioms f(X,y,Z) = f(X,u,Z) -> y = u by i(f(X,y,Z),X,Z) = y
// the result is the number of applicable (6) 
int Prepro::simplifyB (ostream& tab) 
{
  return _problem.replaceCancellationAxioms(tab);
} // Prepro::simplifyB


// Basic simplification plus
//  (5) removal of linear function definitions f(x) = t 
// the result is the number of applicable (5) 
int Prepro::simplifyC (ostream& tab) 
{
  simplifyA (tab);
  return _problem.Problem::removeLinearFunctionDefinitions (tab);
} // Prepro::simplifyC


// Basic simplification plus
//  (5) removal of all function definitions f(x) = t 
// the result is the number of applicable (5) 
int Prepro::simplifyE (ostream& tab) 
{
  simplifyC (tab);
  return _problem.Problem::removeAllFunctionDefinitions (tab);
} // Prepro::simplifyE


// Basic simplification plus
//  (5) elimination of resolvable literals
int Prepro::simplifyD (ostream& tab) 
{
  return _problem.eliminateResolvable (tab);
} // Prepro::simplifyD


// Basic simplification plus
//  (5) negative equality split, i.e. s != t changed into P(s) and ~P(t) for ground s
int Prepro::simplifyF (ostream& tab) 
{
  return _problem.negativeEqualitySplit (3,tab);
} // Prepro::simplifyF


// this is the version of prepro used for experiments with LRS
// for submitting a paper to the special issue of JSC 2000
// on first-order theorem proving
void Prepro::writeVampireCall ( const char* vampire, const char* problemFile, Options& options, 
                                int timeLimit, int memLimit, ostream& script )
{
  script << vampire 
         << ' ' 
         << options
         << " -t"
         << timeLimit
         << " -m"
         << memLimit
         << " "
         << problemFile
         << "\n\n";
} // Prepro::writeVampireCall


void Prepro::normalize () 
{
  _problem.normalize();
} // Prepro::normalize

// 06/04/2001
ostream& operator << ( ostream& str, Prepro& p )
{
  str << p.problem ();
  return str;
}

// ********************************************************************************************
//  Removed functions from previous versions
//    09/05/2001
//        this version was used for CASC-17, it can equally be used for any time-slicing experiments
//      void Prepro::casc17WriteVampireCall ( const char* vampire, const char* problemFile, Options& options, int timeLimit, int memLimit, ostream& script )
//        this is the version of prepro used for experiments with LRS
//        for submitting a paper to the special issue of JSC 2000 on first-order theorem proving
//      void Prepro::JSClrs () 
//        This is the version of prepro used at CASC-17
//      void Prepro::casc17 () 


*/