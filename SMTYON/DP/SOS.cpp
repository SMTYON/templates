// sum over subset.

/*
O(3^N):
for (int mask = 0; mask < (1 << N); mask++) {
    for (int submask = mask; submask > 0; submask = (submask - 1) & mask) {
        F[mask] += A[submask];
    }
    F[mask] += A[0];
}
*/

// SOS DP:

// 1. Without Memory Optimization (2D Array)

/* This implementation directly translates the mathematical recurrence relation into code. 
We use a 2D array dp[mask][i], which represents the sum of subsets for a given mask where 
we are only allowed to toggle the first i bits (from the right).

Time Complexity: O(N * 2^N)
Space Complexity: O(N * 2^N)
*/
#include <iostream>
#include <vector>
using namespace std;

int main() {
    int N = 20; // Example: 20 bits
    int MAX_MASK = 1 << N;
    
    vector<int> A(MAX_MASK, 1); // Example array initialized with 1s
    
    // dp[mask][i]
    // mask goes up to 2^N, i goes up to N
    vector<vector<int>> dp(MAX_MASK, vector<int>(N + 1, 0));

    // Base Case: i = 0 (No bits are allowed to differ)
    // The only valid subset is the mask itself.
    for (int mask = 0; mask < MAX_MASK; mask++) {
        dp[mask][0] = A[mask];
    }

    // DP Transitions
    for (int i = 1; i <= N; i++) {
        for (int mask = 0; mask < MAX_MASK; mask++) {
            // Check if the (i-1)-th bit is ON
            if (mask & (1 << (i - 1))) {
                // If ON: Valid subsets can have this bit as 1 or 0.
                // Add the state where the bit stays 1, and the state where it toggles to 0.
                dp[mask][i] = dp[mask][i - 1] + dp[mask ^ (1 << (i - 1))][i - 1];
            } else {
                // If OFF: Valid subsets must also have this bit as 0.
                dp[mask][i] = dp[mask][i - 1];
            }
        }
    }

    // The final answer for any mask is stored in dp[mask][N]
    // Example output for mask 3 (binary 00...011, which has 4 subsets: 0, 1, 2, 3)
    cout << "Sum for mask 3: " << dp[3][N] << "\n"; 

    return 0;
}


// 2. With Memory Optimization (1D Array)In the 2D version
/* 
calculating dp[mask][i] only requires values from the previous step, dp[...][i-1]. 
Because of how bitwise operations work, when we update the state for an active bit 1,
we are reading from the state where that bit is 0 (mask ^ (1 << i)). 
Since we only ever read from states with a 0 at the current bit position 
to update states with a 1 at the current bit position,
we can safely overwrite the array in place without corrupting the data needed for the current step.

Time Complexity: O(N * 2^N)
Space Complexity: O(2^N)
*/
#include <iostream>
#include <vector>
using namespace std;

int main() {
    int N = 20; // Example: 20 bits
    int MAX_MASK = 1 << N;
    
    vector<int> A(MAX_MASK, 1); // Example array initialized with 1s
    
    // We only need a 1D array of size 2^N
    vector<int> F(MAX_MASK);

    // Step 1: Base Case initialization
    for (int mask = 0; mask < MAX_MASK; ++mask) {
        F[mask] = A[mask];
    }

    // Step 2: In-place SOS DP
    for (int i = 0; i < N; ++i) {
        for (int mask = 0; mask < MAX_MASK; ++mask) {
            // If the i-th bit of the mask is ON
            if (mask & (1 << i)) {
                // Add the value of the subset where the i-th bit is OFF
                F[mask] += F[mask ^ (1 << i)];
            }
        }
    }

    // The final answer for any mask is directly stored in F[mask]
    // Example output for mask 3 (binary 00...011, which has 4 subsets: 0, 1, 2, 3)
    cout << "Sum for mask 3: " << F[3] << "\n";

    return 0;
}