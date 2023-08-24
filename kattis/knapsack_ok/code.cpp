// TDDD95 benpr438
#include <iostream>
#include <vector>
#include <array>
#include <sstream>
#include <algorithm>

using namespace std;

using weight_t = unsigned;
/**
 * @brief Solve a regular 0-1 knapsack problem
 * That is, given a Capacity C of the knapsack,
 * n objects of value vi and weight wi, maximize the total value
 * of the knapsack without exceeding its capacity.
 * 
 * Time complexity: O(C * n) 
 * Space complexity: O(2 * C) = O(C) for the problem itself,
 * * extra intermediate O(n*n) to construct the solution array. 
 * 
 * @param C 
 * @param n 
 * @param weights 
 * @param values 
 * @return vector<int> An array of elements that if taken, maximize the value of the knapsack.
 * There might be multiple such array, not necessarily all of the same size.
 * Returns any of these. 
 */
vector<int> knapsack(int C, int n, std::vector<int> const &weights, vector<int> const &values)
{
    vector<vector<bool>> taken(n);
    vector<int> dp_prev(C + 1);
    vector<int> dp_curr(C + 1);

    dp_prev[0] = 0;
    for (auto c = 1; c < min(C + 1, weights[0]); c++) { // O(C)
        dp_prev[c] = 0;
        taken[0].push_back(false);
    }
    for (auto c = weights[0]; c <= C; c++) { // O(C)
        dp_prev[c] = values[0];
        taken[0].push_back(true);
    }

    for (auto i = 1; i < n; i++) // O(N)
    {
        /* only two arrays suffice, as in 0-1 knapsack we only care about the predecessor's
        value given a maximum bag capacity.
        */
        dp_curr[0] = 0; 
        for (auto c = 1; c <= C; c++) // O(C)
        {
            if (c - weights[i] < 0)      // remaining capacity would not be enough to hold object i.
            {
                dp_curr[c] = dp_prev[c]; // so we cannot add it to the knapsack
                taken[i].push_back(false);
            }
            else // there is enough place to take object i.
            {
                // thus check if it is worth it to take it, or if we rather should ignore it
                if (dp_prev[c] >= dp_prev[c - weights[i]] + values[i]) {
                    taken[i].push_back( false);
                    dp_curr[c] = dp_prev[c];
                } else {
                    taken[i].push_back(true);
                    dp_curr[c] = dp_prev[c - weights[i]] + values[i];
                }
            }
        }
        dp_prev.swap(dp_curr);
    }

    // final result is within dp_prev at capacity C
    // auto max_value = dp_prev[C];
    vector<int> solution{};
    float c = C;
    for (int obj = n-1; obj >= 0 and c >= 1; obj--) { // O(n)
        if (taken[obj][c-1]) {
            solution.push_back(obj);
            c -= weights[obj];
        }
    }
    return solution;
}

int main()
{
    string input;
    while (getline(cin, input))
    {
        stringstream inputss{input};
        int C, n;
        inputss >> C >> n;

        vector<int> weights(n);
        vector<int> values(n);
        for (auto i = 0; i < n; i++)
        {
            int w, v;
            cin >> v >> w;
            weights[i] = w;
            values[i] = v;
        }

        auto indices = knapsack(C, n, weights, values);
        cout << indices.size() << endl;
        for (int p = 0; p < (int) indices.size() - 1; p++)
        {
            cout << indices[p] << " ";
        }
        if (not indices.empty())
            cout << indices.back() << std::endl;
        else
            cout << endl;
        getline(cin, input); // eat remaining \n
    }

    return 0;
}