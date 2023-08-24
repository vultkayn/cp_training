#!/usr/bin/env python3

'''
@author benpr438
@brief Solve a Single-source shortest-path problem, with non-negative weights.
Given a graph as an input, that is a set of vertices V and of edges connecting them E,
all of these edges of positive weight, as well as a vertex source s,
Then find the shortest path from s to all other vertices in the graph.

A shortest path (s,t) is an ordered sequence of edges (s,v1), (v2,v3), ... + (vj, t) whose cumulative sum of the weights is minimal.
'''


from heapq import *

'''
Regular dikstra implementation, to solve a SSSP problem without negative weight
# V = number of vertices, E = number of edges
Time Complexity: O(E log V)
'''
def dijkstra(adj_list, nb_nodes, start):
    costs = [float('inf')] * nb_nodes
    costs[start] = 0
    prioQ = [[costs[start], start]]
    visited = [False] * nb_nodes
    while prioQ:
        cost, node = heappop(prioQ) # O(1) popping head element of PQ
        visited[node] = True
        # we explore all outgoing edges of the current node = O(E) complexity in total for the two loops.
        for weight, neighbor in adj_list[node]: 
            if not visited[neighbor] and (new_cost := weight + cost) < costs[neighbor]:
                heappush(prioQ, [new_cost, neighbor]) # insertion in priority queue = O(log E) complexity
                costs[neighbor] = new_cost
        # since E = V^2 at most in an oriented graph without multiple edges, log(E) <= 2log(V)
        # hence we get the known complexity of O(E log V)

    return costs
    


n, m, q, s = map(int, input().split())
first = True
while [n,m,q,s] != [0,0,0,0]:
    if not first:
        print()
    first = False
    adjacency_list = [[] for _ in range(n)]
    for _ in range(m):
        u, v, w = map(int, input().split())
        adjacency_list[u].append([w, v])

    costs = dijkstra(adjacency_list, n, s)
    for _ in range(q):
        target = int(input())
        print(costs[target] if costs[target] < float('inf') else "Impossible")
    n, m, q, s = map(int, input().split())