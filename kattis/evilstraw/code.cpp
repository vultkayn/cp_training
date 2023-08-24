// benpr438 liu

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <iterator>
#include <algorithm>
using namespace std;
using vvi = vector<vector<long>>;

int ctoi (char c) { return c - 'a';}

bool might_be_palindrome (string const& s)
{
  int frequencies[26] = {0};
  for (char c: s)
    frequencies[ctoi(c)]++;
  int odd = 0;
  for (auto f: frequencies)
    odd += f & 1;
  return odd == 0 or ((s.size () & 1) and odd == 1);
}

/* zzyyxxwwvvuuttssrrqqppoonnmmllkkjjiihhggffeeddccbbaa impossibly long time to solve.
Though, printing the statistics of impl_permutations showed that the input kkjjiihhg 
appeared 1477035 times. So there was 1477034 redundant calls. Caching ?

Result with new unordered_map -> Instantaneous response time on the above test case. */

int impl_permutations (string s, unordered_map<string, int> &appeared)
{
  int beg = 0;
  int len = s.size();
  int mid = len / 2;
  if (s.size() <= 1) return 0;
  // skip already permutated outer substring
  for (; beg < mid and s[beg] == s[len - beg - 1]; beg++);
  if (beg == mid) return 0;

  len -= 2*beg;
  // s.substr() should have cost 100 at most, but doing it now should reduce the cost of the next ones
  // if there were some outer part of the string already done.
  // and makes it easier too with the indices 
  s = s.substr(beg, len);
  if (appeared.find(s) != appeared.end())
    return appeared[s];
  int right = len - 1;
  int minRight = 1000000, minLeft = 1000000;
  for (; s[0] != s[right] and right > 0; right--);
  if (right == 0)
    /* pair not found, s[0] will have to move to the middle. */
    minRight = (len / 2) +  impl_permutations (s.substr(1, len-1), appeared);
  else
    /* Needs to move paired character to "end - 1"*/
    minRight = (len - 1 - right) + impl_permutations (s.substr(1, right - 1) + s.substr(right+1, len - (right + 1)), appeared); 

  int left = 0;
  for (; s[left] != s[len - 1] and left < len; left++);
  if (left == len)
    /* pair not found, s[len - 1] will have to move to the middle. */
    minLeft = (len / 2) + impl_permutations (s.substr(0, len-1), appeared);
  else
    /* Needs to move paired character to "0"*/
    minLeft = left + impl_permutations (s.substr(0, left) + s.substr(left+1, len - (left + 1) - 1), appeared);
  appeared[s] = min(minLeft, minRight);
  return appeared[s];
}

int permutations (string const& s)
{
  std::unordered_map <string, int> appeared;
  return impl_permutations (s, appeared);
}

int main(){
  int n;
  cin >> n;
  while (n--)
  {
    string s;
    cin >> s;
    if (not might_be_palindrome (s))
    {
      cout << "Impossible" << endl;
      continue;
    }
    cout << permutations (s) << endl; 
  }

  return 0;
}
