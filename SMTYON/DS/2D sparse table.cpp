const int MAXN = 505;  // Max Rows
const int MAXM = 505;  // Max Columns
const int LOGN = 10;   // log2(MAXN) + 1
const int LOGM = 10;   // log2(MAXM) + 1

class SparseTable2D {
private:
    // table[jr][ir][jc][ic]
    int st[LOGN][MAXN][LOGM][MAXM];
    int log2_val[max(MAXN, MAXM) + 1];
    int n, m;

    void precomputeLogs() {
        log2_val[1] = 0;
        int max_dim = max(n, m);
        for (int i = 2; i <= max_dim; i++) {
            log2_val[i] = log2_val[i / 2] + 1;
        }
    }

public:
    void build(const vector<vector<int>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) return;
        n = matrix.size();
        m = matrix[0].size();
        
        precomputeLogs();

        // 1. Build the 1D Sparse Table for each row (jr = 0)
        for (int ir = 0; ir < n; ir++) {
            // Base case
            for (int ic = 0; ic < m; ic++) {
                st[0][ir][0][ic] = matrix[ir][ic];
            }
            // Fill 1D RMQ for the current row
            for (int jc = 1; jc <= log2_val[m]; jc++) {
                for (int ic = 0; ic + (1 << (jc - 1)) < m; ic++) {
                    st[0][ir][jc][ic] = min(st[0][ir][jc - 1][ic], 
                                            st[0][ir][jc - 1][ic + (1 << (jc - 1))]);
                }
            }
        }

        // 2. Build the 2D Sparse Table using the 1D tables as a base
        for (int jr = 1; jr <= log2_val[n]; jr++) {
            for (int ir = 0; ir + (1 << (jr - 1)) < n; ir++) {
                for (int jc = 0; jc <= log2_val[m]; jc++) {
                    for (int ic = 0; ic < m; ic++) {
                        // We take the minimum of the top half and bottom half
                        st[jr][ir][jc][ic] = min(st[jr - 1][ir][jc][ic], 
                                                 st[jr - 1][ir + (1 << (jr - 1))][jc][ic]);
                    }
                }
            }
        }
    }

    // Query in O(1)
    int query(int x1, int y1, int x2, int y2) {
        int lenx = x2 - x1 + 1;
        int kx = log2_val[lenx];
        
        int leny = y2 - y1 + 1;
        int ky = log2_val[leny];

        int min_R1 = min(st[kx][x1][ky][y1], 
                         st[kx][x1][ky][y2 + 1 - (1 << ky)]);
                         
        int min_R2 = min(st[kx][x2 + 1 - (1 << kx)][ky][y1], 
                         st[kx][x2 + 1 - (1 << kx)][ky][y2 + 1 - (1 << ky)]);

        return min(min_R1, min_R2);
    }
};