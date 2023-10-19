#ifndef DATABASEHANDLER_UNIT_TESTS_H
#define DATABASEHANDLER_UNIT_TESTS_H

///////////////////////////////////////////////////////////////////////////////
// DatabaseHandlerUnitTests
//
// Defines the unit tests for the DatabaseHandler class.
//
// To make a new unit test suite for a different class:
// 1. Copy this file to a new file named <ClassName>UnitTests.h
//  1a. Include this file in .gitignore
//  1b. Include this file in testMain.cpp
// 2. Replace all instances of "DatabaseHanlder" with "<ClassName>"
// 3. Update the TEST_F testcases with functionality specific to the class
///////////////////////////////////////////////////////////////////////////////
#include <chrono>
#include <cstdlib>
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <iostream>
#include <gtest/gtest.h>
#include <typeinfo>
#include <nlohmann/json.hpp>
#include <fstream>

#include "../include/DatabaseHandler.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

namespace DatabaseHandlerUnitTests {

DatabaseHandler dbHandler = DatabaseHandler("../config.json");

// The fixture for testing class Foo.
class DatabaseHandlerTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  DatabaseHandlerTest() {
      testTranslationOutput = TranslationOutput("sp", "en", "hello", "hola");
      nlohmann::json config;
      std::ifstream configFile("../config.json");
      config = nlohmann::json::parse(configFile);
      std::string connectionStr = config["ConnectionString"];
      std::string dbName = config["DBName"];
      testuri = mongocxx::uri(connectionStr);
      testclient = mongocxx::client(testuri);
      testdb = testclient[dbName];
  }

  ~DatabaseHandlerTest() override {
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

  mongocxx::database testdb;

  mongocxx::uri testuri;

  mongocxx::client testclient;

  TranslationOutput testTranslationOutput;
};

//TEST CREATE_USER()

///////////////////////////////////////////////////////////////////////////////
// Test case: CreateSingleUser
// 
// Description: Test case to create a single user in the 
// database and check if it exists
///////////////////////////////////////////////////////////////////////////////
TEST_F(DatabaseHandlerTest, CreateSingleUser)
{
  bsoncxx::document::view filter;
  auto collection = testdb["translations"];
  int count = collection.count_documents(filter);
  std::pair<int, std::string> res = dbHandler.create_user();
  //If response was 200 the check if it exists in db
  //otherwise, make sure nothing was added to db
  if(res.first == 200){
    auto doc = collection.find_one(make_document(kvp("_id", bsoncxx::oid(res.second))));
    EXPECT_TRUE(doc);
    int newCount = collection.count_documents(filter);
    //Check if count is only incremented by 1 
    EXPECT_EQ(newCount, count + 1);
    //Delete user once done
    collection.delete_one(make_document(kvp("_id", bsoncxx::oid(res.second))));
  } else {
    int newCount = collection.count_documents(filter);
    //Check if count is only incremented by 1 
    EXPECT_EQ(newCount, count);
  }
}


///////////////////////////////////////////////////////////////////////////////
// Test case: CreateMultipleUsers
// 
// Description: Test case to create multiple (2) users in the 
// database and check if they exist
///////////////////////////////////////////////////////////////////////////////
TEST_F(DatabaseHandlerTest, CreateMultipleUsers)
{
  bsoncxx::document::view filter;
  auto collection = testdb["translations"];
  int count = collection.count_documents(filter);
  std::pair<int, std::string> res1 = dbHandler.create_user();
  std::pair<int, std::string> res2 = dbHandler.create_user();
  //Based on the responses, get expected count
  //make sure it matches current db count 
  int updatedCount = count;
  if(res1.first == 200){
    updatedCount++;
  } 
  if(res2.first == 200){
    updatedCount++;
  } 
  int actualCount = collection.count_documents(filter);
  EXPECT_EQ(updatedCount, actualCount);
  //If response was 200 the check if document exists in db
  if(res1.first == 200){
    auto doc = collection.find_one(make_document(kvp("_id", bsoncxx::oid(res1.second))));
    EXPECT_TRUE(doc);
    collection.delete_one(make_document(kvp("_id", bsoncxx::oid(res1.second))));
  }
  if(res2.first == 200){
    auto doc = collection.find_one(make_document(kvp("_id", bsoncxx::oid(res2.second))));
    EXPECT_TRUE(doc);
    collection.delete_one(make_document(kvp("_id", bsoncxx::oid(res2.second))));
  }
}

//TEST DELETE_USER FUNCTION

///////////////////////////////////////////////////////////////////////////////
// Test case: DeleteSingleUser
// 
// Description: Test case to create a single user  
// then delete it to make sure they do not exist in db
///////////////////////////////////////////////////////////////////////////////
TEST_F(DatabaseHandlerTest, DeleteSingleUser)
{
  bsoncxx::document::view filter;
  //Create a new user
  auto collection = testdb["translations"];
  int count = collection.count_documents(filter);
  bsoncxx::document::value new_doc = make_document(kvp("translationData", " "));
  auto res = testdb["translations"].insert_one(std::move(new_doc));
  //Get user's oid
  bsoncxx::oid oid = res->inserted_id().get_oid().value;
  //make it a string
  std::string oidStr = oid.to_string();
  std::pair<int, std::string> resDelete = dbHandler.delete_user(oidStr);
  int newCount = collection.count_documents(filter);
  if(resDelete.first == 200){
    EXPECT_EQ(newCount, count);
    auto doc = collection.find_one(make_document(kvp("_id", bsoncxx::oid(oidStr))));
    EXPECT_TRUE(!(doc));
  } else {
    EXPECT_EQ(newCount, count + 1);
    collection.delete_one(make_document(kvp("_id", bsoncxx::oid(oidStr))));
  }
}

///////////////////////////////////////////////////////////////////////////////
// Test case: DeleteMultipleUsers
// 
// Description: Test case to create multiple users 
// then delete them to make sure they do not exist in db
///////////////////////////////////////////////////////////////////////////////
TEST_F(DatabaseHandlerTest, DeleteMultipleUsers)
{
  bsoncxx::document::view filter;
  //Create a new user
  auto collection = testdb["translations"];
  int count = collection.count_documents(filter);
  bsoncxx::document::value new_doc1 = make_document(kvp("translationData", " "));
  bsoncxx::document::value new_doc2 = make_document(kvp("translationData", " "));
  auto res1 = testdb["translations"].insert_one(std::move(new_doc1));
  auto res2 = testdb["translations"].insert_one(std::move(new_doc2));
  //Get user's oid
  bsoncxx::oid oid1 = res1->inserted_id().get_oid().value;
  bsoncxx::oid oid2 = res2->inserted_id().get_oid().value;
  //make it a string
  std::string oidStr1 = oid1.to_string();
  std::string oidStr2 = oid2.to_string();
  std::pair<int, std::string> resDelete1 = dbHandler.delete_user(oidStr1);
  int newCount = collection.count_documents(filter);
  if(resDelete1.first == 200){
    EXPECT_EQ(newCount, count + 1);
    auto doc = collection.find_one(make_document(kvp("_id", bsoncxx::oid(oidStr1))));
    EXPECT_TRUE(!(doc));
  } else {
    EXPECT_EQ(newCount, count + 2);
    collection.delete_one(make_document(kvp("_id", bsoncxx::oid(oidStr1))));
  }
  std::pair<int, std::string> resDelete2 = dbHandler.delete_user(oidStr2);
  newCount = collection.count_documents(filter);
  if(resDelete2.first == 200){
    EXPECT_EQ(newCount, count);
    auto doc = collection.find_one(make_document(kvp("_id", bsoncxx::oid(oidStr2))));
    EXPECT_TRUE(!(doc));
  } else {
    EXPECT_EQ(newCount, count + 1);
    collection.delete_one(make_document(kvp("_id", bsoncxx::oid(oidStr2))));
  }
}

///////////////////////////////////////////////////////////////////////////////
// Test case: DeleteNonExistantUser
// 
// Description: Test case to create a user then delete it after which
// delete function is called to see whether it returns correct output code
///////////////////////////////////////////////////////////////////////////////
TEST_F(DatabaseHandlerTest, DeleteNonExistantUser)
{
  bsoncxx::document::view filter;
  //Create a new user
  auto collection = testdb["translations"];
  int count = collection.count_documents(filter);
  bsoncxx::document::value new_doc = make_document(kvp("translationData", " "));
  auto res = testdb["translations"].insert_one(std::move(new_doc));
  //Get user's oid
  bsoncxx::oid oid = res->inserted_id().get_oid().value;
  //make it a string
  std::string oidStr = oid.to_string();
  collection.delete_one(make_document(kvp("_id", bsoncxx::oid(oidStr))));
  std::pair<int, std::string> resDelete = dbHandler.delete_user(oidStr);
  int newCount = collection.count_documents(filter);
  EXPECT_EQ(resDelete.first, 404);
  EXPECT_EQ(newCount, count);
}

//TEST POST_TRANSLATION FUNCTION


///////////////////////////////////////////////////////////////////////////////
// Test case: PostSingleTranslation
// 
// Description: Test case to post a single translation to an id
///////////////////////////////////////////////////////////////////////////////
TEST_F(DatabaseHandlerTest, PostSingleTranslation)
{
  //Create a new user
  auto collection = testdb["translations"];
  //Convert Translation to json
  Translations translations = Translations();
  Translations translationsExpected = Translations();
  nlohmann::json newData = translations;
  //Insert new data
  bsoncxx::document::value new_doc = make_document(kvp("translationData", newData.dump()));
  auto res = testdb["translations"].insert_one(std::move(new_doc));
  //Get user's oid
  bsoncxx::oid oid = res->inserted_id().get_oid().value;
  //make it a string
  std::string oidStr = oid.to_string();
  //Post a test translation to current id
  dbHandler.post_translation(oidStr, testTranslationOutput);
  translationsExpected.AddTranslation(testTranslationOutput);
  nlohmann::json expected = translationsExpected;
  auto doc = collection.find_one(make_document(kvp("_id", bsoncxx::oid(oidStr))));
  auto view = doc->view();
  auto element = view["translationData"];
  auto value = element.get_string().value;
  std::string actual = value.to_string();
  collection.delete_one(make_document(kvp("_id", oid)));
  EXPECT_EQ(actual, expected.dump());
}

///////////////////////////////////////////////////////////////////////////////
// Test case: PostLessThanTenTranslations
// 
// Description: Test case to multiple (but less than 10) translations
///////////////////////////////////////////////////////////////////////////////
TEST_F(DatabaseHandlerTest, PostLessThanTenTranslations)
{
  //Create a new user
  auto collection = testdb["translations"];
  //Convert Translation to json
  Translations translations = Translations();
  Translations translationsExpected = Translations();
  nlohmann::json newData = translations;
  //Insert new data
  bsoncxx::document::value new_doc = make_document(kvp("translationData", newData.dump()));
  auto res = testdb["translations"].insert_one(std::move(new_doc));
  //Get user's oid
  bsoncxx::oid oid = res->inserted_id().get_oid().value;
  //make it a string
  std::string oidStr = oid.to_string();
  //Post a test translation to current id
  for(int i = 0; i < 5; i++){
    dbHandler.post_translation(oidStr, testTranslationOutput);
    translationsExpected.AddTranslation(testTranslationOutput);
  }
  nlohmann::json expected = translationsExpected;
  auto doc = collection.find_one(make_document(kvp("_id", bsoncxx::oid(oidStr))));
  auto view = doc->view();
  auto element = view["translationData"];
  auto value = element.get_string().value;
  std::string actual = value.to_string();
  collection.delete_one(make_document(kvp("_id", oid)));
  EXPECT_EQ(actual, expected.dump());
}

///////////////////////////////////////////////////////////////////////////////
// Test case: PostTenTranslations
// 
// Description: Test case to multiple (10) translations to see if it 
// only stores a maximum of 10 translations
///////////////////////////////////////////////////////////////////////////////
TEST_F(DatabaseHandlerTest, PostTenTranslations)
{
  //Create a new user
  auto collection = testdb["translations"];
  //Convert Translation to json
  Translations translations = Translations();
  Translations translationsExpected = Translations();
  nlohmann::json newData = translations;
  //Insert new data
  bsoncxx::document::value new_doc = make_document(kvp("translationData", newData.dump()));
  auto res = testdb["translations"].insert_one(std::move(new_doc));
  //Get user's oid
  bsoncxx::oid oid = res->inserted_id().get_oid().value;
  //make it a string
  std::string oidStr = oid.to_string();
  //Post a test translation to current id
  for(int i = 0; i < 10; i++){
    dbHandler.post_translation(oidStr, testTranslationOutput);
    translationsExpected.AddTranslation(testTranslationOutput);
  }
  nlohmann::json expected = translationsExpected;
  auto doc = collection.find_one(make_document(kvp("_id", bsoncxx::oid(oidStr))));
  auto view = doc->view();
  auto element = view["translationData"];
  auto value = element.get_string().value;
  std::string actual = value.to_string();
  collection.delete_one(make_document(kvp("_id", oid)));
  EXPECT_EQ(actual, expected.dump());
}

///////////////////////////////////////////////////////////////////////////////
// Test case: PostMoreThanTenTranslations
// 
// Description: Test case to multiple (more than 10) translations to see if it 
// only stores a maximum of 10 translations
///////////////////////////////////////////////////////////////////////////////
TEST_F(DatabaseHandlerTest, PostMoreThanTenTranslations)
{
  //Create a new user
  auto collection = testdb["translations"];
  //Convert Translation to json
  Translations translations = Translations();
  Translations translationsExpected = Translations();
  nlohmann::json newData = translations;
  //Insert new data
  bsoncxx::document::value new_doc = make_document(kvp("translationData", newData.dump()));
  auto res = testdb["translations"].insert_one(std::move(new_doc));
  //Get user's oid
  bsoncxx::oid oid = res->inserted_id().get_oid().value;
  //make it a string
  std::string oidStr = oid.to_string();
  //Post a test translation to current id
  for(int i = 0; i < 11; i++){
    dbHandler.post_translation(oidStr, testTranslationOutput);
  } for(int i = 0; i < 10; i++){
    translationsExpected.AddTranslation(testTranslationOutput);
  }
  nlohmann::json expected = translationsExpected;
  auto doc = collection.find_one(make_document(kvp("_id", bsoncxx::oid(oidStr))));
  auto view = doc->view();
  auto element = view["translationData"];
  auto value = element.get_string().value;
  std::string actual = value.to_string();
  collection.delete_one(make_document(kvp("_id", oid)));
  EXPECT_EQ(actual, expected.dump());
}

///////////////////////////////////////////////////////////////////////////////
// Test case: PostTranslationInvalidId
// 
// Description: Test case to multiple (more than 10) translations to see if it 
// only stores a maximum of 10 translations
///////////////////////////////////////////////////////////////////////////////
TEST_F(DatabaseHandlerTest, PostTranslationInvalidId)
{
  //Create a new user
  auto collection = testdb["translations"];
  //Insert new data
  bsoncxx::document::value new_doc = make_document(kvp("translationData", " "));
  auto res = testdb["translations"].insert_one(std::move(new_doc));
  //Get new data oid
  bsoncxx::oid oid = res->inserted_id().get_oid().value;
  std::string oidStr = oid.to_string();
  //Delete it
  collection.delete_one(make_document(kvp("_id", oid)));
  //Check if post_new_translation returns 404
  std::pair<int, std::string> resPost = dbHandler.post_translation(oidStr, testTranslationOutput);
  EXPECT_EQ(404, resPost.first);
}


//TEST GET_TRANSLATION_HISTORY FUNCTION

///////////////////////////////////////////////////////////////////////////////
// Test case: GetEmptyData
// 
// Description: Test get translation history with an empty object (no translations)
///////////////////////////////////////////////////////////////////////////////
TEST_F(DatabaseHandlerTest, GetEmptyData)
{
  //Create a new user
  auto collection = testdb["translations"];
  //Convert Translation to json
  Translations translations = Translations();
  nlohmann::json newData = translations;
  //Insert new data
  bsoncxx::document::value new_doc = make_document(kvp("translationData", newData.dump()));
  auto res = testdb["translations"].insert_one(std::move(new_doc));
  //Get user's oid
  bsoncxx::oid oid = res->inserted_id().get_oid().value;
  //make it a string
  std::string oidStr = oid.to_string();
  std::pair<int, std::string> resGetHistory = dbHandler.get_translation_history(oidStr);
  nlohmann::json expected = translations;
  collection.delete_one(make_document(kvp("_id", oid)));
  EXPECT_EQ(resGetHistory.second, expected.dump());
}

///////////////////////////////////////////////////////////////////////////////
// Test case: GetDataLessThanTen
// 
// Description: Test get translation history for an object with less than 10
// translations
///////////////////////////////////////////////////////////////////////////////
TEST_F(DatabaseHandlerTest, GetDataLessThanTen)
{
  //Create a new user
  auto collection = testdb["translations"];
  //Convert Translation to json
  Translations translations = Translations();
  for(int i = 0; i < 5; i++){
    translations.AddTranslation(testTranslationOutput);
  }
  nlohmann::json newData = translations;
  //Insert new data
  bsoncxx::document::value new_doc = make_document(kvp("translationData", newData.dump()));
  auto res = testdb["translations"].insert_one(std::move(new_doc));
  //Get user's oid
  bsoncxx::oid oid = res->inserted_id().get_oid().value;
  //make it a string
  std::string oidStr = oid.to_string();
  std::pair<int, std::string> resGetHistory = dbHandler.get_translation_history(oidStr);
  nlohmann::json expected = translations;
  collection.delete_one(make_document(kvp("_id", oid)));
  EXPECT_EQ(resGetHistory.second, expected.dump());
}

///////////////////////////////////////////////////////////////////////////////
// Test case: GetDataTen
// 
// Description: Test get translation history for an object with 10
// translations
///////////////////////////////////////////////////////////////////////////////
TEST_F(DatabaseHandlerTest, GetDataTen)
{
  //Create a new user
  auto collection = testdb["translations"];
  //Convert Translation to json
  Translations translations = Translations();
  for(int i = 0; i < 10; i++){
    translations.AddTranslation(testTranslationOutput);
  }
  nlohmann::json newData = translations;
  //Insert new data
  bsoncxx::document::value new_doc = make_document(kvp("translationData", newData.dump()));
  auto res = testdb["translations"].insert_one(std::move(new_doc));
  //Get user's oid
  bsoncxx::oid oid = res->inserted_id().get_oid().value;
  //make it a string
  std::string oidStr = oid.to_string();
  std::pair<int, std::string> resGetHistory = dbHandler.get_translation_history(oidStr);
  nlohmann::json expected = translations;
  collection.delete_one(make_document(kvp("_id", oid)));
  EXPECT_EQ(resGetHistory.second, expected.dump());
}

///////////////////////////////////////////////////////////////////////////////
// Test case: GetDataMoreThanTen
// 
// Description: Test get translation history for an object with more than 10
// translations
///////////////////////////////////////////////////////////////////////////////
TEST_F(DatabaseHandlerTest, GetDataMoreThanTen)
{
  //Create a new user
  auto collection = testdb["translations"];
  //Convert Translation to json
  Translations translations = Translations();
  for(int i = 0; i < 12; i++){
    translations.AddTranslation(testTranslationOutput);
  }
  nlohmann::json newData = translations;
  //Insert new data
  bsoncxx::document::value new_doc = make_document(kvp("translationData", newData.dump()));
  auto res = testdb["translations"].insert_one(std::move(new_doc));
  //Get user's oid
  bsoncxx::oid oid = res->inserted_id().get_oid().value;
  //make it a string
  std::string oidStr = oid.to_string();
  std::pair<int, std::string> resGetHistory = dbHandler.get_translation_history(oidStr);
  nlohmann::json expected = translations;
  collection.delete_one(make_document(kvp("_id", oid)));
  EXPECT_EQ(resGetHistory.second, expected.dump());
}

///////////////////////////////////////////////////////////////////////////////
// Test case: GetDataInvalidId
// 
// Description: Test get translation history for an object id that does not exist
///////////////////////////////////////////////////////////////////////////////
TEST_F(DatabaseHandlerTest, GetDataInvalidId)
{
  //Create a new user
  auto collection = testdb["translations"];
  //Insert new data
  bsoncxx::document::value new_doc = make_document(kvp("translationData", " "));
  auto res = testdb["translations"].insert_one(std::move(new_doc));
  //Get user's oid
  bsoncxx::oid oid = res->inserted_id().get_oid().value;
  collection.delete_one(make_document(kvp("_id", oid)));
  //make it a string
  std::string oidStr = oid.to_string();
  std::pair<int, std::string> resGetHistory = dbHandler.get_translation_history(oidStr);
  EXPECT_EQ(resGetHistory.first, 404);
}


    
} 

#endif
