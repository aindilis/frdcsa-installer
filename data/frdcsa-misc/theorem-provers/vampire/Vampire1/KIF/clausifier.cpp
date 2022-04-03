//
//  file clausifier.cpp
//  main file for the clausifier program
//
//  15/05/2000, Dallas
//

#include <string.h>
#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include <stdio.h>


// parser-related files
#include "pcctscfg.h"
#include "pccts_stdio.h"
#include "tokens.h"
#include "AParser.h"
#include "TPTPParser.h"
#include "DLGLexer.h"


#include "options.h"
#include "tracer.h"

/*
int classify ( int argc, char* argv [ ] )
{
  CommandLine comLine ( argc, argv );

  char* inputFileName;
  if ( ! comLine.findString("i", inputFileName) ) {
    cout << "Cannot read -i option\n";
    return EXIT_FAILURE;
  }

  char* tabFile = "prepro.tab";

  // making tab file for preprocessor 
  ofstream tab (tabFile,ios::app);
  if ( ! tab ) {
    cerr << "Cannot open tab file " << tabFile << "\n";
    return EXIT_FAILURE;
  }

  FILE* inputFile = fopen ( inputFileName, "r" );

  if ( ! inputFile ) {
    cerr << "Cannot open input file\n";
    return EXIT_FAILURE;
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

  parser.init ();

  // parsing
  Problem problem;
  Unit::List* fs = parser.input ();

  problem.build ( inputFileName, fs );
  fclose ( inputFile );
  Prepro prepro (problem);
  prepro.simplifyBasic (tab);
  Property prop (problem);

  char ppp [100];
  prop.toString (ppp);

  cout << "  '" << inputFileName << "' => '" << ppp << "',\n";

  tab.close ();

  return EXIT_SUCCESS;
}  // classify

int simplifyC ( int argc, char* argv [ ] )
{
  CommandLine comLine ( argc, argv );

  char* inputFileName;
  if ( ! comLine.findString("i", inputFileName) ) {
    cout << "Cannot read -i option\n";
    return EXIT_FAILURE;
  }

  FILE* inputFile = fopen ( inputFileName, "r" );

  if ( ! inputFile ) {
    cerr << "Cannot open input file\n";
    return EXIT_FAILURE;
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

  parser.init ();

  // parsing
  Problem* current = new Problem;
  Unit::List* fs = parser.input ();

  current->build ( inputFileName, fs );
  fclose ( inputFile );

  Prepro prepro ( *current ); // now the problem is normalized

  ofstream tab ("prepro.tab");
  if ( ! tab ) {
    cerr << "Cannot open tab file\n";
    return EXIT_FAILURE;
  }
  
  prepro.simplifyBasic (tab);
  prepro.normalize ();
  //cout << prepro;
  // int simplified = prepro.simplifyA (tab);
  // if ( simplified ) {
    // cout << prepro;
    // cout << inputFileName << ": " << simplified << "\n";
    // cout << "  '"  << inputFileName << "' => '" << prepro.classify() << "',\n";
  // }
  tab.close ();

//  Property prop (*current);

//  cout << "'" << inputFileName << "' => '" << prop << "',\n";

  return EXIT_SUCCESS;
}  // simplifyC
*/

int CASC ( int argc, char* argv [ ] )
{
  TRACER ("CASC");

  try {
    Options opts (argc, argv);
    options = &opts;

    sig = new Signature;

    const char* inputFileName = opts.file ();
    FILE* inputFile = fopen ( inputFileName, "r" );

    if ( ! inputFile ) {
      cerr << "Cannot open input file\n";
      return EXIT_FAILURE;
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

    parser.init ();

    //    cout << inputFileName << "\n";

    // parsing
    Unit::List* fs = parser.input ();
    fclose ( inputFile );

    Problem problem (fs);

    problem.preprocess (opts);

    cout << problem;
  }
  catch (MyException me) {
    me.cry (cerr);
    delete IntNameTable::vars;
    delete sig;
    exit (EXIT_SUCCESS);
  }
  catch (MemoryException me) {
    me.cry (cerr);
    delete IntNameTable::vars;
    delete sig;
    exit (EXIT_SUCCESS);
  };

//  cout << c;

//  cout << problem;

//  Prepro prepro (problem);
//  if ( int s = prepro.simplifyBasic () ) {
//    cout << s << " formulas removed\n" << flush;
//    cerr << inputFileName << ": " << s << " formulas removed\n" << flush;
//  }
//  Wizard wizard;
  
  // cout << "  '"  << inputFileName << "' => '";
//  wizard.execute ( problem, timeLimit, memLimit, tabFile, outFile, tab );
  // cerr << inputFileName << "\n" << flush;
  // cout << "',\n";

//  tab.close ();

  // these kludge are for checking memory leaks
  delete IntNameTable::vars;
  delete sig;

  return EXIT_SUCCESS;
}  // CASC


int main ( int argc, char* argv [ ] )
{
  TRACER ("main");

  return CASC (argc,argv);
  // return simplifyC (argc,argv);
//  return classify (argc,argv);
}
