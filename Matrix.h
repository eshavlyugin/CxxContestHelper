template<class ElemType>
class Matrix {
public:
  Matrix( int _w, int _h ) {
    w = _w;
    h = _h;
    a = new ElemType[w * h];
  }

  Matrix( const Matrix& m )
    {
      w = m.w;
      h = m.h;
      a = new ElemType[w * h];
      memcpy( a, m.a, sizeof(ElemType) * w * h );
    }

  Matrix& operator = ( const Matrix& m ) {
    w = m.w;
    h = m.h;
    a = new ElemType[w * h];
    memcpy( a, m.a, sizeof(ElemType) * w * h );
    return *this;
  }

  ~Matrix()
  {
    delete[] a;
  }

  // element access
  ElemType& elem( int x, int y ) { return a[x * w + y]; }
  const ElemType& elem( int x, int y ) const { return a[x * w + y]; }

  // width/height
  int width() const { return w; }
  int height() const { return h; }

  static Matrix<ElemType> diagonal( int size, const ElemType& x )
  {
    Matrix<ElemType> res(size, size);
    for( int i = 0; i < size; i++ ) {
      res.elem(i,i) = x;
    }
    return res;
  }

  Matrix transpose() const
  {
    Matrix res(h, w);
    for( int i = 0; i < w; i++ ) {
      for( int j = 0; j < h; j++ ) {
	res.elem(j, i) = elem(i, j);
      }
    }
    return res;
  }

  // traingulize matrix
  // x y ....
  // 0 0 x1 y1 z1 ...
  // 0 0  0  0  0 x2 ..
  // method doesn't swap columns
  /*Matrix triangulize() {
    for( int i = 0; i < w; i++ ) {
      for( int j = 0; j < h; j++ ) {
	for( int k = i; k < h; k++ ) {
	}
      }
    }
    }*/

  // a^-1
  /*Matrix invert() {
    return Matrix();
    }*/

private:
  int w;
  int h;
  ElemType* a;
};

template<class ElemType>
Matrix<ElemType> operator * (const Matrix<ElemType>& m1, 
			     const Matrix<ElemType>& m2)
{
  // todo: assert m1.height() == m2.width()
  Matrix<ElemType> res( m1.width(), m2.height() );
  Matrix<ElemType> tmp = m2.transpose();
  for( int i = 0; i < m1.width(); i++ ) {
    for( int j = 0; j < m2.height(); j++ ) {
      for( int k = 0; k < m1.height(); k++ ) {
	res.elem(i,j) = res.elem(i,j) + m1.elem(i,k) * tmp.elem(j,k);
      }
    }
  }
  return res;
}

template<class ElemType>
ostream& operator << (ostream& ost, const Matrix<ElemType>& m )
{
  for( int i = 0; i < m.width(); i++ ) {
    for( int j = 0; j < m.height(); j++ ) {
      ost << m.elem(i,j) << " ";
    }
    ost << "\n";
  }
  return ost;
}
