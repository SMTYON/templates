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
        string s,text;cin >> s;
        int n = s.size();

        // failer function 
        //pi[i] = the size of the substring that is a prefix of s
        //and a sufix that ends in i.
        vector<int> pi(n, 0); 
    
        for (int i = 1; i < n; i++) {
            int k = pi[i-1];
            while (k > 0 && s[i] != s[k]) k = pi[k - 1];
            if (s[i] == s[k]) k++;
            pi[i] = k;
        }



        // Finds all starting indices of 'pattern' inside 'text'
        vector<int> matches;
        int n = text.size();
        int m = s.size();
        int k = 0; // 'k' represents how many characters of the pattern we've matched so far

        for (int i = 0; i < n; i++) {
            while (k > 0 && text[i] != s[k]) k = pi[k - 1];
            if (text[i] == s[k]) k++;
            if (k == m) {
                // Calculate the 0-based starting index in the text
                matches.push_back(i-m+1);
                // To find overlapping matches, fall back to the longest valid prefix
                k = pi[k - 1]; 
            }
        }
    }
        
    return 0;
}
