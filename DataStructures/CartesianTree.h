//--------------------------------------------------------------
//--------------------------------------------------------------

template<class NodeType>
struct CartesianTreeNode {
  CartesianTreeNode* left;
  CartesianTreeNode* right;
  NodeType value;
  int heapVal;
  int refCount;
  int elemCount;

  // helpers
  typedef pair< CartesianTreeNode*, CartesianTreeNode* > SplitResult;

  CartesianTreeNode( CartesianTreeNode* leftSubtree, CartesianTreeNode* rightSubtree,
		     const NodeType& _value, int _heapVal )
  {
    left = leftSubtree;
    right = rightSubtree;
    value = _value;
    heapVal = _heapVal;
    elemCount = 1;
    refCount = 0;
    if( left != NULL ) {
      left->refCount++;
      elemCount += left->elemCount;
    }
    if( right != NULL ) {
      right->refCount++;
      elemCount += right->elemCount;
    }
  }

  static SplitResult split( CartesianTreeNode* current, const NodeType& value )
  {
    if( current == NULL ) {
      return SplitResult( NULL, NULL );
    }
    if( current->value > value ) {
      SplitResult splitRes = split( current->left, value );
      CartesianTreeNode* newRight 
	= new CartesianTreeNode( splitRes.second, current->right,
				 current->value, current->heapVal );
      return SplitResult( splitRes.first, newRight );
    } else {
      SplitResult splitRes = split( current->right, value );
      CartesianTreeNode* newLeft 
	= new CartesianTreeNode( current->left, splitRes.first,
				 current->value, current->heapVal );
      return SplitResult( newLeft, splitRes.second );
    }
  }

  static CartesianTreeNode* merge( CartesianTreeNode* left,
				   CartesianTreeNode* right )
  {
    if( left == NULL ) {
      return right;
    }
    if( right == NULL ) {
      return left;
    }
    if( left->heapVal > right->heapVal ) {
      CartesianTreeNode* newRight = merge( left->right, right );
      CartesianTreeNode* result 
	= new CartesianTreeNode( left->left, newRight,
				 left->value, left->heapVal );
      return result;
    } else {
      CartesianTreeNode* newLeft = merge( left, right->left );
      CartesianTreeNode* result 
	= new CartesianTreeNode( newLeft, right->right,
				 right->value, right->heapVal );
      return result;
    }
  }
};

template<class NodeType>
class CartesianTree {
 public:
  typedef CartesianTreeNode<NodeType> TreeNode;
  typedef typename CartesianTreeNode<NodeType>::SplitResult SplitResult;

  explicit CartesianTree( TreeNode* _root )
  {
    root = _root;
    if( root != NULL ) {
      root->refCount++;
    }
  }

  CartesianTree( const CartesianTree& other )
    {
      root = other.root;
      cerr << "Copy constructor" << endl;
      if( root != NULL ) {
	root->refCount++;
      }
    }

  CartesianTree& operator = ( const CartesianTree& other )
    {
      root = other.root;
      cerr << "operator =" << endl;
      if( root != NULL ) {
	root->refCount++;
      }
      return *this;
    }

 CartesianTree() : root(NULL) {}

  ~CartesianTree()
    {
      _decRef( root );
    }

  CartesianTree insertValue( const NodeType& value )
  {
    int heapVal = rand();
    cerr << "inserting (" << heapVal << ", " << value << ")" << endl;
    return CartesianTree( _insert( root, value, heapVal ) );
  }

  // returns true if element was deleted from the tree
  CartesianTree deleteValue( NodeType& value )
  {
    return CartesianTree( _delete( root, value ) );
  }

  CartesianTree extractSubtree( const NodeType& beg, const NodeType& fin, bool remove )
  {
    return CartesianTree();
  }

  int findOrderOf( const NodeType& value )
  {
    return _findOrderOf( root, value );
  }

  NodeType findNthValue( int n )
  {
    return _findNthValue( root, n );
  }

 private:
  // root of the tree
  TreeNode* root;

  void _decRef( TreeNode* node )
  {
    if( node == NULL ) {
      return;
    }
    node->refCount--;
    if( node->refCount == 0 ) {
      cerr << "deleting node " << node << endl;
      _decRef( node->left );
      _decRef( node->right );
      delete node;
    }
  }

  TreeNode* _insert( TreeNode* node, const NodeType& value, int heapVal )
  {
    if( node == NULL ) {
      return new TreeNode( NULL, NULL, value, heapVal );
    }
    if( node->heapVal < heapVal ) {
      SplitResult res;
      res = TreeNode::split( node, value );
      return new TreeNode( res.first, res.second, value, heapVal );
    } else if( value < node->value ) {
      return new TreeNode( _insert( node->left, value, heapVal ), node->right, node->value, node->heapVal );
    } else {
      return new TreeNode( node->left, _insert( node->right, value, heapVal ), node->value, node->heapVal );
    }
  }

  TreeNode* _delete( TreeNode* node, const NodeType& value )
  {
    if( node == NULL ) {
      return NULL;
    }
    if( node->value == value ) {
      return TreeNode::merge( node->left, node->right );
    } else if( node->value > value ) {
      return new TreeNode( _delete( node->left, value ), node->right, node->value, node->heapVal );
    } else {
      return new TreeNode( node->left, _delete( node->right, value ), node->value, node->heapVal );
    }
  }

  NodeType _findNthValue( TreeNode* elem, int n )
  {
    if( elem == NULL ) {
      throw std::invalid_argument("Invalid argument");
    }
    int count = elem->left == NULL ? 0 : elem->left->elemCount;
    if( count == n ) {
      return elem->value;
    } else if( count > n ) {
      return _findNthValue( elem->left, n );
    } else {
      return _findNthValue( elem->right, n - count - 1 );
    }
  }

  int _findOrderOf( TreeNode* elem, const NodeType& value )
  {
    if( elem == NULL ) {
      throw std::invalid_argument("Invalid argument");
    }
    int leftCount = elem->left == NULL ? 0 : elem->left->elemCount;
    if( elem->value == value ) {
      return leftCount;
    } else if( elem->value > value ) {
      return _findOrderOf( elem->left, value );
    } else {
      return leftCount + _findOrderOf( elem->right, value ) + 1;
    }
  }
};

//--------------------------------------------------------------
