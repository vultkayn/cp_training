/**
 * @author benpr438
 * @brief Compute in constant time the intersection of two line segments.
 * If the segments intersects at point only, then this unique intersection point should be given.
 * If they intersect over many points, i.e. over a subsegment of their union, then this line subsegment should be given
 * (as a line segment itself).
 * If there is no intersion of the segment, then it shall be reported.
 * @date 2023-06-03
 *
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
using namespace std;
using vvi = vector<vector<long>>;

#include "../structs.hpp"

int main()
{
  int n;
  cin >> n;
  cout.precision(2);
  for (int i = 0; i < n; i++)
  {
    double x1, y1, x2, y2;
    cin >> x1 >> y1 >> x2 >> y2;
    Line seg1{{x1, y1}, {x2, y2}};

    cin >> x1 >> y1 >> x2 >> y2;
    Line seg2{{x1, y1}, {x2, y2}};

    auto interraw = Line::intersectionPoint(seg1, seg2);

    if (not interraw)
      cout << "none" << endl;
    else
    {
      auto inter{interraw.value()};
      inter.p1.x = round(inter.p1.x * 100) == 0 ? 0 : inter.p1.x;
      inter.p1.y = round(inter.p1.y * 100) == 0 ? 0 : inter.p1.y;
      inter.p2.x = round(inter.p2.x * 100) == 0 ? 0 : inter.p2.x;
      inter.p2.y = round(inter.p2.y * 100) == 0 ? 0 : inter.p2.y;

      cout << fixed << inter << endl;
    }
  }

  return 0;
}
