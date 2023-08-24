from heapq import *
from sys import stderr

v = []
u = []

edges = {}

# read input

n = int(input())
leaves = {1+i for i in range(n+1)}
for _ in range(n):
    node = int(input())
    edges[node] = edges.get(node,0) + 1
    v.append(node)
    leaves.discard(node)

leaves.discard(n+1)
leaves = list(leaves)
heapify(leaves)

print(f"{edges=}, initial {leaves=}", file=stderr)


# find initial leaves

for i, vertex in enumerate(v):
    if not len(leaves):
        print("Error")
        break
    leaf = heappop(leaves)
    u.append(leaf)
    edges[vertex] -= 1
    if edges[vertex] == 0:
        heappush(leaves, vertex)
else:
    for n in u:
        print(n)
