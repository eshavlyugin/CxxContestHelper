template<class ValueType>
ValueType fastPower( const ValueType& val, const ValueType& one, int power )
{
  ValueType res = one;
  ValueType current = val;
  while( power > 0 ) {
    if( power % 2 == 1 ) {
      res = res * current;
    }
    power /= 2;
    current = current * current;
  }
  return res;
}
