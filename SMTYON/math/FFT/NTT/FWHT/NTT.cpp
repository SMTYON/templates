// USE generator TO CLACULATE THE NEW root FOR YOUR mod !!!!!!!!!

//mod has to be a prime of the form c * 2^k + 1 (like 998244353)
// Default is set to 998244353 and its primitive root 3.
#define int ll
ll mod = 998244353; 
ll root = 3;        

int modpow(int b, int e, int m = mod) {
    int ans = 1;
    for (; e; b = (ll)b * b % m, e /= 2)
        if (e & 1) ans = (ll)ans * b % m;
    return ans;
}

// Primitive Root of the mod of form 2^a * b + 1
int generator () {
    vector<int> fact;
    int phi = mod-1,  n = phi;
    for (int i=2; i*i<=n; ++i)
        if (n % i == 0) {
            fact.push_back (i);
            while (n % i == 0)
                n /= i;
        }
    if (n > 1)
        fact.push_back (n);

    for (int res=2; res<=mod; ++res) {
        bool ok = true;
        for (size_t i=0; i<fact.size() && ok; ++i)
            ok &= modpow (res, phi / fact[i]) != 1;
        if (ok)  return res;
    }
    return -1;
}



void ntt(vector<int> &a) {
    int n = (int)a.size(), L = 31 - __builtin_clz(n);
    vector<int> rt(2, 1);
    for (int k = 2, s = 2; k < n; k *= 2, s++) {
        rt.resize(n);
        int z[] = {1, modpow(root, mod >> s, mod)};
        for (int i = k; i < 2*k; ++i) rt[i] = (ll)rt[i / 2] * z[i & 1] % mod;
    }
    vector<int> rev(n);
    for (int i = 0; i < n; ++i) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
    for (int i = 0; i < n; ++i) if (i < rev[i]) swap(a[i], a[rev[i]]);
    for (int k = 1; k < n; k *= 2) {
        for (int i = 0; i < n; i += 2 * k) {
            for (int j = 0; j < k; ++j) {
                int z = (ll)rt[j + k] * a[i + j + k] % mod, &ai = a[i + j];
                a[i + j + k] = ai - z + (z > ai ? mod : 0);
                ai += (ai + z >= mod ? z - mod : z);
            }
        }
    }
}

/*
 * MAIN FUNCTION FOR NTT-FRIENDLY PRIMES (e.g., 998244353)
 * 
 * Usage: vector<int> res = conv(a, b);
 * Returns array where res[k] = sum(a[i] * b[j]) % mod for i+j == k.
 * Automatically sizes the output array to exactly a.size() + b.size() - 1.
 */
vector<int> conv(const vector<int> &a, const vector<int> &b) {
    if (a.empty() || b.empty()) return {};
    int s = (int)a.size() + (int)b.size() - 1, B = 32 - __builtin_clz(s), n = 1 << B;
    int inv = modpow(n, mod - 2, mod);
    vector<int> L(a), R(b), out(n);
    L.resize(n), R.resize(n);
    ntt(L), ntt(R);
    for (int i = 0; i < n; ++i) out[-i & (n - 1)] = (ll)L[i] * R[i] % mod * inv % mod;
    ntt(out);
    return {out.begin(), out.begin() + s};
}


// =======================================================================
// PART 2: ARBITRARY MODULO (Use if modulo is 10^9+7 or not NTT-friendly)
// =======================================================================

// The modulo you actually want your final answer in
int desired_mod = 1000000007;

// Three standard NTT-friendly primes and their roots
const int mod1 = 167772161, root1 = 3;
const int mod2 = 469762049, root2 = 3;
const int mod3 = 754974721, root3 = 11;

/* Chinese Remainder Theorem to stitch the 3 convolutions together */
int CRT(int a, int b, int c, int m1, int m2, int m3) {
    __int128 M = (__int128)m1*m2*m3;
    ll M1 = (ll)m2*m3;
    ll M2 = (ll)m1*m3;
    ll M3 = (ll)m2*m1;

    int M_1 = modpow(M1%m1, m1 - 2, m1);
    int M_2 = modpow(M2%m2, m2 - 2, m2);
    int M_3 = modpow(M3%m3, m3 - 2, m3);

    __int128 ans = (__int128)a*M1*M_1;
    ans += (__int128)b*M2*M_2;
    ans += (__int128)c*M3*M_3;

    return (ans % M) % desired_mod;
}

/*
 * MAIN FUNCTION FOR NON-FRIENDLY PRIMES (e.g., 10^9+7)
 * 
 * Usage: vector<int> res = arbitrary_mod_conv(a, b);
 * Runs the convolution 3 times and stitches the result. 
 * Expected to be ~3x slower than conv().
 */
vector<int> arbitrary_mod_conv(const vector<int>& a, const vector<int>& b) {
    // 1st pass
    mod = mod1; root = root1;
    vector<int> res1 = conv(a, b);
    
    // 2nd pass
    mod = mod2; root = root2;
    vector<int> res2 = conv(a, b);
    
    // 3rd pass
    mod = mod3; root = root3;
    vector<int> res3 = conv(a, b);
    
    // Restore global mod just in case it's used elsewhere
    mod = 998244353; root = 3;
    
    int n = max({res1.size(), res2.size(), res3.size()});
    vector<int> final_res(n);
    
    for (int i = 0; i < n; i++) {
        int v1 = (i < res1.size()) ? res1[i] : 0;
        int v2 = (i < res2.size()) ? res2[i] : 0;
        int v3 = (i < res3.size()) ? res3[i] : 0;
        
        final_res[i] = CRT(v1, v2, v3, mod1, mod2, mod3);
    }
    
    return final_res;
}