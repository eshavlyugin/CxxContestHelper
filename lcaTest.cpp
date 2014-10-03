#include <iostream>
#include <vector>

using namespace std;

#include "RangeTree.h"
#include "Graph/Graph.h"
#include "Graph/LcaFinder.h"

int main()
{
  ///     0
  ///   1    2
  /// 3   4   8
  ///5 6   7
  DiGraph graph(9);
  graph.addEdge( Edge( 0, 1, 1 ) );
  graph.addEdge( Edge( 0, 2, 1 ) );
  graph.addEdge( Edge( 1, 3, 1 ) );
  graph.addEdge( Edge( 1, 4, 1 ) );
  graph.addEdge( Edge( 3, 5, 1 ) );
  graph.addEdge( Edge( 3, 6, 1 ) );
  graph.addEdge( Edge( 4, 7, 1 ) );
  graph.addEdge( Edge( 2, 8, 1 ) );
  LcaFinder lca(graph, 0);
  cerr << lca.findLca(6, 4) << endl; // 1
  cerr << lca.findLca(3, 8) << endl; // 0
  cerr << lca.findLca(5, 6) << endl; // 3
  cerr << lca.findLca(7, 0) << endl; // 0
  return 0;
}
