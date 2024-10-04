
#include <boost/tokenizer.hpp>
#include <string>
#include <iostream>

int main() {
  int i;

  std::string s = "Boost C++ Libraries";
  boost::tokenizer<> tok(s);
  std::cout << "s: " << s << '\n';
  i = 0;
  for (const auto& t : tok) {
    std::cout << i++ << " " << t << '\n';
  }

  std::string s2 = "Boost,C++,Libraries";
  //boost::char_separator<char> sep(",", " ", boost::keep_empty_tokens);
  boost::char_separator<char> sep(",");
  boost::tokenizer<boost::char_separator<char>> tok2(s2, sep);
  
  std::cout << "s2: " << s2 << '\n';
  for (const auto& t : tok2) {
    std::cout << i++ << " " << t << '\n';
  }
}
