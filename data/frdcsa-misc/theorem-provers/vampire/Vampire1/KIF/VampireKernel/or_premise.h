#ifndef OR_PREMISE_H
//=================================================================
#define OR_PREMISE_H
#ifndef NO_DEBUG_VIS 
 #include <iostream.h>
#endif
#include "jargon.h"
#include "VampireKernelDebugFlags.h"
//#include "VampireKernelConst.h"
#include "Clause.h"
#include "prefix.h" 
#include "or_index.h"
#include "NewClause.h"
#include "Array.h"
#include "GlobAlloc.h"
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_OR_PREMISE
 #define DEBUG_NAMESPACE "OR_PREMISE"
#endif
#include "debugMacros.h"
//=================================================
namespace VK
{

class OR_PREMISE
 {
  public: // methods
   OR_PREMISE(); 
   void init();
   OR_PREMISE(NewClause* res); 
   void init(NewClause* res); 
   ~OR_PREMISE() { CALL("destructor ~OR_PREMISE()"); };
   void setResolvent(NewClause* res) { _resolvent = res; };

   PREFIX_SYM* nthMaxLit(ulong n) 
     {
       CALL("nthMaxLit(ulong n)");
       ASSERT(n <= last_max);
       return _maxLiteral[n]; 
     };
   const Clause* TheClause() const { return clause; };

   ulong LastMax() { return last_max; };
   ulong Index() { return index; };
   ulong CutLit() { return cut_lit; };

   void Load(ulong ind,const Clause* c);
   void LoadOptimized(ulong ind,const Clause* c);

   void Unload() 
   {
   };

   void Cut(ulong lit) 
    {
     CALL("Cut(ulong lit)");
     cut_lit = lit;
     _litIsMarked[cut_lit] = true;
    };
   void Load(ulong ind,RES_PAIR_LIST* res_pair)
    {
     CALL("Load(ulong ind,RES_PAIR_LIST* res_pair)");
     Load(ind,res_pair->TheClause());
     Cut((ulong)(res_pair->LitNum()));
    };
   void LoadOptimized(ulong ind,RES_PAIR_LIST* res_pair)
    {
     CALL("LoadOptimized(ulong ind,RES_PAIR_LIST* res_pair)");
     LoadOptimized(ind,res_pair->TheClause());
     Cut((ulong)(res_pair->LitNum()));
    };
   void Cut(TERM* lit) { Cut(clause->NumberOfLiteral(lit)); };
   void UnmarkCut() 
   { 
    CALL("UnmarkCut()");
    _litIsMarked[cut_lit] = false; 
   };
   bool CollectNonmarkedLiterals();
   bool NonmarkedExist() { return nonmarkedExist; };
  public: // output for debugging
   #ifndef NO_DEBUG_VIS  
    ostream& output(ostream& str) const;
   #endif

  protected: // structure
   NewClause* _resolvent;
   const Clause* clause;
   ulong index;
   
   Array<GlobAlloc,PREFIX_SYM,256UL,OR_PREMISE> _termMemory;
   PREFIX_SYM* _freeTermMemory;
   
   Array<GlobAlloc,PREFIX_SYM*,32UL,OR_PREMISE> _maxLiteral;

   Array<GlobAlloc,bool,32UL,OR_PREMISE> _litIsInherentlySelected;

   ulong last_max;

   Array<GlobAlloc,bool,32UL,OR_PREMISE> _litIsMarked;
   
   LiteralList* _allLiterals;
   LiteralList::Element* _firstNonselectedLit;

   ulong cut_lit;
   bool nonmarkedExist;

 }; // class OR_PREMISE
 
//******************* Definitions: *************************************

inline bool OR_PREMISE::CollectNonmarkedLiterals()
 {
  CALL("CollectNonmarkedLiterals()");
  ulong currMax = 0;

  nonmarkedExist = (bool)_firstNonselectedLit;   

  while (currMax <= last_max)
   {   
    if (!(_litIsMarked[currMax])) 
     {
      nonmarkedExist = true;
      if (!(_resolvent->PushPrefixLit(_maxLiteral[currMax],_litIsInherentlySelected[currMax])))
       {
        return false; 
       };
     };
    currMax++;
   };
  
   
  for(LiteralList::Iterator iter(_allLiterals,_firstNonselectedLit);iter.notEnd();iter.next())     
   if (!(_resolvent->PushStandardLit(index,iter.currentLiteral(),iter.currentElement()->isInherentlySelected()))) return false;
  
  return true;
 }; // inline bool OR_PREMISE::CollectNonmarkedLiterals()

}; // namespace VK


//================================================
#undef DEBUG_NAMESPACE
//=================================================================
#endif
