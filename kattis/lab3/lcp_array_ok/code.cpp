/**
 * @author benpr438
 * @brief Given a string S, output the length of the longest string that appears twice within S on a single line.
 * 
 * Once the Suffix Array has been built, construct a Longest Common Prefix (LCP) array, using
 * Kasai's algorithm. The SuffixArray instance will then hold a valid 'lcp' member, that at index i,
 * gives the length of the longest common prefix between suffix ordered at i in the suffixArray, and
 * suffix ordered at i-1. For suffix 0, the value will be 0.
 * Time Complexity: O(N) for the LCP proper, but an extra O(NlogN) to first build the SuffixArray.
 * 
 * @return The maximum of the built lcp array, that is the length of the longest common prefix between two adjacent
 * suffixes
 */

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using vvi = vector<vector<long>>;

#include "../suffixArray.hpp"

// kattis's name is dvaput not lcp_array
int main(){
    int L;
    cin >> L;

    std::string text;
    cin >> text;

    SuffixArray sfxA {text};
    cout << sfxA.buildLCP(text) << endl;

    return 0;
}
