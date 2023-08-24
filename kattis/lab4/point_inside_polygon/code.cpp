#include <iostream>
#include <vector>
using namespace std;
using vvi = vector<vector<long>>;

// dumped geometry_structs.cpp

struct Point;
double signedAreaTriangle (Point a, Point b, Point c);

struct Point {
  Point() : x{0}, y{0} {}
  Point(double x, double y) : x{x}, y{y} {}


  static int ccw (Point a, Point b, Point c) {
      auto areaTwice = signedAreaTriangle(a,b,c);
      if (areaTwice < 0) return -1;
      return (areaTwice > 0);
  }

  int ccw ( Point b, Point c) {
    return Point::ccw(*this, b, c);
  }

  static int dot(Point p1, Point p2) {
    return (p1.x * p2.x) + (p1.y * p2.y);
  }

  Point operator-(Point other) const { 
    return Point(x - other.x, y - other.y);
  }

  double x, y;
};

double signedAreaTriangle (Point a, Point b, Point c) {
  return ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x)) / 2;
}

struct Line {
  Line(Point p1, Point p2) : p1{p1}, p2 {p2} {}

  static bool intersect(Line l1, Line l2) {
    bool test1 = Point::ccw(l1.p1, l1.p2, l2.p1) * Point::ccw(l1.p1, l1.p2, l2.p2); // different sign expected
    bool test2 = Point::ccw(l2.p1, l2.p2, l1.p1) * Point::ccw(l2.p1, l2.p2, l1.p2); // different sign expected
    return (test1 <= 0) and (test2 <= 0);
  }

  bool intersect(Line l2) {
    return Line::intersect(*this, l2);
  }


  Point p1, p2;
};


class Polygon {
public:
  Polygon() {}
  // -1 outside, 1 inside, 0 on the edge
  int contains(Point p) {
    // intersect L = [p, y = p.y] with edges of polygon
    // if number of intersection is even then point is outside
    // else if number is odd then point is inside

    // careful with point on edge -> length of L until intersection
    // if under a certain threshold, consider point on edge

    // complexity O(#edges), could be improved by taking advantage
    // of order of the vertices (counterclockwise or not)
    // for convex polygons we can return as soon as nintersect > 1

    int nintersect = 0;
    // Line inf (p, {10001, p.y}); No longer needed was problem specific 
    // // (cuz maxX was 10000, we could draw 'infinite' horizontal line [p, (10001, p.y)])
    // in the end I merged my two attempts and managed to make it generic (float and integer, no matter grid size)

    // I moved the "onEdge" check out of below loop, I was at first doing everything at once
    // but it was buggy because I had to manage edge cases with conditions and I couldnt pass the test
    // I tried with 2 loops (so double cost), it still passed the lab execution!
    int n = points.size();
    for (int i = 0; i < n; i++) {
      Line edge (points[i], points[(i+1)%n]);
      // test colinearity to know is point is on edge.
      int notColinear = Point::ccw(p, edge.p1, edge.p2);
      bool onSegment = (not notColinear) and Point::dot(p - edge.p1, p - edge.p2) <= 0 ;// dot < 0 means we're not going same way from p -> p1 as with p -> p2
      if (onSegment) return 0;
    }
    
    // thanks to the above we know point is NOT on edge, so we can save conditions
    // edge.miny <= p.y and p.y <= edge.maxy now uses strict comparison
    // this gives us nice property when computing 1/slope that the divider is not zero.
    for (int i = 0; i < n; i++) {
      Line edge (points[i], points[(i+1)%n]); // (i-1)%n had weird behavior

      // hence p is in middle of segment
      // afterwards dont care about being onEdge, it has already been checked for
      if ((edge.p1.y < p.y != edge.p2.y < p.y /*guarantees not division by 0, and edge intersects y=p.y (maybe on left side though)*/)) {
        double xintersect = edge.p1.x + (p.y - edge.p1.y) * (edge.p1.x - edge.p2.x) / (edge.p1.y - edge.p2.y);// = divide by slope;
        // was using intersect method from the course at first, but in this case it proved unnecessary !
        if (p.x < xintersect /*check we will intersect by moving right*/) 
          nintersect++;
      }
    }

    return (nintersect & 1) ? 1 : -1;
  }

  vector<Point> points;
};
int main(){
    int n, m;
    cin >> n;
    while (n) {
        Polygon poly;
        for (int i =0; i < n; i++) {
            Point p;
            cin >> p.x >> p.y;
            poly.points.push_back(p);
        }

        cin >> m;
        for (int i=0; i < m; i++) {
            double qx, qy;
            cin >> qx >> qy;
            int tristate = poly.contains({qx, qy});
            cout << (tristate == -1 ? "out" : (tristate == 1 ? "in" : "on")) << endl;
        }

        cin >> n;
    }

    return 0;
}
