//
//  file schedule.h running scheduled tasks
//


#ifndef __schedule__
#define __schedule__

#include "list.h"
#include "GlobalClock.h"

class Property;
class Problem;
class Options;


class Schedule {
 public:
  typedef List<Options*,CID_OPTION_LIST> List;

  explicit Schedule (Property&, Options& cmdLineOptions);
  ~Schedule ();
  char run (Problem&, BK::GlobalClock::Timer& timer);

 private:

  // structure
  List* _schedule;
  Options& _commandLineOptions;

  // auxiliary function
  static int absoluteTime (int relativeTime);
}; // class Schedule


#endif // __schedule__
