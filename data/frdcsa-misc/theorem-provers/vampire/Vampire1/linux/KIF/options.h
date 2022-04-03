//
//  file options.h
//  defines Vampire options
//

#ifndef __options__
#define __options__


class ostream;

class Opt {
 public:
  enum Tag {
   REMOVE_UNUSED_DEF,
   ARITY_CHECK,
   NO_FREE_VARS,
   MINISCOPE,
   OUTPUT_EQUALITY_INFIX,
   CASC,

   // Kernel options
   MAIN_ALG,
   STATIC_SPLITTING,
   DYNAMIC_SPLITTING,
   SPLITTING_STYLE,
   SPLITTING_WITH_NAMING,
   SELECTION,
   LITERAL_COMPARISON_MODE,
   INHERITED_NEGATIVE_SELECTION,
   FS_SET_MODE,
   FS_OPTIMISED_MULTISET_MODE,
   SIMPLIFY_BY_FS,
   FORWARD_DEMODULATION,
   FD_ON_SPLITTING_BRANCHES,
   BACKWARD_DEMODULATION,
   BD_ON_SPLITTING_BRANCHES,
   BACKWARD_SUBSUMPTION,
   BS_SET_MODE,
   BS_OPTIMISED_MULTISET_MODE,
   ORPHAN_MURDER,
   LRS,
   ELIM_DEF,
   SYMBOL_PRECEDENCE_BY_ARITY,
   HEADER_PRECEDENCE_KINKY,
   TIME_LIMIT,
   MEMORY_LIMIT,
   MAX_SKOLEM,
   MAX_PASSIVE,
   MAX_WEIGHT,
   AGE_WEIGHT_RATIO,
   PROOF,
   TAB,
   TEST_ID,
   PARAMODULATION,
   SYMBOL_WEIGHT_BY_ARITY,
   TERM_WEIGHTING_SCHEME,
   SIMPLIFICATION_ORDERING,
   NONGOAL_PENALTY_COEFFICIENT,
   KIF_INITIAL_DATABASE,
   SHOW_GEN,
   SHOW_PASSIVE,
   SHOW_ACTIVE,
   SHOW_PROFILE,
   SHOW_INPUT,
   TPTP_OUTPUT
  };

  Opt (Tag t,const char* nm,const char* srt) 
    : _tag (t), 
      _name (nm),
      _short (srt),
      _changed (false)
      {}
 virtual bool set (const char* val) = 0;
 const char* name () const { return _name; }

 protected:
  const Tag _tag;
  const char* _name;
  const char* _short;
  bool _changed;
}; // class Opt


template <class C>
class Option :
  public Opt {
 public:
  // constructor
  Option (Opt::Tag t, const C dfl, const char* name, const char* srt = 0 )
    : Opt (t, name, srt),
      _value (dfl),
      _default (dfl)
      {}
  virtual bool set (const char* val);
  C value () const { return _value; }
  void value (C newVal) { _value = newVal; _changed = true; }

 private:
  C _value;
  const C _default;
}; // class Option


class Options {
 public:
  // possible values for various options
  enum MainAlg {
    OTTER = 0,
    DISCOUNT = 3
  };
  enum SplittingStyle {
    BLOCKING = 2,
    PARALLEL = 1
  };
  enum Selection {
    P_1 = 1,
    P_2 = 2,
    P_3 = 3,
    P_4 = 4,
    P_5 = 5,
    P_6 = 6,
    P_7 = 7,
    P_1002 = 1002,
    P_1003 = 1003,
    P_1004 = 1004,
    P_1005 = 1005,
    P_1006 = 1006,
    P_1007 = 1007,
    P_1008 = 1008,
    P_1009 = 1009,
    N_2 = -2,
    N_3 = -3,
    N_4 = -4,
    N_5 = -5,
    N_6 = -6,
    N_7 = -7,
    N_1005 = -1005,
    N_1009 = -1009
  };
  enum LiteralComparisonMode {
    NORMAL = 0,
    KINKY = 1,
    PREDICATE = 2
  };
  enum SimplifyByFS {
    SIMP = 1,
    NO_SIMP = 0
  };
  enum Demodulation {
    NODEM = 0,
    PREORDERED = 1,
    ALL = 4
  };
  enum Lrs {
    LRS_NO = 0,
    LRS_YES = 3
  };
  enum SymbolPrecedenceByArity {
    OCCURRENCE = 0,
    ARITY = 1,
    REVERSE_ARITY = -1
  };
  enum SimplificationOrdering {
    KBO,
    KBO_NONREC
  };
  enum TermWeightingScheme {
    TWS_UNIFORM,
    TWS_NONUNIFORM_CONSTANT
  };

 public:
  Options ();
  Options (int argc, char* argv []);

  // get various options
  const char* file () const { return _file; }
  bool casc () const { return _casc.value (); }

  // preprocessor options
  bool removeUnusedDefs () const { return _removeUnusedDefs.value (); }
  bool arityCheck () const { return _arityCheck.value (); }
  bool noFreeVars () const { return _noFreeVars.value (); }
  bool miniscope () const { return _miniscope.value (); }
  bool outputEqualityInfix () const { return _outputEqualityInfix.value (); }

  // kernel options
  MainAlg mainAlg () const { return _mainAlg.value(); }
  void mainAlg (MainAlg alg) { _mainAlg.value(alg); }

  bool staticSplitting () const { return _staticSplitting.value(); }
  void staticSplitting (bool newVal) { _staticSplitting.value(newVal); }

  bool dynamicSplitting () const { return _dynamicSplitting.value(); }
  void dynamicSplitting (bool newVal) { _dynamicSplitting.value(newVal); }

  SplittingStyle splittingStyle () const { return _splittingStyle.value(); }
  void splittingStyle (SplittingStyle newVal) { _splittingStyle.value(newVal); }

  bool splittingWithNaming () const { return _splittingWithNaming.value(); }
  void splittingWithNaming (bool newVal) { _splittingWithNaming.value(newVal); }

  Selection selection () const { return _selection.value(); }
  void selection (Selection s) { _selection.value (s); }

  LiteralComparisonMode literalComparisonMode () const { return _literalComparisonMode.value(); }
  void literalComparisonMode (LiteralComparisonMode lcm) { _literalComparisonMode.value(lcm); }

  bool inheritedNegativeSelection () const { return _inheritedNegativeSelection.value(); }
  bool fsSetMode () const { return _fsSetMode.value(); }
  bool fsOptimisedMultisetMode () const { return _fsOptimisedMultisetMode.value(); }
  SimplifyByFS simplifyByFs () const { return _simplifyByFs.value(); }

  Demodulation forwardDemodulation () const { return _forwardDemodulation.value(); }
  void forwardDemodulation (Demodulation newVal) { _forwardDemodulation.value(newVal); }

  bool fdOnSplittingBranches () const { return _fdOnSplittingBranches.value(); }
  void fdOnSplittingBranches (bool newVal) { _fdOnSplittingBranches.value(newVal); }

  Demodulation backwardDemodulation () const { return _backwardDemodulation.value(); }
  void backwardDemodulation (Demodulation newVal) { _backwardDemodulation.value(newVal); }

  bool bdOnSplittingBranches () const { return _bdOnSplittingBranches.value(); }
  void bdOnSplittingBranches (bool newVal) { _bdOnSplittingBranches.value(newVal); }

  bool backwardSubsumption () const { return _backwardSubsumption.value(); }
  void backwardSubsumption (bool newVal) { _backwardSubsumption.value(newVal); }

  bool bsSetMode () const { return _bsSetMode.value(); }
  bool bsOptimisedMultisetMode () const { return _bsOptimisedMultisetMode.value(); }
  bool orphanMurder () const { return _orphanMurder.value(); }

  Lrs lrs () const { return _lrs.value(); }
  void lrs (Lrs val) { _lrs.value(val); }

  int elimDef () const { return _elimDef.value(); }
  void elimDef (int newVal) { _elimDef.value(newVal); }

  SymbolPrecedenceByArity symbolPrecedenceByArity () const { return _symbolPrecedenceByArity.value(); }
  void symbolPrecedenceByArity (SymbolPrecedenceByArity newVal) { _symbolPrecedenceByArity.value(newVal); }

  bool headerPrecedenceKinky () const { return _headerPrecedenceKinky.value(); }
  void headerPrecedenceKinky (bool newVal) { _headerPrecedenceKinky.value(newVal); }

  long timeLimit () const { return _timeLimit.value(); }
  void timeLimit ( long newVal ) { _timeLimit.value (newVal); }

  int memoryLimit () const { return _memoryLimit.value(); }
  int maxSkolem () const { return _maxSkolem.value(); }
  long maxPassive () const { return _maxPassive.value(); }

  long maxWeight () const { return _maxWeight.value(); }
  void maxWeight (long newVal) { _maxWeight.value(newVal); }

  int ageWeightRatio () const { return _ageWeightRatio.value(); }
  void ageWeightRatio (int newVal) { _ageWeightRatio.value(newVal); }

  bool proof () const { return _proof.value(); }
  const char* tab () const { return _tab.value(); }
  const char* testId () const { return _testId.value(); }
  bool paramodulation () const { return _paramodulation.value(); }

  int symbolWeightByArity () const { return _symbolWeightByArity.value(); }
  void symbolWeightByArity (int newVal) { _symbolWeightByArity.value(newVal); }

  TermWeightingScheme termWeightingScheme () const { return _termWeightingScheme.value (); }
  void termWeightingScheme (TermWeightingScheme newVal) { _termWeightingScheme.value (newVal); }

  SimplificationOrdering simplificationOrdering () const { return _simplificationOrdering.value(); }
  void simplificationOrdering (SimplificationOrdering newVal) { _simplificationOrdering.value(newVal); }

  float nongoalPenaltyCoefficient () const { return _nongoalPenaltyCoefficient.value(); }
  void nongoalPenaltyCoefficient (float newVal) { _nongoalPenaltyCoefficient.value(newVal); }

  const char* kifInitialDatabase () { return _kifInitialDatabase.value(); }
  void kifInitialDatabase (const char* db) { _kifInitialDatabase.value (db); }

  bool showGen () const { return _showGen.value(); }
  void showGen (bool newVal) { _showGen.value(newVal); }

  bool showPassive () const { return _showPassive.value(); }
  void showPassive (bool newVal) { _showPassive.value(newVal); }

  bool showActive () const { return _showActive.value(); }
  void showActive (bool newVal) { _showActive.value(newVal); }

  bool showProfile () const { return _showProfile.value(); }
  void showProfile (bool newVal) { _showProfile.value(newVal); }

  bool showInput () const { return _showInput.value(); }
  void showInput (bool newVal) { _showInput.value(newVal); }

  bool tptpOutput () const { return _tptpOutput.value(); }
  void tptpOutput (bool newVal) { _tptpOutput.value(newVal); }

  // standard ways of creating options
  static Options* lrs (Selection selection);
  static Options* lrs (Selection selection, LiteralComparisonMode);

  static Options* discount (Selection selection);
  static Options* discount (Selection selection, LiteralComparisonMode);

  static Options* otter (Selection selection);
  void fullSplitting ();

 private:
  // general options

  const char* _file;  // file name of the input file
  Option <bool> _casc;

  // preprocessor options

  Option <bool> _removeUnusedDefs;
  Option <bool> _arityCheck;
  Option <bool> _noFreeVars;
  Option <bool> _miniscope;
  Option <bool> _outputEqualityInfix;

  // Kernel's options

  Option <MainAlg> _mainAlg;
  Option <bool> _staticSplitting;
  Option <bool> _dynamicSplitting;
  Option <SplittingStyle> _splittingStyle;
  Option <bool> _splittingWithNaming;
  Option <Selection> _selection;
  Option <LiteralComparisonMode> _literalComparisonMode;
  Option <bool> _inheritedNegativeSelection;
  Option <bool> _fsSetMode;
  Option <bool> _fsOptimisedMultisetMode;
  Option <SimplifyByFS> _simplifyByFs;
  Option <Demodulation> _forwardDemodulation;
  Option<bool> _fdOnSplittingBranches;
  Option <Demodulation> _backwardDemodulation;
  Option<bool> _bdOnSplittingBranches;
  Option <bool> _backwardSubsumption;
  Option <bool> _bsSetMode;
  Option <bool> _bsOptimisedMultisetMode;
  Option <bool> _orphanMurder;
  Option <Lrs> _lrs;
  Option <int> _elimDef;
  Option <SymbolPrecedenceByArity> _symbolPrecedenceByArity;
  Option <bool> _headerPrecedenceKinky;
  Option <long> _timeLimit;
  Option <int> _memoryLimit;
  Option <int> _maxSkolem;
  Option <long> _maxPassive;
  Option <long> _maxWeight;
  Option <int> _ageWeightRatio;
  Option <bool> _proof;
  Option <const char*> _tab;
  Option <const char*> _testId;
  Option <bool> _paramodulation;
  Option <int> _symbolWeightByArity;
  Option <TermWeightingScheme> _termWeightingScheme;
  Option <SimplificationOrdering> _simplificationOrdering;
  Option <float> _nongoalPenaltyCoefficient;
  Option <const char*> _kifInitialDatabase;
  Option <bool> _showGen;  
  Option <bool> _showPassive;  
  Option <bool> _showActive;  
  Option <bool> _showProfile;  
  Option <bool> _showInput;  
  Option <bool> _tptpOutput;  

  Opt* find (const char*); // find option by name
  static const char* _names [];
}; // class Options


// a kind of kludge. Needed since some options are required for e.g. 
// output
extern Options* options;
 

#endif

