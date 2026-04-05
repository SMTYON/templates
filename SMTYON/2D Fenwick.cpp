struct FenwickTree2D {// 1-based
    int n, m;
    vector<vector<int>> tree;

    FenwickTree2D(int n_size, int m_size) {
        n = n_size;
        m = m_size;
        tree.assign(n+1, vector<int>(m+1, 0));
    }

    // Point update: Add val to coordinate (x, y)
    void update(int x, int y, int val) {
        for (int i = x; i <= n; i += i & -i) {
            for (int j = y; j <= m; j += j & -j) {
                tree[i][j] += val;
            }
        }
    }

    // Prefix query: Sum of rectangle from (1, 1) to (x, y)
    int query(int x, int y) {
        int sum = 0;
        for (int i = x; i > 0; i -= i & -i) {
            for (int j = y; j > 0; j -= j & -j) {
                sum += tree[i][j];
            }
        }
        return sum;
    }

    int query_range(int x1, int y1, int x2, int y2) {
        return query(x2, y2) 
             - query(x1 - 1, y2) 
             - query(x2, y1 - 1) 
             + query(x1 - 1, y1 - 1);
    }

};