struct Node 
{
    ll sum;
    int left_child;
    int right_child;
} NEUTRAL = {0, 0, 0}; 

struct PersistentSegTree 
{
    int size;
    vector<Node> tree;
    vector<int> roots;
    int node_count;

    PersistentSegTree(int n, int q) 
    {
        size = 1;
        while (size < n) size *= 2;
        
        // Max nodes: ~ (2 * size) for base tree + (Q * log2(size)) for updates
        int max_nodes = (2 * size) + (q * 25); 
        tree.resize(max_nodes);
        roots.push_back(0); // 0-th index in roots can serve as a dummy or 0th version
        node_count = 1;
    }

    // Creates a new node in the array pool and returns its index
    int create_node(ll sum, int lc, int rc) 
    {
        int id = node_count++;
        tree[id].sum = sum;
        tree[id].left_child = lc;
        tree[id].right_child = rc;
        return id;
    }

    Node merge(Node a, Node b) 
    {
        Node res; 
        res.sum = a.sum + b.sum;
        // left and right children are handled during node creation, not merging
        return res;
    }

    // Returns the root of the built tree
    int build(vector<int>& a, int lx, int rx) 
    {
        if (rx - lx == 1){
            if (lx < a.size()) return create_node(a[lx], 0, 0);
            else return create_node(0, 0, 0);
        }
        
        int m = (lx + rx) / 2;
        int lc = build(a, lx, m);
        int rc = build(a, m, rx);
        
        Node merged = merge(tree[lc], tree[rc]);
        return create_node(merged.sum, lc, rc);
    }

    // Point update at index i. Returns the root of the NEW version.
    int update(int i, int val, int x, int lx, int rx) 
    {
        if (rx - lx == 1) {
            return create_node(val, 0, 0);
        }
        
        int m = (lx + rx) / 2;
        int lc = tree[x].left_child;
        int rc = tree[x].right_child;
        
        if (i < m) {
            lc = update(i, val, tree[x].left_child, lx, m);
        }else{
            rc = update(i, val, tree[x].right_child, m, rx);
        }
        
        Node merged = merge(tree[lc], tree[rc]);
        return create_node(merged.sum, lc, rc);
    }

    // Zero-based Range Query [l,r)
    Node query(int l, int r, int x, int lx, int rx) 
    {
        if (lx >= r || rx <= l || x == 0) return NEUTRAL;
        if (lx >= l && rx <= r) return tree[x];
        
        int m = (lx + rx) / 2;
        Node left_res = query(l, r, tree[x].left_child, lx, m);
        Node right_res = query(l, r, tree[x].right_child, m, rx);
        
        return merge(left_res, right_res);
    }

    void build(vector<int>& a){roots.push_back(build(a, 0, size));}
    void update(int i, int val, int prev_version){roots.push_back(update(i, val, roots[prev_version], 0, size));}
    ll query(int l, int r, int version){return query(l, r, roots[version], 0, size).sum;}
};