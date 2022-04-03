//
//  File assert.cpp
//


#include "assert.h"


MyException::MyException (const char* file, int line) 
  :  
  _file (file),                       
  _line (line)                        
{ 
  //  cry (cout); 
}  // MyException::MyException

MyException::MyException (const char* msg) 
  :  
  _file (msg),
  _line (-1)                        
{
  //  cry (cout);
} // MyException::MyException                      


void MyException::cry (ostream& str)
{
  if ( _line != -1 ) {
    str << "Error in file: "
	<< _file
	<< ", line: " << _line
	<< '\n' << flush;
  }
  else {
    str << _file << '\n';
  }
} // MyException::cry


MemoryException::MemoryException (const char* file, int line) :  
  _file (file),                       
  _line (line)                        
{
} // MemoryException::MemoryException

void MemoryException::cry (ostream& str)
{
  str << "Memory could not be allocated: "
      << _file
      << ", line: " << _line
      << '\n';
}  // MemoryException::cry

