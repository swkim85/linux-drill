// step2.ex1
/*
#include "mysqrt.h"
namespace mathfunctions {
double sqrt(double x) {
  return detail::mysqrt(x);      // mathfunctions::sqrt 에서는 detail::mysqrt 를 호출
}
}
*/

// step2.ex2
#include "MathFunctions.h"

#ifdef USE_MYMATH
#include "mysqrt.h"
#else
#include <cmath>
#endif

namespace mathfunctions {
double sqrt(double x) {
#ifdef USE_MYMATH
  return detail::mysqrt(x);
#else
  return std::sqrt(x);
#endif

}
}
