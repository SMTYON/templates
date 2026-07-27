void floydWarshall(vector<vector<int>> &dist) {
  int V = dist.size();

  // Add all vertices one by one to
  // the set of intermediate vertices.
  for (int k = 0; k < V; k++) {

    // Pick all vertices as source one by one
    for (int i = 0; i < V; i++) {

      // Pick all vertices as destination
      // for the above picked source
      for (int j = 0; j < V; j++) {

        // shortest path from
        // i to j
        if (dist[i][k] != 1e8 && dist[k][j] != 1e8)
          dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
      }
    }
  }
}

int main() {
  int INF = 100000000;
  vector<vector<int>> dist = {{0, 4, INF, 5, INF},
                              {INF, 0, 1, INF, 6},
                              {2, INF, 0, 3, INF},
                              {INF, INF, 1, 0, 2},
                              {1, INF, INF, 4, 0}};

  floydWarshall(dist);
  for (int i = 0; i < dist.size(); i++) {
    for (int j = 0; j < dist.size(); j++) {
      cout << dist[i][j] << " ";
    }
    cout << endl;
  }
  return 0;
}