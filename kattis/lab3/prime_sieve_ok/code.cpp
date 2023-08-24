/**
 * @author benpr438
 * @brief Implementation of the Sieve of Erathostenes,
 * that is a relatively efficient algorithm for the generation of primes.
 * Given an upper bound N, the algorithm will delete all non-prime numbers (by making sieve[x] = false)
 * from the initial array, as well as return the number of prime numbers found in [0, N].
 * Time complexity: O(N log(log N))
 * @date 2023-06-03
 * 
 */

#include <iostream>
#include <vector>
#include <bitset>
using namespace std;
using vvi = vector<vector<long>>;

/**
 * @brief Returns the number of prime number there are between 0 and N
 *
 * @param n upper bounds until which we should count all the prime numbers
 * @param sieve A "bitfield"  array that will stores the prime numbers. A set bit at position i
 * means i is a prime number. Thus O(n) memory used.
 * @return int The number of prime numbers.
 */
int generate_primes(int n, vector<bool> &sieve)
{
    // 0 and 1 are not primes
    sieve[0] = sieve[1] = false;
    if (n <= 1) // neither 0 nor 1 are primes
        return 0;
    if (n == 2) // only 2 is a prime
        return 1;

    // deal with 2 that is the only even prime number, so that in next loop
    // we can jump 2-units long, that is only going through the odd numbers.
    for (int i = 4; i <= n; i += 2)
        sieve[i] = false;

    int count = 1; // 2 is prime
    // here we know n >= 3
    for (int i = 3; i <= n; i += 2) // don't bother with even numbers. O(n) loop here.
        if (sieve[i]) // this case becomes rarer and rarer, as primes density diminishes the bigger we go.
        {
            /* flip off all multiples of sieve[i], they no longer can be primes number.
            * The number of elements to flip off is thus n / sieve[i].
            * Over all prime numbers, the complexity of the outer loop is thus n/3 + n/5 + n/7 ...
            * That is n (1/3 + 1/5 + 1/7 ...)
            * It is similar looking to the Harmonic's serie, that is the serie of ln(1 + 1/n),
            * and from which we would have taken only the primes divisor.
            * The name of such serie is actually Harmonic Series of primes, that equals log(log(n))
            * The time complexity of this function is thus O(n log(log n))
            */
            for (int j = i; j <= n; j += i)
                sieve[j] = false;
            sieve[i] = true;
            ++count;
        }
    
    return count;
}

int main()
{
    int n, nquery;
    // vector for simplicity, as I believe bitset by default don't store their elements in the free store.
    // while vectors do.

    // default initialization to the maximum entry, trading often unnecessary space with always 
    // a lesser number of allocations, as well as not having to manage the random insertion of new elements.
    vector<bool> sieve(100'000'001, true);
    cin >> n >> nquery;
    auto primes = generate_primes(n, sieve);
    cout << primes << endl;
    for (int i = 0; i < nquery; i++)
    {
        int query;
        cin >> query;
        cout << ((sieve[query]) ? 1 : 0) << endl;
    }

    return 0;
}
