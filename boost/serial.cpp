#include <iostream>
#include <fstream>
#include <string>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/string.hpp>

class Person {
private:
  friend class boost::serialization::access;
  std::string name;
  int age;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & name;
    ar & age;
  }

public:
  Person() {}
  Person(const std::string& n, int a) : name(n), age(a) {}
  void display() const {
    std::cout << "Name: " << name << ", Age: " << age << std::endl;
  }
};

std::string trim(const std::string& str) {
  size_t first = str.find_first_not_of(" \t\n\r");
  if (std::string::npos == first) {
    return str;
  }
  size_t last = str.find_last_not_of(" \t\n\r");
  return str.substr(first, (last - first + 1));
}

int main() {
  std::string name;
  int age;

  std::cout << "Enter name: ";
  std::getline(std::cin, name);
  if (name != ".") {
    std::cout << "Enter age: ";
    std::cin >> age;

    // save object
    std::ofstream file("person.txt");
    boost::archive::text_oarchive  oa(file);
    name = trim(name); // 앞뒤의 공백을 제거
    Person p(name, age);
    oa << p;
  }

  // load object
  std::ifstream  file("person.txt");
  if (file.is_open()) {
    boost::archive::text_iarchive ia(file);
    Person loaded_p;
    ia >> loaded_p;
    std::cout << "Loaded person: ";
    loaded_p.display();
  }

  return 0;
}
