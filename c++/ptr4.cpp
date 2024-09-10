#include <iostream>
#include <memory>

class A {
  std::string name;
  int *data;

public:
  A() {
    std::cout << "자원을 획득함!" << std::endl;
    data = new int[100];
  }
  void set_name(std::string name) {
    this->name = name;
  }
  std::string get_name() {
    return this->name;
  }

  void hello() {
    std::cout << "hello! name : " << this->name << std::endl;
  }

  void goodbye() {
    std::cout << "goodbye! name : " << this->name << std::endl;
  }

  void do_sth(int a) {
    std::cout << "무언가를 한다!" << std::endl;
    data[0] = a;
  }

  ~A() {
    std::cout << "자원을 해제함!" << std::endl;
    delete[] data;
  }
};


// 이렇게 하는 것은 올바르지 않다.
// unique_ptr 은 소유권을 의미한다는 원칙에 위배되므로
void say_hello(std::string name, std::unique_ptr<A>& ptr) {
  std::string oldname = ptr->get_name();

  ptr->set_name(name);
  ptr->hello();

  ptr->set_name(oldname);
}

void say_goodbye(A* ptr) {
  ptr->goodbye();
}

void do_something() {
  std::unique_ptr<A> pa(new A());
  pa->set_name("bob");
  pa->hello();

  // pb 도 객체를 가리키게 할 수 있을까?
  //std::unique_ptr<A> pb = pa; // error
  // error: use of deleted function ‘std::unique_ptr<_Tp, _Dp>::unique_ptr(const std::unique_ptr<_Tp, _Dp>&) [with _Tp = A; _Dp = std::default_delete<A>]’
  // 27 |   std::unique_ptr<A> pb = pa;

  // pb 에 소유권을 이전.
  std::unique_ptr<A> pb = std::move(pa);
  pb->set_name("alice");
  pb->hello();

  //pa->hello(); // segmentation fault

  say_hello("james", pb);
  pb->hello();

  say_goodbye(pb.get());

}

int main() {
  do_something();
}

