//
// File:         GlobAlloc.cpp
// Description:  Memory allocation front-end with debugging facilities.
// Created:      Mar 2, 2000
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//==================================================== 
#include "GlobAlloc.h"
//====================================================

using namespace BK;

long GlobAlloc::InitStatic::_count; // implicitely initialised by 0L
GlobAlloc::Alloc GlobAlloc::_allocator;

GlobAlloc::AllocationFailureHandling* GlobAlloc::AllocationFailureHandling::_currentHandler;

GlobAlloc::AllocationFailureHandling GlobAlloc::_basicAllocationFailureHandling;
 



//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_ALLOC
#define DEBUG_NAMESPACE "GlobAlloc::InitStatic"
#endif 
#include "debugMacros.h"
//=================================================


GlobAlloc::InitStatic::InitStatic()
{ 
  CALL("constructor InitStatic()");
  if (!_count) 
    {
      GlobAlloc::_allocator.init(); 
      GlobAlloc::AllocationFailureHandling::initStatic();
      GlobAlloc::_basicAllocationFailureHandling.activate();
    };  
  _count++;
}; // GlobAlloc::InitStatic::InitStatic()

GlobAlloc::InitStatic::~InitStatic() 
{
  CALL("destructor ~InitStatic() ");
  _count--;
  if (!_count)
    {
      if (DestructionMode::isThorough())
	{
#ifdef DEBUG_MEMORY_LEAKS
	  if (GlobAlloc::occupiedByObjects())
	    DWARN("") << "Memory leak: " 
		      << GlobAlloc::occupiedByObjects() << " bytes lost.\n"; 

	  //DF; ClassDesc::outputAll(cout,true) << "\n";
	  //DF; GlobAlloc::outputInternalStatistics(cout) << "\n";
	  DF; GlobAlloc::outputPersistent(cout,ULONG_MAX) << "\n";
#endif
	};
    };
}; // GlobAlloc::InitStatic::~InitStatic() 

//================================================
#undef DEBUG_NAMESPACE
#ifdef DEBUG_ALLOC
#define DEBUG_NAMESPACE "GlobAlloc"
#endif 
#include "debugMacros.h"
//=================================================

void GlobAlloc::reset()
{
  CALL("reset()");
  _allocator.reset();
}; // void GlobAlloc::reset()


void* GlobAlloc::allocate(size_t size) 
{ 
  CALL("allocate(size_t size)"); 
  ulong nsize = size/sizeof(void*);
  if (size % sizeof(void*)) nsize++;

#ifndef DEBUG_ALLOC
  return _allocator.allocate(nsize);
#else
  void* res = _allocator.allocate(nsize);
  if (!res) return (void*)0;
  void** resMirror = (void**)(_allocator.mirror(res));
  ObjDesc* objDesc = (ObjDesc*)(*resMirror);
  ASSERT(objDesc);
#ifdef DEBUG_ALLOC_OBJ_PTR  
  ASSERT(objDesc->objPtr == res);       
#endif
#ifdef DEBUG_ALLOC_OBJ_STATUS
  ASSERT(objDesc->status = ObjDesc::Persistent);
#endif  
#ifdef DEBUG_ALLOC_OBJ_SIZE
  objDesc->size = size;
#endif
#ifdef DEBUG_ALLOC_OBJ_TYPE
  objDesc->classDesc = ClassDesc::universalClassDesc();
#endif

  /********
  DF; if (((long)res) == 1142755672L) 
    {
      cout << "AGGGAAA\n";
      REPCPH;
    };
  ********/

  return res;
#endif
}; // void* GlobAlloc::allocate(size_t size) 

void* GlobAlloc::tryToAllocate(size_t size) 
{ 
  CALL("tryToAllocate(size_t size)"); 
  ulong nsize = size/sizeof(void*);
  if (size % sizeof(void*)) nsize++;

#ifndef DEBUG_ALLOC
  return _allocator.tryToAllocate(nsize);
#else
  void* res = _allocator.tryToAllocate(nsize);
  if (!res) return (void*)0;
  void** resMirror = (void**)(_allocator.mirror(res));
  ObjDesc* objDesc = (ObjDesc*)(*resMirror);
  ASSERT(objDesc);
#ifdef DEBUG_ALLOC_OBJ_PTR  
  ASSERT(objDesc->objPtr == res);       
#endif
#ifdef DEBUG_ALLOC_OBJ_STATUS
  ASSERT(objDesc->status = ObjDesc::Persistent);
#endif  
#ifdef DEBUG_ALLOC_OBJ_SIZE
  objDesc->size = size;
#endif
#ifdef DEBUG_ALLOC_OBJ_TYPE
  objDesc->classDesc = ClassDesc::universalClassDesc();
#endif

  return res;
#endif
}; // void* GlobAlloc::tryToAllocate(size_t size) 

void GlobAlloc::deallocate(void* obj,size_t size) 
{
  CALL("deallocate(void* obj,size_t size)");
  ulong nsize = size/sizeof(void*);
  if (size % sizeof(void*)) nsize++;
  ASSERT(_allocator.chunk(obj)); 
#ifdef DEBUG_ALLOC           
  ObjDesc* objDesc = *((ObjDesc**)(_allocator.mirror(obj))); 
  ASSERT(objDesc);    
#ifdef DEBUG_ALLOC_OBJ_PTR
  ASSERT(objDesc->objPtr == obj); 
#endif
#ifdef DEBUG_ALLOC_OBJ_STATUS
  ASSERT(objDesc->status == ObjDesc::Persistent); 
#endif
#ifdef DEBUG_ALLOC_OBJ_TYPE
  ASSERT(objDesc->classDesc == ClassDesc::universalClassDesc());
#endif    
#ifdef DEBUG_ALLOC_OBJ_SIZE
  if (objDesc->size != size) 
    { 
      DMSG("size = ") << size << "\nobjDesc:\n" << *objDesc << '\n';
      ASSERT(objDesc->size == size);
    };
#endif
#endif    
  _allocator.deallocate(obj,nsize);
}; //void GlobAlloc::deallocate(void* obj,size_t size)  

#ifndef NO_DEBUG
void* GlobAlloc::allocate(size_t size,ClassDesc* classDesc) 
{ 
  CALL("allocate(size_t size,ClassDesc* classDesc)");
  ulong nsize = size/sizeof(void*);
  if (size % sizeof(void*)) nsize++;
#ifndef DEBUG_ALLOC
  return _allocator.allocate(nsize);
#else
  void* res = _allocator.allocate(nsize);
  void** resMirror = (void**)(_allocator.mirror(res));
  ObjDesc* objDesc = (ObjDesc*)(*resMirror);
  ASSERT(objDesc);
#ifdef DEBUG_ALLOC_OBJ_PTR  
  ASSERT(objDesc->objPtr == res);       
#endif
#ifdef DEBUG_ALLOC_OBJ_SIZE
  objDesc->size = size;
#endif
#ifdef DEBUG_ALLOC_OBJ_STATUS
  ASSERT(objDesc->status = ObjDesc::Persistent);
#endif
#ifdef DEBUG_ALLOC_OBJ_TYPE
  objDesc->classDesc = classDesc;
#endif
  /******
  DF; if (((long)res) == 1143940824L) 
    {
      cout << "AGGGAAA\n";
      REPCPH;
    };
  *******/
  return res;  

#endif
}; // void* GlobAlloc::allocate(size_t size,ClassDesc* classDesc) 


void* GlobAlloc::tryToAllocate(size_t size,ClassDesc* classDesc) 
{ 
  CALL("tryToAllocate(size_t size,ClassDesc* classDesc)");
  ulong nsize = size/sizeof(void*);
  if (size % sizeof(void*)) nsize++;
#ifndef DEBUG_ALLOC
  return _allocator.tryToAllocate(nsize);
#else
  void* res = _allocator.tryToAllocate(nsize);
  void** resMirror = (void**)(_allocator.mirror(res));
  ObjDesc* objDesc = (ObjDesc*)(*resMirror);
  ASSERT(objDesc);
#ifdef DEBUG_ALLOC_OBJ_PTR  
  ASSERT(objDesc->objPtr == res);       
#endif
#ifdef DEBUG_ALLOC_OBJ_SIZE
  objDesc->size = size;
#endif
#ifdef DEBUG_ALLOC_OBJ_STATUS
  ASSERT(objDesc->status = ObjDesc::Persistent);
#endif
#ifdef DEBUG_ALLOC_OBJ_TYPE
  objDesc->classDesc = classDesc;
#endif

  return res;  

#endif
}; // void* GlobAlloc::tryToAllocate(size_t size,ClassDesc* classDesc) 



 
void GlobAlloc::deallocate(void* obj,size_t size,ClassDesc* classDesc) 
{
  CALL("deallocate(void* obj,size_t size,ClassDesc* classDesc)"); 
  ulong nsize = size/sizeof(void*);
  if (size % sizeof(void*)) nsize++;
  ASSERT(_allocator.chunk(obj)); 
#ifdef DEBUG_ALLOC
  ObjDesc* objDesc = *((ObjDesc**)(_allocator.mirror(obj))); 
  ASSERT(objDesc);
#ifdef DEBUG_ALLOC_OBJ_PTR
  ASSERT(objDesc->objPtr == obj); 
#endif
#ifdef DEBUG_ALLOC_OBJ_STATUS
  if (objDesc->status != ObjDesc::Persistent)
    { 
      DMSG("objDesc:\n") << *objDesc << '\n';
      ASSERT(objDesc->status == ObjDesc::Persistent); 
    };     
#endif
#ifdef DEBUG_ALLOC_OBJ_TYPE
  ASSERT(objDesc->classDesc == classDesc);
#endif
#ifdef DEBUG_ALLOC_OBJ_SIZE
  if (objDesc->size != size) 
    { 
      DMSG("size = ") << size << "\nobjDesc:\n" << *objDesc << '\n';
      ASSERT(objDesc->size == size);
    }; 
#endif
#endif
  _allocator.deallocate(obj,nsize);
}; // void GlobAlloc::deallocate(void* obj,size_t size,ClassDesc* classDesc) 
#endif 



//====================================================

