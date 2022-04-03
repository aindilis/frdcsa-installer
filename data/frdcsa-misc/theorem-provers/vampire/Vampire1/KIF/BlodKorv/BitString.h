//
// File:         BitString.h
// Description:  
// Created:      Nov 7, 1999, 16:40
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//======================================================================
#ifndef BIT_STRING_H
//======================================================================
#define BIT_STRING_H 
#include <stddef.h>                                   // for size_t
#include <limits.h>                                   // for ULONG_MAX
#include "jargon.h"
#include "BlodKorvDebugFlags.h"
#include "BitWord.h" 
//=====================================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_BIT_STRING
 #define DEBUG_NAMESPACE "BitString<class Alloc>"
#endif
#include "debugMacros.h"
//=====================================================================
namespace BK 
{

template <class Alloc>
class BitString
{
 private:
  BitString() {};
 public:
  BitString(size_t s) : sz(s) {};
  ~BitString() {};
  void* operator new(size_t,size_t s) 
  {
   CALL("operator new(size_t,size_t s)");
   #ifdef DEBUG_ALLOC_OBJ_TYPE
   ALWAYS(_classDesc.registerAllocated(sizeof(BitString) + sizeof(BitWord)*s));
    return Alloc::allocate(sizeof(BitString) + sizeof(BitWord)*s,&_classDesc);
   #else
    return Alloc::allocate(sizeof(BitString) + sizeof(BitWord)*s);
   #endif
  };
  void operator delete(void* obj) 
  {
   CALL("operator delete(void* obj)");
   #ifdef DEBUG_ALLOC_OBJ_TYPE
   ALWAYS(_classDesc.registerDeallocated(sizeof(BitString) + sizeof(BitWord)*(((BitString*)obj)->size())));
    Alloc::deallocate(obj,sizeof(BitString) + sizeof(BitWord)*(((BitString*)obj)->size()),&_classDesc);
   #else
    Alloc::deallocate(obj,sizeof(BitString) + sizeof(BitWord)*(((BitString*)obj)->size()));
   #endif
  };
  size_t size() const { return sz; }; 
  static ulong minSize() { return sizeof(BitString); }; // has nothing to do with size()
  static ulong maxSize() { return ULONG_MAX; }; // has nothing to do with size()
  const BitWord& word(size_t wn) const { return *(begin() + wn); };
  BitWord& word(size_t wn) { return *(begin() + wn); };
  const BitWord* begin() const { return (const BitWord*)(this + 1); };  
  BitWord* begin() { return (BitWord*)(this + 1); }; 
 private:   
  void* operator new(size_t) { return (void*)0; };
  BitString(const BitString&) {};
  BitString& operator=(const BitString&) { return *this; };
 private:
  size_t sz;
  #ifdef DEBUG_ALLOC_OBJ_TYPE
   static ClassDesc _classDesc;  
  #endif
}; // template <class Alloc> class BitString

}; // namespace BK

//======================================================================
#undef DEBUG_NAMESPACE
//======================================================================
#endif
