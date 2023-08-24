#include <iostream>
#include <vector>
using namespace std;
using vvi = vector<vector<long>>;




template <size_t alphabetSize>
struct PrefixTrie {
  class TrieNode {
      public:
      TrieNode() : nchild(0) {
          fill(succ, succ + alphabetSize, nullptr);
      }

      TrieNode* succ[alphabetSize];
      int nchild = 0; // nb of leaves under this node, this one not included -> allow answer in O(len(prefix)) if prefix already in trie
      int appear = 0;
  };

  int find_or_add(string word) {
      TrieNode *cnode = &root; 
      bool created = false;
      for (char c: word) {
          int idx = c - 'a';
          if (cnode->succ[idx]) {
              cnode = cnode->succ[idx];
          } else {
              // created = true;
              cnode->succ[idx] = new TrieNode();
              cnode = cnode->succ[idx];
          }
      }

      // update nb of childs
      // created |= not cnode->appear;
      // int add_ = (int) (not created); // add 1 if prefix was added already before.
      auto answer = cnode->nchild + cnode->appear;
      cnode->appear ++;

      // if (created) {
          cnode = &root;
          for (char c: word) { // depth 32 at most cuz 32 characters max
              cnode->nchild ++;
              int idx = c - 'a';
              cnode = cnode->succ[idx];
          }
      // }        

      return answer;
  }

  TrieNode root;
};

int main(){
    int N;
    cin >> N;
    PrefixTrie<26> trie;
    string word;
    getline(cin, word); // purge '\n'
    for (int i =0; i < N; i++) {
        getline(cin, word);
        cout << trie.find_or_add(word) << endl;
    }

    return 0;
}
