# greedy: first satisfy the most greedy kids, as if they are fully deprived of candies, they will be so much more angry
from sys import stderr
from heapq import *

candies, n = map(int, input().split())

eagerness = []
total_sum = 0
for _ in range(n): # O(N)
    eagerness.append(int(input()))
    total_sum += eagerness[-1]

def min_anger(eagerness, candies):
    if len(eagerness) == 1:
        return (abs(eagerness[0]) - candies) ** 2

    heapify(eagerness)
    # print("initial heap", eagerness, file=stderr)

    while candies != 0: # O(M) !!!!!!!
        gtest = heappop(eagerness)
        gtest2 = eagerness[0]
        desired = (gtest2 - gtest) + 1
        got = min(candies, desired)
        gtest += got
        candies -= got
        heappush(eagerness, gtest)

    # print("final heap", eagerness, file=stderr)
    angriness = 0
    for elem in eagerness: # O(N)
        # print(elem, file=stderr)
        angriness += elem ** 2
    return angriness


def dichotomy(eagerness, reach, threshold):
    a, b = 0, reach
    while b > a and eagerness[b -1] < threshold:
        mid = (a + b) // 2
        v = eagerness[mid]
        if v < threshold:
            b = mid
        elif v >= threshold:
            a = mid
    reach = b
    return reach

    

def min_angermean(eagerness, candies, total_sum):
    if len(eagerness) == 1:
        return (abs(eagerness[0]) - candies) ** 2
    
    eagerness.sort()

    reach = len(eagerness)
    angriness = 0
    total_sum -= candies # here is the main difference with previously
    for i, eager in enumerate(eagerness):
        child_angriness = min(eager, total_sum // reach) # either child dont have any candy, or all the other with greater needs get the same amount
        # same idea as previously with the threshold, but in this order there no longer is a need to loop on the available candies
        reach -= 1
        total_sum -= child_angriness
        angriness += child_angriness ** 2
    return angriness



print(min_angermean(eagerness, candies, total_sum))