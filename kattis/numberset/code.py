primes1000 = [ 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997 ]

C = int(input())


def primefactors(x, primes):
    factors = 1 << len(primes)
    i = 0
    while i < len(primes) and primes[i] < x:
        p = primes[i]
        if x % p == 0:
            factors |= 1 << i
        i += 1
    return factors

def remdup(sets):
    res = set()
    for s in sets:
        s = frozenset(s)
        res.add(s)
    return res
    
def merge(sets, i, j):
    new = sets[i].union(sets[j])
    sets[i] = sets[j] = new


for ci in range(C):
    A, B, P = map(int, input().split())

    primes = [p for p in primes1000 if P <= p <= B]
    factors = []
    sets = [{x} for x in range(A, B+1)]


    for x in range(A, B+1):
        factors.append(primefactors(x, primes))

    print(factors, primes)

    for i, x in enumerate(range(A, B)):
        for j, y in enumerate(range(x+1, B+1)):
            if factors[i] & factors[j]:
                merge(sets, i, j)

    print(f"Case #{ci}: {len(remdup(sets))}")