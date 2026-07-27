#include <bits/stdc++.h>
using namespace std;

// ==============================================================================
// Tarjan's SCC
//
// O(V + E)
// Graph Type: Directed Graphs only.
//
// Usage:
// 1. Set 'n' to the number of nodes.
// 2. Call SCC::init(n).
// 3. Add directed edges to SCC::adj using SCC::adj[u].push_back(v). (0-indexed)
// 4. Call SCC::build().
// 
// Output:
// - SCC::scc_cnt: Total number of components.
// - SCC::scc_id[u]: The ID of the component node 'u' belongs to (0 to scc_cnt-1).
// - SCC::comp: comp[i] contains all original nodes in component ID 'i'.
// - SCC::condensed_adj: The new DAG where components are contracted into super-nodes.
// ==============================================================================

namespace SCC {
    int n, timer, scc_cnt;
    vector<vector<int>> adj, comp, condensed_adj;
    vector<int> scc_id, tin, low;
    vector<bool> on_stack;
    stack<int> st;

    void init(int nodes) {
        n = nodes; timer = 0; scc_cnt = 0;
        adj.assign(n, vector<int>());
        comp.clear(); condensed_adj.clear();
        scc_id.assign(n, -1);
        tin.assign(n, -1); low.assign(n, -1);
        on_stack.assign(n, false);
        while (!st.empty()) st.pop();
    }

    void dfs(int u) {
        tin[u] = low[u] = timer++;
        st.push(u);
        on_stack[u] = true;

        for (int v : adj[u]) {
            if (tin[v] == -1) {
                dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (on_stack[v]) {
                low[u] = min(low[u], tin[v]);
            }
        }

        if (low[u] == tin[u]) {
            comp.push_back({});
            while (true) {
                int v = st.top(); st.pop();
                on_stack[v] = false;
                scc_id[v] = scc_cnt;
                comp.back().push_back(v);
                if (u == v) break;
            }
            scc_cnt++;
        }
    }

    void build() {
        for (int i = 0; i < n; i++) {
            if (tin[i] == -1) dfs(i);
        }
        
        // Build the condensed DAG
        condensed_adj.assign(scc_cnt, vector<int>());
        for (int u = 0; u < n; u++) {
            for (int v : adj[u]) {
                if (scc_id[u] != scc_id[v]) {
                    condensed_adj[scc_id[u]].push_back(scc_id[v]);
                }
            }
        }
        
        // Remove duplicate edges in the condensed graph
        for (int i = 0; i < scc_cnt; i++) {
            sort(condensed_adj[i].begin(), condensed_adj[i].end());
            condensed_adj[i].erase(unique(condensed_adj[i].begin(), condensed_adj[i].end()), condensed_adj[i].end());
        }
    }
}