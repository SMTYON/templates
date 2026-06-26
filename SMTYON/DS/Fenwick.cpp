struct FenwickTree {// 1-based 
    int n;
    vector<int> tree;

    FenwickTree(int size) {
        n = size;
        tree.assign(n + 1, 0); 
    }

    // constructor from an existing array O(N)
    FenwickTree(const vector<int>& a) {
        FenwickTree(a.size());
        
        for (int i = 0; i < n; i++) {
            update(i + 1, a[i]); 
        }
    }

    void update(int i, int val) {
        for (; i <= n; i += i & -i) {
            tree[i] += val;
        }
    }

    // Prefix sum: Sum of elements in [1, i]
    int query(int i) {
        int sum = 0;
        for (; i > 0; i -= i & -i) {
            sum += tree[i];
        }
        return sum;
    }

    // Binary Lifting: Find the smallest 1-based index where prefix sum >= target
    // Assumes all frequencies are non-negative!
    int lower_bound(int target) {
        int pos = 0;
        
        // Find the greatest power of 2 <= n
        int bitMask = 1;
        while (bitMask <= n) bitMask <<= 1;
        bitMask >>= 1;

        for (; bitMask > 0; bitMask >>= 1) {
            if (pos + bitMask <= n && tree[pos + bitMask] < target) {
                pos += bitMask;
                target -= tree[pos];
            }
        }
        return pos + 1; // Return the 1-based index
    }
};