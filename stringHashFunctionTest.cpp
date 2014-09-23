#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>

using namespace std;

#include "StringAlgo/StringHashFunction.h"

int main()
{
  StringHashFunction hf("xxxxxxxxxxabcdabc", 101);
  // todo: test_assert equal
  cerr << hf.calcHash(10,3) << " " << hf.calcHash(14,3) << endl;
  return 0;
}
