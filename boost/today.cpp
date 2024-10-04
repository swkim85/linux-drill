#include <iostream>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

int main() {
    // 현재 날짜 가져오기
    boost::gregorian::date today = boost::gregorian::day_clock::local_day();

    // 기본 형식으로 출력
    std::cout << "오늘 날짜 (기본 형식): " << today << std::endl;

    // ISO 확장 형식으로 출력
    std::cout << "오늘 날짜 (ISO 확장 형식): " << boost::gregorian::to_iso_extended_string(today) << std::endl;

    // 간단한 형식으로 출력
    std::cout << "오늘 날짜 (간단한 형식): " << boost::gregorian::to_simple_string(today) << std::endl;

    // 사용자 정의 형식으로 출력
    boost::gregorian::date_facet* facet = new boost::gregorian::date_facet("%Y년 %m월 %d일");
    //std::cout.imbue(std::locale(std::cout.getloc(), facet));
    std::cout << "오늘 날짜 (사용자 정의 형식): " << today << std::endl;

    // 현재 날짜와 시간 출력
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    std::cout << "현재 날짜와 시간: " << now << std::endl;

    return 0;
}
