#pragma once

//--------------------------------------------------------------
//--------------------------------------------------------------

struct LcaTreeNode {
  int v;
  int depth;

  LcaTreeNode() : v(-1), depth(-1) {}
  LcaTreeNode( int _v, int _depth ) : v(_v), depth(_depth) {}
};

static const LcaTreeNode InfLcaNode(-1, 100000000);

bool operator > ( const LcaTreeNode& n1, const LcaTreeNode& n2 ) {
  return n1.depth > n2.depth;
}

bool operator < ( const LcaTreeNode& n1, const LcaTreeNode& n2 ) {
  return n1.depth < n2.depth;
}

bool operator == ( const LcaTreeNode& n1, const LcaTreeNode& n2 ) {
  return n1.depth == n2.depth;
}

bool operator != ( const LcaTreeNode& n1, const LcaTreeNode& n2 ) {
  return n1.depth != n2.depth;
}

class LcaFinder {
 public:
  LcaFinder( const DiGraph& graph, int root )
    {
      vector<LcaTreeNode> nodes;
      minIdx.resize( graph.vCount() );
      _traverse( root, -1, 0, graph, nodes, minIdx );
      lcaTree.init( nodes, InfLcaNode, InfLcaNode );
    }

  int findLca( int v1, int v2 )
  {
    int beg = min( minIdx[v1], minIdx[v2] );
    int end = max( minIdx[v1], minIdx[v2] );
    LcaTreeNode node = lcaTree.queryRange( beg, end );
    return node.v;
  }

 private:
  vector<int> minIdx;
  RangeMinTree<LcaTreeNode>::type lcaTree;

  void _traverse( int v, int prev, int depth, const DiGraph& graph,
		  vector<LcaTreeNode>& nodes, vector<int>& minIdx )
  {
    minIdx[v] = nodes.size();
    nodes.push_back( LcaTreeNode( v, depth ) );
    vector<Edge> edges = graph.getEdgesForVertex(v);
    for( int i = 0; i < edges.size(); i++ ) {
      if( edges[i].v2 == prev ) {
	continue;
      }
      _traverse( edges[i].v2, v, depth + 1, graph, nodes, minIdx );
      nodes.push_back( LcaTreeNode( v, depth ) );
    }
  }
};

//---------------------------------------------------------------------
