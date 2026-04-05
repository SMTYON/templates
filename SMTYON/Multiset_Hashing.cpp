const int mod = 1e9 + 7;
const int B = 2;
int r[B];
#define multiHash array<int, B>
int add(int a, int b) { return (a % mod + b % mod + mod) % mod; }
int mul(int a, int b) { return (a % mod * b % mod) % mod; }
int fp(int b, int e)
{
  if (!e)
    return 1;
  int ret = fp(b, e >> 1);
  ret = mul(ret, ret);
  return (e & 1 ? mul(ret, b) : ret);
}

void pre()
{
  mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
  for (int i = 0; i < B; ++i)
    r[i] = rng() + 1;
}

class MultisetHash
{
  vector<multiHash> h;
  vector<int> v;
  int n;

public:
  MultisetHash(vector<int> &v)
  {
    static bool init = []()
    {
      pre();
      return true;
    }();
    this->v = v;
    n = v.size();
    h.resize(n + 1);
    h[0][0] = h[0][1] = 1;
    for (int i = 1; i <= n; ++i)
    {
      int cur = v[i - 1];
      for (int j = 0; j < B; ++j)
      {
        h[i][j] = mul(h[i - 1][j], add(cur, r[j]));
      }
    }
  }
  int get_hash(int l, int r) // 1-based
  {
    multiHash ret;
    for (int i = 0; i < B; ++i)
    {
      ret[i] = mul(h[r][i], fp(h[l - 1][i], mod - 2));
    }
    return (ret[0] | (ret[1] << 32));
  }
};