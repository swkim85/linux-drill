#include <iostream>
#include <string>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

bool is_valid_phone_number(const std::string& phone_number) {
  boost::regex pattern("^\\d{3}-\\d{4}-\\d{4}$");
  return boost::regex_match(phone_number, pattern);
}

std::string remove_hyphens(const std::string& phone_number) {
  boost::regex pattern("-");
  return boost::regex_replace(phone_number, pattern, "");
}

std::vector<std::string> split_phone_number(const std::string& phone_number) {
  boost::regex pattern("(\\d+)-(\\d+)-(\\d+)");
  boost::smatch match;
  std::vector<std::string> parts;
  if (boost::regex_match(phone_number, match, pattern)) {
    for (size_t i = 1; i < match.size(); ++i) {
      parts.push_back(match[i].str());
    }
  }
  return parts;
}


int main() {
  std::string phone_number;
  std::cout << "전화번호를 입력하세요 (000-0000-0000 형식): ";
  std::cin >> phone_number;
  boost::trim(phone_number);

  if (is_valid_phone_number(phone_number)) {
    std::cout << "유효한 전화번호 형식입니다." << std::endl;
    std::cout << "하이픈이 제거된 번호: " << remove_hyphens(phone_number) << std::endl;
  } else {
    std::cout << "유효하지 않은 전화번호 형식입니다." << std::endl;
  }

  std::vector<std::string> parts;
  parts = split_phone_number(phone_number);
  int i = 0;
  for (const auto& part : parts) {
    std::cout << i << " : " << part << std::endl;
    i++;
  }

  return 0;
}

