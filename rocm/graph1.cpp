// graph1.cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Function to perform DFS and find the longest path
void dfs(int node, vector<vector<int>>& adj, vector<bool>& visited, int currentLength, int& maxLength) {
  visited[node] = true;
  maxLength = max(maxLength, currentLength);

  for (int neighbor : adj[node]) {
    if (!visited[neighbor]) {
      dfs(neighbor, adj, visited, currentLength + 1, maxLength);
    }
  }
  visited[node] = false;
}

int main() {
  int n, m; // n: number of vertices, m: number of edges
  cin >> n >> m;
  cout << "number of nodes : " << n << endl;
  cout << "number of edges : " << m << endl;

  vector<vector<int>> adj(n);
  for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u); // If the graph is undirected
  }

  int maxLength = 0;
  vector<bool> visited(n, false);

  for (int i = 0; i < n; ++i) {
    dfs(i, adj, visited, 0, maxLength);
  }

  cout << "The longest path length is: " << maxLength << endl;
  return 0;
}
