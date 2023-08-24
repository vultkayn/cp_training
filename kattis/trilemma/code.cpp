#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using vvi = vector<vector<long>>;

#include "structs.hpp"

// benpr438 liu

int main(){
  int N;
  cin >> N;
  for (int i = 1; i <= N; i++)
  {
    cout << "Case #" << i << ": ";
    Point p1;
    Point p2;
    Point p3;
    bool isoscele = false;
    cin >> p1 >> p2 >> p3;
    if (signedAreaTriangle (p1, p2, p3) == 0)
    {
      cout << "not a triangle" << endl;
      continue;
    }
    
    double d1 = Line(p1 , p2).norm();
    double d2 = Line(p2 , p3).norm();
    double d3 = Line(p1 , p3).norm();

    if (d1 == d2 or d2 == d3 or d1 == d3)
      cout << "isosceles ";
    else
      cout << "scalene ";

    vector<double> norm2s {
      Line(p1, p2).norm2(),
      Line(p2 , p3).norm2(),
      Line(p1 , p3).norm2()
    };
    sort(norm2s.begin(), norm2s.end());

    if (norm2s[0] + norm2s[1] < norm2s[2])
      cout << "obtuse triangle";
    else if (norm2s[0] + norm2s[1] > norm2s[2])
      cout << "acute triangle";
    else
      cout << "right triangle";

      
    cout << endl;
  }
  return 0;
}
