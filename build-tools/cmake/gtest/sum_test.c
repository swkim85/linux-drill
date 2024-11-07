#include <stdio.h>
#include <gtest/gtest.h>

#include "sum.h"

// TEST(TestSuiteName, TestName) {
//   ... test body ...
// }
TEST(sum_test1, test1) {
  EXPECT_EQ(2, sum(1,1));
  EXPECT_EQ(0, sum(1,-1));
}
TEST(sum_test2, test2) {
  EXPECT_EQ(100, sum(50,50));
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
