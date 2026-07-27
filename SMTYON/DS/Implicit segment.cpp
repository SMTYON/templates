#include <iostream>
#include <vector>

using namespace std;

struct Node {
  ll sum;
  int left_child;
  int right_child;
} NEUTRAL = {0, 0, 0};

struct ImplicitSegTree {
  ll MAX_R; // The maximum possible coordinate (e.g., 1e9)
  vector<Node> tree;
  int node_count;
  int root;

  // max_range is the upper bound of your array (e.g., 1e9 or 2e9)
  // expected_updates is usually Q (number of queries/updates)
  ImplicitSegTree(ll max_range, int expected_updates) {
    MAX_R = max_range;

    // Depth is roughly log2(max_range). For 1e9, it's ~30.
    // So each update creates at most ~32 nodes.
    int max_nodes = (expected_updates * 32) + 2;
    tree.resize(max_nodes);

    node_count = 1;
    root = create_node(); // Initialize the root node (index 1)
  }

  int create_node() {
    int id = node_count++;
    tree[id].sum = 0;
    tree[id].left_child = 0;
    tree[id].right_child = 0;
    return id;
  }

  Node merge(Node a, Node b) {
    Node res;
    res.sum = a.sum + b.sum;
    return res;
  }

  // Point update at index i (0-based)
  void update(ll i, ll val, int x, ll lx, ll rx) {
    if (rx - lx == 1) {
      tree[x].sum += val;
      return;
    }

    ll m = (rx + lx) / 2;

    if (i < m) {
      if (!tree[x].left_child)
        tree[x].left_child = create_node();
      update(i, val, tree[x].left_child, lx, m);
    } else {
      if (!tree[x].right_child)
        tree[x].right_child = create_node();
      update(i, val, tree[x].right_child, m, rx);
    }

    Node left_node = tree[x].left_child ? tree[tree[x].left_child] : NEUTRAL;
    Node right_node = tree[x].right_child ? tree[tree[x].right_child] : NEUTRAL;

    // IMPORTANT: Only update the sum. Do not overwrite tree[x] completely.
    tree[x].sum = merge(left_node, right_node).sum;
  }

  // Zero-based Range Query [l, r)
  Node query(ll l, ll r, int x, ll lx, ll rx) {
    if (lx >= r || rx <= l || x == 0)
      return NEUTRAL;
    if (lx >= l && rx <= r)
      return tree[x];

    ll m = (rx + lx) / 2;
    Node left_res = query(l, r, tree[x].left_child, lx, m);
    Node right_res = query(l, r, tree[x].right_child, m, rx);

    return merge(left_res, right_res);
  }

  void update(ll i, ll val) { update(i, val, root, 0, MAX_R); }
  ll query(ll l, ll r) { return query(l, r, root, 0, MAX_R).sum; }
};

int main() {
  int expected_updates = 100000;
  ll max_coordinate = 1000000000; // 1e9

  ImplicitSegTree st(max_coordinate, expected_updates);

  // Update coordinates that are extremely far apart
  st.update(5, 10);
  st.update(500000000, 20); // 5 * 10^8
  st.update(999999999, 30); // 10^9 - 1

  // Range queries over massive empty spaces are perfectly fast and safe
  cout << "Sum [0, 10): " << st.query(0, 10) << "\n";                 // 10
  cout << "Sum [0, 600000000): " << st.query(0, 600000000) << "\n";   // 30
  cout << "Sum [0, 1000000000): " << st.query(0, 1000000000) << "\n"; // 60

  return 0;
}