#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <stdexcept>

using namespace std;

#include "DataStructures/CartesianTree.h"

int main()
{
  vector<int> a(1000);
  CartesianTree<int> tree;
  for( int i = 0; i < 1000; i++ ) {
    a[i] = rand();
    tree = tree.insertValue( a[i] );
  }
  
  sort( a.begin(), a.end() );
  for( int i = 0; i < 1000; i++ ) {
    cerr << (tree.findNthValue( i ) == a[i]) << endl;
  }

  for( int i = 0; i < 1000; i += 2 ) {
    tree = tree.deleteValue( a[i] );
  }
  for( int i = 1; i < 1000; i += 2 ) {
    cerr << (tree.findNthValue( i / 2 ) == a[i]) << endl;
  }
  return 0;
}
