// Revised:   Apr 23, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//            NewClause::NormalizeByCommutativity(TMP_LITERAL* lit)
//            reimplemented.
// Revised:   May 26, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//            Memory leak fixed (recycle() was not called on some TMP_LITERALs).
//====================================================================
#include "NewClause.h"
#include "tmp_literal.h"
#include "ForwardSubsumptionFrontEnd.h"
#include "ClauseAssembler.h"
#include "SimplificationOrdering.h"
#include "BuiltInTheoryDispatcher.h"
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_NEW_CLAUSE
#define DEBUG_NAMESPACE "NewClause"
#endif 
#include "debugMacros.h"
//=================================================

namespace VK
{
  using namespace BK;
  using namespace Gematogen;

bool NewClause::transferToPagedPassive(PagedPassiveClauseStorage& storage,bool showNewClause)
{
  CALL("NewClause::transferToPagedPassive(PagedPassiveClauseStorage& storage,bool showNewClause)");
  _openClause.EndOfClause();
  ASSERT(!_openClause.AllLiterals().empty());

  return _clauseAssembler->assembleClauseInPagedPassiveClauseStorage(storage,
								     _openClause.AllLiterals(),
								     _ancestors,
								     _clauseNumber,
								     showNewClause,
								     false);
}; // bool NewClause::transferToPagedPassive(PagedPassiveClauseStorage& storage,bool showNewClause)

Clause* NewClause::Reanimate()
{
  CALL("Reanimate()");
  _openClause.EndOfClause();

  ASSERT(!_openClause.AllLiterals().empty());
  
  if (_forwardSubsumptionFlag) 
    if (FSubsume()) 
      {
	AnotherForwardSubsumed();
	return (Clause*)0;
      };
  
  // simplify the new clause here
  
  bool redundant;
  Simplify(redundant);
  if (redundant) return (Clause*)0;
  if (_openClause.AllLiterals().empty()) return _clauseAssembler->assembleEmptyClause(_ancestors);

  if (_openClause.AllLiterals().numOfAllLiterals() == 1) 
    {
      return _clauseAssembler->assembleUnitClause(_openClause.AllLiterals().first(),_ancestors);
    };

  return _clauseAssembler->assembleClause(_openClause.AllLiterals(),_ancestors);
}; // Clause* NewClause::Reanimate()


void NewClause::ProcessCollected3(Clause*& answerClause,
                                  PagedPassiveClauseStorage& storage,
                                  ulong& numOfNewClauses,
                                  ulong& numOfRejectedNewClauses,
				  Clause::NewNumber* clauseNumberGenerator,
                                  bool showNewClauses)
{  
  CALL("ProcessCollected3(..)");
  _openClause.EndOfClause();
  
  answerClause = (Clause*)0;
  numOfNewClauses = 0;
  numOfRejectedNewClauses = 0;

  if (_ancestors.inferenceDepth() > _maxInferenceDepth)
    {
      anotherClauseWithTooDeepInference();
      return;
    };

  if (_openClause.AllLiterals().empty()) 
    {
      answerClause = _clauseAssembler->assembleEmptyClause(_ancestors);

      answerClause->SetNumber(clauseNumberGenerator->generate());
      answerClause->setMainSet(Clause::MainSetFree);
      numOfNewClauses++;
      if (showNewClauses) cout << "% " << answerClause << "\n";
      return; 
    };
  
  if (_forwardSubsumptionFlag) 
    if (FSubsume()) 
      {
	AnotherForwardSubsumed();
	return;
      };
  
  // simplify the new clause here
  
  bool redundant;
  Simplify(redundant);
  if (redundant) return;


  if (_ancestors.inferenceDepth() > _maxInferenceDepth)
    {
      anotherClauseWithTooDeepInference();
      return;
    };

  if (_openClause.AllLiterals().containsOnlyAnswerLiterals()) 
    {    
      answerClause = _clauseAssembler->assembleClause(_openClause.AllLiterals(),_ancestors);

      answerClause->SetNumber(clauseNumberGenerator->generate());
      answerClause->setMainSet(Clause::MainSetFree);
      numOfNewClauses++;
      if (showNewClauses) cout << "% " << answerClause << "\n";
      return; 
    };



  if (_ancestors.singletone() && (*(_ancestors.begin()))->IsSplitName())
    {
      // inherited property
      UsedRule(ClauseBackground::Split);
      UsedRule(ClauseBackground::Name);
    } 
  else
    if (_splittingFlag && (_splitting.Split3(_openClause.AllLiterals(),
					     answerClause,
					     storage,
					     numOfNewClauses,
					     numOfRejectedNewClauses,
					     clauseNumberGenerator,
					     showNewClauses)))
      { 
	return;
      };  


  if (_clauseAssembler->assembleClauseInPagedPassiveClauseStorage(storage,_openClause.AllLiterals(),_ancestors,clauseNumberGenerator->generate(),showNewClauses,false))
    {
      numOfNewClauses++;                  
    }
  else
    {
      numOfRejectedNewClauses++;
    };
}; // void NewClause::ProcessCollected3(Clause*& answerClause,...




void NewClause::ProcessCollected2(ClauseQueue& newClauses) 
{  
  CALL("ProcessCollected2()");
  // Now only for input clauses in --main_alg 3
  
  _openClause.EndOfClause();


  if (_openClause.AllLiterals().empty()) 
    {
      Clause* newClause = _clauseAssembler->assembleEmptyClause(_ancestors);
      newClause->setMainSet(Clause::MainSetNew);
      newClauses.enqueue(newClause); 
      return; 
    };
  
  if (_forwardSubsumptionFlag) 
    if (FSubsume()) 
      {
	AnotherForwardSubsumed();
	return;
      };
  
  // simplify the new clause here
  
  bool redundant;
  Simplify(redundant);
  if (redundant) return;
  if (_openClause.AllLiterals().empty()) 
    {
      Clause* newClause = _clauseAssembler->assembleEmptyClause(_ancestors);
      newClause->setMainSet(Clause::MainSetNew);
      newClauses.enqueue(newClause);  
      return;  
    };


  if (_ancestors.singletone() && (*(_ancestors.begin()))->IsSplitName())
    {
      // inherited property
      UsedRule(ClauseBackground::Split);
      UsedRule(ClauseBackground::Name);
    };

  Clause* cl = _clauseAssembler->assembleUnsharedClause(_openClause.AllLiterals(),_ancestors);
  cl->setMainSet(Clause::MainSetNew);
  newClauses.enqueue(cl);
  
}; // void NewClause::ProcessCollected2(ClauseQueue& newClauses)  



void NewClause::ProcessCollected(ClauseQueue& newClauses) 
{  
  CALL("ProcessCollected()");
  _openClause.EndOfClause();

  if (_ancestors.inferenceDepth() > _maxInferenceDepth)
    {
      anotherClauseWithTooDeepInference();
      return;
    };


  if (_openClause.AllLiterals().empty()) 
    {
      Clause* newClause = _clauseAssembler->assembleEmptyClause(_ancestors);
      newClause->setMainSet(Clause::MainSetNew);
      newClauses.enqueue(newClause);  
      return; 
    };
  
  if (_forwardSubsumptionFlag) 
    if (FSubsume()) 
      {
	AnotherForwardSubsumed();
	return;
      };
  
  // simplify the new clause here
  
  bool redundant;
  Simplify(redundant);
  if (redundant) return;
  if (_ancestors.inferenceDepth() > _maxInferenceDepth)
    {
      anotherClauseWithTooDeepInference();
      return;
    };
  if (_openClause.AllLiterals().empty()) 
    {
      Clause* newClause = _clauseAssembler->assembleEmptyClause(_ancestors);
      newClause->setMainSet(Clause::MainSetNew);
      newClauses.enqueue(newClause);
      return;  
    };
  
  if (_negEqSplitting) 
    { 
      _splitting.splitNegativeEqualities(_openClause.AllLiterals(),newClauses);
      if (!_openClause.Weed()) return;
      if (_forwardSubsumptionFlag && FSubsume())
	{ 
	  AnotherForwardSubsumed();
	  return;
	}; 
    };



  if (_openClause.AllLiterals().numOfAllLiterals() == 1)  
    {
      Clause* unit_cl = _clauseAssembler->assembleUnitClause(_openClause.AllLiterals().first(),_ancestors);
      unit_cl->setMainSet(Clause::MainSetNew);
      newClauses.enqueue(unit_cl); 
      return; 
    };
  
  if (_ancestors.singletone() && (*(_ancestors.begin()))->IsSplitName())
    {
      // inherited property
      UsedRule(ClauseBackground::Split);
      UsedRule(ClauseBackground::Name);
    } 
  else
    if (_splittingFlag && (_splitting.Split(_openClause.AllLiterals(),newClauses)))
      { 
	return;
      };  

  Clause* cl = _clauseAssembler->assembleClause(_openClause.AllLiterals(),_ancestors);
  cl->setMainSet(Clause::MainSetNew);
  newClauses.enqueue(cl);
  
}; // void NewClause::ProcessCollected(ClauseQueue& newClauses)  

void NewClause::Simplify(bool& redundant)
{
  CALL("Simplify(bool& redundant)");
  redundant = false;
  if ((_forwardSubsumptionResolution) && (SimplifyByFS()))
    {
      ASSERT_IN(_openClause.AllLiterals().checkIntegrity(),"SimplifyByFS_1");
      UsedRule(ClauseBackground::ForwSubsRes);
      AnotherSimplifiedByFSR();
      MakeVIP();
      if (_openClause.AllLiterals().empty()) return;
    };
  ASSERT_IN(_openClause.AllLiterals().checkIntegrity(),"SimplifyByFS_2");

 simplify:     
  if ((_commutativityTable) && (NormalizeByCommutativity()))
    { 

      ASSERT_IN(_openClause.AllLiterals().checkIntegrity(),"NormalizeByCommutativity_1");
      UsedRule(ClauseBackground::ForwDemod);
      AnotherSimplifiedByDemod();
      MakeVIP();
      if ((redundant = (!_openClause.Weed()))) return;

      if (_openClause.AllLiterals().empty()) return;
      if (_forwardSubsumptionFlag && ((redundant = FSubsume())))
	{ 
	  AnotherForwardSubsumed();
	  return;
	}; 
    };
  ASSERT_IN(_openClause.AllLiterals().checkIntegrity(),"NormalizeByCommutativity_2");

  if (_forwardDemodulation > 0)
    {
    demodulate:
      if (SimplifyByUEq())
	{ 
	  ASSERT_IN(_openClause.AllLiterals().checkIntegrity(),"SimplifyByUEq_1");


	  UsedRule(ClauseBackground::ForwDemod);
	  AnotherSimplifiedByDemod();
	  MakeVIP();

	  if ((redundant = (!_openClause.Weed()))) return;

	  if (_openClause.AllLiterals().empty()) return;
	  if (_forwardSubsumptionFlag && ((redundant = FSubsume())))
	    { 
	      AnotherForwardSubsumed();
	      return;
	    };
 
	  if ((_commutativityTable) && (NormalizeByCommutativity()))
	    { 
	      UsedRule(ClauseBackground::ForwDemod);
	      AnotherSimplifiedByDemod();
	      MakeVIP();
	      if ((redundant = (!_openClause.Weed()))) return;

	      if (_openClause.AllLiterals().empty()) return;
	      if (_forwardSubsumptionFlag && ((redundant = FSubsume())))
		{ 
		  AnotherForwardSubsumed();
		  return;
		};
	      goto demodulate;  
	    };
                       
	}; 
      ASSERT_IN(_openClause.AllLiterals().checkIntegrity(),"SimplifyByUEq_2");    
    }; 


  if (BuiltInTheoryDispatcher::current())
    {
      if (simplifyByBuiltInTheories())
	{
	  UsedRule(ClauseBackground::SimplificationByBuiltInTheories);
	  anotherSimplifiedByBuiltInTheories();
	  if ((redundant = (!_openClause.Weed()))) return;
	  if (_openClause.AllLiterals().empty()) return;
	  if (_forwardSubsumptionFlag && ((redundant = FSubsume())))
	    { 
	      AnotherForwardSubsumed();
	      return;
	    };       
	  goto simplify;
	};
    };

  if (_simplificationByEqRes)
    {
      if (SimplifyByEqRes())
	{

	  ASSERT_IN(_openClause.AllLiterals().checkIntegrity(),"SimplifyByEqRes_1"); 
	  UsedRule(ClauseBackground::EqResSimp);
	  AnotherSimplifiedByEqRes();
	  MakeVIP();
	  if ((redundant = (!_openClause.Weed()))) return;

	  if (_openClause.AllLiterals().empty()) return;
	  if (_forwardSubsumptionFlag && ((redundant = FSubsume())))
	    { 
	      AnotherForwardSubsumed();
	      return;
	    };       
	  goto simplify;
	};  
      ASSERT_IN(_openClause.AllLiterals().checkIntegrity(),"SimplifyByEqRes_2"); 
    }; 


  if ((_forwardSubsumptionResolution) && (SimplifyByFS()))
    {
      ASSERT_IN(_openClause.AllLiterals().checkIntegrity(),"SimplifyByFS_3"); 
      UsedRule(ClauseBackground::ForwSubsRes);
      AnotherSimplifiedByFSR();
      MakeVIP();
      if (_openClause.AllLiterals().empty()) return;
    };
  ASSERT_IN(_openClause.AllLiterals().checkIntegrity(),"SimplifyByFS_4"); 
}; // void NewClause::Simplify(bool& redundant)


inline ulong NewClause::SimplifyByFS()
{
  CALL("SimplifyByFS()");
  return _forwardSubsumption->simplifyBySubsumptionResolution(_openClause.AllLiterals(),_ancestors);
}; // ulong NewClause::SimplifyByFS()


bool NewClause::SimplifyByEqRes()
{
  CALL("SimplifyByEqRes()");

  ulong var;
  const Flatterm* subst_term;
  unsigned long subst_depth;
  long subst_weight;
  TMP_LITERAL* simplif_lit;   

  TMP_LITERAL* el = _openClause.AllLiterals().last();

 try_next_lit:  
   
  if (!el) return false;
  simplif_lit = el;
  if (simplif_lit->isNegative() && 
      simplif_lit->isEquality() && 
      simplif_lit->isUnorderedEquality())
    {
      if (simplif_lit->arg1()->Symbol().isVariable())
	{
	  var = simplif_lit->arg1()->Symbol().var();
	  if (simplif_lit->arg2ContainsVariable(var))
	    {
	      el = el->previous();
	      goto try_next_lit; 
	    };     
	  subst_term = simplif_lit->arg2();
	}
      else 
	if (simplif_lit->arg2()->Symbol().isVariable())
	  {
	    var = simplif_lit->arg2()->Symbol().var();
	    if (simplif_lit->arg1ContainsVariable(var))
	      {
		el = el->previous();
		goto try_next_lit;
	      };
	    subst_term = simplif_lit->arg1();
	  }
	else 
	  {
	    el = el->previous();
	    goto try_next_lit;
	  };   
       
      el = el->next(); 
      _openClause.AllLiterals().remove(simplif_lit);


      subst_depth = ((Flatterm*)subst_term)->Depth();
      ASSERT(subst_depth);
      subst_weight = subst_term->size();

      if (_openClause.AllLiterals().substPossible(var,subst_depth,subst_weight))
	{ 
	  for (TMP_LITERAL* oldEl = _openClause.AllLiterals().first();   
	       oldEl;
	       oldEl = oldEl->next())
	    {        	
	      if (oldEl->containsVariable(var))
		{
		  TMP_LITERAL* spareLiteral = TMP_LITERAL::freshLiteral();
		  oldEl->instantiateVar(*(spareLiteral),var,(Flatterm*)subst_term);
		  // Now spareLiteral contains the new version of the literal
		  _openClause.AllLiterals().replaceBy(oldEl,spareLiteral);
		  TMP_LITERAL* tmp = oldEl;
		  oldEl = spareLiteral;
		  tmp->recycle();
		};    
	    };             
	  simplif_lit->recycle();
	  return true; 
	}
      else 
	{ 
	  if (el) { el = el->previous(); }
	  else
	    el = _openClause.AllLiterals().last();
	  _openClause.AllLiterals().push(simplif_lit);
	  goto try_next_lit;
	};
    } 
  else 
    {
      el = el->previous();
      goto try_next_lit;
    };
}; // bool NewClause::SimplifyByEqRes()




Clause* NewClause::RewriteByUEq(TMP_LITERAL* lit,TMP_LITERAL* new_lit)
{
  CALL("RewriteByUEq(TMP_LITERAL* lit,TMP_LITERAL* new_lit)");
  TERM header = lit->header();
  if (!header.arity()) return (Clause*)0; // propositional literal
  Flatterm::Iterator iter;
  iter.Reset(lit->flatterm());
  iter.NextSym(); // skipping the header
  const Flatterm* rewritten_subterm; 

  while (iter.NotEndOfTerm())
    {
      rewritten_subterm = iter.CurrPos();
      if (rewritten_subterm->Symbol().isComplex())
	{
	  if (_forwardSimplificationByUEq->Rewrite(rewritten_subterm,_splittingBranches))
	    {
	
	      if (_forwardSimplificationByUEq->SplittingBranchesUsed()) 
		AnotherFDOnSplittingBranches();    

	      new_lit->reset();
	      if (header.IsEquality() && header.IsOrderedEq()) header.MakeUnordered();
	      new_lit->pushNonpropHeader(header);        

	      iter.Reset(lit->flatterm());
	      iter.NextSym(); // skipping the header
	      TERM sym;
	      while (iter.NotEndOfTerm())
		{
		  if (iter.CurrPos() == rewritten_subterm)
		    {
		      do
			{
			  sym = _forwardSimplificationByUEq->RHSSymbol();
			  if (sym.isVariable())
			    {
			      new_lit->pushVar(sym);
			    }
			  else // functor
			    {
			      if (sym.arity())
				{
				  if (!new_lit->pushNonconstFun(sym)) goto try_next_position;
				}
			      else // constant
				{
				  new_lit->pushConst(sym);
				};
			    };
			}
		      while (_forwardSimplificationByUEq->RHSNext());
		      iter.after();
		    }
		  else
		    {
		      sym = iter.CurrPos()->Symbol();
        
		      if (sym.isVariable())
			{
			  new_lit->pushVar(sym);
			}
		      else // functor
			{
			  if (sym.arity())
			    {
			      if (!new_lit->pushNonconstFun(sym)) goto try_next_position;
			    }
			  else // constant
			    {
			      new_lit->pushConst(sym);
			    };
			};
        
		      iter.NextSym();
		    };
		};     
	      
	      new_lit->endOfLiteral();
	      Clause* simplifier = _forwardSimplificationByUEq->Rule();  
	      ASSERT(simplifier->Active());  
	      PushAncestor(simplifier);
	      //simplifier->MarkAsSimplifier();
	      simplifier->setFlag(Clause::FlagHasChildren);
	      return simplifier;
	    };
	};

    try_next_position:   
      iter.NextSym();
    };
  return (Clause*)0;
}; // Clause* NewClause::RewriteByUEq(TMP_LITERAL* lit,TMP_LITERAL* new_lit)


bool NewClause::SimplifyByUEq(TMP_LITERAL*& lit,TMP_LITERAL*& spare_lit)
{
  CALL("SimplifyByUEq(TMP_LITERAL*& lit,TMP_LITERAL*& spare_lit)");

  ulong num_of_rew = 0;
  Clause* simplifier;
  while ((simplifier = RewriteByUEq(lit,spare_lit))) // assignment, not a comparison 
    {

      //DF; cout << "\n\nSIMPLIFIER " << simplifier << "\n";
      //DF; cout << lit << "  ----> " << spare_lit << "\n";
 
      ASSERT(((Flatterm*)(spare_lit->flatterm()))->CheckIntegrity());

      _openClause.AllLiterals().replaceBy(lit,spare_lit);
      TMP_LITERAL* new_spare_lit = lit;
      lit = spare_lit;
      spare_lit = new_spare_lit;
      num_of_rew++;
    };      
  
  return (bool)num_of_rew;
}; // bool NewClause::SimplifyByUEq(TMP_LITERAL*& lit,TMP_LITERAL*& spare_lit) 

bool NewClause::SimplifyByUEq()
{  
  CALL("SimplifyByUEq()");
 
  if (_fDOnSplittingBranchesFlag) 
    {
      // collect _splittingBranches 
      _splittingBranches.reset();
      DOP(_splittingBranches.unfreeze());
      for (const TMP_LITERAL* el = _openClause.AllLiterals().first(); el; el = el->next())
	if (el->header().IsSplitting()) 
	  _splittingBranches.addSafe(el->header().HeaderNum());
      DOP(_splittingBranches.freeze()); 
    };  

  ulong num_of_simpl_lits = 0;

  TMP_LITERAL* lit;
  for (TMP_LITERAL* el = _openClause.AllLiterals().first(); el; el = el->next())
    {
      lit = el;
      TMP_LITERAL* spareLiteral = TMP_LITERAL::freshLiteral();

      if (SimplifyByUEq(lit,spareLiteral)) // may swap lit and spareLiteral
	{ 
	  num_of_simpl_lits++;
	};
      el = lit;
      spareLiteral->recycle();
    }; 
  return (bool)num_of_simpl_lits; 
}; // bool NewClause::SimplifyByUEq()


bool NewClause::simplifyByBuiltInTheories(TMP_LITERAL*& lit,TMP_LITERAL*& spareLit)
{
  CALL("simplifyByBuiltInTheories(TMP_LITERAL*& lit,TMP_LITERAL*& spareLit)");
  ulong numOfSimplifications = 0UL;
  Clause* simplifier;
  while ((simplifier = BuiltInTheoryDispatcher::current()->simplify(lit,spareLit))) // assignment, not a comparison 
    {

      //DF; cout << "\n\nSIMPLIFIER " << simplifier << "\n";
      //DF; cout << lit << "  ----> " << spareLit << "\n";

      PushAncestor(simplifier);
      simplifier->setFlag(Clause::FlagHasChildren);

      ASSERT(((Flatterm*)(spareLit->flatterm()))->CheckIntegrity());

      _openClause.AllLiterals().replaceBy(lit,spareLit);
      TMP_LITERAL* newSpareLit = lit;
      lit = spareLit;
      spareLit = newSpareLit;
      ++numOfSimplifications;
    };     

  return (bool)numOfSimplifications;
}; // bool NewClause::simplifyByBuiltInTheories(TMP_LITERAL*& lit,TMP_LITERAL*& spareLit)


bool NewClause::simplifyByBuiltInTheories()
{
  CALL("simplifyByBuiltInTheories()");
  ASSERT(BuiltInTheoryDispatcher::current());
  ulong numOfSimplifiedLiterals = 0UL;
  TMP_LITERAL* lit;  
  for (TMP_LITERAL* el = _openClause.AllLiterals().first(); el; el = el->next())
    {
      lit = el;
      TMP_LITERAL* spareLiteral = TMP_LITERAL::freshLiteral();
      if (simplifyByBuiltInTheories(lit,spareLiteral)) // may swap lit and spareLiteral
	++numOfSimplifiedLiterals;
      el = lit;
      spareLiteral->recycle();
    };
  return (bool)numOfSimplifiedLiterals; 
}; // bool NewClause::simplifyByBuiltInTheories()


bool NewClause::PushLiteral(TERM* t,bool inherentlySelected)
{
  CALL("PushLiteral(TERM* t,bool inherentlySelected)");
  TERM header = *t;
  if (!(header.arity())) 
    {
      return PushPropLit(header,inherentlySelected);
    };
  if (!(OpenNonpropLiteral(header,inherentlySelected))) return false;
  TERM::Iterator iter;
  iter.Reset(t);
  while (iter.Next()) 
    {
      if (iter.CurrentSymIsVar())
	{
	  PushVar(iter.Symbol());
	}
      else
	{
	  if (iter.Symbol().arity())
	    {
	      if (!(PushNonconstFun(iter.Symbol()))) return false;
	    }
	  else PushConst(iter.Symbol()); 
	}; 
    };
  return CloseLiteral();
}; // bool NewClause::PushLiteral(TERM* t,bool inherentlySelected)

bool NewClause::LoadClause(const Clause* c)
{
  CALL("LoadClause(const Clause* c)");

  Reset();

  for(LiteralList::Iterator iter(c->LitList());iter.notEnd();iter.next())
    if (!(PushLiteral(iter.currentLiteral(),iter.currentElement()->isInherentlySelected()))) return false;
       
  return true;
}; // void NewClause::LoadClause(Clause* c)

bool NewClause::LoadClause(PagedPassiveClauseStorage::ClauseLiterals& literals)
{
  CALL("LoadClause(PagedPassiveClauseStorage::ClauseLiterals& literals)");
  literals.reset();
  Reset();

  while (literals.nextLiteral())
    {
      bool inherentlySelected = literals.currentLiteralIsInherentlySelected();
      TERM sym;
      ALWAYS(literals.nextSymbol(sym)); // header
      if (!sym.arity()) 
	{
	  if (!PushPropLit(sym,inherentlySelected)) return false;
	}
      else
	{
	  if (!(OpenNonpropLiteral(sym,inherentlySelected))) return false; 
	  while (literals.nextSymbol(sym)) if (!PushSymbol(sym)) return false;
	  if (!CloseLiteral()) return false;
	};
    };
  return true;
}; // bool NewClause::LoadClause(PagedPassiveClauseStorage::ClauseLiterals& literals)



bool NewClause::PushRewrittenLit(PREFIX_SYM* lit,
                                 PREFIX_SYM* redex,
                                 TERM replacing_term,
                                 ulong index,
                                 bool inherentlySelected)
{
  CALL("PushRewrittenLit(PREFIX_SYM* lit,PREFIX_SYM* redex,TERM replacing_term,ulong index,bool inherentlySelected)");
  TERM header = lit->Head().Func();
  ASSERT(header.arity());
  if (header.IsEquality()) header.MakeUnordered(); 
  if (!(OpenNonpropLiteral(header,inherentlySelected))) return false;
  
  PREFIX::ITER iter;
  iter.Reset(lit);
  TERM sym;
  iter.SkipSym(); // skipping the header
  
  do
    {
      if (iter.CurrVar()) // free variable
	{ 
	  if (!(PushVar(_varRenaming.Rename(iter.CurrVar())))) { return false; }; 
	  iter.SkipSym(); 
	}
      else // functor
	{
	  if (iter.CurrSubterm() == redex)
	    {
	      if (!PushStandardTerm(index,replacing_term)) return false;
	      iter.SkipTerm();
	    }
	  else
	    {
	      sym = iter.CurrSubterm()->Head().Func();
	      if (sym.arity())
		{
		  if (!(PushNonconstFun(sym))) return false;
		}
	      else // constant
		PushConst(sym);
	      iter.SkipSym();
	    }; 
	};
    }
  while (!iter.Empty()); 
 
  return CloseLiteral();
}; //  bool NewClause::PushRewrittenLit(PREFIX_SYM* lit,...  



bool NewClause::NormalizeByCommutativity()
{
  CALL("NormalizeByCommutativity()");

  ulong num_of_norm_lits = 0;
 
  for (TMP_LITERAL* el = _openClause.AllLiterals().first(); el; el = el->next())
    {
      if (el->arity() && (NormalizeByCommutativity(el)))
	{ 
	  num_of_norm_lits++;
	}; 
    };
  return (bool)num_of_norm_lits; 
}; // bool NewClause::NormalizeByCommutativity()

bool NewClause::NormalizeByCommutativity(TMP_LITERAL* lit)
{
  CALL("NormalizeByCommutativity(TMP_LITERAL* lit)");
  const Clause* commutativityLaw;
  bool literalChanged = false;

  for (Flatterm* currentSubterm = lit->flatterm()->lastSymbol();
       currentSubterm != lit->flatterm();
       currentSubterm = currentSubterm->previous())
    {
      if (currentSubterm->isComplex() &&
	  ((commutativityLaw = _commutativityTable->commutativityLaw(currentSubterm->functor()))))
	{
	  if (SimplificationOrdering::current()->compareWithTransposition(currentSubterm) == Greater)
	    {
	      currentSubterm->BinSwapArgs(); 
	      literalChanged = true;        
	      PushAncestor((Clause*)commutativityLaw);	      
	    };
	};     
    };


  if (lit->isEquality() && literalChanged) lit->makeEqUnordered();
  return literalChanged;
}; // bool NewClause::NormalizeByCommutativity(TMP_LITERAL* lit)



inline Clause* NewClause::FSubsume() 
{
  CALL("FSubsume()");
  return _forwardSubsumption->subsume(_openClause.AllLiterals());  
}; // Clause* NewClause::FSubsume()


Clause* NewClause::FSubsume(Clause* cl)
{
  CALL("FSubsume(Clause* cl)");
  LoadClause(cl);
  _openClause.EndOfClause();
  return FSubsume();
}; // Clause* NewClause::FSubsume(Clause* cl)



ostream& NewClause::output(ostream& str) const 
{
  return outputAncestors(str << _openClause);
};
 
ostream& NewClause::outputAncestors(ostream& str) const
{
  ClauseBackground::outputRules(str << '[',_ancestors.Rules());  
  for (Clause* const * anc = _ancestors.last(); anc >= _ancestors.begin(); anc--)
    str << (*anc)->Number() << ' '; 
  return str << ']';              
};
   
}; // namespace VK




//===========================================================================

