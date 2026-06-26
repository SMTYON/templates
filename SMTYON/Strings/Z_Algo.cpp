#include <bits/stdc++.h>
using namespace std;
#define all(a)  a.begin(),a.end()
#define ll long long
#define ld long double
#define endl '\n'
#define SMTYON ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
const ll mod = 1e9+7 , inf = 1e9 , MAXN = 1e5+5;


signed main() {
    SMTYON
    int x_x = 1;
    //cin >> x_x;
    while (x_x--) {
        // the size of the substring that is a prefix of s[0,n-1] and also a prefix of s[i,n-1].
        string s;cin >> s;
        int n = s.size();
        // z[0] = 0   -> will not change any thing in the  logic
        vector<int> z(n);
        for(int i=1,l=0,r=-1; i <n ;i++){
            /* if i is in the range that we allready explored then use the old calculations.
               else start from the begining with the naiv solution.    
            */
            int k = (i<r)? min(z[(i-l)],(r-i)):0;
            while(i+k < n && s[k] == s[i+k]) k++;
            // if we explored a bigger rang store it.
            if(i+k > r){
                l = i;
                r = i+k;
            }
            z[i] = k;
        }
    }
        
    return 0;
}
