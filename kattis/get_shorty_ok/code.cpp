#include <iostream>
#include <queue>
#include <cmath>
#include <vector>
#include <utility>

using namespace std;

struct edge_t
{
  double factor;
  size_t to;
  edge_t (double f, size_t t) : factor{f}, to{t} {}
};

inline bool operator<(edge_t lhs, edge_t rhs)
{
  return lhs.factor < rhs.factor;
}

using adjlist_t = vector<vector<edge_t>>;


/* def dijkstra(adj_list, nb_nodes, start):
    costs = [0] * nb_nodes
    costs[start] = 1.0
    prioQ = [[costs[start], start]]
    visited = [False] * nb_nodes
    while prioQ:
        cost, node = heappop(prioQ) # O(1) popping head element of PQ
        visited[node] = True
        # we explore all outgoing edges of the current node = O(E) complexity in total for the two loops.
        for factor, neighbor in adj_list[node]:
            if not visited[neighbor] and (new_cost := factor * cost) > costs[neighbor]:
                heappush(prioQ, [new_cost, neighbor]) # insertion in priority queue = O(log E) complexity
                costs[neighbor] = new_cost
        # since E = V^2 at most in an oriented graph without multiple edges, log(E) <= 2log(V)
        # hence we get the known complexity of O(E log V)

    return costs */


vector<double> dijkstra (adjlist_t adj_list, size_t nb_nodes, size_t start)
{
    vector<double> costs (nb_nodes, 0);
    costs[start] = 1.0;
    priority_queue<edge_t> prioQ;
    prioQ.emplace(costs[start], start);
    vector<bool> visited (nb_nodes, false);
    while (! prioQ.empty() )
    {
      auto [cost, node] = prioQ.top();
      prioQ.pop();
      visited[node] = true;
      for (auto e : adj_list[node])
      {
        if (double new_cost {e.factor * cost}; not visited[e.to] and new_cost >= costs[e.to])
        {
          prioQ.emplace(new_cost, e.to);
          costs[e.to] = new_cost;
        }
      }
    }
    return costs;
}

int main ()
{
  size_t n, m;
  cin >> n >> m;
  cout.precision(4);
  while (n != 0 or m != 0)
  {
      adjlist_t adjacency_list (n, vector<edge_t>());
      for (auto i = 0; i < m; i++)
      {
        size_t x, y;
        double f;
        cin >> x >> y >> f;
        adjacency_list[x].emplace_back(f, y);
        adjacency_list[y].emplace_back(f, x);
      }

      auto costs = dijkstra(adjacency_list, n, 0);
      double rounded = round (costs[n-1] * 10000.f) / 10000.f;
      cout << fixed << rounded << endl;
      cin >> n >> m;
  }

}