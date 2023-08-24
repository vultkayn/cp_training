#pragma once

#include <string>
#include <vector>

struct SuffixArray
{
  struct ngram
  {
    int beg_idx;
    int lrank;
    int rrank;
    ngram(int idx, int lhs, int rhs) : beg_idx{idx}, lrank{lhs}, rrank{rhs} {}
  };
  
/**
 * @author benpr438
 * @brief Build a Suffix Array, that is a structure that given a string,
 * will build an array of its suffixes, all lexicographically sorted.
 * Element of this array does not contain a suffix proper, but the index in the initial
 * string where it begins.
 * Time Complexity: O(Nlog²N) complexity since I'm using builtin sort (=IntroSort) instead of radix or counting.  
 * 
 */
  SuffixArray(std::string const &text);

  // return the ith smallest suffix (in lexico order) starting position.
  inline int getSuffix(int query)
  {
    return suffixes[query];
  }

  // Use kasai's algorithm
  // return max LCP
/**
 * @author benpr438
 * @brief Once the Suffix Array has been built, construct a Longest Common Prefix (LCP) array, using
 * Kasai's algorithm. The SuffixArray instance will then hold a valid 'lcp' member, that at index i,
 * gives the length of the longest common prefix between suffix ordered at i in the suffixArray, and
 * suffix ordered at i-1. For suffix 0, the value will be 0.
 * Time Complexity: O(N)  
 * 
 * @return The maximum of the built lcp array, that is the length of the longest common prefix between two adjacent
 * suffixes
 */
  size_t buildLCP(std::string const &text);

  // lexico sorted
  std::vector<int> suffixes;
  std::vector<size_t> lcp;
};