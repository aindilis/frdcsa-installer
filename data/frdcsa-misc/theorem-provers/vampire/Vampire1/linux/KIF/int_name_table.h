//
//  File int_name_table.h
//
//  Defines the class IntNameTable for a table of names, 
//    where insert returns an integer rather than char*
//
//  made from NameTable, 13/05/2000, flight Manchester-Atlanta
//


#ifndef __int_name_table__
#define __int_name_table__


#include <ostream.h>


#include "list.h"
#include "pair.h"


typedef Pair <int,char*,CID_ENTRY> Entry;
typedef List <Entry*,CID_ENTRY_LIST> EntryList;


// ****************** class IntNameTable **********************

class IntNameTable 
{
 public:
  
  IntNameTable ();
  ~IntNameTable ();
  int insert ( const char* str );
  const char* operator[] ( int num ) const;
  int numberOfSymbols ();

  // friend ostream& operator << (ostream &stream,IntNameTable& table);
  static IntNameTable* vars;

 private:

  static const int _noOfBuckets;
  EntryList ** _buckets;
  int _nextNumber;

  // hashing
  static int hash ( const char* );  // hashing function
}; // class NameTable


// ostream& operator << (ostream &stream,IntNameTable& table);



#endif


