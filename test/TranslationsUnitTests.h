#ifndef TRANSLATIONS_UNIT_TESTS_H
#define TRANSLATIONS_UNIT_TESTS_H

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
#include <nlohmann/json.hpp>

#include "../include/Translations.h"

namespace TranslationsUnitTests {

// The fixture for testing class Foo.
class TranslationsTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  TranslationsTest () {
    testTranslationOutput = TranslationOutput("sp", "en", "hello", "hola");
  }

  ~TranslationsTest () override {
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
  }

  void TearDown() override {
    expectedVector.clear();
  }

  TranslationOutput testTranslationOutput;
  std::vector<TranslationOutput> expectedVector;

};

// ADD_TRANSLATION TESTS

///////////////////////////////////////////////////////////////////////////////
// Test case: AddOneTranslation
// 
// Description: Test case to add one translation data object to a translations
// object.
///////////////////////////////////////////////////////////////////////////////
TEST_F(TranslationsTest , AddOneTranslation)
{
  Translations translations = Translations();
  translations.AddTranslation(testTranslationOutput);
  expectedVector.push_back(testTranslationOutput);
  nlohmann::json jsonObj = translations;
  nlohmann::json jsonExpected = expectedVector;
  EXPECT_EQ(jsonObj["translationData"].dump(), jsonExpected.dump());
}

///////////////////////////////////////////////////////////////////////////////
// Test case: AddLessThanTenTranslations
// 
// Description: Test case to add less than 10 translations
///////////////////////////////////////////////////////////////////////////////
TEST_F(TranslationsTest , AddLessThanTenTranslations)
{
  Translations translations = Translations();
  for(int i = 0; i < 5; i++){
    translations.AddTranslation(testTranslationOutput);
    expectedVector.push_back(testTranslationOutput);
  }
  nlohmann::json jsonObj = translations;
  nlohmann::json jsonExpected = expectedVector;
  EXPECT_EQ(jsonObj["translationData"].dump(), jsonExpected.dump());
}

///////////////////////////////////////////////////////////////////////////////
// Test case: AddTenTranslations
// 
// Description: Test case to add 10 translations
///////////////////////////////////////////////////////////////////////////////
TEST_F(TranslationsTest , AddTenTranslations)
{
  Translations translations = Translations();
  for(int i = 0; i < 10; i++){
    translations.AddTranslation(testTranslationOutput);
    expectedVector.push_back(testTranslationOutput);
  }
  nlohmann::json jsonObj = translations;
  nlohmann::json jsonExpected = expectedVector;
  EXPECT_EQ(jsonObj["translationData"].dump(), jsonExpected.dump());
}

///////////////////////////////////////////////////////////////////////////////
// Test case: AddMoreThanTenTranslations
// 
// Description: Test case to add more than 10 translations
// the final translations object must have 10 translations (since it is expected
// to store only the 10 most recent translations)
///////////////////////////////////////////////////////////////////////////////
TEST_F(TranslationsTest , AddMoreThanTenTranslations)
{
  Translations translations = Translations();
  for(int i = 0; i < 12; i++){
    translations.AddTranslation(testTranslationOutput);
  }
  for (int i = 0; i < 10; i++){
    expectedVector.push_back(testTranslationOutput);
  }
  nlohmann::json jsonObj = translations;
  nlohmann::json jsonExpected = expectedVector;
  EXPECT_EQ(jsonObj["translationData"].dump(), jsonExpected.dump());
}

} // namespace TranslationsUnitTests

#endif // TRANSLATION_UNIT_TESTS_H
