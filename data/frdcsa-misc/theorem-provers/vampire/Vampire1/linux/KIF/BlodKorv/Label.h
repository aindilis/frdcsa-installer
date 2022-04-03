//
// File:         Label.h
// Description:  Visual representation of big numbers and pointers.
// Created:      Sep 24, 1999, 15:10
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//==========================================================================
#ifndef NO_DEBUG
#ifndef LABEL_H
//=========================================================================
#define LABEL_H
#ifndef NO_DEBUG_VIS
 #include <iostream.h>
#endif
#include <limits.h>
#include "jargon.h"
//=========================================================================

namespace BK 
{
class Label
{
 public:
  Label() { init(); };
  void init() {};
  Label(ulong n) { init(n); };
  Label(long n) { init(n); };
  Label(void* p) { init(p); };
  ~Label() {};
  void init(ulong n) 
  {
   num = n;
   for (int i = Length - 1; i>=0; i--)
    {
     str[i] = capital(n%4);     
     n = n/4;
    };
   str[Length] = (char)0;
  };
  void init(long n) { init((ulong)n); };
  void init(void* p) { init((ulong)p); };
  const char* string() const { return str; };
  static const ulong Length; // = ((sizeof(ulong)*CHAR_BIT)/4) + (((sizeof(ulong)*CHAR_BIT)%4) ? 1 : 0);
 private:
  char capital(ulong n) { return (char)('A' + (n%16)); }; 
 private:
  ulong num;
  char str[((sizeof(ulong)*CHAR_BIT)/4) + (((sizeof(ulong)*CHAR_BIT)%4) ? 1 : 0) + 1]; // [Length + 1]
};

}; // namespace BK



#ifndef NO_DEBUG_VIS
 inline ostream& operator<<(ostream& ostr,const BK::Label& label)
 {
  return ostr << label.string();
 };
#endif

//===========================================================================
#endif
#endif
