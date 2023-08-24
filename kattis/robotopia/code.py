n = int(input())


for _ in range(n):
    l1, a1, l2, a2, lt, at = map(int, input().split())
    x, y = 0.1, 0.1
    if a2 - a1*l2/l1 == 0:
        x = (lt + lt/l1) / (1 + l1)
        y = (x - lt / l1) / l2
    else:
        y = (at*l1 - a1*lt)/(a2*l1 -a1*l2)
        x = lt/l1 - y*l2/l1

    if int(x) != x or int(y) != y or int(y) <= 0 or int(x) <= 0:
        print('?')
    else:
        print(x, y)