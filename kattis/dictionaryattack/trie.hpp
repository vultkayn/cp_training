#include <string>
#include <memory>
#include <algorithm>
#include <array>

// benpr438 liu

namespace alphabet
{
  struct english {
    constexpr static std::size_t size = 52;
    constexpr static const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    constexpr static inline
    std::size_t index (char c)
    {
      return c < 'a' ? c - 'A' : c - 'a' + 26;
    }
  };

  struct lowercase {
    constexpr static std::size_t size = 26;
    constexpr static const char charset[] = "abcdefghijklmnopqrstuvwxyz";
    constexpr static inline
    std::size_t index (char c)
    {
      return c - 'a';
    }
  };
}

template <class Alphabet>
class PrefixTrie {
  struct TrieNode {
      TrieNode () : nchild(0) {
          std::fill(succ, succ + Alphabet::size, nullptr);
      }

      TrieNode* succ[PrefixTrie::alphabet_t::size];
      int nchild = 0; // nb of leaves under this node, this one not included -> allow answer in O(len(prefix)) if prefix already in trie
      bool leaf = false;
  };

  /* 0 not found and created.
    1 found.
  */
public:
  using alphabet_t = Alphabet;
  int get_or_add (std::string const& word);
  bool find (std::string const& word, bool strict = false) const;

private:
  TrieNode root;
};

template <class Alphabet>
int PrefixTrie<Alphabet>::get_or_add (std::string const& word) {
  TrieNode *cnode = &root;
  int found = 0;
  for (char c: word) {
      int idx = alphabet_t::index (c);
      if (cnode->succ[idx])
          cnode = cnode->succ[idx];
      else {
          // created = true;
          cnode->succ[idx] = new TrieNode();
          cnode = cnode->succ[idx];
      }
  }
  if (cnode->leaf == true)
    found = 1;
  cnode->leaf = true;
  return found;
}

template<class Alphabet>
bool PrefixTrie<Alphabet>::find (std::string const& word, bool strict) const
{
  const TrieNode *cnode = &root;
  for (char c: word) {
      int idx = alphabet_t::index (c);
      if (cnode->succ[idx])
          cnode = cnode->succ[idx];
      else
        return false;
  }
  return strict == false or cnode->leaf == true;
}
