//
// File:         Tab.h
// Description:  Tabulating primitives.
// Created:      Feb 18, 2000, 16:22
// Author:       Alexandre Riazanov
// mail:         riazanov@cs.man.ac.uk
//======================================================================
#ifndef TAB_H
//=======================================================================
#define TAB_H
#if (COMPILER != COMPILER_MS_VISUAL_C_PLUS_PLUS)
 #include <strstream.h>
#else
 #include <strstrea.h>
#endif
#include <string.h>
#include <iostream.h>
#include "jargon.h"
#include "DestructionMode.h"
//=======================================================================
namespace BK 
{
template <class Alloc> 
class Tab
{
 public:
  class ULong
  {
   public:  
    ULong() { reset(0UL); };
    ULong(ulong n) { reset(n); };
    ~ULong() {};
    void reset(ulong n) 
    {
     ostrstream strstr;    
     strstr << n << '\0';   
     ulong ldiff = 10 - strlen(strstr.str()); // too rigid
     strcpy(str + ldiff,strstr.str());
     for (unsigned i = 0; i < ldiff; i++) str[i] = ' ';
    };
    const char* string() const { return str; };
    static ulong Length() { return 10; }; //	too rigid
   private: 
    char str[10 + 1]; // too rigid
  }; // class ULong
 
  class String
  { 
   public:
    String(const ulong& sz) : _size(sz)
    {      
     _str = Alloc::allocate(sz);
     reset(""); 
    };
    String(const ulong& sz,const char* s) : _size(sz)
    {
     _str = Alloc::allocate(sz); 
     reset(s); 
    };
    ~String() 
    {
     if (DestructionMode::isThorough()) Alloc::deallocate(_str,_size);
    };
    void reset(const char* s) 
    {
     ulong ldiff;
     if (strlen(s) > _size) 
      {
       ldiff = strlen(s) - _size;
       strcpy(_str,s + ldiff);
      }
     else
      {
       ldiff = _size - strlen(s);
       strcpy(_str+ldiff,s);
       for (unsigned i = 0; i < ldiff; i++) _str[i] = ' ';
      };
    };
    const char* string() const { return _str; };
    //static ulong Length() { return _size; };  
   private:
    const ulong _size;
    char* _str; 
 }; // class String 
}; // template <class Alloc> class Tab

}; // namespace BK

template <class Alloc>
ostream& operator<<(ostream& ostr,const BK::Tab<Alloc>::ULong& num)
{
 return ostr << num.string();
};

template <class Alloc>
ostream& operator<<(ostream& ostr,const BK::Tab<Alloc>::String& str)
{
 return ostr << str.string();
};

//========================================================================
#endif
