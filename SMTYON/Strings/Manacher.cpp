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
        string tm;
        cin >> tm;
        string s = "#";
        for(auto x : tm) {
            s += x;
            s += '#';
        }
        int n = s.size();
        /*
        s = # + tm[0] + # + t[1] + # + ........ + #
        because if the palindrom has an even lenght without the # will
        be a pain, but now we can say that a # is a center of an even palindrom
        ( make it an odd palindrom with a # as a center).
        */
       vector<int> man(n);
       for(int i = 0,l=0,r=-1; i < n ;i++){
        /* if i is in the calculated range take your answer from the prevuios
        calculations NOTE:(the prevuios calcultion can make you bigger the the 
        caluclated range which is not right as you do not know what after r
        that is why we take the minemum with r-i)
        else the answer is 1 as any char is palindrom. 
        */
            int k = (i < r)? min(man[l+(r-i)],(r-i)): 1; 
            while(i-k >= 0 && i+k < n && s[i-k] == s[i+k])k++;
            // if the current calculated range is bigger save it
            if(i+k > r){
                l = i-k;
                r = i+k;
            }
            // k-1 is the length of the palindrom in the original string (tm)
            man[i] = k-1;
       }

       /*
       if i need to the length of the ith char in the original string
       1- i is the center of an odd palindrom -> man[2*i + 1]
       2- i is the char after the center in an even palindrom -> man[2*i]
       example:
       tm = "aaaab"
       i = 2.
       man[2*i + 1] = 3 -> "_aaa_"
       man[2*i]     = 4 -> "aaa_"
       */


       

    }
        
    return 0;
}
