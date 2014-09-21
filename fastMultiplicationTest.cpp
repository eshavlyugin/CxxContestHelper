#include <iostream>

using namespace std;

#include "Numeric/FastPower.h"
#include "Numeric/Modular.h"

int main()
{
  ModP<31> x(3);
  long long val = fastPower( x, ModP<31>(1), 7 );
  cout << val << endl;
  return 0;
}
