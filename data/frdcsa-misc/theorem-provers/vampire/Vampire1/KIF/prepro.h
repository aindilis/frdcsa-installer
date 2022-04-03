//
//  file prepro.h
//  defines preprocessor
//
//  09/06/2000 Manchester
//  31/03/2001 Manchester, changed by introducing functions corresponding 
//              to series of experiments, eg casc17
//  08/04/2001 Manchester, general () now has an argument (ostream)
//  09/05/2001 Manchester, several functions zipped out
//  10/05/2001 Manchester, simplifyA
//  15/05/2001 Manchester, simplifyB
//  22/05/2001 Manchester, simplifyC
//  23/05/2001 Manchester, simplifyBasic/1
//  29/05/2001 Manchester, simplifyD
//  30/05/2001 Manchester, simplifyBasic/0, simplifyE/0
//  31/05/2001 Manchester, normalize/0, simplifyF
//


#ifndef __prepro__
#define __prepro__


class ostream;
class Problem;
class Options;


class Prepro {
 public:
  // Prepro ( Problem&, int timeLimit, int memory );
  Prepro ( Problem& );
/*
  char* classify ();  // returns classification of the problem (eq UEQ)
*/
  int simplifyBasic ();  // simplifications that should always be done
/*
  int simplifyA (ostream& tab);  // Basic + dummy function definitions
  int simplifyB (ostream& tab);  // A + removal of cancellation axioms
  int simplifyC (ostream& tab);  // removal of linear function definitions
  int simplifyD (ostream& tab);  // elimination of resolvable
  int simplifyE (ostream& tab);  // removal of all function definitions
  int simplifyF (ostream& tab);  // negative equality split
  void normalize ();  // normalize the problem
  Problem& problem () { return _problem; }

 private:
  void writeVampireCall ( const char* vampire, const char* problemFile, Options&, 
                          int timeLimit, int memLimit, ostream& );
*/
  Problem& _problem;
/*
  int _timeLimit;
  int _memoryLimit;
//  ostream& _out;
//  ostream& _tab;
//  ostream& _script;
*/
};

ostream& operator << ( ostream&, Prepro& );

#endif
