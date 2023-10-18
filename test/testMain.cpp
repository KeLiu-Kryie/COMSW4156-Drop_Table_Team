///////////////////////////////////////////////////////////////////////////////
// testMain.cpp
//
//
//
// Sourced from: https://google.github.io/googletest/primer.html?fbclid=IwAR2zKNqpWxoU78kRUMEYyy49xkAZxu0-Zd5sjNc7qGCi3n3_Horj5r6xch0
//
///////////////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>

#include "TranslationUnitTests.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
