// ptr1.cpp
#include <iostream>
#include <memory>

class A {
  std::string name;
  int *data;

 public:
  A(std::string name) {
    this->name = name;
    data = new int[100];
    std::cout << "자원을 획득함! name : " << this->name << std::endl;
  }

  ~A() {
    std::cout << "소멸자 호출! name : " << this->name << std::endl;
    delete[] data;
  }
};

int main() {
  A *pa = new A("pa");

  // 할당된 객체가 소멸되지 않음!
  // 즉, 400 바이트 (4 * 100) 만큼의 메모리 누수 발생

  std::unique_ptr<A> pb(new A("pb"));
}
