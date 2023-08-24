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
    cout << n << " " << f << " " << graph.maxFlowSolution().size() << endl;
    for (auto const &edg : graph.maxFlowSolution())
        cout << edg.from << " " << edg.to << " " << edg.flow << endl;

    return 0;
}
