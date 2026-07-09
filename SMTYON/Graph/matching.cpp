#include <iostream>
#include <vector>

using namespace std;

/**
 * A reusable template for Bipartite Matching with capacities on the right-side nodes.
 * Uses a modified Kuhn's Algorithm.
 * O(total_number_of_nodes * total_number_of_edges).
 */
struct MultiBipartiteMatcher {
    int n; // Number of left-side nodes
    int m; // Number of right-side nodes
    vector<vector<int>> adj;      // adj[u] = list of right nodes that left node u can connect to
    vector<int> capacity;         // capacity[v] = max number of left nodes that right node v can accept
    vector<vector<int>> match;    // match[v] = list of left nodes currently assigned to right node v
    vector<bool> vis;             // Visited array to prevent cycles during augmenting paths

    // Constructor
    MultiBipartiteMatcher(int n_left, int n_right) {
        n = n_left;
        m = n_right;
        adj.resize(n);
        capacity.assign(m, 1);    // Default capacity is 1 (standard bipartite matching)
        match.resize(m);
    }

    // Add a directed edge from left node u to right node v
    void add_edge(int u, int v) {
        adj[u].push_back(v);
    }

    // Set the maximum capacity for a specific right node v
    void set_capacity(int v, int cap) {
        capacity[v] = cap;
    }

    // Set a uniform capacity for all right nodes
    void set_all_capacities(int cap) {
        capacity.assign(m, cap);
    }

    // The DFS function to find an augmenting path
    bool dfs(int u) {
        for (int v : adj[u]) {
            if (vis[v]) continue; // Skip if this right node is already part of the current path
            vis[v] = true;

            // Base case: If there is still room in node v, simply assign u to it
            if (match[v].size() < capacity[v]) {
                match[v].push_back(u);
                return true;
            }

            // Recursive case: Node v is full. Try to reassign one of its current occupants.
            for (int i = 0; i < match[v].size(); ++i) {
                int prev_u = match[v][i];
                
                // If we can find an alternate valid assignment for the previous node
                if (dfs(prev_u)) {
                    match[v][i] = u; // Take their spot
                    return true;
                }
            }
        }
        return false;
    }

    // Main function to compute the maximum matching
    int max_matching() {
        int total_matches = 0;
        for (int i = 0; i < n; ++i) {
            vis.assign(m, false); // Reset visited array for each left node
            if (dfs(i)) {
                total_matches++;
            }
        }
        return total_matches;
    }
};

// ==========================================
// Example Usage
// ==========================================
int main() {
    // 5 Left nodes (0 to 4), 3 Right nodes (0 to 2)
    MultiBipartiteMatcher matcher(5, 3);
    
    // Set capacities: Suppose right node 0 can take 2, node 1 can take 1, node 2 can take 2
    matcher.set_capacity(0, 2);
    matcher.set_capacity(1, 1);
    matcher.set_capacity(2, 2);

    // Add valid edges (Left -> Right)
    matcher.add_edge(0, 0);
    matcher.add_edge(0, 1);
    matcher.add_edge(1, 2);
    matcher.add_edge(2, 1);
    matcher.add_edge(3, 0);
    matcher.add_edge(4, 2);

    // Compute the result
    int max_assignments = matcher.max_matching();

    cout << "Maximum elements matched: " << max_assignments << "\n";

    // Optional: Print the actual assignments
    for (int v = 0; v < matcher.m; ++v) {
        cout << "Right node " << v << " is matched with Left nodes: ";
        for (int u : matcher.match[v]) {
            cout << u << " ";
        }
        cout << "\n";
    }

    return 0;
}