// Revised:     May 28, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk 
//              Representation of indexed objects changed: ulong* ---> ulong
//===========================================================
#ifndef COMPONENT_NAME_H
//===========================================================
#define COMPONENT_NAME_H
#ifndef NO_DEBUG_VIS
#include <iostream.h>
#endif
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
#include "VampireKernelConst.h"
#include "fs_code.h"
#include "fs_command.h"
#include "ExpandingStack.h"
#include "ObjectPool.h"
#include "TmpLitList.h"
#include "GlobAlloc.h"
#include "ExpandingStack.h"
#include "Array.h"
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_COMPONENT_NAME
#define DEBUG_NAMESPACE "CN_QUERY"
#endif 
#include "debugMacros.h"
//=================================================
namespace VK
{
  using namespace BK;
  using namespace Gematogen;

class CN_QUERY
{
 public:
  class LIT
    {    
    public:  
      LIT() {};
      ~LIT() {};
      void* operator new(size_t)
	{
	  CALL("LIT::operator new(size_t)");
	  return GlobAlloc::allocate(sizeof(LIT));
	};
      void operator delete(void* obj)
	{
	  CALL("LIT::operator delete(void* obj)");
	  GlobAlloc::deallocate(obj,sizeof(LIT));
	};
      const Flatterm* Args() const { return arg1; };
      void SetArgs(const Flatterm* a) { arg1 = a; };      
      const Flatterm* Arg1() const { return arg1; }; // for equations
      const Flatterm* Arg2() const { return arg2; }; // for equations
      void SetArgs(const Flatterm* a1,const Flatterm* a2)
	{ arg1 = a1; arg2 = a2; };
      LIT* Next() const { return next; };
      void SetNext(LIT* nl) { next = nl; };
      void Set(const Flatterm* a,LIT* nl) { arg1 = a; next = nl; };
      void Set(const Flatterm* a1,const Flatterm* a2,LIT* nl) 
	{ arg1 = a1; arg2 = a2; next = nl; };
      void Capture() { captured = true; };
      void Release() { captured = false; };
      bool Captured() const { return captured; };
    public:
      ostream& output(ostream& str) const { return (Args() - 1)->outputTerm(str); };
    private:
      const Flatterm* arg1;
      const Flatterm* arg2;
      bool captured;
      LIT* next;
    }; // class LIT

 public:
  CN_QUERY() : 
#ifdef NO_DEBUG 
    _litsWithHeader((LIT*)0,(int)0),
#else
    _litsWithHeader((LIT*)0,"CN_QUERY::_litsWithHeader"),
#endif

    _posOrdEqHeaderNum(TERM::positiveOrdEqHeader().HeaderNum()),
    _negOrdEqHeaderNum(TERM::negativeOrdEqHeader().HeaderNum()),
    _presentHeaders(DOP("CN_QUERY::_presentHeaders")),
    collect_signature(true) // default
    {
      CALL("constructor CN_QUERY()");
      DOP(_litsWithHeader.freeze());
      signature = (FunSet*)0; 
    };

  ~CN_QUERY() { CALL("destructor ~CN_QUERY()"); };
  void ResetQuery()
    {
      CALL("ResetQuery()");
      _litPool.reset();
      while (_presentHeaders.nonempty())
	{
	  _litsWithHeader[_presentHeaders.pop()] = (LIT*)0;
	};

      last_pos_unord_eq = (LIT*)0;
      last_neg_unord_eq = (LIT*)0;
      if (collect_signature) 
	{
	  if (signature) delete signature;
	  signature = new FunSet();
	};
      numOfLits = 0;
    }; // void ResetQuery()

  void SetCollectSignature(bool fl) 
    { 
      collect_signature = fl; 
    };
  void LinkEquations()
    {
      CALL("LinkEquations()") 
	if (last_pos_unord_eq) 
	  last_pos_unord_eq->SetNext(FirstLit(_posOrdEqHeaderNum));
      if (last_neg_unord_eq) 
	last_neg_unord_eq->SetNext(FirstLit(_negOrdEqHeaderNum));
    }; 
  void EndOfQuery()
    {
      LinkEquations();
    }; 
  void PushLit(const Flatterm* lit)
    {
      CALL("PushLit(const Flatterm* lit)");
      ASSERT(!lit->Symbol().IsEquality());

      DOP(_litsWithHeader.unfreeze());
      LIT** lit_lst_ptr = &(_litsWithHeader.sub(lit->Symbol().HeaderNum()));
      DOP(_litsWithHeader.freeze());


      LIT* currLit = _litPool.reserveObject();

      if (*lit_lst_ptr)
	{
	  currLit->Set(lit + 1,*lit_lst_ptr);
	}
      else // no literals with this header yet
	{
	  _presentHeaders.pushSafe(lit->Symbol().HeaderNum());
	  currLit->Set(lit + 1,(LIT*)0);
	};
      *lit_lst_ptr = currLit; 
      currLit->Release(); 
      if (collect_signature) CollectFunctors(lit);
    };   
    
  void PushUnordEq(TERM header,const Flatterm* arg1,const Flatterm* arg2)
    {
      CALL("PushUnordEq(TERM header,const Flatterm* arg1,const Flatterm* arg2)");
      ASSERT(header.IsUnorderedEq());
     

      DOP(_litsWithHeader.unfreeze());
      LIT** lit_lst_ptr = &(_litsWithHeader.sub(header.HeaderNum()));
      DOP(_litsWithHeader.freeze());


      LIT* currLit = _litPool.reserveObject();
      if (*lit_lst_ptr)
	{
	  currLit->Set(arg1,arg2,*lit_lst_ptr);
	}
      else // no literals with this header yet
	{

	  _presentHeaders.pushSafe(header.HeaderNum());
	  currLit->Set(arg1,arg2,(LIT*)0);
	  if (header.Negative()) { last_neg_unord_eq = currLit; }
	  else // positive
	    last_pos_unord_eq = currLit;
	};
      *lit_lst_ptr = currLit;
      currLit->Release();
      if (collect_signature) 
	{
	  AddFunctor(header.HeaderNum());
	  CollectFunctors(arg1); 
	  CollectFunctors(arg2);
	};
    };
    
  void PushOrdEq(TERM header,const Flatterm* arg1,const Flatterm* arg2)
    {
      CALL("PushOrdEq(TERM header,const Flatterm* arg1,const Flatterm* arg2)");
      ASSERT(header.IsEquality() && header.IsOrderedEq());

      DOP(_litsWithHeader.unfreeze());
      LIT** lit_lst_ptr = &(_litsWithHeader.sub(header.HeaderNum()));
      DOP(_litsWithHeader.freeze());


      LIT* currLit = _litPool.reserveObject();
      if (*lit_lst_ptr)
	{
	  currLit->Set(arg1,arg2,*lit_lst_ptr);
	}
      else // no literals with this header yet
	{

	  _presentHeaders.pushSafe(header.HeaderNum());
	  currLit->Set(arg1,arg2,(LIT*)0);
	};
      *lit_lst_ptr = currLit;
      currLit->Release(); 
      if (collect_signature) 
	{
	  header.MakeUnordered();
	  AddFunctor(header.HeaderNum());
	  CollectFunctors(arg1); 
	  CollectFunctors(arg2);
	};
    }; 
    
  LIT* FirstLit(ulong header_num)  
    {
      CALL("FirstLit(ulong header_num)");
      if (header_num >= _litsWithHeader.size()) return (LIT*)0;
      return _litsWithHeader[header_num];
    }; 
   

  const FunSet* Signature() const { return signature; }; 

  ulong NumOfDiffVars() const { return numOfDiffVars; };
  ulong NumOfLits() const { return numOfLits; }
  void Load(TmpLitList& component,ulong numOfVars)
    {
      CALL("Load(TmpLitList& component,ulong numOfVars)");
      ResetQuery(); 
      numOfDiffVars = numOfVars;
      numOfLits = component.numOfAllLiterals();
      for (const TMP_LITERAL* el = component.first();
	   el;
	   el = el->next())
	{
	  if (el->isEquality())
	    {	      
	      if (el->isOrderedEquality())   
		{
		  PushOrdEq(el->header(),el->arg1(),el->arg2());
		}      
	      else PushUnordEq(el->header(),el->arg1(),el->arg2());
	    } 
	  else PushLit(el->flatterm());
	};
    }; // void Load(TmpLitList& component,ulong numOfVars)

 private:
  void AddFunctor(ulong fun_num)
    {
      if (!(signature->write((size_t)fun_num)))
	{
	  FunSet* new_sig = signature->add((size_t)fun_num);
	  delete signature;
	  signature = new_sig; 
	};
    };
  void CollectFunctors(const Flatterm* t)
    {
      CALL("CollectFunctors(const Flatterm* t)");
      ASSERT(collect_signature);
      Flatterm::Iterator iter(t);
      TERM sym;
      while (iter.NotEndOfTerm())
	{
	  sym = iter.CurrPos()->symbolRef();
	  if (sym.isComplex()) AddFunctor(sym.HeaderNum());       
	  iter.NextSym();
	};
    };
    
 public: 
  ostream& output(ostream& str) const
    {
      for (ulong p = 0; p < _presentHeaders.size(); p++)
	for (const LIT* lit = _litsWithHeader[_presentHeaders.nth(p)]; lit; lit = lit->Next())
	  lit->output(str) << '\n';
      return str;
    };

 
 private: // structure 
  ObjectPool<GlobAlloc,LIT> _litPool;

  Array<GlobAlloc,LIT*,32UL,CN_QUERY> _litsWithHeader;
  const ulong _posOrdEqHeaderNum;
  const ulong _negOrdEqHeaderNum;

  ExpandingStack<GlobAlloc,ulong,32UL,CN_QUERY> _presentHeaders;

  LIT* last_pos_unord_eq;
  LIT* last_neg_unord_eq;
  bool collect_signature;
  FunSet* signature;
  ulong numOfDiffVars;
  ulong numOfLits;
}; // class CN_QUERY
 
}; // namespace VK

//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_COMPONENT_NAME
#define DEBUG_NAMESPACE "ComponentName"
#endif 
#include "debugMacros.h"
//=================================================
namespace VK
{
  using namespace BK;
  using namespace Gematogen;
class ComponentName 
{
 public:
  ComponentName() 
    : variantIndex(subst), 
    query(), 
    dummyComponent(),
    backtrack_lit(DOP("backtrack_lit")),
    backtrack_instr(DOP("backtrack_instr")),
    backtrack_cursor(DOP("backtrack_cursor"))
    {
      CALL("constructor ComponentName()");
      dummyTmpLiteral = TMP_LITERAL::freshLiteral();
      variantIndex.SetOldStyle(false);
      variantIndex.SetOldStyle(true); // must be so, because the components are integrated in the old style
      variantIndex.SetUseSigFilters(false);
      query.SetCollectSignature(false); 
      dummyComponent.push(dummyTmpLiteral);
      DOP(backtrack_lit.freeze());
    };
  ~ComponentName() 
    { 
      CALL("destructor ~ComponentName()"); 
      if (DestructionMode::isThorough()) 
	{
	  dummyComponent.reset(); // will also recycle dummyTmpLiteral!
	  destroyIndex();
	};
    };
  void destroyIndex();
  bool findName(TmpLitList& component,ulong& name);

  
  bool findName(const Flatterm* term,ulong& name)
    {
      CALL("findName(const Flatterm* term,ulong& name)");
      makeDummyLit(term);
      return findName(dummyComponent,name); 
    };
  

  void integrate(TmpLitList& component,ulong name)
    {
      CALL("integrate(TmpLitList& component,ulong name)");
      variantIndex.Integrate(component,(void*)name);
    };

  void integrate(const Flatterm* term,ulong name)
    {
      CALL("integrate(const Flatterm* term,ulong name)");
      makeDummyLit(term);
      integrate(dummyComponent,name);
    };

  bool nameFor(TmpLitList& component,ulong& name,bool& newName);

  bool nameFor(const Flatterm* term,ulong& name,bool& newName);

#ifndef NO_DEBUG_VIS 
  ostream& outputQuery(ostream& str) { return query.output(str); };
#endif

 private: 
  bool findName(TmpLitList& component,ulong numOfVars,ulong& name);
   
  bool findVariant(ulong& name);  
 
  void makeDummyLit(const Flatterm* term)
    {
      CALL("makeDummyLit(const Flatterm* term)");
      dummyTmpLiteral->reset();
      dummyTmpLiteral->pushNonpropHeader(TERM::dummyPositiveHeader1());
      dummyTmpLiteral->pushTerm(term); // very inefficient
      dummyTmpLiteral->endOfLiteral();
    };
 private:
  const Flatterm* subst[VampireKernelConst::MaxClauseSize]; 
  FS_CODE variantIndex;
  CN_QUERY query;
  TMP_LITERAL* dummyTmpLiteral;
  TmpLitList dummyComponent;

  ExpandingStack<GlobAlloc,CN_QUERY::LIT*,32UL,ComponentName> backtrack_lit;

  Stack<FS_COMMAND*,MAX_FS_CODE_DEPTH> backtrack_instr;
  Stack<const Flatterm*,MAX_FS_CODE_DEPTH> backtrack_cursor; 
}; // class ComponentName 
}; // namespace VK
//================================================
#undef DEBUG_NAMESPACE
//===============================================================
#endif
