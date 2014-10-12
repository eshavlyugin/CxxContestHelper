#pragma once

//-------------------------------------------------
//-------------------------------------------------

struct Edge {
  int v1;
  int v2;
  int w;

  Edge() : v1(-1), v2(-1), w(-1) {}
  Edge( int _v1, int _v2, int _w ) : v1(_v1), v2(_v2), w(_w) {}
};

class Graph {
 public:
  Graph( int vCount ) {
    edges.resize( vCount );
    removed.resize( vCount );
  }

  int vCount() const { 
    return edges.size();
  }

  void addEdge( const Edge& e )
  {
    Edge invE = e;
    std::swap(invE.v1, invE.v2);
    edges[e.v1].push_back( e );
    edges[invE.v1].push_back( invE );
  }

  void removeVertex( int v ) 
  {
    removed[v] = true;
  }

  vector<Edge> getEdgesForVertex( int v ) const
    {
      vector<Edge> result;
      if( removed[v] ) {
	return result;
      }
      result = edges[v];
      int curPt = 0;
      for( int i = 0; i < result.size(); i++ ) {
	if( !removed[result[i].v1] && !removed[result[i].v2] ) {
	  result[curPt++] = result[i];
	}
      }
      result.resize( curPt );
      return result;
    }

 private:
  vector< vector<Edge> > edges;
  vector<bool> removed;
};

//----------------------------------------------------------------
