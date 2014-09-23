class StringHashFunction {
public:
  StringHashFunction( const string& st, unsigned long long mult = -1 );

  // calculate hash function for substring that beg
  unsigned long long calcHash(int startIdx, int length);

private:
  string _originalStr;
  // i'th element of array contains the following sum:
  // s[0] * p^len + s[1] * p^{len-1} + ...
  vector<unsigned long long> _partialSums;
  vector<unsigned long long> _multPowers;

  unsigned long long _mult;

  void _initHashFunction();
};

StringHashFunction::StringHashFunction( const string& st, 
					unsigned long long mult )
{
  _originalStr = st;
  if( mult < 0 ) {
    _mult = (rand() % 500 + 100) & ~1;
  } else {
    _mult = mult;
  }
  _initHashFunction();
}

unsigned long long StringHashFunction::calcHash( int startIndex, int len )
{
  // todo: add assertions
  unsigned long long res = _partialSums[startIndex+len-1];
  res -= startIndex == 0 ? 0 : _partialSums[startIndex-1];
  return res * (startIndex == 0 ? 1 : _multPowers[startIndex-1]);
}

void StringHashFunction::_initHashFunction()
{
  int len = _originalStr.size();
  if( len == 0 ) {
    return;
  }
  _multPowers.resize(len);
  _partialSums.resize(len);
  _multPowers[0] = _mult;
  for( int i = 1; i < len; i++ ) {
    _multPowers[i] = _multPowers[i-1] * _mult;
  }
  _partialSums[0] = _originalStr[0] * _multPowers[len-1];
  for( int i = 1; i < len; i++ ) {
    _partialSums[i] = _partialSums[i-1] + 
      _originalStr[i] * _multPowers[len-i-1];
  }
}
