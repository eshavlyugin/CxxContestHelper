#include <vector>
#include <iostream>

using namespace std;

#include "Arithmetic/Modular.h"

int main() {
  int n = 17;
  for( int i = 1; i < n; i++ ) {
    int x = invMod( i, n );
    // test_assert( x * i % n == 1 )
    cerr << x * i % n << endl;
  }
  return 0;
}
