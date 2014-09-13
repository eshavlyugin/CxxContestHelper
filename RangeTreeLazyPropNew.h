#include "Intervals.h"
#include "Predicates.h"

//-----------RangeTreeLazyPropNew.h----begins------
//-------------------------------------------------

/* updateLazyPred - binary predicate: 
   UpdatePred <*> UpdatePred = UpdatePred */
/* mergeResPred - binary predicate:
   ResPred <*> ResPred = ResPred */
/* splitLazyPred - ternary predicate: 
   f( UpdateType, Range, Subrange ) = UpdateType */
/* lazyToResPred - binary predicate:
   f( UpdateType, RangeType ) = lazyToResPred */
template<class RangeType, class UpdateType, class ResType,
  class UpdateLazyPred, class MergeResPred, 
  class SplitLazyPred, class MergeLazyToResPred,
  UpdateType defUpdate = UpdateType(), ResType defRes = ResType()>
class RangeTreeLazyProp {
public:
  RangeTreeLazyProp( const RangeType& universe ) 
  {
    root = new RangeTreeNode( universe );
  }
  ~RangeTreeLazyProp() 
  {
    _freeRangeNode( root );
  }

  // update a given range with a given value
  void update( const RangeType& range, const UpdateType& value ) {
    return _update( root, value, range );
  }

  // query the value of a given interval
  ResType queryRange( const RangeType& range ) {
    return _queryRange( root, range );
  }

private:
  // structure representing a single node of the RangeTree
  struct RangeTreeNode {
    /* range covered by the given node */
    RangeType range;
    /* lazy value of update operations that hasn't been propagated yet */
    UpdateType lazy; 
    /* saved value of query entire range operation 
       (without lazy value of node) */
    ResType saved;
    /* left node */
    RangeTreeNode* left;
    /* right node */
    RangeTreeNode* right;
    /* is the value saved */
    bool hasLazy;
    
    RangeTreeNode( const RangeType& _range ) 
    : range(_range), left(NULL), right(NULL),
      hasLazy(false), lazy(defUpdate), saved(defRes) {}
  };

  // root element
  RangeTreeNode* root;

  /* predicate objects */
  UpdateLazyPred updateLazyPred;
  MergeResPred mergeResPred;
  SplitLazyPred splitLazyPred;
  MergeLazyToResPred mergeLazyToResPred;

  void _update( RangeTreeNode* node, const UpdateType& updateVal, 
		const RangeType& updateRange );
  ResType _queryRange( RangeTreeNode* node, const RangeType& queryRange );
  void _freeRangeNode( RangeTreeNode* node );
  void _updateNodeValue( RangeTreeNode* node );
};


template<class RangeType, class UpdateType, class ResType,
  class UpdateLazyPred, class MergeResPred,
  class SplitLazyPred, class MergeLazyToResPred,
  UpdateType defUpdate, ResType defRes>
  void RangeTreeLazyProp<RangeType, UpdateType, ResType,
  UpdateLazyPred, MergeResPred, SplitLazyPred, MergeLazyToResPred, 
  defUpdate, defRes>::_freeRangeNode( RangeTreeNode* node )
{
  if( node == NULL ) {
    return;
  }
  _freeRangeNode( node->left );
  _freeRangeNode( node->right );
  delete node;
}

template<class RangeType, class UpdateType, class ResType,
  class UpdateLazyPred, class MergeResPred, 
  class SplitLazyPred, class MergeLazyToResPred,
  UpdateType defUpdate, ResType defRes>
  void RangeTreeLazyProp<RangeType, UpdateType, ResType,
  UpdateLazyPred, MergeResPred, SplitLazyPred, MergeLazyToResPred, 
  defUpdate, defRes>::_update( 
			      RangeTreeNode* node, 
			      const UpdateType& updateVal, 
			      const RangeType& updateRange )
{
  // no intersection - nothing to update
  if( !updateRange.intersectWith( node->range ) ) {
    return;
  }
  // if the range is completely inside another range
  if( node->range.inside( updateRange ) ) {
    UpdateType tmpVal = splitLazyPred( updateVal, node->range, updateRange );
    node->lazy = updateLazyPred( node->lazy, tmpVal );
    node->hasLazy = true;
    _updateNodeValue(node);
    return;
  }
  // general case

  // is the node hasn't been expanded yet, expand it
  if( node->left == NULL && node->right == NULL ) {
    RangeType leftRange, rightRange;
    node->range.split( leftRange, rightRange );
    node->left = new RangeTreeNode( leftRange );
    node->right = new RangeTreeNode( rightRange );
  }
  //!todo update left lazy
  if( node->hasLazy ) {
    node->left->lazy = updateLazyPred( node->left->lazy, 
				       splitLazyPred( node->lazy, 
						      node->range,
						      node->left->range) );
    node->left->hasLazy = true;
    _updateNodeValue( node->left );
    node->right->lazy = updateLazyPred( node->right->lazy, 
					splitLazyPred( node->lazy, 
						       node->range,
						       node->right->range) );
    node->right->hasLazy = true;
    _updateNodeValue( node->right );
    node->lazy = defUpdate;
    node->hasLazy = false;
  }

  // first update childs
  _update( node->left, updateVal, updateRange );
  _update( node->right, updateVal, updateRange );
  // then merge values of a child
  _updateNodeValue( node );
}

template<class RangeType, class UpdateType, class ResType,
  class UpdateLazyPred, class MergeResPred, 
  class SplitLazyPred, class MergeLazyToResPred,
  UpdateType defUpdate, ResType defRes>
  ResType RangeTreeLazyProp<RangeType, UpdateType, ResType,
  UpdateLazyPred, MergeResPred, SplitLazyPred, MergeLazyToResPred,
  defUpdate, defRes>::_queryRange( 
				  RangeTreeNode* node, 
				  const RangeType& range )
{
  // already in the leaf
  if( node == NULL ) {
    return defRes;
  }
  if( !range.intersectWith( node->range ) ) {
    return defRes;
  }
  if( node->range.inside( range ) ) {
    return node->saved;
  }
  ResType res = _queryRange( node->left, range );
  res = mergeResPred( res, _queryRange( node->right, range ) );
  if( node->hasLazy ) {
    res = mergeLazyToResPred( res, node->lazy, node->range.intersect(range) );
  }
  return res;
}

template<class RangeType, class UpdateType, class ResType,
  class UpdateLazyPred, class MergeResPred,
  class SplitLazyPred, class MergeLazyToResPred,
  UpdateType defUpdate, ResType defRes>
  void RangeTreeLazyProp<RangeType, UpdateType, ResType,
  UpdateLazyPred, MergeResPred, SplitLazyPred, MergeLazyToResPred,
  defUpdate, defRes>::_updateNodeValue( RangeTreeNode* node )
{
    node->saved = defRes;
    if( node->left != NULL ) {
      node->saved = mergeResPred( node->saved, node->left->saved );
    }
    if( node->right != NULL ) {
      node->saved = mergeResPred( node->saved, node->right->saved );
    }
    if( node->hasLazy ) {
      node->saved = mergeLazyToResPred( node->saved, node->lazy, node->range );
    }
}

// some useful instantinations of a range tree structures
template<class Type, class RangeType>
  struct RangeSumTree {
    typedef RangeTreeLazyProp<RangeType, Type, Type,
      NumSumPredicate, NumSumPredicate, 
      IdentityPredicate, RangeSumPredicate, 0, 0> type;
  };

//-----------RangeTreeLazyPropNew.h----ends--------------
//-------------------------------------------------------
