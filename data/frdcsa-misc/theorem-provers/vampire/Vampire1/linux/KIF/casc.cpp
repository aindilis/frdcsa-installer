//
//  file casc.h - intended for casc only
//


#include <fstream.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


#include "casc.h"


const float Casc::slowness = Casc::identifySlowness ();


// puts in result the processor slowness in MHz
// relative to the local 993 MHz computers used as as unit
// of speed. Returns 0 if the speed identification is
// unsuccessful
// 07/07/2002 Manchester
const float Casc::identifySlowness ()
{
  char line [200];

  ifstream info ("/proc/cpuinfo"); // the file does not exist
  if (! info) {
    return 0.0;
  }

  while ( info.getline(line, 200) ) {
    char* pos = strstr (line, "cpu MHz");
    if (pos) { // CPU info line found
      // find the first digit in the line
      while (*pos && ! isdigit(*pos)) {
	pos++;
      }
      // either end of line or a digit is found
      if (! *pos) { // end of line
	continue; // try following lines
      }
      // a digit is found
      char* endptr = 0;
      float cpuSpeed = (float)(strtod (pos, &endptr));
      if (*endptr) { // error returned by strtol
        continue;
      }
      return (993.333 / cpuSpeed);
    }
  }

  // end of file, the string has not been found
  return 0.0;
} // Casc::identifySlowness

