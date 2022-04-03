//  Class BitTable
//
//  Implementation of bit tables
//
//  990227 Uppsala
//


#include <ostream.h>


#include "bittable.h"


BitTable::BitTable ( int xs, int ys ) :
  _xsize (xs),
  _ysize (ys)
{
  ASSERT ( debug_bit_table, xs > 0 && ys > 0 );

  int tableSize = xs * ys / wordSize + 1;
  _table = new unsigned int [ tableSize ];

  // initialize all words by 0
  for ( int k = 0L; k < tableSize; k++)
    _table [k] = 0UL;
}  // BitTable::BitTable


BitTable::~BitTable()
{
  delete [] _table;
}


// A debug function, shows all (x,y) such that table[x,y] is true
void BitTable::showAll ( ostream& str ) const
{
  str << "True values in the table are:\n";

  for ( int x = 0; x < _xsize; ++x ) {
    
    str << x << ':';
    
    for ( int y = x - 1; y >= 0; --y ) {
      if ( get (x, y) ) 
        str << ' ' << y;
      else 
        {}
    }
    
    str << '\n';
  }

  str << "\nEnd of table\n";
}  // BitTable::showAll
