#ifndef OR_INDEX_H
//==================================================================
#define OR_INDEX_H
#ifndef NO_DEBUG_VIS
#include <iostream.h>
#endif
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "GlobAlloc.h"
#include "Array.h"
#include "VampireKernelConst.h"
#include "Term.h"
#include "prefix.h"
#include "variables.h"
#include "MultisetOfVariables.h"
#include "unification.h"
#include "BackParaCandidates.h"
#include "SimplificationOrdering.h"
#include "DestructionMode.h"
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_OR_INDEX
#define DEBUG_NAMESPACE "OR_INDEX"
#endif 
#include "debugMacros.h"
//=================================================
namespace VK
{

class Clause;
class BackParaCandidates;

class OR_INDEX_TREE_NODE;

class OR_INDEX
{
 public:
  OR_INDEX();

  ~OR_INDEX() { CALL("destructor ~OR_INDEX()"); };

#ifndef NO_DEBUG
  bool clean() const;
#endif

  void SetParamodulation(bool fl) { paramodulation = fl; }; 
  void Destroy();
  OR_INDEX_TREE_NODE** Tree(const TERM& hd) 
    { 
      CALL("Tree(const TERM& hd)");
      DOP(_literalIndex.unfreeze());
      OR_INDEX_TREE_NODE** res = &(_literalIndex.sub(hd.HeaderNum()));   
      DOP(_literalIndex.freeze());
      return res;
    };


  OR_INDEX_TREE_NODE** FParaTree(const TERM& fun) 
    {
      CALL("FParaTree(const TERM& fun)");
      DOP(_literalIndex.unfreeze());
      OR_INDEX_TREE_NODE** res = &(_literalIndex.sub(fun.HeaderNum()));   
      DOP(_literalIndex.freeze());
      return res;
    }; 
  OR_INDEX_TREE_NODE** FParaFromVarTree() { return &forward_para_from_var_tree; }; 
  OR_INDEX_TREE_NODE** BParaTree() { return &backward_para_tree; }; 
  void Integrate(Clause* c);
  void Remove(Clause* c);
   
 private:
  OR_INDEX_TREE_NODE* CreateBranch(OR_INDEX_TREE_NODE*& leaf);
  void Integrate(TERM* lit,Clause* cl,unsigned lit_num);
  OR_INDEX_TREE_NODE* IntegratePropLit(TERM lit); // returns the leaf
  OR_INDEX_TREE_NODE* OR_INDEX::IntegrateArgs(OR_INDEX_TREE_NODE** tree); // returns the leaf
  void integrateForBackwardSuperposition(TERM* lit,Clause* cl,unsigned lit_num);
  void integrateBackSupRedex(TERM* redex,Clause* cl,unsigned lit_num,
			     SimplificationOrdering::StoredConstraint* constr);
  void integrateForForwardSuperposition(TERM* lit,Clause* cl,unsigned lit_num);
  void integratePreorderedForForwardSuperposition(TERM* lit,Clause* cl,unsigned lit_num); 
  
  void integrateConstrainedForForwardSuperposition(const TERM& lhs,const TERM& rhs,Clause* cl,unsigned lit_num,
					  SimplificationOrdering::StoredConstraint* constr);
  

  OR_INDEX_TREE_NODE* IntegrateNonpropLit(TERM* lit); // returns the leaf
   
  OR_INDEX_TREE_NODE* LeafAndBranch(TERM* t,OR_INDEX_TREE_NODE** tree,OR_INDEX_TREE_NODE**& branch);
  void RemovePropLit(TERM* lit,Clause* cl);
  void Remove(TERM* t,Clause* cl,OR_INDEX_TREE_NODE** tree);
  void RemoveBPCandidates(TERM* t,Clause* cl,OR_INDEX_TREE_NODE** tree);
  void RemoveBPRedex(TERM* t,Clause* cl);
  void RemoveFromBPI(TERM* lit,Clause* cl);
  void RemoveFromFPI(TERM* lit,Clause* cl);
  void Remove(TERM* lit,Clause* cl);
   
  static void collectVarsAndFuncs(const TERM* t,MultisetOfVariables<VampireKernelConst::MaxNumOfVariables>& vars,ulong& funcs)
    {
      CALL("collectVarsAndFuncs(const TERM* t,MultisetOfVariables<VampireKernelConst::MaxNumOfVariables>& vars,ulong& funcs)");
      ASSERT(t->isComplex());
      vars.Reset();
      funcs = 0;
      TERM::Iterator termIter(t);       
      do 
	{
	  if (termIter.CurrentSymIsVar())
	    {
	      vars.Register(termIter.symbolRef().var());
	    } 
	  else funcs++;
	}
      while (termIter.Next());  
    }; // void collectVarsAndFuncs(const TERM* t,MultisetOfVariables<VampireKernelConst::MaxNumOfVariables>& vars,ulong& funcs)

 private: // auxilliary data for maintainence operations
  TERM::IteratorWithDepth l_iter;
  MultisetOfVariables<VampireKernelConst::MaxNumOfVariables> var_counter;
  TERM pseudo_literal[2];
  MultisetOfVariables<VampireKernelConst::MaxNumOfVariables> _vars1;
  MultisetOfVariables<VampireKernelConst::MaxNumOfVariables> _vars2;
 private: // structure

   
  Array<GlobAlloc,OR_INDEX_TREE_NODE*,128UL,OR_INDEX> _literalIndex;

  OR_INDEX_TREE_NODE* forward_para_from_var_tree;  
  OR_INDEX_TREE_NODE* backward_para_tree; 
  bool paramodulation;
}; // class OR_INDEX 

}; // namespace VK
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_OR_INDEX
#define DEBUG_NAMESPACE "RES_PAIR_LIST"
#endif 
#include "debugMacros.h"
//=================================================
namespace VK
{
class RES_PAIR_LIST
{
 private: // sructure
  unsigned lit_num;
  Clause* clause;
  TERM term; // for forward and backward paramodulation
  SimplificationOrdering::StoredConstraint* _orderingConstraint; // for forward superposition
  RES_PAIR_LIST* next_pair;
 public: // constructors and memory management
  RES_PAIR_LIST(unsigned ln,Clause* cl,RES_PAIR_LIST* np) : 
    lit_num(ln), 
    clause(cl), 
    _orderingConstraint((SimplificationOrdering::StoredConstraint*)0),
    next_pair(np) 
    {
    };
  RES_PAIR_LIST(unsigned ln,Clause* cl,TERM t,SimplificationOrdering::StoredConstraint* cns,RES_PAIR_LIST* np) 
    // for forward paramodulation
    : lit_num(ln), 
    clause(cl), 
    term(t),
    _orderingConstraint(cns),
    next_pair(np) 
    {        
      if (cns) cns = SimplificationOrdering::current()->copy(cns);
    }; 
  RES_PAIR_LIST(unsigned ln,Clause* cl,TERM* t,SimplificationOrdering::StoredConstraint* cns,RES_PAIR_LIST* np) 
    // for backward paramodulation
    : lit_num(ln), 
    clause(cl), 
    _orderingConstraint(cns),
    next_pair(np) 
    {
      term.SetFirst(t);
      if (cns) cns = SimplificationOrdering::current()->copy(cns);
    };
  ~RES_PAIR_LIST() 
    { 
      CALL("destructor ~RES_PAIR_LIST()"); 
      if (DestructionMode::isThorough())
	{
	  if (_orderingConstraint)
	    SimplificationOrdering::current()->releaseConstraint(_orderingConstraint);
	};
    };
  void* operator new(size_t)
    {
      return GlobAlloc::allocate(sizeof(RES_PAIR_LIST));
    };
  void operator delete(void* obj)
    {
      GlobAlloc::deallocate(obj,sizeof(RES_PAIR_LIST));
    };
 public:
  unsigned LitNum() const { return lit_num; };
  Clause* TheClause() const { return clause; };


  SimplificationOrdering::StoredConstraint* orderingConstraint() { return _orderingConstraint; };


  TERM ReplacingTerm() const { return term; }; // for forward paramodulation
  TERM* Redex() const { return term.First(); }; // for backward paramodulation
  RES_PAIR_LIST* Next() { return next_pair; };
  RES_PAIR_LIST** Tail() { return &next_pair; };
   
   
  static void Remove(Clause* c,RES_PAIR_LIST*& lst)
    {
      CALL("Remove(Clause* c,RES_PAIR_LIST*& lst)");
      RES_PAIR_LIST* pair_to_delete;
      while ((lst) && (lst->TheClause() == c))
	{
	  pair_to_delete = lst;
	  lst = lst->Next();
	  delete pair_to_delete;
	};
      if (!lst) return;
      RES_PAIR_LIST** tail = lst->Tail();
      while (*tail) 
	{
	  if ((*tail)->TheClause() == c)
	    {
	      pair_to_delete = *tail;
	      *tail = (*tail)->Next();
	      delete pair_to_delete;  
	    }
	  else tail = (*tail)->Tail(); 
	}; 
    }; // void Remove(Clause* const c,RES_PAIR_LIST* lst)

  static void Destroy(RES_PAIR_LIST* lst)
    {
      CALL("Destroy(RES_PAIR_LIST* lst)");
      RES_PAIR_LIST* tmp;
      while (lst) 
	{
	  tmp = lst->Next();    
	  delete lst;      
	  lst = tmp;
	};
    }; // void Destroy(RES_PAIR_LIST* lst) 
}; // class RES_PAIR_LIST

}; // namespace VK

//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_OR_INDEX
#define DEBUG_NAMESPACE "OR_INDEX_TREE_NODE"
#endif 
#include "debugMacros.h"
//================================================= 
namespace VK
{
class OR_INDEX_TREE_NODE
{
 public:
  enum TAG
  {
    FUNC,
    CONST,
    VAR,
    VAR_FIRST_OCC,
    LEAF
  };
 private:
  struct INTERMEDIATE
  {
    TAG tag;
    unsigned long depth;
    PREF_SYM symbol;
    OR_INDEX_TREE_NODE* below;
    OR_INDEX_TREE_NODE* alternative;
  };
  struct LEAF_NODE
  {
    TAG tag;
    unsigned long depth;
    TERM term;
    union
    {
      RES_PAIR_LIST* clauses;
      BackParaCandidates* bp_candidates;
    };
  };
 public: 
  OR_INDEX_TREE_NODE() {};
  ~OR_INDEX_TREE_NODE() {};
  
  static OR_INDEX_TREE_NODE* NewNode(TAG t,PREF_SYM s,unsigned d)
    {
      INTERMEDIATE* res = (INTERMEDIATE*)(GlobAlloc::allocate(sizeof(INTERMEDIATE)));
      res->tag = t;
      res->depth = d;
      res->symbol = s;
      res->below = 0;
      res->alternative = 0;
      return (OR_INDEX_TREE_NODE*)res;
    };
  void DeleteNode()
    {
      GlobAlloc::deallocate((void*)this,sizeof(INTERMEDIATE));
    };
  static OR_INDEX_TREE_NODE* NewLeaf()
    {
      LEAF_NODE* res = (LEAF_NODE*)(GlobAlloc::allocate(sizeof(LEAF_NODE)));
      res->tag = LEAF;
      res->depth = 0;
      res->term.SetFirst((TERM*)0);
      res->clauses = (RES_PAIR_LIST*)0; 
      return (OR_INDEX_TREE_NODE*)res;
    }; 
  void DeleteLeaf()
    {
      CALL("DeleteLeaf()");
      if (((LEAF_NODE*)this)->clauses) RES_PAIR_LIST::Destroy(((LEAF_NODE*)this)->clauses);
      GlobAlloc::deallocate((void*)this,sizeof(LEAF_NODE));
    };

  void DeleteBPILeaf()
    {
      CALL("DeleteBPILeaf()");
      if (((LEAF_NODE*)this)->bp_candidates) delete ((LEAF_NODE*)this)->bp_candidates;
      GlobAlloc::deallocate((void*)this,sizeof(LEAF_NODE));
    };  
   
  const TAG& Tag() const { return ((INTERMEDIATE*)this)->tag; }; // applicable to leaves
  bool IsLeaf() const { return (Tag() == LEAF); };
  bool IsVar() const { return ((Tag() == VAR) || (Tag() == VAR_FIRST_OCC)); };
  const PREF_SYM& Symbol() const { return ((INTERMEDIATE*)this)->symbol; };
  OR_INDEX_TREE_NODE* Below() const { return ((INTERMEDIATE*)this)->below; };
  OR_INDEX_TREE_NODE** BelowAddr() { return &(((INTERMEDIATE*)this)->below); };
  void SetBelow(OR_INDEX_TREE_NODE* b) { ((INTERMEDIATE*)this)->below = b; };
  OR_INDEX_TREE_NODE* Alternative() const { return ((INTERMEDIATE*)this)->alternative; };
  OR_INDEX_TREE_NODE** AlternativeAddr() { return &(((INTERMEDIATE*)this)->alternative); };
  void SetAlternative(OR_INDEX_TREE_NODE* a) { ((INTERMEDIATE*)this)->alternative = a; };
  unsigned long Depth() const { return ((INTERMEDIATE*)this)->depth; }; // applicable to leaves
   
  bool SameSymbol(const TERM s) const
    {
      if (s.isVariable())
	{
	  return (Symbol().var() == Unifier::current()->variableBase(s.var()));
	}
      else // here s must be a functor
	{ 
	  return (Symbol().Func() == s);
	};
    }; // bool SameSymbol(const TERM s) const

  // for leaves:
  const TERM& Term() const { return ((LEAF_NODE*)this)->term; };
  void SetTerm(TERM* t) { ((LEAF_NODE*)this)->term.SetFirst(t); };
  void SetTerm(TERM t) { ((LEAF_NODE*)this)->term = t; };
  RES_PAIR_LIST* Clauses() const { return ((LEAF_NODE*)this)->clauses; };
  BackParaCandidates* BPCandidates() const { return ((LEAF_NODE*)this)->bp_candidates; };
  void AddPair(unsigned ln,Clause* cl)
    {
      ((LEAF_NODE*)this)->clauses = new RES_PAIR_LIST(ln,cl,((LEAF_NODE*)this)->clauses);
    };
  void addForwardSupTriple(unsigned ln,Clause* cl,TERM t,SimplificationOrdering::StoredConstraint* constr)
    {
      ((LEAF_NODE*)this)->clauses = new RES_PAIR_LIST(ln,cl,t,constr,((LEAF_NODE*)this)->clauses);
    }; 
  
  void addBackSupTriple(unsigned ln,Clause* cl,TERM* t,SimplificationOrdering::StoredConstraint* constr)
    {
      CALL("addBackSupTriple(unsigned ln,Clause* cl,TERM* t,SimplificationOrdering::StoredConstraint* constr)");
      BackParaCandidates* candidates = ((LEAF_NODE*)this)->bp_candidates;
      if (!candidates)         
	{
	  candidates = new BackParaCandidates();
	  ((LEAF_NODE*)this)->bp_candidates = candidates;
	};    
      candidates->add(cl,ln,t,constr);
    }; 

  void Remove(Clause* c)
    {
      RES_PAIR_LIST::Remove(c,((LEAF_NODE*)this)->clauses);
    };
  
      
  void RemoveBPCandidates(Clause* c)      
    {
      CALL("RemoveBPCandidates(Clause* c)");
      ASSERT(((LEAF_NODE*)this)->bp_candidates);
      BackParaCandidates* candidates = ((LEAF_NODE*)this)->bp_candidates;
      candidates->remove(c); 
      if (candidates->empty()) 
	{
	  delete candidates;
	  ((LEAF_NODE*)this)->bp_candidates = (BackParaCandidates*)0;
	};     
    };

 public: 
  static void Destroy(OR_INDEX_TREE_NODE* tree);
  static void DestroyBPI(OR_INDEX_TREE_NODE* tree);
 public: // methods for debugging
#ifndef NO_DEBUG_VIS
  ostream& output(ostream& str) const { return output(str,0); };
  ostream& output(ostream& str,unsigned var_offset) const;
#endif
}; // class OR_INDEX_TREE_NODE

}; // namespace VK

#ifndef NO_DEBUG_VIS
ostream& operator<<(ostream& str,const VK::OR_INDEX_TREE_NODE::TAG& tag);
#endif



//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_OR_INDEX
#define DEBUG_NAMESPACE "OR_INDEX_PATH"
#endif 
#include "debugMacros.h"
//================================================= 
namespace VK
{
class OR_INDEX_PATH
{
 private:
  OR_INDEX_TREE_NODE** bookmark[VampireKernelConst::MaxTermSize];
  OR_INDEX_TREE_NODE*** next_bookmark;
  OR_INDEX_TREE_NODE* path[VampireKernelConst::MaxTermSize];
  OR_INDEX_TREE_NODE** next;
  OR_INDEX_TREE_NODE** changed_suffix;
  OR_INDEX_TREE_NODE* curr_node;
  unsigned var_offset;
  Unifier::Variable* var_extracted;
 public:
  OR_INDEX_PATH() {};
  ~OR_INDEX_PATH() {};
  void init() {};
  void SetIndex(ulong index) 
    {
      CALL("SetIndex(ulong index)");
      var_offset = Unifier::variableOffset(index); 
    };
  ulong GetIndex() const { return Unifier::variableIndex(var_offset); };
  Unifier::Variable* Shift(Unifier::Variable* const v) { return (v + var_offset); }; 
  void Reset(OR_INDEX_TREE_NODE* const tree)
    {
      CALL("Reset(OR_INDEX_TREE_NODE* const tree)");
      ASSERT(tree);
      next = path;
      curr_node = tree;
      next_bookmark = bookmark;
    };
  

  //static void ResetStatic() { Unifier::Variable::Reset(); };


  OR_INDEX_TREE_NODE* CurrNode() const { return curr_node; };
  OR_INDEX_TREE_NODE** Position() const { return next; };
  OR_INDEX_TREE_NODE** ChangedSuffix() const { return changed_suffix; };
  void MakeBookmark() { *next_bookmark = Position(); next_bookmark++; };
  void ForgetBookmark() { next_bookmark--; };
  OR_INDEX_TREE_NODE** LastBookmark() const { return *(next_bookmark-1); };
  Unifier::Variable* VarExtracted() const { return var_extracted; };
  void MemorizeCurrent() { *next = curr_node; next++; };
  void RestoreCurrent() { next--; curr_node = *next; };
  void Below() 
    {
      CALL("Below()");
      ASSERT(!(curr_node->IsLeaf()));
      MemorizeCurrent();
      curr_node = curr_node->Below();
    };
  void Alternative() 
    { 
      CALL("!(curr_node->IsLeaf())");
      ASSERT(!(curr_node->IsLeaf())); 
      curr_node = curr_node->Alternative();
    };
  void ExtractTerm()
    {
      MakeBookmark();
      if (curr_node->IsVar())
	{
	  var_extracted = Shift(curr_node->Symbol().var());
	  Below();
	}
      else // complex term to be extracted
	{
	  var_extracted = (Unifier::Variable*)0;
	  unsigned long init_depth = curr_node->Depth();
	  do 
	    {
	      Below();
	    }
	  while (init_depth < curr_node->Depth());
	};
    }; // void ExtractTerm()
  bool ModifyTerm();
  bool ExtractTermWithout(Unifier::Variable* const var,bool& same_var);
  bool ModifyTermWithout(Unifier::Variable* const var,
			 bool& same_var);
  void ToPrefix(OR_INDEX_TREE_NODE** pos,PREFIX_SYM*& memory);
 public: // methods for debugging
#ifndef NO_DEBUG_VIS
  ostream& output(ostream& str) const;
#endif
}; // class OR_INDEX_PATH 

}; // namespace VK

//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_OR_INDEX
#define DEBUG_NAMESPACE "OR_PROCESSOR"
#endif 
#include "debugMacros.h"
//================================================= 
namespace VK
{
class OR_PROCESSOR : private OR_INDEX_PATH
{  
 private:
  PREFIX_SYM memory[VampireKernelConst::MaxTermSize];
  PREFIX_SYM* free_memory;
  PREFIX::ITER_WITH_BACKTRACK* term[VampireKernelConst::MaxTermSize];
  PREFIX::ITER_WITH_BACKTRACK** next_term;
  PREFIX::ITER_WITH_BACKTRACK* curr_iter;
 public: // made public because VC++ doesn't like friends,
  // see ostream& operator<<(ostream& str,const OR_PROCESSOR::SITUATION& sit) 
  enum SITUATION
  {
    START_OF_EXTRACTION,
    VAR_SAME_VAR,
    VAR_ANYTHING,
    FUNCTOR_FUNCTOR,
    FUNCTOR_FREE_VAR,
    FUNCTOR_BOUND_VAR
  };
 private:
  SITUATION backtrack_situation[VampireKernelConst::MaxTermSize*2];
  SITUATION* curr_sit; 
 private:
  void NextIter() 
    {
      CALL("NextIter()");
      next_term++;
      curr_iter = *next_term;
      if (!curr_iter) 
	{
	  curr_iter = new PREFIX::ITER_WITH_BACKTRACK(); 
	  *next_term = curr_iter;
	};
      ASSERT(next_term - term <= (long)VampireKernelConst::MaxTermSize);
    };
  void ForgetIter() 
    {
      CALL("ForgetIter()");
      next_term--; 
      ASSERT(next_term >= term);
      curr_iter = *next_term;
    };
  PREFIX::ITER_WITH_BACKTRACK* LastIter() { return *(next_term-1); };
 public:
  OR_PROCESSOR() : OR_INDEX_PATH() 
    {
      for (ulong i = 0; i < VampireKernelConst::MaxTermSize; i++)
	{
	  term[i] = (PREFIX::ITER_WITH_BACKTRACK*)0;
	};
    };
  ~OR_PROCESSOR() {};
  void init() 
    {
      OR_INDEX_PATH::init();
      for (ulong i = 0; i < VampireKernelConst::MaxTermSize; i++)
	{
	  term[i] = (PREFIX::ITER_WITH_BACKTRACK*)0;
	}; 
    };
  //  static void ResetStatic() { OR_INDEX_PATH::ResetStatic(); };
  void SetIndex(ulong index) 
    {
      CALL("SetIndex(ulong index)");
      OR_INDEX_PATH::SetIndex(index); 
    };
  ulong GetIndex() const { return OR_INDEX_PATH::GetIndex(); };
  OR_INDEX_TREE_NODE* CurrNode() const { return OR_INDEX_PATH::CurrNode(); };
  void Below() { OR_INDEX_PATH::Below(); };
  void Alternative() { OR_INDEX_PATH::Alternative(); };
  void RestoreCurrent() { OR_INDEX_PATH::RestoreCurrent(); };
  Unifier::Variable* Shift(Unifier::Variable* v) { return OR_INDEX_PATH::Shift(v); };
 public: // commands
  void Start(OR_INDEX_TREE_NODE* tree) 
    {
      free_memory = memory;
      curr_sit = backtrack_situation;
      next_term = term;
      OR_INDEX_PATH::Reset(tree); 
    };
  void Bind(Unifier::Variable* v);
  bool AlterBinding();
  bool BindWithOccCheck(Unifier::Variable* v,bool& same_var);
  bool AlterBindingWithOccCheck(Unifier::Variable* v,bool& same_var);
 private:
  bool BacktrackExtraction();
  bool CompleteExtraction();
 public:
  bool ExtractUnifiable(PREFIX_SYM* word);
  bool AlterUnifiable();
 public: // methods for debugging
#ifndef NO_DEBUG_VIS
  ostream& outputPath(ostream& str) const { return OR_INDEX_PATH::output(str); };  
  friend ostream& operator<<(ostream& str,const OR_PROCESSOR::SITUATION& sit);
#endif  
}; // class OR_PROCESSOR : private OR_INDEX_PATH
}; // namespace VK

#ifndef NO_DEBUG_VIS
ostream& operator<<(ostream& str,const VK::OR_PROCESSOR::SITUATION& sit);
#endif

//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_OR_INDEX
#define DEBUG_NAMESPACE "OR_INDEX_PATH"
#endif 
#include "debugMacros.h"
//================================================= 
namespace VK
{
inline bool OR_INDEX_PATH::ModifyTerm()
{
  CALL("ModifyTerm()");
  OR_INDEX_TREE_NODE** begin = LastBookmark();
   
  ASSERT(begin < Position());
  
 rollback:
  RestoreCurrent();
  changed_suffix = Position();
  if (Position() == begin)  
    {
      Alternative();
      if (curr_node) { ForgetBookmark(); ExtractTerm(); return true; }
      else { ForgetBookmark(); return false; };
    };
  Alternative();
  if (curr_node)
    {
      unsigned long init_depth = (*begin)->Depth();
      do 
	{
	  Below();
	}
      while (init_depth < curr_node->Depth());
      var_extracted = (Unifier::Variable*)0;
      return true;
    }
  else { goto rollback; };
}; // bool OR_INDEX_PATH::ModifyTerm()

inline bool OR_INDEX_PATH::ExtractTermWithout(Unifier::Variable* const var,bool& same_var)
{
  CALL("ExtractTermWithout(Unifier::Variable* const var,bool& same_var)");
  MakeBookmark();
  same_var = false;
 check_node:
  switch (curr_node->Tag())
    {
    case OR_INDEX_TREE_NODE::FUNC:
      {
	var_extracted = (Unifier::Variable*)0;
	unsigned long init_depth = curr_node->Depth();
	OR_INDEX_TREE_NODE** init_pos = Position();
	Below();
      check_depth:
        if (curr_node->Depth() <= init_depth) { return true; };
      intern_check_node:
	switch (curr_node->Tag())
          {
	  case OR_INDEX_TREE_NODE::FUNC: Below(); goto intern_check_node;
	  case OR_INDEX_TREE_NODE::CONST: Below(); goto check_depth;
	  case OR_INDEX_TREE_NODE::VAR: 
            {
	      bool free_var;
	      void* bind = Shift(curr_node->Symbol().var())->Unref(free_var);
	      if (free_var)
		{
		  if ((Unifier::Variable*)bind == var) { goto try_alternative; } 
		  else { Below(); goto check_depth; };
		}
	      else // variable bound by a complex term in the node
		{
		  if (OCCUR_CHECK::Occurs(var,(PREFIX_SYM*)bind))
		    { goto try_alternative; }
		  else { Below(); goto check_depth; };
		};
            };
	  case OR_INDEX_TREE_NODE::VAR_FIRST_OCC: Below(); goto check_depth;
                      
           
#ifdef DEBUG_NAMESPACE
	  default: ICP("ICP1"); return false;
#elif defined _SUPPRESS_WARNINGS_
	  default: return false;
#endif
          };
      try_alternative:
        Alternative();
        if (curr_node) { goto intern_check_node; }
        else // backtrack needed
	  {
	    RestoreCurrent();
	    if (Position() == init_pos) 
	      {
		Alternative();
		if (curr_node) { goto check_node; }
		else { ForgetBookmark(); return false; };
	      }
	    else { goto try_alternative; };
	  };
      }; // case OR_INDEX_TREE_NODE::FUNC
    case OR_INDEX_TREE_NODE::CONST: 
      Below(); 
      var_extracted = (Unifier::Variable*)0;
      return true;
    case OR_INDEX_TREE_NODE::VAR: 
      {
	bool free_var;
	void* bind = Shift(curr_node->Symbol().var())->Unref(free_var);
	if (free_var)
	  {
	    var_extracted = Shift(curr_node->Symbol().var());
	    Below();
	    same_var = (((Unifier::Variable*)bind) == var);
	    return true;
	  }
	else
	  {
	    if (OCCUR_CHECK::Occurs(var,(PREFIX_SYM*)bind))
	      {
		Alternative();
		if (curr_node) { goto check_node; }
		else { ForgetBookmark(); return false; };
	      }
	    else
	      {
		var_extracted = Shift(curr_node->Symbol().var());
		Below();
		return true;
	      };
	  };
      };
    case OR_INDEX_TREE_NODE::VAR_FIRST_OCC:
      var_extracted = Shift(curr_node->Symbol().var());
      Below(); 
      return true;
      
#ifdef DEBUG_NAMESPACE
    default: ICP("ICP2"); return false;
#elif defined _SUPPRESS_WARNINGS_
    default: return false;
#endif
    };
  ICP("ICP3");
  return false;  
}; // bool OR_INDEX_PATH::ExtractTermWithout(Unifier::Variable* var,bool& same_var)

inline bool OR_INDEX_PATH::ModifyTermWithout(Unifier::Variable* const var,bool& same_var)
{
  CALL("ModifyTermWithout(Unifier::Variable* const var,bool& same_var)");
  OR_INDEX_TREE_NODE** begin = LastBookmark();
  
  ASSERT(begin < Position());
  
  unsigned long init_depth = (*begin)->Depth();
  
  same_var = false; 
  changed_suffix = Position()-1;
 rollback:
  RestoreCurrent();
  if (Position() < changed_suffix) { changed_suffix = Position(); };
  if (Position() == begin)  
    {
      Alternative();
      ForgetBookmark();
      return ((curr_node) && (ExtractTermWithout(var,same_var)));
    };
 try_alternative:
  Alternative();
  if (curr_node)
    {
    check_node:
      switch (curr_node->Tag())
        {
	case OR_INDEX_TREE_NODE::FUNC: Below(); goto check_node;
	case OR_INDEX_TREE_NODE::CONST: Below(); goto check_depth;
	case OR_INDEX_TREE_NODE::VAR:
          {
	    bool free_var;
	    void* bind = Shift(curr_node->Symbol().var())->Unref(free_var);
	    if (free_var)
	      {
		if ((Unifier::Variable*)bind == var) { goto try_alternative; } 
		else { Below(); goto check_depth; };
	      }
	    else // variable bound by a complex term in the node
	      {
		if (OCCUR_CHECK::Occurs(var,(PREFIX_SYM*)bind))
		  { goto try_alternative; }
		else { Below(); goto check_depth; };
	      };
          };
	case OR_INDEX_TREE_NODE::VAR_FIRST_OCC: Below(); goto check_depth;
             
#ifdef DEBUG_NAMESPACE
	default: ICP("ICP1"); return false;
#elif defined _SUPPRESS_WARNINGS_
	default: return false;
#endif
        };
      ICP("ICP2");
      return false;
    check_depth:
      if (curr_node->Depth() <= init_depth) { var_extracted = (Unifier::Variable*)0; return true; }
      else 
        {
	  goto check_node; 
        };
    }
  else { goto rollback; };
}; // bool OR_INDEX_PATH::ModifyTermWithout(Unifier::Variable* var,.......

inline void OR_INDEX_PATH::ToPrefix(OR_INDEX_TREE_NODE** pos,PREFIX_SYM*& memory)
{
  CALL("ToPrefix(OR_INDEX_TREE_NODE** pos,PREFIX_SYM*& memory)");
  OR_INDEX_TREE_NODE* node;
  while (pos < Position())
    {
      node = *pos;
      switch (node->Tag())
	{
	case OR_INDEX_TREE_NODE::FUNC:
	  memory->Head() = node->Symbol();
	  memory->ResetBracket();
	  break;
	case OR_INDEX_TREE_NODE::CONST:
	  memory->Head() = node->Symbol();
	  memory->SetBracket(memory+1);
	  break; 
	case OR_INDEX_TREE_NODE::VAR: 
	case OR_INDEX_TREE_NODE::VAR_FIRST_OCC: 
	  memory->Head().MkVar(Shift(node->Symbol().var()));
	  memory->SetBracket(memory+1); 
	  break;
#ifdef DEBUG_NAMESPACE
	default: ICP("ICP1"); return; 
#elif defined _SUPPRESS_WARNINGS_
	default: return; 
#endif
	};
      pos++;
      memory++;   
    }; 
}; // void OR_INDEX_PATH::ToPrefix(OR_INDEX_TREE_NODE** pos,PREFIX_SYM*& memory)

}; // namespace VK

//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_OR_INDEX
#define DEBUG_NAMESPACE "OR_PROCESSOR"
#endif 
#include "debugMacros.h"
//=================================================
namespace VK
{ 
inline void OR_PROCESSOR::Bind(Unifier::Variable* v)
{
  OR_INDEX_PATH::ExtractTerm();
  if (OR_INDEX_PATH::VarExtracted())
    {
      Unifier::current()->BindAlone(v,(void*)(OR_INDEX_PATH::VarExtracted()));
    }
  else // complex term extracted
    {
      PREFIX_SYM* word = free_memory;
      OR_INDEX_PATH::ToPrefix(OR_INDEX_PATH::LastBookmark(),free_memory);
      word->SetBracket(free_memory);
      Unifier::current()->BindAlone(v,(void*)word);
    };
}; // void OR_PROCESSOR::Bind(Unifier::Variable* v)
    
inline bool OR_PROCESSOR::AlterBinding()
{
  CALL("AlterBinding()");
  Unifier::Variable* v = Unifier::current()->LastBound();
  void* bind = v->Binding();
  
  ASSERT(bind);

  Unifier::current()->UnbindLast();
  
  if (OR_INDEX_PATH::ModifyTerm())
    {
      if (OR_INDEX_PATH::VarExtracted())
	{
	  Unifier::current()->BindAlone(v,(void*)(OR_INDEX_PATH::VarExtracted()));
	  if (!(Unifier::current()->isVariable(bind))) 
	    {
	      free_memory = (PREFIX_SYM*)bind; 
	    };
	}
      else // complex term extracted
	{
	  if (Unifier::current()->isVariable(bind))
	    {
	      PREFIX_SYM* word = free_memory;
	      OR_INDEX_PATH::ToPrefix(OR_INDEX_PATH::LastBookmark(),free_memory);
	      word->SetBracket(free_memory);
	      Unifier::current()->BindAlone(v,(void*)word);
	    }
	  else
	    {
	      PREFIX_SYM* changed_suffix = (PREFIX_SYM*)bind + (OR_INDEX_PATH::ChangedSuffix() - OR_INDEX_PATH::LastBookmark());
	      free_memory = changed_suffix;
	      ASSERT(free_memory >= ((PREFIX_SYM*)bind));
        
	      PREFIX_SYM* old_end_of_word = ((PREFIX_SYM*)bind)->Bracket();
	      OR_INDEX_PATH::ToPrefix(OR_INDEX_PATH::ChangedSuffix(),free_memory);
        
	      ASSERT(free_memory > ((PREFIX_SYM*)bind));
        
	      ((PREFIX_SYM*)bind)->SetBracket(free_memory);
	      PREFIX::ResetCorruptedBrackets(((PREFIX_SYM*)bind)+1,changed_suffix,old_end_of_word,free_memory);
	      Unifier::current()->BindAlone(v,bind);
	    };
	}
      return true;
    }
  else 
    {
      if (!(Unifier::current()->isVariable(bind))) { free_memory = (PREFIX_SYM*)bind; };
      return false; 
    };
}; // bool OR_PROCESSOR::AlterBinding()
    
inline bool OR_PROCESSOR::BindWithOccCheck(Unifier::Variable* v,bool& same_var)
{
  CALL("BindWithOccCheck(Unifier::Variable* v,bool& same_var)");
  if (OR_INDEX_PATH::ExtractTermWithout(v,same_var))
    {
      if (same_var) { return true; };
      if (OR_INDEX_PATH::VarExtracted())
	{
	  Unifier::current()->BindAlone(v,(void*)(OR_INDEX_PATH::VarExtracted())); 
	}
      else // complex term extracted
	{
	  PREFIX_SYM* word = free_memory;
	  ASSERT(free_memory);
      
	  OR_INDEX_PATH::ToPrefix(OR_INDEX_PATH::LastBookmark(),free_memory);
	  word->SetBracket(free_memory);
      
	  ASSERT(PREFIX::CorrectComplexTerm(word));
      
	  ASSERT(!(OCCUR_CHECK::Occurs(v,word)));
      
	  Unifier::current()->BindAlone(v,(void*)word);
	};
      return true;
    }
  else 
    {
      return false;
    };
}; // bool OR_PROCESSOR::BindWithOccCheck(Unifier::Variable* v,bool& same_var)
 
inline bool OR_PROCESSOR::AlterBindingWithOccCheck(Unifier::Variable* v,bool& same_var)
{
  CALL("AlterBindingWithOccCheck(Unifier::Variable* v,bool& same_var)");
  void* bind = v->Binding();
  bool was_var = true;
  if (bind) 
    {
      // here we suppose that v is the last bound variable
      ASSERT(v == Unifier::current()->LastBound());
      Unifier::current()->UnbindLast(); 
      if (!(Unifier::current()->isVariable(bind))) { was_var = false; free_memory = (PREFIX_SYM*)bind; };
    };
  if (OR_INDEX_PATH::ModifyTermWithout(v,same_var))
    {
      if (OR_INDEX_PATH::VarExtracted())
	{
	  if (!(same_var)) { Unifier::current()->BindAlone(v,(void*)(OR_INDEX_PATH::VarExtracted())); };
	}
      else // complex term extracted
	{
	  if (was_var)
	    {
	      PREFIX_SYM* word = free_memory;
	      OR_INDEX_PATH::ToPrefix(OR_INDEX_PATH::LastBookmark(),free_memory);
	      ASSERT(free_memory);
        
	      word->SetBracket(free_memory);

	      ASSERT(PREFIX::CorrectComplexTerm(word));
	      ASSERT(!(OCCUR_CHECK::Occurs(v,word)));
        
	      Unifier::current()->BindAlone(v,(void*)word);
	    }
	  else
	    {
	      PREFIX_SYM* changed_suffix = (PREFIX_SYM*)bind + (OR_INDEX_PATH::ChangedSuffix() - OR_INDEX_PATH::LastBookmark());
	      free_memory = changed_suffix;
        
	      ASSERT(free_memory);
        
	      PREFIX_SYM* old_end_of_word = ((PREFIX_SYM*)bind)->Bracket();
	      OR_INDEX_PATH::ToPrefix(OR_INDEX_PATH::ChangedSuffix(),free_memory);
	      ASSERT(free_memory);
        
	      ((PREFIX_SYM*)bind)->SetBracket(free_memory);
	      PREFIX::ResetCorruptedBrackets(((PREFIX_SYM*)bind)+1,changed_suffix,old_end_of_word,free_memory);
       
	      ASSERT(PREFIX::CorrectComplexTerm((PREFIX_SYM*)bind));
	      ASSERT(!(OCCUR_CHECK::Occurs(v,(PREFIX_SYM*)bind)));

	      Unifier::current()->BindAlone(v,bind);
	    };
	}
      return true;
    }
  else 
    {
      return false; 
    };
}; // bool OR_PROCESSOR::AlterBindingWithOccCheck(Unifier::Variable* v,bool& same_var)

}; // namespace VK
//==================================================================
#endif
