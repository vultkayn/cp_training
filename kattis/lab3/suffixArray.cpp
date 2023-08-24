#include "suffixArray.hpp"

#include <algorithm>

SuffixArray::SuffixArray(std::string const &text)
{
  std::vector<ngram> ngrams;
  int k = 1;
  // initial one character of each suffix
  for (size_t i = 0; i < text.size(); i++)
  {
    ngrams.emplace_back(i, (char)(text[i]), (i == text.size() - 1 ? -1 : (char)text[i + 1]));
  }
  auto comp = [](ngram a, ngram b)
  { return (a.lrank < b.lrank) or (a.lrank == b.lrank and a.rrank < b.rrank); };
  std::sort(ngrams.begin(), ngrams.end(), comp);
  // sorted pairs of characters
  int plrank, prrank, rank;
  std::vector<int> indices(ngrams.size()); // map suffix index to position in ngrams
  for (k = 4; k < text.size(); k <<= 1)
  {
    // give new left rank
    plrank = ngrams[0].lrank;
    prrank = ngrams[0].rrank;
    ngrams[0].lrank = rank = 0;
    indices[ngrams[0].beg_idx] = 0; // forgot that line
    for (size_t i = 1; i < text.size(); i++)
    {
      if (not(plrank == ngrams[i].lrank and prrank == ngrams[i].rrank))
        ++rank;
      plrank = ngrams[i].lrank, prrank = ngrams[i].rrank;
      ngrams[i].lrank = rank;
      indices[ngrams[i].beg_idx] = i; // remember idx (surely have cahnged after previous sort)
    }

    // give new right rank according to ngrams[beg_idx + k].lrank if beg_idx + k not out of bounds
    for (size_t i = 0; i < text.size(); i++)
    {
      if (ngrams[i].beg_idx + (k >> 1) >= text.size())
        ngrams[i].rrank = -1;
      else
        ngrams[i].rrank = ngrams[indices[ngrams[i].beg_idx + (k >> 1)]].lrank;
    }

    // sort suffixes
    sort(ngrams.begin(), ngrams.end(), comp);
  }

  for (auto ngram : ngrams)
  {
    suffixes.push_back(ngram.beg_idx);
  }
}

size_t longestPrefixL(std::string const &text, size_t h, size_t sfx1, size_t sfx2)
{
  sfx1 += h;
  sfx2 += h;
  while (std::max(sfx1, sfx2) < text.size() and text[sfx1++] == text[sfx2++]) h++;
  
  return h;
}

/// @brief 
/// @param text 
/// @return max LCP length 
size_t SuffixArray::buildLCP(std::string const &text)
{
  auto n = suffixes.size();
  size_t maxL = 0;
  lcp.resize(n);
  std::vector<size_t> rank(n);
  for (size_t i =0; i < n; i++)
    rank[suffixes[i]] = i;

  size_t h = 0;
  lcp[0] = 0;
  for (size_t sfxPos =0; sfxPos < n; sfxPos++){ // O(n)
    auto precedingSfxPos = n;
    if (rank[sfxPos] > 0) // precding suffix exists
      precedingSfxPos = suffixes[rank[sfxPos] - 1];
    if (precedingSfxPos < n)
    {
      size_t k = h;
      size_t sfx1 = sfxPos + k;
      size_t sfx2 = precedingSfxPos + k;
      while (std::max(sfx1, sfx2) < text.size() and text[sfx1++] == text[sfx2++]) k++;
  
      lcp[rank[sfxPos]] = k;
      h = k;
      // lcp[rank[sfxPos]] = longestPrefixL(text, h, sfxPos, precedingSfxPos);
      maxL = std::max(k, maxL);
    }
    h = h == 0 ? 0 : h - 1;
  }
  return maxL;
}
