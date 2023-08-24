
known = []
l, d, n = map(int, input().split())
for _ in range(n):
    known.append(int(input()))

known.sort()

def solve(known, l, d, n):
    if l < 12:
        return 0

    if not known:
        return (l - 12) // d + 1 # all spots are ready

    can_add = (known[0] - 6) // d + 1 - 1 # between first pigeon and left pole
    can_add += (l - 6 - known[-1]) // d + 1 - 1 # between last pigeon and right pole

    if len(known) == 1:
        return can_add

    for inf, sup in zip(known[:-1], known[1:]):
        can_add += (sup - inf) // d + 1 - 2
    
    return can_add
    

print(solve(known, l, d, n))