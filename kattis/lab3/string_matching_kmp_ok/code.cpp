/**
 * @author benpr438
 * @brief
 * Given a text and a pattern, find the positions of all the occurences of pattern in text
 * on the fly (no offline preprocessing). This is done with the KMP algorithm, in O(nlog M) time.
 * @date 2023-06-03
 * 
 * 
 */

#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
using namespace std;
using vvi = vector<vector<long>>;

/**
 * @brief Build a vector containing the positions of all the occurences of pattern in text, from first to last,
 * using the KMP algorithm.
 * 
 * Given text.size() > pattern.size(), otherwise the answer is trivial, find all occurences of 'pat' in O(NlogM) time. 
 * 
 * @param pat a non-empty string of length M
 * @param text a non-empty of length N
 * @return vector<int> 
 */
vector<int> kmp_solve(string const& pat, string const& text) {
    vector<int> prefix_fnc (pat.size(), -1);
    prefix_fnc[0] = 0;

    // compute pi for pattern
    for (int i = 1; i < pat.size(); i++) { // O(M)
        auto j = prefix_fnc[i-1];
        while (j != 0) { // O(log M)
            if (pat[i] == pat[j]) {
                prefix_fnc[i] = j + 1;
                break;
            }
            j = prefix_fnc[j-1];
        }
        if (prefix_fnc[i] == -1)
            prefix_fnc[i] = (int) (pat[i] == pat[0]);
    }

    // online computing for text
    int j = (int) (text[0] == pat[0]);
    vector<int> occurrences;
    for (int i = 1; i < text.size(); i++) { // O(N)
        if (j == pat.size()) { // match found
            occurrences.push_back(i - j);
            // j = 0; 2nd ERROR was here, since for example if we look for 'aa', then if we reset we will only find even indices, and skip all odds.
            j = prefix_fnc[j-1];// rather go back just one step.
            // continue; // FIRST ERROR was to continue here, cuz we would then miss every other match if they were contiguous.
        }
        // regular algorithm otherwise
        // we know j < pat.size() at this point, so no problem to fit in prefix_fnc
        auto new_pi = -1;
        while (j != 0) { // O(log M)
            if (text[i] == pat[j]) {
                new_pi = j + 1;
                break;
            }
            j = prefix_fnc[j-1];
        }
        if (new_pi == -1)
            new_pi = (int) (text[i] == pat[0]);
        j = new_pi;
    }
    // FIRST ERROR BIS was to forget the last possible element by adding this extra cehck.
    if (j == pat.size()) { // match found
        occurrences.push_back(text.size() - j);
    }
    return occurrences;
}

int main(){
    string pattern, text;
    while (getline(cin, pattern)) {
        getline(cin, text);

        auto occurrences = kmp_solve(pattern, text);
        auto occ = begin(occurrences);
        if (occ != end(occurrences)) {
            for (; occ != end(occurrences)-1 ; occ++)
                cout << *occ << ' ';
            cout << *occ;
        }
        cout << endl;
    }

    return 0;
}
