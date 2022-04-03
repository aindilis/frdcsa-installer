//
//  file wizard.h (class Wizard)
//    NOTE: Wizard is an abstract Wizard, has nothing to do with Gandalf
//  07/06/2001 Manchester
//


#ifndef __wizard__
#define __wizard__


class Problem;
class ostream;


class Wizard {
 public:

  Wizard () {};
  void execute (Problem&, int timeLimit, int memoryLimit, char* tabFile, char* outFile, ostream& tab);
}; // class Wizard


#endif // __wizard__
