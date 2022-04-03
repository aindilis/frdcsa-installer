//
//  file options.cpp
//  implements Vampire options
//
//  11/06/2000, Manchester
//  11/12/2000, Manchester, subsumption modes added, defaults changed to optimized subsumption
//  31/12/2000, Manchester, splitting-related options changed
//  03/01/2001, Manchester, demodulationOnSplittingBranches added
//  06/06/2001 Manchester, completely rewritten
//


#include <string.h>
#include <stdlib.h>
#include <iostream.h>
#include <limits.h>

#include "assert.h"
#include "options.h"
#include "tracer.h"


Options* options = 0;


Options::Options (int argc, char* argv [])
  :
  _file (0),
  _casc (Opt::CASC, false, "casc"),
  _removeUnusedDefs (Opt::REMOVE_UNUSED_DEF, false, "remove_unused_defs"),
  _arityCheck (Opt::ARITY_CHECK, false, "arity_check"),
  _noFreeVars (Opt::NO_FREE_VARS, false, "no_free_vars"),
  _miniscope (Opt::MINISCOPE, false, "miniscope"),
  _outputEqualityInfix (Opt::OUTPUT_EQUALITY_INFIX, false, "output_equality_infix"),
  // Kernel's options
  _mainAlg (Opt::MAIN_ALG, OTTER, "man_alg"),
  _staticSplitting (Opt::STATIC_SPLITTING, false, "static_splitting"),
  _dynamicSplitting (Opt::DYNAMIC_SPLITTING, false, "dynamic_splitting"),
  _splittingStyle (Opt::SPLITTING_STYLE, PARALLEL, "splitting_style"),
  _splittingWithNaming (Opt::SPLITTING_WITH_NAMING, true, "splitting_with_naming"),
  _selection (Opt::SELECTION, P_4, "selection"),
  _literalComparisonMode (Opt::LITERAL_COMPARISON_MODE, NORMAL, "literal_comparison_mode"),
  _inheritedNegativeSelection (Opt::INHERITED_NEGATIVE_SELECTION, false, "inherited_negative_selection"),
  _fsSetMode (Opt::FS_SET_MODE, false, "fs_set_mode"),
  _fsOptimisedMultisetMode (Opt::FS_OPTIMISED_MULTISET_MODE, false, "fs_optimised_multiset_mode"),
  _simplifyByFs (Opt::SIMPLIFY_BY_FS, SIMP, "simplify_by_fs"),
  _forwardDemodulation (Opt::FORWARD_DEMODULATION, ALL, "forward_demodulation"),  
  _fdOnSplittingBranches (Opt::FD_ON_SPLITTING_BRANCHES, true, "fd_on_splitting_branches"),
  _backwardDemodulation (Opt::BACKWARD_DEMODULATION, ALL, "backward_demodulation"),
  _bdOnSplittingBranches (Opt::BD_ON_SPLITTING_BRANCHES, true, "bd_on_splitting_branches"),
  _backwardSubsumption (Opt::BACKWARD_SUBSUMPTION, false, "backward_subsumption"),
  _bsSetMode (Opt::BS_SET_MODE, false, "bs_set_mode"),
  _bsOptimisedMultisetMode (Opt::BS_OPTIMISED_MULTISET_MODE, false, "bs_optimised_multiset_mode"),
  _orphanMurder (Opt::ORPHAN_MURDER, false, "orphan_murder"),
  _lrs (Opt::LRS, LRS_YES, "lrs"),
  _elimDef (Opt::ELIM_DEF, 0, "elim_def"),
  _symbolPrecedenceByArity (Opt::SYMBOL_PRECEDENCE_BY_ARITY, ARITY, "symbol_precedence_by_arity"),
  _headerPrecedenceKinky (Opt::HEADER_PRECEDENCE_KINKY, false, "header_precedence_kinky"),
  _timeLimit (Opt::TIME_LIMIT, LONG_MAX, "time_limit"),
  _memoryLimit (Opt::MEMORY_LIMIT, 300000, "memory_limit"),
  _maxSkolem (Opt::MAX_SKOLEM, 4096, "max_skolem"),
  _maxPassive (Opt::MAX_PASSIVE, LONG_MAX, "max_passive"),
  _maxWeight (Opt::MAX_WEIGHT, LONG_MAX, "max_weight"),
  _ageWeightRatio (Opt::AGE_WEIGHT_RATIO, 5, "age_weight_ratio"),
  _proof (Opt::PROOF, true, "proof"),
  _tab (Opt::TAB, 0, "tab"),
  _testId (Opt::TEST_ID, "unspecified_test", "tab"),
  _paramodulation (Opt::PARAMODULATION, true, "paramodulation"),
  _symbolWeightByArity (Opt::SYMBOL_WEIGHT_BY_ARITY, 0, "symbol_weight_by_arity"),
  _termWeightingScheme (Opt::TERM_WEIGHTING_SCHEME, TWS_UNIFORM, "term_weighting_scheme"),
  _simplificationOrdering (Opt::SIMPLIFICATION_ORDERING, KBO_NONREC, "simplification_ordering"),
  _nongoalPenaltyCoefficient (Opt::NONGOAL_PENALTY_COEFFICIENT, 1.0, "nongoal_penalty_coefficient" ),
  _kifInitialDatabase (Opt::KIF_INITIAL_DATABASE, 0, "kif_initial_database"),
  _showGen (Opt::SHOW_GEN, false, "show_gen"),
  _showPassive (Opt::SHOW_PASSIVE, false, "show_passive"),
  _showActive (Opt::SHOW_ACTIVE, false, "show_active"),
  _showProfile (Opt::SHOW_PROFILE, false, "show_profile"),
  _showInput (Opt::SHOW_INPUT, false, "show_input"),
  _tptpOutput (Opt::TPTP_OUTPUT, false, "tptp_output")
{
  TRACER ( "Options::Options" );

  char** last = argv + argc;
  char** current = argv + 1;

  while (current != last) {
    Opt* opt = find (*current); // find option by name
    if (opt) { // found
      current++; // advance current, it should now point to the value of the option
      if (current == last) { // wrong, no value
        cerr << "Error in command line after " << current[-1] << "\n";
	throw MyException ("Terminated by error in command line");
      }
      // value found
      if (! opt->set (*current)) { // option not set successfully
        cerr << "Error in command line: a bad value ("
             << current[0] << ") for "
             << current[-1] << "\n";
	throw MyException ("Terminated by error in command line");
      }
      // now option is set successfully, go to the next option
      current++;
    }
    // option not found, should be the input file name
    else if (current+1 != last) { // there is something beyond the file name
      cerr << "Error in command line: " << *current << " is not a valid option\n";
      throw MyException ("Terminated by error in command line");
    }
    else { // the last option, should be the file name then
      _file = *current;

      const long maxTime = LONG_MAX/10;
      if (timeLimit() > maxTime) {
	timeLimit (maxTime);
      }

      // changing time to deciseconds
      timeLimit (timeLimit()*10);
      return;
    }
  }
  
  cerr << "Error in command line: input file name missing\n";
  throw MyException ("Terminated by error in command line");
} // Options::Options


// simply a copy of the previous options, without initialising
// from the command line
Options::Options ()
  :
  _file (0),
  _casc (Opt::CASC, false, "casc"),
  _removeUnusedDefs (Opt::REMOVE_UNUSED_DEF, true, "remove_unused_defs"),
  _arityCheck (Opt::ARITY_CHECK, true, "arity_check"),
  _noFreeVars (Opt::NO_FREE_VARS, true, "no_free_vars"),
  _miniscope (Opt::MINISCOPE, true, "miniscope"),
  _outputEqualityInfix (Opt::OUTPUT_EQUALITY_INFIX, false, "output_equality_infix"),
  // Kernel's options
  _mainAlg (Opt::MAIN_ALG, OTTER, "man_alg"),
  _staticSplitting (Opt::STATIC_SPLITTING, false, "static_splitting"),
  _dynamicSplitting (Opt::DYNAMIC_SPLITTING, false, "dynamic_splitting"),
  _splittingStyle (Opt::SPLITTING_STYLE, PARALLEL, "splitting_style"),
  _splittingWithNaming (Opt::SPLITTING_WITH_NAMING, true, "splitting_with_naming"),
  _selection (Opt::SELECTION, P_4, "selection"),
  _literalComparisonMode (Opt::LITERAL_COMPARISON_MODE, NORMAL, "literal_comparison_mode"),
  _inheritedNegativeSelection (Opt::INHERITED_NEGATIVE_SELECTION, false, "inherited_negative_selection"),
  _fsSetMode (Opt::FS_SET_MODE, false, "fs_set_mode"),
  _fsOptimisedMultisetMode (Opt::FS_OPTIMISED_MULTISET_MODE, false, "fs_optimised_multiset_mode"),
  _simplifyByFs (Opt::SIMPLIFY_BY_FS, SIMP, "simplify_by_fs"),
  _forwardDemodulation (Opt::FORWARD_DEMODULATION, ALL, "forward_demodulation"),
  _fdOnSplittingBranches (Opt::FD_ON_SPLITTING_BRANCHES, true, "fd_on_splitting_branches"),
  _backwardDemodulation (Opt::BACKWARD_DEMODULATION, ALL, "backward_demodulation"),
  _bdOnSplittingBranches (Opt::BD_ON_SPLITTING_BRANCHES, true, "bd_on_splitting_branches"),
  _backwardSubsumption (Opt::BACKWARD_SUBSUMPTION, true, "backward_subsumption"),
  _bsSetMode (Opt::BS_SET_MODE, false, "bs_set_mode"),
  _bsOptimisedMultisetMode (Opt::BS_OPTIMISED_MULTISET_MODE, false, "bs_optimised_multiset_mode"),
  _orphanMurder (Opt::ORPHAN_MURDER, false, "orphan_murder"),
  _lrs (Opt::LRS, LRS_YES, "lrs"),
  _elimDef (Opt::ELIM_DEF, 0, "elim_def"),
  _symbolPrecedenceByArity (Opt::SYMBOL_PRECEDENCE_BY_ARITY, ARITY, "symbol_precedence_by_arity"),
  _headerPrecedenceKinky (Opt::HEADER_PRECEDENCE_KINKY, false, "header_precedence_kinky"),
  _timeLimit (Opt::TIME_LIMIT, LONG_MAX, "time_limit"),
  _memoryLimit (Opt::MEMORY_LIMIT, 300000, "memory_limit"),
  _maxSkolem (Opt::MAX_SKOLEM, 4096, "max_skolem"),
  _maxPassive (Opt::MAX_PASSIVE, LONG_MAX, "max_passive"),
  _maxWeight (Opt::MAX_WEIGHT, LONG_MAX, "max_weight"),
  _ageWeightRatio (Opt::AGE_WEIGHT_RATIO, 5, "age_weight_ratio"),
  _proof (Opt::PROOF, true, "proof"),
  _tab (Opt::TAB, 0, "tab"),
  _testId (Opt::TEST_ID, "unspecified_test", "tab"),
  _paramodulation (Opt::PARAMODULATION, true, "paramodulation"),
  _symbolWeightByArity (Opt::SYMBOL_WEIGHT_BY_ARITY, 0, "symbol_weight_by_arity"),
  _termWeightingScheme (Opt::TERM_WEIGHTING_SCHEME, TWS_UNIFORM, "term_weighting_scheme"),
  _simplificationOrdering (Opt::SIMPLIFICATION_ORDERING, KBO_NONREC, "simplification_ordering"),
  _nongoalPenaltyCoefficient (Opt::NONGOAL_PENALTY_COEFFICIENT, 1.0, "nongoal_penalty_coefficient" ),
  _kifInitialDatabase (Opt::KIF_INITIAL_DATABASE, 0, "kif_initial_database"),
  _showGen (Opt::SHOW_GEN, false, "show_gen"),
  _showPassive (Opt::SHOW_PASSIVE, false, "show_passive"),
  _showActive (Opt::SHOW_ACTIVE, false, "show_active"),
  _showProfile (Opt::SHOW_PROFILE, false, "show_profile"),
  _showInput (Opt::SHOW_INPUT, false, "show_input"),
  _tptpOutput (Opt::TPTP_OUTPUT, false, "tptp_output")
{
} // Options::Options


bool Option<bool>::set (const char* val)
{
  _changed = true;

  if (! strcmp (val,"on")) {
    _value = true;
  }
  else if (! strcmp (val,"off")) {
    _value = false;
  }
  else { // wrong value
    return false;
  }

  return true;
} // Option<bool>::set


bool Option<const char*>::set (const char* val)
{
  _changed = true;
  _value = val;

  return true;
} // Option<const char*>::set


bool Option<int>::set (const char* val)
{
  _changed = true;

  char* endptr = 0;
  _value = (int)(strtol (val, &endptr, 10));

  if (*endptr) { // error returned by strtol
    return false;
  }

  return true;
} // Option<int>::set


bool Option<long>::set (const char* val)
{
  _changed = true;

  char* endptr = 0;
  _value = strtol (val, &endptr, 10);

  if (*endptr) { // error returned by strtol
    return false;
  }
  return true;
} // Option<long>::set


bool Option<float>::set (const char* val)
{
  _changed = true;

  char* endptr = 0;
  _value = (float)(strtod (val, &endptr));

  if (*endptr) { // error returned by strtol
    return false;
  }
  return true;
} // Option<float>::set


bool Option<Options::MainAlg>::set (const char* val)
{
  _changed = true;

  if (! strcmp (val,"0")) {
    _value = Options::OTTER;
  }
  else if (! strcmp (val,"3")) {
    _value = Options::DISCOUNT;
  }
  else { // wrong value
    return false;
  }

  return true;
} // Option<MainAlg>::set


bool Option<Options::Lrs>::set (const char* val)
{
  _changed = true;

  if (! strcmp (val,"0")) {
    _value = Options::LRS_NO;
  }
  else if (! strcmp (val,"3")) {
    _value = Options::LRS_YES;
  }
  else { // wrong value
    return false;
  }

  return true;
} // Option<Options::Lrs>::set


bool Option<Options::SymbolPrecedenceByArity>::set (const char* val)
{
  _changed = true;

  if (! strcmp (val,"0")) {
    _value = Options::OCCURRENCE;
  }
  else if (! strcmp (val,"1")) {
    _value = Options::ARITY;
  }
  else if (! strcmp (val,"-1")) {
    _value = Options::REVERSE_ARITY;
  }
  else { // wrong value
    return false;
  }

  return true;
} // Option<Options::SymbolPrecedenceByArity>::set


bool Option<Options::SimplifyByFS>::set (const char* val)
{
  _changed = true;

  if (! strcmp (val,"1")) {
    _value = Options::SIMP;
  }
  else if (! strcmp (val,"0")) {
    _value = Options::NO_SIMP;
  }
  else { // wrong value
    return false;
  }

  return true;
} // Option<Options::SimplifyByFS>::set


bool Option<Options::TermWeightingScheme>::set (const char* val)
{
  _changed = true;

  if (! strcmp (val,"uniform")) {
    _value = Options::TWS_UNIFORM;
  }
  else if (! strcmp (val,"nonuniform_constant")) {
    _value = Options::TWS_NONUNIFORM_CONSTANT;
  }
  else { // wrong value
    return false;
  }

  return true;
} // Option<Options::TermWeightingScheme>::set


bool Option<Options::Demodulation>::set (const char* val)
{
  _changed = true;

  if (! strcmp (val,"0")) {
    _value = Options::NODEM;
  }
  else if (! strcmp (val,"1")) {
    _value = Options::PREORDERED;
  }
  else if (! strcmp (val,"4")) {
    _value = Options::ALL;
  }
  else { // wrong value
    return false;
  }

  return true;
} // Option<Options::Demodulation>::set


bool Option<Options::LiteralComparisonMode>::set (const char* val)
{
  _changed = true;

  if (! strcmp (val,"0")) {
    _value = Options::NORMAL;
  }
  else if (! strcmp (val,"1")) {
    _value = Options::KINKY;
  }
  else if (! strcmp (val,"2")) {
    _value = Options::PREDICATE;
  }
  else { // wrong value
    return false;
  }

  return true;
} // Option<LiteralComparisonMode>::set

bool Option<Options::Selection>::set (const char* val)
{
  _changed = true;

  if (! strcmp (val,"1")) {
    _value = Options::P_1;
  }
  else if (! strcmp (val,"2")) {
    _value = Options::P_2;
  }
  else if (! strcmp (val,"3")) {
    _value = Options::P_3;
  }
  else if (! strcmp (val,"4")) {
    _value = Options::P_4;
  }
  else if (! strcmp (val,"5")) {
    _value = Options::P_5;
  }
  else if (! strcmp (val,"6")) {
    _value = Options::P_6;
  }
  else if (! strcmp (val,"7")) {
    _value = Options::P_7;
  }
  else if (! strcmp (val,"-2")) {
    _value = Options::N_2;
  }
  else if (! strcmp (val,"-3")) {
    _value = Options::N_3;
  }
  else if (! strcmp (val,"-4")) {
    _value = Options::N_4;
  }
  else if (! strcmp (val,"-5")) {
    _value = Options::N_5;
  }
  else if (! strcmp (val,"-6")) {
    _value = Options::N_6;
  }
  else if (! strcmp (val,"-7")) {
    _value = Options::N_7;
  }
  else if (! strcmp (val,"1002")) {
    _value = Options::P_1002;
  }
  else if (! strcmp (val,"1003")) {
    _value = Options::P_1003;
  }
  else if (! strcmp (val,"1004")) {
    _value = Options::P_1004;
  }
  else if (! strcmp (val,"1005")) {
    _value = Options::P_1005;
  }
  else if (! strcmp (val,"1006")) {
    _value = Options::P_1006;
  }
  else if (! strcmp (val,"1007")) {
    _value = Options::P_1007;
  }
  else if (! strcmp (val,"1008")) {
    _value = Options::P_1008;
  }
  else if (! strcmp (val,"1009")) {
    _value = Options::P_1009;
  }
  else if (! strcmp (val,"-1005")) {
    _value = Options::N_1005;
  }
  else if (! strcmp (val,"-1009")) {
    _value = Options::N_1009;
  }
  else { // wrong value
    return false;
  }

  return true;
} // Option<Selection>::set


bool Option<Options::SplittingStyle>::set (const char* val)
{
  _changed = true;

  if (! strcmp (val,"2")) {
    _value = Options::BLOCKING;
  }
  else if (! strcmp (val,"1")) {
    _value = Options::PARALLEL;
  }
  else { // wrong value
    return false;
  }

  return true;
} // Option<SplittingStyle>::set


bool Option<Options::SimplificationOrdering>::set (const char* val)
{
  _changed = true;

  if (! strcmp (val,"standard_KB")) {
    _value = Options::KBO;
  }
  else if (! strcmp (val,"nonrecursive_KB")) {
    _value = Options::KBO_NONREC;
  }
  else { // wrong value
    return false;
  }

  return true;
} // Option<SimplificationOrdering>::set


Opt* Options::find (const char* name) // find option by name
{
  if (*name != '-') {
    return 0;
  }
  name++;
  if (*name != '-') { // short name, like -t
    // now name is whatever follows '-'
    switch (*name) {
      case 'T': 
        if (! strcmp(name,"T") ) {
          return &_testId;
        }
        return 0;

      case 'm': 
        if (! strcmp(name,"m") ) {
          return &_memoryLimit;
        }
        return 0;

      case 'p': 
        if (! strcmp(name,"p") ) {
          return &_proof;
        }
        return 0;

      case 't': 
        if (! strcmp(name,"t") ) {
          return &_timeLimit;
        }
        return 0;

      case 'w': 
        if (! strcmp(name,"w") ) {
          return &_maxWeight;
        }
        return 0;

    default:
      // no short names so far
      return 0;
    }
  }

  name ++;
  // now name is whatever follows '--'

  switch (*name) {
    case 'a': 
      if (! strcmp(name,"age_weight_ratio") ) {
        return &_ageWeightRatio;
      }
      if (! strcmp(name,"arity_check") ) {
        return &_arityCheck;
      }
      return 0;

    case 'b': 
      if (! strcmp(name,"backward_demodulation") ) {
        return &_backwardDemodulation;
      }
      if (! strcmp(name,"backward_subsumption") ) {
        return &_backwardSubsumption;
      }
      if (! strcmp(name,"bd_on_splitting_branches") ) {
        return &_bdOnSplittingBranches;
      }
      if (! strcmp(name,"bs_optimised_multiset_mode") ) {
        return &_bsOptimisedMultisetMode;
      }
      if (! strcmp(name,"bs_set_mode") ) {
        return &_bsSetMode;
      }
      return 0;

    case 'c': 
      if (! strcmp(name,"casc") ) {
        return &_casc;
      }
      return 0;

    case 'd': 
      if (! strcmp(name,"dynamic_splitting") ) {
        return &_dynamicSplitting;
      }
      return 0;

    case 'e': 
      if (! strcmp(name,"elim_def") ) {
        return &_elimDef;
      }
      return 0;

    case 'f': 
      if (! strcmp(name,"fd_on_splitting_branches") ) {
        return &_fdOnSplittingBranches;
      }
      if (! strcmp(name,"forward_demodulation") ) {
        return &_forwardDemodulation;
      }
      if (! strcmp(name,"fs_optimised_multiset_mode") ) {
        return &_fsOptimisedMultisetMode;
      }
      if (! strcmp(name,"fs_set_mode") ) {
        return &_fsSetMode;
      }
      return 0;

    case 'h': 
      if (! strcmp(name,"header_precedence_kinky") ) {
        return &_headerPrecedenceKinky;
      }
      return 0;

    case 'i': 
      if (! strcmp(name,"inherited_negative_selection") ) {
        return &_inheritedNegativeSelection;
      }
      return 0;

    case 'k': 
      if (! strcmp(name,"kif_initial_database") ) {
        return &_kifInitialDatabase;
      }
      return 0;

    case 'l': 
      if (! strcmp(name,"literal_comparison_mode") ) {
        return &_literalComparisonMode;
      }
      if (! strcmp(name,"lrs") ) {
        return &_lrs;
      }
      return 0;

    case 'm':
      if (! strcmp(name,"main_alg") ) {
        return &_mainAlg;
      }
      if (! strcmp(name,"max_passive") ) {
        return &_maxPassive;
      }
      if (! strcmp(name,"max_skolem") ) {
        return &_maxSkolem;
      }
      if (! strcmp(name,"max_weight") ) {
        return &_maxWeight;
      }
      if (! strcmp(name,"memory_limit") ) {
        return &_memoryLimit;
      }
      if (! strcmp(name,"miniscope") ) {
        return &_miniscope;
      }
      return 0;

    case 'n':
      if (! strcmp(name,"nongoal_penalty_coefficient") ) {
        return &_nongoalPenaltyCoefficient;
      }
      if (! strcmp(name,"no_free_vars") ) {
        return &_noFreeVars;
      }
      return 0;

    case 'o':
      if (! strcmp(name,"output_equality_infix") ) {
        return &_outputEqualityInfix;
      }
      if (! strcmp(name,"orphan_murder") ) {
        return &_orphanMurder;
      }
      return 0;

    case 'p':
      if (! strcmp(name,"proof") ) {
        return &_proof;
      }
      if (! strcmp(name,"paramodulation") ) {
        return &_paramodulation;
      }
      return 0;

    case 'r':
      if (! strcmp(name,"remove_unused_defs") ) {
        return &_removeUnusedDefs;
      }
      return 0;

    case 's':
      if (! strcmp(name,"static_splitting") ) {
        return &_staticSplitting;
      }
      if (! strcmp(name,"splitting_with_naming") ) {
        return &_splittingWithNaming;
      }
      if (! strcmp(name,"splitting_style") ) {
        return &_splittingStyle;
      }
      if (! strcmp(name,"selection") ) {
        return &_selection;
      }
      if (! strcmp(name,"show_gen") ) {
        return &_showGen;
      }
      if (! strcmp(name,"show_passive") ) {
        return &_showPassive;
      }
      if (! strcmp(name,"show_active") ) {
        return &_showActive;
      }
      if (! strcmp(name,"show_profile") ) {
        return &_showProfile;
      }
      if (! strcmp(name,"show_input") ) {
        return &_showInput;
      }
      if (! strcmp(name,"simplification_ordering") ) {
        return &_simplificationOrdering;
      }
      if (! strcmp(name,"simplify_by_fs") ) {
        return &_simplifyByFs;
      }
      if (! strcmp(name,"symbol_precedence_by_arity") ) {
        return &_symbolPrecedenceByArity;
      }
      if (! strcmp(name,"symbol_weight_by_arity") ) {
        return &_symbolWeightByArity;
      }
      return 0;

    case 't':
      if (! strcmp(name,"tab") ) {
        return &_tab;
      }
      if (! strcmp(name,"term_weighting_scheme") ) {
        return &_termWeightingScheme;
      }
      if (! strcmp(name,"test_id") ) {
        return &_testId;
      }
      if (! strcmp(name,"time_limit") ) {
        return &_timeLimit;
      }
      if (! strcmp(name,"tptp_output") ) {
        return &_tptpOutput;
      }
      return 0;

    default:
      return 0;
  }
} // Options::find


Options* Options::lrs (Selection sel)
{
  Options* options = new Options;
  options->mainAlg (OTTER);
  options->lrs (LRS_YES);
  options->selection (sel);

  return options;
} // Options::lrs (Selection selection)


Options* Options::lrs (Selection sel, LiteralComparisonMode lcm)
{
  Options* options = lrs (sel);
  options->literalComparisonMode (lcm);

  if (lcm == KINKY) {
    options->headerPrecedenceKinky (true);
  }

  return options;
} // Options::lrs (Selection selection)


Options* Options::discount (Selection sel)
{
  Options* options = new Options;
  options->mainAlg (DISCOUNT);
  options->lrs (LRS_NO);
  options->selection (sel);

  return options;
} // Options::discount (Selection selection)


Options* Options::discount (Selection sel, LiteralComparisonMode lcm)
{
  Options* options = discount (sel);
  options->literalComparisonMode (lcm);

  if (lcm == KINKY) {
    options->headerPrecedenceKinky (true);
  }

  return options;
} // Options::discount (Selection selection)


Options* Options::otter (Selection sel)
{
  Options* options = new Options;
  options->mainAlg (OTTER);
  options->lrs (LRS_NO);
  options->selection (sel);

  return options;
} // Options::otter (Selection selection)


void Options::fullSplitting ()
{
  staticSplitting (true);
  dynamicSplitting (true);
  splittingStyle (PARALLEL);
  splittingWithNaming (true);
  fdOnSplittingBranches (true);
  bdOnSplittingBranches (true);
} // Options::fullSplitting

