#include <iostream>
#include <deque>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include <boost/foreach.hpp>

namespace bg = boost::geometry;
typedef bg::model::d2::point_xy<double> Point;
typedef bg::model::polygon<Point> Polygon;

void print_point(const Point& p) {
  std::cout << "(" << bg::get<0>(p) << ", " << bg::get<1>(p) << ") ";
}
void print_poly(const Polygon& poly) {
  std::cout << "Polygon coordinates: ";
  bg::for_each_point(poly, print_point);
  std::cout << std::endl;
}


int main() {
  typedef boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double> > polygon;

  polygon green, blue;
  boost::geometry::read_wkt(
      "POLYGON((2 1.3,2.4 1.7,2.8 1.8,3.4 1.2,3.7 1.6,3.4 2,4.1 3,5.3 2.6,5.4 1.2,4.9 0.8,2.9 0.7,2 1.3)"
          "(4.0 2.0, 4.2 1.4, 4.8 1.9, 4.4 2.2, 4.0 2.0))", green);
  boost::geometry::read_wkt(
      "POLYGON((4.0 -0.5 , 3.5 1.0 , 2.0 1.5 , 3.5 2.0 , 4.0 3.5 , 4.5 2.0 , 6.0 1.5 , 4.5 1.0 , 4.0 -0.5))", blue);

  print_poly(green);
  print_poly(blue);

// desmos format
// polygon((2,1.3),(2.4,1.7),(2.8,1.8),(3.4,1.2),(3.7,1.6),(3.4,2),(4.1,3),(5.3,2.6),(5.4,1.2),(4.9,0.8),(2.9,0.7),(2,1.3))
// polygon((4.0,2.0),(4.2,1.4),(4.8,1.9),(4.4,2.2),(4.0,2.0))
// polygon((4.0,-0.5),(3.5,1.0),(2.0,1.5),(3.5,2.0),(4.0,3.5),(4.5,2.0),(6.0,1.5),(4.5,1.0),(4.0,-0.5))
// polygon((0,0),(4,0),(6,3),(4,6),(0,6),(0,0))
// polygon((2,1),(6,1),(6,7),(2,7),(2,1))
// polygon((2,6),(4,6),(6,3),(4.66667,1),(2,1),(2,6))

  polygon poly1, poly2;
  bg::read_wkt("POLYGON((0 0, 0 6, 4 6, 6 3, 4 0, 0 0))", poly1); // clockwise
  bg::read_wkt("POLYGON((2 1, 2 7, 6 7, 7 1, 2 1))", poly2); // clockwise

  std::deque<polygon> output;
  //boost::geometry::intersection(green, blue, output);
  boost::geometry::intersection(poly1, poly2, output);

  std::cout << "output.size(): " << output.size() << std::endl;

  int i = 0;
  std::cout << "intersections:" << std::endl;
  BOOST_FOREACH(polygon const& p, output) {
    print_poly(p);
    std::cout << i++ << ": " << boost::geometry::area(p) << std::endl;
  }

  return 0;
}
