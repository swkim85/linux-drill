#include <iostream>
#include <vector>
#include <algorithm>

void print_vec(std::vector<int> vec) {
  size_t size = vec.size();      // 벡터의 크기
  for (int i = 0; i < size; i++) {
    std::cout << vec[i] << " ";
  }
  std::cout << std::endl;
}

int main() {

  std::vector<int> vec1;                // 빈 벡터
  std::vector<int> vec2(5);             // 크기가 5이고 모든 요소가 0인 벡터
  std::vector<int> vec3(5, 2);          // 크기가 5이고 모든 요소가 2인 벡터
  std::vector<int> vec4 = {1, 2, 3, 4}; // 초기화 리스트 사용
  std::vector<int> vec5(vec4);  // 복사 생성자


  vec5.push_back(10);    // 끝에 요소 추가
  vec5.emplace_back(8);   // 끝에 요소 추가 (더 효율적)

  // 반복자 사용
  for (auto it = vec5.begin(); it != vec5.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;

  // 범위 기반 for 루프
  for (const auto& elem : vec5) {
    std::cout << elem << " ";
  }
  std::cout << std::endl;

  // 정렬
  //#include <algorithm> 
  std::sort(vec5.begin(), vec5.end());

  std::cout << "vec5:" << std::endl;
  print_vec(vec5);

  vec5.resize(10);    // 크기 조정
  //vec1.reserve(20);   // 용량 예약

  std::cout << "vec5:" << std::endl;
  print_vec(vec5);

  vec5.pop_back();    // 마지막 요소 제거
  //vec5.clear();       // 모든 요소 제거

  std::cout << "vec5:" << std::endl;
  print_vec(vec5);

  return 0;
}
