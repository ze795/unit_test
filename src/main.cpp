#include <iostream>
#include <gtest/gtest.h>
#include "addr_pri.h"
#include "stub_mock.h"
#include "stub.h"

using namespace std;

class curTest : public testing::Test {
 protected:
  void SetUp() override {
  }
  void TearDown() override {
    SMOCK_CLEAR;
  }
};

int func(int a) {
  cout << "test" << endl;
  return 0;
}

int func_stub(int a) {
  cout << "test_stub" << endl;
  return 1;
}

TEST_F(curTest, 001) {
  F_SMOCK(func, func_stub);
  EXPECT_EQ(func(1), 0);
  SMOCK_CLEAR;
}

TEST_F(curTest, 002) {
  F_SMOCK(func, func_stub);
  EXPECT_EQ(func(1), 1);
  SMOCK_CLEAR;
}

int main(int argc, char **argv) {
  printf("Running main() from %s\n", __FILE__);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
