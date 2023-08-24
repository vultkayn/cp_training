#include <iostream>
#include <vector>
using namespace std;

#include "flowGraph.hpp"

using vvi = vector<vector<long>>;

int main()
{
    size_t n, m, s, t;
    cin >> n >> m >> s >> t;

    FlowGraph graph{n, s, t};
    for (int i=0; i < m; i++)
    {
        unsigned long u, v;
        long long c; // c is capacity
        cin >> u >> v >> c;
        graph.addEdge({u, v, c});
    }

    auto f = graph.solve();
    auto cut = graph.minCutSolution ();
    cout << cut.size() << endl;
    for (auto vtx : cut)
        cout << vtx << endl;

    return 0;
}
