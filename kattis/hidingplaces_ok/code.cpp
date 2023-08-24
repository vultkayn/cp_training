#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
using vvi = vector<vector<long>>;

int inf = 100;

/*
Grid is :


y=7 | 56 57 58 59 60 61 62 63
...
y=1 | 08 09 10 11 12 13 14 15
y=0 | 00 01 02 03 04 05 06 07

x:    0  1  2  3  4  5  6  7
*/

int getx(int vertex)
{
  return vertex % 8;
}

int gety(int vertex)
{
  return vertex / 8;
}

int getvtx(int x, int y)
{
  return x + y * 8;
}

struct move_t
{
  int dx;
  int dy;

  // return -1 if cannot move
  inline int move_to(int from) const
  {
    auto x = getx(from) + dx;
    auto y = gety(from) + dy;
    if (x < 0 or x > 7)
      return -1;

    if (y < 0 or y > 7)
      return -1;

    return getvtx(x, y);
  }
};

struct cell_t
{
  int idx;
  int dist;
};

bool operator> (cell_t lhs, cell_t rhs)
{
  return lhs.dist > rhs.dist;
}

// In theory O (V^3) = O (64*64*64)
vector<vector<cell_t>>
all_knight_moves()
{
  size_t nb_nodes = 64;
  vector<vector<cell_t>> dist(nb_nodes, vector<cell_t>(nb_nodes, {-1, inf}));


  move_t moves[8] =
      {
          {-1, 2}, // up
          {1, 2},
          {-1, -2}, // down
          {1, -2},
          {2, -1}, // right
          {2, 1},
          {-2, -1}, // left
          {-2, 1},
      };

  // init distances
  for (auto vtx = 0; vtx < nb_nodes; vtx++)
  {
    for (move_t m: moves)
    {
      auto to = m.move_to (vtx);
      if (to >= 0)
      {
        dist[vtx][to].dist = 1;
        dist[to][vtx].dist = 1;
        dist[vtx][to].idx = to;
      }
      
    }
  }

  for (auto i = 0; i < nb_nodes; ++i)
    dist[i][i].dist = 0;

  for (long k = 0; k < nb_nodes; k++) // FIXME order matters !!!
  {
    for (long u = 0; u < nb_nodes; u++)
    {
      for (long v = 0; v < nb_nodes; v++)
      {
        auto uv_c = dist[u][v].dist;
        auto uk_c = dist[u][k].dist;
        auto kv_c = dist[k][v].dist;
        dist[u][v].idx = v;
        dist[u][k].idx = k;
        dist[k][v].idx = v;
        if (uk_c < inf and kv_c < inf) //  there exists a path
          dist[u][v].dist = max(-inf, min(uv_c, uk_c + kv_c));
      }
    }
  }
  return dist;
}

int main()
{
  int n;
  cin >> n;

  auto dists = all_knight_moves();
  
  for (auto i = 0; i < dists.size(); i++)
    replace_if(
        dists[i].begin(), dists[i].end(),
        [](cell_t x)
        { return x.dist == inf; },
        cell_t{-1, -inf});

  for (int i = 0; i < n; i++)
  {
    char named_x;
    int y;
    cin >> named_x >> y;
    int x = named_x - 'a';
    int start = getvtx(x, y-1);
    sort (dists[start].begin(), dists[start].end(), [](cell_t lhs, cell_t rhs){
      if (lhs.dist != rhs.dist) return lhs.dist > rhs.dist;
      if (gety (lhs.idx) != gety (rhs.idx)) return gety (lhs.idx) > gety (rhs.idx);
      return getx (lhs.idx) < getx (rhs.idx);
    });
    int maxdist = dists[start][0].dist;
    cout << maxdist;
    for (int i = 0, curr_d = maxdist; curr_d == maxdist and i < dists[start].size(); i++)
    {
      curr_d = dists[start][i].dist;
      if (curr_d == maxdist)
      {
        auto idx = dists[start][i].idx;
        char cx = static_cast<char>(getx (idx)) + 'a';
        cout << ' ' << cx << gety (idx) + 1;
      }
    }
    cout << endl;
  }

  return 0;
}
