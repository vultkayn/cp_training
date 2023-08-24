#include "flowGraph.hpp"
#include <deque>
#include <algorithm>
#include <iostream>

FlowGraph::FlowGraph(std::size_t nVertices, vertex_t s, vertex_t t)
    : m_graph(nVertices), m_capacity(nVertices), m_solvedFlow{}, m_s{s}, m_t{t}
{
  for (auto i = 0; i < nVertices; i++)
  {
    m_capacity[i] = std::vector<capacity_t>(nVertices);
  }
}

static long long do_bfs (graph_t<FlowEdge> const& graph,
  std::vector<size_t>& predecessors,
  std::vector<std::vector<capacity_t>> const& capacity,
  size_t t,
  size_t s)
{
  std::fill(predecessors.begin(), predecessors.end(), -1);
  std::deque<std::size_t> bfsQueue {};
  bfsQueue.emplace_back (s);
  while (! bfsQueue.empty ())
  {
    auto from = bfsQueue.front ();
    bfsQueue.pop_front ();

    for (auto edge: graph[from])
    {
      auto to = edge.to;
      if (predecessors[to] == -1 and capacity[from][to].c)
      {
        predecessors[to] = from;
        bfsQueue.emplace_back (to);
        if (to == t)
          return true;
      }
    }
  }
  return false; // never reached m_t
}

long long FlowGraph::solve ()
{
  if (solved)
    return m_maxFlow;
  m_maxFlow = 0;
  m_solvedFlow.clear();

  if (m_graph.size() == 0)
  {
    solved = true;
    m_maxFlow = 0;
    return m_maxFlow;
  }

  std::vector<size_t> predecessors (m_graph.size(), -1);
  while (do_bfs (m_graph, predecessors, m_capacity, m_t, m_s))
  {
    long long Mflow = 8'000'000;
    auto to = m_t;
    while (to != m_s) {
      auto from = predecessors[to];
      Mflow = std::min (Mflow, m_capacity[from][to].c);
      to = from;
    }
    to = m_t;
    while (to != m_s) {
      auto from = predecessors[to];
      m_capacity[from][to].c -= Mflow;
      m_capacity[to][from].c += Mflow;
      to = from;
    }
    m_maxFlow += Mflow;
  }
  solved = true;
  return m_maxFlow;
}

void FlowGraph::addEdge(Edge edge)
{
  auto f = edge.from;
  auto t = edge.to;
  m_graph[f].emplace_back(f, t, edge.w);
  m_graph[t].emplace_back(t, f, 0);
  m_capacity[f][t].c = edge.w; // original
  m_capacity[f][t].residual = false;

  solved = false;
}

std::vector<FlowEdge> const &FlowGraph::maxFlowSolution()
{
  if (solved == true and not m_solvedFlow.empty())
    return m_solvedFlow;
  m_solvedFlow.clear();

  // O(E²)
  for (int i = 0; i < m_graph.size(); i++)
  {
    auto &adj_list{m_graph[i]};
    for (auto &edge : adj_list)
    {
      if (m_capacity[edge.from][edge.to].residual) continue; 
      if (auto f = edge.w - m_capacity[edge.from][edge.to].c; f > 0)
      {
        edge.flow = f;
        m_solvedFlow.push_back(edge);
      }
    }
  }
  return m_solvedFlow;
}


std::vector<std::size_t> FlowGraph::minCutSolution () const
{
  if (solved == false) return {};

  std::vector<std::size_t> cut {};
  std::vector<std::size_t> dfsStack {m_s};
  std::vector<bool> visited (m_graph.size (), false);
  visited[m_s] = true;
  while (! dfsStack.empty ())
  {
    auto curr = dfsStack.back ();
    dfsStack.pop_back ();
    cut.push_back (curr);
    auto &adj_list{m_graph[curr]};
    for (auto &edge : adj_list)
    {
      if (auto c = m_capacity[edge.from][edge.to].c; c > 0 and !visited[edge.to])
      {
        dfsStack.push_back(edge.to);
        visited[edge.to] = true;  // so never get more than once into the stack
      }
    }
  }
  
  return cut;
}
