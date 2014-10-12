#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

#include "Predicates.h"
#include "DataStructures/RangeTree.h"
#include "StringAlgo/SuffixArray.h"

int main()
{
  string s(4000000, 'a');
  //  string s("abcdeabdef");
  SuffixArray sa;
  sa.initialize( s );
  vector<int> elems = sa.getSortedPrefixes();
  vector<int> lcp = sa.getLcp();
  return 0;
}

