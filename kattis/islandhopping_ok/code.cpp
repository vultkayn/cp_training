/**
 * @author benpr438
 * @brief Solve the Minimum Spanning Tree problem.
 * That is, given a graph (V,E), find a subset S of E so that
 * (V,S) is a spanning tree (there is no cycle and all V are connected, i.e. from any v, there exists a path to any other u)
 * and the sum of the weights of the edges in S is minimal.
 * @date 2023-06-02
 *
 */

#include <iostream>
#include <queue>
#include <vector>
#include "disjointSet.hpp"
#include <algorithm>
#include <cmath>


using namespace std;

/**
 * @brief Represent an edge from u to v, with a weight of w.
 * Natural ordering is done by comparing the weights.
 *
 */
struct bridge
{
    long u, v;
    double w;
    bridge(int u, int v, double w) : u{u}, v{v}, w{w} {}
    bool operator<(bridge e) const { return w < e.w; }
    bool operator>(bridge e) const { return w > e.w; }
};

using tree_t = vector<bridge>;
using sol_t = pair<double, tree_t>;

/**
 * @brief Solve the MST problem using Kruskal algorithm, and a union-find structure.
 * Do not modify the input graph.
 * Time Complexity: O(E log E)
 * Additional memory: O(E)
 * @param edges
 * @param n
 * @return sol_t
 */
double find_mst_bridges_len(tree_t &edges, long n)
{
    sort(edges.begin(), edges.end()); // sort by non-decreasing weight, O(E log E)
    auto set = disjointSet(0, n);     // O(V) time and space.

    double length = 0;
    long long maxsize = 0;

    if (n == 1)
        return 0;

    for (auto const &edge : edges)
    { // O(E)
        auto [u, v, w]{edge};
        auto su{set.find(u)}; // almost O(1) (if disjointset is well flattened)
        auto sv{set.find(v)};
        if (su != sv)
        {
            set.doUnion(su, sv); // almost O(1) (if disjointset is well flattened)
            maxsize = max(maxsize, set.size(su));
            length += w;
        }
        if (maxsize == n)
            return length;
    }
    return 0;
}

double dist(double x1, double y1, double x2, double y2)
{
    return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

tree_t build_bridges(vector<pair<double, double>> const &endpoints)
{
    const auto m{endpoints.size()};
    tree_t graph;
    for (size_t i = 0; i < m; i++)
        for (size_t j = i + 1; j < m; j++)
            graph.emplace_back(i, j, dist(endpoints[i].first, endpoints[i].second, endpoints[j].first, endpoints[j].second));
    return graph;
}

int main()
{
    size_t n;
    cin >> n;

    while (n--)
    {
        size_t m;
        cin >> m;
        vector<pair<double, double>> endpoints;
        for (int i = 0; i < m; i++)
        {
            double x, y;
            cin >> x >> y;
            endpoints.emplace_back(x, y);
        }

        auto bridges {build_bridges(endpoints)};
        double sol = find_mst_bridges_len(bridges, m);
        cout << sol << endl;
    }

    return 0;
}
