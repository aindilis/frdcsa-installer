//
// File:         GMap.h
// Description:  Simple generic finite mappings on ordered domains.
// Created:      Oct 08, 2000, 17:10
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//============================================================================
#ifndef G_MAP_H
//=============================================================================
#define G_MAP_H 
#include "jargon.h"
#include "BlodKorvDebugFlags.h"
#include "GSortTree.h" 
#include "DestructionMode.h"
//============================================================================
#ifdef DEBUG_G_MAP
 #define DEBUG_NAMESPACE "GMap<class Alloc,class DomType,class RangeType>"
#else 
 #undef DEBUG_NAMESPACE
#endif
#include "debugMacros.h"
//===========================================================================
namespace BK 
{

template <class Alloc,class DomType,class RangeType>
class GMap
{
 public: 
  GMap() { init(); };
  void init()
  {
   #ifdef DEBUG_G_MAP
    _name = "?";
   #endif
  };
  GMap(const char* nm)  { init(nm); };
  void init(const char* nm) 
  {
   #ifdef DEBUG_G_MAP
    _name = nm;
   #endif  
  };
  GMap(const RangeType& def) { init(def); };
  void init(const RangeType& def)
  { 
    _sortTree = def; 
   #ifdef DEBUG_G_MAP
    _name = "?";
   #endif
  };
  GMap(const RangeType& def,const char* nm) { init(def,nm); };
  void init(const RangeType& def,const char* nm)
  {
    _sortTree = def;
#ifdef DEBUG_G_MAP
    _name = nm;
#endif
  }; 
  ~GMap() 
    { 
      CALLM(_name,"destructor ~GMap()"); 
      if (DestructionMode::isThorough()) destroyThoroughly(); 
    }; 
  void destroyThoroughly() 
    { 
      CALLM(_name,"destroy()"); 
      _sortTree.destroyThoroughly();
      init();
    };
  void setDefVal(const RangeType& def) { defVal() = def; _sortTree.setDefInfo(def); };
  bool map(const DomType& x,RangeType& y) const
  {
   CALLM(_name,"map(const DomType& x,RangeType& y) const");
   SortTree::Node* nd = _sortTree.node(x);
   if (nd) y = nd->info();
   return nd;    
  };

  RangeType& map(const DomType& x)
  {
   CALLM(_name,"map(const DomType& x)");
   return _sortTree.info(x); 
  };   

 private:  
  typedef GMSortTree<Alloc,DomType,RangeType> SortTree;
 public:
  class LeftRightIter
  {
   public:
    LeftRightIter() : _treeIter() {};
    LeftRightIter(const GMap& m) : _treeIter(m._sortTree) {};
    ~LeftRightIter() { CALL("LeftRightIter::destructor ~LeftRightIter()"); };
    void reset(const GMap& m) { _treeIter.reset(m._sortTree); };
    operator bool() const { return (bool)_treeIter; };
    void next() { _treeIter.next(); };
    const DomType& xVal() const { return _treeIter.value(); }; 
    const RangeType& yVal() const { return _treeIter.info(); };     
   private: 
    SortTree::LeftRightIter _treeIter;  
  };
 private:
  RangeType& defVal() { return *((RangeType*)_defVal); };
  const RangeType& defVal() const { return &((const RangeType*)_defVal); };
 private:
  char _defVal[sizeof(RangeType)];
  SortTree _sortTree;
  #ifdef DEBUG_G_MAP
   const char* _name;
  #endif
 friend class LeftRightIter;
}; // template <class Alloc,class DomType,class RangeType> class GMap


}; // namespace BK

//======================================================================
#undef DEBUG_NAMESPACE
//======================================================================
#endif
