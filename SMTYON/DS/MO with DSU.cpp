struct Edge {
  int u, v;
};

struct DSU {
  vector<int> par, sz;
  int comp;
  struct State {
    int u, v; // u becomes child of v
    bool united;
  };
  vector<State> history;

  DSU(int n) {
    par.resize(n);
    iota(all(par), 0);
    sz.assign(n, 1);
    comp = n;
  }

  int find(int i) {
    while (par[i] != i)
      i = par[i];
    return i;
  }

  bool unite(int i, int j) {
    int root_i = find(i);
    int root_j = find(j);
    if (root_i != root_j) {
      if (sz[root_i] < sz[root_j])
        swap(root_i, root_j);
      // Attach root_j to root_i
      history.push_back({root_j, root_i, true});
      par[root_j] = root_i;
      sz[root_i] += sz[root_j];
      comp--;
      return true;
    }
    history.push_back({-1, -1, false}); // No merge happened
    return false;
  }

  void rollback() {
    if (history.empty())
      return;
    State last = history.back();
    history.pop_back();
    if (last.united) {
      // last.u was attached to last.v
      par[last.u] = last.u;
      sz[last.v] -= sz[last.u];
      comp++;
    }
  }

  int snapshot() { return history.size(); }

  void rollbackTo(int snapshot) {
    while (history.size() > snapshot) {
      rollback();
    }
  }

  void reset(int n) {
    iota(par.begin(), par.end(), 0);
    sz.assign(n, 1);
    history.clear();
    comp = n;
  }
};

int block_size;

struct Query {
  int l, r, idx;
  bool operator<(const Query &other) const {
    if (l / block_size != other.l / block_size)
      return l / block_size < other.l / block_size;
    return r < other.r;
  }
};

vector<int> mo(int n, const vector<Edge> &edges, vector<Query> queries) {
  int m = edges.size();
  block_size = sqrt(m) + 1;
  vector<int> answers(queries.size());
  sort(all(queries));

  DSU dsu(n);
  int num_blocks = (m + block_size - 1) / block_size;
  int q_idx = 0;

  for (int b = 0; b < num_blocks; b++) {
    dsu.reset(n);

    int block_end = min(m, (b + 1) * block_size);
    int cur_r = block_end;

    while (q_idx < queries.size() && queries[q_idx].l / block_size == b) {
      Query q = queries[q_idx];

      if (q.r < block_end) {
        int snap = dsu.snapshot();
        for (int i = q.l; i <= q.r; i++) {
          dsu.unite(edges[i].u, edges[i].v);
        }
        answers[q.idx] = dsu.comp;
        dsu.rollbackTo(snap);
      }

      else {
        while (cur_r <= q.r) {
          dsu.unite(edges[cur_r].u, edges[cur_r].v);
          cur_r++;
        }
        int snap = dsu.snapshot();
        for (int i = q.l; i < block_end; i++) {
          dsu.unite(edges[i].u, edges[i].v);
        }

        answers[q.idx] = dsu.comp;
        dsu.rollbackTo(snap);
      }
      q_idx++;
    }
  }
  return answers;
}

void SM() {
  int n, m, q;
  cin >> n >> m >> q;

  vector<Edge> edges(m);
  for (int i = 0; i < m; i++) {
    cin >> edges[i].u >> edges[i].v;
    edges[i].u--;
    edges[i].v--;
  }

  vector<Query> queries(q);
  for (int i = 0; i < q; i++) {
    cin >> queries[i].l >> queries[i].r;
    queries[i].l--;
    queries[i].r--;
    queries[i].idx = i;
  }

  vector<int> ans = mo(n, edges, queries);

  for (auto x : ans)
    cout << x << endl;
}

signed main() {
  SMTYON
  int x_x = 1;
  cin >> x_x;
  while (x_x--) {
    SM();
  }

  return 0;
}
