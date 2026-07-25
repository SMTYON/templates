#include <bits/stdc++.h>
using namespace std;
#define all(a)  a.begin(),a.end()
#define ll long long
#define int long long
#define ld long double
#define endl '\n'
#define SMTYON ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
const ll mod = 1e9+7 , inf = 1e9 , MAXN = 1e5+5;
#define matrix vector<vector<ll>>
#define zero(n,m) matrix(n , vector<ll>(m,0))

void init(matrix &M , matrix &res)
{
    // intialize the values of the matrices
    M[0][0] = 0 , M[0][1] = 1 , M[1][0] = 1 , M[1][1] = 1;
    res[0][0]=0 , res[1][0]=1;
}

matrix mul(matrix &a , matrix &b)
{
    int n = a.size() , m = b[0].size();
    matrix res = zero(n , m);

    for(int i=0; i<n; i++)
    {
        for(int j=0; j<m; j++)
        {
            for(int k=0; k<n; k++)
            {
                res[i][j] += a[i][k]*b[k][j];
                res[i][j] %= mod;
            }
        }
    }
    return res;
}

matrix matrix_fp(matrix &a , ll p)
{
    if (p==1) return a;
    matrix res = matrix_fp(a , p/2);
    res = mul(res , res);
    if (p%2) res = mul(res , a);

    return res;
}

int main()
{
    SMTYON
    ll n;
    cin >> n;
    if (n<2)
    {
        cout << n << "\n";
    }
    else
    {
        matrix M = zero(2,3) , A = zero(2,1);
        init(M , A);


        M = matrix_fp(M , n-2+1);
        A = mul(M , A);

        cout << A[1][0] << '\n';
    }

    return 0;
}



// if you have to matrices depend on i
matrix power(matrix a, ll p) {
    int n = a.size();
    matrix res = zero(n, n);
    for(int i = 0; i < n; i++) res[i][i] = 1; 
    
    while(p > 0) {
        if(p & 1) res = mul(res, a);
        a = mul(a, a);
        p >>= 1;
    }
    return res;
}

matrix matrix_fp(matrix &o , matrix &e , ll p , int type)// o -> odd
{
    if (p == 0) {
        int n = o.size();
        matrix res = zero(n, n);
        for(int i=0; i<n; i++) res[i][i] = 1;
        return res;
    }
    
    matrix base;
    if (type == 0) base = mul(o, e);
    else           base = mul(e, o);

    matrix res = power(base, p / 2);
    if (p % 2 == 1) {
        res = mul(res, (type == 0) ? o : e);
    }
    
    return res;
}