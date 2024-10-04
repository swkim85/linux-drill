// vector.cpp
#include <iostream>
#include <ctime>
#include <fstream>
#include <typeinfo>
#include <deque>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <boost/random.hpp>
#include <boost/random/uniform_real_distribution.hpp>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/io/svg/svg_mapper.hpp>

#include <boost/foreach.hpp>

#define DIM 3
namespace ublas = boost::numeric::ublas;
namespace bg = boost::geometry;


ublas::vector<double> make_unit_vector() {
  ublas::vector<double> v (DIM);
  for (unsigned i = 0; i < v.size (); ++ i) {
    v(i) = 1;
  }
  return v;
}

ublas::vector<double> make_random_vector(double min, double max) {
  // 난수 생성기 설정
  boost::random::mt19937 gen(std::time(0));
  boost::random::uniform_real_distribution<> dist(min, max);

  // 벡터 생성 및 채우기
  ublas::vector<double> vec(DIM);
  for (int i = 0; i < DIM; ++i) {
    vec[i] = dist(gen);
  }
  return vec;
}

void print_vector(std::string title, ublas::vector<double> &v) {
  std::cout << title << v << std::endl;
}

int main () {
  typedef bg::model::d2::point_xy<double> point_t;
  typedef bg::model::polygon<point_t>  polygon_t;

  ublas::vector<double> v1(DIM), v2(DIM);
  ublas::vector<double> result(DIM);
  v1 = make_unit_vector();
  v2 = make_unit_vector();
  result = v1 + v2;
  print_vector("result", result);

  v1 = make_random_vector(-10, 10);
  print_vector("random vector", v1);

  polygon_t  polygon;
  bg::exterior_ring(polygon) = {
      {0, 0}, {0, 5}, {5, 5}, {5, 0}, {0, 0}
  };
  std::cout << "Type of polygon: " << typeid(polygon).name() << std::endl;


  // 다각형 생성
  polygon_t poly;
  bg::read_wkt("POLYGON((0 0,0 7,4 2,2 0,0 0))", poly);

  // 점 생성
  point_t p(1, 1);

  // 점이 다각형 내부에 있는지 확인
  bool within = bg::within(p, poly);
  std::cout << "Point is within polygon: " << (within ? "Yes" : "No") << std::endl;

  // 다각형 면적 계산
  double area = bg::area(poly);
  std::cout << "Polygon area: " << area << std::endl;

  // 두 다각형 생성
  polygon_t poly1, poly2;
  bg::read_wkt("POLYGON((0 0, 4 0, 6 3, 4 6, 0 6, 0 0))", poly1);
  bg::read_wkt("POLYGON((2 1, 6 1, 6 7, 2 7, 2 1))", poly2);

/*
  std::ofstream  svg("polygon.svg");
  bg::svg_mapper<point_t> mapper(svg, 100, 100);
  mapper.add(poly2);
  mapper.map(poly2, "fill-opacity:0.5;fill:rgb(153,204,0);stroke:rgb(153,204,0);stroke-width:2");
*/

  // 교차 영역 계산
  //std::vector<polygon_t> intersection;
  std::deque<polygon_t> intersection;
  bg::intersection(poly1, poly2, intersection);
  int i = 0;
  std::cout << "intersection.size: " << intersection.size() << std::endl;
  BOOST_FOREACH(polygon_t const& p, intersection) {
    std::cout << i++ << ": " << boost::geometry::area(p) << std::endl;
  }


/*
  std::ofstream  svg("inter.svg");
  bg::svg_mapper<point_t> mapper(svg, 100, 100);
  for (const auto& p: intersection) {
    std::cout << "Type of p: " << typeid(p).name() << std::endl;
    mapper.add(p);
    mapper.map(p , "fill-opacity:0.5;fill:rgb(153,204,0);stroke:rgb(153,204,0);stroke-width:1");
  }
*/

}


