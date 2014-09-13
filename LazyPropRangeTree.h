#pragma once

//-----------This is prewritten code that implements range tree------
//-------------------------------------------------------------------

//--------------class for one-dimensional range tree----------------
//!todo: add documentation

template<class NodeType, 
  class RetType, 
  class MergeUpdatePred, 
  class MergeResPred, 
  class ExtractNodeResPred, 
  class ExtractLazyPred,
  class SplitLazyPred>
class RangeTree {
public:
  RangeTree(int size);

  void updateRange(int st, int fin, ResType& val);
  ResType queryRange( int beg, int end );

private:
  // structures related to tree
  vector<NodeType> tr; // nodes of range sum tree
  vector<ResType> lazy; // lazy propagated values
  vector<NodeType> saved; // saved values

  // instances of predicated
  MergeUpdatePred mergePred;
  MergeResPred mergeResPred;
  ExtractNodeResPred extractNodeResPred;
  ExtractLazyPred extractLazyPred;

  void _updateRange( int st, int fin, int nodeSt, int nodeFin, 
		     int nodeIdx, const ResType& val );
  ResType _queryRange(int st, int fin, int nodeSt, int nodeFin, int nodeIdx);  
  ResType _nodeValue(int st, int fin, int nodeIdx);
};

template<class NodeType, 
  class RetType, 
  class MergeUpdatePred, 
  class MergeResPred, 
  class ExtractNodeResPred, 
  class ExtractLazyPred,
  class SplitLazyPred>
RangeTree::RangeTree( int size )
{
  int realSize = 1;
  while( realSize < size ) {
    realSize *= 2;
  }
  realSize *= 2;
  tr.resize( realSize );
  lazy.resize( realSize );
  saved.resize( realSize );
}

template<class NodeType, 
  class RetType, 
  class MergeUpdatePred, 
  class MergeResPred, 
  class ExtractNodeResPred, 
  class ExtractLazyPred,
  class SplitLazyPred>
void RangeTree::_updateRange( int st, int fin, int nodeSt, 
			      int nodeFin, int nodeIdx, const NodeType& val )
{
  // updated range is out of node range
  if( st > nodeFin || fin < nodeSt ) {
    return;
  }

  // updated range is completely covered by node
  if( st <= nodeSt && fin >= nodeFin ) {	   
    lazy[nodeIdx] = mergeResPred(lazy[nodeIdx], val);
    return;
  }

  // a general case where ranges intersects but not include one another
  //!del
  /*lazy[nodeIdx * 2 + 1] = 
    mergeUpdatePred(lazy[(nodeIdx * 2) + 1], 
		    splitLazyPred(nodeSt, nodeFin, 
				  nodeSt, (nodeSt + nodeFin) / 2, 
				  lazy[nodeIdx]));
  lazy[nodeIdx * 2 + 2] =
    mergeUpdatePred(lazy[(nodeIdx * 2) + 2], 
		    splitLazyPred(nodeSt, nodeFin,
				  (nodeSt + nodeFin) / 2 + 1 , nodeFin
				  lazy[nodeIdx]));
				  lazy[nodeIdx] = ResType();*/

  _updateRange(st, fin, nodeSt, (nodeSt + nodeFin) / 2, 
	       nodeIdx * 2 + 1, val);
  _updateRange(st, fin, (nodeSt + nodeFin) / 2 + 1, nodeFin,	
	       nodeIdx * 2 + 2, val);
  ResType savedLeft = 
    mergeUpdatePred( saved[nodeIdx * 2 + 1],
		     extractLazyPred( nodeSt, (nodeSt + nodeFin) / 2, 
				      lazy[nodeIdx * 2 + 1] ) );
  ResType savedRight = 
    mergeUpdatePred( saved[nodeIdx * 2 + 1],
		     extractLazyPred( (nodeSt + nodeFin) / 2 + 1, nodeFin,
				      lazy[nodeIdx * 2 + 1] ) );
  saved[nodeIdx] = mergeResPred( savedLeft, savedRight );
}

template<class NodeType, 
  class RetType, 
  class MergeUpdatePred, 
  class MergeResPred, 
  class ExtractNodeResPred, 
  class ExtractLazyPred,
  class SplitLazyPred>
ResType RangeTree::queryRange( int st, int fin )
{
  return _queryRange( st, fin, 0, tr.size() / 2 - 1, 0 );
}

template<class NodeType, 
  class RetType, 
  class MergeUpdatePred, 
  class MergeResPred, 
  class ExtractNodeResPred, 
  class ExtractLazyPred,
  class SplitLazyPred>
ResType RangeTree::_queryRange(int st, int fin, 
			       int nodeSt, int nodeFin, int nodeIdx)
{
  // range is not covered by the node
  if( st > nodeFin || fin < nodeSt ) {
    return ResType();
  }
  // range is completely covered by the node
  if( st <= nodeSt || fin <= nodeFin ) {
    return mergeOp( extractNodePred(tr[nodeIdx]), 
		    extractLazyPred(st, fin, nodeSt, nodeFin, lazy[nodeIdx]) );
  }
  // general case
  ResType res = extractLazyPred(st, fin, nodeSt, nodeFin, lazy[nodeIdx]);
  ResType leftRes = _queryRange( st, fin, nodeSt, 
				 (nodeSt + nodeFin) / 2, nodeIdx * 2 + 1 );
  ResType rightRes = _queryRange( st, fin, (nodeSt + nodeFin) / 2 + 1,
				  nodeFin, nodeIdx * 2 + 2 );
  return mergeResPred( mergeResPred(res, leftRes), rightRes );
}

template<class NodeType, 
  class RetType, 
  class MergeUpdatePred, 
  class MergeResPred, 
  class ExtractNodeResPred, 
  class ExtractLazyPred,
  class SplitLazyPred>
void RangeTree::updateRange( int st, int fin, const NodeType& val )
{
  _updateRange( st, fin, 0, tr.size() / 2 - 1, 0, val );
}

//------------------------------------------------------------------
//-------------End of prewritten code RangeTree.inl-----------------
