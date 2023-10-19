#ifndef INCLUDE_DATABASEHANDLER_H_
#define INCLUDE_DATABASEHANDLER_H_

#include <chrono>
#include <string>
#include <utility>

#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

#include "TranslationOutput.h"
#include "Translations.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

class DatabaseHandler {
 public:
        explicit DatabaseHandler(std::string configPath) ;

        // Get translation history
        std::pair<int, std::string> get_translation_history(std::string id);

        // Create a new user
        std::pair<int, std::string> create_user();

        // Delete
        std::pair<int, std::string> delete_user(std::string id);

        // Post translation to user
        std::pair<int, std::string> post_translation(std::string id, TranslationOutput newTranslation);

 private:
        // Create an instance. Note there must only be one instance
        mongocxx::instance inst{};

        // Database
        mongocxx::database db;

        mongocxx::uri uri;

        mongocxx::client client;
};

#endif  // INCLUDE_DATABASEHANDLER_H_
