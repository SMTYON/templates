#include <bits/stdc++.h>
using namespace std;
#define all(a) a.begin(), a.end()
#define ll long long
#define ld long double
#define endl '\n'
#define SMTYON                                                                 \
  ios_base::sync_with_stdio(0);                                                \
  cin.tie(0);                                                                  \
  cout.tie(0);
const ll mod = 1e9 + 7, inf = 1e9, MAXN = 1e5 + 5;

// Basic Modular Operations:
ll add(ll a, ll b) { return (a + b + mod) % mod; }
ll mul(ll a, ll b) { return (a * b) % mod; }
ll fp(ll base, ll power) {
  if (!power)
    return 1;
  ll res = fp(base, power / 2);
  res = mul(res, res);
  if (power & 1)
    res = mul(res, base);
  return res;
}
ll inverse(ll n) { return fp(n, mod - 2); }
ll divide(ll a, ll b) { return mul(a, inverse(b)); }

// Combinatorics (nCr, nPr, Stars & Bars):
ll fact[MAXN], invFact[MAXN];
void precompute_factorials() {
  fact[0] = 1;
  invFact[0] = 1;
  for (int i = 1; i < MAXN; i++) {
    fact[i] = mul(fact[i - 1], i);
  }

  invFact[MAXN - 1] = inverse(fact[MAXN - 1]);
  for (int i = MAXN - 2; i >= 1; i--) {
    invFact[i] = mul(invFact[i + 1], i + 1);
  }
}

ll nCr(int n, int r) {
  if (r < 0 || r > n)
    return 0;
  return mul(fact[n], mul(invFact[r], invFact[n - r]));
}

ll nPr(int n, int r) {
  if (r < 0 || r > n)
    return 0;
  return mul(fact[n], invFact[n - r]);
}

// Stars and Bars
// Problem: Distribute n identical items into k distinct bins.
ll stars_and_bars(int n, int k) {
  if (k == 0)
    return (n == 0);
  return nCr(n + k - 1, k - 1);
}

ll catalan(int n) {
  ll numer = nCr(2 * n, n);
  ll denom = inverse(n + 1);
  return mul(numer, denom);
}

// Works for n up to 10^18, provided r is small (< 10^6)
// Complexity: O(r)
ll nCr_large_n(ll n, int r) {
  if (r < 0 || r > n)
    return 0;
  if (r == 0 || r == n)
    return 1;
  if (r > n / 2)
    r = n - r;

  ll numer = 1;
  for (int i = 0; i < r; i++) {
    numer = mul(numer, (n - i) % mod);
  }

  return mul(numer, invFact[r]);
}

// Returns number of ways to arrange n items such that none are in their
// original spot. Complexity: O(N) precompute, O(1) query.
vector<ll> Dn;
void precompute_derangements(int n) {
  Dn.resize(n + 1);
  Dn[0] = 1;
  Dn[1] = 0;
  for (int i = 2; i <= n; i++) {
    Dn[i] = mul(i - 1, add(Dn[i - 1], Dn[i - 2]));
  }
}

signed main() {
  SMTYON
  int x_x = 1;
  // cin >> x_x;
  while (x_x--) {
  }

  return 0;
}
