//
// File:         VampireKernelInterceptMacros.h
// Description:  Macros for intercepting some operations from the library.
// Created:      Dec 05, 2000, 14:00
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//====================================================
#ifndef HELL_INTERCEPT_MACROS_H  
//====================================================
#define HELL_INTERCEPT_MACROS_H 
#include <iostream.h> 
#include "Exit.h"
#include "Signature.h"
//====================================================   

// Indexing operations for forward matching:     

#ifdef INTERCEPT_FORWARD_MATCHING_INDEXING
                   
#include "Logging.h"

extern Logging FMILog; // declared in VampireKernelInterceptMacros.cpp
extern const Flatterm* interceptedForwardMatchingQuery;        
extern long maxSizeOfFMILogFile; 
inline void checkMaxSizeOfFMILogFile()
{
  if (FMILog.counter() >= maxSizeOfFMILogFile) 
    { 
      cout << "%    Forced termination: file specified by --fmilog is full.\n"; 
      Exit::exit(0); 
    }; 
};
#define INTERCEPT_FORWARD_MATCHING_INDEXING_INTEGRATE(term) \
  { \
   FMILog << "+" << Logging::NormalisedTerm(term) << "\n"; \
   FMILog.flush(); \
   checkMaxSizeOfFMILogFile(); \
  }

#define INTERCEPT_FORWARD_MATCHING_INDEXING_REMOVE(term) \
  { \
   FMILog << "-" << Logging::NormalisedTerm(term) << "\n"; \
   FMILog.flush(); \
   checkMaxSizeOfFMILogFile(); \
  }

#define INTERCEPT_FORWARD_MATCHING_INDEXING_RETRIEVAL_QUERY(term) \
  ( \
   (interceptedForwardMatchingQuery=term), \
   term \
  )

#define INTERCEPT_FORWARD_MATCHING_INDEXING_RETRIEVAL_RESULT(res)\
  ( \
     (\
      (interceptedForwardMatchingQuery) ? \
       ( \
        (res) ? \
	 ( \
          (FMILog << "!" << interceptedForwardMatchingQuery << "\n"), \
          interceptedForwardMatchingQuery = (const Flatterm*)0, \
          FMILog.flush(), \
          checkMaxSizeOfFMILogFile(), \
          res \
         ) \
	 : \
	 ( \
          (FMILog << "?" << interceptedForwardMatchingQuery << "\n"), \
          interceptedForwardMatchingQuery = (const Flatterm*)0, \
          FMILog.flush(), \
          checkMaxSizeOfFMILogFile(), \
          res \
         ) \
       ) \
       : \
       res \
     ) \
  )
#endif


// Undefining the macros in the case an efficient version of Vampire is built  

#ifndef INTERCEPT_FORWARD_MATCHING_INDEXING

#undef INTERCEPT_FORWARD_MATCHING_INDEXING_INTEGRATE
#define INTERCEPT_FORWARD_MATCHING_INDEXING_INTEGRATE(term) 
 
#undef INTERCEPT_FORWARD_MATCHING_INDEXING_REMOVE
#define INTERCEPT_FORWARD_MATCHING_INDEXING_REMOVE(term)

#undef INTERCEPT_FORWARD_MATCHING_INDEXING_RETRIEVAL_QUERY  
#define INTERCEPT_FORWARD_MATCHING_INDEXING_RETRIEVAL_QUERY(term) term

#undef INTERCEPT_FORWARD_MATCHING_INDEXING_RETRIEVAL_RESULT
#define INTERCEPT_FORWARD_MATCHING_INDEXING_RETRIEVAL_RESULT(res) res
 
#endif

// Indexing operations for backward subsumption:
    
#ifdef INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING
                   
#include "Logging.h"

#ifdef INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_WITH_COMMENTS
#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_COMMENT(com) com
#else
#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_COMMENT(com) 
#endif
 
extern Logging BSILog; // defined in VampireKernelInterceptMacros.cpp
extern const Clause* interceptedBackwardSubsumptionQuery;
extern ulong interceptedBackwardSubsumptionResultTotal;       
extern long maxSizeOfBSILogFile; 
extern ulong interceptedBackwardSubsumptionNextSymbolToDeclare;
inline void checkMaxSizeOfBSILogFile()
{
  if (BSILog.counter() >= maxSizeOfBSILogFile) 
    { 
      cout << "%    Forced termination: file specified by --bsilog is full.\n"; 
      INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_COMMENT(BSILog << "// end of file\n");
      Exit::exit(0); 
    }; 
};


#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_SIGNATURE_UPDATE \
  { \
    BSILog.outputSignatureUpdate(*Signature::current(), \
                                 interceptedBackwardSubsumptionNextSymbolToDeclare); \
  }


#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_INTEGRATE(clause) \
  { \
   BSILog << '+' << '[' << Logging::Integer32(clause->Number()) << ']'; \
   INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_COMMENT(\
     BSILog << " // " << Logging::StdClause(clause)); \
   BSILog << '\n'; \
   BSILog.flush(); \
   checkMaxSizeOfBSILogFile(); \
  }

#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_REMOVE(clause) \
  { \
   BSILog << '-' << '[' << Logging::Integer32(clause->Number()) << ']'; \
   INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_COMMENT(\
     BSILog << " // " << Logging::StdClause(clause)); \
   BSILog << '\n'; \
   BSILog.flush(); \
   checkMaxSizeOfBSILogFile(); \
  }

#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_RETRIEVAL_QUERY(clause) \
  { \
   INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_SIGNATURE_UPDATE; \
   interceptedBackwardSubsumptionQuery=clause; \
   interceptedBackwardSubsumptionResultTotal = 0UL; \
   BSILog << '?' << '[' << Logging::Integer32(clause->Number()) << ']'; \
   INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_COMMENT(\
     BSILog << " // " << Logging::StdClause(clause)); \
   BSILog << '\n'; \
   BSILog.flush(); \
   checkMaxSizeOfBSILogFile(); \
  }

#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_RETRIEVAL_RESULT(clause) \
  { \
   interceptedBackwardSubsumptionResultTotal++; \
   BSILog << '!' << '[' << Logging::Integer32(clause->Number()) << ']'; \
   INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_COMMENT(\
     BSILog << "\n // " << Logging::StdClause(clause)); \
   BSILog << '\n'; \
   BSILog.flush(); \
   checkMaxSizeOfBSILogFile(); \
  }

#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_RETRIEVAL_TOTAL \
  { \
   BSILog << "$" << interceptedBackwardSubsumptionResultTotal << " \n"; \
   BSILog.flush(); \
   checkMaxSizeOfBSILogFile(); \
  } 

#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_INTRODUCE_TERM_ID(term) \
  { \
    INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_SIGNATURE_UPDATE; \
    if (term->arity()) \
     { \
       BSILog << "#[" << Logging::Integer32(term) << ']' << term; \
       INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_COMMENT(\
         BSILog << " // " \
                << Logging::StdTerm(term,*Signature::current())); \
       BSILog << '\n'; \
       BSILog.flush(); \
       checkMaxSizeOfBSILogFile(); \
     } \
  } 


#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_INTRODUCE_LITERAL_ID(literal) \
  { \
    INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_SIGNATURE_UPDATE; \
    if (literal->arity()) \
      { \
        BSILog << "*[" << Logging::Integer32(literal) << ']'\
               << (literal->Positive() ? "++" : "--") \
               << literal; \
        INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_COMMENT(\
          BSILog << " // " << Logging::StdTerm(literal,*Signature::current())); \
        BSILog << '\n'; \
        BSILog.flush(); \
        checkMaxSizeOfBSILogFile(); \
      } \
  } 


#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_INTRODUCE_CLAUSE_ID(clause) \
  { \
   INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_SIGNATURE_UPDATE; \
   BSILog << "|" \
          << '[' << Logging::Integer32(clause->Number()) << ']' \
          << clause; \
   INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_COMMENT(\
     BSILog << "\n // " << Logging::StdClause(clause)); \
   BSILog << '\n'; \
   BSILog.flush(); \
   checkMaxSizeOfBSILogFile(); \
  } 

#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_UNBIND_TERM_ID(term) \
  { \
    if (term->arity()) \
     { \
       BSILog << "~#[" << Logging::Integer32(term) << ']'; \
       INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_COMMENT(\
         BSILog << " // " << Logging::StdTerm(term,*Signature::current())); \
       BSILog << '\n'; \
       BSILog.flush(); \
       checkMaxSizeOfBSILogFile(); \
     } \
  } 


#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_UNBIND_LITERAL_ID(literal) \
  { \
    if (literal->arity()) \
      { \
        BSILog << "~*[" << Logging::Integer32(literal) << ']'; \
        INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_COMMENT(\
          BSILog << " // " << Logging::StdTerm(literal,*Signature::current())); \
        BSILog << '\n'; \
        BSILog.flush(); \
        checkMaxSizeOfBSILogFile(); \
      } \
  } 


#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_UNBIND_CLAUSE_ID(clause) \
  { \
   BSILog << "~|" \
          << '[' << Logging::Integer32(clause->Number()) << "]\n"; \
   INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_COMMENT(\
     BSILog << " // " << Logging::StdClause(clause)); \
   BSILog << '\n'; \
   BSILog.flush(); \
   checkMaxSizeOfBSILogFile(); \
  } 



#endif

// Undefining the macros in the case an efficient version of Vampire is built  

#ifndef INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING

#undef INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_SIGNATURE_UPDATE
#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_SIGNATURE_UPDATE

#undef INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_INTEGRATE
#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_INTEGRATE(clause) 
 
#undef INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_REMOVE
#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_REMOVE(clause)

#undef INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_RETRIEVAL_QUERY  
#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_RETRIEVAL_QUERY(clause)

#undef INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_RETRIEVAL_RESULT
#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_RETRIEVAL_RESULT(res) 
  
#undef INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_RETRIEVAL_TOTAL 
#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_RETRIEVAL_TOTAL

#undef INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_INTRODUCE_TERM_ID
#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_INTRODUCE_TERM_ID(term)

#undef INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_INTRODUCE_LITERAL_ID
#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_INTRODUCE_LITERAL_ID(literal)

#undef INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_INTRODUCE_CLAUSE_ID
#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_INTRODUCE_CLAUSE_ID(clause)

#undef INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_UNBIND_TERM_ID
#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_UNBIND_TERM_ID(term)

#undef INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_UNBIND_LITERAL_ID
#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_UNBIND_LITERAL_ID(literal)

#undef INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_UNBIND_CLAUSE_ID
#define INTERCEPT_BACKWARD_SUBSUMPTION_INDEXING_UNBIND_CLAUSE_ID(clause)


#endif



// Indexing operations for backward matching:
    
#ifdef INTERCEPT_BACKWARD_MATCHING_INDEXING
                   
#include "Logging.h"
#include <string.h>

#ifdef INTERCEPT_BACKWARD_MATCHING_INDEXING_WITH_COMMENTS
#define INTERCEPT_BACKWARD_MATCHING_INDEXING_COMMENT(com) com
#else
#define INTERCEPT_BACKWARD_MATCHING_INDEXING_COMMENT(com) 
#endif
 
extern Logging BMILog; // declared in VampireKernelInterceptMacros.cpp
extern const TERM* interceptedBackwardMatchingQuery;
extern ulong interceptedBackwardMatchingResultTotal;       
extern long maxSizeOfBMILogFile; 
inline void checkMaxSizeOfBMILogFile()
{
  static long n = strlen("\\ end of file\n"); 
  if (BMILog.counter() >= maxSizeOfBMILogFile - n) 
    { 
      cout << "%    Forced termination: file specified by --bmilog is full.\n"; 
      INTERCEPT_BACKWARD_MATCHING_INDEXING_COMMENT(BMILog << "// end of file\n";)
      Exit::exit(0); 
    }; 
};


#define INTERCEPT_BACKWARD_MATCHING_INDEXING_INTEGRATE(term) \
  { \
   if (term->arity()) \
    { \
     BMILog << "+" << term; \
     INTERCEPT_BACKWARD_MATCHING_INDEXING_COMMENT( \
      BMILog << "         // " << Logging::StdTerm(term,*Signature::current()); \
                                                 ) \
     BMILog << "\n"; \
     BMILog.flush(); \
     checkMaxSizeOfBMILogFile(); \
    }; \
  }

#define INTERCEPT_BACKWARD_MATCHING_INDEXING_REMOVE(term) \
  { \
   if (term->arity()) \
    { \
     BMILog << "-" << term; \
     INTERCEPT_BACKWARD_MATCHING_INDEXING_COMMENT( \
      BMILog << "         // " << Logging::StdTerm(term,*Signature::current()); \
                                                 ) \
     BMILog << "\n"; \
     BMILog.flush(); \
     checkMaxSizeOfBMILogFile(); \
    }; \
  }

#define INTERCEPT_BACKWARD_MATCHING_INDEXING_RETRIEVAL_QUERY(term) \
  { \
   interceptedBackwardMatchingQuery=term; \
   if (term->arity()) \
    { \
     interceptedBackwardMatchingResultTotal = 0UL; \
     BMILog << "?" << term; \
     INTERCEPT_BACKWARD_MATCHING_INDEXING_COMMENT( \
      BMILog << "         // " << Logging::StdTerm(term,*Signature::current()); \
                                                 ) \
     BMILog << "\n"; \
     BMILog.flush(); \
     checkMaxSizeOfBMILogFile(); \
    }; \
  }

#ifdef INTERCEPT_BACKWARD_MATCHING_INDEXING_SHOW_MATCHES
#define INTERCEPT_BACKWARD_MATCHING_INDEXING_RETRIEVAL_RESULT(res) \
   { \
    if (interceptedBackwardMatchingQuery->arity()) \
     { \
      interceptedBackwardMatchingResultTotal++; \
      BMILog << " !" << res; \
      INTERCEPT_BACKWARD_MATCHING_INDEXING_COMMENT( \
       BMILog << "         // " << Logging::StdTerm(res,*Signature::current()); \
                                                 ) \
      BMILog << "\n"; \
      BMILog.flush(); \
      checkMaxSizeOfBMILogFile(); \
     }; \
   }
#else 
#define INTERCEPT_BACKWARD_MATCHING_INDEXING_RETRIEVAL_RESULT(res) \
   { \
    if (interceptedBackwardMatchingQuery->arity()) \
     { \
      interceptedBackwardMatchingResultTotal++; \
     }; \
   }
#endif

#define INTERCEPT_BACKWARD_MATCHING_INDEXING_RETRIEVAL_TOTAL \
  { \
   if (interceptedBackwardMatchingQuery->arity()) \
    { \
     BMILog << "$" << interceptedBackwardMatchingResultTotal << " \n"; \
     BMILog.flush(); \
     checkMaxSizeOfBMILogFile(); \
    }; \
  } 

#define INTERCEPT_BACKWARD_MATCHING_INDEXING_RETRIEVAL_ABORT \
  { \
   if (interceptedBackwardMatchingQuery->arity()) \
    { \
     BMILog << "@" << interceptedBackwardMatchingResultTotal << " \n"; \
     BMILog.flush(); \
     checkMaxSizeOfBMILogFile(); \
    }; \
  } 

#endif

// Undefining the macros in the case an efficient version of Vampire is built  

#ifndef INTERCEPT_BACKWARD_MATCHING_INDEXING

#undef INTERCEPT_BACKWARD_MATCHING_INDEXING_INTEGRATE
#define INTERCEPT_BACKWARD_MATCHING_INDEXING_INTEGRATE(term) 
 
#undef INTERCEPT_BACKWARD_MATCHING_INDEXING_REMOVE
#define INTERCEPT_BACKWARD_MATCHING_INDEXING_REMOVE(term)

#undef INTERCEPT_BACKWARD_MATCHING_INDEXING_RETRIEVAL_QUERY  
#define INTERCEPT_BACKWARD_MATCHING_INDEXING_RETRIEVAL_QUERY(term)

#undef INTERCEPT_BACKWARD_MATCHING_INDEXING_RETRIEVAL_RESULT
#define INTERCEPT_BACKWARD_MATCHING_INDEXING_RETRIEVAL_RESULT(res) 

#undef INTERCEPT_BACKWARD_MATCHING_INDEXING_RETRIEVAL_ABORT
#define INTERCEPT_BACKWARD_MATCHING_INDEXING_RETRIEVAL_ABORT
  
#undef INTERCEPT_BACKWARD_MATCHING_INDEXING_RETRIEVAL_TOTAL 
#define INTERCEPT_BACKWARD_MATCHING_INDEXING_RETRIEVAL_TOTAL

#endif

//====================================================    
#endif




