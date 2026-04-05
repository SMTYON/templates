struct Trie
{
  struct Node
  {
    int p = 0;
    int ch[26]{};
  };
  vector<Node> tree;
  Trie()
  {
    tree.emplace_back();
  }

  void insert(string s)
  {
    int node = 0;
    for (auto &it : s)
    {
      int c = it - 'a';
      if (tree[node].ch[c] == 0)
      {
        tree[node].ch[c] = tree.size();
        tree.emplace_back();
      }
      node = tree[node].ch[c];
      tree[node].p++;
    }
  }

  void erase(string s)
  {
    int node = 0;
    for (auto &it : s)
    {
      int c = it - 'a';
      node = tree[node].ch[c];
      tree[node].p--;
    }
  }

  int query(string s)
  {
    int node = 0;
    int ret = 0;
    for (auto &it : s)
    {
      int c = it - 'a';
      if (tree[tree[node].ch[c]].p == 0)
        return ret;
      node = tree[node].ch[c];
      ret++;
    }
    return ret;
  }
};