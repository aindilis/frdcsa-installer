//
// File:         ClauseAssembler.h
// Description:  Implements clause sharing.
// Created:      Dec 13, 2001 
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
// Note:         Was CLAUSE_INDEX in clause_index.h.
// Note:         Integrate and Remove methods need a revision,
//               they became very strange after several modifications.
//============================================================================
#ifndef CLAUSE_ASSEMBLER_H
//=============================================================================
#define CLAUSE_ASSEMBLER_H 
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "LiteralSharing.h"
#include "variables.h"
namespace VK
{
  using namespace BK;

class SHARING_INDEX;
class DTREE_NODE;
class LiteralSelection;
class TMP_LITERAL;
class TmpLitList;
class Clause;
class OpenClauseBackground;
class PagedPassiveClauseStorage;
class SymbolInfo;
}; // namespace VK

//============================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_CLAUSE_ASSEMBLER
#define DEBUG_NAMESPACE "ClauseAssembler"
#endif
#include "debugMacros.h"
//============================================================================

namespace VK
{
  using namespace BK;
class ClauseAssembler
{
 public:
  ClauseAssembler(SHARING_INDEX* si,const LiteralSelection* litSelection) 
    : _sharingIndex(si), 
    _litSharing(si), 
    _litSelection(litSelection),
    _inheritNegativeSelection(false),
    _equalityIsPossible(true),
    _eliminatedPredicateTable((const SymbolInfo*)0),
    _nongoalPenaltyCoefficient(1.0),
    _useNongoalPenaltyCoefficient(false)
    {
    };
  ~ClauseAssembler() { CALL("destructor ~ClauseAssembler()"); };
  void setInheritNegativeSelection(bool flag) { _inheritNegativeSelection = flag; };
  void setEqualityIsPossibleFlag(bool flag) 
    {
      _equalityIsPossible = flag;
    };
  void setEliminatedPredicateTable(const SymbolInfo* tbl)
    {
      _eliminatedPredicateTable = tbl;
    };

  void setNongoalPenaltyCoefficient(float coeff)
    {
      _nongoalPenaltyCoefficient = coeff;
      if (_nongoalPenaltyCoefficient < 1.0) _nongoalPenaltyCoefficient = 1.0;
      _useNongoalPenaltyCoefficient = (_nongoalPenaltyCoefficient != 1.0);
    };

  Clause* assembleEmptyClause(OpenClauseBackground& ancestors) const;
  Clause* assembleUnitClause(TMP_LITERAL* lit,OpenClauseBackground& ancestors);
  Clause* assembleClauseWithEvthngSlctd(TmpLitList& lits,OpenClauseBackground& ancestors);
   
  Clause* assembleClause(TmpLitList& lits,OpenClauseBackground& ancestors);
 
  Clause* assembleUnsharedClause(TmpLitList& lits,OpenClauseBackground& ancestors);
 
  bool assembleClauseInPagedPassiveClauseStorage(PagedPassiveClauseStorage& storage,TmpLitList& lits,OpenClauseBackground& ancestors,long clauseNum,bool showNewClause,bool markAsName);
   

  //   void Integrate(Clause* cl);
  void removeFromSharing(Clause* cl);
   
  //void Destroy() { _litSharing.Destroy(); };
  void destroy() { _litSharing.Destroy(); }; 


#ifndef NO_DEBUG
  bool clean() const { return _litSharing.clean(); };
#endif


 private:
   
  DTREE_NODE** Tree(const TERM& header) { return _litSharing.Tree(header); };
 

  TERM* shareTmpLiteral(const TMP_LITERAL* lit,Clause* cl);

 
  TERM* assembleUnsharedLit(const TMP_LITERAL& lit);

  void writeIntoPagedPassiveClauseStorage(PagedPassiveClauseStorage& storage,TmpLitList& lits);  

  void writeIntoPagedPassiveClauseStorage(PagedPassiveClauseStorage& storage,TMP_LITERAL* lit);
 
  
  TERM* Integrate(const Flatterm* lit,Clause* cl);
  TERM* IntegrateEq(const TERM& header,const Flatterm* arg1,const Flatterm* arg2,Clause* cl);
  void removeFromSharing(TERM* lit);

 private:
  SHARING_INDEX* _sharingIndex;
  LiteralSharing _litSharing;
  INT_VAR_RENAMING _intVarRenaming;
  VAR_WEIGHTING _varWeighting;
  TERM::FastOpenUnshared _openUnsharedTerm;
  const LiteralSelection* _litSelection;
  bool _inheritNegativeSelection;
  bool _equalityIsPossible;
  const SymbolInfo* _eliminatedPredicateTable;
  float _nongoalPenaltyCoefficient;
  bool _useNongoalPenaltyCoefficient;
}; // class ClauseAssembler

}; // namespace VK

//============================================================================
#endif
