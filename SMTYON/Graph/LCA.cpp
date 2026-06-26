struct LCA
{
    const int LG = 20;
    int timer = 0;
    vector<int> in, out;
    vector<vector<int>> par;
    LCA(int n, int root, vector<vector<int>> &adj)
    {
        in = out = vector<int>(n + 2);
        par = vector<vector<int>>(n + 2, vector<int>(LG));
        dfs(root, root, adj);
    }
    void dfs(int node, int p, vector<vector<int>> &adj)
    {
        
        in[node] = timer++;
        par[node][0] = p;
        for (int j = 1; j < LG; j++)
        {
            par[node][j] = par[par[node][j - 1]][j - 1];
        }
        for (auto &child : adj[node])
        {
            if (child == p)
                continue;
            dfs(child, node, adj);
        }
        out[node] = timer++;
    }

    int kth(int u , int k){
        for (int i = LG - 1; ~i; i--)
        {
            if((k>>i)&1){
                u = par[u][i];
            }
        }
        return u;
    }

    bool isPar(int node, int p)
    {
        return in[p] <= in[node] && out[p] >= out[node];
    }

    int lca(int u, int v)
    {
        if (isPar(v, u))
            return u;
        for (int i = LG - 1; ~i; i--)
        {
            if (!isPar(v, par[u][i]))
            {
                u = par[u][i];
            }
        }
        return par[u][0];
    }

};