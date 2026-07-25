/*
The Problem: You are given a tree with N nodes. 
Every node has a specific color. For every single node in the tree, 
you need to output the number of distinct colors present in its subtree.
*/
#include <iostream>
#include <vector>
using namespace std;

const int MAXN = 1e5+5;
vector<int> adj[MAXN];
int color[MAXN], ans[MAXN];

// Tree properties
int sz[MAXN];      // Size of the subtree
int heavy[MAXN];   // The heavy child of a node (-1 if leaf)

// Global state tracking
int cnt[MAXN];     // Global frequency array
int distinct_colors = 0; // Running answer for the global state

// ---------------------------------------------------------
// PHASE 1: Precalculate subtree sizes and heavy children
// ---------------------------------------------------------
void dfs_sz(int u, int p) {
    sz[u] = 1;
    heavy[u] = -1;
    int max_sub = 0;

    for (int v : adj[u]) {
        if (v != p) {
            dfs_sz(v, u);
            sz[u] += sz[v];
            
            // If this child is the biggest so far, mark it as heavy
            if (sz[v] > max_sub) {
                max_sub = sz[v];
                heavy[u] = v;
            }
        }
    }
}

// Helper function to add/remove a subtree from the global state
// 'val' is 1 to add, -1 to remove. 
// 'skip' allows us to completely ignore the heavy child's subtree when combining.
void update_subtree(int u, int p, int val, int skip) {
    // Process current node
    if (val == 1 && cnt[color[u]] == 0) distinct_colors++;
    if (val == -1 && cnt[color[u]] == 1) distinct_colors--;
    cnt[color[u]] += val;

    // Process all children EXCEPT the 'skip' node
    for (int v : adj[u]) {
        if (v != p && v != skip) {
            update_subtree(v, u, val, skip);
        }
    }
}

// ---------------------------------------------------------
// PHASE 2: The actual Sack (DSU on Tree) logic
// ---------------------------------------------------------
void dfs_sack(int u, int p, bool keep) {
    // Step 1: Process all LIGHT children first. 
    // Pass 'keep = false' so their data is wiped from the global state.
    for (int v : adj[u]) {
        if (v != p && v != heavy[u]) {
            dfs_sack(v, u, false);
        }
    }

    // Step 2: Process the HEAVY child.
    // Pass 'keep = true' so its massive subtree data stays in the global array.
    if (heavy[u] != -1) {
        dfs_sack(heavy[u], u, true);
    }

    // Step 3: Combine.
    // Add the current node 'u' and all its light subtrees into the global state.
    // We pass heavy[u] as the 'skip' parameter because it's ALREADY in the array.
    update_subtree(u, p, 1, heavy[u]);

    // Step 4: The global state now perfectly represents the subtree of 'u'. Record answer.
    ans[u] = distinct_colors;

    // Step 5: Clean up.
    // If 'u' is a light child itself (keep == false), wipe it from the global state 
    // before we return to 'u's parent.
    if (!keep) {
        update_subtree(u, p, -1, -1); // pass -1 to skip nothing
    }
}



