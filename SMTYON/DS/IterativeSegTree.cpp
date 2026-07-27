#include <bits/stdc++.h>
using namespace std;

template <typename T> struct IterativeSegTree {
  int n;
  vector<T> tree;

  // Default value for out-of-bounds or neutral element
  // For sum: 0, For max: -INF, For min: INF
  const T NEUTRAL = 0;

  IterativeSegTree(int size) {
    n = size;
    tree.assign(2 * n, NEUTRAL);
  }

  IterativeSegTree(const vector<T> &a) {
    n = a.size();
    tree.assign(2 * n, NEUTRAL);
    build(a);
  }

  // The combination operation. Change this for Max, Min, GCD, etc.
  T combine(T a, T b) { return a + b; }

  // O(N) build
  void build(const vector<T> &a) {
    for (int i = 0; i < n; i++) {
      tree[n + i] = a[i];
    }
    for (int i = n - 1; i > 0; i--) {
      tree[i] = combine(tree[i << 1], tree[i << 1 | 1]);
    }
  }

  // Point update: set a[p] = value in O(log N)
  void update(int p, T value) {
    for (tree[p += n] = value; p > 1; p >>= 1) {
      tree[p >> 1] = combine(tree[p], tree[p ^ 1]);
    }
  }

  // Range query in [l, r) in O(log N)
  T query(int l, int r) {
    T resL = NEUTRAL, resR = NEUTRAL;
    for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
      if (l & 1)
        resL = combine(resL, tree[l++]);
      if (r & 1)
        resR = combine(tree[--r], resR);
    }
    return combine(resL, resR);
  }
};

int main() {
  // Example usage
  vector<long long> a = {5, 2, 8, 1, 3};
  IterativeSegTree<long long> seg(a);

  // Query sum from index 1 to 3 (values: 2, 8, 1) -> expects 11
  cout << "Sum [1, 4): " << seg.query(1, 4) << "\n";

  // Update index 2 to 10 (array becomes: 5, 2, 10, 1, 3)
  seg.update(2, 10);

  // Query sum from index 1 to 3 again (values: 2, 10, 1) -> expects 13
  cout << "Sum [1, 4): " << seg.query(1, 4) << "\n";

  return 0;
}