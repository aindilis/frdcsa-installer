//
//  file query.h 
//  defines class Query of KIF XML queries
//


#ifndef __query__
#define __query__


#include <string>


class Formula;


class Query {
 public:
  class Attribute;
  enum QueryType {
    ASSERTION,
    GOAL,
    TERMINATE
  };

  // constructors
  Query (QueryType qtype, Formula* formula, Attribute* attributes);
  Query () : _type (TERMINATE) {}
  ~Query ();

  // query structure
  QueryType type () const { return _type; }
  Formula* formula () const { return _formula; }
  int timeLimit () const { return _timeLimit; }
  int bindingsLimit () const { return _bindingsLimit; }

 private:
  // structure
  QueryType _type; // a query can be a goal or an assertion
  Formula* _formula;
  Attribute* _attributes;
  int _timeLimit;
  int _depthLimit;
  int _bindingsLimit;
}; // class Query


class Query::Attribute {
 public:
  // constructors
  Attribute (const string name, const string value) :
    _name (name),
    _value (value)
    {}

  // query the structure
  const string name () const { return _name; }
  const string value () const { return _value; }
  Attribute* next () const { return _next; }
  void next (Attribute* att) { _next = att; }
 private:
  const string _name;
  const string _value;
  Attribute* _next;
}; // class Query::Attribute


#endif // __query_
