#include "Predicates.h"

//-----------RangeTree.h----begins------
//-------------------------------------------------

template<class UpdateType, class ResType,
  class UpdateNodePred, 
  class MergeResPred, 
  class NodeToResPred>
class RangeTree {
public:
  RangeTree() {}

  RangeTree( int size, const UpdateType& _defUpdate, const ResType& _defRes ) 
  {
    defUpdate = _defUpdate;
    defRes = _defRes;
    int realSize = _findTreeArraySize(size);
    tree.resize(realSize, defUpdate);
  }

  // construct tree from array. Faster than one by one elements insertion
  RangeTree( const vector<UpdateType>& elems, const UpdateType& _defUpdate, const ResType& _defRes );

  void init( const vector<UpdateType>& elems, const UpdateType& _defUpdate, const ResType& _defRes );
  // update a given element with a given value
  void update( int elem, const UpdateType& val );

  // query the value of a given interval
  ResType queryRange( int first, int last );

  // predicate setters. Note that using them when tree is built may cause unpredictable behaviour
  // Make sure you understand what you're doing
  void setNodeToResPred( const NodeToResPred& pred) { nodeToResPred = pred; }

private:
  // elements of tree. Always have a node value n + 1
  vector<UpdateType> tree;

  ResType defRes;
  UpdateType defUpdate;

  /* predicate objects */
  UpdateNodePred updateNodePred;
  MergeResPred mergeResPred;
  NodeToResPred nodeToResPred;

  int _findTreeArraySize( int elemsCount );
};

template<class UpdateType, class ResType,
  class UpdateNodePred, class MergeResPred,
  class NodeToResPred>
  RangeTree<UpdateType, ResType, UpdateNodePred, MergeResPred,
  NodeToResPred>::RangeTree( const vector<UpdateType>& elems, const UpdateType& _defUpdate, const ResType& _defRes )
{
  init( elems, _defUpdate, _defRes );
}

template<class UpdateType, class ResType,
  class UpdateNodePred, class MergeResPred,
  class NodeToResPred>
  int RangeTree<UpdateType, ResType, UpdateNodePred, MergeResPred,
  NodeToResPred>::_findTreeArraySize( int elemsCount )
{
  int size = 4;
  while( size < elemsCount ) {
    size *= 2;
  }
  return size * 2;
}

template<class NodeType, class ResType,
  class UpdateNodePred, class MergeResPred,
  class NodeToResPred>
  void RangeTree<NodeType, ResType, UpdateNodePred, MergeResPred, NodeToResPred>::init( const vector<NodeType>& elems, const NodeType& _defUpdate, const ResType& _defRes )
{
  defRes = _defRes;
  defUpdate = _defUpdate;
  int realSize = _findTreeArraySize( elems.size() );
  tree.resize( realSize, defUpdate );
  int leafFirstIndex = tree.size() / 2 - 1;
  // copy elements into tree leafs
  for( int i = 0; i < elems.size(); i++ ) {
    tree[i + leafFirstIndex] = elems[i];
  }
  // constructing remaining nodes
  for( int i = leafFirstIndex - 1; i >= 0; i-- ) {
    tree[i] = updateNodePred(tree[i*2+1], tree[i*2+2]);
  }
}

template<class UpdateType, class ResType,
  class UpdateNodePred, class MergeResPred,
  class NodeToResPred>
  void RangeTree<UpdateType, ResType, UpdateNodePred, MergeResPred,
  NodeToResPred>::update( int elem, const UpdateType& val )
{
  // todo: assert index out of bounds
  elem += tree.size() / 2 - 1;
  tree[elem] = val;
  while( elem != 0 ) {
    elem = (elem - 1) / 2;
    tree[elem] = updateNodePred( tree[elem*2+1], tree[elem*2+2] );
  }
}


template<class NodeType, class ResType,
  class UpdateNodePred, class MergeResPred,
  class NodeToResPred>
  ResType RangeTree<NodeType, ResType, UpdateNodePred, MergeResPred,
  NodeToResPred>::queryRange( int first, int last )
{
  ResType leftRes = defRes;
  ResType rightRes = defRes;
  first += tree.size() / 2 - 1;
  last += tree.size() / 2 - 1;
  while( first <= last ) {
    if( first % 2 == 0 ) {
      leftRes = mergeResPred( leftRes, nodeToResPred( tree[first] ) );
    }
    if( last % 2 == 1 ) {
      rightRes = mergeResPred( nodeToResPred( tree[last] ), rightRes );
    }
    first = first / 2;
    last = last / 2 - 1;
  }
  return mergeResPred( leftRes, rightRes );
}

template<class ValueType>
struct RangeMinTree {
  typedef RangeTree<ValueType, ValueType, NumMinPredicate, 
    NumMinPredicate, IdentityPredicate> type;
};

//-----------RangeTree.h----ends--------------
//-------------------------------------------------------
