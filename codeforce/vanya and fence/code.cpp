#include <iostream>
#include <vector>
using namespace std;
using vvi = vector<vector<long>>;

int main(){
  int n, h;
  cin >> n >> h;
  int height;
  int minwidth = 0;
  while (cin >> height)
    minwidth += height > h ? 2 : 1;

  cout << minwidth << endl;
  return 0;
}
