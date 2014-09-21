#include "Predicates.h"

//-----------RangeTree.h----begins------
//-------------------------------------------------

template<class UpdateType, class ResType,
  class UpdateNodePred, 
  class MergeResPred, 
  class MergeNodeToResPred,
  UpdateType defUpdate = UpdateType(), 
  ResType defRes = ResType()>
class RangeTree {
public:
  RangeTree( int size ) 
  {
    int realSize = _findTreeArraySize(size);
    tree.resize(realSize, defUpdate);
  }

  // construct tree from array. Faster than one by one elements insertion
  RangeTree( const vector<UpdateType>& elems );

  // update a given element with a given value
  void update( int elem, const UpdateType& val );

  // query the value of a given interval
  ResType queryRange( int first, int last );

private:
  // elements of tree. Always have a node value n + 1
  vector<UpdateType> tree;

  /* predicate objects */
  UpdateNodePred updateNodePred;
  MergeResPred mergeResPred;
  MergeNodeToResPred mergeLazyToResPred;

  int _findTreeArraySize( int elemsCount );
};

template<class UpdateType, class ResType,
  class UpdateNodePred, class MergeResPred,
  class MergeNodeToResPred, UpdateType defUpdate, ResType defRes>
  RangeTree<UpdateType, ResType, UpdateNodePred, MergeResPred,
  MergeNodeToResPred, defUpdate, defRes>::RangeTree( const vector<UpdateType>& elems )
{
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
  class MergeNodeToResPred, UpdateType defUpdate, ResType defRes>
  int RangeTree<UpdateType, ResType, UpdateNodePred, MergeResPred,
  MergeNodeToResPred, defUpdate, defRes>::_findTreeArraySize( int elemsCount )
{
  int size = 4;
  while( size < elemsCount ) {
    size *= 2;
  }
  return size * 2;
}

template<class UpdateType, class ResType,
  class UpdateNodePred, class MergeResPred,
  class MergeNodeToResPred, UpdateType defUpdate, ResType defRes>
  void RangeTree<UpdateType, ResType, UpdateNodePred, MergeResPred,
  MergeNodeToResPred, defUpdate, defRes>::update( int elem, const UpdateType& val )
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
  class MergeNodeToResPred, NodeType defUpdate, ResType defRes>
  ResType RangeTree<NodeType, ResType, UpdateNodePred, MergeResPred,
  MergeNodeToResPred, defUpdate, defRes>::queryRange( int first, int last )
{
  ResType leftRes = defRes;
  ResType rightRes = defRes;
  first += tree.size() / 2 - 1;
  last += tree.size() / 2 - 1;
  while( first <= last ) {
    if( first % 2 == 0 ) {
      leftRes = mergeResPred( leftRes, tree[first] );
    }
    if( last % 2 == 1 ) {
      rightRes = mergeResPred( tree[last], rightRes );
    }
    first = first / 2;
    last = last / 2 - 1;
  }
  return mergeResPred( leftRes, rightRes );
}

//-----------RangeTree.h----ends--------------
//-------------------------------------------------------
