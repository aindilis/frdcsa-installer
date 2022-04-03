#include <iostream.h>
#include <stdlib.h>
#include "Timer.h"
using namespace BK;

long randomNumber()
{
  int base = rand() % 256;
  int bit = rand() % 2;
  if (bit) return base;
  return -base;
};


#define DIFF_NUMBERS 100000
#define ITERATIONS 10000

long arg1[DIFF_NUMBERS];
long arg2[DIFF_NUMBERS];
float farg1[DIFF_NUMBERS];
float farg2[DIFF_NUMBERS];
double darg1[DIFF_NUMBERS];
double darg2[DIFF_NUMBERS];

int main()
{
  for (int i = 0; i < DIFF_NUMBERS; ++i)
    {
      arg1[i] = randomNumber();
      arg2[i] = randomNumber();
      
      farg1[i] = (float)arg1[i];
      farg2[i] = (float)arg2[i];
      
      darg1[i] = (double)arg1[i];
      darg2[i] = (double)arg2[i];      
    };
  
  Timer timer;
  long n;
  float fn;
  double dn;
  bool b;


  cout << "MULTIPLICATION\n";
  timer.reset();
  timer.start();
  for (int j = 0; j < ITERATIONS; ++j)
    for (int i = 0; i < DIFF_NUMBERS; ++i)
      {
        n = arg1[i] * arg2[i];
      };

  timer.stop();
  cout << "   long   " << timer.elapsed() << "\n";

  timer.reset();
  timer.start();
  for (int j = 0; j < ITERATIONS; ++j)
    for (int i = 0; i < DIFF_NUMBERS; ++i)
      {
        fn = farg1[i] * farg2[i];
      };
  timer.stop();
  cout << "   float  " << timer.elapsed() << "\n";

  timer.reset();
  timer.start();
  for (int j = 0; j < ITERATIONS; ++j)
    for (int i = 0; i < DIFF_NUMBERS; ++i)
      {
        dn = darg1[i] * darg2[i];
      };
  timer.stop();
  cout << "   double " << timer.elapsed() << "\n";




  cout << "ADDITION\n";
  timer.reset();
  timer.start();
  for (int j = 0; j < ITERATIONS; ++j)
    for (int i = 0; i < DIFF_NUMBERS; ++i)
      {
        n = arg1[i] + arg2[i];
      };

  timer.stop();
  cout << "   long   " << timer.elapsed() << "\n";

  timer.reset();
  timer.start();
  for (int j = 0; j < ITERATIONS; ++j)
    for (int i = 0; i < DIFF_NUMBERS; ++i)
      {
        fn = farg1[i] + farg2[i];
      };
  timer.stop();
  cout << "   float  " << timer.elapsed() << "\n";

  timer.reset();
  timer.start();
  for (int j = 0; j < ITERATIONS; ++j)
    for (int i = 0; i < DIFF_NUMBERS; ++i)
      {
        dn = darg1[i] + darg2[i];
      };
  timer.stop();
  cout << "   double " << timer.elapsed() << "\n";



  cout << "SUBTRACTION\n";
  timer.reset();
  timer.start();
  for (int j = 0; j < ITERATIONS; ++j)
    for (int i = 0; i < DIFF_NUMBERS; ++i)
      {
        n = arg1[i] - arg2[i];
      };

  timer.stop();
  cout << "   long   " << timer.elapsed() << "\n";

  timer.reset();
  timer.start();
  for (int j = 0; j < ITERATIONS; ++j)
    for (int i = 0; i < DIFF_NUMBERS; ++i)
      {
        fn = farg1[i] - farg2[i];
      };
  timer.stop();
  cout << "   float  " << timer.elapsed() << "\n";

  timer.reset();
  timer.start();
  for (int j = 0; j < ITERATIONS; ++j)
    for (int i = 0; i < DIFF_NUMBERS; ++i)
      {
        dn = darg1[i] - darg2[i];
      };
  timer.stop();
  cout << "   double " << timer.elapsed() << "\n";



  cout << "EQUALITY\n";
  timer.reset();
  timer.start();
  for (int j = 0; j < ITERATIONS; ++j)
    for (int i = 0; i < DIFF_NUMBERS; ++i)
      {
        b = (arg1[i] == arg2[i]);
      };

  timer.stop();
  cout << "   long   " << timer.elapsed() << "\n";

  timer.reset();
  timer.start();
  for (int j = 0; j < ITERATIONS; ++j)
    for (int i = 0; i < DIFF_NUMBERS; ++i)
      {
        b = (farg1[i] == farg2[i]);
      };
  timer.stop();
  cout << "   float  " << timer.elapsed() << "\n";

  timer.reset();
  timer.start();
  for (int j = 0; j < ITERATIONS; ++j)
    for (int i = 0; i < DIFF_NUMBERS; ++i)
      {
        b = (darg1[i] == darg2[i]);
      };
  timer.stop();
  cout << "   double " << timer.elapsed() << "\n";


  cout << "GREATER\n";
  timer.reset();
  timer.start();
  for (int j = 0; j < ITERATIONS; ++j)
    for (int i = 0; i < DIFF_NUMBERS; ++i)
      {
        b = (arg1[i] > arg2[i]);
      };

  timer.stop();
  cout << "   long   " << timer.elapsed() << "\n";

  timer.reset();
  timer.start();
  for (int j = 0; j < ITERATIONS; ++j)
    for (int i = 0; i < DIFF_NUMBERS; ++i)
      {
        b = (farg1[i] > farg2[i]);
      };
  timer.stop();
  cout << "   float  " << timer.elapsed() << "\n";

  timer.reset();
  timer.start();
  for (int j = 0; j < ITERATIONS; ++j)
    for (int i = 0; i < DIFF_NUMBERS; ++i)
      {
        b = (darg1[i] > darg2[i]);
      };
  timer.stop();
  cout << "   double " << timer.elapsed() << "\n";

  
};
