#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>

using namespace std;

#include "RangeTreeLazyPropNew.h"

void testGeneralTree()
{
  RangeTreeLazyProp<Interval1d<int>, int, int, NumSumPredicate,
		    NumMaxPredicate, IdentityPredicate, NumSumPredicate, 
		    0, 0> tree( Interval1d<int>( 0, 100000 ) );
  tree.update( Interval1d<int>(2, 10), 3 );
  cout << tree.queryRange( Interval1d<int>( 4, 9 ) ) << endl;
  cout << tree.queryRange( Interval1d<int>( 10, 11 ) ) << endl;
  cout << tree.queryRange( Interval1d<int>( 20, 27 ) ) << endl;
}

void testRangeSumTree()
{
  RangeSumTree<int, Interval1d<int> >::type tr( Interval1d<int>(0, 100000 ) );
  tr.update( Interval1d<int>(3, 19), 3 );
  cout << tr.queryRange( Interval1d<int>( 4, 10 ) ) << endl;
}

void testRangeSumTree2d()
{
  RangeSumTree<int, Interval2d<int> >::type tr( Interval2d<int>(0, 100000,
								0, 100000) );
  tr.update( Interval2d<int>(3, 20, 3, 20), 1 );
  cout << tr.queryRange( Interval2d<int>( 17, 30, 16, 30 ) ) << endl;
}

int main()
{
  testGeneralTree();
  testRangeSumTree();
  testRangeSumTree2d();
  return 0;
}
