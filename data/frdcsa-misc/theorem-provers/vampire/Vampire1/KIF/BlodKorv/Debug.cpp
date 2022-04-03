//
// File:         Debug.cpp
// Description:   
// Created:      Oct 1, 1999, Alexandre Riazanov, riazanov@cs.man.ac.uk
// Revised:      Jan 17, 2002, Alexandre Riazanov, riazanov@cs.man.ac.uk
//               1) class InitStatic added instead of the old 
//               scheme for initilising the static part. 
//               2) nextEvent() made hookable from outside.  
//==============================================================================
#ifndef NO_DEBUG
//==============================================================================
#include <iostream.h>
#include <signal.h>
#include <limits.h>
#include "Debug.h"
#include "Exit.h"
//==============================================================================
using namespace BK;

long Debug::InitStatic::_count; // Implicitely initialised to 0L
const char* Debug::job;  
ulong Debug::eventId;
const char* Debug::controlPoint;
const char* Debug::callName;
long Debug::callDepth;
const char* Debug::objectName;
const char* Debug::nameSpace;
const char* Debug::errorDescription; 
Debug::StrList* Debug::controlPoints;
Debug::StrList* Debug::callNames;
Debug::StrList* Debug::objectNames;
Debug::StrList* Debug::nameSpaces;
Debug::StrList* Debug::recycledSL;
ostream* Debug::trace;
bool Debug::traceCP;
bool Debug::traceAfterError;
void (*Debug::nextEventHook)(ulong);
void (*Debug::errorHook)() = 0;


Debug::Call::Call(const char* nameSp,const char* callNm)
{
 callDepth++;
 controlPoints = newStrList(controlPoint,controlPoints);  
 callNames = newStrList(callName,callNames);
 objectNames = newStrList(objectName,objectNames);
 nameSpaces = newStrList(nameSpace,nameSpaces);
 controlPoint = "start";
 callName = callNm;
 objectName = "?";
 nameSpace = nameSp;
 nextEvent();
 if (trace)
  {
   *trace << eventId << " -> [" << callDepth << "] " << nameSpace << "::" << callName 
          << " [" << objectName << "] from " << controlPoints->hd << '\n'; 
  };
}; // Debug::Call::Call(const char* nameSp,const char* callNm)

Debug::Call::Call(const char* nameSp,const char* objName,const char* callNm)
{ 
 callDepth++; 
 controlPoints = newStrList(controlPoint,controlPoints);  
 callNames = newStrList(callName,callNames);
 objectNames = newStrList(objectName,objectNames);
 nameSpaces = newStrList(nameSpace,nameSpaces);
 controlPoint = "start";
 callName = callNm;
 objectName = objName;
 nameSpace = nameSp;
 nextEvent();
 if (trace)
  {
   *trace << eventId << " -> [" << callDepth << "] " << nameSpace << "::" << callName 
          << " [" << objectName << "] from " << controlPoints->hd << '\n'; 
  };
}; // Debug::Call::Call(const char* nameSp,const char* objName,const char* callNm)

Debug::Call::~Call()
{
 const char* prevControlPoint = controlPoint;
 controlPoint = "return";
 nextEvent();
 StrList* tmp;
 if (trace)
  {
   *trace << eventId  << " <- [" << callDepth << "] " << nameSpace << "::" << callName 
          << " [" << objectName << "] after " << prevControlPoint << '\n'; 
  };
 
 // popping control point
 controlPoint = controlPoints->hd;
 tmp = controlPoints->tl;
 recycle(controlPoints);
 controlPoints = tmp; 

 // popping call name
 callName = callNames->hd;
 tmp = callNames->tl;
 recycle(callNames);
 callNames = tmp; 

 // popping object names
 objectName = objectNames->hd;
 tmp = objectNames->tl;
 recycle(objectNames);
 objectNames = tmp; 

 // popping name space
 nameSpace = nameSpaces->hd;
 tmp = nameSpaces->tl;
 recycle(nameSpaces);
 nameSpaces = tmp;

 callDepth--;   
}; // Debug::Call::~Call() 


void Debug::initStatic()
{
 #ifdef DEBUG_ERROR_SIGNALS
  initSigHandlers();
 #endif
 callDepth = 0;
 job = "unknown job";
 eventId = 0;
 controlPoint = "start";
 callName = "main";
 objectName = "main";
 nameSpace = "main";
 errorDescription = "unknown";
 controlPoints = (StrList*)0;
 callNames = (StrList*)0;
 objectNames = (StrList*)0;
 nameSpaces = (StrList*)0;
 recycledSL = (StrList*)0;
 trace = (ostream*)0;
 traceCP = false;   
 traceAfterError = false;
 nextEventHook = 0;
 errorHook = 0;
};

void Debug::reportCP(ulong ev,const char* ns,const char* cn,const char* cp,const char* on)
{
 cout << ev << " " << ns << "::" << cn << '/' << cp << " [" << on << ']';
};

void Debug::error()
{
 cout << "Job " << job << " aborted.\n"; 
 cout << "Error : " << errorDescription << "\n in ";
 reportCP(eventId,nameSpace,callName,controlPoint,objectName);
 cout << '\n';
 history(); 
 cout << "The job was " << job << '\n';
 if (!traceAfterError) tracingOff();
 if (errorHook) errorHook();
 Exit::exit(1);
};

void Debug::error(const char* msg)
{
 errorDescription = msg;
 error();
};

void Debug::debugFeatureInternalFault(const char* msg)
{
 cout << "Job " << job << " aborted.\n"; 
 cout << "Error : debug feature internal fault : " << msg << "\n in ";
 reportCP(eventId,nameSpace,callName,controlPoint,objectName);
 cout << '\n';
 history(); 
 cout << "The job was " << job << '\n';
 if (!traceAfterError) tracingOff();
 Exit::exit(1);
};

void Debug::reportCP()
{
 reportCP(eventId,nameSpace,callName,controlPoint,objectName); 
 cout << '\n';
};

void Debug::history()
{
 StrList* cpl = controlPoints;
 StrList* cnl = callNames;
 StrList* onl = objectNames;
 StrList* nsl = nameSpaces;
 int indent = 0;
 while (cpl)
  {
   for (int i = 0; i < indent; i++) cout << ' ';
   cout << "|_ ";
   reportCP(0,nsl->hd,cnl->hd,cpl->hd,onl->hd);
   cout << "\n";
   indent++;
   cpl = cpl->tl; 
   cnl = cnl->tl;
   onl = onl->tl;
   nsl = nsl->tl;
  };                 
}; // void Debug::history()

void Debug::reportSlowProgress(const char* checkName,float maxInt,float realInt)
{
  cout << "Debug warning: slow progress in " << checkName 
       << ", max. interval = " << maxInt << ", real interval = " << realInt << '\n';  
}; // void Debug::reportSlowProgress(const char* checkName,float maxInt,float realInt)


void Debug::initSigHandlers()
{
 #ifdef TARGET_OS_TYPE 	 
  #if (TARGET_OS_TYPE == OS_TYPE_UNIX)
   signal(SIGFPE,handleSIGFPE);
   signal(SIGILL,handleSIGILL);
   signal(SIGSEGV,handleSIGSEGV);
   signal(SIGBUS,handleSIGBUS);  
   signal(SIGTRAP,handleSIGTRAP);
   signal(SIGABRT,handleSIGABRT);
  #elif (TARGET_OS_TYPE == OS_TYPE_WIN32)
   signal(SIGFPE,handleSIGFPE);
   signal(SIGILL,handleSIGILL);
   signal(SIGSEGV,handleSIGSEGV); 
  #else
   #error "Unsupported target OS type, check TARGET_OS_TYPE"
  #endif
 #else
  #error "Undefined TARGET_OS_TYPE"
 #endif
};
                
void Debug::handleSIGFPE(int)
{
 error("Program Error Signal SIGFPE (arithmetic error)");
};
void Debug::handleSIGILL(int)
{
 error("Program Error Signal SIGILL (illegal instruction)");
};
void Debug::handleSIGSEGV(int)
{
 error("Program Error Signal SIGSEGV (segmentation violation)");
};
void Debug::handleSIGBUS(int) 
{
 error("Program Error Signal SIGBUS (bus error)");
};
void Debug::handleSIGABRT(int)
{
 error("Program Error Signal SIGABRT");
};
void Debug::handleSIGTRAP(int)
{
 error("Program Error Signal SIGTRAP");
};


//=========================================================================
#endif
