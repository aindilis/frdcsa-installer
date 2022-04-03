//
// File:         ObjDesc.h
// Description:  Structure for object descriptors for debugging memory management. 
// Created:      Mar 3, 2000
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//====================================================
#ifndef NO_DEBUG
#ifndef OBJ_DESC_H
//====================================================
#define OBJ_DESC_H
#include <iostream.h>                
#include <stdlib.h> 
#include "jargon.h"
#include "BlodKorvDebugFlags.h"
#include "ClassDesc.h"
#include "Debug.h"
//====================================================
namespace BK 
{
struct ObjDesc 
{
 enum Status { Persistent = 12345678, Recycled = 87654321 };
 #ifdef DEBUG_ALLOC_OBJ_PTR
  void* objPtr;
 #endif
 #ifdef DEBUG_ALLOC_OBJ_SIZE
  size_t size;
 #endif   
 #ifdef DEBUG_ALLOC_OBJ_STATUS
  Status status;       
 #endif
 #ifdef DEBUG_ALLOC_OBJ_TYPE
  ClassDesc* classDesc;
 #endif
 #ifdef DEBUG_ALLOC_EVENT_ID
  ulong eventId;
 #endif
  ObjDesc() {};
  ~ObjDesc() {};
 void* operator new (size_t) 
 {
  void* res = ::malloc(sizeof(ObjDesc)); 
  if (!res) 
   Debug::debugFeatureInternalFault("!ObjDesc::operator new (size_t)::res");
  return res; 
 };
 void operator delete(void* obj,size_t) 
 {
  free(obj);
 }; 
}; // struct ObjDesc

inline ostream& operator << (ostream& str,const ObjDesc& od)
{
 #ifdef DEBUG_ALLOC_OBJ_PTR
  str << "[" << (unsigned long)od.objPtr << "] ";
 #endif
 #ifdef DEBUG_ALLOC_OBJ_SIZE
  str << "size(" << od.size << ") ";
 #endif   
 #ifdef DEBUG_ALLOC_OBJ_STATUS
  str << "status(";
  switch (od.status)
   {
    case ObjDesc::Persistent: str << "Persistent"; break;
    case ObjDesc::Recycled: str << "Recycled"; break;
    default: str << "Unknown(=" << (ulong)od.status << ")";
   };  
  #ifdef DEBUG_ALLOC_EVENT_ID
   str << ',' << od.eventId;  
  #endif
  str << ") ";
 #endif
 #ifdef DEBUG_ALLOC_OBJ_TYPE
  str << ":" << *od.classDesc;
 #endif
 
 return str;
}; 
}; // namespace BK

//====================================================
#endif
#endif

