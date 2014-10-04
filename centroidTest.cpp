#include <vector>
#include <iostream>

using namespace std;

#include "Graph/Centroid.h"

template<class NodeType>
ostream& operator << (ostream& ost, vector<NodeType>& v )
{
  for( int i = 0; i < v.size(); i++ ) {
    ost << v[i] << " ";
  }
  return ost;
}

int main()
{
  Graph tree(7);
  tree.addEdge( Edge(0, 1, 1) );
  tree.addEdge( Edge(0, 2, 1) );
  tree.addEdge( Edge(1, 3, 1) );
  tree.addEdge( Edge(1, 4, 1) );
  tree.addEdge( Edge(2, 5, 1) );
  tree.addEdge( Edge(5, 6, 1) );
  vector<int> centers = getCentersForSubtree( tree, 0, -1 ); // 2 0
  vector<int> centers2 = getCentersForSubtree( tree, 1, 0 ); // 1
  cerr << centers << "\n" << centers2 << "\n";
  CentroidDecomposition decomp = buildCentroidDecomposition(tree);
  for( int i = 0; i < decomp.childs.size(); i++ ) {
    cerr << decomp.childs[i].size() << " " << decomp.childs[i] << "\n";
  }
  cerr << decomp.root << endl;
  const int NN = 300000;
  Graph bigTree(NN);
  for( int i = 1; i < NN; i++ ) {
    bigTree.addEdge( Edge( i-1, i, 1 ) );
  }
  CentroidDecomposition decomp2 = buildCentroidDecomposition(bigTree);
  cerr << "OK" << endl;
  return 0;
}
