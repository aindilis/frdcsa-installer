/** This software is an adaptation of the theorem prover Vampire for
 * working with large knowledge bases in the KIF format, see 
 * http://www.prover.info for publications on Vampire.
 *
 * Copyright (C) Andrei Voronkov and Alexandre Riazanov
 *
 * @author Alexandre Riazanov <riazanov@cs.man.ac.uk>
 * @author Andrei Voronkov <voronkov@cs.man.ac.uk>, <andrei@voronkov.com>
 *
 * @date 06/06/2003
 * 
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

//
//  File tracer.cpp
//  01/05/2002 Manchester
//


#include "Tracer.hpp"

#if DEBUG_TRACE_ALL

#include <iostream>

MyTracer* MyTracer::_current = 0;


MyTracer::MyTracer (const char* fun) 
  :
  _fun (fun),
  _previous (_current)
{
  //cout << "+" << fun << "\n";
  _current = this;
} // MyTracer::MyTracer


MyTracer::~MyTracer () 
{
  //cout << "-" << _fun << "\n";
  _current = _previous;
} // MyTracer::~MyTracer


// print the stack
// 24/10/2002 Manchester
void MyTracer::printStack (ostream& str)
{
  int depth = 0;

  _current->printStack (str, depth);
} // MyTracer::printStack (ostream& str)


// print the stack
// 24/10/2002 Manchester
void MyTracer::printStack (ostream& str, int& depth)
{
  if (! this) { // beginning of the stack
    return;
  }
  _previous->printStack (str, depth);
  for (int i = depth; i > 0; i--) {
    str << ' ';
  }
  str << _fun << "\n";
  depth ++;
} // MyTracer::printStack (ostream& str, int& depth)


#endif // DEBUG_TRACE_ALL
