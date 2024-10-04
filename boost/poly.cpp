#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <iostream>

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
  Polygon poly;
  bg::read_wkt("POLYGON((0 0,0 5,5 5,5 0,0 0))", poly);

  print_poly(poly);
    //std::cout << "Polygon coordinates: ";
    //bg::for_each_point(poly, print_point);
    //std::cout << std::endl;

  return 0;
}
