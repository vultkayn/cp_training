/**
 * @author benpr438
 * @brief All Pairs Shortest Path problem.
 * Given a graph (V, E), we want to find the shortest path from s to t,
 * for any (s,t) in V^2.
 * To do so, use the Floyd Warshall algorithm, using dynamic programming to find all these pairs
 * in O(V^3) time, using O(V^2) additional memory.
 * 
 * Solve the graph at once, then answer each query in constant time, simply by dereferencing the result array.
 * Floyd Warshall algorithm is robust enough that any negative cycle can be detected and reported.
 * 
 * @date 2023-06-02
 * 
 */
#include <iostream>
#include <vector>


using namespace std;
constexpr long inf = 10'000'000;

/**
 * @brief Floyd Warshall algorithm to solve All Pairs Shortest Path of the graph
 * given by the edge matrix mtx. Thus no vector in the graph can contain a multiple edge,
 * however negative weight edges are supported.
 *  
 * 
 * @param mtx 
 * @param nb_nodes 
 * @return vector<vector<long>> A vector of the shortest path from (s,t), given by v[s][t].
 * If there exists no path (s,t), has value inf, if there exists a path but through a negative cycle,
 * then contains -inf.
 */
vector<vector<long>> floyd_warshall(vector<vector<long>> mtx, long nb_nodes)
{
    auto& dist = mtx;
    for (auto i = 0; i < nb_nodes; ++i)
        dist[i][i] = 0;

    for (long k = 0; k < nb_nodes; k++) // FIXME order matters !!!
    {
        for (long u = 0; u < nb_nodes; u++)
        {
            for (long v = 0; v < nb_nodes; v++)
            {
                auto uv_c = dist[u][v];
                auto uk_c = dist[u][k];
                auto kv_c = dist[k][v];
                if (uk_c < inf and kv_c < inf) //  there exists a path
                    dist[u][v] = max(-inf, min(uv_c, uk_c + kv_c));
            }
        }
    }

    // detect negative circles (if there exist an intermediate node for which min value to itself is < 0)
    for (long u = 0; u < nb_nodes; u++)
    {
        for (long v = 0; v < nb_nodes; v++)
        {
            for (long k = 0; k < nb_nodes and dist[u][v] != -inf; k++)
            {
                if (dist[k][k] < 0 and dist[u][k] != inf and dist[k][v] != inf){
                    dist[u][v] = - inf;
                }
            }
        }
    }
    return dist;
}

int main(){
    size_t n, m, q;
    cin >> n >> m >> q;
    while (n  and q) {
        vector<vector<long>> adj_mtx(n);
        for (long i = 0; i < n; i++)
        {
            for (long j = 0; j < n; j++)
                adj_mtx[i].push_back(inf);
        }

        for (long i = 0; i < m; i++)
        {
            long u,v,c;
            cin >> u >> v >> c;
            adj_mtx[u][v] = min(adj_mtx[u][v], c); // TODO one node can be specified twice -_- (multigraph)
        }

        auto distances = floyd_warshall(adj_mtx, n);
        
        for (auto i = 0; i < q; i++)
        {
            size_t s, t;
            cin >> s >> t;
            auto const& d = distances[s][t];
            if (d == inf)
                cout << "Impossible" << endl;
            else if (d == - inf)
                cout << "-Infinity" << endl;
            else
                cout << d << endl;
        }

        cin >> n >> m >> q;
    }
    return 0;
}
