//----------------------------------------------------
//---------------------------------------------------

template<class CoordType>
struct Interval1d {
  CoordType st; // left coordinate of the interval
  CoordType fin; // right coordinate of the interval

  // constructors
  Interval1d() : st(0), fin(0) {}
  Interval1d( const CoordType& _st, const CoordType& _fin ) 
  {
    st = _st;
    fin = _fin;
    if( st > fin ) {
      std::swap( st, fin );
    }
  }

  bool intersectWith( const Interval1d& other ) const
  {
    return !(st > other.fin || fin < other.st);
  }

  bool inside( const Interval1d& other ) const
  {
    return st >= other.st && fin <= other.fin;
  }
  
  Interval1d intersect( const Interval1d& other ) const
  {
    // assert isIntersects
    return Interval1d( std::max( st, other.st ), std::min( fin, other.fin ) );
  }

  void split( Interval1d& left, Interval1d& right ) const
  {
    left.st = st;
    left.fin = (st + fin) / 2;
    right.st = left.fin + 1;
    right.fin = fin;
  }
  
  CoordType area() const
  {
    return fin - st + 1;
  }
};

template<class NumType>
inline ostream& operator << ( ostream& ost, const Interval1d<NumType>& i )
{
  return ost << "Int(" << i.st << "," << i.fin << ")";
}

template<class CoordType>
struct Interval2d {
  CoordType stX; // left coordinate of the interval
  CoordType stY; // right coordinate of the interval
  CoordType finX; // right coordinate of the interval
  CoordType finY; // right coordinate of the interval

  // constructors
  Interval2d() : stX(0), finX(0), stY(0), finY(0) {}
  Interval2d( const CoordType& _stX, const CoordType& _finX,
	      const CoordType& _stY, const CoordType& _finY) 
  {
    stX = _stX;
    finX = _finX;
    stY = _stY;
    finY = _finY;
    if( stY > finY ) {
      std::swap( stY, finY );
    }
    if( stX > finX ) {
      std::swap( stX, finX );
    }
  }

  bool intersectWith( const Interval2d& other ) const
  {
    return !(stX > other.finX || finX < other.stX)
      && !(stY > other.finY || finY < other.stY);
  }

  bool inside( const Interval2d& other ) const
  {
    return stX >= other.stX && finX <= other.finX
      && stY >= other.stY && finY <= other.finY;
  }
  
  Interval2d intersect( const Interval2d& other ) const
  {
    // assert isIntersects
    return Interval2d(std::max(stX, other.stX), std::min(finX, other.finX),
		      std::max(stY, other.stY), std::min(finY, other.finY));
  }

  void split( Interval2d& left, Interval2d& right ) const
  {
    if( finY - stY >= finX - stX ) {
      left.stY = stY;
      left.finY = (stY + finY) / 2;
      right.stY = left.finY + 1;
      right.finY = finY;
      left.stX = stX;
      left.finX = finX;
      right.stX = stX;
      right.finX = finX;
    } else {
      left.stX = stX;
      left.finX = (stX + finX) / 2;
      right.stX = left.finX + 1;
      right.finX = finX;
      left.stY = stY;
      left.finY = finY;
      right.stY = stY;
      right.finY = finY;
    }
  }
  
  CoordType area() const
  {
    return (finX - stX + 1) * (finY - stY + 1);
  }
};

template<class NumType>
inline ostream& operator << ( ostream& ost, const Interval2d<NumType>& i )
{
  return ost << "Int(" << i.stX << "," << i.finX << "," << i.stY << "," << i.finY << ")";
}

template<class CoordType>
struct Interval3d {
};

//----------------------------------------------------
//---------------------------------------------------
