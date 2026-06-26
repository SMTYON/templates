const int N = 1e5 + 5, mod = 1e9 + 7;
const int B = 2;
const int b[] = {29, 31};
int pw[B][N], inv[B][N], invB[B];
int n;

#define multiHash array<int, B>
int add(int a, int b) { return (a % mod + b % mod + mod) % mod; }
int mul(int a, int b) { return (a % mod * b % mod) % mod; }
int fp(int a, int b) {
    if (!b)
        return 1;
    int ret = fp(a, b >> 1);
    ret = mul(ret, ret);
    return (b & 1 ? mul(ret, a) : ret);
}

void pre() {
    for (int i = 0; i < B; ++i) {
        pw[i][0] = inv[i][0] = 1;
        invB[i] = fp(b[i], mod - 2);
    }
    for (int i = 1; i < N; ++i)
        for (int base = 0; base < B; ++base)
            pw[base][i] = mul(pw[base][i - 1], b[base]), inv[base][i] = mul(inv[base][i - 1], invB[base]);
}

class Hash {
private:
    vector<multiHash> h;
    string s;
    int n;

public:
    int hash_char(char c, int i, int base) { return mul(c - 'a' + 1, pw[base][i]); }
    Hash(string s) {
        static bool init = []() {
            pre();
            return true;
        }();
        this->s = s;
        n = s.size();
        h.resize(n + 1);
        for (int i = 1; i <= n; ++i)
            for (int base = 0; base < B; ++base)
                h[i][base] = add(h[i - 1][base], hash_char(s[i - 1], i, base));
    }
    multiHash get_hash(int l, int r) { // one based !!!
        multiHash ret;
        for (int base = 0; base < B; ++base) {
            ret[base] = mul(add(h[r][base], -h[l - 1][base]), inv[base][l - 1]);
        }
        return ret;
    }
};

multiHash merge_hashes(const multiHash& h1, const multiHash& h2, int len1) {
    multiHash res;
    for (int i = 0; i < B; ++i) {
        // Formula: Hash(A) + Hash(B) * Base^Length(A)
        int shifted_h2 = mul(h2[i], pw[i][len1]); 
        res[i] = add(h1[i], shifted_h2);
    }
    return res;
}

bool isPalindrome(int l, int r, int n, Hash &fh, Hash &bh) // 0-based
{
    return fh.get_hash(l + 1, r + 1) == bh.get_hash(n - r, n - l);
}

