#pragma once
#include <vector>
#include <type_traits>
#include <stdexcept>


using vertex_t = std::size_t;

class FlowGraph;
class FlowEdge;
struct Edge;

struct Edge
{
  vertex_t from;
  vertex_t to;
  long long w;
};

struct capacity_t
{
  long long c = 0;
  bool residual = true;
};


template <typename EdgeType, class = std::enable_if_t<std::is_base_of_v<Edge, std::decay_t<EdgeType>>>>
using graph_t = std::vector<std::vector<EdgeType>>;

struct FlowEdge : public Edge
{
  FlowEdge (vertex_t from, vertex_t to, long long c)
  : Edge {from, to, c} {}
  long long flow = 0;
  inline void pushFlow (long long f)
  {
    flow += f;
  }
};
class FlowGraph
{


public:
  FlowGraph(std::size_t nVertices, vertex_t s, vertex_t t);

  /**
   * @brief First call to this method will solve the maximum flow problem
   * using the Ford-Fulkerson's variant called Edmonds-Karp. (BFS instead of DFS)
   * Time complexity is thus O(V.E^2)
   *
   * Every calls return the maximum flow.
   */
  long long solve();
  void addEdge(Edge edge);
    /**
   * @brief Call only valid after the graph has been solved (through a call to solve)
   * Return a vector of all edges contributing to the maximum flow in O(E^2)
   */
  std::vector<FlowEdge> const &maxFlowSolution();
  /**
   * @brief Call only valid after the graph has been solved (through a call to solve)
   * Returns a vector of the nodes that are part of the minimum cut of the graph.
   * Time Complexity is O(VE)
   */
  std::vector<std::size_t> minCutSolution() const;

private:
  std::vector<std::vector<FlowEdge>> m_graph;
  std::vector<std::vector<capacity_t>> m_capacity;
  std::vector<FlowEdge> m_solvedFlow;
  long long m_maxFlow = 0;
  vertex_t m_s;
  vertex_t m_t;
  bool solved = false;
};
