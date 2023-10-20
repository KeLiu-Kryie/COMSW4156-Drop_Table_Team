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

///////////////////////////////////////////////////////////////////////////////
// Test case: TranslateHolaFromSpanishToEnglish
// 
// Description: Test case to translate "Hola" from Spanish to English.
//              The expected translation is "Hello".
///////////////////////////////////////////////////////////////////////////////
TEST_F(TranslatorTest, TranslateHolaFromSpanishToEnglish)
{
  sleep(1);
  std::string outputText;
  translator->doTranslation(outputText, "Hola", "spa", "en");

   // Find "Hello" in outputText
   EXPECT_NE(outputText.find("Hello"), std::string::npos);
}

///////////////////////////////////////////////////////////////////////////////
// Test case: TranslateHelloFromEnglishToChinese
// 
// Description: Test case to translate "Hello" from English to Chinese.
//              The expected translation is "\u4f60\u597d".
///////////////////////////////////////////////////////////////////////////////
TEST_F(TranslatorTest, TranslateHelloFromEnglishToChinese)
{
  sleep(1);
  std::string outputText;
  translator->doTranslation(outputText, "Hello", "en", "zh");
   // Find "你好" unicdoe in outputText
   EXPECT_NE(outputText.find("u4f60"), std::string::npos);
}

///////////////////////////////////////////////////////////////////////////////
// Test case: TranslateHolaFromSpanishToChinese
// 
// Description: Test case to translate "Hola" from Spanish to Chinese.
//              The expected translation is "\u4f60\u597d".
///////////////////////////////////////////////////////////////////////////////
TEST_F(TranslatorTest, TranslateHolaFromSpanishToChinese)
{
  sleep(1);
  std::string outputText;
  translator->doTranslation(outputText, "Hola", "spa", "zh");
   // Find "\u4f60\u597d" unicdoe in outputText
   EXPECT_NE(outputText.find("u4f60"), std::string::npos);
}

///////////////////////////////////////////////////////////////////////////////
// Test case: TranslateHelloFromEnglishToJapenese
// 
// Description: Test case to translate "Hello" from English to Japenese.
//              The expected translation is "\u4f60\u597d".
///////////////////////////////////////////////////////////////////////////////
TEST_F(TranslatorTest, TranslateHelloFromEnglishToJapenese)
{
  sleep(1);
  std::string outputText;
  translator->doTranslation(outputText, "Hello", "en", "jp");
   // Find "\u3053\u3093\u306b\u3061\u306f" unicdoe in outputText
   EXPECT_NE(outputText.find("3053"), std::string::npos);
}

///////////////////////////////////////////////////////////////////////////////
// Test case: TranslateColumbia UniversityFromEnglishToKorean
// 
// Description: Test case to translate "Columbia University" from English to Korean.
//              The expected translation is "\uceec\ub7fc\ube44\uc544 \ub300\ud559\uad50".
///////////////////////////////////////////////////////////////////////////////
TEST_F(TranslatorTest, TranslateColumbiaUniversityFromEnglishToKorean)
{
  sleep(1);
  std::string outputText;
  translator->doTranslation(outputText, "ColumbiaUniversity", "en", "kor");
   // Find "\uceec\ub7fc\ube44\uc544 \ub300\ud559\uad50" unicdoe in outputText
   EXPECT_NE(outputText.find("uceec"), std::string::npos);
}

///////////////////////////////////////////////////////////////////////////////
// Test case: Translate IngegneriadelsoftwareFromItalianToEnglish
// 
// Description: Test case to translate "Ingegneria del software" from Italian to English.
//              The expected translation is "Software engineering".
///////////////////////////////////////////////////////////////////////////////
TEST_F(TranslatorTest, TranslateIngegneriadelsoftwareFromItalianToEnglish)
{
  sleep(1);
  std::string outputText;
  translator->doTranslation(outputText, "Ingegneria del software", "it", "en");
   // Find "Software engineering" unicdoe in outputText
   EXPECT_NE(outputText.find("Software engineering"), std::string::npos);
}

///////////////////////////////////////////////////////////////////////////////
// Test case: Translate TranslateIchliebeSoftwareentwicklungFromGermanToEnglish
// 
// Description: Test case to translate "Ich liebe Softwareentwicklung" from German to English.
//              The expected translation is "I love software development".
///////////////////////////////////////////////////////////////////////////////
TEST_F(TranslatorTest, TranslateIchliebeSoftwareentwicklungFromGermanToEnglish)
{
  sleep(1);
  std::string outputText;
  translator->doTranslation(outputText, "Ich liebe Softwareentwicklung", "de", "en");
   // Find "I love software development" unicdoe in outputText
   EXPECT_NE(outputText.find("I love software development"), std::string::npos);
}

///////////////////////////////////////////////////////////////////////////////
// Test case: Translate TranslateIchliebeSoftwareentwicklungFromEnglishToCzech
// 
// Description: Test case to translate "I have an apple" from English to Czech.
//              The expected translation is "M\u00e1m jablko".
///////////////////////////////////////////////////////////////////////////////
TEST_F(TranslatorTest, TranslateIhaveanappleFromenToCzech)
{
  sleep(1);
  std::string outputText;
  translator->doTranslation(outputText, "I have an apple", "en", "cs");
   // Find "M\u00e1m jablko" unicdoe in outputText
   EXPECT_NE(outputText.find("jablko"), std::string::npos);
}

TEST_F(TranslatorTest, TranslateLoremIpsumToSpanish)
{
  std::string outputText;
  std::string data = file_to_text("test.pdf");
  translator->doTranslation(outputText, data, "en", "spa");

   // Find "espicie" in outputText
   EXPECT_NE(outputText.find("espicie"), std::string::npos);
}

TEST_F(TranslatorTest, NullPDFToSpanish)
{
  std::string outputText;
  std::string data = file_to_text("nonexistent.pdf");
  translator->doTranslation(outputText, data, "en", "spa");

  std::cerr << outputText << "\n";
   // Find "ERROR" in outputText
   EXPECT_NE(outputText.find("ERROR"), std::string::npos);
}

TEST_F(TranslatorTest, TranslateCedricToSpanish)
{
  std::string outputText;
  std::string data = ocr::optical_character_recognition("test.png", "eng");
  translator->doTranslation(outputText, data, "en", "spa");

   // Find "propio" in outputText
   EXPECT_NE(outputText.find("propio"), std::string::npos);
}

TEST_F(TranslatorTest, NullPNGToSpanish)
{
  std::string outputText;
  std::string data = ocr::optical_character_recognition("nonexistent.png", "eng");
  translator->doTranslation(outputText, data, "en", "spa");

   // Find "ERROR" in outputText
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
