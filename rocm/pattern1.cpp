#include <iostream>
#include <random>
#include <chrono>

#define MAX 4

#define DATA_SIZE    1024000
#define PATTERN_SIZE 5

int simple_find_pattern(int *data, int ds, int *pattern, int ps) {
  for (int i = 0; i <= ds - ps; i++) {
    bool flag = true;
    for (int j = 0; j < ps; j++) {
      if (data[i+j] != pattern[j]) {
        flag = false;
        break;
      }
    }
    if (flag) return i;
  }
  return -1; // not found
}

void make_kmp_table(int *pattern, int ps, int *table) {
  for (int i = 0; i < ps; i++) table[i] = 0; // 0으로 초기화
  int j = 0; 
  for (int i = 1; i < ps; i++) {
    while (j > 0 && pattern[i] != pattern[j]) {
      j = table[j - 1];
    }
    if (pattern[i] == pattern[j]) {
      table[i] = ++j;
    }
  }
}

int kmp_find_pattern(int *data, int ds, int *pattern, int ps, int *table) {
  int j = 0;
  for (int i = 0; i <= ds; i++) {
    while (j > 0 && data[i] != pattern[j]) {
      j = table[j - 1];
    }
    if (data[i] == pattern[j]) {
      if (j == ps - 1) {
        return i - ps + 1;
        j = table[j];
      } else {
        j++;
      }
    }
  }
  return -1;
}


void print_vec(std::string title, int *data, int n) {
  if (title.length()) std::cout << title ;
  std::cout << " [ ";
  for (int i = 0; i < n; ++i) std::cout << data[i] << " ";
  std::cout << " ]" << std::endl;
}

void initialize_vector_random(int *data, int n) {
  std::random_device  rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, MAX);
  for (int i = 0; i < n; i++) {
    data[i] = dis(gen);
  }
}
void initialize_vector_incremental(int *data, int n, int start) {
  for (int i = 0; i < n; ++i) {
    data[i] = start + i;
  }
}

int data[DATA_SIZE];
int pattern[PATTERN_SIZE];
int kmp_table[PATTERN_SIZE]; // failure function (partial match)

int main(void) {

  constexpr int ds = DATA_SIZE;
  constexpr int ps = PATTERN_SIZE;
  initialize_vector_random(data, ds);
  initialize_vector_random(pattern, ps);
  //initialize_vector_incremental(data, ds, 0);
  //initialize_vector_incremental(pattern, ps, ds-ps-1);
  print_vec("data", data, ds);
  print_vec("pattern", pattern, ps);

  auto start = std::chrono::high_resolution_clock::now();
  int result = simple_find_pattern(data, ds, pattern, ps);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> t1 = end - start;
  std::cout << "elapsed time(s) simple_find_pattern : " << t1.count() << std::endl;
  if (result == -1) {
    printf("not found.\n");
  } else {
    printf("Found at index = %d \n", result+1);
  }

  make_kmp_table(pattern, ps, kmp_table);
  print_vec("kmp_table", kmp_table, ps);

  start = std::chrono::high_resolution_clock::now();
  result = kmp_find_pattern(data, ds, pattern, ps, kmp_table);
  end = std::chrono::high_resolution_clock::now();
  t1 = end - start;
  std::cout << "elapsed time(s) kmp_find_pattern : " << t1.count() << std::endl;
  if (result == -1) {
    printf("not found.\n");
  } else {
    printf("Found at index = %d \n", result+1);
  }
  
  return 0;
}
