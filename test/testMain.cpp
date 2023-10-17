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

#include "../include/Translator.h"

namespace TranslationUnitTests {

// The fixture for testing class Foo.
class TranslatorTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  TranslatorTest() {
      translator = new Translator();
  }

  ~TranslatorTest() override {
     if(translator != nullptr)
         delete translator;
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
  Translator* translator;
};


///////////////////////////////////////////////////////////////////////////////
// Test case: TranslateHelloFromEnglishToSpanish
// 
// Description: Test case to translate "Hello" from English to Spanish.
//              The expected translation is "Hola".
///////////////////////////////////////////////////////////////////////////////
TEST_F(TranslatorTest, TranslateHelloFromEnglishToSpanish)
{
  std::string outputText;
  translator->doTranslation(outputText, "Hello", "en", "spa");
  std::cout << outputText << std::endl;

   // Find "Hola" in outputText
   EXPECT_NE(outputText.find("Hola"), std::string::npos);
}

///////////////////////////////////////////////////////////////////////////////
// Test case: DoesXyz
// 
// Description: Test case to do Xyz.
///////////////////////////////////////////////////////////////////////////////
TEST_F(TranslatorTest, DoesXyz)
{
  // Exercises the Xyz feature of Foo.
  EXPECT_NE(5,4);
}

}  // namespace project

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
