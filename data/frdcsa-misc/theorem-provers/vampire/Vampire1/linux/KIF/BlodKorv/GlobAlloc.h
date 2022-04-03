//
// File:         GlobAlloc.h
// Description:  Memory allocation front-end with debugging facilities.
// Created:      Mar 2, 2000
// Revised:      Nov 28, 2001. 
//               1) Some debugging stuff has been transferred to _allocator. 
//               2) allocateLarge and deallocateLarge are gone.
// Author:       Alexandre Riazanov 
// mail:         riazanov@cs.man.ac.uk  
//====================================================
#ifndef GLOB_ALLOC_H
//====================================================
#define GLOB_ALLOC_H   
#include <stdlib.h>
#include "jargon.h"
#include "BlodKorvDebugFlags.h"  
#include "ClassDesc.h"
#include "ObjDesc.h"
#include "AllocationFailureHandling.h"
#include "Allocator.h"
#include "DestructionMode.h"
//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_ALLOC
 #define DEBUG_NAMESPACE "GlobAlloc"
#endif 
#include "debugMacros.h"
//=================================================

#define GLOB_ALLOC_CHUNK_SIZE 16000UL
#define GLOB_ALLOC_MAX_OBJECT_SIZE 1023UL
// ^ in machine words

namespace BK 
{

class GlobAlloc  
{
 public:
  class AllocationFailureHandlingId {};
  typedef AllocationFailureHandling<AllocationFailureHandlingId> AllocationFailureHandling;
 public:
  class InitStatic
  {
   public: 
    InitStatic();
    ~InitStatic();
   private:
    static long _count;
  };
 public:
  static void reset();
  static AllocationFailureHandling& basicAllocationFailureHandling() 
  { 
   return _basicAllocationFailureHandling;
  }; 
  static void setHardMemoryLimit(long lim)
  { 
   CALL("setHardMemoryLimit(long lim)");
   _allocator.setHardMemoryLimit(lim/(sizeof(void*)));
  };  
  static void setAllocationBufferSize(long abs) 
  { 
   _allocator.setBufferSize(abs);  
  };
  static long occupiedByObjects() { return sizeof(void*)*_allocator.occupiedByObjects(); };
  static long occupiedByChunks() { return sizeof(void*)*_allocator.occupiedByChunks(); }; 
  static long freeInTheCurrentChunk() { return sizeof(void*)*_allocator.freeInTheCurrentChunk(); };
  static long totalBufferDeficit() { return sizeof(void*)*_allocator.totalBufferDeficit(); };
  static long activeBufferDeficit() { return sizeof(void*)*_allocator.activeBufferDeficit(); };
  static void annulBufferDeficit() { _allocator.annulBufferDeficit(); };

  static void* allocate(size_t size);
  static void* tryToAllocate(size_t size); 
  static void deallocate(void* obj,size_t size); 
#ifndef NO_DEBUG
  static void* allocate(size_t size,ClassDesc* classDesc); 
  static void* tryToAllocate(size_t size,ClassDesc* classDesc); 
  static void deallocate(void* obj,size_t size,ClassDesc* classDesc); 
#endif

  #ifdef DEBUG_ALLOC
   static ObjDesc* hostObj(void* ptr) 
   { 
    return *((ObjDesc**)(_allocator.mirror(ptr)));
   };
  #endif

#ifndef NO_DEBUG
   static ostream& outputInternalStatistics(ostream& str)
     {
       return _allocator.outputInternalStatistics(str);
     };
#endif

#ifdef DEBUG_ALLOC_OBJ_STATUS
  static ostream& outputPersistent(ostream& str,ulong maxNumOfObjects)
    {
      return _allocator.outputPersistent(str,maxNumOfObjects);
    };
#endif


 //private:   
 public: // made public to please VC++
  typedef Allocator<AllocationFailureHandling,GLOB_ALLOC_MAX_OBJECT_SIZE,GLOB_ALLOC_CHUNK_SIZE> Alloc;
 private:
  static Alloc _allocator;
  static AllocationFailureHandling _basicAllocationFailureHandling; 
 friend class InitStatic;
}; // class GlobAlloc


static GlobAlloc::InitStatic globAllocInitStatic;


}; // namespace BK

//====================================================
#undef DEBUG_NAMESPACE
#undef GLOB_ALLOC_CHUNK_SIZE
#undef GLOB_ALLOC_MAX_OBJECT_SIZE 
//====================================================
#endif
 
