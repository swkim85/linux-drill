#!/usr/bin/python3
# graph_gen.py
import networkx as nx
import matplotlib.pyplot as plt

# edge list로부터 그래프 생성
edgelist = [(0, 3), (1, 4), (1, 5), (1, 6), (1, 7), (2, 3), (2, 7), (2, 8), (3, 8), (3, 9), (4, 5), (4, 7), (4, 8), (4, 9), (5, 8), (6, 9), (8, 9)]
G = nx.from_edgelist(edgelist)
n_nodes = G.number_of_nodes()

#n_nodes = 10
#probility = 0.2
#G = nx.gnp_random_graph(n=n_nodes, p=probility)

nx.draw(G, with_labels=True,
        node_color='lightblue',
        edge_color='gray')

edges = G.edges()
edges = list(edges)  # 파이썬 리스트로 변환
print(edges)
n_edges = len(edges)

print (n_nodes, n_edges) # 정점의 개수, 간선의 개수
for edge in edges:
  print (edge[0], edge[1])
