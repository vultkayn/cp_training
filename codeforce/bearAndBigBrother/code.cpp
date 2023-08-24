#include <iostream>
#include <vector>
using namespace std;
using vvi = vector<vector<long>>;

int main(){
  int a, b;
  cin >> a >> b;
  int i = 0;
  while (a <= b)
  {
    a *= 3;
    b <<= 1;
    i ++;
  }
  cout << i << endl;

  return 0;
}
