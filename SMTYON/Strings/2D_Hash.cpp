const int N = 2005;
const int B = 2;
const int MOD[B] = {(int) 1e9 + 7, (int) 1e9 + 9};
const int bx[B] = {29, 31};
const int by[B] = {37, 41};

int pwx[B][N], pwy[B][N];
int invx[B][N], invy[B][N];
int invBx[B], invBy[B];

#define multiHash array<int, B>

int add(int a, int b, int mod) { return (a + b) % mod; }
int sub(int a, int b, int mod) { return (a - b + mod) % mod; }
int mul(int a, int b, int mod) { return (1LL * a * b) % mod; }

int fp(int a, int b, int mod) {
    int ret = 1;
    while (b) {
        if (b & 1)
            ret = mul(ret, a, mod);
        a = mul(a, a, mod);
        b >>= 1;
    }
    return ret;
}

void pre2D() {
    for (int k = 0; k < B; k++) {
        pwx[k][0] = pwy[k][0] = invx[k][0] = invy[k][0] = 1;
        invBx[k] = fp(bx[k], MOD[k] - 2, MOD[k]);
        invBy[k] = fp(by[k], MOD[k] - 2, MOD[k]);
        for (int i = 1; i < N; i++) {
            pwx[k][i] = mul(pwx[k][i - 1], bx[k], MOD[k]);
            pwy[k][i] = mul(pwy[k][i - 1], by[k], MOD[k]);
            invx[k][i] = mul(invx[k][i - 1], invBx[k], MOD[k]);
            invy[k][i] = mul(invy[k][i - 1], invBy[k], MOD[k]);
        }
    }
}

class Hash2D {
    vector<vector<multiHash>> h;
    int n, m;

public:
    Hash2D(vector<string> &grid) {
        static bool init = []() {
            pre2D();
            return true;
        }();
        n = grid.size();
        m = grid[0].size();
        h.assign(n + 1, vector<multiHash>(m + 1));
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                for (int k = 0; k < B; k++) {
                    int val = mul(mul(grid[i - 1][j - 1] - 'a' + 1, pwx[k][i], MOD[k]), pwy[k][j], MOD[k]);
                    h[i][j][k] = add(add(sub(add(h[i - 1][j][k], h[i][j - 1][k], MOD[k]), h[i - 1][j - 1][k], MOD[k]),
                                         val, MOD[k]),
                                     0, MOD[k]);
                }
            }
        }
    }

    multiHash get_hash(int x1, int y1, int x2, int y2) // 1-based
    {
        multiHash ret;
        for (int k = 0; k < B; k++) {
            int cur = h[x2][y2][k];
            cur = sub(cur, h[x1 - 1][y2][k], MOD[k]);
            cur = sub(cur, h[x2][y1 - 1][k], MOD[k]);
            cur = add(cur, h[x1 - 1][y1 - 1][k], MOD[k]);
            // normalize
            cur = mul(cur, invx[k][x1 - 1], MOD[k]);
            cur = mul(cur, invy[k][y1 - 1], MOD[k]);
            ret[k] = cur;
        }
        return ret;
    }
};