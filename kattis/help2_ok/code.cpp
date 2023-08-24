#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

void read_string(vector<string> &res)
{
  string s1;
  getline(cin, s1);
  istringstream ss1{s1};
  while (ss1 >> s1)
    res.push_back(s1);
}

string collapse(vector<string> const &p)
{
  string res;
  for (int i = 0; i < p.size() - 1; i++)
    res += p[i] + ' ';
  return res + p.back();
}

string solve(vector<string> &p1, vector<string> &p2)
{
  if (p1.size() != p2.size())
    return "-";
  // deal with placeholder one by one

  bool placeholder = true;
  while (placeholder) // O(n). Total is O(16n^2)
  {
    placeholder = false;
    // deal with the case "one placeholder on a side, none on the other"
    // as dealing with the case "one placeholder each side" first would lead to false negative.
    for (int i = 0; i < p1.size(); i++) // O(n)
    {
      if (p1[i][0] == '<' and p2[i][0] != '<')
      {
        auto s = p1[i];
        placeholder = true;
        replace(p1.begin(), p1.end(), s, p2[i]); // O(16)
        break;
      }
      else if (p1[i][0] != '<' and p2[i][0] == '<')
      {
        auto s = p2[i];
        replace(p2.begin(), p2.end(), s, p1[i]); // O(16)
        placeholder = true;
        break;
      }
    }
    if (placeholder == false)
      for (int i = 0; i < p1.size(); i++) // O(n). Can actually return to first case, e.g with input "<a> <a>" and "<b> <x>"
      {
        if (p1[i][0] == '<' and p2[i][0] == '<')
        {
          placeholder = true;
          auto s = p1[i];
          replace(p1.begin(), p1.end(), s, string("x")); // O(16)
          s = p2[i];
          replace(p2.begin(), p2.end(), s, string("x")); // O(16)
          break;
        }
      }
  }

  string res;
  auto s1{collapse(p1)};
  auto s2{collapse(p2)};
  // cout << "strings collasped: \n" << s1 << "\n" << s2 << "\nEND<<<" << endl;
  return (s1 != s2) ? "-" : s1;
}

int main()
{
  int n;
  string c;
  cin >> n;
  getline(cin, c);
  for (int i = 0; i < n; i++)
  {
    vector<string> p1;
    vector<string> p2;
    read_string(p1);
    read_string(p2);

    string res{solve(p1, p2)};
    cout << res << endl;
  }

  return 0;
}