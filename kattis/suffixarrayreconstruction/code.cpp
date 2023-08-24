#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;
using vvi = vector<vector<long>>;

// benpr438 liu

void drain_cin (int s)
{
  while (s--)
  {
    int p;
    string suffix;
    cin >> p >> suffix;
  }
}

bool solve(string &recovery, int l, int s)
{
  int counter = l;
  while (s--)
  {
    int p;
    string suffix;
    cin >> p >> suffix;
    p--; /* data is given 1-base indexed.  */
    bool star = false;
    /* check suffix left-to-right until star if any.  */
    for (int i = 0; i < l-p and i < suffix.size(); i++)
    {
      if ((star = (suffix[i] == '*')))
        break;
      if (recovery[p+i] != '#' and recovery[p+i] != suffix[i])
      {
        /* Different match was found previously.  */
        drain_cin (s);
        return false;
      }
      if (recovery[p+i] == '#')
        counter--;
      recovery[p+i] = suffix[i];
    }

    if (star)
    {
      /* If star, then there might be arbitrarily long missing patch of characters,
      so only check right to it, as we know sure it matches the end of recovery (as it is a suffix).  */
      int j = recovery.size() - 1;
      for (int i = suffix.size() - 1; i >= 0; i--, j--)
      {
        /* check suffix right-to-left until star if any.  */
        if (suffix[i] == '*')
          break;
        if (recovery[j] != '#' and recovery[j] != suffix[i])
        {
          /* Different match was found previously.  */
          drain_cin (s);
          return false;
        }
        if (recovery[j] == '#')
          counter--;
        recovery[j] = suffix[i];
      }
    }
  }
  return counter <= 0;
}

int main()
{
  ios_base::sync_with_stdio(false); // disable flushing after each newline
  int t;
  cin >> t;

  while (t--)
  {
    int l, s;
    cin >> l >> s;
    string recovery(l, '#');      
    if (not solve(recovery, l, s))
      cout << "IMPOSSIBLE\n";
    else
      cout << recovery << '\n';
  }
  cout << flush;
  return 0;
}
