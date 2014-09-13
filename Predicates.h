//--------------------------------------------------------------------
//--------------------------------------------------------------------

struct NumMinPredicate {
  template<class NumType>
  NumType operator()( const NumType& t1, const NumType& t2 )
  {
    return std::min( t1, t2 );
  }
};

struct NumMaxPredicate {
  template<class NumType>
  NumType operator()( const NumType& t1, const NumType& t2 )
  {
    return std::max( t1, t2 );
  }
};

struct NumSumPredicate {
  template<class NumType>
  NumType operator() (const NumType& t1, const NumType& t2 )
  {
    return t1 + t2;
  }

  //!todo: move somewhere
  template<class NumType, class DoesntMatter>
  NumType operator() (const NumType& t1, const NumType& t2, 
		      const DoesntMatter& )
  {
    return t1 + t2;
  }
};


struct IdentityPredicate {
  template<class ValueType, class DoesntMatter, class DoesntMatter2>
    ValueType operator() ( const ValueType& res, DoesntMatter&, DoesntMatter2& )
  {
    return res;
  }
};

struct RangeSumPredicate {
  template<class ValueType, class IntervalType>
    ValueType operator() ( const ValueType& val, const ValueType& val2,
			   const IntervalType& ii )
  {
    return val + val2 * ii.area();
  }
};

//-------------------------------------------------------------------
//-------------------------------------------------------------------
