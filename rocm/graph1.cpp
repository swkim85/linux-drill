// graph1.cpp
#include <iostream>
#include <vector>
#include <algorithm>

// Function to perform DFS and find the longest path
void dfs(int node, std::vector<std::vector<int>>& adj, std::vector<bool>& visited, int currentLength, int& maxLength) {
  visited[node] = true;
  maxLength = std::max(maxLength, currentLength);

/*
  std::cout
   << " node : " << node
   << " currentLength : " << currentLength
   << " maxLength : " << maxLength
   << std::endl;
*/

  for (int neighbor : adj[node]) {
    if (!visited[neighbor]) {
      dfs(neighbor, adj, visited, currentLength + 1, maxLength);
    }
  }
  visited[node] = false;
}

void print_adj(std::vector<std::vector<int>>& adj) {
    for (size_t i = 0; i < adj.size(); ++i) {
        std::cout << "Node " << i << ": ";
        for (size_t j = 0; j < adj[i].size(); ++j) {
            std::cout << adj[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
  int n, m; // n: number of vertices, m: number of edges
  std::cin >> n >> m;
  std::cout << "number of nodes : " << n << std::endl;
  std::cout << "number of edges : " << m << std::endl;

  std::vector<std::vector<int>> adj(n);
  for (int i = 0; i < m; ++i) {
    int u, v;
    std::cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u); // If the graph is undirected
  }
  print_adj(adj);

  int maxLength = 0;
  std::vector<bool> visited(n, false);

  for (int i = 0; i < n; ++i) {
    std::cout << " node : " << i << std::endl;
    dfs(i, adj, visited, 0, maxLength);
  }

  std::cout << "The longest path length is: " << maxLength << std::endl;
  return 0;
}
