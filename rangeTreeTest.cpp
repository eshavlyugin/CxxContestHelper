#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

#include "RangeTree.h"

int main()
{
  RangeTree<int, int, NumSumPredicate, 
	    NumSumPredicate, NumSumPredicate, 0, 0> sumTree(300);
  sumTree.update( 100, 1 );
  sumTree.update( 120, 2 );
  cout << sumTree.queryRange( 1, 200 ) << endl;
  cout << sumTree.queryRange( 80, 110 ) << endl;
  cout << sumTree.queryRange( 111, 130 ) << endl;
  cout << sumTree.queryRange( 121, 122 ) << endl;
  return 0;
}
