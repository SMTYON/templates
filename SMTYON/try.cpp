#include <bits/stdc++.h>
using namespace std;



queue<int> solve(queue<int> q, int v){
    queue<int> ans;
    int prev = -1;

    while(q.size()){
        int cur = q.front();
        q.pop();
        if(cur == v){
            ans.push(cur);
            cur = prev;
        }else if(prev != -1){
            ans.push(prev);
        }
        prev = cur;
    }
    ans.push(prev);
    return ans;
}

int main(){
    queue<int> a;
    a.push(1);
    a.push(2);
    a.push(3);

    a = solve(a,3);
    while(a.size()){
        cout << a.front() << ' ';
        a.pop();
    }
}
