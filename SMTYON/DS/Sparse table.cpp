struct Sparse {
  vector<int> log;
  vector<vector<int>> table;
  Sparse(int n, vector<int> &a) {
    log.resize(n + 5);
    log[1] = 0;
    // ->   i   -> 10101011
    // -> i / 2 -> 01010101
    for (int i = 2; i <= n; i++) {
      log[i] = log[i / 2] + 1;
    }

    table = vector<vector<int>>(n + 5, vector<int>(log[n] + 5, 0));
    // table[N][log(N)]
    for (int i = 0; i < n; i++)
      table[i][0] = a[i];
    // levels
    for (int j = 1; j <= log[n]; j++) {

      for (int k = 0; k + (1 << j) <= n; k++) {
        table[k][j] = __gcd(table[k][j - 1], table[(1 << (j - 1)) + k][j - 1]);
      }
    }
  }
  int query(int l, int r) {
    // l--, r--; // For 1 based query
    int x = r - l + 1;
    x = log[x];
    return __gcd(table[l][x], table[r - (1 << x) + 1][x]);
  }
};