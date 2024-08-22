
#include <algorithm>
#include <stdexcept>
#include <initializer_list>
#include <iostream>

template <typename T>
class Vector {
private:
    T* data;
    size_t capacity;
    size_t size;

    void realloc(size_t newCapacity) {
        T* newData = new T[newCapacity];
        size_t newSize = std::min(size, newCapacity);
        
        for (size_t i = 0; i < newSize; i++) {
            newData[i] = std::move(data[i]);
        }

        delete[] data;

    std::cout << " Vector realloc()" 
              << "   newCapacity: " << newCapacity
              << "   newSize: " << newSize
              << std::endl;

        data = newData;
        capacity = newCapacity;
        size = newSize;
    }

public:
    Vector() : data(nullptr), capacity(0), size(0) {}

    Vector(std::initializer_list<T> init) : Vector() {
        reserve(init.size());
        for (const auto& item : init) {
            push_back(item);
        }
    }

    ~Vector() {
        delete[] data;
    }

    T* begin() {
        return data;
    }

    const T* begin() const {
        return data;
    }

    T* end() {
        return data + size;
    }

    const T* end() const {
        return data + size;
    }

    void push_back(const T& value) {
        if (size >= capacity) {
            realloc(capacity == 0 ? 1 : capacity * 2);
        }
        data[size++] = value;
    }

    void pop_back() {
        if (size > 0) {
            size--;
        }
    }

    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    T& at(size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    size_t getSize() const { return size; }
    size_t getCapacity() const { return capacity; }

    void reserve(size_t newCapacity) {
        if (newCapacity > capacity) {
            realloc(newCapacity);
        }
    }

    void resize(size_t newSize) {
        if (newSize > capacity) {
            realloc(newSize);
        }
        size = newSize;
    }

    void clear() {
        size = 0;
    }

    bool empty() const {
        return size == 0;
    }
};


//#include "ocs_vector.h" // 위에서 정의한 vector 클래스가 있는 헤더 파일

int main() {
    // vector 생성 및 초기화
    Vector<int> vec = {1, 2, 3, 4, 5};

    // 요소 추가
    vec.push_back(6);
    vec.push_back(7);

    // 크기 출력
    std::cout << "Vector size: " << vec.getSize() << std::endl;
    std::cout << "Vector capacity: " << vec.getCapacity() << std::endl;

    // 반복자를 사용한 요소 출력
    std::cout << "Vector elements: ";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 인덱스를 사용한 요소 접근
    std::cout << "Third element: " << vec[2] << std::endl;

    std::cout << "벡터 복사 " << std::endl;
    Vector<int> vec2 = vec;

    std::cout << "벡터 이동 " << std::endl;
    Vector<int> vec3 = std::move(vec);

/*
    std::cout << "벡터 내용 출력 " << std::endl;
    std::cout << "vec3 elements: ";
    for (const auto& elem : vec3) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    std::cout << "벡터 내용 출력 " << std::endl;
*/

    //std::cout << "해제 " << std::endl;
    //delete vec;
    //delete vec2;
    //delete vec3;

    return 0;
}
