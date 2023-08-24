#! /usr/bin/env python3

"""
@author benpr438

 Find the shortest path from node 'start' to a target node,
 within a graph that can have negative weights on its edges,
 thus potentially a negative cycle i.e. a cycle whose the sum of its edges' weight
 is negative.
 Such cycle might lead to a never-ending shortest path algorithm if done naively,
 as simply going through this cycle again and again would decrease the overall cost of the path.
 
 When dealing with negative weights Dijkstra is hardly applicable
 as if there is any negative-weight cycle, the algorithm would fail.
 Instead we use Bellman Ford, that can report such cycles.
 We are dealing with multiple queries, but always from the same starting point,
 so a single source algorithm is applicable.
 We compute the shortest path from 'start' to all other nodes in one go,
 then every query will be answered in O(1) time.

 O(V) memory usage. O(VE) time complexity.
"""
def bellman_ford(nb_nodes, edges, start):
    distances = [float('inf')] * nb_nodes
    distances[start] = 0
    
    # total time complexity: O(VE) time complexity.
    for _ in range(nb_nodes-1): # O(V) complexity
        updated = False
        for from_, to_, cost in edges: # O(E) complexity
            if distances[from_] < float('inf'): # reachable
                if (offer := distances[from_] + cost) < distances[to_]: # can we relax the current path to TO_?
                    updated = True 
                    distances[to_] = offer

        if not updated: # if we could not relax any subpath (s, v), then we won't be able to next iteration either.
            return distances
    # one last time to determine all edges that can still be relaxed -> for all vertices of such edges arbitrary shortest exists
    # (negative weight cycle reachable from start)
    # could redo it for all |V| edges right ?
    # because all accessibles should be accessible in less than |V] edges (as in bellman ford)
    for _ in range(nb_nodes):
        for from_, to_, cost in edges:
            if distances[from_] < float("inf"):
                if distances[from_] + cost < distances[to_]:
                    distances[to_] = - float('inf')

    return distances

    

n,m,q,s = map(int, input().split())
first = True
while [n,m,q,s] != [0,0,0,0]:
    if not first:
        print()
    first = False

    edges = []
    for _ in range(m):
        edges.append(tuple(map(int, input().split())))


    distances = bellman_ford(n, edges, s)
    for _ in range(q):
        t = int(input())
        if distances[t] == float('inf'):
            print("Impossible")
        elif distances[t] == -float('inf'):
            print("-Infinity")
        else:
            print(distances[t])

    n,m,q,s = map(int, input().split())