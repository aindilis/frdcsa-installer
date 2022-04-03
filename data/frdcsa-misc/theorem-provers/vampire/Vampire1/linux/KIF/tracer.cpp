//
//  File tracer.cpp
//  01/05/2002 Manchester
//


#include <iostream.h>

#include "tracer.h"


int Tracer::_depth = 0;


Tracer::Tracer (const char* fun) 
  : 
  _fun (fun) 
{
  // print _depth spaces and increase _depth
  for (int i = _depth++ - 1; i >= 0; i--) {
    cout << ' ';
  }
  cout << '+' << fun << "\n" << flush;
//  cout << fun << "\n" << flush;
} // Tracer::Tracer


Tracer::~Tracer () 
{
  // decrease _depth and print _depth spaces

  for (int i = --_depth - 1; i >= 0; i--) {
    cout << ' ';
  }
  cout << '-' << _fun << "\n" << flush;
} // Tracer::~Tracer

