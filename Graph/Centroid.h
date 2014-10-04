#pragma once

#include "Graph.h"

//------------------------------------------------------

struct CentersRes {
  int maxDist;
  vector<int> centers;
  
  CentersRes() : maxDist(1000000000) {}
};

struct CentroidDecomposition {
  int root;
  vector< vector<int> > childs;
 
  CentroidDecomposition() : root(-1) {}
};

void _getMaxDepthsOfChildren( const Graph& tree, int v, int prevV, 
			     int depth, vector<int>& res )
{
  res[v] = 0;
  vector<Edge> edges = tree.getEdgesForVertex( v );
  for( int i = 0; i < edges.size(); i++ ) {
    if( edges[i].v2 == prevV ) {
      continue;
    }
    _getMaxDepthsOfChildren( tree, edges[i].v2, v, depth + edges[i].w, res );
    res[v] = std::max( res[v], res[edges[i].v2] + edges[i].w );
  }
}

void getMaxDepthsOfChildren( const Graph& tree, int root, vector<int>& res )
{
  _getMaxDepthsOfChildren( tree, root, -1, 0, res );
}

// todo: too long. Think about shorter way of implementation. Or split for methods if it's too bad
void _getCentersForSubtree( const Graph& graph, int v, 
			    int prevV, int distFromPrevV,
			    const vector<int>& maxChildDepths, CentersRes& res )
{
  vector<Edge> edges = graph.getEdgesForVertex( v );
  // Searching for deepest ancestor of a given node
  int best = -1;
  for( int i = 0; i < edges.size(); i++ ) {
    if( prevV == edges[i].v2 ) {
      continue;
    }
    if( best < 0 || maxChildDepths[edges[i].v2] + edges[i].w > 
	maxChildDepths[edges[best].v2] + edges[best].w ) 
      {
	best = i;
      }
  }
  // Searching for second deepest ancestor of a given node
  int secondBest = -1;
  for( int i = 0; i < edges.size(); i++ ) {
    if( prevV == edges[i].v2 || best == i ) {
      continue;
    }
    if( secondBest < 0 
	|| maxChildDepths[edges[i].v2] + edges[i].w > 
	maxChildDepths[edges[secondBest].v2] + edges[secondBest].w )		\
      {
	secondBest = i;
      }
  }
  // Iterating for children
  for( int i = 0; i < edges.size(); i++ ) {
    if( edges[i].v2 == prevV ) {
      continue;
    }
    int dist = 0;
    if( i == best ) {
      dist = secondBest >= 0 ? 
	maxChildDepths[edges[secondBest].v2] + edges[secondBest].w : 0;
    } else {
      dist = best >= 0 ? maxChildDepths[edges[best].v2] + edges[best].w : 0;
    }
    _getCentersForSubtree( graph, edges[i].v2, v, 
			   max(dist, distFromPrevV) + edges[i].w,
			   maxChildDepths, res );
  }
  // adding current vertex to result
  int maxDist = distFromPrevV;
  if( best >= 0 ) {
    maxDist = std::max( maxDist, maxChildDepths[edges[best].v2] + edges[best].w );
  }
  if( res.maxDist > maxDist ) {
    res.maxDist = maxDist;
    res.centers.clear();
    res.centers.push_back(v);
  } else if( res.maxDist == maxDist ) {
    res.centers.push_back(v);
  }
}

vector<int> getCentersForSubtree( const Graph& tree, 
				   int root, int rootParent )
{
  // just a static buffer that allows don't reallocate array of size of a tree
  static vector<int> maxDepthsForChilds;
  if( maxDepthsForChilds.size() < tree.vCount() ) {
    maxDepthsForChilds.resize( tree.vCount() );
  }
  getMaxDepthsOfChildren( tree, root, maxDepthsForChilds );
  CentersRes res;
  _getCentersForSubtree( tree, root, rootParent, 0, maxDepthsForChilds, res );
  return res.centers;
}

int _buildCentroidDecomposition( Graph& tree, int v, CentroidDecomposition& result )
{
  vector<int> centers = getCentersForSubtree( tree, v, -1 );
  int center = centers[0];
  vector<Edge> edges = tree.getEdgesForVertex( center );
  tree.removeVertex( center );
  for( int i = 0; i < edges.size(); i++ ) {
    result.childs[center].push_back(_buildCentroidDecomposition( tree, edges[i].v2, result ));
  }
  return center;
}

CentroidDecomposition buildCentroidDecomposition( const Graph& _tree )
{
  Graph tree = _tree;
  CentroidDecomposition result;
  result.childs.resize( tree.vCount() );
  result.root = _buildCentroidDecomposition( tree, 0, result );
  return result;
}

