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

constexpr long INF = 10'000'000;

using namespace std;

/**
 * @brief Represent an edge from u to v, with a weight of w.
 * Natural ordering is done by comparing the weights.
 * 
 */
struct road {
    long u,v,w;
    road(int u, int v, int w) : u{u}, v{v}, w{w} {}
    bool operator<(road e) const {return w < e.w; }
    bool operator>(road e) const {return w > e.w; }
};

using tree_t = vector<road>;
using sol_t = pair<long, tree_t>;


/**
 * @brief Solve the MST problem using Kruskal algorithm, and a union-find structure.
 * Do not modify the input graph.
 * Time Complexity: O(E log E)
 * Additional memory: O(E)
 * @param edges 
 * @param n 
 * @return sol_t 
 */
sol_t mst_kruskal(vector<road> edges, long n) {
    sort(edges.begin(), edges.end()); // sort by non-decreasing weight, O(E log E)
    auto set = disjointSet(0, n); // O(V) time and space.


    sol_t sol;
    sol.first = 0;
    long long maxsize = 0;

    if (n == 1) return {0, {}};

    for (auto const& edge: edges) { // O(E)
        auto [u,v,w] {edge};
        auto su {set.find(u)}; // almost O(1) (if disjointset is well flattened)
        auto sv {set.find(v)};
        if (su != sv) {
            set.doUnion(su, sv); // almost O(1) (if disjointset is well flattened)
            maxsize = max(maxsize, set.size(su));
            sol.first += w;
            sol.second.emplace_back(u, v, w);
        }
        if (maxsize == n)
            return sol;
    }
    return {};
}


int main() {
    size_t n,m;
    cin >> n >> m;

    auto lexicoComp = [](road lhs, road rhs) {
        return lhs.u < rhs.u or (lhs.u == rhs.u and lhs.v < rhs.v);
    };

    while (n)
    {
        vector<road> edges;
        for (int i =0; i < m; i++) {
            long u, v, w;
            cin >> u >> v >> w;
            edges.emplace_back(min(u, v), max(u, v), w);
        }

        sol_t sol = mst_kruskal(edges, n);
        if (sol.second.empty() and n != 1)
            cout << "Impossible" << endl;
        else {
            cout << sol.first << endl;
            sort(sol.second.begin(), sol.second.end(), lexicoComp);
            for (auto const& edge: sol.second) {
                cout << edge.u << " " << edge.v << endl;
            }
        }

        cin >> n >> m;
    }

    return 0;
}
