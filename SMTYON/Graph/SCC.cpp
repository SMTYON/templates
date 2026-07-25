#include <bits/stdc++.h>
using namespace std;
#define all(a) a.begin(), a.end()
#define ll long long
const int MAXN = 1e5 + 5;

// DO NOT BUT ANOTHER VARIABLE n IN THE MAIN !!!!!
// --- SCC Variables ---
int n, turn, scc_cnt;           // scc_cnt tracks the total number of components
vector<vector<int>> adj;        // Original graph
vector<vector<int>> comp;       // comp[i] stores all nodes belonging to SCC with ID i
vector<vector<int>> condensed_adj; // Adjacency list of the new DAG (condensed graph)
vector<int> scc_id;             // scc_id[u] gives the continuous SCC ID (0 to scc_cnt-1) for node u
vector<int> id, low;            // Tarjan's arrays
vector<bool> on_st;             // Tracks if a node is currently in the current DFS stack
stack<int> st;                  // Tarjan's stack

// 1. Initialize all variables for a new test case
void init() {
    turn = 0;
    scc_cnt = 0; // Reset component count
    
    comp.clear();          
    condensed_adj.clear(); 
    scc_id.assign(n, -1);
    id.assign(n, -1);
    low.assign(n, -1);
    on_st.assign(n, false);
    
    while (!st.empty()) st.pop(); // Clear the stack safely
}

// 2. Tarjan's DFS
void dfs(int u) {
    id[u] = low[u] = turn++;
    st.push(u);
    on_st[u] = true;
    
    for (int v : adj[u]) {
        if (id[v] == -1) dfs(v);
        if (on_st[v]) low[u] = min(low[u], low[v]); 
    }

    // If 'u' is the root of an SCC
    if (low[u] == id[u]) {
        comp.push_back(vector<int>()); // Create a new empty component
        
        while (true) {
            int cur = st.top();
            st.pop();
            on_st[cur] = false;
            
            scc_id[cur] = scc_cnt;          // Assign the continuous ID
            comp.back().push_back(cur);     // Add node to the current component
            
            if (cur == u) break;            // Stop once we pop the root
        }
        scc_cnt++; // Increment component count for the next SCC
    }
}

// 3. Build the SCCs and the Condensed Graph (DAG)
void build_scc() {
    init();
    // Step A: Find SCCs
    for (int i = 0; i < n; i++) {
        if (id[i] == -1) dfs(i);
    }
    
    // Step B: Build Condensed Graph
    condensed_adj.assign(scc_cnt, vector<int>());
    
    for (int u = 0; u < n; u++) {
        for (int v : adj[u]) {
            // If u and v belong to DIFFERENT components, add a directed edge
            if (scc_id[u] != scc_id[v]) {
                condensed_adj[scc_id[u]].push_back(scc_id[v]);
            }
        }
    }
    
    // Step C: Remove duplicate edges in the condensed graph
    for (int i = 0; i < scc_cnt; i++) {
        sort(all(condensed_adj[i]));
        condensed_adj[i].erase(unique(all(condensed_adj[i])), condensed_adj[i].end());
    }
}