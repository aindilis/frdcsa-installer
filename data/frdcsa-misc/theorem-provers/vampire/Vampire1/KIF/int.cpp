//
// File int.cpp (library)
//
//  09/06/2000 Manchester, toString()
//


#include "int.h"


void Int::toString ( int i, char* str )
{
  if ( i == 0 ) {
    str[0] = '0';
    str[1] = 0;
    return;
  }
  // i != 0
  if ( i < 0 ) {
    *str = '-';
    str ++;
    i = -i;
  }
  // i > 0

  char tmp [20];
  // put digits to tmp, but in the reverse order
  int j = 0;
  while ( i != 0 ) {
    tmp[j] = (i % 10) + '0';
    i = i / 10;
    j++;
  }
  // copy digits to tmp, converting then in the right order
  j--;
  while ( j >= 0 ) {
    *str = tmp[j];
    str++;
    j--;
  }
  *str = 0;
} // Int::toString
