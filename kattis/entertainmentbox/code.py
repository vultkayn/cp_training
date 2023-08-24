from heapq import *

n, k = map(int, input().split())


shows = []
for _ in range(n):
    shows.append(tuple(map(int, input().split())))
shows.sort(key=lambda v: v[1]) # sort by increasing end time

finish_times = [0] * k
counter = 0

for show in shows:
    beg, end = show 
    if beg >= finish_times[0]:
        heapreplace(finish_times, end)
        counter += 1

print(counter)
