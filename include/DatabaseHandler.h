
#include <chrono>

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
        DatabaseHandler();
//Get transaltion history
        std::pair<int, std::string> get_translation_history(std::string id);

//Post

        std::pair<int, std::string> create_user();

//Delete

        std::pair<int, std::string> delete_user(std::string id);

        std::pair<int, std::string> post_translation(std::string id, TranslationOutput newTranslation);

        void setupCollectionEndpoint(std::string uri_str, std::string dbName);

private:
  // Create an instance. Note there must only be one instance
        mongocxx::instance inst{};
  //Database
        mongocxx::database db;

        mongocxx::uri uri;

        mongocxx::client client;

};
