#include <iostream>
#include <vector>
#include <string>
using namespace std;
using vvi = vector<vector<long>>;

// benpr438 liu

#include "trie.hpp"

using alphabet::english;
using Trie = PrefixTrie<english>;

bool digit (char c)
{
  return '0' <= c and c <= '9';
}

struct password
{
  string word;
  unsigned nbdigits;
};

bool weak_pwd (Trie const& trie, password pwd, unsigned moves)
{
  if (moves < pwd.nbdigits) return false;
  if (moves == 0) return trie.find (pwd.word, true);
  
  if (pwd.nbdigits)
  {
    // 15 loops at most
    pwd.nbdigits--;
    for (char& c: pwd.word)
      if (digit (c))
      {
        char old = c;
        for (auto i = 0; i < english::size; i++)
        {
          c = english::charset[i];
          if (weak_pwd (trie, pwd, moves-1)) return true;
        }
        c = old;
        return false;
      }
  }
  
  // no digits lefts: try swapping around.
  for (auto i = 0; i < pwd.word.size() -1; i++)
  {
    swap (pwd.word[i], pwd.word[i+1]);
    if (weak_pwd (trie, pwd, moves-1)) return true;
    swap (pwd.word[i], pwd.word[i+1]);
  }
  return trie.find (pwd.word, true);
}

int main () {
  int N;
  cin >> N;
  Trie trie;
  string word;
  getline (cin, word);
  while (N--)
  {
    getline (cin, word);
    trie.get_or_add (word);
  }

  while (getline (cin, word))
  {
    unsigned nbdigits = 0;
    for (char c: word)
      nbdigits += digit (c); 
    if (not weak_pwd (trie, {word, nbdigits}, 3))
      cout << word << endl;
  }

  return 0;
}
