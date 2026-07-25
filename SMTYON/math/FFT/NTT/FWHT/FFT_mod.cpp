#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
using namespace std;

using ll = long long;
using C = complex<double>;
void fft(vector<C>& a) {
    int n = (int)a.size(), L = 31 - __builtin_clz(n);
    static vector<complex<long double>> R(2, 1);
    static vector<C> rt(2, 1);  
    
    for (static int k = 2; k < n; k *= 2) {
        R.resize(n); 
        rt.resize(n);
        auto x = polar(1.0L, acos(-1.0L) / k);
        for (int i = k; i < 2 * k; i++) {
            rt[i] = R[i] = i & 1 ? R[i / 2] * x : R[i / 2];
        }
    }
    
    vector<int> rev(n);
    for (int i = 0; i < n; i++) {
        rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
    }
    for (int i = 0; i < n; i++) {
        if (i < rev[i]) swap(a[i], a[rev[i]]);
    }
    
    for (int k = 1; k < n; k *= 2) {
        for (int i = 0; i < n; i += 2 * k) {
            for (int j = 0; j < k; j++) {
                // Hand-rolled complex multiplication for speed
                auto x = (double *)&rt[j + k];
                auto y = (double *)&a[i + j + k];        
                C z(x[0] * y[0] - x[1] * y[1], x[0] * y[1] + x[1] * y[0]);           
                
                a[i + j + k] = a[i + j] - z;
                a[i + j] += z;
            }
        }
    }
}
 
/* 
 * MAIN FUNCTION: convMod<MODULO>(a, b)
 * Computes the convolution of two arrays modulo M using the Split FFT trick.
 * Resistant to floating point precision errors up to M <= 10^9.
 */
template<int M> 
vector<int> convMod(const vector<int> &a, const vector<int> &b) {
    if (a.empty() || b.empty()) return {};
    
    vector<int> res((int)a.size() + (int)b.size() - 1);
    int B = 32 - __builtin_clz((int)res.size());
    int n = 1 << B;
    int cut = int(sqrt(M));
    vector<C> L(n), R(n), outs(n), outl(n);
    
    for (int i = 0; i < (int)a.size(); i++) {
        L[i] = C((int)a[i] / cut, (int)a[i] % cut);
    }
    for (int i = 0; i < (int)b.size(); i++) {
        R[i] = C((int)b[i] / cut, (int)b[i] % cut);
    }
    
    fft(L); 
    fft(R);
    
    for (int i = 0; i < n; i++) {
        int j = -i & (n - 1);
        outl[j] = (L[i] + conj(L[j])) * R[i] / (2.0 * n);
        outs[j] = (L[i] - conj(L[j])) * R[i] / (2.0 * n) / C(0, 1);
    }
    
    fft(outl); 
    fft(outs);
    
    for (int i = 0; i < (int)res.size(); i++) {
        ll av = ll(real(outl[i]) + 0.5);
        ll cv = ll(imag(outs[i]) + 0.5);
        ll bv = ll(imag(outl[i]) + 0.5) + ll(real(outs[i]) + 0.5);
        res[i] = ((av % M * cut + bv) % M * cut + cv) % M;
    }
    
    return res;
}

int main() {
    vector<int> a = {1, 2, 3};
    vector<int> b = {4, 5};
    
    // Calculates the convolution modulo 10^9+7
    vector<int> result = convMod<1000000007>(a, b);
    
    // result[k] = sum(a[i] * b[j]) % 1000000007 for i+j == k
    
    return 0;
}