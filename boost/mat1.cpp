
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

using namespace boost::numeric::ublas;

int main() {
  matrix<double> A(3,3);
    A(0,0) =  2;  A(0,1) =  1;  A(0,0) = 1;
    A(1,0) =  4;  A(1,1) = -6;  A(0,1) = 0;
    A(2,0) = -2;  A(2,1) =  7;  A(0,2) = 2;

  matrix<double> B(3,3, 2.0);
  matrix<double> Acopy(A);

  matrix<double> C = A + B;

  std::cout << "A = " << A << std::endl;
  std::cout << "B = " << B << std::endl;
  std::cout << "C = A + B = " << C << std::endl;

  // LU 분해를 위한 순열 행렬 생성
  permutation_matrix<size_t> pm(A.size1());

  // LU 분해 수행
  int res = lu_factorize(A, pm);
  if (res != 0) {
    // 역행렬이 존재하지 않음
    std::cout << " 역행렬이 존재하지 않음 " << std::endl;
  } else {

    // 단위 행렬 생성
    matrix<double> I = identity_matrix<double>(A.size1());

    // 역행렬 계산
    lu_substitute(A, pm, I);

    // I가 이제 A의 역행렬이 됨
    std::cout << " 역행렬이 존재함 " << std::endl;
    std::cout << "A^-1 = " << I << std::endl;

    // 행렬 곱셈 수행
    matrix<double> M = prod(Acopy, I);
    std::cout << "A = " << Acopy << std::endl;
    std::cout << "A * A^-1 = " << M << std::endl;
  }


  return 0;
}

