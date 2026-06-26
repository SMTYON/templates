#include <bits/stdc++.h>
using namespace std;
#define all(a)  a.begin(),a.end()
#define ll long long
#define ld long double
#define endl '\n'
#define SMTYON ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
const ll mod = 1e9+7 , inf = 1e9 , MAXN = 1e5+5;

vector<short> spf(MAXN);

void sieve() {
    for(int i = 2 ;i*i <= MAXN ; i++){
        if(spf[i])continue;
        for(int j = i*i ;j <= MAXN ; j += i){
            spf[j] = i;
        }
    }
}

vector<int> getFactorization(int x) {
    vector<int> factors;
    while (x > 1) {
        int p = spf[x];
        if(!p) p = x;
        factors.push_back(p);
        x /= p;
    }
    return factors;
}

signed main() {
    SMTYON
    int x_x = 1;
    //cin >> x_x;
    while (x_x--) {
        
    }
        
    return 0;
}
