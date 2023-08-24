#include <iostream>
#include <vector>
#include <string>
using namespace std;
using vvi = vector<vector<long>>;

// benpr438 liu

constexpr int maxH = 1001;

enum move_t : int
{
  none = 0,
  down = 1,
  up = 2
};


void solve (vector<int> const& distances)
{
  // highestPoint[building][height]
  constexpr int dummyH = 10000;
  vector<vector<int>> highestPoint (distances.size(), vector<int>(maxH, dummyH));
  vector<vector<int>> moves (distances.size(), vector<int>(maxH, move_t::none));

  highestPoint[0][distances[0]] = distances[0];
  moves[0][distances[0]] = up;

  // double loop is O(40 * 1000) at most
  for (int b = 1; b < distances.size(); b++)
    for (int h = 0; h < maxH; h++)
    {
      highestPoint[b][h] = h;
      // up
      if (auto prevH = h - distances[b]; prevH >= 0)
      {
        highestPoint[b][h] = max(h, highestPoint[b-1][prevH]);
        moves[b][h] = up;
      }
      // down
      else if (auto prevH = h + distances[b]; prevH < maxH)
      {
        // up wasnt possible
        highestPoint[b][h] = highestPoint[b-1][prevH];
        moves[b][h] = down;
      }

      // both up and down were possible
      auto upH = h - distances[b];
      auto downH = h + distances[b];
      if (upH >= 0 and downH < maxH)
      {
        // take best move (that is that gives minimal highestPoint)
        if (highestPoint[b-1][upH] < highestPoint[b-1][downH])
        {
          moves[b][h] = up;
          highestPoint[b][h] = max(h, highestPoint[b-1][upH]);
        }
        else
        {
          moves[b][h] = down;
          highestPoint[b][h] = highestPoint[b-1][downH];
        }
      }
    }

  if (highestPoint[distances.size()-1][0] >= dummyH)
  {
    cout << "IMPOSSIBLE" << endl;
    return;
  }

  string moveStr (distances.size(), 'a');
  for (int i = distances.size()-1, h = 0; i >= 0; i--)
  {
    switch (moves[i][h])
    {
      case up:
        moveStr [i] = 'U';
        h -= distances[i];
        break;
      case down:
        moveStr [i] = 'D';
        h += distances[i];
        break;
      default:
        cout << "IMPOSSIBLE" << endl;
        return;
    }
  }
  cout << moveStr << endl;
}


int main(){
  int N;
  cin >> N;
  while (N--)
  {
    int M;
    cin >> M;
    vector<int> distances(M);
    for (int i =0; i < M; i++)
      cin >> distances[i];
    
    solve (distances);
  }
  return 0;
}
