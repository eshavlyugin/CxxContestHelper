//-------------------------------------------------
//-------------------------------------------------

struct Edge {
  int v1;
  int v2;
  int w;

  Edge() : v1(-1), v2(-1), w(-1) {}
  Edge( int _v1, int _v2, int _w ) : v1(_v1), v2(_v2), w(_w) {}
};

class DiGraph {
 public:
  DiGraph( int vCount ) {
    edges.resize( vCount );
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

  vector<Edge> getEdgesForVertex( int v ) const
    {
      return edges[v];
    }

 private:
  vector< vector<Edge> > edges;
};
