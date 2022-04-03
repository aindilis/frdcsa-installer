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
// Revised:     Jul 14 2002, Andrei Voronkov, voronkov@cs.man.ac.uk
//              
// Note:        
//============================================================================


#include <iostream> 
#include <fstream.h>
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
antlr::TokenStreamSelector* selector;// = new antlr::TokenStreamSelector();


// kernel files
// #include "Tabulation.h"
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
#include "problem.h"
#include "int_name_table.h"
#include "formula.h"
#include "clause.h"
#include "query.h"


// VK::Tabulation* tabulation = 0;
void runtimeErrorHook();
void debugSystemInternalErrorHook();
void setSignalHandlers();
void cancelSignalHandling();
void outputRefutation (ostream&, const VampireKernel&, Problem&, int indent);
void outputRefutation (const VampireKernel::Clause* clause,
		       ostream& str, 
		       const VampireKernel& kernel, 
		       UlongList*& previousClauseNumbers,
		       int indent);
void outputRefutation (Unit* unit,
		       ostream& str, 
		       UlongList*& previousUnitNumbers,
		       int indent);
bool insertIfNotThere (ulong number, UlongList* & orderedList);
void outputClause (const VampireKernel::Clause* clause,
		   ostream& str, 
		   const VampireKernel& kernel,
		   int indent);
void outputClause (Clause* clause, ostream& str, int indent, int number);
void outputFormula (Formula* formula, ostream& str, int indent, int number);
void outputUnit (Unit* unit, ostream& str, int indent);
void outputSubterms (const VampireKernel::Subterm* subterms,
  		     ostream& str, 
		     const VampireKernel& kernel);
// void outputFormula (Formula* f, ostream& str);
Query* readQuery ();

// controlling the output syntax
const bool kifSyntax = true;

// XML output tags
const char* proofTag = "proof";
const char* proofStepTag = "proofStep";
const char* premisesTag = "premises";
const char* conclusionTag = "conclusion";
const char* clauseTag = "clause";
const char* formulaTag = "formula";

// KIF symbols
const char* kifNot = "not";

const char* basename(const char* fileName)
{
  int c = strlen(fileName) - 1; 
  while ((c >= 0) && (fileName[c] != '/')) c--;
  c++;
  char* res = new char[strlen(fileName) - c + 1];
  strcpy(res,fileName + c); 
  return res;
};


// const char* generateJobId()
// {
//   time_t sessionTime = time((time_t*)0); 
//   tm* gmt = gmtime(&sessionTime); 
//   int sessionDateDD = gmt->tm_mday;
//   int sessionDateMM = gmt->tm_mon;    
//   int sessionDateYY = 1900 + gmt->tm_year;
//   int sessionTimeHH = gmt->tm_hour;
//   int sessionTimeMM = gmt->tm_min;
//   int sessionTimeSS = gmt->tm_sec; 
//   pid_t sessionProcessId = getpid();
//   ostrstream str;
//   const char* hhh = (sessionTimeHH < 10) ? "0" : "";
//   const char* mmm = (sessionTimeMM < 10) ? "0" : "";
//   const char* sss = (sessionTimeSS < 10) ? "0" : "";
//   str << sessionDateDD << sessionDateMM  << sessionDateYY;
//   str << hhh << sessionTimeHH << mmm << sessionTimeMM << sss << sessionTimeSS;
//   str << sessionProcessId << '\0';
//   char* res = new char[strlen(str.str()) + 1];
//   strcpy(res,str.str());
//   return res; 
  
// }; // const char* generateJobId()


// ofstream* createTab (Options& opts, const char* jobId)
// {
//   // read tab file from options
//   const char* tabulationFileName = opts.tab ();
//   const char* testId = opts.testId ();

//   if (! tabulationFileName ) {
//     return 0;
//   }

//   ofstream* tabStream = new ofstream;
//   tabStream->open (tabulationFileName, ios::app);
//   if ( ! (*tabStream) ) {
//     throw MyException ( "Can not open file for tabulation");
//   };

//   tabulation = new VK::Tabulation();
//   tabulation->assignStream (tabStream);
//   tabulation->assignJob (jobId);

//   const char* problemFileBaseName = basename (opts.file());
//   tabulation->header (testId, problemFileBaseName);   
//   tabulation->problemName (problemFileBaseName);
// #ifndef NO_DEBUG
//   BK::Debug::assignJob(problemFileBaseName);
// #endif 

//   tabulation->testId (testId);

//   char hostname[256];
//   gethostname(hostname,256);
//   tabulation->hostName ((const char*)hostname);

//   time_t jobTime = time((time_t*)0);
//   tm* gmt = gmtime(&jobTime); 
//   int jobDateDD = gmt->tm_mday;
//   int jobDateMM = gmt->tm_mon;    
//   int jobDateYY = 1900 + gmt->tm_year;
//   int jobTimeHH = gmt->tm_hour;
//   int jobTimeMM = gmt->tm_min;
//   int jobTimeSS = gmt->tm_sec;

//   tabulation->jobDate(jobDateDD,jobDateMM,jobDateYY);
//   tabulation->jobTime(jobTimeHH,jobTimeMM,jobTimeSS);
//   tabulation->version(VampireKernel::versionNumber(),
// 		      VampireKernel::versionDescription());
//   tabulation->versionDate();
//   tabulation->versionTime();

//   return tabStream;
// } // createTab


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
//   kernel.proof = vampire.proof ();
  // in KIF the proof output is handled by the preprocessor
  kernel.proof = false;
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
  kernel.show_gen = vampire.showGen ();
  kernel.show_passive = vampire.showPassive ();
  kernel.show_active = vampire.showActive ();
  kernel.show_profile = vampire.showProfile ();
  kernel.show_input = vampire.showInput ();
  kernel.show_opt = false; // for the KIF version 
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
  sig->addArithmetic (); // add arithmetic to the signature

  kernel.beginInputSignature();
  
  // pass predicate symbols
  Signature::PredIterator preds (sig);
  while (preds.more()) {
    ulong symbolId; // to return symbolId
    Signature::Pred* pred = static_cast<Signature::Pred*>(preds.next());
    kernel.registerInputPredicate(pred->name(),
				  pred->arity(),
				  sig->isAnswer(pred), // answer predicate sign
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

  // tell the kernel to interpret function and predicate symbols
  kernel.interpretFunction ("+", 2, "floating_point_arithmetic", "binary_plus");
  kernel.interpretFunction ("-", 2, "floating_point_arithmetic", "binary_minus");
  kernel.interpretFunction ("-", 1, "floating_point_arithmetic", "unary_minus");
  kernel.interpretFunction ("*", 2, "floating_point_arithmetic", "multiply");
  kernel.interpretFunction ("min", 2, "floating_point_arithmetic", "min2");
  kernel.interpretFunction ("max", 2, "floating_point_arithmetic", "max2");

  kernel.interpretPredicate (">", 2, "floating_point_arithmetic", "greater");
  kernel.interpretPredicate (">=", 2, "floating_point_arithmetic", "greater_or_equal");
  kernel.interpretPredicate ("<=", 2, "floating_point_arithmetic", "less_or_equal");
  kernel.interpretPredicate ("<", 2, "floating_point_arithmetic", "less");
} // passSignature


// output sp spaces to str
// 23/07/2002 Torrevieja
void spaces (ostream& str, int sp)
{
  ASS(debug_vampire, sp >= 0);

  while (sp > 0) {
    sp --;
    str << ' ';
  }
} // spaces


// output answer (if any) to str
// 23/07/2002 Torrevieja
void outputAnswer (ostream& str, 
		   const VampireKernel& kernel, 
		   Problem& problem,
		   int indent,
		   int number)
{
  spaces (str, indent);
  str << "<answer result='yes' number='" << number << "'>\n";

  if (problem.answerLiteral()) { // no answer literal - no binding
    const VampireKernel::Literal* goalLiterals = kernel.refutation()->literals();

    spaces (str, indent+2);
    str << "<bindingSet type='";
    if (! goalLiterals) { // anything is the answer
      str << "inconsistent";
    }
    else if (! goalLiterals->next()) { // exactly one answer literal
      str << "definite";
    }
    else {
      str << "disjunctive";
    }
    str << "'>\n";

    // output each binding
    while (goalLiterals) {
      spaces (str, indent+4);
      str << "<binding>\n";

      const VampireKernel::Subterm* subterms = goalLiterals->firstArgument();
      VarList::Iterator vs (problem.originalAnswerVariables());
      while (vs.more()) {
	ASS (debug_vampire, subterms);
	spaces (str, indent+6);
	cout << "<var name='" 
	     << (*IntNameTable::vars) [vs.next()]
	     << "' value='";
	if (subterms->isVariable()) {
	  str << "?X" << subterms->variableNumber ();
	}
	else { // non-variable
	  const VampireKernel::Subterm* args = subterms->firstArgument ();
	  if (args) {
	    str << '(';
	  }
	
	  ulong symbol = subterms->topFunction();
	  if (kernel.isNumericConstantId(symbol)) {
	    str << kernel.numericConstantValue (symbol);
	  }
	  else {
	    str << kernel.symbolPrintName (symbol);
	  }
	  if (args) { // there are arguments
	    outputSubterms (args, str, kernel);
	  }
	}
	str << "'/>\n";
	subterms = subterms->next ();
      }

      spaces (str, indent+4);
      str << "</binding>\n";
      goalLiterals = goalLiterals->next();
    }
    spaces (str, indent+2);
    str << "</bindingSet>\n";
  }

  outputRefutation (str, kernel, problem, indent+2);
  spaces (str, indent+2);
  str << "</answer>\n";
} // outputAnswer


// output refutation to str
// 23/07/2002 Torrevieja
void outputRefutation (ostream& str, 
		       const VampireKernel& kernel, 
		       Problem& problem, 
		       int indent)
{
  TRACER ("outputRefutation/4");

  if (kifSyntax) {
    spaces (str, indent);
    str << '<' << proofTag << ">\n";
  }

  UlongList* clauseNumbers = UlongList::empty ();
  outputRefutation (kernel.refutation (), str, kernel, clauseNumbers, indent+2);
  clauseNumbers->destroy ();

  if (kifSyntax) {
    spaces (str, indent);
    str << "</" << proofTag << ">\n";
  }
} // outputRefutation


void outputRefutation (const VampireKernel::Clause* clause,
		       ostream& str, 
		       const VampireKernel& kernel, 
		       UlongList*& previousClauseNumbers,
		       int indent)
{
  TRACER ("outputRefutation/5");

  if (clause->isInputClause()) { // the clause is an input clause 
    // output its parent unit
    outputRefutation ((Unit*)clause->inputClauseOrigin(),
		      str,
		      previousClauseNumbers,
		      indent);
  }
  else { // the clause is not an input clause
    // output all parents of the clause
    for (const VampireKernel::Clause::Ancestor* parent = clause->firstAncestor();
	 parent;
	 parent = parent->next()) {
      ASS (debug_vampire, parent->clause() != clause);

      if (parent->clause()->isBuiltInTheoryFact()) {
	continue;
      }

      outputRefutation (parent->clause(), 
			str, 
			kernel, 
			previousClauseNumbers, 
			indent);
    }
  }

  if (insertIfNotThere (clause->number(), previousClauseNumbers)) {
    // the clause was not output previously
    if ( kifSyntax ) {
      spaces (str, indent);
      str << '<' << proofStepTag << ">\n";

      // output premises
      spaces (str, indent+2);
      str << '<' << premisesTag << ">\n";
      if (clause->isInputClause()) { // the clause is an input clause 
	// output its parent unit
	outputUnit ((Unit*)clause->inputClauseOrigin(), str, indent+4);
      }
      else { // not an input clause
	for (const VampireKernel::Clause::Ancestor* parent = clause->firstAncestor();
	     parent;
	     parent = parent->next()) {
	  if (parent->clause()->isBuiltInTheoryFact()) {
	    continue;
	  }
	  outputClause (parent->clause(), 
			str, 
			kernel, 
			indent + 4);
	}
      }
      spaces (str, indent+2);
      str << "</" << premisesTag << ">\n";

      // output conclusion
      spaces (str, indent+2);
      str << '<' << conclusionTag << ">\n";
      outputClause (clause, str, kernel, indent+4);
      spaces (str, indent+2);
      str << "</" << conclusionTag << ">\n";

      spaces (str, indent);
      str << "</" << proofStepTag << ">\n";
    }
    else { // not KIF syntax
      outputClause (clause, str, kernel, 0);
      str << "\n";
    }
  }
  // the number of the clause was already in previousClauseNumbers
  // nothing to do
} // outputRefutation


void outputRefutation (Unit* unit,
		       ostream& str,
		       UlongList*& previousUnitNumbers,
		       int indent)
{
  TRACER ("outputRefutation/4.1");

  Unit::List::Iterator parents (unit->parents());
  while ( parents.more() ) {
    outputRefutation (parents.next(), str, previousUnitNumbers, indent);
  }

  if (insertIfNotThere ((ulong)unit->number(), previousUnitNumbers)) {
    if (kifSyntax) {
      spaces (str, indent);
      str << '<' << proofStepTag << ">\n";

      // output premises
      spaces (str, indent+2);
      str << '<' << premisesTag << ">\n";
      for (Unit::List* parents = unit->parents();
	   parents;
	   parents = parents->tail()) {
	outputUnit (parents->head(), str, indent+4);
      }
      spaces (str, indent+2);
      str << "</" << premisesTag << ">\n";

      // output conclusion
      spaces (str, indent+2);
      str << '<' << conclusionTag << ">\n";
      outputUnit (unit, str, indent+4);
      spaces (str, indent+2);
      str << "</" << conclusionTag << ">\n";

      spaces (str, indent);
      str << "</" << proofStepTag << ">\n";
    }
    else { // not KIF syntax
      // the unit was not output previously
      outputUnit (unit, str, 0);
      str << "\n";
    }
  }
  // the number of the clause was already in previousClauseNumbers
  // nothing to do
} // outputRefutation


bool insertIfNotThere (ulong number, UlongList* & orderedList)
{
  if ( orderedList->isEmpty() || number > orderedList->head() ) {
    UlongList::push (number, orderedList);
    return true;
  }
  UlongList* current = orderedList;
  for (;;) {
    if (number == current->head()) { // found in the list
      return false;
    }
    UlongList* next = current->tail ();
    if (next->isEmpty() || number > next->head() ) {
      current->tail ( new UlongList(number, next) );
      return true;
    }
    current = next;
  }
} // insertIfNotThere


void outputSubterms (const VampireKernel::Subterm* subterms,
  		     ostream& str, 
		     const VampireKernel& kernel)
{
  if (kifSyntax) {
    while (subterms) {
      // output the first subterm
      str << ' ';
      if (subterms->isVariable()) {
	str << "?X" << subterms->variableNumber ();
      }
      else { // non-variable
	const VampireKernel::Subterm* args = subterms->firstArgument ();
	if (args) {
	  str << '(';
	}
	
	ulong symbol = subterms->topFunction();
	if (kernel.isNumericConstantId(symbol)) {
	  str << kernel.numericConstantValue (symbol);
	}
	else {
	  str << kernel.symbolPrintName (symbol);
	}
	if (args) { // there are arguments
	  outputSubterms (args, str, kernel);
	}
      }
      subterms = subterms->next ();
    }
    str << ')';
  }
  else { // not KIF syntax
    // output the first subterm
    if (subterms->isVariable()) {
      str << 'X' << subterms->variableNumber ();
    }
    else { // non-variable
      ulong symbol = subterms->topFunction();
      if (kernel.isNumericConstantId(symbol)) {
	str << kernel.numericConstantValue (symbol);
      }
      else {
	str << kernel.symbolPrintName (symbol);
      }
      const VampireKernel::Subterm* args = subterms->firstArgument ();
      if (args) { // there are arguments
	str << '(';
	outputSubterms (args, str, kernel);
	str << ')';
      }
    }

    // output the rest of the subterms, if any
    const VampireKernel::Subterm* rest = subterms->next();
    if (rest) {
      str << ',';
      outputSubterms (rest, str, kernel);
    }
  }
} // outputSubterms


void outputLiterals (const VampireKernel::Literal* literals,
  		     ostream& str, 
		     const VampireKernel& kernel,
		     int indent)
{
  if (kifSyntax) {
    while (literals) {
      spaces(str, indent);
      if (! literals->isPositive()) {
        str << '(' << kifNot << ' ';
      }
      const VampireKernel::Subterm* args = literals->firstArgument ();
      if (args) {
	str << '(';
      }
      const char* pred =  kernel.symbolPrintName (literals->predicate());
      if (strcmp ("equal", pred)) { // not equality
	str << pred;
      }
      else {
	str << '=';
      }
      if (args) {
        outputSubterms (args, str, kernel);
      }
      if (! literals->isPositive()) {
        str << ')';
      }
      str << "\n";
      literals = literals->next ();
    }
  }
  else {
    // output the first literal
    if (! literals->isPositive()) {
      str << '~';
    }
    str << kernel.symbolPrintName (literals->predicate());
    const VampireKernel::Subterm* args = literals->firstArgument ();
    if (args) { // there are arguments
      str << '(';
      outputSubterms (args, str, kernel);
      str << ')';
    }

    // output the rest of the literals, if any
    const VampireKernel::Literal* rest = literals->next();
    if (rest) {
      str << " | ";
      outputLiterals (rest, str, kernel, indent);
    }
  }
} // outputLiterals


void outputClause (const VampireKernel::Clause* clause,
		   ostream& str, 
		   const VampireKernel& kernel,
		   int indent)
{
  if (kifSyntax) {
    spaces (str, indent);
    str << "<" << clauseTag << " number='" << clause->number() << "'>\n";
    outputLiterals (clause->literals(), str, kernel,indent+2);
    spaces (str, indent); 
    str << "</" << clauseTag << ">\n"; 
  }
  else { // not KIF syntax
    str << clause->number () << ". ";
    const VampireKernel::Literal* literals = clause->literals();
    if (literals) { // non-empty clause
      outputLiterals (literals, str, kernel, indent);
    }
    else { // empty clause
      str << "#";
    }

    // output parents
    str << " [";
    if (clause->isInputClause()) {
      Unit* parent = (Unit*)clause->inputClauseOrigin();
      str << parent->number();
    }
    else {
      for (const VampireKernel::Clause::Ancestor* parents = clause->firstAncestor();
	   parents;
	   parents = parents->next()) {
	if (! parents->clause()->isBuiltInTheoryFact()) {
	  str << parents->clause()->number();
	  if (parents->next()) {
	    str << ',';
	  }
	}
      }
    }
    str << ']';
  }
} // outputClause


void outputLiteral (Literal* literal, ostream& str)
{
  ASS(debug_vampire, kifSyntax);

  if (literal->negative()) {
    str << '(' << kifNot << ' ';
  }
  literal->atom()->outputKIF (str);
  if (literal->negative()) {
    str << ')';
  }
} // outputLiteral


void outputClause (Clause* clause,
		   ostream& str, 
		   int indent,
		   int clauseNumber)
{
  ASS(debug_vampire, kifSyntax);
  spaces (str, indent);
  str << "<" << clauseTag << " number='" << clauseNumber << "'>\n";

  Literal::List::Iterator literals (clause->literals());
  while (literals.more()) {
    spaces (str, indent+2);
    outputLiteral (literals.next(), str);
    str << "\n";
  }

  spaces (str, indent); 
  str << "</" << clauseTag << ">\n"; 
} // outputClause


void outputFormula (Formula* formula,
		    ostream& str, 
  		    int indent,
		    int formulaNumber)
{
  ASS(debug_vampire, kifSyntax);

  spaces (str, indent);
  str << "<" << formulaTag << " number='" << formulaNumber << "'>\n";

  formula->outputKIF (str, indent+2);
  str << "\n";

  spaces (str, indent); 
  str << "</" << formulaTag << ">\n"; 
} // outputClause


void outputUnit (Unit* unit, ostream& str, int indent)
{
  if (kifSyntax) {
    if (unit->unitType() == CLAUSE) {
      outputClause (unit->clause (), str, indent, unit->number());
    }
    else {
      outputFormula (unit->formula (), str, indent, unit->number());
    }    
  }
  else { // not KIF syntax
    str << unit->number () << ". ";
    if (unit->unitType() == CLAUSE) {
      str << unit->clause ();
    }
    else {
      str << unit->formula ();
    }

    str << " [";
    for (Unit::List* parents = unit->parents(); parents; parents = parents->tail()) {
      str << parents->head()->number();
      if (parents->tail()) {
	str << ',';
      }
    }
    str << ']';
  }
} // outputUnit


int passClauses (Problem& problem, VampireKernel& kernel)
{
  kernel.beginInputClauses();
  int lastClauseNumber = 0;

  Unit::Chain::Iterator units (problem.units());

  while (units.more()) {
    // pick the next clause from the problem
    Unit* u = units.next ();
    ASS(debug_vampire, u->unitType() == CLAUSE);
    Clause* c = u->clause ();
    //    cout << c << "\n";
    int lastVar = -1;
    Renaming renaming;

    // read its number
    int number = u->number();
    if (number > lastClauseNumber) {
      lastClauseNumber = number;
    }
    
    // communicate the clause
    kernel.beginInputClause (number, 
			     u, 
			     u->inputType() != AXIOM,  // goal flag
			     u->inputType() == AXIOM); // SOS flag

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
  
      kernel.saturate(lastClauseNumber+1,timeLimit);
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


char rerun (VampireKernel& kernel)
{
  try 
    {
      kernel.lookForAnotherAnswer();
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
} // rerun (VampireKernel& kernel)


void runtimeErrorHook()
{
  if (VampireKernel::currentSession())
    {
      cout << "% Trying to shut down current kernel session...\n";
      VampireKernel::currentSession()->reportError();
    };
//   if (tabulation)
//     {
//       tabulation->errorMessage("runtime","some module detected inconsistency in run time");
//     };
}; // void runtimeErrorHook()


void debugSystemInternalErrorHook()
{  
  if (VampireKernel::currentSession())
    {
      cout << "% Trying to shut down current kernel session...\n";
      VampireKernel::currentSession()->reportError();
    };
//   if (tabulation)
//     {
//       tabulation->errorMessage("debug","debugging system detected internal inconsistency");
//     };
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
// 	  VampireKernel::currentSession()->tabulate();
	};
//       if (tabulation) tabulation->result('0');
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
//               VampireKernel::currentSession()->tabulate();
	    }
// 	    if (tabulation) tabulation->result('0');
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
//           VampireKernel::currentSession()->tabulate();
	};
//       if (tabulation)
// 	{
// 	  const char* errorQualifier = "Error signal";
// 	  char errorMessage[256];
// 	  sprintf(errorMessage,"error signal %d received",sigNum);
// 	  tabulation->errorMessage(errorQualifier,errorMessage);
// 	};
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


Formula::List* readInput (const char* inputFileName)
{

  std::ifstream inFile (inputFileName);
  
  if(!inFile) {
    return 0;
  };

  Formula::List* formulas;

  try {
    KIFLexer kifLexer(inFile); 
    KIFParser parser (kifLexer);
    formulas = parser.start();
  } 
   catch (antlr::CharStreamException& io)
     {
       std::cerr << "Error: ANTLR I/O Exception : " <<io.toString() << "\n";
       return 0;
     }
  catch(antlr::RecognitionException& e)
    {
      std::cerr << "Error: ANTLR Recognition Exception : "
                << e.toString() << "\n";
      return 0;
    }  
  catch(antlr::TokenStreamRecognitionException& e)
    {
      std::cerr << "Error: ANTLR Token Recognition Exception: "
                << "line " << e.recog.getLine() 
                << " : " << e.toString() << '\n';
      return 0;
    }  
  catch(antlr::TokenStreamException& e)
    {
      std::cerr << "Error: ANTLR Token Stream Exception : "
                << e.toString() << "\n";
      return 0;
    }  
  catch(antlr::ANTLRException& e) 
    { 
      std::cerr << "Error: some ANTLR Exception: " << e.toString() << '\n';
      return 0;
    }

  return formulas;
} // readInput


void kif_loop ( Options& opts, 
	        Formula::List* formulas, 
	        BK::GlobalClock::Timer& timer )
{
  TRACER ("kif_loop");

  Problem problem (formulas);
  // preprocess the problem
  problem.preprocess (opts);

  // create options for the kernel
  VampireKernel::Options kernelOptions;
  kernelOptions.std_output = 0; // to disable output
  // passOptions (opts, kernelOptions, jobId);
  passOptions (opts, kernelOptions, "");

  // main KIF loop
  for (;;) {
    // read next query
    Query* query = readQuery ();
    if (! query) { // something is wrong, maybe syntax error
      //      cerr << "syntax error detected\n";
      // do nothing
      if (kifSyntax) {
	cout << "<assertionResponse>\n"
	  "  Syntax error detected\n"
	  "</assertionResponse>\n";
      }
      continue;
    }

    // process query
    switch (query->type()) {
    case Query::ASSERTION:
      cout << "<assertionResponse>\n"
	"  Formula has been added to the session database\n"
	"</assertionResponse>\n";
      // add assertion formula to the problem
      problem.addUnit (new Unit (0, HYPOTHESIS, query->formula()));
      break;

    case Query::GOAL: {
      int answers = 0;
      if (kifSyntax) {
	cout << "<queryResponse>\n";
      }

      // add goal formula to the problem
      problem.addUnit (new Unit (0, 
				 CONJECTURE, 
				 new Formula (Formula::NOT, query->formula())));
      problem.preprocess (opts);
//       if (problem.answerLiteral()) {
// 	cout << "Answer literal: " << problem.answerLiteral () << "\n";
// 	cout << "Answer variables: ";
// 	VarList::Iterator vs (problem.originalAnswerVariables());
// 	while (vs.more()) {
// 	  cout << (*IntNameTable::vars) [vs.next()] << ' ';
// 	}
// 	cout << "\n";
//       }

      // create new kernel
      VampireKernel kernel;

      // run the kernel until the time expires
      char result = run (kernel, 10 * query->timeLimit(), kernelOptions, problem);

      switch (result) {
      case '+': {
	answers ++;
	outputAnswer (cout, kernel, problem, 2, answers);
	for (int attempts = query->bindingsLimit() - 1; attempts > 0; attempts--) {
	  result = rerun (kernel);
	  switch (result) {
	  case '+':
	    answers++;
	    outputAnswer (cout, kernel, problem, 2, answers);
	    break;

	  case '-':
	  case '0':
	    if ( ! kifSyntax ) {
	      cout << "No more proofs\n";
	    }

	    // no more attempts in any of these cases:
	    attempts = 0;
	    break;

	  default:
	    ASS (debug_vampire, false);
	  }
	}
	break;
      }

      case '-':
	if ( ! kifSyntax ) {
	  cout << "Satisfiability detected\n";
	}
	break;

      case '0':
	if ( ! kifSyntax ) {
	  cout << "Refutation not found\n";
	}
	break;
      }

      //      kernel.closeSession(); 
      problem.removeAllConjectures ();
      if (kifSyntax) {
	if (answers == 0) { // no proof found
	  cout <<
	    "  <answer result='no'>\n"
	    "  </answer>\n";
	}
	else {
	}
	cout << "  <summary proofs='" << answers << "'/>\n";
	cout << "</queryResponse>\n";
      }
      break;
    }

    case Query::TERMINATE:
      delete query;
      return;

    default:
      ASS (debug_vampire, false);
    }

    delete query;
  }
//   // tabulate the result
//   kernel.tabulate ();
//   if (tabulation) tabulation->result(result);
}  // kif_loop


int main ( int argc, char* argv [ ] )
{
  TRACER ("main");

  BK::GlobalClock::Timer timer;

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
    Options opts (argc, argv);
    options = &opts;

    // open input file
    const char* inputFileName = opts.file ();
    Formula::List* formulas = readInput (inputFileName);
    if (! formulas) {
      cerr << "Cannot open input file or syntax error in the file\n";
      return EXIT_FAILURE;
    }

    // create problem from the input
    kif_loop (opts, formulas, timer);

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


Query* readQuery ()
{
  TRACER ("readQuery");

  // selector for dealing with both lexers simultaneously
  antlr::TokenStreamSelector select;
  selector = &select;

  try {
    // create outter lexer and associate it with input stream
    XMLLexer xmlLexer(std::cin); 
    // create inner lexer and associate it with the same stream as outer one's
    KIFLexer kifLexer(xmlLexer.getInputState()); 
    
    // set selector to deal with this two parsers
    selector->addInputStream( &xmlLexer, "XMLLexer");
    selector->addInputStream( &kifLexer, "KIFLexer");
    selector->select("XMLLexer");

    // create parser and run it
    XMLParser parser(select);

    return parser.start ();
  } 
   catch (antlr::CharStreamException& io)
     {
       // std::cerr << "Error: ANTLR I/O Exception : " <<io.toString() << "\n";
       return 0;
     }
  catch(antlr::RecognitionException& e)
    {
      // std::cerr << "Error: ANTLR Recognition Exception : "
      //           << e.toString() << "\n";
      return 0;
    }  
  catch(antlr::TokenStreamRecognitionException& e)
    {
      // std::cerr << "Error: ANTLR Token Recognition Exception: "
//                 << "line " << e.recog.getLine() 
//                 << " : " << e.toString() << '\n';
      return 0;
    }  
  catch(antlr::TokenStreamException& e)
    {
//       std::cerr << "Error: ANTLR Token Stream Exception : "
//                 << e.toString() << "\n";
      return 0;
    }  
  catch(antlr::ANTLRException& e) 
    { 
//       std::cerr << "Error: some ANTLR Exception: " << e.toString() << '\n';
      return 0;
    }
} // readQuery

// int main(int n_args, char ** args)
// {
//   if(n_args!=2)
//     {
//       std::cerr<<"ONE file is needed for parsing"; 
//       exit(1);
//     };
  
//   std::ifstream inFile(args[1]);
//   std::ofstream outFile("result");
  
//   if(!inFile)
//     {
//       std::cerr<<"Error: can not open file:\""<<args[1]<<"\".\n"; 
//       exit(1);
//     };
  
//   //ANTLR_USING_NAMESPACE(antlr);
//   try {
    
//     // create outter lexer and associate it with input stream
//     XMLLexer xmlLexer(inFile); 
//     // create inner lexer and associate it with the same stream as outer one's
//     KIFLexer kifLexer(xmlLexer.getInputState()); 
    
//     // set selector to deal with this two parsers
//     selector.addInputStream( &xmlLexer, "XMLLexer");
//     selector.addInputStream( &kifLexer, "KIFLexer");
//     selector.select("XMLLexer");

//     // create parser and run it
//     XMLParser parser(selector);
//     parser.start(std::cout);//outFile);
    
//     outFile.close();
    
//   } 
//    catch (antlr::CharStreamException& io)
//      {
//        std::cerr << "Error: ANTLR I/O Exception : " <<io.toString() << "\n";
//        exit(1);
//      }
//   catch(antlr::RecognitionException& e)
//     {
//       std::cerr << "Error: ANTLR Recognition Exception : "
//                 << e.toString() << "\n";
//       exit(1);
//     }  
//   catch(antlr::TokenStreamRecognitionException& e)
//     {
//       std::cerr << "Error: ANTLR Token Recognition Exception: "
//                 << "line " << e.recog.getLine() 
//                 << " : " << e.toString() << '\n';
//       exit(1);
//     }  
//   catch(antlr::TokenStreamException& e)
//     {
//       std::cerr << "Error: ANTLR Token Stream Exception : "
//                 << e.toString() << "\n";
//       exit(1);
//     }  
//   catch(antlr::ANTLRException& e) 
//     { 
//       std::cerr << "Error: some ANTLR Exception: " << e.toString() << '\n';
//       exit(1);
//     }

//   return 1;
// };

// --- end of file ---
