#include <iostream>
#include <cmath>
using namespace std;

// 13! > 2^31
// hence possible prime factors of n!, given n! <= 2^31, are {2,3,5,7,11}
#define div " divides "
#define divn " does not divide "


const char* m_divide_n(unsigned m, unsigned n) {
    if (not m) return divn;
    if ((m == 1 and !n) or m <= n) return div; // cmon 5 doesn't divide 0 ? prob not
    unsigned sqm = sqrt(m);
    for (unsigned d = 2; d <= sqm and m > n; d++) { // O(2**15) iterations at most (if m <= n then n! will be divided by)
        unsigned k = 0;
        while (!(m % d)) {
            m /= d;
            k++;
        }
        unsigned Q = 0;
        unsigned power = d;
        while (power <= n and Q < k) { // O(63) at most
            Q += n / power;
            power *= d;
        }

        if (Q < k) return divn;
    }

    // if m was prime and bigger than n, then at this point it will still be bigger
    // because its dividers are 1 < d=2 and m > sqrt(m)
    
    return (m <= n) ? div : divn;
}

int main() {
    unsigned n, m;
    while (cin >> n >> m) {
        cout << m << m_divide_n(m, n) << n << '!' << endl;
    }
    return 0;
}

/*
First IDEA: [problem, decomposing m might proved too expensive (obviously duh).]
For m to divide n!, prime factors of m must be a subset (counted with their multiplicty) of n's prime factors.
Thus, decompose m into its prime factors. Write occurrence of each in an array.
Then do integer division of n by each of m's prime factor. If the result is less than the written down multiplicity,
then m cannot divide n!.

Let F be a unique prime factor of m.
Let Q = n // F;
Then, F, F*2, F*3, ... F*Q etc will appear in n!. Gotta consider power of F appearing too !
Eg: for F = 2, and n = 6, 6/2 = 3, yet in 2*4*6 2 appears 4 times, because a 2 was forgotten in '4=2*2'.
Hence in n!, F will have a multiplicity of 1 + 2 + 3 +... + Q = Q(Q+1)/2
If Q(Q+1)/2 < multiplicity_in_m(F), then m do not divide n!. 
*/

/*
Second IDEA: [GOOD]
if m <= n, then m divides n!. O(1) test
if m > n, m being prime, then m cannot divide n!, as then none of 1..n contains m as a prime factor. worst O(2^14) test, burk!

what if we apply IDEA 1, but don't care about redundancy of dividers of m ?
Just iterate through dividers of m, divise m by their amount, note their multiplicity.
Let D such divider, if D^k divides m then it must also divides n for m to divide n.
By same calculus that above in IDEA 1, Let Q_i = n // (D^i), if sum(Q_i) < k, then m does NOT divide n.
Since n <= 2^31, sum(Q) cannot be over (upperbound for simplicity) 32 + 16 + 8 + 4 + 2 + 1 = 63

Then we just have to consider all dividers of m, prime of not, which are less than sqrt(m) (O(2^15) -> about 32000, doable!)

Loop should then at most cost O(63 * 2^15) = O(10^6) nice
*/