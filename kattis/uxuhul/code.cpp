#include <iostream>
#include <vector>
#include <array>
using namespace std;
using vvi = vector<vector<long>>;

// benpr438 liu

enum pebble_state : int
{
  NNN = 0,
  NNY,
  NYN,
  NYY,
  YNN,
  YNY,
  YYN,
  YYY,
  LAST
};

ostream& operator<< (ostream &out, pebble_state s)
{
  switch (s)
  {
    case NNN:
      return out << "NNN";
    case NNY:
      return out << "NNY";
    case NYN:
      return out << "NYN";
    case NYY:
      return out << "NYY";
    case YNN:
      return out << "YNN";
    case YNY:
      return out << "YNY";
    case YYN:
      return out << "YYN";
    case YYY:
      return out << "YYY";
    default:
      return out; 
  }
}

pebble_state vote1 (int from)
{
  return static_cast <pebble_state>(from ^ 1);
}
pebble_state vote2 (int from)
{
  return static_cast <pebble_state>(from ^ 2);
}
pebble_state vote3 (int from)
{
  return static_cast <pebble_state>(from ^ 4);
}


int main(){
  int n, m;
  cin >> n;
  while (n--)
  {
    cin >> m;
    vector<array<int, LAST>> preferences (m);
    vector<array<pebble_state, LAST>> votes (m);

    for (int i = 0; i < m; i++)
      for (int j = 0; j < LAST; j++)
        cin >> preferences[i][j];

    for (int i = 0; i < LAST; i++)
    {
      auto v1 = preferences.back()[vote1 (i)];
      auto v2 = preferences.back()[vote2 (i)];
      auto v3 = preferences.back()[vote3 (i)];
      if (v1 <= v2 and v1 <= v3)
        votes.back()[i] = vote1 (i);
      else if (v2 <= v1 and v2 <= v3)
        votes.back()[i] = vote2 (i);
      else votes.back()[i] = vote3 (i);
    }

    for (int priest = m-2; priest >= 0; priest--)
      for (int state = 0; state < LAST; state++)
      {
        auto v1 = preferences[priest][votes[priest+1][vote1 (state)]];
        auto v2 = preferences[priest][votes[priest+1][vote2 (state)]];
        auto v3 = preferences[priest][votes[priest+1][vote3 (state)]];
        if (v1 <= v2 and v1 <= v3)
          votes[priest][state] = votes[priest+1][vote1 (state)];
        else if (v2 <= v1 and v2 <= v3)
          votes[priest][state] = votes[priest+1][vote2 (state)];
        else votes[priest][state] = votes[priest+1][vote3 (state)];
      }
    
    cout << votes[0][static_cast<int>(NNN)] << endl;
  }
  return 0;
}
