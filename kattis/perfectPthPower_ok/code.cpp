#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
using namespace std;
using vvi = vector<vector<long>>;

/* int solvePth(int x) 
{
    int sign = x < 0 ? -1 : 1;
    x = abs(x);

    // if x negative, P cannot be even

    for (int i = 2; i * i <= x; i++)
    {
        double res = logl(x) / logl(i); // could try pow 1 / P also
        int resi = round(res);
        if (abs((long double)resi - res) < 0.00)
            if (sign == -1 and (resi & 1) == 0)
                continue;
            else
                return resi;
    }

    return 1;
}
 */


/**
 * @brief Returns the number of prime number there are between 0 and N
 * Prime sieve method.
 *
 * @param n upper bounds until which we should count all the prime numbers
 * @param sieve A "bitfield"  array that will stores the prime numbers. A set bit at position i
 * means i is a prime number. Thus O(n) memory used.
 * @return vector<int> prime number 
 */
vector<int> generate_primes(int n)
{
    vector<bool> sieve(n+1, true);
    vector<int> res {2};
    // 0 and 1 are not primes
    sieve[0] = sieve[1] = false;
    if (n <= 1) // neither 0 nor 1 are primes
        return {};
    if (n == 2) // only 2 is a prime
        return res;

    // deal with 2 that is the only even prime number, so that in next loop
    // we can jump 2-units long, that is only going through the odd numbers.
    for (int i = 4; i <= n; i += 2)
        sieve[i] = false;
    

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
            res.push_back(i);
        }
    
    return res;
}

template<typename Iterator>
unsigned gcd(Iterator const& begin, Iterator const& end) {
    if (begin == end)
        return 1;
    
    auto b = begin;
    auto v = *begin;
    if (b++ == end)
        return std::gcd(v, *(++begin));
    return std::gcd(v, gcd(++begin, end));
}

/**
 * @brief x = y ^ p then y = x ^(1/p)
 * Taking the prime decomposition of x, such x = p1^e1 . p2^e2 . ... . pk ^ ek
 * It means than for all ei, ei / p must be an integer as y is an integer.
 * 
 * Then the greatest possible divisor would be P, such divisor is the greatest common divisor.
 * Except if x < 0, then P cannot be even.
 * 
 * For prime decomposition, we only need to consider primes until sqrt(x), as otherwise their exponent would be either 0 or 1,
 * then the greatest P would 1, we already have that. 
 * 
 * So instead build decomposition with primes <= sqrt(x), then if remaining part of x is still not 1, there is a prime factor bigger than sqrt(x) -> P = 1 
 * 
 * @return int 
 */

unsigned solvePth(long long x, vector<int> const& primes) {
    short sign = x < 0 ? -1 : 1;
    x = ( x < 0 ) ? -x : x;

    // prime decomposition
    unsigned gcd_exponents = 0;
    for (int i = 0, p = primes[0]; p*p <= x and i < primes.size(); ++i)
    {
        p = primes[i];
        
        unsigned factor_exponent = 0;
        while (x % p == 0)
        {
            x /= p;
            factor_exponent++;
        }
        if (factor_exponent) {
            gcd_exponents = std::gcd(gcd_exponents, factor_exponent);
        }
        
    }

    unsigned pth = max((unsigned) 1, gcd_exponents);

    if (sign < 0)
        while ((pth & 1) == 0)
            pth >>= 1;
    
    return pth;
}

int main()
{
    long long x;
    cin >> x;
    auto primes = generate_primes(65536);
    while (x)
    {
        cout << solvePth(x, primes) << endl;
        cin >> x;
    }

    return 0;
}
