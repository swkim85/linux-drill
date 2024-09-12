#include <iostream>

void leakyFunction() {
    int* ptr = new int;  // 동적으로 메모리 할당
    *ptr = 10;
    
    // delete ptr; 를 호출하지 않고 함수가 종료됨
}

int main() {
    for (int i = 0; i < 1000000; i++) {
        leakyFunction();
    }
    
    std::cout << "프로그램 종료" << std::endl;
    return 0;
}
