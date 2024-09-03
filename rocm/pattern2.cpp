#include <iostream>
#include <random>
#include <chrono>

#define PATTERN_SIZE 5

#define DATA_SIZE    1024
//#define DATA_SIZE    1073741824 // 1GB
//#define DATA_SIZE    2147483648 // 2GB

void simple_find_pattern(char *data, int ds, char *pattern, int ps) {
  std::cout << "=== simple_find_pattern ===" << std::endl;
  unsigned long number_of_compare = 0;
  unsigned int find_count = 0;
  for (unsigned int i = 0; i <= ds - ps; i++) {
    bool flag = true;
    for (unsigned int j = 0; j < ps; j++) {
      number_of_compare++;
      if (data[i+j] != pattern[j]) {
        flag = false; break;
      }
    }
    if (flag) { 
      find_count++;
      //std::cout << " " << i;
    }
  }

  if (find_count) std::cout << "find count: " << find_count << std::endl;
  else std::cout << "pattern not found" << std::endl;

  std::cout << "total number_of_compare : " << number_of_compare << std::endl;
}

void kmp_find_pattern(char *data, int ds, char *pattern, int ps, int *table) {
  std::cout << "=== kmp_find_pattern ===" << std::endl;
  unsigned long number_of_compare = 0;
  unsigned int find_count = 0;
  unsigned int j = 0;
  for (unsigned int i = 0; i <= ds; i++) {

    while (1) {
      if (j > 0) number_of_compare++;
      if (j > 0 && data[i] != pattern[j]) {
        j = table[j - 1];
      } else break;
    }

    number_of_compare++;
    if (data[i] == pattern[j]) {
      if (j == ps - 1) {
        find_count++;
        //std::cout << " " << i - ps + 1;
        j = table[j];
      } else {
        j++;
      }
    }
  }

  if (find_count) std::cout << "find count: " << find_count << std::endl;
  else std::cout << "pattern not found" << std::endl;

  std::cout << "total number_of_compare : " << number_of_compare << std::endl;
}

void make_kmp_table(char *pattern, int ps, int *table) {
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


void print_vec_int(std::string title, int *data, int n) {
  if (title.length()) std::cout << title ;
  std::cout << " [ ";
  for (int i = 0; i < n; ++i) std::cout << data[i] << " ";
  std::cout << " ]" << std::endl;
}


void print_vec(std::string title, char *data, int n) {
  if (title.length()) std::cout << title ;
  std::cout << " [ ";
  for (int i = 0; i < n; ++i) std::cout << data[i] << "";
  std::cout << " ]" << std::endl;
}

void initialize_vector_worstcase_data(char *data, int n) {
  for (int i = 0; i < n; i++) {
         if (i % 4 == 0) data[i] = 'A';
    else if (i % 4 == 1) data[i] = 'A';
    else if (i % 4 == 2) data[i] = 'A';
    else if (i % 4 == 3) data[i] = 'B';
  }
}
void initialize_vector_worstcase_patt(char *patt, int n) {
  for (int i = 0; i < n; i++) {
    patt[i] = 'A';
  }
}

void initialize_vector_onevalue(char *data, int n) {
  for (int i = 0; i < n; i++) data[i] = 'A';
}
void initialize_vector_random(char *data, int n) {
  std::random_device  rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, 4);
  int x = (int)'A';
  //std::cout << " x " << x  << std::endl;
  for (int i = 0; i < n; i++) {
    int rand = dis(gen);
    data[i] = (char)(x+rand);
  }
}

//char data[DATA_SIZE];
char pattern[PATTERN_SIZE];
int  kmp_table[PATTERN_SIZE]; // failure function (partial match)

int main(void) {

  constexpr unsigned int ds = DATA_SIZE;
  constexpr unsigned int ps = PATTERN_SIZE;
  char *data = new char[ds];

  // random
  std::cout << "random initialize " << std::endl;
  initialize_vector_random(data, ds);
  initialize_vector_random(pattern, ps);

  // one value vector
  //std::cout << "onevalue initialize " << std::endl;
  //initialize_vector_onevalue(data, ds);
  //initialize_vector_onevalue(pattern, ps);

  // worstcase
  //std::cout << "worstcase initialize " << std::endl;
  //initialize_vector_worstcase_data(data, ds);
  //initialize_vector_worstcase_patt(pattern, ps);

  //print_vec("data", data, ds);
  std::cout << "data length : " << ds << std::endl;
  print_vec("pattern", pattern, ps);

  make_kmp_table(pattern, ps, kmp_table);
  print_vec_int("kmp_table", kmp_table, ps);

  auto start = std::chrono::high_resolution_clock::now();
  simple_find_pattern(data, ds, pattern, ps);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> t1 = end - start;
  std::cout << "elapsed time(s) : " << t1.count() << std::endl;

  start = std::chrono::high_resolution_clock::now();
  kmp_find_pattern(data, ds, pattern, ps, kmp_table);
  end = std::chrono::high_resolution_clock::now();
  t1 = end - start;
  std::cout << "elapsed time(s) : " << t1.count() << std::endl;
  
  delete[] data;
  return 0;
}
