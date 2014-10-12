//------------------------------------------------------------------

struct SuffixArrayElem {
  int posInStr;
  int commonPref;

  SuffixArrayElem() : posInStr(0), commonPref(0) {}
  SuffixArrayElem( int _posInStr, int _commonPref ) : posInStr(_posInStr), commonPref(_commonPref) {}
};

class SuffixArray {
public:
  void initialize( const string& s )
  {
    int n = s.size();
    origStr = s;
    
    vector<int> buckets(n);
    vector<bool> bucketBegin(n);
    _presort( bucketBegin );
    vector<int> inv(n);
    vector<int> nextPrefixes(s.size());
    vector<int> counts(n);

    for( int prefLen = 1; prefLen < n; prefLen *= 2 ) {
      // initializing auxilary structures
      _fillBuckets( bucketBegin, buckets );
      std::fill( counts.begin(), counts.end(), 0 );
      for( int i = 0; i < n; i++ ) {
	inv[prefixes[i]] = i;
      }
      int curPt = 0;
      while( curPt < n ) {
	// finding boundaries of current buckets
	int nextPt = curPt;
	while( nextPt < n && buckets[nextPt] == buckets[curPt] ) {
	  nextPt++;
	}
	// first pass - calculating end of next buckets
	for( int i = curPt; i < nextPt; i++ ) {
	  if( prefixes[i] >= prefLen ) {
	    int inverted = inv[prefixes[i] - prefLen];
	    int bucket = buckets[inverted];
	    int idx = bucket + counts[bucket];
	    nextPrefixes[idx] = prefixes[inverted];
	    counts[bucket]++;
	  }
	}
	// second pass - marking ends of buckets
	for( int i = curPt; i < nextPt; i++ ) {
	  if( prefixes[i] >= prefLen ) {
	    int bucket = buckets[inv[prefixes[i] - prefLen]];
	    int idx = bucket + counts[bucket];
	    if( idx < n ) {
	      bucketBegin[idx] = true;
	    }
	  }
	}
	curPt = nextPt;
      }

      // dealing with prefixes that have length strictly less than prefLen
      for( int i = 0; i < n; i++ ) {
	if( prefixes[i] + prefLen >= s.size() ) {
	  int bucket = buckets[i];
	  int idx = bucket + counts[bucket];
	  nextPrefixes[idx] = prefixes[i];
	  counts[bucket]++;
	  idx++;
	  if( idx < s.size() ) {
	    bucketBegin[idx] = true;
	  }
	}
      }

      prefixes = nextPrefixes;
    }

    _calcLcp();
  }

  string getOriginalStr() { return origStr; }
  vector<int> getSortedPrefixes() const { return prefixes; }
  vector<int> getLcp() const { return lcp; }

private:
  string origStr;
  vector<int> prefixes;
  vector<int> lcp;

  void _presort( vector<bool>& bucketBegin )
  {
    int n = origStr.size();
    vector<int> buckets(256);
    vector<int> counts(256);
    prefixes.resize(n);

    // initial sorting
    for( int i = 0; i < n; i++ ) {
      counts[origStr[i]]++;
    }

    int curPt = 0;
    for( int i = 0; i < counts.size(); i++ ) {
      if( counts[i] != 0 ) {
	buckets[i] = curPt;
	bucketBegin[curPt] = true;
	curPt += counts[i];
      }
    }

    fill( counts.begin(), counts.end(), 0 );

    for( int i = 0; i < n; i++ ) {
      int idx = buckets[origStr[i]];
      prefixes[idx + counts[idx]] = i;
      counts[idx]++;
    }
  }

  void _fillBuckets( const vector<bool>& bucketBegin, vector<int>& buckets )
  {
    for( int i = 0; i < bucketBegin.size(); i++ ) {
      buckets[i] = bucketBegin[i] ? i : buckets[i-1];
    }
  }

  void _calcLcp()
  {
    int curLcp = 0;
    int n = origStr.size();
    lcp.resize(n);
    vector<int> inv(n);
    for( int i = 0; i < n; i++ ) {
      inv[prefixes[i]] = i;
    }
    for( int i = 0; i < prefixes.size(); i++ ) {
      int ind = inv[i];
      if( ind == 0 ) {
	continue;
      }
      while( prefixes[ind-1] + curLcp < n && prefixes[ind] + curLcp < n 
	     && origStr[prefixes[ind-1] + curLcp] == origStr[prefixes[ind] + curLcp] ) 
	{
	  curLcp++;
	}
      lcp[ind] = curLcp;
      if( curLcp > 0 ) {
	curLcp--;
      }
    }
  }
};

//------------------------------------------------------------------
