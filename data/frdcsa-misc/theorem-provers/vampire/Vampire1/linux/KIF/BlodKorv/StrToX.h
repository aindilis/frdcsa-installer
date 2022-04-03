//
// File:         StrToX.h
// Description:  Simple string parsing. 
// Created:      Jan 30, 2000, 19:00
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//======================================================================
#ifndef STR_TO_X_H
//======================================================================
#define STR_TO_X_H
#include "jargon.h"
#if (COMPILER != COMPILER_MS_VISUAL_C_PLUS_PLUS)
 #include <strstream.h>
#else
 #include <strstrea.h>
#endif
//#include <string.h>
#include "RuntimeError.h"
//======================================================================
namespace BK 
{

class StrToX
 {
  public:
   StrToX(
          #if (COMPILER != COMPILER_MS_VISUAL_C_PLUS_PLUS)  
           const char* s
          #else
           char* s
          #endif
		 ) 
	: strm(s,strlen(s) + 1) 
   {
   }; 
   ~StrToX() {};
   bool end() 
   { 
    if (strm.eof()) return true;
    char ch; 
    strm >> ch;
    #if (COMPILER != COMPILER_MS_VISUAL_C_PLUS_PLUS)  
     strm.unget();
    #else
	 strm.putback(ch);
     if (!strm.good()) 
      RuntimeError::report("bad istrstream in " __FILE__);
    #endif
    return !ch;
   };  

   bool read(char& n) { strm >> n; return (strm); }; 
   bool read(int& n) { strm >> n; return (strm); };
   bool read(unsigned& n) { strm >> n; return (strm); };
   bool read(long& n) { strm >> n; return (strm); };
   bool read(ulong& n) { strm >> n; return (strm); }; 
   bool read(float& n) { strm >> n; return (strm); };
   bool read(double& n) { strm >> n; return (strm); };

   bool conv(char& n) { strm >> n; return (strm) && end(); }; 
   bool conv(int& n) { strm >> n; return (strm) && end(); };
   bool conv(unsigned& n) { strm >> n; return (strm) && end(); };
   bool conv(long& n) { strm >> n; return (strm) && end(); };
   bool conv(ulong& n) { strm >> n; return (strm) && end(); }; 
   bool conv(float& n) { strm >> n; return (strm) && end(); };
   bool conv(double& n) { strm >> n; return (strm) && end(); };

   #if (COMPILER != COMPILER_MS_VISUAL_C_PLUS_PLUS)  
    static bool read(char& n,const char* s) { StrToX str(s); return str.read(n); };
    static bool read(int& n,const char* s) { StrToX str(s); return str.read(n); };
    static bool read(unsigned& n,const char* s) { StrToX str(s); return str.read(n); };
    static bool read(long& n,const char* s) { StrToX str(s); return str.read(n); };
    static bool read(ulong& n,const char* s) { StrToX str(s); return str.read(n); };
    static bool read(float& n,const char* s) { StrToX str(s); return str.read(n); };
    static bool read(double& n,const char* s) { StrToX str(s); return str.read(n); };
   
    static bool conv(char& n,const char* s) { StrToX str(s); return str.conv(n); };
    static bool conv(int& n,const char* s) { StrToX str(s); return str.conv(n); };
    static bool conv(unsigned& n,const char* s) { StrToX str(s); return str.conv(n); };
    static bool conv(long& n,const char* s) { StrToX str(s); return str.conv(n); };
    static bool conv(ulong& n,const char* s) { StrToX str(s); return str.conv(n); };
    static bool conv(float& n,const char* s) { StrToX str(s); return str.conv(n); };
    static bool conv(double& n,const char* s) { StrToX str(s); return str.conv(n); };
  #else
    static bool read(char& n,char* s) { StrToX str(s); return str.read(n); };
    static bool read(int& n,char* s) { StrToX str(s); return str.read(n); };
    static bool read(unsigned& n,char* s) { StrToX str(s); return str.read(n); };
    static bool read(long& n,char* s) { StrToX str(s); return str.read(n); };
    static bool read(ulong& n,char* s) { StrToX str(s); return str.read(n); };
    static bool read(float& n,char* s) { StrToX str(s); return str.read(n); };
    static bool read(double& n,char* s) { StrToX str(s); return str.read(n); };
   
    static bool conv(char& n,char* s) { StrToX str(s); return str.conv(n); };
    static bool conv(int& n,char* s) { StrToX str(s); return str.conv(n); };
    static bool conv(unsigned& n,char* s) { StrToX str(s); return str.conv(n); };
    static bool conv(long& n,char* s) { StrToX str(s); return str.conv(n); };
    static bool conv(ulong& n,char* s) { StrToX str(s); return str.conv(n); };
    static bool conv(float& n,char* s) { StrToX str(s); return str.conv(n); };
    static bool conv(double& n,char* s) { StrToX str(s); return str.conv(n); };
  #endif

  private:
   istrstream strm;
 }; // class StrToX
}; // namespace BK

//======================================================================
#endif
