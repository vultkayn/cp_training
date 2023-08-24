  #include <iostream>
  #include <vector>
  #include <cmath>
  using namespace std;
  using vvi = vector<vector<long>>;

  // benpr438 Liu 

  /*
      Since computing in [A, B] is difficult (even on paper)
      because we have to trim all numbers under A, and since we are dealing we sum
      we can do [1,B] - [1, A-1], as in the "howmanyzeros" proof of concept.

      We then have way less edges cases, and we focus on just solving in [1, N] twice.
      As in a previous problem (was it during a solving session), we consider sving intermediate result
      dependent only on the length of the number.

      Because B-A can be 10^15, even a linear algorithm is not possible in O(B-A).
      This guides us to look at the other parameters, that is L = #digitsMax = log(B-A) = 16 and S <= 135
      We can easily imagine an algorithm in O(L S^3) or O(L^2 S^2).

      Using DP, we will have to loop through all S -> O(S)
          Then loop through L -> O(L)
              Again loop through  S -> O(S)

      We could go we a "naive" DP at first, that is not splitting the information over L
      but rather have a 1-D DP array, function of S
      DP[S] = set of elements in [1, N] whose sum is S
      then easily (just by taking care of edges cases when a + b > 10 ^ (ceil( log10(max(a, b)) ) )
      we can get DP[S] = Union(DP[j] + DP[i], 1 < i < j < S).

      But then memory usage can get verrry large.

      So I had to find an alternative, which to use a 2D DP array

      Let's denote DP[L][S] this DP array
      From L-1 to L we add 1-digit. that is, from a sum S_{L-1}, by adding 1 digit we can reach any of S_{L-1} + k, 0 <= k <= 9.
      we thus have to consider all sum obtained with one less digit that are in reach of S.
      DP[L][S] = sum(DP[L-1][S - k], 0 <= k <= 9) cost => O(10)

      The base case are then obviously L = 0, but then for any sum S DP[0][S] = 0
      and DP[1][S] = 1 if any S <= 9 else 0

      DP[L][9*L + x], x > 0 is not an edge case, just 0 if x >= 10
  */

  vector<vector<long long>> init (long long N, int S)
  {
    N += 10000;
    S = 136;
    const auto lmax = 1 + (int)ceil(log10(N)); // 1-based array for semantics

    vector<vector<long long>> DP(lmax, vector<long long>(S + 1, 0));
    // edge case L = 1
    for (int s = 0; s <= S and s < 10; s++)
    {
      DP[1][s] = (int)s < 10;
    }
    DP[0][0] = 1;

    for (int l = 2; l < lmax; l++)
    {
      for (int s = 0; s <= S; s++)
      {
        for (int k = 0; k < 10 and s - k >= 0; k++)
          DP[l][s] += DP[l - 1][s - k];
      }
    }
    return DP;
  }

  // O (1)
  long long count (vector<vector<long long>> const& DP, long long N, int S)
  {
    if (N == 0) return (long long) (S == 0);
    // now pick only those below B
    // B = D di di-1 ... d0
    // Then we can safely pick DP[lmax][S - x], 0 <= x < D
    // for x == D, we can only count the numbers of length lmax -1,
    // s.a. d'i d'i-1 ... d'-0 <= di di-1 ... d0
    auto x = N;
    auto ndigits = (int)ceil(log10(N));
    long long pow10 = std::pow (10, ndigits-1);
    long long counter = 0;
    while (ndigits > 1) // max 16 iterations
    {
      auto leftmost_digit = x / pow10;
      for (int k = 0; k < leftmost_digit and S - k >= 0; k++) // max 9 iterations
        counter += DP[ndigits-1][S - k];
      S -= leftmost_digit;
      x %= pow10;
      pow10 /= 10;
      ndigits--;
    }

    return counter + (S <= x);
  }

  // recursive
  long long count2(vector<vector<long long>> const& DP, long long N, int S)
  {
    if (S < 0) return 0;
    if (N == 0) return S == 0;
    // auto ndigits = 1 + (int) log10(N); // imprecise
    
    long long counter = 0;
    long long leftmost_digit = 0;
    auto tmp = N;
    int ndigits = 0;
    long long pow10 = 1;
    while (tmp)
    {
      leftmost_digit = tmp;
      tmp /= 10;
      ndigits++;
      pow10 *= 10;
    }
    if (pow10 > 1) pow10 /= 10;
    
    for (int k = 0; k < leftmost_digit and S - k >= 0; k++)
      counter += DP[ndigits-1][S-k];
    return counter + count2 (DP, N % pow10, S - leftmost_digit);
  }

  // interval is ordered, and the function count (x, S) is monotonous and increasing in x (natural order)
  // we can thus use binary search on it. We get O(log (B - A)) complexity, but log10 (B - A) is 16 at most.
  // and a call to count as we saw is a low constant, so this binary search is viable.
  long long lowest (vector<vector<long long>> const& DP, long long A, long long B, int S)
  {
    // count is so low cost that as a binary search function, and still get o(log N) 
    auto low = A;
    auto high = B;
    long long mid = low;
    while (low < high)
    {
      mid = (high + (low+1)) / 2;
      // whenever we find the lowest x >= low, such sum_digits (x) == S
      // we'll obviously get count(x,S) == (0 if digits(low)==S else 1) + count(low, S);
      if (count2 (DP, mid, S) - count2 (DP, low, S) > 0)
        high = mid-1;
      else // = 0 means digits(low) != S and we are between low, and lowest x s.a. digits(x)=S
        low = mid;
    }
    return low+1;
  }

  int main()
  {
    long long A, B, S;
    cin >> A >> B >> S;
    auto DP = init (B, S);
    cout << count2 (DP, B, S) - count2 (DP, A-1, S) << endl;
    cout << lowest (DP, A, B, S) << endl;

    return 0;
  }
