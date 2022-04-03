//
//  file query.cpp
//  implements class Query of KIF XML queries
//

#include <stdlib.h>

#include "query.h"
#include "formula.h"
#include "tracer.h"

const string timeLimitString = "timeLimit";
const string depthLimitString = "depthLimit";
const string bindingsLimitString = "bindingsLimit";


// build a query and read values from attributes
// 04/08/2002 TOrrevieja
Query::Query (QueryType qtype, Formula* formula, Attribute* attributes)
  :
  _type (qtype),
  _formula (formula),
  _attributes (attributes),
  _timeLimit (5),
  _depthLimit (-1),
  _bindingsLimit (5)
{
  formula->inc();

  if (qtype == GOAL) { // query, attributes should be sent
    for (Attribute* attr = attributes; attr; attr = attr->next()) {
      if (attr->name() == timeLimitString) {
	// truncate quotes
	char* endptr; 
	_timeLimit = strtoul (attr->value().data(), &endptr, 10);
	// cout << "time limit reset to " << _timeLimit << "\n";
      }
      else if (attr->name() == depthLimitString) {
	// truncate quotes
	char* endptr;
	_depthLimit = strtoul (attr->value().data(), &endptr, 10);
	// cout << "depth limit reset to " << _depthLimit << "\n";
      }
      else if (attr->name() == bindingsLimitString) {
	// truncate quotes
	char* endptr = 0; // 
	_bindingsLimit = strtoul (attr->value().data(), &endptr, 10);
	// cout << "bindings limit reset to " << _bindingsLimit << "\n";
      }
    }
  }
} // Query::Query


Query::~Query ()
{
  TRACER ("Query::~Query");

  switch (_type) {
  case GOAL:
  case ASSERTION:
    _formula->dec ();
    break;

  case TERMINATE:
    break;
  }
} // Query::~Query
