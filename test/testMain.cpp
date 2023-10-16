///////////////////////////////////////////////////////////////////////////////
// testMain.cpp
//
//
//
// Sourced from: https://google.github.io/googletest/primer.html?fbclid=IwAR2zKNqpWxoU78kRUMEYyy49xkAZxu0-Zd5sjNc7qGCi3n3_Horj5r6xch0
//
///////////////////////////////////////////////////////////////////////////////
#include <iostream>

#include <gtest/gtest.h>

namespace my {
namespace project {
namespace {

// The fixture for testing class Foo.
class FooTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  FooTest() {
     // You can do set-up work for each test here.
  }

  ~FooTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Class members declared here can be used by all tests in the test suite
  // for Foo.
};

// Tests that the Foo::Bar() method does Abc.
TEST_F(FooTest, DoesAbc)
{
  EXPECT_EQ(1,1);
}

// Tests that Foo does Xyz.
TEST_F(FooTest, DoesXyz)
{
  // Exercises the Xyz feature of Foo.
  EXPECT_NE(5,4);
}

}  // namespace
}  // namespace project
}  // namespace my

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
