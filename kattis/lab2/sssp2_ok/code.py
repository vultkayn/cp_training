#!/usr/bin/env python3

'''
@author benpr438
@brief Solve a Single-source shortest-path problem, with a time table to respect.

Similar to a train station schedule, certain edges (eg roads) can only be taken at specific times.
Actually, some of them can only be taken once, while some others can be taken a repetitive intervals,
but only after their first departure time.

A traveller trying to go from s to t, at a first departing time of 0, wants to reach t as early as possible.
They can stand still at a node, waiting for the next train.
The roads however have a certain length, thus it takes a certain time to go cross it. A road (u,v) can have any
non-negative length, this length being the required minimum time go from u to v.

That is, if there is a departing train at station u at time t, and (u,v) is of length d, then we can reach v at t+d.

The problem is to find the shortest path (s,t), i.e.  an ordered sequence of edges (s,v1), (v2,v3), ... + (vj, t) such as the arriving time at t is the earliest.
'''



# SSSP Non negative weights input -> 

from heapq import *
from sys import stderr


"""
@returns An array of the earliest arrival time to every nodes, departing from node 'start'.
If it impossible to reach u starting from 'start', then arrivals[u] will have an infinite arrival time.
"""
def dijkstra_time_table(adj_list, nb_nodes, start):
    # if period == 0 -> min_departure_time only
    # else period > 0 -> min_departure_t + k*period are valid departure time
    # lets keep track of minimum arrival time (= shortest distance in regular dijsktra)
    # and of all incoming nodes that give that minimum
    # if new strict minimum is reached, flush list of incoming node and add the new one
    # don't visit a node twice (as in regular dijsktra)
    arrivals = [float('inf')]  * nb_nodes
    arrivals[start] = 0
    prioQ = [[arrivals[start], start]]
    visited = [False] * nb_nodes
    while prioQ: # O(E)
        arrival_time, node = heappop(prioQ)
        visited[node] = True
        for traveltime, neighbor, min_departure_t, period in adj_list[node]: # O(V) if no possible edge, O(E) otherwise
            departure_t = float('inf')
            if arrival_time <= min_departure_t:
                departure_t = min_departure_t
            elif period != 0:
                departure_t = arrival_time + (period - (arrival_time - min_departure_t) % period) % period
            neigh_arrival_t = departure_t + traveltime

            if not visited[neighbor] and neigh_arrival_t <= arrivals[neighbor]:
                heappush(prioQ, [neigh_arrival_t, neighbor]) # O(log E)
                arrivals[neighbor] = neigh_arrival_t

    return arrivals
    

n, m, q, s = map(int, input().split())
first = True
while [n,m,q,s] != [0,0,0,0]:
    if not first:
        print()
    first = False
    adjacency_list = [[] for _ in range(n)]
    for _ in range(m):
        u, v, t0, P, d = map(int, input().split())
        adjacency_list[u].append([d, v, t0, P]) # traveltime, neighbor, min_departure_t, period

    arrivals = dijkstra_time_table(adjacency_list, n, s)
    print(f"{arrivals=}", file=stderr)
    for _ in range(q):
        target = int(input())
        print(arrivals[target] if arrivals[target] < float('inf') else "Impossible")
    n, m, q, s = map(int, input().split())