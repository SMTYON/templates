
struct Node {
  bool lazy = 0;
} NEUTRAL;

struct SegTree {
  int size;
  vector<Node> tree;

  SegTree(int n) {
    size = 1;
    while (size < n)
      size *= 2;
    tree.resize(2 * size);
  }

  Node merage(Node a, Node b) {
    Node res;

    return res;
  }
  void build(vector<int> &a, int x, int lx, int rx) {
    if (rx - lx == 1) {
      if (lx < a.size()) {
      };
      return;
    }
    int m = (lx + rx) / 2;
    build(a, 2 * x + 1, lx, m);
    build(a, 2 * x + 2, m, rx);
    tree[x] = merage(tree[2 * x + 1], tree[2 * x + 2]);
  }

  void push(int x, int lx, int rx) {
    if (tree[x].lazy) {
      tree[x].lazy = 0;
    }
  }

  void update(int l, int r, int x, int lx, int rx) {
    push(x, lx, rx);
    if (lx >= r || rx <= l)
      return;
    if (lx >= l && rx <= r) {
      tree[x].lazy = 1;
      push(x, lx, rx);
      return;
    }
    int m = (lx + rx) / 2;
    update(l, r, 2 * x + 1, lx, m);
    update(l, r, 2 * x + 2, m, rx);
    tree[x] = merage(tree[2 * x + 1], tree[2 * x + 2]);
  }

  // zero based Range Query [l,r)
  // Node query(int l, int r, int x, int lx, int rx)
  // {
  //     push(x, lx, rx);
  //     if (lx >= r || rx <= l) return NEUTRAL;
  //     if (lx >= l && rx <= r) return tree[x];
  //     int m = (lx + rx)/2;
  //     return merage(query(l, r, 2*x+1, lx, m),query(l, r, 2*x+2, m, rx));
  // }

  // zero based index Query i
  // Node query(int i, int x, int lx, int rx)
  // {
  //     push(x, lx, rx);
  //     if(rx-lx == 1){
  //         return tree[x];
  //     }
  //     int m = (lx + rx)/2;
  //     if(i < m) return query(i, 2*x+1, lx, m);
  //     else return query(i, 2*x+2, m, rx);
  // }

  void build(vector<int> &a) { build(a, 0, 0, size); }
  void update(int l, int r) { update(l, r, 0, 0, size); }
  Node query(int l, int r) { return query(l, r, 0, 0, size); }
};