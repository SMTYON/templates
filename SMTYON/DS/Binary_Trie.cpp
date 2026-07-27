struct BinaryTrie {
  const int BIT = 31;
  struct Node {
    int ch[2]{};
    int p = 0;
  };
  vector<Node> tree;
  BinaryTrie() dp sos {
    tree.emplace_back();
    insert(0);
  }

  void insert(int x) {
    int node = 0;
    for (int i = BIT; ~i; --i) {
      int bit = (1 << i);
      bool cur = (x & bit);
      if (tree[node].ch[cur] == 0) {
        tree[node].ch[cur] = tree.size();
        tree.emplace_back();
      }
      node = tree[node].ch[cur];
      tree[node].p++;
    }
  }

  void erase(int x) {
    int node = 0;
    for (int i = BIT; ~i; --i) {
      int bit = (1 << i);
      bool cur = (x & bit);
      node = tree[node].ch[cur];
      tree[node].p--;
    }
  }

  int mx_xor(int x) {
    int node = 0;
    int ret = 0;
    for (int i = BIT; ~i; --i) {
      int bit = (1 << i);
      bool cur = (x & bit);
      if (tree[tree[node].ch[cur ^ 1]].p) {
        ret += (1 << i);
        cur ^= 1;
      }
      node = tree[node].ch[cur];
    }
    return ret;
  }
};