
#include <iostream>
#include <typeinfo>

class Base {
public:
    virtual ~Base() {}
};

class Derived : public Base {};

int main() {
    Base* b1 = new Base();
    Base* b2 = new Derived();
    
    std::cout << typeid(*b1).name() << std::endl;
    std::cout << typeid(*b2).name() << std::endl;

    if (typeid(*b1) == typeid(Derived)) {
      std::cout << "b1 is Derived type" << std::endl;
    }    
    if (typeid(*b2) == typeid(Derived)) {
      std::cout << "b2 is Derived type" << std::endl;
    }    

    delete b1;
    delete b2;
    return 0;
}

