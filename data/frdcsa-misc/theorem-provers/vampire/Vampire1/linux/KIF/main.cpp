//==================================================================-*-C++-*-=
// File:        main.cpp
// Description: Just include functions to test parser for KTF and 
//              simplified XML
// Part of:     
// Status:      
// Created:     Jul 09, 2002, Andrey Chaltsev, chaltsev@cs.man.ac.uk
// Doc:         
// Compliance:
// Reviewed:    
//              
// Revised:     
//              
// Note:        
//============================================================================


#include <iostream> 
#include <fstream.h>

// include of KIF lexer and parser MUST be BEFORE XML lexer and parser
// to force correct odder of parsers generation and compilation
#include "KIFLexer.hpp"
#include "KIFParser.hpp"
// include of KIF lexer and parser MUST be BEFORE XML lexer and parser
// to force correct odder of parsers generation and compilation
#include "XMLLexer.hpp"
#include "XMLParser.hpp"

#include "antlr/RecognitionException.hpp"
#include "antlr/CharStreamException.hpp"
#include "antlr/TokenStreamRecognitionException.hpp"

#include "antlr/TokenStreamSelector.hpp"
// create selector to deal with both lexers simultaneously
// it must be global to allow parsers to switch between lexers
antlr::TokenStreamSelector selector;// = new antlr::TokenStreamSelector();


int main(int n_args, char ** args)
{
  if(n_args!=2)
    {
      std::cerr<<"ONE file is needed for parsing"; 
      exit(1);
    };
  
  std::ifstream inFile(args[1]);
  std::ofstream outFile("result");
  
  if(!inFile)
    {
      std::cerr<<"Error: can not open file:\""<<args[1]<<"\".\n"; 
      exit(1);
    };
  
  //ANTLR_USING_NAMESPACE(antlr);
  try {
    
    // create outter lexer and associate it with input stream
    XMLLexer xmlLexer(inFile); 
    // create inner lexer and associate it with the same stream as outer one's
    KIFLexer kifLexer(xmlLexer.getInputState()); 
    
    // set selector to deal with this two parsers
    selector.addInputStream( &xmlLexer, "XMLLexer");
    selector.addInputStream( &kifLexer, "KIFLexer");
    selector.select("XMLLexer");

    // create parser and run it
    XMLParser parser(selector);
    parser.start(std::cout);//outFile);
    
    outFile.close();
    
  } 
   catch (antlr::CharStreamException& io)
     {
       std::cerr << "Error: ANTLR I/O Exception : " <<io.toString() << "\n";
       exit(1);
     }
  catch(antlr::RecognitionException& e)
    {
      std::cerr << "Error: ANTLR Recognition Exception : "
                << e.toString() << "\n";
      exit(1);
    }  
  catch(antlr::TokenStreamRecognitionException& e)
    {
      std::cerr << "Error: ANTLR Token Recognition Exception: "
                << "line " << e.recog.getLine() 
                << " : " << e.toString() << '\n';
      exit(1);
    }  
  catch(antlr::TokenStreamException& e)
    {
      std::cerr << "Error: ANTLR Token Stream Exception : "
                << e.toString() << "\n";
      exit(1);
    }  
  catch(antlr::ANTLRException& e) 
    { 
      std::cerr << "Error: some ANTLR Exception: " << e.toString() << '\n';
      exit(1);
    }

  std::cout<<"\n---End---\n";               
  return 1;
};

// --- end of file ---
