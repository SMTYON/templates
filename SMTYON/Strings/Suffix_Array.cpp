#include <bits/stdc++.h>
using namespace std;
#define all(a)  a.begin(),a.end()
#define SMTYON ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

void count_sort(vector<int>& p ,vector<int>& c){
    int n = p.size();
    vector<int> cnt(n),pos(n);
    for(auto x : c) cnt[x]++;
    for(int i = 1 ;i < n ;i++) pos[i] = pos[i-1]+cnt[i-1];

    vector<int> new_p(n);
    for(int i = 0 ;i <n ;i++){
        int idx = c[p[i]];
        new_p[pos[idx]] = p[i];
        pos[idx]++;
    }
    p = new_p;
}

signed main() {
    SMTYON
    
    string s;
    cin >> s;
    s += '$';
    //!! n is the size of the string + '$' 
    int n = s.size();
    // IF YOU ARE GOING TO COMBINE TWO STRINGS THE SIPRATORS 
    // HAS TO BE DIFF -> s + '#' + t + '$'.

    // p[i] = the index where the lexicographically smallest ith suffix starts from.
    // c[i] = which category the suffix i belongs to (equal suffixs belongs to same category)
    /*
    test:
        ababba
    p:
        6 5 0 2 4 1 3
    lcp:
        0 0 1 2 0 2 1 
    */

    // Time = O(n*log^2(n))
    {
        vector<int> p(n),c(n);
        // base case
        vector<pair<char ,int>> a(n);
        for(int i =0 ;i < n;i++) a[i] = {s[i] , i};
        sort(all(a));
        for(int i =0 ;i < n;i++) p[i] = a[i].second;
        c[p[0]] = 0;
        for(int i =1 ;i < n;i++) c[p[i]] = c[p[i-1]] + (a[i].first != a[i-1].first);

        // transition
        int k = 0;
        while((1 << k) < n){
            vector<pair<pair<int,int> ,int>> a(n);
            for(int i =0 ;i < n;i++){
                a[i] = {{c[i] , c[(i+(1<<k)) % n]},i};
            }
            sort(all(a));
            for(int i =0 ;i < n;i++) p[i] = a[i].second;
            c[p[0]] = 0;
            for(int i =1 ;i < n;i++) c[p[i]] = c[p[i-1]] + (a[i].first != a[i-1].first);
            k++;
        }
    }

    // Time = O(n*log(n))
    vector<int> p(n),c(n);
    {
        // base case:
        vector<pair<char ,int>> a(n);
        for(int i =0 ;i < n;i++) a[i] = {s[i],i};
        sort(all(a));
        for(int i =0 ;i < n;i++) p[i] = a[i].second;
        c[p[0]] = 0;
        for(int i = 1; i < n;i++) c[p[i]] = c[p[i-1]] + (a[i].first != a[i-1].first);

        // iterations:
        int k = 0;
        while((1 << k) < n){
            /*
                we take binifit of the old p is already sorted
                so we add the first part of the new p to the old p
                and only sort on the first part as the second is allready sorted.
                new_p[i] = (2^k new chars) + old_p[i];
            */
            // transition to new p:
            for(int i = 0 ;i <n ;i ++) p[i] = (p[i] - (1 << k) + n)%n;

            // sort on the first half:
            count_sort(p,c);
            vector<int> new_c(n);
            new_c[p[0]] = 0;
            for(int i = 1 ;i < n;i++){
                pair<int,int> prev = {c[p[i-1]] , c[(p[i-1] + (1 << k))%n]};
                pair<int,int> cur = {c[p[i]] , c[(p[i] + (1 << k))%n]};
                new_c[p[i]] = new_c[p[i-1]] + (prev != cur);          
            }
            c = new_c;
            k++;
        }

    }


    // longest common prefix LCP: O(n)
    vector<int> lcp(n);
    int k = 0;
    for(int i =0 ;i < n-1 ;i++){
        int idx = c[i];
        if(idx == 0)continue;
        int j = p[idx-1];
        while(s[i+k] == s[j+k]) k++;
        lcp[idx] = k;
        k = max(0 , k-1);
    }
    /*
    lcp[i] = lcp[ s[pos[i],.....n-1] , s[pos[i-1],.........n-1] ].
    to find the lcp between suffix starts at i and a suffix starts at j:
    min(lcp[i+1] , lcp[i+2], ....., lcp[j]).
    this is normal range query which you can use segment tree or sparse table.
    */

}
