// 
// Deterministic bases guarantee correctness for all numbers up to 2^64
#define int long long
int power(int x, int y, int p)
{
    int res = 1;     
    x = x % p;  
    while (y > 0){
        if (y & 1)
            // Cast to __int128_t to prevent overflow when multiplying 10^10 * 10^10
            res = (int)(((unsigned __int128)res * x) % p);
        y = y >> 1; 
        x = (int)(((unsigned __int128)x * x) % p);
    }
    return res;
}

bool isPrime(int n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;

    int d = n - 1;
    int s = 0;
    while (d % 2 == 0) {
        d /= 2;
        s++;
    }

    // Deterministic bases guarantee correctness for all numbers up to 2^64
    vector<int> bases = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (int a : bases) {
        if (n <= a) break;
        int x = power(a, d, n);
        if (x == 1 || x == n - 1) continue;
        
        bool composite = true;
        for (int r = 1; r < s; r++) {
            x = (int)(((unsigned __int128)x * x) % n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }

    return true;
}