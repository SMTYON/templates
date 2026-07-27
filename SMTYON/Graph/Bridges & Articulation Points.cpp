#include <bits/stdc++.h>
using namespace std;

// ==============================================================================
// Bridges and Articulation Points (Cut-Vertices)
//
// O(V + E)
// Graph Type: Undirected Graphs only.
//
// Usage:
// 1. Set 'n' to the number of nodes.
// 2. Call UndirectedGraph::init(n).
// 3. Add UNDIRECTED edges using UndirectedGraph::add_edge(u, v). (0-indexed)
// 4. Call UndirectedGraph::find_bridges_and_points().
// 
// Output:
// - UndirectedGraph::bridges: A vector of pairs {u, v} representing cut-edges.
// - UndirectedGraph::is_art_point[u]: A boolean array. True if node 'u' is a cut-vertex.
// 
// Note: This template handles disconnected graphs automatically.
// ==============================================================================

namespace UndirectedGraph {
    int n, timer;
    vector<vector<int>> adj;
    vector<int> tin, low;
    
    // Outputs
    vector<pair<int, int>> bridges;
    vector<bool> is_art_point;

    void init(int nodes) {
        n = nodes; timer = 0;
        adj.assign(n, vector<int>());
        tin.assign(n, -1);
        low.assign(n, -1);
        bridges.clear();
        is_art_point.assign(n, false);
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs(int u, int p = -1) {
        tin[u] = low[u] = timer++;
        int children = 0;

        for (int v : adj[u]) {
            if (v == p) continue; // Don't go immediately back to parent
            
            if (tin[v] != -1) {
                // Back-edge found
                low[u] = min(low[u], tin[v]);
            } else {
                // Forward-edge (tree edge)
                dfs(v, u);
                low[u] = min(low[u], low[v]);
                
                // --- BRIDGE CONDITION ---
                // If the lowest reachable node from v is strictly below u, 
                // the edge u-v is the only way out for v.
                if (low[v] > tin[u]) {
                    bridges.push_back({u, v});
                }
                
                // --- ARTICULATION POINT CONDITION ---
                // If we are not the root, and v cannot reach above u, u is a bottleneck.
                if (low[v] >= tin[u] && p != -1) {
                    is_art_point[u] = true;
                }
                children++;
            }
        }
        
        // --- ROOT ARTICULATION POINT CONDITION ---
        // The root of the DFS tree is an articulation point ONLY if it has >1 independent children.
        if (p == -1 && children > 1) {
            is_art_point[u] = true;
        }
    }

    void find_bridges_and_points() {
        for (int i = 0; i < n; i++) {
            if (tin[i] == -1) {
                dfs(i); // Process each connected component
            }
        }
    }
}