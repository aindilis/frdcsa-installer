#ifndef __BITTABLE_H
#  define __BITTABLE_H


#include "assert.h"
#include "debug.h"
#define wordSize (sizeof (int) * 8)


class ostream;


// ********* class BitTable ****************

class BitTable
{
  public:

	  BitTable ( int xsize, int ysize );
	  ~BitTable ();
    bool get ( int x, int y ) const;
    void setTrue ( int x, int y );
    void setFalse ( int x, int y );

  private:

    int _xsize;
    int _ysize;
    unsigned int* _table; 
    void find ( int x, int y, int& word, int& bit) const;
    void showAll ( ostream& ) const;

  friend ostream& operator<< ( ostream& str, const BitTable& tbl );
}; // class Bittable


// ********* class BitTable, definitions ****************


inline 
ostream& operator<< ( ostream& str, const BitTable& tbl )
{
  tbl.showAll (str);
  return str;
}


// find finds thwe word number and the bit number inside the 
// word of (x,y)
inline 
void BitTable::find ( int x, int y, int& word, int& bit ) const
{
  ASSERT ( debug_bit_table,
           x < _xsize &&
           x >= 0 &&
           y < _ysize &&
           y >= 0 );

  int bitNumber = x * _ysize + y;
  word = bitNumber / wordSize;
  bit = bitNumber % wordSize;
}  // BitTable::find


inline
bool BitTable::get ( int x, int y ) const
{
  // finding word and bit
  int word;
  int bit;
  find ( x, y, word, bit );
  
  // the mask corresponding to the bit
  unsigned int mask = 1UL << bit;

  return ( ( _table[word] & mask ) != 0UL );
}  // BitTable::get


// sets table[x,y] to true
inline
void BitTable::setTrue ( int x, int y )
{
  // finding word and bit
  int word;
  int bit;
  find ( x, y, word, bit );
  
  // the mask corresponding to the bit
  unsigned int mask = 1UL << bit;

  _table [ word ] = _table [ word ] | mask;
}  // BitTable::setTrue


// sets table[x,y] to false
inline
void BitTable::setFalse ( int x, int y )
{
  // finding word and bit
  int word;
  int bit;
  find ( x, y, word, bit );
  
  // the mask corresponding to the bit
  unsigned int mask = ~ ( 1UL << bit );

  _table [ word ] = _table [ word ] & mask;
}  // BitTable::setFalse


// ********* end class BitTable ****************

#endif