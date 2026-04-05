const int mod = 1e9 + 7;

// 1. Basic Modular Operations
// ---------------------------------------------------------

ll add(ll a, ll b) {return (a + b) % mod;}
ll sub(ll a, ll b) {return ((a - b) % mod + mod) % mod;}
ll mul(ll a, ll b) {return (a * b) % mod;}
ll fp(ll base, ll power) {
    if(!power) return 1;
    ll res = fp(base,power/2);
    res = mul(res,res);
    if(power&1) res = mul(res,base);
    return res;
}
ll inverse(ll n) {return fp(n, mod - 2);}
ll divide(ll a, ll b) {return mul(a, inverse(b));}


// 2. Factorization
//----------------------------------------------------------
const int MAXN = 1e7+5; 
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

// 3. Combinatorics (nCr, nPr, Stars & Bars)
// ---------------------------------------------------------

const int MAX_N = 2e5 + 5; 
ll fact[MAX_N], invFact[MAX_N];
void precompute_factorials() {
    fact[0] = 1;
    invFact[0] = 1;
    for (int i = 1; i < MAX_N; i++) {
        fact[i] = mul(fact[i - 1], i);
    }

    invFact[MAX_N - 1] = inverse(fact[MAX_N - 1]);
    for (int i = MAX_N - 2; i >= 1; i--) {
        invFact[i] = mul(invFact[i + 1], i + 1);
    }
}

ll nCr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return mul(fact[n], mul(invFact[r], invFact[n - r]));
}

ll nPr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return mul(fact[n], invFact[n - r]);
}

// Stars and Bars
// Problem: Distribute n identical items into k distinct bins.
ll stars_and_bars(int n, int k) {
    if (k == 0) return (n == 0);
    return nCr(n + k - 1, k - 1);
}

long long catalan(int n) {
    long long numer = nCr(2 * n, n);
    long long denom = inverse(n + 1);
    return mul(numer, denom);
}


// Works for n up to 10^18, provided r is small (< 10^6)
// Complexity: O(r)
long long nCr_large_n(long long n, int r) {
    if (r < 0 || r > n) return 0;
    if (r == 0 || r == n) return 1;
    if (r > n / 2) r = n - r; 
    
    long long numer = 1;
    for (int i = 0; i < r; i++) {
        numer = mul(numer, (n - i) % mod);
    }
    
    return mul(numer, invFact[r]); 
}


// Returns number of ways to arrange n items such that none are in their original spot.
// Complexity: O(N) precompute, O(1) query.
vector<long long> Dn;
void precompute_derangements(int n) {
    Dn.resize(n + 1);
    Dn[0] = 1; 
    Dn[1] = 0;
    for(int i = 2; i <= n; i++) {
        Dn[i] = mul(i - 1, add(Dn[i - 1], Dn[i - 2]));
    }
}


// Solves a^x = b (mod m)
// Returns -1 if no solution found.
// Complexity: O(sqrt(m) * log(m))
long long discrete_log(long long a, long long b, long long m) {
    a %= m; b %= m;
    long long k = 1, add = 0, g;
    while ((g = gcd(a, m)) > 1) {
        if (b == k) return add;
        if (b % g) return -1;
        b /= g; m /= g; add++;
        k = (k * 1ll * a / g) % m;
    }

    long long n = sqrt(m) + 1;
    long long an = 1;
    for (int i = 0; i < n; ++i)
        an = (an * 1ll * a) % m;

    map<long long, int> vals;
    long long cur = b;
    for (int q = 0; q <= n; ++q) {
        vals[cur] = q;
        cur = (cur * 1ll * a) % m;
    }

    cur = k;
    for (int p = 1; p <= n; ++p) {
        cur = (cur * 1ll * an) % m;
        if (vals.count(cur)) {
            long long ans = n * p - vals[cur] + add;
            return ans;
        }
    }
    return -1;
}

// 4. Fibonacci (Matrix Exponentiation)
// ---------------------------------------------------------

void multiply_matrix(ll F[2][2], ll M[2][2]) {
    ll a = add(mul(F[0][0], M[0][0]), mul(F[0][1], M[1][0]));
    ll b = add(mul(F[0][0], M[0][1]), mul(F[0][1], M[1][1]));
    ll c = add(mul(F[1][0], M[0][0]), mul(F[1][1], M[1][0]));
    ll d = add(mul(F[1][0], M[0][1]), mul(F[1][1], M[1][1]));
    
    F[0][0] = a; F[0][1] = b;
    F[1][0] = c; F[1][1] = d;
}

void power_matrix(ll F[2][2], ll n) {
    if (n == 0 || n == 1) return;
    ll M[2][2] = {{1, 1}, {1, 0}};
    
    power_matrix(F, n / 2);
    multiply_matrix(F, F);
    
    if (n % 2 != 0)
        multiply_matrix(F, M);
}

// Returns the nth Fibonacci number modulo mod
// Complexity: O(log n) - Efficient for n up to 10^18
ll fibonacci(ll n) {
    if (n == 0) return 0;
    ll F[2][2] = {{1, 1}, {1, 0}};
    power_matrix(F, n - 1);
    return F[0][0];
}

// 1. Euler's Totient Function (Phi)
// Counts the number of integers in range [1, n] that are coprime to n.
int phi(int n) {
    int result = n;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            while (n % i == 0)
                n /= i;
            result -= result / i; 
        }
    }
    if (n > 1)
        result -= result / n;
    return result;
}

// 2. Precompute Phi for 1 to n (Sieve-like approach)
// Useful if you need phi for many numbers up to N.
// Complexity: O(N log(log N))
void phi_1_to_n(int n) { 
    vector<int> phi(n + 1);
    
    for (int i = 0; i <= n; i++)
        phi[i] = i;

        
    for (int i = 2; i <= n; i++) {
        if (phi[i] == i) { // i is prime
            for (int j = i; j <= n; j += i)
                phi[j] -= phi[j] / i; 
        }
    }
}

// 3. Binary GCD (Stein's Algorithm)
// Faster than standard recursive Euclidean GCD on some hardware.
int gcd(int a, int b) {
    if (!a || !b) return a | b;
    
    unsigned shift = __builtin_ctz(a | b);
    a >>= __builtin_ctz(a); 
    
    do {
        b >>= __builtin_ctz(b); 
        if (a > b) swap(a, b);
        b -= a; 
    } while (b);
    
    return a << shift; 
}

// 4. Extended Euclidean Algorithm
// Solves: a*x + b*y = gcd(a,b)
// Returns: gcd(a,b). Updates x and y with a valid solution.
int egcd(int a, int b, int& x, int& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    int x1, y1;
    int d = egcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}

// 5. Linear Diophantine Equation: Find ANY solution
// Solves: a*x + b*y = c
// Returns: true if solution exists, updates x0, y0, and g=gcd(a,b).
bool find_any_solution(int a, int b, int c, int &x0, int &y0, int &g) {
    g = egcd(abs(a), abs(b), x0, y0);
    if (c % g) {
        return false; // No solution if c is not divisible by gcd
    }

    // Scale the solution from egcd (which solved for =g) to =c
    x0 *= c / g;
    y0 *= c / g;
    
    // Fix signs because egcd assumed positive a, b
    if (a < 0) x0 = -x0;
    if (b < 0) y0 = -y0;
    return true;
}

bool find_positive_solutions(int a, int b, int c, int &x, int &y, int &g) {
    if (!find_any_solution(a, b, c, x, y, g)) return false;

    double k_min = ceil(-(double)x * g / b);
    
    double k_max = floor((double)y * g / a);

    if (k_min <= k_max) {
        int k = (int)k_min;
        x = x + k * (b / g);
        y = y - k * (a / g);
        return true;
    }

    return false; 
}


void shift_solution(int & x, int & y, int a, int b, int cnt) {
    x += cnt * b;
    y -= cnt * a;
}

// 6. Linear Diophantine Equation: Count ALL solutions in range
// Counts solutions to a*x + b*y = c where minx <= x <= maxx and miny <= y <= maxy.
int find_all_solutions(int a, int b, int c, int minx, int maxx, int miny, int maxy) {
    int x, y, g;
    if (!find_any_solution(a, b, c, x, y, g))
        return 0;
    if(a == 0 && b == 0) {
        if (c == 0) return (ll)(maxx - minx + 1) * (maxy - miny + 1);
        return 0; 
    }
    if(a == 0 || b == 0) return 1; 
    
    a /= g;
    b /= g;

    int sign_a = a > 0 ? +1 : -1;
    int sign_b = b > 0 ? +1 : -1;

    shift_solution(x, y, a, b, (minx - x) / b);
    if (x < minx) shift_solution(x, y, a, b, sign_b);
    if (x > maxx) return 0;
    int lx1 = x;

    shift_solution(x, y, a, b, (maxx - x) / b);
    if (x > maxx) shift_solution(x, y, a, b, -sign_b);
    int rx1 = x;

    shift_solution(x, y, a, b, -(miny - y) / a);
    if (y < miny) shift_solution(x, y, a, b, -sign_a);
    if (y > maxy) return 0;
    int lx2 = x;

    shift_solution(x, y, a, b, -(maxy - y) / a);
    if (y > maxy) shift_solution(x, y, a, b, sign_a);
    int rx2 = x;

    if (lx2 > rx2) swap(lx2, rx2);
    int lx = max(lx1, lx2);
    int rx = min(rx1, rx2);

    if (lx > rx) return 0;
    return (rx - lx) / abs(b) + 1;
}

// ---------------------------------------------------------
// Chinese Remainder Theorem (CRT)
// ---------------------------------------------------------

ll egcd_ll(ll a, ll b, ll& x, ll& y) {
    if (b == 0) { x = 1; y = 0; return a; }
    ll x1, y1;
    ll d = egcd_ll(b, a % b, x1, y1);
    x = y1; y = x1 - y1 * (a / b);
    return d;
}

/**
 * Solves system: 
 * x = a[0] (mod m[0])
 * x = a[1] (mod m[1])
 * ...
 * Handles NON-COPRIME moduli.
 * * @param A Vector of remainders
 * @param M Vector of moduli
 * @return pair<val, lcm> where x = val (mod lcm). Returns {-1, -1} if no solution.
 */
pair<ll, ll> chinese_remainder_theorem(const vector<ll>& A, const vector<ll>& M) {
    if (A.size() != M.size()) return {-1, -1};
    
    ll x = 0; // Current solution
    ll lcm = 1; // Current LCM of processed moduli

    for (size_t i = 0; i < A.size(); i++) {
        ll a = A[i];
        ll m = M[i];
        
        // Normalize: We want to solve x + k * lcm = a (mod m)
        // Rewrite as: k * lcm = a - x (mod m)
        // This is a linear congruence: A'k = B' (mod m)
        
        ll p, q;
        ll g = egcd_ll(lcm, m, p, q); // p*lcm + q*m = g
        
        // Check for existence: (a - x) must be divisible by gcd(lcm, m)
        if ((a - x) % g != 0) return {-1, -1}; 
        
        // Solve for k:
        // p * lcm = g (mod m)  ==> multiply by (a - x)/g
        // k = p * (a - x) / g  (mod m/g)
        
        // Use __int128 to prevent overflow during multiplication if available, 
        // otherwise ensure inputs are within safe bounds.
        ll m_div_g = m / g;
        ll multiplier = (a - x) / g;
        
        // Safe modular multiplication for k = (p * multiplier) % m_div_g
        ll k = (p % m_div_g * (multiplier % m_div_g)) % m_div_g;
        if (k < 0) k += m_div_g;
        
        // Update solution x
        x += k * lcm;
        lcm *= m_div_g; // Update global LCM
        
        // Keep x within [0, lcm-1]
        if (x < 0) x += lcm;
    }
    return {x, lcm};
}