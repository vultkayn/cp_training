/**
 * @author benpr438 
 * @brief Greedy search of the optimal interval cover of the given interval [A, B],
 * that is finding a subset of 'intervals' of minimal size, whose the union of
 * its elements covers [A,B].
 * If no such cover could be found, returns an empty vector.
 * 
 * This version do not expect the input vector to be sorted, nor will it modify it.
 * Instead, a copy is made.
 * 
 * Time complexity: O(n log n), with n = intervals.size()
 * 
 * @param intervals A non-ordered
 * @param A Lower bound of the interval to cover
 * @param B Upper bound of the interval to cover
 * @return vector<int> A vector of minimal length containing all indices of 'intervals' used to build the interval cover. 
 */
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;
using vvi = vector<vector<long>>;


/**
 * @brief Interval [s, f]
 * Natural ordering by comparing s.
 * 
 */
struct interval
{
    double s;
    double f;

    bool operator<(interval oth) const
    {
        return s < oth.s;
    }
};

/**
 * @brief An interval as part of an ordered Container.
 * Contains an interval 'inter', and its initial index 'idx'
 * within the container.
 * 
 * Intervals natural ordering is preserved
 */
struct input
{
    interval inter;
    int idx;
    bool operator<(input oth) const
    {
        return inter < oth.inter;
    }

    input(double s, double f, int idx) : inter{s, f}, idx{idx} {}
};

// n=intervals.size() -> complexity 
/**
 * @brief Greedy search of the optimal interval cover of the given interval [A, B],
 * that is finding a subset of 'intervals' of minimal size, whose the union of
 * its elements covers [A,B].
 * If no such cover could be found, returns an empty vector.
 * 
 * This version do not expect the input vector to be sorted, nor will it modify it.
 * Instead, a copy is made.
 * 
 * Time complexity: O(n log n), with n = intervals.size()
 * 
 * @param intervals A non-ordered
 * @param A Lower bound of the interval to cover
 * @param B Upper bound of the interval to cover
 * @return vector<int> A vector of minimal length containing all indices of 'intervals' used to build the interval cover. 
 */
vector<int> solveIntervalCover(vector<input> intervals, double A, double B)
{
    sort(intervals.begin(), intervals.end()); // sort by lower bound of the interval O(n log n)

    vector<int> indices{};
    double t = A;
    double maxF = t - 1;
    bool last = false;
    int opt = -1;
    for (int i = 0; i < intervals.size();) // O(n) since the inner while loop increases i
    {
        while (i < intervals.size() and intervals[i].inter.s <= t)
        {
            if (maxF < intervals[i].inter.f)
            {
                maxF = intervals[i].inter.f;
                opt = i;
            }
            i++;
        } // equivalent to the t= max(f, s <= t <= f) of the video
        t = maxF;
        if (opt == -1)
            return {};
        indices.push_back(intervals[opt].idx);
        if (not(i < intervals.size() and intervals[i].inter.s <= maxF and t < B))
            return (maxF >= B ? indices : vector<int>{});
    }

    return (maxF >= B ? indices : vector<int>{});
}

int main()
{

    string s;
    while (getline(cin, s))
    {
        double A, B;
        stringstream ss{s};
        ss >> A >> B;
        int n;
        cin >> n;

        vector<input> intervals;
        for (int i = 0; i < n; i++)
        {
            double s, f;
            cin >> s >> f;
            intervals.emplace_back(s, f, i);
        }

        auto optNbIntervals = solveIntervalCover(intervals, A, B);
        if (optNbIntervals.empty())
            cout << "impossible" << endl;
        else
        {
            cout << optNbIntervals.size() << endl;
            for (int i = 0; i < optNbIntervals.size() - 1; i++)
                cout << optNbIntervals[i] << " ";

            cout << optNbIntervals.back() << endl;
        }

        getline(cin, s); // clear dangling \n
    }

    return 0;
}
