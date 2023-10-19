#ifndef TRANSLATION_UNIT_TESTS_H
#define TRANSLATION_UNIT_TESTS_H

///////////////////////////////////////////////////////////////////////////////
// TranslationUnitTests
//
// Defines the unit tests for the Translator class.
//
// To make a new unit test suite for a different class:
// 1. Copy this file to a new file named <ClassName>UnitTests.h
//  1a. Include this file in .gitignore
//  1b. Include this file in testMain.cpp
// 2. Replace all instances of "Translator" with "<ClassName>"
// 3. Update the TEST_F testcases with functionality specific to the class
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <gtest/gtest.h>

#include "../include/Translator.h"
#include "../include/pdftotext.h"
#include "../include/ocr.h"

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

   // Find "Hola" in outputText
   EXPECT_NE(outputText.find("Hola"), std::string::npos);
}

TEST_F(TranslatorTest, TranslateLoremIpsumToSpanish)
{
  std::string outputText;
  std::string data = file_to_text("test.pdf");
  translator->doTranslation(outputText, data, "en", "spa");

   // Find "Hola" in outputText
   EXPECT_NE(outputText.find("el"), std::string::npos);
}

TEST_F(TranslatorTest, NullPDFToSpanish)
{
  std::string outputText;
  std::string data = file_to_text("nonexistent.pdf");
  translator->doTranslation(outputText, data, "en", "spa");

   // Find "Hola" in outputText
   EXPECT_NE(outputText.find("UNABLE"), std::string::npos);
}

TEST_F(TranslatorTest, TranslateCedricToSpanish)
{
  std::string outputText;
  std::string data = ocr::optical_character_recognition("test.png", "eng");
  translator->doTranslation(outputText, data, "en", "spa");

   // Find "Hola" in outputText
   EXPECT_NE(outputText.find("propio"), std::string::npos);
}

TEST_F(TranslatorTest, NullPNGToSpanish)
{
  std::string outputText;
  std::string data = ocr::optical_character_recognition("nonexistent.png", "eng");
  translator->doTranslation(outputText, data, "en", "spa");

   // Find "Hola" in outputText
   EXPECT_NE(outputText.find("ERROR"), std::string::npos);
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
    
} // namespace TranslationUnitTests

#endif // TRANSLATION_UNIT_TESTS_H
