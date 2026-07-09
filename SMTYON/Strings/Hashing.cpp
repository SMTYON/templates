#include <bits/stdc++.h>
using namespace std;

//! DONOT FORGET TO CHANGE N
const int N = 4e5+5;
const int B = 2; 
const int mods[3] = {1000000007, 1000000009, 998244353};
int base[B];
int pw[B][N], inv[B][N];

inline int add(int a, int b, int m) { 
    int res = a+b; 
    return res >= m ? res-m : res; 
}
inline int sub(int a, int b, int m) { 
    int res = a-b; 
    return res < 0 ? res+m : res; }
inline int mul(int a, int b, int m) { return (1LL * a * b) % m; }

int fp(int b, int power, int m) {
    int res = 1;
    b %= m;
    while (power > 0) {
        if (power & 1) res = mul(res, b, m);
        b = mul(b, b, m);
        power >>= 1;
    }
    return res;
}

//! DONOT FORGET TO CALL IN MAIN
void initPows() {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    
    for (int k = 0; k < B; k++) {
        uniform_int_distribution<int> dist(300, mods[k]-2);
        base[k] = dist(rng);
        
        pw[k][0] = inv[k][0] = 1;
        int b_inv = fp(base[k], mods[k]-2, mods[k]);
        
        for (int i = 1; i < N; i++) {
            pw[k][i] = mul(pw[k][i-1], base[k], mods[k]);
            inv[k][i] = mul(inv[k][i-1], b_inv, mods[k]);
        }
    }
}

struct Agent73 {
    // 1-based indexing for prefixes
    vector<array<int, B>> pref;
    
    Agent73(const string& s) {
        int n = s.size();
        pref.assign(n+1, {});
        
        for (int i = 1; i <= n; i++) {
            for (int k = 0; k < B; k++) {
                // val * B^(i-1)
                int char_val = mul(s[i-1], pw[k][i-1], mods[k]); 
                pref[i][k] = add(pref[i-1][k], char_val, mods[k]);
            }
        }
    }
    
    // l and r are 1-based
    array<int, B> getHash(int l, int r) {
        array<int, B> res;
        for (int k = 0; k < B; k++) {
            // (Pref[r]-Pref[l-1]) * Inv[l-1]
            int raw_diff = sub(pref[r][k], pref[l-1][k], mods[k]);
            res[k] = mul(raw_diff, inv[k][l-1], mods[k]);
        }
        return res;
    }
};