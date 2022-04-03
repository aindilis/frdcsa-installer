//
//  File string.cpp - implements strings that are deallocated automatically after use
//  08/04/2001, Manchester, operator << 
//


#include <string.h>
#include <iostream.h>


#include "string.h"


String::String ()
  :
  _str ( new char [1] )
{
  *_str = 0;
} // String::String


String::String ( const char* string )
  :
  _str ( new char [ strlen(string) + 1 ] )
{
  strcpy ( _str, string );
} // String::String


// 04/06/2001
// makes a string obtained by concatenating prefix with postfix
String::String ( const String* prefix, const char* postfix )
{
  int prefixLength = prefix->length ();
  _str = new char [ prefixLength + strlen(postfix) + 1 ];
  strcpy ( _str, prefix->string() );
  strcpy ( _str + prefixLength, postfix );
}


String::~String ()
{
  delete [] _str;
} // String::~String


void String::operator += ( const char* string )
{
  int length = strlen ( _str );

  char* result = new char [ length + strlen(string) + 1];
  strcpy ( result, _str );
  strcpy ( result + length, string );

  delete [] _str;

  _str = result;
} // String::operator +=


ostream& operator << ( ostream& str , const String* string )
{
  str << string->_str;

  return str;
} // operator << ( ostream& str, const String& string )


ostream& operator << ( ostream& str, const String& string )
{
  str << string._str;

  return str;
} // operator << ( ostream& str, const String& string )


int String::length () const
{
  return strlen(_str);
} // String::string


