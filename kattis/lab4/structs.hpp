#pragma once

#include <vector>
#include <ostream>
#include <optional>

struct Point;
struct intersection_t;
struct Line;
class Polygon;

double signedAreaTriangle(Point a, Point b, Point c);
Point min(Point a, Point b);
Point max(Point a, Point b);
Point operator-(Point lhs, Point other);
Point operator+(Point lhs, Point other);
Point operator*(Point lhs, double scalar);
bool operator<(Point lhs, Point other);
bool operator==(Point lhs, Point other);
std::ostream &operator<<(std::ostream &os, Point const &p);
std::ostream &operator<<(std::ostream &os, intersection_t const &inter);

struct Point
{

  Point() : x{0}, y{0} {}
  Point(double x, double y) : x{x}, y{y} {}

  // TRUE if a -> b -> c turns counter clock wise
  static int ccw(Point a, Point b, Point c)
  {
    auto areaTwice = signedAreaTriangle(a, b, c);
    if (areaTwice < 0)
      return -1;
    return (areaTwice > 0);
  }
  static double dot(Point p1, Point p2)
  {
    return (p1.x * p2.x) + (p1.y * p2.y);
  }

  static double cross(Point p1, Point p2)
  {
    return p1.x * p2.y - p1.y * p2.x;
  }

  int ccw(Point b, Point c)
  {
    return Point::ccw(*this, b, c);
  }

  double x, y;
};

struct intersection_t
{
  enum class Type
  {
    colinear,
    single
  };

  Type type;
  Point p1;
  Point p2;
};

struct Line
{
  struct coeff_t
  {
    double A;
    double B;
    double C;

    coeff_t(double A, double B, double C) : A{A}, B{B}, C{C} {}

    coeff_t(Line l)
        : A{l.p2.y - l.p1.y},
          B{l.p1.x - l.p2.x},
          C{A * l.p1.x + B * l.p1.y} {}
  };

  Line(Point p1, Point p2) : p1{p1}, p2{p2}
  {
    stdForm = coeff_t(*this);
  }

  static bool intersect(Line l1, Line l2);
  static bool colinear(Line l1, Line l2);
  bool contains(Point p) const;
  double distanceTo(Line oth) const;
  inline bool isPoint() const
  {
    return p1 == p2;
  }

  double norm2() const;
  double norm() const;
  bool intersect(Line l2) const;
  static std::optional<intersection_t> intersectionPoint(Line seg1, Line seg2);

  Point p1, p2;
  coeff_t stdForm{0, 0, 0};
};

class Polygon
{
private:
  void recomputeArea();

public:
  Polygon() {}
  // -1 outside P is inside the polygon, 1 inside, 0 on the edge
  int contains(Point p);

  // returns unsigned area of the polygon
  double area();

  inline void push(Point newP)
  {
    uptodate = false;
    points.push_back(newP);
  }

private:
  std::vector<Point> points;
  double m_area = 0;
  bool uptodate = false;
};


using scatter_t = std::vector<Point>;

std::pair<Point, Point> closest_pair(scatter_t &points);