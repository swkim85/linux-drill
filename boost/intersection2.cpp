#include <iostream>
#include <deque>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include <boost/foreach.hpp>

namespace bg = boost::geometry;
typedef bg::model::d2::point_xy<double> Point;
typedef bg::model::polygon<Point> Polygon;
typedef boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double> > polygon;

void print_point(const Point& p) {
  std::cout << "(" << bg::get<0>(p) << ", " << bg::get<1>(p) << ") ";
}
void print_poly(const Polygon& poly) {
  std::cout << "Polygon coordinates: ";
  bg::for_each_point(poly, print_point);
  std::cout << std::endl;
}


int main() {

  Polygon poly1, poly2;
  bg::read_wkt("POLYGON((0 0, 0 6, 4 6, 6 3, 4 0, 0 0))", poly1); // clockwise
  bg::read_wkt("POLYGON((2 1, 2 7, 6 7, 7 1, 2 1))", poly2); // clockwise
  print_poly(poly1);
  print_poly(poly2);

  std::deque<Polygon> output;
  boost::geometry::intersection(poly1, poly2, output);
  std::cout << "output.size(): " << output.size() << std::endl;

  int i = 0;
  std::cout << "intersections:" << std::endl;
  BOOST_FOREACH(Polygon const& p, output) {
    print_poly(p);
    std::cout << i++ << ": " << boost::geometry::area(p) << std::endl;
  }

  return 0;
}
