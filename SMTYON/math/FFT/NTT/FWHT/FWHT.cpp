ll mod = 998244353; // Set your modulo here
int add(int a, int b) { return (a + b) % mod;}
int sub(int a, int b) { return (a - b + mod) % mod;}
ll poww(ll a, ll b) {
  ll ret = 1;
  while (b) {
    if (b & 1) ret = ret * a % mod;
    a = a * a % mod;
    b >>= 1;
  }
  return ret;
}

void fwht(vector<int> &a, bool inv, int f) {
  int sz = a.size();
  for (int len = 1; 2 * len <= sz; len <<= 1) {
    for (int i = 0; i < sz; i += 2 * len) {
      for (int j = 0; j < len; j++) {
        int x = a[i + j];
        int y = a[i + j + len];
 
        if (f == 0) { // Bitwise AND
          if (!inv) {
            a[i + j] = add(x, y);
            a[i + j + len] = y;
          } else {
            a[i + j] = sub(x, y);
            a[i + j + len] = y;
          }
        }
        else if (f == 1) { // Bitwise OR
          if (!inv) {
            a[i + j] = x;
            a[i + j + len] = add(x, y);
          } else {
            a[i + j] = x;
            a[i + j + len] = sub(y, x);
          }
        }
        else if (f == 2) { // Bitwise XOR
          a[i + j] = add(x, y);
          a[i + j + len] = sub(x, y);
        }
      }   
    }
  }
}
 
/*
 * MAIN BLACK BOX FUNCTION: mul(a, b, f)
 * 
 * WHAT IT DOES:
 * Calculates the number of ways to combine elements from array `a` 
 * and array `b` using a bitwise operation `f`.
 * 
 * FLAG OPTIONS (f):
 * - 0 : AND (i & j == k)
 * - 1 : OR  (i | j == k)
 * - 2 : XOR (i ^ j == k)
 * 
 * HOW TO FORMAT INPUTS:
 * - a[i] = the number of items you have with value `i`.
 * - b[j] = the number of items you have with value `j`.
 * NOTE: The array size represents the maximum possible value. 
 * If your highest number is 1000, your array must be size 1001.
 * 
 * WHAT THE OUTPUT MEANS:
 * Returns a vector `result` where:
 * - result[k] = total combinations of one item from `a` and one 
 *               item from `b` where (i OP j) = k.
 */
vector<int> mul(vector<int> a, vector<int> b, int f) { 
  // 1. Auto-pad arrays to the nearest power of 2
  int max_sz = max(a.size(), b.size());
  int sz = 1;
  while (sz < max_sz) sz <<= 1;
  a.resize(sz, 0);
  b.resize(sz, 0);

  // 2. Transform both arrays to point-value representation
  fwht(a, false, f);  
  fwht(b, false, f);
  
  // 3. Multiply them in O(N)
  vector<int> c(sz);
  for (int i = 0; i < sz; ++i) {
    c[i] = 1ll * a[i] * b[i] % mod;
  }
  
  // 4. Transform back to coefficient representation
  fwht(c, true, f);
  
  // 5. Normalize (ONLY XOR requires division by N)
  if (f == 2) {
    int sz_inv = poww(sz, mod - 2);
    for (int i = 0; i < sz; ++i) {
      c[i] = 1ll * c[i] * sz_inv % mod;
    }
  }
  return c;
}