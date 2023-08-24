/**
 * @author benpr438
 * @brief Build a Suffix Array, that is a structure that given a string,
 * will build an array of its suffixes, all lexicographically sorted.
 * Element of this array does not contain a suffix proper, but the index in the initial
 * string where it begins.
 * @date 2023-06-03
 * 
 * 
 */

#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;


/**
 * @brief Implementation details of the algorithm
 * 
 */
namespace details {
    /**
     * @brief a n-gram, n being a power of 2 (when possible),
     * is subdivision of the initial strings for efficient comparison.
     * Has a left rank and a right rank, that allows ordering by comparing only two integers,
     * instead of full strings.
     * 
     */
    struct ngram {
        int beg_idx;
        int lrank;
        int rrank;
        ngram(int idx, int lhs, int rhs) : beg_idx{idx}, lrank{lhs}, rrank{rhs} {}
    };
}
struct SuffixArray {
    // O(nlog²n) complexity since I'm using builtin sort (=IntroSort) instead of radix or counting.
    SuffixArray(string const& text) {
        vector<details::ngram> ngrams;
        int k = 1;
        // initial one character of each suffix
        for (int i = 0; i < text.size(); i++) {
            ngrams.emplace_back(i, (char) (text[i]), (i == text.size() -1 ? -1 : (char) text[i+1]));
        }
        auto comp = [](details::ngram a, details::ngram b) {return (a.lrank < b.lrank) or (a.lrank == b.lrank and a.rrank < b.rrank);};
        sort(ngrams.begin(), ngrams.end(), comp);
        // sorted pairs of characters
        int plrank, prrank, rank; 
        vector<int> indices (ngrams.size()); // map suffix index to position in ngrams
        for (k=4; k < text.size() << 1; k <<= 1) {
            // give new left rank
            plrank = ngrams[0].lrank;
            prrank = ngrams[0].rrank;
            ngrams[0].lrank = rank = 0;
            indices[ngrams[0].beg_idx] = 0; // forgot that line
            for (int i =1; i < text.size(); i++) {
                if (not (plrank == ngrams[i].lrank and prrank == ngrams[i].rrank))
                    ++rank;
                plrank = ngrams[i].lrank, prrank = ngrams[i].rrank;
                ngrams[i].lrank = rank;
                indices[ngrams[i].beg_idx] = i; // remember idx (surely have cahnged after previous sort)
            }

            // give new right rank according to ngrams[beg_idx + k].lrank if beg_idx + k not out of bounds
            for (int i = 0; i < text.size(); i++) {
                if (ngrams[i].beg_idx + (k>>1) >= text.size())
                    ngrams[i].rrank = -1;
                else
                    ngrams[i].rrank = ngrams[indices[ngrams[i].beg_idx + (k>>1)]].lrank;
            }

            // sort suffixes
            sort(ngrams.begin(), ngrams.end(), comp);
        }

        for (auto ngram: ngrams) {
            suffixes.push_back(ngram.beg_idx);
        }
    }

    // return the ith smallest suffix (in lexico order) starting position.
    inline int getSuffix(int query) {
        return suffixes[query];
    }

    // lexico sorted 
    vector<int> suffixes;
};

int main(){
    string text {};
    while (getline(cin, text)) {
        SuffixArray arr(text);
        int nb_query;
        cin >> nb_query;
        int query;
        while (--nb_query) {
            cin >> query;
            cout << arr.getSuffix(query) << ' ';
        }
        cin >> query;
        cout << arr.getSuffix(query) <<  endl;
        // second getline to clean \n 
        getline(cin, text);
    }
    return 0;
}
