#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
using vvi = vector<vector<long>>;

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* PYTHON super slow Proof of Concept substraction is good 
def nbzero (n):
    if n < 0:
        return 0
    if n == 0:
        return 1
    nb = 0
    for i in range(n+1):
        for c in str(i):
            if c == '0':
                nb += 1
    return nb

m, n = [0,0]
while m >= 0:
    m, n = map(int, input().split())
    nbn = nbzero(n)
    nbm = nbzero(m-1)

    print(f"{nbn=}, {nbm=}, between {nbn - nbm}") */


// instead let's save number of 0 occuring depending on the digit rank

long countzeros(long n) {
    if (n < 0) return 0;
    if (n == 0) return 1;

    long nofzero = 0;
    // case for first digit apart
    nofzero += n / 10 + 1;

    long prev = 10;
    long digit = 100;
    while (digit < n) {
        nofzero += MAX(n / digit - 1, 0) * prev + MIN(n % digit + 1, prev);
        prev = digit;
        digit *= 10;
    }

    return nofzero;
}


int main(){

    long m = 0, n;
    while (m >= 0) {
        cin >> m >> n;
        cout << countzeros(n) << endl;
    }

    return 0;
}
