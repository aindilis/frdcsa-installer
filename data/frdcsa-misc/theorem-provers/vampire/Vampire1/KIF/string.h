//
//  File string.h - implements strings that are deallocated automatically after use
//
//  22/06/2000, Manchester
//  08/04/2001, Manchester, operator << 
//  09/04/2001, Manchester, operators +=, length(), string()
//


#ifndef __string__
#define __string__


class ostream;


// *************** class String, definition ***********************


class String {
 public:
  
  String ();
  ~String ();
  explicit String ( const char* str );
  String ( const String* prefix, const char* postfix );
  void operator += ( const char* str ); // appends str to the end
  void operator += ( const String* str ); // appends str to the end
  void operator += ( int ); // converts integer to string and appends it to the end
  int length () const;
  const char* string () const;

 private:

  char* _str;

 friend ostream& operator << ( ostream&, const String* str );
 friend ostream& operator << ( ostream&, const String& str );
}; // class String


ostream& operator << ( ostream&, const String* str );
ostream& operator << ( ostream&, const String& str );


// *************** class String, implementation *******************


inline
const char* String::string () const
{
  return _str;
} // String::string


#endif
