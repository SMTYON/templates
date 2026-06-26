
struct DSU {
    vector<int> par, sz;
    struct State {
        int u, v; // u becomes child of v
        bool united;
    };
    vector<State> history;

    DSU(int n) {
        par.resize(n);
        iota(all(par), 0);
        sz.assign(n, 1);
    }

    int find(int i) {
        while (par[i] != i) i = par[i];
        return i;
    }

    bool unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            if (sz[root_i] < sz[root_j]) swap(root_i, root_j);
            // Attach root_j to root_i
            history.push_back({root_j, root_i, true});
            par[root_j] = root_i;
            sz[root_i] += sz[root_j];
            return true;
        }
        history.push_back({-1, -1, false}); // No merge happened
        return false;
    }

    void rollback() {
        if (history.empty()) return;
        State last = history.back();
        history.pop_back();
        if (last.united) {
            // last.u was attached to last.v
            par[last.u] = last.u;
            sz[last.v] -= sz[last.u];
        }
    }

    int snapshot() {
        return history.size();
    }

    void rollbackTo(int snapshot) {
        while (history.size() > snapshot) {
            rollback();
        }
    }

    void reset(int n) {
        iota(par.begin(), par.end(), 0);
        sz.assign(n, 1);
        history.clear();
        comps = n;
    }
};
