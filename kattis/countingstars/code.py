
def readF():
    m, n = input().split(" ")
    m = int(m); n = int(n)
    sky = [[0] * n for _ in range(m)] 
    for i in range(m):
        line = input()
        for j, cell in enumerate(line):
            sky[i][j] = int(cell == '-')
    return sky

def lookup(sky,i,j):
    if i < 0 or j < 0 or i == len(sky) or j == len(sky[i]) or sky[i][j] == 0 or sky[i][j] == 2:
        return 
    sky[i][j] = 2
    lookup(sky, i, j+1)
    lookup(sky, i-1, j)
    lookup(sky, i, j-1)
    lookup(sky, i+1, j)

cas = 0
while True:
    cas += 1
    counter = 0
    try:
        sky = readF()
        for i, row in enumerate(sky):
            for j in range(len(row)):
                if sky[i][j] == 1:
                    lookup(sky, i, j)
                    counter += 1
        print(f"Case {cas}: {counter}")
    except:
        break
