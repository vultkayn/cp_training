// TDDD95 benpr438
#include <iostream>
#include <vector>
using namespace std;
using vvi = vector<vector<long>>;



/**
 * @brief Return indices of the LIS (Longest Increasing Subsequence) within the given sequence of values SEQ.
 * A subsequence is an ordered subset of the initial sequence, that is we must keep the relative order of the elements,
 * but can skip some to build the sequence.
 * An increasing subsequence is one whose for any two ordered elements x_i, x_j, with i < j, then x_i < x_j.
 * This function finds any such subsequence that is of maximal length. 
 * 
 * Worst case time complexity of O(N log N), with N = SEQ.size().
 * O(N) extra memory.
 * 
 * @param seq The sequence for which to compute the LIS
 * @return vector<size_t> Indices within SEQ of the optimal longest LIS of SEQ.
 */
vector<size_t> solveLIS(vector<long> const &seq)
{
/**
 * @brief I call "optimal LIS" a LIS whose the last element is minimal.
 * Indeed, such LIS will be the easiest to extend, thus I call it optimal.
 * 
 * The idea of the algorithm below is that instead of trying all LIS that could
 * accomodate an extension by X[i], we only consider the optimal LIS that could do so.
 * By doing so, we no longer need to consider all j such as j < i and X[j] < X[i],
 * but only the actual LIS.
 * 
 * More specifically, among the previously found LIS,
 * we consider the shortest optimal LIS that could not be extended by X[i].
 * Let's name it minB as minimal bigger of length LEN(minB).
 * Then all optimal LIS of length < LEN(minB) ends with an element lower than X[i].
 * In particular, if we take the longest of those, that is a LIS of length LEN(minB) -1,
 * we can add X[i] at the end of it, thus obtaining a new optimal LIS of length LEN(minB).
 * If no such minB was found, it means that currently all optimal LIS ends with something lower than X[i].
 * We can just pick the longest of them all, add X[i], thus forming a new longest LIS ending with X[i].
 * 
 * Since the lengths are trivially ordered within the array of optimals, we can apply a binary search on it,
 * hence reducing the complexity of the inner loop from O(n) to O(log maxL).
 * In the worst case, maxL = N, thus a total time complexity of O(N log N).
*/

    vector<size_t> optimal_last;
    vector<size_t> predecessors(seq.size());
    auto &opt{optimal_last};
    opt.push_back(0); // dummy first element never accessed to get 1-based indexing

    size_t maxL = 1;
    opt.push_back(0); // at initialization, the optimal LIS of length 1 ends with seq[0];
    predecessors[0] = 0;
    for (size_t i = 1; i < seq.size(); i++)
    { // -> loop all elements = O(N)
        // binary search -> O(log maxL) = O(N) complexity
        size_t left = 1, right = maxL + 1;
        while (left < right)
        {
            size_t mid = (left + right) / 2;
            if (seq[opt[mid]] < seq[i])
                left = mid + 1; // left is always reachable.
            else
                right = mid; // right is always one beyond, so we will never reconsider mid anyway.
        }
        // since l was chosen to be the smallest l such as X[opt[l]] > X[i]
        // it means that X[opt[l-1]] <= X[i].
        // Thus it we take the LIS ending at opt[l-1], we get a LIS of length (l-1)
        // such as its last element is lower than the current X[i].
        // Hence we can make a new LIS of length l, ending at X[i], with is better than the previous optimal
        // (since X[opt[l]] > X[i]).
        // Thus at this point opt[l] = i;
        size_t l = left; // l is the smallest LIS length such as X[opt[l]] is bigger than X[i]
        predecessors[i] = opt[l - 1];
        if (l <= maxL)
            opt[l] = i;
        else
        {
            // no l was found such as there was a LIS whose last element was bigger than X[i]
            // Otherwise, all current optimal LIS ends with something lower than X[i]
            // Thus we can add X[i] to the longest of them all, and form a new LIS of length maxL + 1
            opt.push_back(i);
            maxL++;
        }
    }

    // Build solution array.
    std::vector<size_t> LIS_elems(maxL);
    for (size_t idx = opt[maxL], L = maxL; L > 0; idx = predecessors[idx])
        LIS_elems[--L] = idx;
    return LIS_elems;
}

int main()
{

    string Nstr{};
    while (getline(cin, Nstr))
    {
        size_t N = stoi(Nstr);

        vector<long> sequence(N);
        for (size_t i = 0; i < N; i++)
        {
            long x;
            cin >> x;
            sequence[i] = x;
        }

        auto lis{solveLIS(sequence)};
        cout << lis.size() << '\n';
        for (size_t i = 0; i < lis.size() - 1; i++)
            cout << lis[i] << ' ';
        cout << lis.back() << endl;

        getline(cin, Nstr); // read dangling \n
    }

    return 0;
}
