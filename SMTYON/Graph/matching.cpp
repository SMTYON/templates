#include <bits/stdc++.h>
using namespace std;

struct BipartiteMatcher {
    // Time Complexity: O(V * E) where V = left nodes, E = edges
    // König's Theorem: Minimum Vertex Cover (MVC) = Maximum Bipartite Matching (MBM)
    // Complement: Maximum Independent Set (MIS) = (Total Nodes) - MBM
    
    int n, m;
    vector<vector<int>> adj;
    vector<int> cap, matchL;
    vector<vector<int>> matchR; // matchR[v] = left nodes matched to right node v
    vector<bool> vis;

    // Default capacity is 1 (standard 1-to-1 bipartite matching)
    BipartiteMatcher(int n, int m) : n(n), m(m), adj(n), cap(m, 1), matchL(n, -1), matchR(m) {}

    void addEdge(int u, int v) { adj[u].push_back(v); }
    
    void setCapacity(int v, int c) { cap[v] = c; }

    bool dfs(int u) {
        for (int v : adj[u]) {
            if (vis[v]) continue;
            vis[v] = true;

            // Base case: Right node has free capacity
            if (matchR[v].size() < cap[v]) {
                matchR[v].push_back(u);
                matchL[u] = v;
                return true;
            }

            // Recursive case: Try to reassign an existing match
            for (int i = 0; i < matchR[v].size(); ++i) {
                int prev_u = matchR[v][i];
                if (dfs(prev_u)) {
                    matchR[v][i] = u;
                    matchL[u] = v; // DFS updates prev_u's matchL automatically
                    return true;
                }
            }
        }
        return false;
    }

    int maxMatching() {
        int match = 0;
        for (int u = 0; u < n; u++) {
            vis.assign(m, false);
            if (dfs(u)) match++;
        }
        return match;
    }

    // Returns {coverL, coverR}
    pair<vector<int>, vector<int>> minVertexCover() {
        vector<bool> visL(n, false), visR(m, false);
        queue<int> q;
        
        // Start BFS from all unmatched left nodes
        for (int u = 0; u < n; u++) {
            if (matchL[u] == -1) {
                visL[u] = true;
                q.push(u);
            }
        }

        while (q.size()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (!visR[v]) {
                    visR[v] = true;
                    for (int matched_u : matchR[v]) {
                        if (!visL[matched_u]) {
                            visL[matched_u] = true;
                            q.push(matched_u);
                        }
                    }
                }
            }
        }

        vector<int> coverL, coverR;
        for (int u = 0; u < n; u++) if (!visL[u]) coverL.push_back(u);
        for (int v = 0; v < m; v++) if (visR[v]) coverR.push_back(v);
        return {coverL, coverR};
    }

    // Returns {indL, indR}
    pair<vector<int>, vector<int>> maxIndependentSet() {
        auto [coverL, coverR] = minVertexCover();
        
        vector<bool> inCovL(n, false), inCovR(m, false);
        for (int u : coverL) inCovL[u] = true;
        for (int v : coverR) inCovR[v] = true;

        vector<int> indL, indR;
        for (int u = 0; u < n; u++) if (!inCovL[u]) indL.push_back(u);
        for (int v = 0; v < m; v++) if (!inCovR[v]) indR.push_back(v);
        return {indL, indR};
    }

    bool isPerfectMatching() {
        for (int u = 0; u < n; u++) {
            if (matchL[u] == -1) return false;
        }
        return true;
    }
};