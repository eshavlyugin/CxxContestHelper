#include "../Likely.h"

void extEuclid( long long a, long long b, long long& x, long long& y )
{
  if( b == 0 ) {
    x = 1;
    y = 0;
  } else {
    extEuclid( b, a % b, y, x );
    y = y - (a / b) * x;
  }
}

long long invMod( long long x, long long mod )
{
  // todo: add checks
  long long a, b;
  extEuclid( x, mod, a, b );
  return (a % mod + mod) % mod;
}

template<long long modulo>
class ModP
{
public:
  ModP() { _value = 0; }
  ModP( long long val ) {
    _value = val;
  }

  long long value() { 
    if( _value > modulo ) {
      _value = _value % modulo;
    }
    return _value;
  }
  
  template<class NumType> 
  ModP& operator = (NumType x) {
    _value = x;
  }

  friend ModP<modulo> operator * (const ModP<modulo>& x, const ModP<modulo>& y)
  {
    if( UNLIKELY( x._value > modulo ) ) {
      x._value = x._value % modulo;
    }
    if( UNLIKELY( y._value > modulo ) ) {
      y._value = y._value % modulo;
    }
  
    return ModP<modulo>( x._value * y._value );
  }

  friend ModP<modulo> operator + (const ModP<modulo>& x, const ModP<modulo>& y )
  {
    if( UNLIKELY( x._value > 6 * modulo * modulo ) ) {
      x._value = x._value % modulo;
    }
    if( UNLIKELY( y._value > 6 * modulo * modulo ) ) {
      y._value = y._value % modulo;
    }
    return ModP<modulo>( x._value + y._value );
  }

  
  operator long long() { return value(); }

private:
  mutable long long _value;
};

template< long long modulo >
ostream& operator << (ostream& ost, ModP<modulo> x )
{
  ost << x.value();
  return ost;
}
