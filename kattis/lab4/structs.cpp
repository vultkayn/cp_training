#include "structs.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

Point operator-(Point lhs, Point other)
{
  return Point(lhs.x - other.x, lhs.y - other.y);
}
Point operator+(Point lhs, Point other)
{
  return Point(lhs.x + other.x, lhs.y + other.y);
}
Point operator*(Point lhs, double scalar)
{
  return Point(lhs.x * scalar, lhs.y * scalar);
}
bool operator<(Point lhs, Point other)
{ // Integer only currently
  return (lhs.x < other.x) or (lhs.x == other.x and lhs.y < other.y);
}

bool operator==(Point lhs, Point rhs)
{
  return lhs.x == rhs.x and lhs.y == rhs.y;
}

std::ostream &operator<<(std::ostream &os, Point const &p)
{
  return os << p.x << " " << p.y;
}

std::ostream &operator<<(std::ostream &os, intersection_t const &inter)
{
  using type_t = intersection_t::Type;

  switch (inter.type)
  {
  case type_t::colinear:
    os << inter.p1 << " " << inter.p2;
    break;
  case type_t::single:
    os << inter.p1;
    break;
  default:
    throw std::runtime_error("impossible intersection_t type");
    break;
  }
  return os;
}

std::optional<intersection_t> Line::intersectionPoint(Line seg1, Line seg2)
{
  using type_t = intersection_t::Type;
  if (not Line::intersect(seg1, seg2))
    return {std::nullopt};

  if (Line::colinear(seg1, seg2))
  { // intersect AND colinear
    Point p1 = min(seg1.p1, seg1.p2);
    Point p2 = min(seg2.p1, seg2.p2);
    Point P1 = max(seg1.p1, seg1.p2);
    Point P2 = max(seg2.p1, seg2.p2);

    Point intersect1 = max(p1, p2);
    Point intersect2 = min(P1, P2);

    if (intersect2 < intersect1)
      std::swap(intersect1, intersect2);

    type_t type = intersect1 == intersect2 ? type_t::single : type_t::colinear;
    return std::make_optional<intersection_t>({type, intersect1, intersect2});
  }

  // put under standard form
  auto [a1, b1, c1]{seg1.stdForm};
  auto [a2, b2, c2]{seg2.stdForm};
  auto det{Point::cross({a1, b1}, {a2, b2})};
  Point intersect{Point::cross({c1, b1}, {c2, b2}) / det, Point::cross({a1, c1}, {a2, c2}) / det};
  return std::make_optional<intersection_t>({type_t::single, intersect});
}

double signedAreaTriangle(Point a, Point b, Point c)
{
  return ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x)) / 2;
}

Point min(Point a, Point b) { return a < b ? a : b; }
Point max(Point a, Point b) { return a < b ? b : a; }

bool Line::intersect(Line l1, Line l2)
{
  short ccw1_21 = Point::ccw(l1.p1, l1.p2, l2.p1);
  short ccw1_22 = Point::ccw(l1.p1, l1.p2, l2.p2);
  short test1 = ccw1_21 * ccw1_22;                                                 // different sign expected
  short test2 = Point::ccw(l2.p1, l2.p2, l1.p1) * Point::ccw(l2.p1, l2.p2, l1.p2); // different sign expected

  bool colinear = ccw1_21 == 0 and ccw1_22 == 0;
  if (colinear)
  {
    if (l1.isPoint() and l2.isPoint())
      return l1.p1 == l2.p1;
    if (l1.isPoint() or l2.isPoint())
      return (l1.isPoint() and l2.contains(l1.p1)) or (l2.isPoint() and l1.contains(l2.p1));

    short col = l1.contains(l2.p1) + l1.contains(l2.p2) + l2.contains(l1.p1) + l2.contains(l1.p2); // or (Point::dot(l2.p2 - l1.p1, l2.p2 - l1.p2) <= 0);
    return col >= 2;
  }
  return (test1 <= 0) and (test2 <= 0);
}

bool Line::colinear(Line l1, Line l2)
{
  short ccw1_21 = Point::ccw(l1.p1, l1.p2, l2.p1);
  short ccw1_22 = Point::ccw(l1.p1, l1.p2, l2.p2);
  return ccw1_21 == 0 and ccw1_22 == 0;
}

bool Line::contains(Point p) const
{
  using std::min, std::max;
  if (isPoint())
    return p == p1;
  if (stdForm.B == 0)
    return p.x == p1.x and p.y >= min(p1.y, p2.y) and p.y <= max(p1.y, p2.y);
  return p.x >= min(p1.x, p2.x) and p.x <= max(p1.x, p2.x) and stdForm.C == stdForm.A * p.x + stdForm.B * p.y;
}

double Line::distanceTo(Line oth) const
{
  if (intersect(oth))
    return 0;

  return 0.0;
}

double Line::norm2() const
{
  double xcomp = (p2.x - p1.x);
  double ycomp = (p2.y - p1.y);
  return xcomp * xcomp + ycomp * ycomp;
}

double Line::norm() const
{
  return sqrt(norm2());
}

bool Line::intersect(Line l2) const
{
  return Line::intersect(*this, l2);
}

// -1 outside, 1 inside, 0 on the edge
int Polygon::contains(Point p)
{
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
  for (int i = 0; i < n; i++)
  {
    Line edge(points[i], points[(i + 1) % n]);
    // test colinearity to know is point is on edge.
    int notColinear = Point::ccw(p, edge.p1, edge.p2);
    bool onSegment = (not notColinear) and Point::dot(p - edge.p1, p - edge.p2) <= 0; // dot < 0 means we're not going same way from p -> p1 as with p -> p2
    if (onSegment)
      return 0;
  }

  // thanks to the above we know point is NOT on edge, so we can save conditions
  // edge.miny <= p.y and p.y <= edge.maxy now uses strict comparison
  // this gives us nice property when computing 1/slope that the divider is not zero.
  for (int i = 0; i < n; i++)
  {
    Line edge(points[i], points[(i + 1) % n]); // (i-1)%n had weird behavior

    // hence p is in middle of segment
    // afterwards dont care about being onEdge, it has already been checked for
    if (((edge.p1.y < p.y) != (edge.p2.y < p.y) /*guarantees not division by 0, and edge intersects y=p.y (maybe on left side though)*/))
    {
      double xintersect = edge.p1.x + (p.y - edge.p1.y) * (edge.p1.x - edge.p2.x) / (edge.p1.y - edge.p2.y); // = divide by slope;
      // was using intersect method from the course at first, but in this case it proved unnecessary !
      if (p.x < xintersect /*check we will intersect by moving right*/)
        nintersect++;
    }
  }

  return (nintersect & 1) ? 1 : -1;
}

/**
 * @brief Implementation of the actual math to compute the signed area of a polygon.
 * It updates the polygon m_area member.
 * Done in O(n) time.
 * 
 */
void Polygon::recomputeArea()
{
  if (points.size() == 0)
  {
    m_area = 0;
    std::cout << "size is 0" << std::endl;
    return;
  }

  double prevx, prevy, initx, inity, x, y;
  prevx = initx = x = points[0].x;
  prevy = inity = y = points[0].y;
  double area = 0;

  // O(n) algorithm
  for (int i = 1; i < points.size(); i++)
  {
    // even though coords are integer use complete formula for sign
    x = points[i].x, y = points[i].y;
    area += (double)((x + prevx) * (y - prevy));
    prevx = x, prevy = y;
  }

  area += (double)((initx + x) * (inity - y)); // dont forget edge between last->first
  area /= 2;                                   // Obviously divide only at the end to reduce floating point errors.
  m_area = area;
  uptodate = true;
}

/**
 * @brief Compute the signed area of the polygon.
 * The area is actually remembered between calls, so as long as the polygon
 * is not modified, the area will remain the same, thus this function returns immediately.
 * Otherwise, compute the area in O(n) time.
 * 
 * @return double the signed area of this polygon.
 */
double Polygon::area()
{
  if (uptodate == false)
    recomputeArea();
  return m_area;
}

namespace scatter_impl
{
  double dist(Point a, Point b)
  {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
  }

  std::pair<Point, Point> resolveBand(scatter_t const &band, double mindist)
  {
    Point p1 = {-200000, -100000}, p2{100000, 200000};
    for (std::size_t i = 0; i < band.size(); i++)
    {
      for (int j = i + 1; j < 7; j++)
      {
        if (auto newd = dist(band[i], band[j]); newd < mindist)
        {
          mindist = newd;
          p1 = band[i];
          p2 = band[j];
        }
      }
    }
    return {p1, p2};
  }

  std::pair<Point, Point> naive_closest(scatter_t::iterator begx, scatter_t::iterator endx)
  {

    double mindistance = 1'000'000;
    Point p1, p2;
    for (auto i = begx; i != endx; i++)
      for (auto j = i + 1; j != endx; j++)
        if (auto newd = dist(*i, *j); newd < mindistance)
        {
          mindistance = newd;
          p1 = *i;
          p2 = *j;
        }
    return {p1, p2};
  }

  std::pair<Point, Point> closest_pair_impl(scatter_t::iterator begx, scatter_t::iterator endx, scatter_t::iterator begy, scatter_t::iterator endy)
  {
    // solve iteratively
    auto n = endx - begx;
    if (n <= 3)
      return naive_closest(begx, endx);

    auto midl = n / 2;
    auto midpoint = begx + midl;

    scatter_t lefty(midl);
    scatter_t righty(n - midl);
    int li = 0, ri = 0;
    for (auto i = begy; i != endy; i++)
    {
      if (i->x < midpoint->x and li < midl)
        lefty[li++] = *i;
      else if (ri < n - midl)
        righty[ri++] = *i;
      else
        lefty[li++] = *i;
    }

    // sorted by y, I partition them into those "to the left" and those "to the right".
    // number of test in the "copy_if" below thus gets halved from one recursive call to the other deeper.
    auto [pl1, pl2] = closest_pair_impl(begx, midpoint, lefty.begin(), lefty.end());
    auto [pr1, pr2] = closest_pair_impl(midpoint, endx, righty.begin(), righty.end());

    if (dist(pl1, pl2) < dist(pr1, pr2))
    {
      pr1 = pl1;
      pr2 = pl2;
    }
    const double mindistance = dist(pr1, pr2);

    // solve intersection
    scatter_t band;
    std::copy_if(begy, endy, std::back_inserter(band), [&](Point point)
                 { return std::abs(point.x - midpoint->x) < mindistance; });

    auto [b1, b2] = resolveBand(band, mindistance);
    if (dist(b1, b2) < dist(pr1, pr2))
      return {b1, b2};

    return {pr1, pr2};
  }
}

std::pair<Point, Point> closest_pair(scatter_t &points)
{
  std::sort(points.begin(), points.end());
  scatter_t pointsY{points};
  std::sort(pointsY.begin(), pointsY.end(), [](Point lhs, Point rhs)
            { return lhs.y < rhs.y or (lhs.y == rhs.y and lhs.x < rhs.x); }); // O(n log n)
  return scatter_impl::closest_pair_impl(points.begin(), points.end(), pointsY.begin(), pointsY.end());
}