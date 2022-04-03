#include <string>
#include <iostream.h>

int main ()
{
  string a = "Hello world";
  a = "\"" + a + "\"";
  cout << a;
}
