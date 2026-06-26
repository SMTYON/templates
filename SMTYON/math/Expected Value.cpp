#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define ld long double
/*
E(x) = x * P(x).
E(x+y) = E(x) + E(y) -> dependant or independant.
E(x*y) = E(x) * E(y) -> only if independant.
E(x/y) != E(x) / E(y).
*/

ld fp(ld a , ll p){
    if(p == 0) return 1.0;
    ld res = fp(a,p/2);
    res *= res;
    if(p&1) res *= a;
    return res;
}