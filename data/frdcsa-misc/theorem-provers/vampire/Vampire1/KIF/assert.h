//
//  File assert.h
//  991120 Manchester, made from debug.h in kk
//


#ifndef __assert__
#  define __assert__


#include <iostream.h>
#include <stdlib.h>


// ***************** MyException ********************


class MyException {
 public:                                
  MyException (const char* file, int line);
  MyException (const char* msg);
  virtual void cry (ostream& str);

 private:                               
  const char* _file;
  int _line;
}; // MyException


// ***************** MemoryException ********************


class MemoryException {
 public:                                
  MemoryException (const char* file, int line);
  void cry (ostream& str);

 private:                               
  const char* _file;
  int _line;
}; // MemoryException


// ***************** ASSERT and IF ********************


#define EXCEPTION(Class)                  \
  class Class :                           \
    public MyException                    \
  {                                       \
   public:                                \
    Class (const char* file, int line)    \
      : MyException (file, line) {}       \
  }

// renaamedd to ASS not too conflict with Shura's ASSERT
#define ASS(Switch,Cond)                      \
    if ( (Switch) && ! (Cond) ) {             \
      throw MyException (__FILE__,__LINE__);  \
    }                                         


#define NO_MEMORY                                 \
      throw MemoryException (__FILE__,__LINE__);  

#define IF( Switch, Statement )               \
    if ( (Switch) ) {                         \
      Statement;                              \
    }                                         \
    else {}


#endif // __assert__


