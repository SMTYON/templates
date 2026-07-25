#include <iostream>
#include <vector>
#include <map>

using namespace std;

const int MAXN = 100005;
vector<int> adj[MAXN];
int color[MAXN];
int ans[MAXN];

// Array of maps to hold the data for each node
map<int, int> mp[MAXN];

void dfs_small_to_large(int u, int p) {
    // 1. Add the current node's color to its own map
    mp[u][color[u]]++;

    for (int v : adj[u]) {
        if (v == p) continue;

        // 2. Process the child
        dfs_small_to_large(v, u);

        // 3. SMALL-TO-LARGE TRICK:
        // If the child's map is larger, swap them.
        // std::swap works in O(1) time by just swapping internal pointers!
        if (mp[v].size() > mp[u].size()) {
            swap(mp[u], mp[v]);
        }

        // 4. Merge the smaller map (v) into the larger map (u)
        for (auto const& [col, count] : mp[v]) {
            mp[u][col] += count;
        }
        
        // 5. Free up memory to avoid Memory Limit Exceeded
        // Since we swapped earlier, mp[v] might contain the larger data, 
        // so we must clear it now that its elements are safely in mp[u].
        mp[v].clear();
    }

    // 6. The number of distinct colors in node u's subtree is the size of the map
    ans[u] = mp[u].size();
}