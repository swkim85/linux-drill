#include <gtest/gtest.h>
#include <iostream>
#include <random>
#include "sort.h"

TEST(TSuit1, Basic) {
  int data[] = {9,8,7,6,5,4,3,2,1,0};
  int size = sizeof(data) / sizeof(data[0]);
  EXPECT_FALSE( is_sorted(data, size) );
  heapSort(data, size);
  EXPECT_TRUE( is_sorted(data, size) );
}

TEST(TSuit2, Random) {
  int data[1000];
  int size = sizeof(data) / sizeof(data[0]);
  initialize_vector(data, size) ;
  heapSort(data, size);
  print_vec("random array", data, size);
  EXPECT_TRUE( is_sorted(data, size) );
}


